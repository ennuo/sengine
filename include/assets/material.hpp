#ifndef SENGINE_MATERIAL_HPP
#define SENGINE_MATERIAL_HPP

#define MAX_TEXTURES 8

#include <utility>

#include "core/types.hpp"

#include "assets/asset.hpp"
#include "assets/shader.hpp"
#include "assets/texture.hpp"

namespace enums {
    enum class MaterialFlags
    {
        None = 0x0,
        Wireframe = 0x1,
        DisableCulling = 0x2
    };
}

namespace assets {
    class Material : public Asset {
    public:
        Material();

        void Save(const string &filePath) override;
        void Load(const string &filePath) override;

        inline u32 GetFlags() const { return flags; }
        inline Ref<Shader> GetShader() const { return shader; }

        inline void SetFlags(u32 newFlags) { flags = newFlags; }
        inline void SetTexture(Ref<Texture> texture, u32 index) { textures[index] = std::move(texture); }

        void Bind();
    private:
        u32 flags;
        Ref<Shader> shader;

        Array<Ref<Texture>, MAX_TEXTURES> textures;
    };
}

#endif //SENGINE_MATERIAL_HPP
