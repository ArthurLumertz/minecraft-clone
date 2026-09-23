#include "QuadMesh.h"
#include <glad/glad.h>

const std::vector<float> QuadMesh::vertices = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};
const std::vector<float> QuadMesh::texCoords = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
};
const std::vector<float> QuadMesh::normals = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
};
const std::vector<uint32_t> QuadMesh::indices = {
    0, 1, 2,
    2, 3, 0
};

QuadMesh::QuadMesh() : Mesh(vertices, texCoords, normals, indices, GL_TRIANGLES) {
}
