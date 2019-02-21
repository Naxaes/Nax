#include "loader.h"

#include <gtest/gtest.h>

// TEST(Test, Name) { ... code ... )

TEST(Splitting, NewLine)
{
    std::string source =
        "v -0.5, -0.5, 0.0\n"
        "v 0.5, 0.5, 0.0\n"
        "v -0.5, 0.5, 0.0\n"
        "v 0.5, -0.5, 0.0";
    auto result = Split(source, '\n');

    // Correct size
    EXPECT_EQ(result.size(), 4);

    // Correct values
    EXPECT_EQ(result[0], "v -0.5, -0.5, 0.0");
    EXPECT_EQ(result[1], "v 0.5, 0.5, 0.0");
    EXPECT_EQ(result[2], "v -0.5, 0.5, 0.0");
    EXPECT_EQ(result[3], "v 0.5, -0.5, 0.0");
}


TEST(ParsingOBJ, Quad)
{
    std::string source =
        "v -0.5, -0.5, 0.0\n"
        "v 0.5, 0.5, 0.0\n"
        "v -0.5, 0.5, 0.0\n"
        "v 0.5, -0.5, 0.0\n"
        "vn 0.0, 0.0, 0.0\n"
        "vn 0.0, 0.0, 0.0\n"
        "vn 0.0, 0.0, 0.0\n"
        "vn 0.0, 0.0, 0.0\n"
        "f 1//1 2//2 3//3\n"
        "f 1//1 4//4 2//2\n";
    auto result = Parse(source);

    // Correct amount of indices.
    EXPECT_EQ(result.second.size(), 6);

    // Correct index values.
    EXPECT_EQ(result.second[0], 0);
    EXPECT_EQ(result.second[1], 1);
    EXPECT_EQ(result.second[2], 2);
    EXPECT_EQ(result.second[3], 0);
    EXPECT_EQ(result.second[4], 3);
    EXPECT_EQ(result.second[5], 1);

    // Correct amount of vertices.
    EXPECT_EQ(result.first.size(), 4);

    // Correct position values.
    EXPECT_FLOAT_EQ(result.first[0].position.x, -0.5f);
    EXPECT_FLOAT_EQ(result.first[0].position.y, -0.5f);
    EXPECT_FLOAT_EQ(result.first[0].position.z,  0.0f);

    EXPECT_FLOAT_EQ(result.first[1].position.x,  0.5f);
    EXPECT_FLOAT_EQ(result.first[1].position.y,  0.5f);
    EXPECT_FLOAT_EQ(result.first[1].position.z,  0.0f);

    EXPECT_FLOAT_EQ(result.first[2].position.x, -0.5f);
    EXPECT_FLOAT_EQ(result.first[2].position.y,  0.5f);
    EXPECT_FLOAT_EQ(result.first[2].position.z,  0.0f);

    EXPECT_FLOAT_EQ(result.first[3].position.x,  0.5f);
    EXPECT_FLOAT_EQ(result.first[3].position.y, -0.5f);
    EXPECT_FLOAT_EQ(result.first[3].position.z,  0.0f);

    // Correct normal values.
    EXPECT_FLOAT_EQ(result.first[0].normal.x, 0.0f);
    EXPECT_FLOAT_EQ(result.first[0].normal.y, 0.0f);
    EXPECT_FLOAT_EQ(result.first[0].normal.z, 0.0f);

    EXPECT_FLOAT_EQ(result.first[1].normal.x, 0.0f);
    EXPECT_FLOAT_EQ(result.first[1].normal.y, 0.0f);
    EXPECT_FLOAT_EQ(result.first[1].normal.z, 0.0f);

    EXPECT_FLOAT_EQ(result.first[2].normal.x, 0.0f);
    EXPECT_FLOAT_EQ(result.first[2].normal.y, 0.0f);
    EXPECT_FLOAT_EQ(result.first[2].normal.z, 0.0f);

    EXPECT_FLOAT_EQ(result.first[3].normal.x, 0.0f);
    EXPECT_FLOAT_EQ(result.first[3].normal.y, 0.0f);
    EXPECT_FLOAT_EQ(result.first[3].normal.z, 0.0f);
}


TEST(ParsingOBJ, Cube)
{
    std::string source =
        "# cube.obj\n"
        "#\n"
        "\n"
        "mtllib cube.mtl\n"
        "o cube\n"
        "\n"
        "\n"
        "v -0.500000 -0.500000 0.500000\n"
        "v 0.500000 -0.500000 0.500000\n"
        "v -0.500000 0.500000 0.500000\n"
        "v 0.500000 0.500000 0.500000\n"
        "v -0.500000 0.500000 -0.500000\n"
        "v 0.500000 0.500000 -0.500000\n"
        "v -0.500000 -0.500000 -0.500000\n"
        "v 0.500000 -0.500000 -0.500000\n"
        "\n"
        "vt 0.000000 0.000000\n"
        "vt 1.000000 0.000000\n"
        "vt 0.000000 1.000000\n"
        "vt 1.000000 1.000000\n"
        "\n"
        "vn 0.000000 0.000000 1.000000\n"
        "vn 0.000000 1.000000 0.000000\n"
        "vn 0.000000 0.000000 -1.000000\n"
        "vn 0.000000 -1.000000 0.000000\n"
        "vn 1.000000 0.000000 0.000000\n"
        "vn -1.000000 0.000000 0.000000\n"
        "\n"
        "g cube\n"
        "usemtl cube\n"
        "s 1\n"
        "f 1/1/1 2/2/1 3/3/1\n"
        "f 3/3/1 2/2/1 4/4/1\n"
        "s 2\n"
        "f 3/1/2 4/2/2 5/3/2\n"
        "f 5/3/2 4/2/2 6/4/2\n"
        "s 3\n"
        "f 5/4/3 6/3/3 7/2/3\n"
        "f 7/2/3 6/3/3 8/1/3\n"
        "s 4\n"
        "f 7/1/4 8/2/4 1/3/4\n"
        "f 1/3/4 8/2/4 2/4/4\n"
        "s 5\n"
        "f 2/1/5 8/2/5 4/3/5\n"
        "f 4/3/5 8/2/5 6/4/5\n"
        "s 6\n"
        "f 7/1/6 1/2/6 5/3/6\n"
        "f 5/3/6 1/2/6 3/4/6";

    auto result = Parse(source);

    // Correct amount of indices.
    EXPECT_EQ(result.second.size(), 36);

    // Correct index values.
    EXPECT_EQ(result.second[0], 0);
    EXPECT_EQ(result.second[1], 1);
    EXPECT_EQ(result.second[2], 2);
    EXPECT_EQ(result.second[3], 2);
    EXPECT_EQ(result.second[4], 1);
    EXPECT_EQ(result.second[5], 3);

    // Correct amount of vertices.
    EXPECT_EQ(result.first.size(), 24);

}