#include "managers/asset_manager.hpp"

managers::AssetManager *g_AssetManager = new managers::AssetManager();

namespace managers {
    AssetManager::AssetManager() {
        assets = std::map<std::string, std::shared_ptr<assets::Asset>>();
    }

    template<>
    std::string AssetManager::GetAssetPath<assets::Shader>(const std::string &assetName) {
        return (ASSET_FILE_PREFIX + "shader/" + assetName + ASSET_FILE_SUFFIX);
    }

    template<>
    std::string AssetManager::GetAssetPath<assets::Model>(const std::string &assetName) {
        return (ASSET_FILE_PREFIX + "model/" + assetName + ASSET_FILE_SUFFIX);
    }

    template<>
    std::string AssetManager::GetAssetPath<assets::Texture>(const std::string &assetName) {
        return (ASSET_FILE_PREFIX + "texture/" + assetName + ASSET_FILE_SUFFIX);
    }
}