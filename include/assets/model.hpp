#ifndef SENGINE_TEST1_MODEL_H
#define SENGINE_TEST1_MODEL_H

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

    struct Mesh {
        Ref<assets::Material> material;
        s32 firstVertex;
        s32 vertexCount;
        s32 indexStart;
        s32 indexCount;
        GLenum primitiveType;
    };
}

namespace assets {
    class Model : public Asset {
    public:
        Model();
        Model(Vec<structs::Vertex> vertices, Vec<GLushort> indices, Ref<assets::Material> material);
        Model(Vec<structs::Vertex> vertices, Vec<GLushort> indices, Vec<structs::Mesh> meshes);
        ~Model();

        void Save(const string &filePath) override;
        void Load(const string &filePath) override;
        void LoadFromFile(const string &filePath);

        inline Vec<structs::Mesh> &GetMeshes() { return meshes; }

        inline void Bind() { glBindVertexArray(VAO); }

    private:
        void Setup();

        Vec<structs::Mesh> meshes;
        Vec<structs::Vertex> vertices;
        Vec<GLushort> indices;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };
}

#endif //SENGINE_TEST1_MODEL_H
