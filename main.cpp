#include "stdafx.h"
#include "RT_math.h"
#include "RT_dds.h"
#include "RT_xnalara.h"
#include "RT_raytrace.h"
using namespace std;

int main()
{
 //ConvertBMPToDDS(L"C:\\Users\\semory\\Desktop\\DESKTOP\\xnalara\\Alcina Dimitrescu\\dim_face1_diff.bmp");

 // load model into ray tracer
 if(!LoadModel(L"C:\\Users\\semory\\Desktop\\DESKTOP\\xnalara\\Alcina Dimitrescu\\xps.xps"))
 // if(!LoadModel(L"C:\\Users\\semory\\Desktop\\DESKTOP\\xnalara\\RG17\\xps.xps"))
    return 0;

 // initialize default camera and lights
 InitCamera();
 SetCameraPosition(0.0f, 2.6f, 1.3f);
 InitLights();

 // set render target dimensions
 SetClearColor(vector4D<float>(0.0f, 0.0f, 0.0f, 0.0f));
 SetJitterDims(4);

 // render
 // cout << " 240 x  135" << endl; SetTargetDims( 240,  135); Trace(L"test-240x135.bmp");
 // cout << " 480 x  270" << endl; SetTargetDims( 480,  270); Trace(L"test-480x270.bmp");
 // cout << " 720 x  405" << endl; SetTargetDims( 720,  405); Trace(L"test-720x405.bmp");
 // cout << "1080 x  607" << endl; SetTargetDims(1080,  607); Trace(L"test-1080x607.bmp");
 cout << "1920 x 1080" << endl; SetTargetDims(1920, 1080); Trace(L"test-1920x1080a.bmp");
 // cout << "7680 x 4320" << endl; SetTargetDims(7680, 4320); Trace(L"test-7680x4320.bmp");

 SetCameraPosition(0.0f, 1.5f, 6.0f);
 cout << "1920 x 1080" << endl; SetTargetDims(1920, 1080); Trace(L"test-1920x1080b.bmp");

 return 0;
}