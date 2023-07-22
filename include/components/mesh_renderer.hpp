#ifndef SENGINE_ASSET_CREATOR_MESH_RENDERER_HPP
#define SENGINE_ASSET_CREATOR_MESH_RENDERER_HPP

#include "core/types.hpp"
#include "components/component.hpp"
#include "assets/texture.hpp"
#include "assets/material.hpp"
#include "assets/model.hpp"

namespace components {
    class MeshRenderer : public Component {
    public:
        MeshRenderer(EntityId entityId);
        inline void SetModel(Ref<assets::Model> newModel) { model = std::move(newModel); }
        void Render() override;
    private:
        Ref<assets::Model> model;
    };
}

#endif //SENGINE_ASSET_CREATOR_MESH_RENDERER_HPP
