#include <crossguid/guid.hpp>

#include "assets/asset.hpp"
#include "core/log.hpp"

#include "utils/asset_util.hpp"
#include "managers/asset_manager.hpp"

namespace assets {
    int Asset::assetVersion = 1;

    Asset::Asset(enums::AssetType assetType) : assetInfo()
    {
        auto guid = xg::newGuid();
        assetInfo = structs::AssetInfo{ assetType, guid.bytes() };
    }

    Asset::~Asset()
    {
    }

    void Asset::WriteAssetInfo(std::fstream &file)
    {
        char header[ASSET_INFO_BYTES];
        assetInfo.ToBytes(header);
        file.write(header, static_cast<s32>(ASSET_INFO_BYTES));
    }

    void Asset::ReadAssetInfo(std::fstream &file)
    {
        char header[ASSET_INFO_BYTES];
        file.read(header, static_cast<s32>(ASSET_INFO_BYTES));
        assetInfo = structs::AssetInfo::FromBytes(header);
    }
}