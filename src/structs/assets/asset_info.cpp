#include "structs/asset_info.hpp"
#include "enums/asset_revision.hpp"

const size_t ASSET_INFO_BYTES = 128;
const char *ASSET_INFO_SIGNATURE = "ASSET";
const unsigned short ASSET_INFO_VERSION = 1;
const enums::AssetRevision ASSET_INFO_REVISION = enums::AssetRevision::TextureFlags;