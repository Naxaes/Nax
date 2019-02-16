#include "loader.h"

#include <string>
#include <sstream>
#include <unordered_map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


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