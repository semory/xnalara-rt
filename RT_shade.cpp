#include "stdafx.h"
#include "RT_math.h"
#include "RT_lights.h"
#include "RT_texture.h"
#include "RT_xnalara.h"
#include "RT_depth.h"
#include "RT_shade.h"

// RG00 = done
// RG01 = 

static bool enable_shading = true;
static bool enable_DM = true; // diffuse maps
static bool enable_BM = true; // bump maps
static bool enable_SM = true; // specular maps

// lighting variables
static bool k_conservation = true;
static float ambient_co[3] = { 1.0f, 1.0f, 1.0f };
static float ka = 0.50f;
static float ks = 0.10f;

#pragma region HELPER_FUNCTIONS

static vector4D<float> SampleTexture(const XNATexture& texture, float u, float v)
{
 return texture.data->sampler(texture.data, u, v);
}

template<uint32_t rows, uint32_t cols>
static vector4D<float> SampleTexture(const XNAMesh& mesh, uint32_t index, const float (&uv)[rows][cols])
{
 // sample texture given mesh, texture index, and interpolated UVs
 const XNATexture& texture = mesh.textures[index];
 return texture.data->sampler(texture.data, uv[texture.channel][0], uv[texture.channel][1]);
}

template<uint32_t rows, uint32_t cols>
static vector4D<float> SampleTexture(const XNAMesh& mesh, uint32_t index, const float (&uv)[rows][cols], float scale)
{
 // sample texture given mesh, texture index, and interpolated UVs
 const XNATexture& texture = mesh.textures[index];
 return texture.data->sampler(texture.data, scale*uv[texture.channel][0], scale*uv[texture.channel][1]);
}

template<uint32_t rows, uint32_t cols>
static vector4D<float> SampleDiffuse(const XNAMesh& mesh, uint32_t index, const float (&uv)[rows][cols])
{
 if(enable_DM) return SampleTexture(mesh, index, uv);
 vector4D<float> sample;
 sample.r = sample.g = sample.b = 0.75f;
 sample.a = 1.0f;
 return sample;
}

template<uint32_t rows, uint32_t cols>
static vector4D<float> SampleDiffuse(const XNAMesh& mesh, uint32_t index, const float (&color)[4], const float (&uv)[rows][cols])
{
 vector4D<float> sample; 
 if(enable_DM) sample = SampleTexture(mesh, index, uv);
 else {
    sample.r = sample.g = sample.b = 0.75f;
    sample.a = 1.0f;
   }
 sample[0] *= color[0];
 sample[1] *= color[1];
 sample[2] *= color[2];
 sample[3] *= color[3];
 return sample;
}

static float Saturate(float value)
{
 if(value < 0.0f) return 0.0f;
 if(value > 1.0f) return 1.0f;
 return value;
}

#pragma endregion HELPER_FUNCTIONS

#pragma region SHADING_FUNCTIONS

vector4D<float> RG00(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // if there is a texture, sample the first one
 if(mesh.n_textures) {
    const XNAVertex& v0 = mesh.verts[refs[0]];
    const XNAVertex& v1 = mesh.verts[refs[1]];
    const XNAVertex& v2 = mesh.verts[refs[2]];
    uint32_t channel = mesh.textures[0].channel;
    float ipt;
    float uv[2] = {
     u*modf(v0.uv[channel][0], &ipt) + v*modf(v1.uv[channel][0], &ipt) + w*modf(v2.uv[channel][0], &ipt),
     u*modf(v0.uv[channel][1], &ipt) + v*modf(v1.uv[channel][1], &ipt) + w*modf(v2.uv[channel][1], &ipt)
    };
    return SampleTexture(mesh.textures[0], uv[0], uv[1]);
   }

 // otherwise
 vector4D<float> sample;
 sample[0] = 0.75f;
 sample[1] = 0.75f;
 sample[2] = 0.75f;
 sample[3] = 1.00f;
 return sample;
}

