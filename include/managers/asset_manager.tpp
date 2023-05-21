#ifndef SENGINE_ASSET_MANAGER_TPP
#define SENGINE_ASSET_MANAGER_TPP

#include "assets/asset.hpp"
#include "assets/font.hpp"
#include "assets/material.hpp"
#include "assets/model.hpp"
#include "assets/shader.hpp"
#include "assets/sound.hpp"
#include "assets/texture.hpp"

namespace managers {
    template<typename T = assets::Asset>
    std::shared_ptr<T> AssetManager::LoadAsset(const std::string &assetName) {
        std::shared_ptr<T> asset;
        const std::string assetPath = GetAssetPath<T>(assetName);

        try {
            asset = GetAsset<T>(assetName);
            if ((std::filesystem::exists(assetPath)) && (asset == nullptr)) {
                asset = std::make_shared<T>();
                asset->Load(assetPath);
                assetMap.insert({ assetPath, asset });
            }
        }
        catch(const std::exception &e) {
            core::Log::Warn("an error occured when trying to get asset ", assetName, ":", e.what());
            asset = nullptr;
        }

        if (asset == nullptr) {
            core::Log::Warn("asset \"{}\" not found, using default", assetPath);

            const std::string defaultAssetPath = GetAssetPath<T>(ASSET_DEFAULT_NAME);
            if (!std::filesystem::exists(defaultAssetPath)) core::Log::Error("missing default asset \"{}\"", defaultAssetPath);

            asset = LoadAsset<T>(ASSET_DEFAULT_NAME);
        }

        return asset;
    }

    template<typename T = assets::Asset>
    void AssetManager::UnLoadAsset(const std::string &assetName) {
        const std::string assetPath = GetAssetPath<T>(assetName);
        auto find = assetMap.find(assetPath);
        if (find == assetMap.end()) return;

        assetMap.erase(find);
    }

    template<typename T = assets::Asset>
    std::shared_ptr<T> AssetManager::GetAsset(const std::string &assetName) {
        const std::string assetPath = GetAssetPath<T>(assetName);
        std::shared_ptr<assets::Asset> asset = GetAssetRaw(assetPath);
        return dynamic_pointer_cast<T>((asset == nullptr) ? nullptr : asset);
    }

    template<typename T = assets::Asset>
    bool AssetManager::AssetExists(const std::string &assetName) {
        return (GetAsset<T>(assetName) != nullptr);
    }

    template<typename T = assets::Asset>
    std::shared_ptr<T> AssetManager::GetDefaultAsset() {
        return LoadAsset<T>(".default");
    };
}

#endif //SENGINE_ASSET_MANAGER_TPP