#ifndef __RT_XNALARA_H
#define __RT_XNALARA_H

// forward declarations
struct XNAHeader;
struct XNABone;
struct XNAVertex;
struct XNATextureData;
struct XNATexture;
struct XNAFace;
struct XNAGlobalFace;
struct XNAMeshParams;
struct XNAMesh;
struct XNAModel;

// typedefs
typedef vector4D<unsigned char> (*XNAShadeFunction)(const XNAMesh&, const uint32_t*, const ray3D&, float, float, float);

struct XNAHeader {
 unsigned int magic;
 unsigned short version[2];
 std::wstring creator;
 unsigned int n_item;
 std::wstring rn;
 std::wstring un;
 std::wstring filename;
 std::vector<unsigned int> optional;
};

struct XNABone {
 std::wstring name;
 unsigned short parent;
 float position[3];
};

struct XNAVertex {
 float position[3];
 float normal[3];
 unsigned char color[4];
 float uv[2][2];
 float tangent[2][4];
 unsigned short blendindices[4];
 float blendweights[4];
};

struct XNATexture {
 XNATextureData* data;
 std::wstring name;
 unsigned int channel;
};

struct XNAFace {
 unsigned int refs[3];
};

struct XNAGlobalFace {
 union {
  struct { uint32_t refs[3], mesh_index; };
  uint32_t data[4];
 };
};

struct XNAMeshParams {
 uint16_t render_group;
 uint8_t optional[2];
 std::wstring fullname;
 std::wstring name;
 float params[3];
 std::vector<std::wstring> pivots;
 XNAShadeFunction shader;
};

struct XNAMesh {
 XNAMeshParams params;
 unsigned int n_channels;
 unsigned int n_textures;
 unsigned int n_vert;
 unsigned int n_face;
 std::vector<XNATexture> textures;
 std::vector<XNAVertex> verts;
 std::vector<XNAFace> faces;
};

struct XNAModel {
 bool isXPS;
 XNAHeader header;
 unsigned int n_jnts;
 unsigned int n_mesh;
 std::vector<XNABone> bonelist;
 std::vector<XNAMesh> meshlist;
 std::vector<XNAGlobalFace> facelist;
};

bool LoadXNAMeshBin(const wchar_t* filename, XNAModel* model);
bool LoadXPSMeshBin(const wchar_t* filename, XNAModel* model);
bool FreeXNAMesh(XNAModel* model);

#endif