vector4D<float> RG01(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // RG01
 //	7 textures: diffuse, lightmap, bumpmap, maskmap, repeater, repeater
 //	Alpha: N (RG20 = Y)
 // Posing: Y
 // Shading: Y
 // Parameters:	BumpSpecularAmount, Bump1UVScale, Bump2UVScale
 // Notes: N/A
 // XNALaraShader: DiffuseLightmapBump3

 // triangle vertices
 const XNAVertex& v0 = mesh.verts[refs[0]];
 const XNAVertex& v1 = mesh.verts[refs[1]];
 const XNAVertex& v2 = mesh.verts[refs[2]];

 // interpolated normal
 float N[3] = {
  u*v0.normal[0] + v*v1.normal[0] + w*v2.normal[0],
  u*v0.normal[1] + v*v1.normal[1] + w*v2.normal[1],
  u*v0.normal[2] + v*v1.normal[2] + w*v2.normal[2]
 };
 float N_norm = vector3D_normalize(N);

 // interpolated color
 float color[4] = {
  u*v0.color[0] + v*v1.color[0] + w*v2.color[0],
  u*v0.color[1] + v*v1.color[1] + w*v2.color[1],
  u*v0.color[2] + v*v1.color[2] + w*v2.color[2],
  u*v0.color[3] + v*v1.color[3] + w*v2.color[3]
 };

 // interpolated UV coordinates
 float ipt;
 float uv[2][2];
 for(uint32_t i = 0; i < mesh.n_channels; i++) {
     uv[i][0] = u*modf(v0.uv[i][0], &ipt) + v*modf(v1.uv[i][0], &ipt) + w*modf(v2.uv[i][0], &ipt);
     uv[i][1] = u*modf(v0.uv[i][1], &ipt) + v*modf(v1.uv[i][1], &ipt) + w*modf(v2.uv[i][1], &ipt);
    }

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, 0, color, uv);

 //
 // NORMAL MAPPING
 //

 if(enable_BM)
   {
    // face UVs
    uint32_t channel = mesh.textures[2].channel;
    vector2D<float> uv0(v0.uv[channel]);
    vector2D<float> uv1(v1.uv[channel]);
    vector2D<float> uv2(v2.uv[channel]);
   
    // UV face edges
    vector2D<float> tea = uv1 - uv0;
    vector2D<float> teb = uv2 - uv0;
   
    // you can't bump map degenerate UVs
    float scale = tea[0]*teb[1] - teb[0]*tea[1];
    if(!(std::abs(scale) < 1.0e-6f))
      {
       // interpolated tangent
       float T[4];
       T[0] = u*v0.tangent[channel][0] + v*v1.tangent[channel][0] + w*v2.tangent[channel][0];
       T[1] = u*v0.tangent[channel][1] + v*v1.tangent[channel][1] + w*v2.tangent[channel][1];
       T[2] = u*v0.tangent[channel][2] + v*v1.tangent[channel][2] + w*v2.tangent[channel][2];
       T[3] = u*v0.tangent[channel][3] + v*v1.tangent[channel][3] + w*v2.tangent[channel][3];
       float T_norm = vector3D_normalize(T);
    
       // N = T cross B (Z = X x Y)
       // T = B cross N (X = Y x Z)
       // B = N cross T (Y = Z x X)
       float B[3];
       vector3D_vector_product(B, N, T);
       B[0] *= T[3]; // for mirrored UVs
       B[1] *= T[3]; // for mirrored UVs
       B[2] *= T[3]; // for mirrored UVs
       vector3D_normalize(B);
    
       // sample textures
       auto NM_sample = SampleTexture(mesh, 2, uv);
       auto MM_sample = SampleTexture(mesh, 3, uv);
       auto R1_sample = SampleTexture(mesh, 4, uv, mesh.params.params[1]);
       auto R2_sample = SampleTexture(mesh, 5, uv, mesh.params.params[2]);
    
       // combine bumpmap colors
       // note that NM, R1, and R2 can use different UV layers, but the tangent space
       // normal mapping uses the UV layer specified by the NM texture.
       float final_bump[3];
       final_bump[0] = NM_sample.r + (R1_sample.r - 0.5f)*MM_sample.r + (R2_sample.r - 0.5f)*MM_sample.g;
       final_bump[1] = NM_sample.g + (R1_sample.g - 0.5f)*MM_sample.r + (R2_sample.g - 0.5f)*MM_sample.g;
       final_bump[2] = NM_sample.b + (R1_sample.b - 0.5f)*MM_sample.r + (R2_sample.b - 0.5f)*MM_sample.g;
    
       // remap sample to <[-1, +1],[-1, +1],[0, +1]>
       float N_unperturbed[3] = {
        final_bump[0]*2.0f - 1.0f, // r -> [0.0,1.0] so this maps to [-1.0,+1.0]
        final_bump[1]*2.0f - 1.0f, // g -> [0.0,1.0] so this maps to [-1.0,+1.0]
        final_bump[2]              // b -> [0.0,1.0] so unchanged as we cannot have negative normal (means invisible)
       };
       vector3D_normalize(N_unperturbed);
    
       // rotate normal
       N[0] = N_unperturbed[0]*N[0] + N_unperturbed[1]*B[0] + N_unperturbed[2]*T[0];
       N[1] = N_unperturbed[0]*N[1] + N_unperturbed[1]*B[1] + N_unperturbed[2]*T[1];
       N[2] = N_unperturbed[0]*N[2] + N_unperturbed[1]*B[2] + N_unperturbed[2]*T[2];
       vector3D_normalize(N);
      }
    }

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    float wo[3] = {
     -ray.direction[0],
     -ray.direction[1],
     -ray.direction[2],
    };

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = GetDirectionalLight(i);
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = true; // ShadowTest(&hitpoint[0], &wi[0]);
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(wo, &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    auto LM_sample = SampleTexture(mesh, 1, uv);
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0])*LM_sample.r;
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1])*LM_sample.g;
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2])*LM_sample.b;
   }

 // finished
 if(mesh.params.render_group == 1) DM_sample.a = 1.0f;
 return DM_sample;
}

