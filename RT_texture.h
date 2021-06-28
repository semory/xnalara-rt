#ifndef __RT_TEXTURE_H
#define __RT_TEXTURE_H

// XNATextureManager
// ----------------------------------------------------------------------------
// For most models, the same textures are used for different meshes. Instead of
// loading the same texture multiple times, each textures is loaded into memory
// only once. An unordered map can keep a reference count on textures that have
// already been loaded.

enum class XNATextureFormat : unsigned short {
 UNKNOWN,
 BGRA,
 BGRX,
 BGR,
};

// change this to enum class
#define XNA_ALPHA             0x0001u
#define XNA_CUBEMAP           0x0200u
#define XNA_CUBEMAP_POSITIVEX 0x0400u
#define XNA_CUBEMAP_NEGATIVEX 0x0800u
#define XNA_CUBEMAP_POSITIVEY 0x1000u
#define XNA_CUBEMAP_NEGATIVEY 0x2000u
#define XNA_CUBEMAP_POSITIVEZ 0x4000u
#define XNA_CUBEMAP_NEGATIVEZ 0x8000u

struct XNATextureData {
 unsigned int dx;
 unsigned int dy;
 unsigned int pitch;
 unsigned int type;
 unsigned short mipmap;
 XNATextureFormat format;
 std::unique_ptr<unsigned char[]> data;
 std::function<vector4D<unsigned char>(XNATextureData*, float, float)> sampler;
};

// Global Texture Functions
bool LoadTexture(const wchar_t* filename, XNATextureData** out);
bool FreeTexture(const wchar_t* filename);
const XNATextureData* FindTexture(const wchar_t* filename);

#endif
