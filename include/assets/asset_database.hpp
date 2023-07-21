#ifndef SENGINE_ASSET_DATABASE_HPP
#define SENGINE_ASSET_DATABASE_HPP

#include "core/types.hpp"
#include "assets/asset.hpp"

#include <unordered_map>

namespace assets {
    class AssetDatabase : public Asset {
    public:
        struct AssetData {
            string path;
            enums::AssetType type;
            Guid guid;
        };

        AssetDatabase();

        static Ref<AssetDatabase> Build(std::string & root);

        bool ContainsAsset(const Guid &guid);
        Guid GetAssetGuid(const string &path);
        string GetAssetPath(const Guid& guid);

        void Save(const std::string &filePath) override;
        void Load(const std::string &filePath) override;
        void Clear() override;
    private:
        Vec<AssetData> assets;
    };
}



#endif //SENGINE_ASSET_DATABASE_HPP
