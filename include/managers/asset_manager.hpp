#ifndef SENGINE_ASSET_CREATOR_ASSET_MANAGER_HPP
#define SENGINE_ASSET_CREATOR_ASSET_MANAGER_HPP

#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include "core/types.hpp"

#include "enums/asset_type.hpp"
#include "managers/manager.hpp"
#include "assets/asset.hpp"
#include "assets/font.hpp"
#include "assets/material.hpp"
#include "assets/model.hpp"
#include "assets/shader.hpp"
#include "assets/sound.hpp"
#include "assets/texture.hpp"
#include "assets/asset_database.hpp"

const string DEFAULT_ASSET_NAME = ".default";

namespace managers {
    class AssetManager : public Manager {
    public:
        struct AssetInstance {
            string path, name;
            Guid guid;
            enums::AssetType type;
            WeakRef<assets::Asset> ref;
        };

        AssetManager();

        template<typename T>
        static string GetAssetPath(const string &name);
        static string GetAssetPath(const string &folderName, const string &name);

        template<typename T>
        bool AssetExists(const string &name);
        bool AssetExists(const string &path);
        bool AssetExists(const Guid &guid);

        enums::AssetType GetAssetType(const Guid &guid);
        string GetAssetName(const Guid &guid);
        string GetAssetPath(const Guid &guid);

        template<typename T>
        Ref<T> LoadDefaultAsset();

        template<typename T>
        Ref<T> LoadAssetByPath(const string &path);
        template<typename T>
        Ref<T> LoadAsset(const string &name);
        template<typename T>
        Ref<T> LoadAsset(const Guid &guid);

        template<typename T>
        Ref<T> LoadAssetOrDefault(const string &name);
        template<typename T>
        Ref<T> LoadAssetOrDefault(const Guid &guid);

        template<typename T>
        void ReloadAsset(const Guid &guid);

        void LoadDefaultAssets();

    private:
        std::unordered_map<Guid, AssetInstance> assets;
        std::array<Ref<assets::Asset>, enums::ASSET_TYPE_SIZE> defaults;

        template<typename T>
        static void AssertTemplateIsAsset();

        template<typename T>
        const AssetInstance &FindAsset(const string &name);
        const AssetInstance &FindAsset(const string &path);
        const AssetInstance &FindAsset(const Guid &guid);
    };
}

#include "managers/asset_manager.tpp"
#endif //SENGINE_ASSET_CREATOR_ASSET_MANAGER_HPP