vector4D<float> RG02(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG03(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG04(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG05(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG06(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG07(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG08(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG09(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG10(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG11(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG12(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG13(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG14(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG15(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG16(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG17(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG18(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG19(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG20(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // same as RG01 but with alpha
 return RG01(mesh, refs, ray, u, v, w);
}

vector4D<float> RG21(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG22(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // RG22
 //	7 textures: diffuse, lightmap, bumpmap, maskmap, repeater, repeater, specmap
 //	Alpha: N
 // Posing: Y
 // Shading: Y
 // Parameters:	BumpSpecularAmount, Bump1UVScale, Bump2UVScale
 // Notes: Same as RG1 but with added specular map
 // XNALaraShader: DiffuseLightmapBump3SpecularPS
// 
 // triangle vertices
 const XNAVertex& v0 = mesh.verts[refs[0]];
 const XNAVertex& v1 = mesh.verts[refs[1]];
 const XNAVertex& v2 = mesh.verts[refs[2]];

 // interpolated normal
 float N[3] = {
  u*v0.normal[0] + v*v1.normal[0] + w*v2.normal[0],
  u*v0.normal[1] + v*v1.normal[1] + w*v2.normal[1],
  u*v0.normal[2] + v*v1.normal[2] + w*v2.normal[2]
 };
 float N_norm = vector3D_normalize(N);

 // interpolated UV coordinates
 float ipt;
 float uv[2][2];
 for(uint32_t channel = 0; channel < mesh.n_channels; channel++) {
     uv[channel][0] = u*modf(v0.uv[channel][0], &ipt) + v*modf(v1.uv[channel][0], &ipt) + w*modf(v2.uv[channel][0], &ipt);
     uv[channel][1] = u*modf(v0.uv[channel][1], &ipt) + v*modf(v1.uv[channel][1], &ipt) + w*modf(v2.uv[channel][1], &ipt);
    }

 // interpolated color
 float color[3] = {
  u*v0.color[0] + v*v1.color[0] + w*v2.color[0],
  u*v0.color[1] + v*v1.color[1] + w*v2.color[1],
  u*v0.color[2] + v*v1.color[2] + w*v2.color[2]
 };

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample;
 if(enable_DM) DM_sample = SampleTexture(mesh, 0, uv);
 else DM_sample.r = DM_sample.g = DM_sample.b = 0.75f;
 DM_sample.a = 1.0f;

 // multiply by color data
 DM_sample[0] *= color[0];
 DM_sample[1] *= color[1];
 DM_sample[2] *= color[2];

 //
 // NORMAL MAPPING
 //

 // main bump map properties
 uint32_t texture_index = 2;
 uint32_t channel = mesh.textures[texture_index].channel;

 // face UVs
 vector2D<float> uv0(v0.uv[channel]);
 vector2D<float> uv1(v1.uv[channel]);
 vector2D<float> uv2(v2.uv[channel]);

 // UV face edges (you can't bump map degenerate UVs)
 vector2D<float> tea = uv1 - uv0;
 vector2D<float> teb = uv2 - uv0;
 float scale = (tea[0]*teb[1] - teb[0]*tea[1]);

 // 
 if(enable_BM && !(std::abs(scale) < 1.0e-6f))
   {
    // interpolated tangent
    float T[4];
    T[0] = u*v0.tangent[channel][0] + v*v1.tangent[channel][0] + w*v2.tangent[channel][0];
    T[1] = u*v0.tangent[channel][1] + v*v1.tangent[channel][1] + w*v2.tangent[channel][1];
    T[2] = u*v0.tangent[channel][2] + v*v1.tangent[channel][2] + w*v2.tangent[channel][2];
    T[3] = u*v0.tangent[channel][3] + v*v1.tangent[channel][3] + w*v2.tangent[channel][3];
    vector3D_normalize(T);

    // N = T cross B (Z = X x Y)
    // T = B cross N (X = Y x Z)
    // B = N cross T (Y = Z x X)
    float B[3];
    vector3D_vector_product(B, N, T);
    B[0] *= T[3]; // for mirrored UVs
    B[1] *= T[3]; // for mirrored UVs
    B[2] *= T[3]; // for mirrored UVs
    vector3D_normalize(B);

    // sample textures
    auto NM_sample = SampleTexture(mesh, 2, uv);
    auto MM_sample = SampleTexture(mesh, 3, uv);
    auto R1_sample = SampleTexture(mesh, 4, uv, mesh.params.params[1]);
    auto R2_sample = SampleTexture(mesh, 5, uv, mesh.params.params[2]);

    // combine bumpmap colors
    // note that NM, R1, and R2 can use different UV layers, but the tangent space
    // normal mapping uses the UV layer specified by the NM texture.
    float final_bump[3];
    final_bump[0] = NM_sample.r + (R1_sample.r - 0.5f)*MM_sample.r + (R2_sample.r - 0.5f)*MM_sample.g;
    final_bump[1] = NM_sample.g + (R1_sample.g - 0.5f)*MM_sample.r + (R2_sample.g - 0.5f)*MM_sample.g;
    final_bump[2] = NM_sample.b + (R1_sample.b - 0.5f)*MM_sample.r + (R2_sample.b - 0.5f)*MM_sample.g;

    // remap sample to <[-1, +1],[-1, +1],[0, +1]>
    float N_unperturbed[3] = {
     final_bump[0]*2.0f - 1.0f, // r -> [0.0,1.0] so this maps to [-1.0,+1.0]
     final_bump[1]*2.0f - 1.0f, // g -> [0.0,1.0] so this maps to [-1.0,+1.0]
     final_bump[2]              // b -> [0.0,1.0] so unchanged as we cannot have negative normal (means invisible)
    };
    vector3D_normalize(N_unperturbed);

    // rotate normal
    N[0] = N_unperturbed[0]*N[0] + N_unperturbed[1]*B[0] + N_unperturbed[2]*T[0];
    N[1] = N_unperturbed[0]*N[1] + N_unperturbed[1]*B[1] + N_unperturbed[2]*T[1];
    N[2] = N_unperturbed[0]*N[2] + N_unperturbed[1]*B[2] + N_unperturbed[2]*T[2];
    vector3D_normalize(N);
   }

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // sample textures
    auto LM_sample = SampleTexture(mesh, 1, uv); // lightmap
    auto SM_sample = SampleTexture(mesh, 6, uv); // specular map

    // wo is "from hit-point to camera"
    float wo[3] = {
     -ray.direction[0],
     -ray.direction[1],
     -ray.direction[2],
    };

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = GetDirectionalLight(i);
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = true; // ShadowTest(&hitpoint[0], &wi[0]);
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(wo, &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.x);
              specular[1] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.y);
              specular[2] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.z);
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0])*LM_sample.r;
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1])*LM_sample.g;
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2])*LM_sample.b;
   }

 // finished
 return DM_sample;
}

vector4D<float> RG23(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // RG23
 //	7 textures: diffuse, lightmap, bumpmap, maskmap, repeater, repeater, specmap
 //	Alpha: N
 // Posing: Y
 // Shading: Y
 // Parameters:	BumpSpecularAmount, Bump1UVScale, Bump2UVScale
 // Notes: Same as RG1 but with added specular map
 // XNALaraShader: DiffuseLightmapBump3SpecularPS

 // triangle vertices
 const XNAVertex& v0 = mesh.verts[refs[0]];
 const XNAVertex& v1 = mesh.verts[refs[1]];
 const XNAVertex& v2 = mesh.verts[refs[2]];

 // interpolated normal
 float N[3] = {
  u*v0.normal[0] + v*v1.normal[0] + w*v2.normal[0],
  u*v0.normal[1] + v*v1.normal[1] + w*v2.normal[1],
  u*v0.normal[2] + v*v1.normal[2] + w*v2.normal[2]
 };
 float N_norm = vector3D_normalize(N);

 // interpolated UV coordinates
 float ipt;
 float uv[2][2];
 for(uint32_t channel = 0; channel < mesh.n_channels; channel++) {
     uv[channel][0] = u*modf(v0.uv[channel][0], &ipt) + v*modf(v1.uv[channel][0], &ipt) + w*modf(v2.uv[channel][0], &ipt);
     uv[channel][1] = u*modf(v0.uv[channel][1], &ipt) + v*modf(v1.uv[channel][1], &ipt) + w*modf(v2.uv[channel][1], &ipt);
    }

 // interpolated color
 float color[4] = {
  u*v0.color[0] + v*v1.color[0] + w*v2.color[0],
  u*v0.color[1] + v*v1.color[1] + w*v2.color[1],
  u*v0.color[2] + v*v1.color[2] + w*v2.color[2],
  u*v0.color[3] + v*v1.color[3] + w*v2.color[3]
 };

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample;
 if(enable_DM) DM_sample = SampleTexture(mesh, 0, uv);
 else {
    DM_sample.r = DM_sample.g = DM_sample.b = 0.75f;
    DM_sample.a = 1.0f;
   }

 // multiply by color data
 DM_sample[0] *= color[0];
 DM_sample[1] *= color[1];
 DM_sample[2] *= color[2];
 DM_sample[3] *= color[3];

 //
 // NORMAL MAPPING
 //

 // main bump map properties
 uint32_t texture_index = 2;
 uint32_t channel = mesh.textures[texture_index].channel;

 // face UVs
 vector2D<float> uv0(v0.uv[channel]);
 vector2D<float> uv1(v1.uv[channel]);
 vector2D<float> uv2(v2.uv[channel]);

 // UV face edges (you can't bump map degenerate UVs)
 vector2D<float> tea = uv1 - uv0;
 vector2D<float> teb = uv2 - uv0;
 float scale = (tea[0]*teb[1] - teb[0]*tea[1]);

 // 
 if(enable_BM && !(std::abs(scale) < 1.0e-6f))
   {
    // interpolated tangent
    float T[4];
    T[0] = u*v0.tangent[channel][0] + v*v1.tangent[channel][0] + w*v2.tangent[channel][0];
    T[1] = u*v0.tangent[channel][1] + v*v1.tangent[channel][1] + w*v2.tangent[channel][1];
    T[2] = u*v0.tangent[channel][2] + v*v1.tangent[channel][2] + w*v2.tangent[channel][2];
    T[3] = u*v0.tangent[channel][3] + v*v1.tangent[channel][3] + w*v2.tangent[channel][3];
    vector3D_normalize(T);

    // N = T cross B (Z = X x Y)
    // T = B cross N (X = Y x Z)
    // B = N cross T (Y = Z x X)
    float B[3];
    vector3D_vector_product(B, N, T);
    B[0] *= T[3]; // for mirrored UVs
    B[1] *= T[3]; // for mirrored UVs
    B[2] *= T[3]; // for mirrored UVs
    vector3D_normalize(B);

    // sample textures
    auto NM_sample = SampleTexture(mesh, 2, uv);
    auto MM_sample = SampleTexture(mesh, 3, uv);
    auto R1_sample = SampleTexture(mesh, 4, uv, mesh.params.params[1]);
    auto R2_sample = SampleTexture(mesh, 5, uv, mesh.params.params[2]);

    // combine bumpmap colors
    // note that NM, R1, and R2 can use different UV layers, but the tangent space
    // normal mapping uses the UV layer specified by the NM texture.
    float final_bump[3];
    final_bump[0] = NM_sample.r + (R1_sample.r - 0.5f)*MM_sample.r + (R2_sample.r - 0.5f)*MM_sample.g;
    final_bump[1] = NM_sample.g + (R1_sample.g - 0.5f)*MM_sample.r + (R2_sample.g - 0.5f)*MM_sample.g;
    final_bump[2] = NM_sample.b + (R1_sample.b - 0.5f)*MM_sample.r + (R2_sample.b - 0.5f)*MM_sample.g;

    // remap sample to <[-1, +1],[-1, +1],[0, +1]>
    float N_unperturbed[3] = {
     final_bump[0]*2.0f - 1.0f, // r -> [0.0,1.0] so this maps to [-1.0,+1.0]
     final_bump[1]*2.0f - 1.0f, // g -> [0.0,1.0] so this maps to [-1.0,+1.0]
     final_bump[2]              // b -> [0.0,1.0] so unchanged as we cannot have negative normal (means invisible)
    };
    vector3D_normalize(N_unperturbed);

    // rotate normal
    N[0] = N_unperturbed[0]*N[0] + N_unperturbed[1]*B[0] + N_unperturbed[2]*T[0];
    N[1] = N_unperturbed[0]*N[1] + N_unperturbed[1]*B[1] + N_unperturbed[2]*T[1];
    N[2] = N_unperturbed[0]*N[2] + N_unperturbed[1]*B[2] + N_unperturbed[2]*T[2];
    vector3D_normalize(N);
   }

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // sample textures
    auto LM_sample = SampleTexture(mesh, 1, uv); // lightmap
    auto SM_sample = SampleTexture(mesh, 6, uv); // specular map

    // wo is "from hit-point to camera"
    float wo[3] = {
     -ray.direction[0],
     -ray.direction[1],
     -ray.direction[2],
    };

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = GetDirectionalLight(i);
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = true; // ShadowTest(&hitpoint[0], &wi[0]);
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(wo, &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.x);
              specular[1] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.y);
              specular[2] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.z);
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0])*LM_sample.r;
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1])*LM_sample.g;
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2])*LM_sample.b;
   }

 // finished
 return DM_sample;
}

vector4D<float> RG24(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // RG24
 //	4 textures: diffuse, lightmap, bumpmap, specmap
 //	Alpha: N
 // Posing: Y
 // Shading: Y
 // Parameters:	BumpSpecularAmount
 // Notes: Same as RG2 but with added specular map
 // XNALaraShader: DiffuseLightmapBumpSpecularPS_1

 // triangle vertices
 const XNAVertex& v0 = mesh.verts[refs[0]];
 const XNAVertex& v1 = mesh.verts[refs[1]];
 const XNAVertex& v2 = mesh.verts[refs[2]];

 // interpolated normal
 float N[3] = {
  u*v0.normal[0] + v*v1.normal[0] + w*v2.normal[0],
  u*v0.normal[1] + v*v1.normal[1] + w*v2.normal[1],
  u*v0.normal[2] + v*v1.normal[2] + w*v2.normal[2]
 };
 float N_norm = vector3D_normalize(N);

 // interpolated UV coordinates
 float ipt;
 float uv[2][2];
 for(uint32_t channel = 0; channel < mesh.n_channels; channel++) {
     uv[channel][0] = u*modf(v0.uv[channel][0], &ipt) + v*modf(v1.uv[channel][0], &ipt) + w*modf(v2.uv[channel][0], &ipt);
     uv[channel][1] = u*modf(v0.uv[channel][1], &ipt) + v*modf(v1.uv[channel][1], &ipt) + w*modf(v2.uv[channel][1], &ipt);
    }

 // interpolated color (alpha not needed)
 float color[3] = {
  u*v0.color[0] + v*v1.color[0] + w*v2.color[0],
  u*v0.color[1] + v*v1.color[1] + w*v2.color[1],
  u*v0.color[2] + v*v1.color[2] + w*v2.color[2]
 };

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample;
 if(enable_DM) DM_sample = SampleTexture(mesh, 0, uv);
 else DM_sample.r = DM_sample.g = DM_sample.b = 0.75f;
 DM_sample.a = 1.0f;

 // multiply by color data
 DM_sample[0] *= color[0];
 DM_sample[1] *= color[1];
 DM_sample[2] *= color[2];

 //
 // NORMAL MAPPING
 //

 // main bump map properties
 uint32_t texture_index = 2;
 uint32_t channel = mesh.textures[texture_index].channel;

 // face UVs
 vector2D<float> uv0(v0.uv[channel]);
 vector2D<float> uv1(v1.uv[channel]);
 vector2D<float> uv2(v2.uv[channel]);

 // UV face edges (you can't bump map degenerate UVs)
 vector2D<float> tea = uv1 - uv0;
 vector2D<float> teb = uv2 - uv0;
 float scale = (tea[0]*teb[1] - teb[0]*tea[1]);

 // 
 if(enable_BM && !(std::abs(scale) < 1.0e-6f))
   {
    // interpolated tangent
    float T[4];
    T[0] = u*v0.tangent[channel][0] + v*v1.tangent[channel][0] + w*v2.tangent[channel][0];
    T[1] = u*v0.tangent[channel][1] + v*v1.tangent[channel][1] + w*v2.tangent[channel][1];
    T[2] = u*v0.tangent[channel][2] + v*v1.tangent[channel][2] + w*v2.tangent[channel][2];
    T[3] = u*v0.tangent[channel][3] + v*v1.tangent[channel][3] + w*v2.tangent[channel][3];
    vector3D_normalize(T);

    // N = T cross B (Z = X x Y)
    // T = B cross N (X = Y x Z)
    // B = N cross T (Y = Z x X)
    float B[3];
    vector3D_vector_product(B, N, T);
    B[0] *= T[3]; // for mirrored UVs
    B[1] *= T[3]; // for mirrored UVs
    B[2] *= T[3]; // for mirrored UVs
    vector3D_normalize(B);

    // sample texture and remap sample to <[-1, +1],[-1, +1],[0, +1]>
    auto NM_sample = SampleTexture(mesh, 2, uv);
    float N_unperturbed[3] = {
     NM_sample.r*2.0f - 1.0f, // r -> [0.0,1.0] so this maps to [-1.0,+1.0]
     NM_sample.g*2.0f - 1.0f, // g -> [0.0,1.0] so this maps to [-1.0,+1.0]
     NM_sample.b              // b -> [0.0,1.0] so unchanged as we cannot have negative normal (means invisible)
    };
    vector3D_normalize(N_unperturbed);

    // rotate normal
    N[0] = N_unperturbed[0]*N[0] + N_unperturbed[1]*B[0] + N_unperturbed[2]*T[0];
    N[1] = N_unperturbed[0]*N[1] + N_unperturbed[1]*B[1] + N_unperturbed[2]*T[1];
    N[2] = N_unperturbed[0]*N[2] + N_unperturbed[1]*B[2] + N_unperturbed[2]*T[2];
    vector3D_normalize(N);
   }

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // sample textures
    auto LM_sample = SampleTexture(mesh, 1, uv); // lightmap
    auto SM_sample = SampleTexture(mesh, 3, uv); // specular map

    // wo is "from hit-point to camera"
    float wo[3] = {
     -ray.direction[0],
     -ray.direction[1],
     -ray.direction[2],
    };

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = GetDirectionalLight(i);
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = true; // ShadowTest(&hitpoint[0], &wi[0]);
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(wo, &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.x);
              specular[1] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.y);
              specular[2] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.z);
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0])*LM_sample.r;
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1])*LM_sample.g;
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2])*LM_sample.b;
   }

 // finished
 return DM_sample;
}

