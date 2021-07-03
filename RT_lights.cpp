#include "stdafx.h"
#include "RT_math.h"
#include "RT_lights.h"

static DirectionalLight lightlist[3];

void ComputeLightDirection(float* D, float horz, float vert)
{
 float r1 = radians(horz);
 float r2 = radians(vert);
 float m[16];
 RotationMatrix4D_YZ(m, r1, -r2); // order is Z (vert) then Y (horz)
 float v[3] = { 1.0f, 0.0f, 0.0f };
 float x[3];
 Matrix4DVector3DMul(x, m, v);
 vector3D_normalize(x);
 D[0] = -x[0];
 D[1] = -x[1];
 D[2] = -x[2];
}

void InitLights(void)
{
 // light #1 (XNALara 9.7.8 default)
 lightlist[0].enabled = true;
 lightlist[0].angle_horz = 315.0f;
 lightlist[0].angle_vert =  35.0f;
 lightlist[0].color[0] = lightlist[0].color[1] = lightlist[0].color[2] = lightlist[0].color[3] = 1.0f;
 lightlist[0].intensity = 1.1f;
 lightlist[0].shadow_depth = 0.4f;
 ComputeLightDirection(lightlist[0].direction, lightlist[0].angle_horz, lightlist[0].angle_vert);

 // light #2 (XNALara 9.7.8 default)
 // XNALara 9.7.8 has all lights enabled by default; disabled lights have intensity set to 0.
 lightlist[1].enabled = false;
 lightlist[1].angle_horz = 225.0f;
 lightlist[1].angle_vert = 35.0f;
 lightlist[1].color[0] = lightlist[1].color[1] = lightlist[1].color[2] = lightlist[1].color[3] = 1.0f;
 lightlist[1].intensity = 0.0f;
 lightlist[1].shadow_depth = 0.4f;
 ComputeLightDirection(lightlist[1].direction, lightlist[1].angle_horz, lightlist[1].angle_vert);

 // light #3 (XNALara 9.7.8 default)
 // XNALara 9.7.8 has all lights enabled by default; disabled lights have intensity set to 0.
 lightlist[2].enabled = false;
 lightlist[2].angle_horz = 180.0f;
 lightlist[2].angle_vert = 90.0f;
 lightlist[2].color[0] = lightlist[2].color[1] = lightlist[2].color[2] = lightlist[2].color[3] = 1.0f;
 lightlist[2].intensity = 0.0f;
 lightlist[2].shadow_depth = 0.4f;
 ComputeLightDirection(lightlist[2].direction, lightlist[2].angle_horz, lightlist[2].angle_vert);
}

const DirectionalLight& GetDirectionalLight(uint32_t index)
{
 return lightlist[index];
}