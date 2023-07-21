#ifndef SENGINE_ASSET_CREATOR_TEXTURE_RENDERER_HPP
#define SENGINE_ASSET_CREATOR_TEXTURE_RENDERER_HPP

#include "core/types.hpp"
#include "components/component.hpp"
#include "assets/texture.hpp"
#include "assets/material.hpp"
#include "assets/model.hpp"

namespace components {
    class TextureRenderer : public Component {
    public:
        TextureRenderer(EntityId entityId);

        inline Ref<assets::Texture> GetTexture() const { return texture; }
        inline Ref<assets::Material> GetMaterial() const { return material; }

        inline void SetMaterial(Ref<assets::Material> newMaterial) { material = std::move(newMaterial); }
        inline void SetTexture(Ref<assets::Texture> newTexture) { texture = std::move(newTexture); }

        void Render() override;
    private:
        Ref<assets::Texture> texture;
        Ref<assets::Material> material;
        Ref<assets::Model> model;
    };
}

#endif //SENGINE_ASSET_CREATOR_TEXTURE_RENDERER_HPP
