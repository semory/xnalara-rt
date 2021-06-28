#include "stdafx.h"
#include "RT_math.h"
#include "RT_depth.h"
#include "RT_texture.h"
#include "RT_xnalara.h"
#include "RT_shade.h"

#pragma region HELPER_FUNCTIONS

inline vector4D<float> SampleTexture(const XNATexture& texture, float u, float v)
{
 auto sample = texture.data->sampler(texture.data, u, v);
 vector4D<float> retval;
 retval.r = sample.r/255.0f;
 retval.g = sample.g/255.0f;
 retval.b = sample.b/255.0f;
 retval.a = sample.a/255.0f;
 return retval;
}

inline float Saturate(float value)
{
 if(value < 0.0f) return 0.0f;
 if(value > 1.0f) return 1.0f;
 return value;
}

#pragma endregion HELPER_FUNCTIONS

#pragma region SHADING_FUNCTIONS

vector4D<unsigned char> RG00(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // triangle vertices
 const XNAVertex& v0 = mesh.verts[refs[0]];
 const XNAVertex& v1 = mesh.verts[refs[1]];
 const XNAVertex& v2 = mesh.verts[refs[2]];

 // interpolated UV coordinates
 float intpart;
 float uv[2] = {
  u*modf(v0.uv[0][0], &intpart) + v*modf(v1.uv[0][0], &intpart) + w*modf(v2.uv[0][0], &intpart),
  u*modf(v0.uv[0][1], &intpart) + v*modf(v1.uv[0][1], &intpart) + w*modf(v2.uv[0][1], &intpart)
 };

 // sample image
 if(mesh.n_textures) {
    const XNATexture& texture = mesh.textures[0];
    if(texture.data) return texture.data->sampler(texture.data, uv[0], uv[1]);
   }

 // otherwise
 vector4D<unsigned char> sample;
 sample[0] = 0;
 sample[1] = 0;
 sample[2] = 0;
 sample[3] = 255;
 return sample;
}

vector4D<unsigned char> RG01(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG02(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG03(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG04(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG05(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG06(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG07(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG08(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG09(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG10(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG11(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG12(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG13(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG14(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG15(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG16(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG17(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG18(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG19(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG20(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG21(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG22(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG23(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG24(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG25(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG26(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG27(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG28(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG29(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG30(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG31(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG32(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG33(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG34(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG35(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG36(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG37(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG38(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG39(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG40(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG41(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG42(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG43(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG_thorgear(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG_thorglow(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<unsigned char> RG_handguns(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

#pragma endregion SHADING_FUNCTIONS