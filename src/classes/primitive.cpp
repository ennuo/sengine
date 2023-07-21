#include "classes/primitive.hpp"
#include "assets/texture.hpp"
#include "exceptions/not_implemented.hpp"
#include "utils/asset_util.hpp"
#include "managers/asset_manager.hpp"

namespace classes {
    Primitive::Primitive(Vec<structs::Vertex> vertices, Vec<GLuint> indices) : vertices(std::move(vertices)), indices(std::move(indices)) { }

    Mesh Primitive::GetMesh(const structs::PrimitiveType &primitiveType) {
        switch (primitiveType) {
            case structs::PrimitiveType::Plane: {
                static Vec<structs::Vertex> vertices = {
                        {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }}, // left
                        {{ 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }}, // right
                        {{ -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }}, // top left
                        {{ 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }}, // top left
                };

                static Vec<GLuint> indices = {
                        2, 0, 1,
                        3, 2, 1
                };

                static Primitive plane = Primitive(vertices, indices);
                static Mesh mesh = plane.GenerateMesh();
                return mesh;
            }

            default: {
                throw exceptions::NotImplemented("Primitive::GetMesh");
            }
        }
    }

    classes::Mesh Primitive::GenerateMesh() {
        static auto assetManager = g_Engine->GetManager<managers::AssetManager>();
        return { vertices, indices, assetManager->LoadDefaultAsset<assets::Material>() };
    }
}