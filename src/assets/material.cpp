#include "assets/material.hpp"
#include "core/engine.hpp"
#include "utils/asset_util.hpp"
#include "managers/asset_manager.hpp"

namespace assets {
    Material::Material() : Asset(enums::AssetType::Material),
    flags(static_cast<u32>(enums::MaterialFlags::None)),
    textures(), shader()
    {
    }

    Material::Material(Ref<Shader> shader, Ref<Texture> texture) :  Asset(enums::AssetType::Material),
    flags(static_cast<u32>(enums::MaterialFlags::None)), textures(), shader(std::move(shader))
    {
        textures[0] = std::move(texture);
    }

    void Material::Load(const string &filePath)
    {
        static auto assetManager = g_Engine->GetManager<managers::AssetManager>();

        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::in);

        ReadAssetInfo(file);

        file.read(reinterpret_cast<char *>(&flags), sizeof(u32));

        Guid guid;
        file.read(reinterpret_cast<char *>(&guid), sizeof(Guid));
        if (guid.isValid())
        {
            shader = assetManager->LoadAsset<assets::Shader>(guid);
        }

        for (u32 index = 0; index < MAX_TEXTURES; ++index)
        {
            file.read(reinterpret_cast<char *>(&guid), sizeof(Guid));
            if (guid.isValid())
            {
                textures[index] = assetManager->LoadAsset<assets::Texture>(guid);
            }
        }
    }

    void Material::Save(const string &filePath)
    {
        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::out | std::ios::trunc);
        WriteAssetInfo(file);

        file.write(reinterpret_cast<const char *>(&flags), sizeof(u32));

        static Guid empty{};
        Guid guid;

        guid = shader != nullptr ? shader->GetGuid() : empty;
        file.write(reinterpret_cast<const char *>(&guid), sizeof(Guid));

        for (const auto& texture : textures)
        {
            guid = texture != nullptr ? texture->GetGuid() : empty;
            file.write(reinterpret_cast<const char *>(&guid), sizeof(Guid));
        }

        file.close();
    }

    void Material::Bind()
    {
        if (shader == nullptr)
        {
            glUseProgram(0);
            return;
        }

        glUseProgram(shader->GetProgram());

        for (u32 index = 0; index < MAX_TEXTURES; ++index)
        {
            auto& texture = textures[index];
            if (texture != nullptr)
            {
                glActiveTexture(GL_TEXTURE0 + index);
                glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->GetWrapU());
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->GetWrapV());
            }
        }

        glActiveTexture(GL_TEXTURE0);
    }
}