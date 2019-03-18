#include "loader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <deque>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "debug.h"


std::unordered_map<std::string, Texture> loaded_textures {};


TexturedModel LoadModel(const std::string& path);
TexturedModel ProcessNode(const aiScene* scene, const std::string& directory);
Mesh ProcessMesh(aiMesh* mesh);
std::vector<Texture> ProcessMaterials(aiMaterial* material, const std::string& directory);
std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name, const std::string& directory);
unsigned int TextureFromFile(std::string path, bool gamma = false);


TexturedModel LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "[Assimp Error]: " << importer.GetErrorString();
        return {};
    }

    std::string directory = path.substr(0, path.find_last_of('/'));

    return ProcessNode(scene, directory);
}


TexturedModel ProcessNode(const aiScene* scene, const std::string& directory)
{
    std::vector<TexturedMesh> meshes;
    std::deque<aiNode*> queue {scene->mRootNode};

    while (!queue.empty())
    {
        aiNode* node = queue.front();
        queue.pop_front();

        for (unsigned int j = 0; j < node->mNumMeshes; j++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[j]];
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            Mesh x = ProcessMesh(mesh);
            std::vector<Texture> y = ProcessMaterials(material, directory);

            meshes.push_back({x, y});
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
            queue.push_back(node->mChildren[i]);
    }

    return {meshes};
}

Mesh ProcessMesh(aiMesh* mesh)
{
    // data to fill
    std::vector<Vertex>   vertices;
    std::vector<unsigned> indices;

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex {};

        // positions
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        // normals
        if (mesh->mNormals)
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.texture_coordinate.x = mesh->mTextureCoords[0][i].x;
            vertex.texture_coordinate.y = mesh->mTextureCoords[0][i].y;
        }

        // tangent
        if (mesh->mTangents)
        {
            vertex.tangent.x = mesh->mTangents[i].x;
            vertex.tangent.y = mesh->mTangents[i].y;
            vertex.tangent.z = mesh->mTangents[i].z;
        }

        // bitangent
        if (mesh->mBitangents)
        {
            vertex.bitangent.x = mesh->mBitangents[i].x;
            vertex.bitangent.y = mesh->mBitangents[i].y;
            vertex.bitangent.z = mesh->mBitangents[i].z;
        }

        vertices.push_back(vertex);
    }

    // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return IndexedModel(vertices, indices);
}


std::vector<Texture> ProcessMaterials(aiMaterial* material, const std::string& directory)
{
    std::vector<Texture> textures;

    // process materials
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    // 2. specular maps
    std::vector<Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    // 3. normal maps
    std::vector<Texture> normal_maps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", directory);
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
    // 4. height maps
    std::vector<Texture> height_maps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", directory);
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    return textures;
}


// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name, const std::string& directory)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString ai_path;
        mat->GetTexture(type, i, &ai_path);
        std::string path = ai_path.C_Str();

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        auto it = loaded_textures.find(path);
        if (it == loaded_textures.end())
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(directory + "/" + path);  // TODO(ted): Make cross-platform compatible
            texture.type = type_name;
            textures.push_back(texture);

            // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            loaded_textures[path] = texture;
        }
        else
        {
            textures.push_back(it->second);
        }
    }

    return textures;
}


unsigned int TextureFromFile(std::string path, bool gamma)
{
    GLuint textureID;
    GLCALL(glGenTextures(1, &textureID));

    int width, height, components;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, 0);

    if (data)
    {
        GLenum format = 0;
        if (components == 1)
            format = GL_RED;
        else if (components == 3)
            format = GL_RGB;
        else if (components == 4)
            format = GL_RGBA;
        else
            std::cerr << "[stb-image Error]: Unknown number of components " << components << "." << std::endl;

        GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "[stb-image Error]: Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }

    return textureID;
}







std::vector<std::string> Split(std::string source, char delimiter)
{
    std::vector<std::string> result;

    size_t last = 0;
    size_t next = 0;

    while ((next = source.find(delimiter, last)) != std::string::npos)
    {
        std::string value = source.substr(last, next-last);
        result.push_back(value);
        last = next + 1;
    }

    std::string value = source.substr(last);
    result.push_back(value);

    return result;
}


std::pair<std::vector<Vertex>, std::vector<GLuint>> Parse(std::string source)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texture_coordinates;
    std::vector<glm::vec3> normals;

    // TODO(ted): Currently assuming we'll never have more than roughly 2^16 vertices.
    std::unordered_map<unsigned long long, unsigned> vertex;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    std::istringstream stream(source);
    std::string line;


    while ((std::getline(stream, line)))
    {
        // Parse vertices
        if (line[0] == 'v' && line[1] == ' ')
        {
            glm::vec3 position;
            unsigned i = 0;

            std::size_t start = line.find(' ');  // We'll be skipping the first part.
            while (start != std::string::npos)
            {
                std::size_t stop = line.find(' ', start + 1);
                std::string value = line.substr(start, stop - start);
                position[i++] = std::stof(value);
                start = stop;
            }

            positions.push_back(position);
        }

        // Parse texture coordinates
        if (line[0] == 'v' && line[1] == 't')
        {
            glm::vec2 texture_coordinate;
            unsigned i = 0;

            std::size_t start = line.find(' ');  // We'll be skipping the first part.
            while (start != std::string::npos)
            {
                std::size_t stop = line.find(' ', start + 1);
                std::string value = line.substr(start, stop - start);
                texture_coordinate[i++] = std::stof(value);
                start = stop;
            }

            texture_coordinates.push_back(texture_coordinate);
        }

        // Parse normals
        if (line[0] == 'v' && line[1] == 'n')
        {
            glm::vec3 normal;
            unsigned i = 0;

            std::size_t start = line.find(' ');  // We'll be skipping the first part.
            while (start != std::string::npos)
            {
                std::size_t stop = line.find(' ', start + 1);
                std::string value = line.substr(start, stop - start);
                normal[i++] = std::stof(value);
                start = stop;
            }

            normals.push_back(normal);
        }

        // Parse faces
        if (line[0] == 'f')
            break;
    }

    if (positions.empty())
        positions.push_back(glm::vec3(0));
    if (texture_coordinates.empty())
        texture_coordinates.push_back(glm::vec2(0));
    if (normals.empty())
        normals.push_back(glm::vec3(0));

    // Parse faces
    do
    {
        if (line[0] == 'f')
        {
            std::vector<std::string> faces = Split(line, ' ');

            unsigned long long index[3];
            for (unsigned i = 1; i < 4; ++i)  // Skipping first entry (the 'f').
            {
                std::vector<std::string> values = Split(faces[i], '/');

                for (unsigned j = 0; j < 3; ++j)
                    if (!values[j].empty())
                        index[j] = static_cast<unsigned long long>(std::stoi(values[j]) - 1);  // OBJ-files are 1-indexed.
                    else
                        index[j] = 0;

                unsigned long long mapping = (index[0] << 32) + (index[1] << 16) + index[2];
                const auto& duplicate = vertex.find(mapping);
                if (duplicate != vertex.end())
                {
                    indices.emplace_back(duplicate->second);
                }
                else
                {
                    Vertex v{positions[index[0]], texture_coordinates[index[1]], normals[index[2]]};
                    vertices.push_back(v);
                    indices.emplace_back(vertices.size() - 1);
                    vertex.emplace(mapping, indices.back());
                }
            }
        }


    } while ((std::getline(stream, line)));


    return std::make_pair(vertices, indices);
}