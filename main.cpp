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
 // Render(L"D:\\XNALara\\Models\\original\\ghalali_key\\ghalali_key.obj");
 // Render(L"D:\\XNALara\\Models\\original\\glass_cage\\glass_cage.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\glasses\\glasses.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_brute_minion\\gol_brute_minion.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_chompy\\gol_chompy.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_doppelganger\\gol_doppelganger.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_fire_lizard\\gol_fire_lizard.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_fire_shaman\\gol_fire_shaman.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_lara\\gol_lara.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_lara_biker\\gol_lara_biker.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_lara_jungle_shorts\\gol_lara_jungle_shorts.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_lara_legend\\gol_lara_legend.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_poison_gatekeeper\\gol_poison_gatekeeper.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_poison_lizard\\gol_poison_lizard.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_poison_minion\\gol_poison_minion.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_poison_shaman\\gol_poison_shaman.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_shield_demon\\gol_shield_demon.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_shield_gatekeeper\\gol_shield_gatekeeper.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_small_minion\\gol_small_minion.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_totec\\gol_totec.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_trex\\gol_trex.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\gol_xolotl\\gol_xolotl.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\grapple\\grapple.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\hair_alister\\hair_alister.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\hair_amanda\\hair_amanda.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\hair_natla\\hair_natla.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\helicopter\\helicopter.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\jellyfish\\jellyfish.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\knight_thrall\\knight_thrall.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\kraken\\kraken.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\laptop\\model.obj");
 // Render(L"D:\\XNALara\\Models\\original\\lara_bathing_suit\\lara_bathing_suit.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_bathing_suit_nogear\\lara_bathing_suit_nogear.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_casual\\lara_casual.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_bikini_blackwhite\\lara_dlc_bikini_blackwhite.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_bikini_blue\\lara_dlc_bikini_blue.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_bikini_camo\\lara_dlc_bikini_camo.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_bikini_yellow\\lara_dlc_bikini_yellow.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_casual_explorer\\lara_dlc_casual_explorer.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_classic_fixed\\lara_dlc_classic.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_designer_fashion_winner\\lara_dlc_designer_fashion_winner.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_designer_peoples_choice\\lara_dlc_designer_peoples_choice.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_drysuit_red\\lara_dlc_drysuit_red.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_jungle_shorts_grey\\lara_dlc_jungle_shorts_grey.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_legend_fixed\\lara_dlc_legend.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_dlc_wetsuit_blue\\lara_dlc_wetsuit_blue.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_drysuit\\lara_drysuit.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_jungle_heavy\\lara_jungle_heavy.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_jungle_pants\\lara_jungle_pants.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_jungle_shorts\\lara_jungle_shorts.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_snow_heavy\\lara_snow_heavy.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_snow_light\\lara_snow_light.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_wetsuit\\lara_wetsuit.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\lara_wetsuit_nogear\\lara_wetsuit_nogear.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\legend_peru_village\\model.obj");
 // Render(L"D:\\XNALara\\Models\\original\\mayan_thrall\\mayan_thrall.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\mercenary\\mercenary.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\mjolnir\\mjolnir.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\motorcycle\\motorcycle.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\naga_green\\naga_green.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\naga_red\\naga_red.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\natla\\natla.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\natla_nowings\\natla_nowings.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\panther\\panther.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\panther_thrall\\panther_thrall.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\pda\\pda.obj");
 // Render(L"D:\\XNALara\\Models\\original\\poacher\\poacher.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\ponytail\\ponytail.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\recorder\\recorder.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\scion1\\scion1.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\scion2\\scion2.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\scion3\\scion3.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\shark_blue\\shark_blue.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\shark_greenland\\shark_greenland.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\ship\\ship.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\ship_natla_room\\ship_natla_room.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\silenthill3_chapel\\model.obj");
 // Render(L"D:\\XNALara\\Models\\original\\skydome_thailand_sea\\skydome_thailand_sea.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\specular_example\\model.obj");
 // Render(L"D:\\XNALara\\Models\\original\\thailand_mirror_statue_left\\thailand_mirror_statue_left.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\thailand_mirror_statue_right\\thailand_mirror_statue_right.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\tiger1\\tiger1.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\tiger2\\tiger2.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\viking_thrall\\viking_thrall.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_ak_rifle\\weapon_ak_rifle.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_assault_rifle\\weapon_assault_rifle.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_grenade\\weapon_grenade.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_handgun\\weapon_handgun.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_shotgun\\weapon_shotgun.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_speargun\\weapon_speargun.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_tranquilizer\\weapon_tranquilizer.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\weapon_uzi\\weapon_uzi.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\wings\\wings.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\winston\\winston.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\wraith_stone\\wraith_stone.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\yacht\\yacht.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\yeti_thrall\\yeti_thrall.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\yeti_thrall_nochains\\yeti_thrall_nochains.mesh");
 // Render(L"D:\\XNALara\\Models\\original\\zip\\zip.mesh");

 Render(L"D:\\XNALara\\Models\\XNA\\trdaz\\ArNoSurge_Ionasal_Default\\generic_item.mesh");

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