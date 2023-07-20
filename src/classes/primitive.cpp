#include "classes/primitive.hpp"
#include "assets/texture.hpp"
#include "exceptions/not_implemented.hpp"

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
        return { vertices, indices, nullptr };
    }
}