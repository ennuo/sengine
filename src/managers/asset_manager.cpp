#include <fmt/format.h>

#include "core/log.hpp"
#include "exceptions/exception.hpp"
#include "exceptions/invalid_arguments.hpp"
#include "managers/asset_manager.hpp"

namespace managers {
    AssetManager::AssetManager() : assets(), defaults() {}

    void AssetManager::LoadDefaultAssets()
    {
        core::Log::Info("LOADING ASSET DATABASE");
        LoadDefaultAsset<assets::AssetDatabase>();

        core::Log::Info("LOADING TEXTURE");
        LoadDefaultAsset<assets::Texture>();

        core::Log::Info("LOADING SHADER");
        LoadDefaultAsset<assets::Shader>();

        //core::Log::Info("LOADING FONT");
        //LoadDefaultAsset<assets::Font>();

        core::Log::Info("LOADING MATERIAL");
        LoadDefaultAsset<assets::Material>();

        //LoadDefaultAsset<assets::Model>();
        //LoadDefaultAsset<assets::Sound>();
    }

    string AssetManager::GetAssetPath(const string &folderName, const string &name)
    {
        static string assetFilePrefix = "resources/assets/";
        static string assetFileSuffix = ".asset";

        return (assetFilePrefix + folderName + "/" + name + assetFileSuffix);
    }

    bool AssetManager::AssetExists(const Guid &guid)
    {
        if (assets.contains(guid))
        {
            auto& asset = assets[guid];
            if (asset.ref.expired())
            {
                assets.erase(guid);
                return false;
            }

            return true;
        }

        return false;
    }

    bool AssetManager::AssetExists(const string &path)
    {
        for (const auto& pair : assets)
        {
            auto& asset = pair.second;
            if (asset.path == path)
            {
                if (asset.ref.expired())
                {
                    assets.erase(asset.guid);
                    return false;
                }

                return true;
            }
        }

        return false;
    }

    enums::AssetType AssetManager::GetAssetType(const Guid &guid) {
        auto& asset = FindAsset(guid);
        return asset.type;
    }

    string AssetManager::GetAssetName(const Guid &guid) {
        auto& asset = FindAsset(guid);
        return asset.name;
    }

    string AssetManager::GetAssetPath(const Guid &guid) {
        auto& asset = FindAsset(guid);
        return asset.path;
    }

    const AssetManager::AssetInstance &AssetManager::FindAsset(const string &path)
    {
        for (const auto& pair : assets)
        {
            auto& asset = pair.second;
            if (asset.path == path)
            {
                return asset;
            }
        }

        throw exceptions::InvalidArguments(fmt::format("FindAsset of name \"{}\" doesn't exist", path));
    }

    const AssetManager::AssetInstance &AssetManager::FindAsset(const Guid &guid)
    {
        auto asset = assets.find(guid);
        if (asset == assets.end())
        {
            throw exceptions::InvalidArguments(fmt::format("GetAssetData of path \"{}\" doesn't exist", guid.str()));
        }
        return asset->second;
    }
}