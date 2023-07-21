#ifndef SENGINE_ASSET_CREATOR_PRIMITIVE_HPP
#define SENGINE_ASSET_CREATOR_PRIMITIVE_HPP

#include "core/types.hpp"
#include "structs/primitive_type.hpp"
#include "assets/model.hpp"

namespace classes {
    class Primitive {
    public:
        static Ref<assets::Model> GetModel(const structs::PrimitiveType &primitiveType);
        Ref<assets::Model> GenerateModel();
    private:
        Primitive(std::vector<structs::Vertex> vertices, std::vector<GLushort> indices);

        std::vector<structs::Vertex> vertices;
        std::vector<GLushort> indices;
    };
}

#endif //SENGINE_ASSET_CREATOR_PRIMITIVE_HPP
