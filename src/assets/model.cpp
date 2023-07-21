#include <fstream>
#include <iostream>

#include "assets/model.hpp"
#include "core/engine.hpp"
#include "managers/asset_manager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using structs::Vertex;
using structs::Bone;
using structs::Mesh;
using assets::Material;

namespace assets {
    Model::Model() : Asset(enums::AssetType::Model),
    vertices(), indices(), meshes(),
    VAO(0), VBO(0), EBO(0)
    {
    }

    Model::Model(Vec<Vertex> vertices, Vec<GLushort> indices, Ref<Material> material) :
    Asset(enums::AssetType::Model),
    vertices(std::move(vertices)), indices(std::move(indices)), meshes(),
    VAO(0), VBO(0), EBO(0)
    {
        meshes.emplace_back(
                material,
                0,
                this->vertices.size(),
                0,
                this->indices.size(),
                GL_TRIANGLES
        );

        Setup();
    }

    Model::Model(Vec<Vertex> vertices, Vec<GLushort> indices, Vec<Mesh> meshes) :
    Asset(enums::AssetType::Model),
    vertices(std::move(vertices)), indices(std::move(indices)), meshes(std::move(meshes)),
    VAO(0), VBO(0), EBO(0)
    {
        Setup();
    }

    Model::~Model()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Model::Setup()
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLushort)), &indices[0], GL_STATIC_DRAW);

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

    void Model::LoadFromFile(const std::string &filePath)
    {
        static auto assetManager = g_Engine->GetManager<managers::AssetManager>();

        Assimp::Importer importer {};
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
        if ((!scene) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode))
            core::Log::Error("model assimp error: {}", importer.GetErrorString());

        u32 base = 0;
        for (s32 i = 0; i < scene->mNumMeshes; ++i)
        {
            aiMesh* aMesh = scene->mMeshes[i];

            structs::Mesh primitive {
                assetManager->LoadDefaultAsset<assets::Material>(),
                static_cast<s32>(base),
                static_cast<s32>(aMesh->mNumVertices),
                static_cast<s32>(indices.size()),
                static_cast<s32>(aMesh->mNumFaces * 3),
                GL_TRIANGLES
            };

            for (s32 j = 0; j < aMesh->mNumVertices; ++j)
            {
                structs::Vertex vertex {};
                std::memcpy(&vertex.position, &aMesh->mVertices[j], sizeof(vec3));
                if (aMesh->HasNormals())
                    std::memcpy(&vertex.normal, &aMesh->mNormals[j], sizeof(vec3));
                if (aMesh->HasTextureCoords(0))
                    std::memcpy(&vertex.texCoords, &aMesh->mTextureCoords[0][j], sizeof(vec2));
                vertices.push_back(vertex);
            }

            for (s32 j = 0; j < aMesh->mNumFaces; ++j)
            {
                aiFace face = aMesh->mFaces[j];
                for (s32 k = 0; k < face.mNumIndices; ++k)
                    indices.push_back(base + face.mIndices[k]);
            }

            meshes.push_back(primitive);
            base += aMesh->mNumVertices;
        }

        Setup();
    }

    void Model::Load(const std::string &filePath)
    {
        static auto assetManager = g_Engine->GetManager<managers::AssetManager>();

        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::in);

        ReadAssetInfo(file);

        u32 vertexCount, indexCount, primitiveCount, boneCount;

        file.read(reinterpret_cast<char *>(&vertexCount), sizeof(u32));
        file.read(reinterpret_cast<char *>(&indexCount), sizeof(u32));
        file.read(reinterpret_cast<char *>(&primitiveCount), sizeof(u32));
        file.read(reinterpret_cast<char *>(&boneCount), sizeof(u32));

        vertices.resize(vertexCount);
        indices.resize(indexCount);
        meshes.resize(primitiveCount);

        file.read(reinterpret_cast<char *>(vertices.data()), sizeof(structs::Vertex) * vertexCount);
        file.read(reinterpret_cast<char *>(indices.data()), sizeof(GLuint) * indexCount);

        structs::Mesh primitive;
        Guid guid;
        for (u32 index = 0; index < primitiveCount; ++index)
        {
            file.read(reinterpret_cast<char *>(&guid), sizeof(Guid));
            primitive.material = guid.isValid() ?
                    assetManager->LoadAsset<assets::Material>(guid) :
                    assetManager->LoadDefaultAsset<assets::Material>();

            file.read(reinterpret_cast<char *>(&primitive.firstVertex), sizeof(s32));
            file.read(reinterpret_cast<char *>(&primitive.vertexCount), sizeof(s32));
            file.read(reinterpret_cast<char *>(&primitive.indexStart), sizeof(s32));
            file.read(reinterpret_cast<char *>(&primitive.indexCount), sizeof(s32));
            file.read(reinterpret_cast<char *>(&primitive.primitiveType), sizeof(GLenum));

            meshes[index] = primitive;
        }

        Setup();
    }

    void Model::Save(const std::string &filePath)
    {
        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::out | std::ios::trunc);
        WriteAssetInfo(file);

        u32 vertexCount = vertices.size();
        u32 indexCount = indices.size();
        u32 primitiveCount = meshes.size();
        u32 boneCount = 0;

        file.write(reinterpret_cast<const char *>(&vertexCount), sizeof(u32));
        file.write(reinterpret_cast<const char *>(&indexCount), sizeof(u32));
        file.write(reinterpret_cast<const char *>(&primitiveCount), sizeof(u32));
        file.write(reinterpret_cast<const char *>(&boneCount), sizeof(u32));

        file.write(reinterpret_cast<const char *>(vertices.data()), sizeof(structs::Vertex) * vertexCount);
        file.write(reinterpret_cast<const char *>(indices.data()), sizeof(GLushort) * indexCount);

        static Guid empty {};
        for (const auto& primitive : meshes)
        {
            Guid guid = primitive.material != nullptr ? primitive.material->GetGuid() : empty;
            file.write(reinterpret_cast<const char *>(&guid), sizeof(Guid));
            file.write(reinterpret_cast<const char *>(&primitive.firstVertex), sizeof(s32));
            file.write(reinterpret_cast<const char *>(&primitive.vertexCount), sizeof(s32));
            file.write(reinterpret_cast<const char *>(&primitive.indexStart), sizeof(s32));
            file.write(reinterpret_cast<const char *>(&primitive.indexCount), sizeof(s32));
            file.write(reinterpret_cast<const char *>(&primitive.primitiveType), sizeof(GLenum));
        }

        file.close();
    }
}