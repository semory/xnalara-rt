#ifndef __RT_LIGHTS_H
#define __RT_LIGHTS_H

// default XNALara/XPS light
// angle horizontal = 315 (0 = -x-axis, 90 = +z-axis, 180 = +x-axis, 270 = -z-axis, 360 = -x-axis)
// angle vertical = 35 (-90, light on bottom, 0 = no shadow, 90 = light on top)
// intensity = 1.1
// shadow depth = 0.4

// <horz, vert>
// this rotates about z-axis, changing vert
// <0, 90> = <0, -1, 0> // light above
// <0, 0> = <-1, 0, 0> // light on side
// <0, -90> = <0, 1, 0> // light below

struct DirectionalLight {
 bool enabled;
 float angle_horz;
 float angle_vert;
 float direction[3];
 float color[4];
 float intensity;
 float shadow_depth;
};

void ComputeLightDirection(float* D, float horz, float vert);
void InitLights(void);
const DirectionalLight& GetDirectionalLight(uint32_t index);

#endif
