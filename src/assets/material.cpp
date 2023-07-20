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

    void Material::Load(const string &filePath)
    {
        static auto assetManager = g_Engine->GetManager<managers::AssetManager>();

        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::in);

        ReadAssetInfo(file);

        file.read(reinterpret_cast<char *>(&flags), sizeof(u32));

        AssetInfoGuid guid;
        file.read(reinterpret_cast<char *>(&guid), sizeof(AssetInfoGuid));




        // ???
    }

    void Material::Save(const string &filePath)
    {

        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::out | std::ios::trunc);
        WriteAssetInfo(file);


        file.write(reinterpret_cast<const char *>(&flags), sizeof(u32));

        file.write(reinterpret_cast<const char *>(shader != nullptr ? shader->GetGuid().bytes().data() : invalidGUID), sizeof(AssetInfoGuid));
        for (const auto& texture : textures)
        {
            file.write(reinterpret_cast<const char *>(texture != nullptr ? texture->GetGuid().bytes().data() : invalidGUID), sizeof(AssetInfoGuid));
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