#ifndef SENGINE_TEST1_ASSET_TYPE_H
#define SENGINE_TEST1_ASSET_TYPE_H

namespace enums {
    enum class AssetType {
        Font,
        Material,
        Model,
        Shader,
        Sound,
        Texture,
        AssetDatabase,

        Count
    };

    const int ASSET_TYPE_SIZE = static_cast<const int>(AssetType::Count);
}

#endif //SENGINE_TEST1_ASSET_TYPE_H
