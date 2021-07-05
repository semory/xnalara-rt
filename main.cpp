#include "stdafx.h"
#include "RT_math.h"
#include "RT_raytrace.h"
using namespace std;

int main()
{
 // load model into ray tracer
 if(!LoadModel(L"C:\\Users\\semory\\Desktop\\DESKTOP\\xnalara\\Alcina Dimitrescu\\xps.xps"))
 // if(!LoadModel(L"C:\\Users\\semory\\Desktop\\DESKTOP\\xnalara\\RG17\\xps.xps"))
    return 0;

 // initialize default camera and lights
 InitCamera();
 InitLights();

 // set render target dimensions
 SetClearColor(vector4D<float>(0.0f, 0.0f, 0.0f, 0.0f));
 SetTargetDims(1920, 1080);
 SetJitterDims(4);

 // render
 Trace(L"test.bmp");
 return 0;
}