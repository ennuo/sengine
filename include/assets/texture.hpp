#ifndef SENGINE_TEST1_TEXTURE1_H
#define SENGINE_TEST1_TEXTURE1_H

#include "core/types.hpp"

#include "assets/asset.hpp"
#include "structs/asset_info.hpp"

namespace enums {
    enum class TextureType
    {
        RGBA = 0,
        BGRA = 1
    };

    enum class TextureFlags
    {
        None = 0,
        NoSRGB = 1
    };
}

namespace assets {
    class Texture : public Asset {
    public:
        Texture(unsigned char* data, enums::TextureType type, s32 width, s32 height);
        Texture();
        ~Texture();

        void Save(const string &filePath) override;
        void Load(const string &filePath) override;
        void LoadFromFile(const string &filePath);
        void LoadFromCompressedData(unsigned char *buffer, s32 len);

        inline s32 GetWrapU() const { return wrapU; }
        inline s32 GetWrapV() const { return wrapV; }
        inline GLuint GetTextureID() const { return textureId; }

    private:
        void BindTexture();
        void Free();

        s32 width, height;

        s32 wrapU, wrapV;
        u32 type, flags;

        s64 dataSize;
        unsigned char* data;

        GLuint textureId;
    };
}

#endif //SENGINE_TEST1_TEXTURE1_H
