#include "core/engine.hpp"
#include "classes/mesh.hpp"
#include "assets/texture.hpp"
#include "utils/asset_util.hpp"
#include "managers/asset_manager.hpp"

using structs::Vertex;
using structs::Bone;
using structs::SubMesh;
using assets::Material;

namespace classes {
    Mesh::Mesh(Vec<Vertex> vertices, Vec<GLuint> indices, Ref<Material> material) :
    vertices(std::move(vertices)), indices(std::move(indices)), submeshes(),
    VAO(0), VBO(0), EBO(0)
    {
        submeshes.emplace_back(
            material,
            0,
            this->vertices.size(),
            0,
            this->indices.size(),
            GL_TRIANGLES
        );

        SetupMesh();
    }

    Mesh::Mesh(Vec<Vertex> vertices, Vec<GLuint> indices, Vec<SubMesh> submeshes) :
    vertices(std::move(vertices)), indices(std::move(indices)), submeshes(std::move(submeshes)),
    VAO(0), VBO(0), EBO(0)
    {
        SetupMesh();
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Mesh::SetupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        GLsizei vertexSize = sizeof(structs::Vertex);
        GLsizei verticesSize = static_cast<GLsizei>(vertices.size());
        glBufferData(GL_ARRAY_BUFFER, verticesSize * vertexSize, &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*) nullptr);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(structs::Vertex, normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(structs::Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Mesh::Draw()
    {
        glBindVertexArray(VAO);
        for (const auto& submesh : submeshes)
        {
            submesh.material->Bind();
            glDrawElements(submesh.primitiveType, submesh.indexCount, GL_UNSIGNED_INT, (void*)(submesh.indexStart * sizeof(GLuint)));
        }
        glBindVertexArray(0);
    }
}
