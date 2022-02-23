#include "stdafx.h"
#include "RT_math.h"
#include "RT_xnalara.h"
#include "RT_obsolete.h"
#include "RT_raytrace.h"
using namespace std;

bool Render(const wchar_t* filename)
{
 // filename properties
 auto fullpath = std::filesystem::path(filename);
 STDSTRINGW pathname = fullpath.parent_path(); // path no filename no extension
 STDSTRINGW shrtname = fullpath.stem(); // filename no extension
 STDSTRINGW shrtpath = fullpath.parent_path().filename(); // short parent pathname

 // load model
 if(!LoadModel(filename)) return false;

 // initialize default camera and lights
 InitCamera();
 SetCameraPosition(0.0f, 1.0f, 4.0f);
 InitLights();

 // set rendering variables
 SetClearColor(vector4D<float>(0.0f, 0.4f, 0.7f, 1.0f));
 SetJitterDims(4);

 // render
 SetTargetDims(1920, 1080);
 std::cout << "1920 x 1080" << std::endl;
 STDSTRINGW outname = shrtpath;
 outname += L"_1920x1080.bmp";
 Trace(outname.c_str());

 return true;
}

int Run(void)
{
 // having trouble with this data: v0 = 1.94833, 24.5353, 3.01768
 // having trouble with this data: v1 = 4.37479, 24.7778, 1.29473
 // having trouble with this data: v2 = 3.69471, 23.2584, 2.71028
 // having trouble with this data: uv0 = 0, 1
 // having trouble with this data: uv1 = 0, 0
 // having trouble with this data: uv2 = 0.5, 0.5

 // load model into ray tracer
 // Render(L"D:\\XNALara\\Models\\original\\albino_spider\\albino_spider.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\alister\\alister.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\amanda_heavy\\amanda_heavy.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\amanda_light\\amanda_light.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\amelia\\amelia.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\braid\\braid.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\chompy\\generic_item.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\croft_manor_back\\croft_manor_back.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\croft_manor_hall\\croft_manor_hall.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\croft_manor_hall_lq\\croft_manor_hall_lq.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\diver_captain\\diver_captain.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\doppelganger\\doppelganger.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\dummy\\dummy.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\excalibur\\excalibur.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\foliage_tree1\\foliage_tree1.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\foliage_tree2\\foliage_tree2.mesh"); // UV mapping problem with sampler
 // Render(L"D:\\XNALara\\Models\\original\\foliage_tree3\\foliage_tree3.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\foliage_tree4\\foliage_tree4.mesh"); // UV mapping problem with sampler
 // Render(L"D:\\XNALara\\Models\\original\\glass_cage\\glass_cage.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\glasses\\glasses.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_brute_minion\\gol_brute_minion.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_chompy\\gol_chompy.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_doppelganger\\gol_doppelganger.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_fire_lizard\\gol_fire_lizard.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_fire_shaman\\gol_fire_shaman.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_lara\\gol_lara.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_lara_biker\\gol_lara_biker.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_lara_jungle_shorts\\gol_lara_jungle_shorts.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_lara_legend\\gol_lara_legend.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_poison_gatekeeper\\gol_poison_gatekeeper.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_poison_lizard\\gol_poison_lizard.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_poison_minion\\gol_poison_minion.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_poison_shaman\\gol_poison_shaman.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_shield_demon\\gol_shield_demon.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_shield_gatekeeper\\gol_shield_gatekeeper.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_small_minion\\gol_small_minion.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_totec\\gol_totec.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_trex\\gol_trex.mesh");
 Render(L"D:\\XNALara\\Models\\original\\gol_xolotl\\gol_xolotl.mesh");

 // Render(L"D:\\XNALara\\Models\\original\\lara_bathing_suit\\lara_bathing_suit.mesh");
 // Render(L"C:\\Users\\moby_\\Desktop\\xnalara\\models\\Alcina Dimitrescu\\xps.xps");
 // Render(L"C:\\Users\\moby_\\Desktop\\xnalara\\models\\RG17\\xps.xps");

 return 0;
}

int main()
{
 // important! initialize classic model information
 InitClassicModels();
 auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
 int retval = Run();
 CoUninitialize();
 return retval;
}