vector4D<float> RG25(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // RG25
 //	4 textures: diffuse, lightmap, bumpmap, specmap
 //	Alpha: Y
 // Posing: Y
 // Shading: Y
 // Parameters:	BumpSpecularAmount
 // Notes: Same as RG24 but with alpha
 // XNALaraShader: DiffuseLightmapBumpSpecularPS_1

 // triangle vertices
 const XNAVertex& v0 = mesh.verts[refs[0]];
 const XNAVertex& v1 = mesh.verts[refs[1]];
 const XNAVertex& v2 = mesh.verts[refs[2]];

 // interpolated normal
 float N[3] = {
  u*v0.normal[0] + v*v1.normal[0] + w*v2.normal[0],
  u*v0.normal[1] + v*v1.normal[1] + w*v2.normal[1],
  u*v0.normal[2] + v*v1.normal[2] + w*v2.normal[2]
 };
 float N_norm = vector3D_normalize(N);

 // interpolated UV coordinates
 float ipt;
 float uv[2][2];
 for(uint32_t channel = 0; channel < mesh.n_channels; channel++) {
     uv[channel][0] = u*modf(v0.uv[channel][0], &ipt) + v*modf(v1.uv[channel][0], &ipt) + w*modf(v2.uv[channel][0], &ipt);
     uv[channel][1] = u*modf(v0.uv[channel][1], &ipt) + v*modf(v1.uv[channel][1], &ipt) + w*modf(v2.uv[channel][1], &ipt);
    }

 // interpolated color
 float color[4] = {
  u*v0.color[0] + v*v1.color[0] + w*v2.color[0],
  u*v0.color[1] + v*v1.color[1] + w*v2.color[1],
  u*v0.color[2] + v*v1.color[2] + w*v2.color[2],
  u*v0.color[3] + v*v1.color[3] + w*v2.color[3]
 };

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample;
 if(enable_DM) DM_sample = SampleTexture(mesh, 0, uv);
 else {
    DM_sample.r = DM_sample.g = DM_sample.b = 0.75f;
    DM_sample.a = 1.0f;
   }

 // multiply by color data
 DM_sample[0] *= color[0];
 DM_sample[1] *= color[1];
 DM_sample[2] *= color[2];
 DM_sample[3] *= color[3];

 //
 // NORMAL MAPPING
 //

 // main bump map properties
 uint32_t texture_index = 2;
 uint32_t channel = mesh.textures[texture_index].channel;

 // face UVs
 vector2D<float> uv0(v0.uv[channel]);
 vector2D<float> uv1(v1.uv[channel]);
 vector2D<float> uv2(v2.uv[channel]);

 // UV face edges (you can't bump map degenerate UVs)
 vector2D<float> tea = uv1 - uv0;
 vector2D<float> teb = uv2 - uv0;
 float scale = (tea[0]*teb[1] - teb[0]*tea[1]);

 // 
 if(enable_BM && !(std::abs(scale) < 1.0e-6f))
   {
    // interpolated tangent
    float T[4];
    T[0] = u*v0.tangent[channel][0] + v*v1.tangent[channel][0] + w*v2.tangent[channel][0];
    T[1] = u*v0.tangent[channel][1] + v*v1.tangent[channel][1] + w*v2.tangent[channel][1];
    T[2] = u*v0.tangent[channel][2] + v*v1.tangent[channel][2] + w*v2.tangent[channel][2];
    T[3] = u*v0.tangent[channel][3] + v*v1.tangent[channel][3] + w*v2.tangent[channel][3];
    vector3D_normalize(T);

    // N = T cross B (Z = X x Y)
    // T = B cross N (X = Y x Z)
    // B = N cross T (Y = Z x X)
    float B[3];
    vector3D_vector_product(B, N, T);
    B[0] *= T[3]; // for mirrored UVs
    B[1] *= T[3]; // for mirrored UVs
    B[2] *= T[3]; // for mirrored UVs
    vector3D_normalize(B);

    // sample texture and remap sample to <[-1, +1],[-1, +1],[0, +1]>
    auto NM_sample = SampleTexture(mesh, 2, uv);
    float N_unperturbed[3] = {
     NM_sample.r*2.0f - 1.0f, // r -> [0.0,1.0] so this maps to [-1.0,+1.0]
     NM_sample.g*2.0f - 1.0f, // g -> [0.0,1.0] so this maps to [-1.0,+1.0]
     NM_sample.b              // b -> [0.0,1.0] so unchanged as we cannot have negative normal (means invisible)
    };
    vector3D_normalize(N_unperturbed);

    // rotate normal
    N[0] = N_unperturbed[0]*N[0] + N_unperturbed[1]*B[0] + N_unperturbed[2]*T[0];
    N[1] = N_unperturbed[0]*N[1] + N_unperturbed[1]*B[1] + N_unperturbed[2]*T[1];
    N[2] = N_unperturbed[0]*N[2] + N_unperturbed[1]*B[2] + N_unperturbed[2]*T[2];
    vector3D_normalize(N);
   }

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // sample textures
    auto LM_sample = SampleTexture(mesh, 1, uv); // lightmap
    auto SM_sample = SampleTexture(mesh, 3, uv); // specular map

    // wo is "from hit-point to camera"
    float wo[3] = {
     -ray.direction[0],
     -ray.direction[1],
     -ray.direction[2],
    };

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = GetDirectionalLight(i);
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = true; // ShadowTest(&hitpoint[0], &wi[0]);
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(wo, &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.x);
              specular[1] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.y);
              specular[2] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.z);
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0])*LM_sample.r;
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1])*LM_sample.g;
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2])*LM_sample.b;
   }

 // finished
 return DM_sample;
}

vector4D<float> RG26(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG27(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // don't show for now, used for reflection
 vector4D<float> rv;
 rv.r = rv.g = rv.b = 0.0f;
 rv.a = 0.0f;
 return rv; 
 // return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG28(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG29(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 // don't show for now, used for reflection
 vector4D<float> rv;
 rv.r = rv.g = rv.b = 0.0f;
 rv.a = 0.0f;
 return rv; 
 // return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG30(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG31(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG32(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG33(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG34(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG35(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG36(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG37(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG38(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG39(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG40(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG41(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG42(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG43(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG_thorgear(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG_thorglow(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

vector4D<float> RG_handguns(const XNAMesh& mesh, const uint32_t* refs, const ray3D& ray, float u, float v, float w)
{
 return RG00(mesh, refs, ray, u, v, w);
}

#pragma endregion SHADING_FUNCTIONS