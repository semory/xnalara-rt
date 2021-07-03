#ifndef __RT_RAYTRACE_H
#define __RT_RAYTRACE_H

// timing functions
bool SetTraceTiming(bool state);
bool GetTraceTiming(void);

// camera functions
void InitCamera(void);

// lighting functions
void InitLights2(void);
void ResetLight(int index);

// shading functions
bool SetShadingState(bool state);
bool GetShadingState(void);

// model functions
bool LoadModel(const wchar_t* filename);

// ray tracing functions
void SetClearColor(const vector4D<float>& color);
void SetTargetDims(uint32_t dx, uint32_t dy);
void SetJitterDims(uint32_t n);
bool Trace(const wchar_t* filename);

// private ray tracing functions
void RefreshJitterMatrix(void);
void InitDepthTest(float tn, float tf);
void UpdateDepthTest(const vector4D<float>& color, float dt, uint8_t alpha);
vector4D<float> BlendPixels(void);

// shader functions
struct XNAShaderData;
vector4D<float> RG00(const XNAShaderData* input);
vector4D<float> RG01(const XNAShaderData* input);
vector4D<float> RG02(const XNAShaderData* input);
vector4D<float> RG03(const XNAShaderData* input);
vector4D<float> RG04(const XNAShaderData* input);
vector4D<float> RG05(const XNAShaderData* input);
vector4D<float> RG06(const XNAShaderData* input);
vector4D<float> RG07(const XNAShaderData* input);
vector4D<float> RG08(const XNAShaderData* input);
vector4D<float> RG09(const XNAShaderData* input);
vector4D<float> RG10(const XNAShaderData* input);
vector4D<float> RG11(const XNAShaderData* input);
vector4D<float> RG12(const XNAShaderData* input);
vector4D<float> RG13(const XNAShaderData* input);
vector4D<float> RG14(const XNAShaderData* input);
vector4D<float> RG15(const XNAShaderData* input);
vector4D<float> RG16(const XNAShaderData* input);
vector4D<float> RG17(const XNAShaderData* input);
vector4D<float> RG18(const XNAShaderData* input);
vector4D<float> RG19(const XNAShaderData* input);
vector4D<float> RG20(const XNAShaderData* input);
vector4D<float> RG21(const XNAShaderData* input);
vector4D<float> RG22(const XNAShaderData* input);
vector4D<float> RG23(const XNAShaderData* input);
vector4D<float> RG24(const XNAShaderData* input);
vector4D<float> RG25(const XNAShaderData* input);
vector4D<float> RG26(const XNAShaderData* input);
vector4D<float> RG27(const XNAShaderData* input);
vector4D<float> RG28(const XNAShaderData* input);
vector4D<float> RG29(const XNAShaderData* input);
vector4D<float> RG30(const XNAShaderData* input);
vector4D<float> RG31(const XNAShaderData* input);
vector4D<float> RG32(const XNAShaderData* input);
vector4D<float> RG33(const XNAShaderData* input);
vector4D<float> RG34(const XNAShaderData* input);
vector4D<float> RG35(const XNAShaderData* input);
vector4D<float> RG36(const XNAShaderData* input);
vector4D<float> RG37(const XNAShaderData* input);
vector4D<float> RG38(const XNAShaderData* input);
vector4D<float> RG39(const XNAShaderData* input);
vector4D<float> RG40(const XNAShaderData* input);
vector4D<float> RG41(const XNAShaderData* input);
vector4D<float> RG42(const XNAShaderData* input);
vector4D<float> RG43(const XNAShaderData* input);
vector4D<float> RG_thorgear(const XNAShaderData* input);
vector4D<float> RG_thorglow(const XNAShaderData* input);
vector4D<float> RG_handguns(const XNAShaderData* input);

#endif
