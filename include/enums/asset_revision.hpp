#ifndef SENGINE_REVISION_HPP
#define SENGINE_REVISION_HPP

namespace enums {
    enum class AssetRevision : unsigned int {
        Initial = 0,
        TextureFlags = 1, // Added load flags to texture asset
    };
}

#endif //SENGINE_REVISION_HPP
