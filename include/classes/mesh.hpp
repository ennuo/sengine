#ifndef SENGINE_ASSET_CREATOR_MESH_HPP
#define SENGINE_ASSET_CREATOR_MESH_HPP

#include "core/types.hpp"

#include "assets/texture.hpp"
#include "assets/shader.hpp"
#include "assets/material.hpp"

namespace structs {
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texCoords;
    };

    struct Bone {

        explicit Bone(string boneName) :
        name(std::move(boneName)),
        inverse(), position(),
        rotation(1.0f, 0.0f, 0.0f, 0.0f),
        scale(1.0f, 1.0f, 1.0f)
        {
        }

        string name;

        mat4 inverse;

        vec3 position;
        quat rotation;
        vec3 scale;
    };

    struct SubMesh {
        Ref<assets::Material> material;
        s32 firstVertex;
        s32 vertexCount;
        s32 indexStart;
        s32 indexCount;
        GLenum primitiveType;
    };
}

namespace classes {
    class Mesh {
    public:
        Mesh(Vec<structs::Vertex> vertices, Vec<GLuint> indices, Ref<assets::Material> material);
        Mesh(Vec<structs::Vertex> vertices, Vec<GLuint> indices, Vec<structs::SubMesh> submeshes);
        ~Mesh();

        void Draw();
    private:
        void SetupMesh();

        Vec<structs::Vertex> vertices;
        Vec<GLuint> indices;
        Vec<structs::SubMesh> submeshes;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };
}

#endif //SENGINE_ASSET_CREATOR_MESH_HPP
