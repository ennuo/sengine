#include <filesystem>

#include "assets/asset_database.hpp"

namespace assets {
    AssetDatabase::AssetDatabase() : Asset(enums::AssetType::AssetDatabase),
    assets()
    {
    }

    bool AssetDatabase::ContainsAsset(const Guid &guid)
    {
        for (const auto& asset : assets)
        {
            if (asset.guid == guid)
                return true;
        }

        return false;
    }

    Guid AssetDatabase::GetAssetGuid(const string &path)
    {
        for (const auto& asset : assets)
        {
            if (asset.path == path)
                return asset.guid;
        }
        return {};
    }

    string AssetDatabase::GetAssetPath(const Guid &guid)
    {
        for (const auto& asset : assets)
        {
            if (asset.guid == guid)
                return asset.path;
        }
        return {};
    }

    void AssetDatabase::Load(const string &filePath)
    {
        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::in);

        ReadAssetInfo(file);

        u32 count;
        file.read(reinterpret_cast<char *>(&count), sizeof(u32));
        assets.resize(count);

        AssetData data;
        char path[256];
        for (u32 index = 0; index < count; ++index)
        {
            u32 len;
            file.read(reinterpret_cast<char *>(&len), sizeof(u32));
            file.read(path, len);
            path[len] = '\0';

            data.path = std::string(path);
            file.read(reinterpret_cast<char *>(&data.type), sizeof(enums::AssetType));
            file.read(reinterpret_cast<char *>(&data.guid), sizeof(AssetInfoGuid));

            assets.push_back(data);
        }

        file.close();
    }

    void AssetDatabase::Save(const string &filePath)
    {
        std::fstream file;
        file.open(filePath, std::ios::binary | std::ios::out | std::ios::trunc);
        WriteAssetInfo(file);

        u32 count = assets.size();
        file.write(reinterpret_cast<const char *>(&count), sizeof(u32));

        for (const auto& asset : assets)
        {
            u32 len = asset.path.size();
            file.write(reinterpret_cast<const char*>(&len), sizeof(u32));
            file.write(asset.path.c_str(), len);

            file.write(reinterpret_cast<const char *>(&asset.type), sizeof(enums::AssetType));
            file.write(reinterpret_cast<const char *>(asset.guid.bytes().data()), sizeof(AssetInfoGuid));
        }

        file.close();
    }

    void AssetDatabase::Clear()
    {
        assets.clear();
    }

    Ref<AssetDatabase> AssetDatabase::Build(std::string &root)
    {
        auto database = std::make_shared<AssetDatabase>();
        char header[ASSET_INFO_BYTES];
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
        {
            if (entry.is_directory()) continue;

            std::fstream file;
            file.open(entry.path(), std::ios::binary | std::ios::in);
            if (file.is_open())
            {
                file.read(header, static_cast<s32>(ASSET_INFO_BYTES));
                if (file.good())
                {
                    auto info = structs::AssetInfo::FromBytes(header);

                    string path = entry.path().generic_string();
                    std::replace(path.begin(), path.end(), '\\', '/');

                    database->assets.emplace_back(path, info.assetType, Guid(info.guid));
                }
                file.close();
            }
        }

        return database;
    }
}