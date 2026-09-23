#include "CubeMesh.h"
#include "../Texture/Texture.h"
#include <glad/glad.h>

const std::vector<float> CubeMesh::vertices = {
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};
const std::vector<float> CubeMesh::texCoords = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};
const std::vector<float> CubeMesh::normals = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, -1.0f,
    1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, -1.0f, 0.0f
};
const std::vector<uint32_t> CubeMesh::indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    3, 2, 6, 6, 7, 3,
    0, 1, 5, 5, 4, 0,
    1, 2, 6, 6, 5, 1,
    0, 3, 7, 7, 4, 0
};

CubeMesh::CubeMesh() : Mesh(vertices, texCoords, normals, indices, GL_TRIANGLES) {
}

void CubeMesh::addFace(
    std::vector<float>& vertices,
    std::vector<float>& texCoords,
    std::vector<float>& normals,
    std::vector<uint32_t>& indices,
    const glm::vec3& position,
    Face face,
    uint32_t indexOffset,
    const glm::vec3& color,
    const Region& region,
    const Texture& atlas,
    float height
) {

    static const float faceVertices[6][12] = {
        {0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0},
        {0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1},
        {0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0},
    };

    static const float normalsData[6][3] = {
        {-1, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    static const float faceTexCoords[6][8] = {
        // Left
        {
            0, 0,
            0, 1,
            1, 1,
            1, 0
        },

        // Right
        {
            0, 0,
            0, 1,
            1, 1,
            1, 0
        },

        // Top
        {
            0, 0,
            1, 0,
            1, 1,
            0, 1
        },

        // Bottom
        {
            0, 1,
            1, 1,
            1, 0,
            0, 0
        },

        // Front
        {
            0, 0,
            1, 0,
            1, 1,
            0, 1
        },

        // Back
        {
            1, 0,
            0, 0,
            0, 1,
            1, 1
        }
    };

    int f = static_cast<int>(face);

    for (int i = 0; i < 4; i++) {
        float vx = faceVertices[f][i * 3 + 0];
        float vy = faceVertices[f][i * 3 + 1];
        float vz = faceVertices[f][i * 3 + 2];

        vy *= height;

        vertices.push_back(vx + position.x);
        vertices.push_back(vy + position.y);
        vertices.push_back(vz + position.z);

        glm::vec2 baseUV(
            faceTexCoords[f][i * 2 + 0],
            1.0f - faceTexCoords[f][i * 2 + 1]
        );

        glm::vec2 atlasUV = toAtlasUV(
            baseUV,
            region,
            atlas
        );

        texCoords.push_back(atlasUV.x);
        texCoords.push_back(atlasUV.y);

        normals.push_back(normalsData[f][0]);
        normals.push_back(normalsData[f][1]);
        normals.push_back(normalsData[f][2]);
    }

    indices.push_back(indexOffset + 0);
    indices.push_back(indexOffset + 1);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset + 3);
    indices.push_back(indexOffset + 0);
}

glm::vec2 CubeMesh::toAtlasUV(
    const glm::vec2& uv,
    const Region& region,
    const Texture& texture
) {
    float texW = static_cast<float>(texture.getWidth());
    float texH = static_cast<float>(texture.getHeight());

    float u0 = region.x / texW;
    float v0 = region.y / texH;

    float u1 = (region.x + region.width) / texW;
    float v1 = (region.y + region.height) / texH;

    return {
        glm::mix(u0, u1, uv.x),
        glm::mix(v0, v1, uv.y)
    };
}