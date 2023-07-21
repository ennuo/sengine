#ifndef SENGINE_ASSET_CREATOR_ASSET_MANAGER_TPP
#define SENGINE_ASSET_CREATOR_ASSET_MANAGER_TPP

#include <fmt/format.h>
#include <type_traits>
#include <filesystem>

#include "core/log.hpp"
#include "utils/asset_util.hpp"
#include "exceptions/invalid_template.hpp"

namespace managers {
    template<typename T>
    string AssetManager::GetAssetPath(const string &name) 
    {
        AssertTemplateIsAsset<T>();

        static std::unordered_map<std::type_index, string> assetFolderNames = {
                { typeid(assets::Font), "font" },
                { typeid(assets::Material), "material" },
                { typeid(assets::Model), "model" },
                { typeid(assets::Shader), "shader" },
                { typeid(assets::Sound), "sound" },
                { typeid(assets::Texture), "texture" },
                { typeid(assets::AssetDatabase), "assetdb" }
        };

        auto find = assetFolderNames.find(typeid(T));
        if (find == assetFolderNames.end()) throw exceptions::InvalidTemplate(fmt::format("type \"{}\" doesn't exist in AssetManager::AssetFolderNames", typeid(T).name()));

        string folderName = find->second;
        return GetAssetPath(folderName, name);
    }

    template<typename T>
    bool AssetManager::AssetExists(const string &name)
    {
        AssertTemplateIsAsset<T>();

        string path = GetAssetPath<T>(name);
        return AssetExists(path);
    }

    template<typename T>
    Ref<T> AssetManager::LoadAssetOrDefault(const Guid &guid)
    {
        auto asset = LoadAsset<T>(guid);
        if (asset != nullptr) return asset;
        return LoadDefaultAsset<T>();
    }

    template<typename T>
    Ref<T> AssetManager::LoadAssetOrDefault(const string &name)
    {
        auto asset = LoadAsset<T>(name);
        if (asset != nullptr) return asset;
        return LoadDefaultAsset<T>();
    }

    template<typename T>
    Ref<T> AssetManager::LoadAssetByPath(const string &path)
    {
        if (AssetExists(path))
        {
            auto& asset = FindAsset(path);
            return std::static_pointer_cast<T>(asset.ref.lock());
        }

        if (!std::filesystem::exists(path))
        {
            core::Log::Warn(fmt::format("tried to load asset from \"{}\", but the file doesn't exist", path));
            return nullptr;
        }

        try
        {
            core::Log::Info(fmt::format("loading asset from \"{}\"", path));

            auto asset = std::make_shared<T>();
            asset->Load(path);

            assets[asset->GetGuid()] = {
                    path,
                    "",
                    asset->GetGuid(),
                    utils::GetAssetType<T>(),
                    std::weak_ptr<T>(asset)
            };

            return asset;
        }
        catch (...) {
            core::Log::Info("oops!");
            return nullptr;
        }
    }

    template<typename T>
    Ref<T> AssetManager::LoadAsset(const Guid &guid)
    {
        AssertTemplateIsAsset<T>();
        if (AssetExists(guid))
        {
            return std::static_pointer_cast<T>(assets[guid].ref.lock());
        }

        auto database = LoadDefaultAsset<assets::AssetDatabase>();
        if (database != nullptr && database->ContainsAsset(guid))
        {
            return LoadAssetByPath<T>(database->GetAssetPath(guid));
        }

        return nullptr;
    }

    template<typename T>
    Ref<T> AssetManager::LoadAsset(const string &name) {
        AssertTemplateIsAsset<T>();

        string path = GetAssetPath<T>(name);
        return LoadAssetByPath<T>(path);
    }

    template<typename T>
    Ref<T> AssetManager::LoadDefaultAsset() {
        auto asset = LoadAsset<T>(DEFAULT_ASSET_NAME);

        auto type = utils::GetAssetType<T>();
        defaults[static_cast<s32>(type)] = asset;

        return asset;
    }

    template<typename T>
    void AssetManager::ReloadAsset(const Guid &guid)
    {
        // TODO: Implement this
        // should be able to reload the resource without
        // destructing / constructing a new pointer!
    }

    template<typename T>
    void AssetManager::AssertTemplateIsAsset() {
        if (std::is_base_of<assets::Asset, T>::value) return;

        throw exceptions::InvalidTemplate(fmt::format("template \"{}\" is not derived from Asset", typeid(T).name()));
    }

    template<typename T>
    const AssetManager::AssetInstance &AssetManager::FindAsset(const string &name) {
        AssertTemplateIsAsset<T>();
        return GetAssetData(GetAssetPath<T>(name));
    }
}

#endif //SENGINE_ASSET_CREATOR_ASSET_MANAGER_TPP
