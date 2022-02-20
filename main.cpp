#include "stdafx.h"
#include "RT_math.h"
#include "RT_xnalara.h"
#include "RT_obsolete.h"
#include "RT_raytrace.h"
using namespace std;

int Run(void)
{
 // load classic model
 XNAModel model;
 // LoadXNAMeshBin(L"D:\\XNALara\\Models\\original\\albino_spider\\albino_spider.mesh", &model);
 // LoadXNAMeshBin(L"D:\\XNALara\\Models\\original\\alister\\alister.mesh", &model);
 // LoadXNAMeshBin(L"D:\\XNALara\\Models\\original\\amanda_heavy\\amanda_heavy.mesh", &model);
 // LoadXNAMeshBin(L"D:\\XNALara\\Models\\original\\amanda_light\\amanda_light.mesh", &model);
 // LoadXNAMeshBin(L"D:\\XNALara\\Models\\original\\lara_bathing_suit\\lara_bathing_suit.mesh", &model);

 // load model into ray tracer
 // if(!LoadModel(L"D:\\XNALara\\Models\\original\\albino_spider\\albino_spider.mesh")) return 0;
 // if(!LoadModel(L"D:\\XNALara\\Models\\original\\alister\\alister.mesh")) return 0;
 // if(!LoadModel(L"D:\\XNALara\\Models\\original\\amanda_heavy\\amanda_heavy.mesh")) return 0;
 // if(!LoadModel(L"D:\\XNALara\\Models\\original\\amanda_light\\amanda_light.mesh")) return 0;
 if(!LoadModel(L"D:\\XNALara\\Models\\original\\lara_bathing_suit\\lara_bathing_suit.mesh")) return 0;
 // if(!LoadModel(L"C:\\Users\\moby_\\Desktop\\xnalara\\models\\Alcina Dimitrescu\\xps.xps"))
 // if(!LoadModel(L"C:\\Users\\moby_\\Desktop\\xnalara\\models\\RG17\\xps.xps"))

 // initialize default camera and lights
 InitCamera();
 // SetCameraPosition(0.0f, 2.6f, 1.3f);
 SetCameraPosition(0.0f, 1.0f, 4.0f);
 InitLights();

 // set render target dimensions
 SetClearColor(vector4D<float>(0.0f, 0.4f, 0.7f, 1.0f));
 SetJitterDims(4);

 // render
 // cout << " 240 x  135" << endl; SetTargetDims( 240,  135); Trace(L"test-240x135.bmp");
 // cout << " 480 x  270" << endl; SetTargetDims( 480,  270); Trace(L"test-480x270.bmp");
 // cout << " 720 x  405" << endl; SetTargetDims( 720,  405); Trace(L"test-720x405.bmp");
 // cout << "1080 x  607" << endl; SetTargetDims(1080,  607); Trace(L"test-1080x607.bmp");
 cout << "1920 x 1080" << endl; SetTargetDims(1920, 1080); Trace(L"test-1920x1080a.bmp");
 // cout << "7680 x 4320" << endl; SetTargetDims(7680, 4320); Trace(L"test-7680x4320.bmp");

 return 0;
}

int main()
{
 // important! initialize classic model information
 InitClassicModels();
 // run application
 auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
 int retval = Run();
 CoUninitialize();
 return retval;

 return 0;
}