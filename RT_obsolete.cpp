#include "stdafx.h"
#include "stdxna.h"
#include "RT_win32.h"
#include "RT_math.h"
#include "RT_obsolete.h"

static std::map<STDSTRINGW, XNAClassicModel> modelmap;

#pragma region CLASSIC_DECLARATIONS

void InitAlbinoSpider(void);
void InitAlister(void);
void InitAmandaHeavy(void);
void InitAmandaLight(void);
void InitAmelia(void);
void InitBraid(void);
void InitCroftManorBack(void);
void InitCroftManorHall(void);
void InitCroftManorHallLQ(void);
void InitDiverCaptain(void);
void InitDoppelganger(void);
void InitDummy(void);
void InitExcalibur(void);
void InitExternObj(void);
void InitFoliageTree1(void);
void InitFoliageTree2(void);
void InitFoliageTree3(void);
void InitFoliageTree4(void);
void InitGlassCage(void);
void InitGlasses(void);
void InitGoLBruteMinion(void);
void InitGoLChompy(void);
void InitGoLDoppelganger(void);
void InitGoLFireLizard(void);
void InitGoLFireShaman(void);
void InitGoLLara(void);
void InitGoLLaraBiker(void);
void InitGoLLaraJungleHeavy(void);
void InitGoLLaraJungleShorts(void);
void InitGoLLaraLegend(void);
void InitGoLPoisonGatekeeper(void);
void InitGoLPoisonLizard(void);
void InitGoLPoisonMinion(void);
void InitGoLPoisonShaman(void);
void InitGoLShieldDemon(void);
void InitGoLShieldGatekeeper(void);
void InitGoLSkeleton(void);
void InitGoLSmallMinion(void);
void InitGoLTotec(void);
void InitGoLTRex(void);
void InitGoLXolotl(void);
void InitGrapple(void);
void InitHairAlister(void);
void InitHairAmanda(void);
void InitHairNatla(void);
void InitHelicopter(void);
void InitJellyfish(void);
void InitKnightThrall(void);
void InitKraken(void);
void InitLara(void);
void InitLaraBathingSuit(void);
void InitLaraBathingSuitNoGear(void);
void InitLaraCasual(void);
void InitLaraDLCBikiniBlackWhite(void);
void InitLaraDLCBikiniBlue(void);
void InitLaraDLCBikiniCamo(void);
void InitLaraDLCBikiniYellow(void);
void InitLaraDLCCasualExplorer(void);
void InitLaraDLCClassic(void);
void InitLaraDLCDesignerFashionWinner(void);
void InitLaraDLCDesignerPeoplesChoice(void);
void InitLaraDLCDrySuitRed(void);
void InitLaraDLCJungleShortsGrey(void);
void InitLaraDLCLegend(void);
void InitLaraDLCWetSuitBlue(void);
void InitLaraDrySuit(void);
void InitLaraJungleHeavy(void);
void InitLaraJunglePants(void);
void InitLaraJungleShorts(void);
void InitLaraSnowHeavy(void);
void InitLaraSnowLight(void);
void InitLaraWetSuit(void);
void InitLaraWetSuitNoGear(void);
void InitMayanThrall(void);
void InitMercenary(void);
void InitMjolnir(void);
void InitMotorcycle(void);
void InitNagaGreen(void);
void InitNagaRed(void);
void InitNatla(void);
void InitNatlaNoWings(void);
void InitPanther(void);
void InitPantherThrall(void);
void InitPoacher(void);
void InitPonytail(void);
void InitRecorder(void);
void InitScion1(void);
void InitScion2(void);
void InitScion3(void);
void InitSharkBlue(void);
void InitSharkGreenland(void);
void InitShip(void);
void InitShipNatlaRoom(void);
void InitSkyDomeThailandSea(void);
void InitThailandMirrorStatueLeft(void);
void InitThailandMirrorStatueRight(void);
void InitTiger1(void);
void InitTiger2(void);
void InitVikingThrall(void);
void InitWeaponAKRifle(void);
void InitWeaponAssaultRifle(void);
void InitWeaponGrenade(void);
void InitWeaponHandgun(void);
void InitWeaponShotgun(void);
void InitWeaponSpeargun(void);
void InitWeaponTranquilizer(void);
void InitWeaponUzi(void);
void InitWings(void);
void InitWinston(void);
void InitWraithStone(void);
void InitYacht(void);
void InitYetiThrall(void);
void InitYetiThrallNoChains(void);
void InitZip(void);

#pragma endregion CLASSIC_DECLARATIONS

#pragma region CLASSIC_MODEL

void XNAClassicModel::AddMeshParams(const STDSTRINGW& name, uint16_t rg)
{
 XNAClassicMeshParams cmp;
 cmp.render_group = rg;
 cmp.params[0] = 0.0f;
 cmp.params[1] = 0.0f;
 cmp.params[2] = 0.0f;
 params.insert_or_assign(name, cmp);
}

void XNAClassicModel::AddMeshParams(const STDSTRINGW& name, uint16_t rg, float a)
{
 XNAClassicMeshParams cmp;
 cmp.render_group = rg;
 cmp.params[0] = a;
 cmp.params[1] = 0.0f;
 cmp.params[2] = 0.0f;
 params.insert_or_assign(name, cmp);
}

void XNAClassicModel::AddMeshParams(const STDSTRINGW& name, uint16_t rg, float a, float b, float c)
{
 XNAClassicMeshParams cmp;
 cmp.render_group = rg;
 cmp.params[0] = a;
 cmp.params[1] = b;
 cmp.params[2] = c;
 params.insert_or_assign(name, cmp);
}

void XNAClassicModel::AddCameraTarget(const STDSTRINGW& name, const STDSTRINGW& b1)
{
 targets.insert_or_assign(name, XNACameraTarget(b1));
}

void XNAClassicModel::AddCameraTarget(const STDSTRINGW& name, const STDSTRINGW& b1, const STDSTRINGW& b2)
{
 targets.insert_or_assign(name, XNACameraTarget(b1, b2));
}

#pragma endregion CLASSIC_MODEL

#pragma region CLASSIC_PUBLIC_DEFINITIONS

void InitClassicModels(void)
{
 InitAlbinoSpider();
 InitAlister();
 InitAmandaHeavy();
 InitAmandaLight();
 InitAmelia();
 InitBraid();
 InitCroftManorBack();
 InitCroftManorHall();
 InitCroftManorHallLQ();
 InitDiverCaptain();
 InitDoppelganger();
 InitDummy();
 InitExcalibur();
 InitExternObj();
 InitFoliageTree1();
 InitFoliageTree2();
 InitFoliageTree3();
 InitFoliageTree4();
 InitGlassCage();
 InitGlasses();
 InitGoLBruteMinion();
 InitGoLChompy();
 InitGoLDoppelganger();
 InitGoLFireLizard();
 InitGoLFireShaman();
 InitGoLLara();
 InitGoLLaraBiker();
 InitGoLLaraJungleHeavy();
 InitGoLLaraJungleShorts();
 InitGoLLaraLegend();
 InitGoLPoisonGatekeeper();
 InitGoLPoisonLizard();
 InitGoLPoisonMinion();
 InitGoLPoisonShaman();
 InitGoLShieldDemon();
 InitGoLShieldGatekeeper();
 InitGoLSkeleton();
 InitGoLSmallMinion();
 InitGoLTotec();
 InitGoLTRex();
 InitGoLXolotl();
 InitGrapple();
 InitHairAlister();
 InitHairAmanda();
 InitHairNatla();
 InitHelicopter();
 InitJellyfish();
 InitKnightThrall();
 InitKraken();
 InitLara();
 InitLaraBathingSuit();
 InitLaraBathingSuitNoGear();
 InitLaraCasual();
 InitLaraDLCBikiniBlackWhite();
 InitLaraDLCBikiniBlue();
 InitLaraDLCBikiniCamo();
 InitLaraDLCBikiniYellow();
 InitLaraDLCCasualExplorer();
 InitLaraDLCClassic();
 InitLaraDLCDesignerFashionWinner();
 InitLaraDLCDesignerPeoplesChoice();
 InitLaraDLCDrySuitRed();
 InitLaraDLCJungleShortsGrey();
 InitLaraDLCLegend();
 InitLaraDLCWetSuitBlue();
 InitLaraDrySuit();
 InitLaraJungleHeavy();
 InitLaraJunglePants();
 InitLaraJungleShorts();
 InitLaraSnowHeavy();
 InitLaraSnowLight();
 InitLaraWetSuit();
 InitLaraWetSuitNoGear();
 InitMayanThrall();
 InitMercenary();
 InitMjolnir();
 InitMotorcycle();
 InitNagaGreen();
 InitNagaRed();
 InitNatla();
 InitNatlaNoWings();
 InitPanther();
 InitPantherThrall();
 InitPoacher();
 InitPonytail();
 InitRecorder();
 InitScion1();
 InitScion2();
 InitScion3();
 InitSharkBlue();
 InitSharkGreenland();
 InitShip();
 InitShipNatlaRoom();
 InitSkyDomeThailandSea();
 InitThailandMirrorStatueLeft();
 InitThailandMirrorStatueRight();
 InitTiger1();
 InitTiger2();
 InitVikingThrall();
 InitWeaponAKRifle();
 InitWeaponAssaultRifle();
 InitWeaponGrenade();
 InitWeaponHandgun();
 InitWeaponShotgun();
 InitWeaponSpeargun();
 InitWeaponTranquilizer();
 InitWeaponUzi();
 InitWings();
 InitWinston();
 InitWraithStone();
 InitYacht();
 InitYetiThrall();
 InitYetiThrallNoChains();
 InitZip();
}

const XNAClassicModel* GetClassicModel(const STDSTRINGW& name)
{
 auto search = modelmap.find(name);
 if(search == modelmap.end()) return nullptr;
 return &(search->second);
}

#pragma endregion CLASSIC_PUBLIC_DEFINITIONS

#pragma region CLASSIC_DEFINITIONS

void InitAlbinoSpider(void)
{
 // not a lara model
 XNAClassicModel info;
 info.laracroft = false;

 // mesh properties
 info.AddMeshParams(L"body",  2, 0.3f);
 info.AddMeshParams(L"eyes",  2, 0.4f);
 info.AddMeshParams(L"legs1", 2, 0.3f);
 info.AddMeshParams(L"legs2", 2, 0.3f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaws upper left 1", L"head jaws upper right 2");
 info.AddCameraTarget(L"body", L"root body");

 // add model information
 modelmap.insert_or_assign(L"albino_spider", info);
}

void InitAlister(void)
{
 // not a lara model
 XNAClassicModel info;
 info.laracroft = false;

 // mesh properties
 info.AddMeshParams(L"buttons",    1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"coat1left",  1, 0.05f,  8.0f, 12.0f);
 info.AddMeshParams(L"coat1right", 1, 0.05f,  8.0f, 12.0f);
 info.AddMeshParams(L"coat2",      1, 0.05f,  8.0f, 12.0f);
 info.AddMeshParams(L"leather",    1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"shirt",      1, 0.10f,  4.0f,  4.0f);
 info.AddMeshParams(L"trousers",   1, 0.05f, 16.0f, 16.0f);
 info.AddMeshParams(L"bodyleft",   2, 0.10f);
 info.AddMeshParams(L"bodyright",  2, 0.10f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"eyes",       4, 0.10f);
 info.AddMeshParams(L"hair1",      5);
 info.AddMeshParams(L"metal",      5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"hair3",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7); 
 info.AddMeshParams(L"eyelashes",  7); 
 info.AddMeshParams(L"eyeshading", 7); 
 info.AddMeshParams(L"glass",      7); 
 info.AddMeshParams(L"necklace",   8, 0.10f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"arm left shoulder 1", L"arm right shoulder 1");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"alister", info);
}

void InitAmandaHeavy(void)
{
 // not a lara model
 XNAClassicModel info;
 info.laracroft = false;

 // mesh properties
 info.AddMeshParams(L"bands1",     1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"belt",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"boots",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"coat",       1, 0.10f, 16.0f, 12.0f);
 info.AddMeshParams(L"sleeves",    1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"trousers",   1, 0.10f, 10.0f, 10.0f);
 info.AddMeshParams(L"body",       2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"hand2left",  2, 0.10f);
 info.AddMeshParams(L"hand2right", 2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"eyes",       4, 0.10f);
 info.AddMeshParams(L"metal1",     4, 0.60f);
 info.AddMeshParams(L"metal3",     4, 0.60f);
 info.AddMeshParams(L"metal5",     4, 0.60f);
 info.AddMeshParams(L"zipper1",    4, 0.10f);
 info.AddMeshParams(L"zipper2",    4, 0.10f);
 info.AddMeshParams(L"hair1",      5, 0.10f);
 info.AddMeshParams(L"fur2",       5, 0.10f);
 info.AddMeshParams(L"hair3",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"hair2left",  7);
 info.AddMeshParams(L"hair2right", 7);
 info.AddMeshParams(L"jewelry",    7);
 info.AddMeshParams(L"scar",       7);
 info.AddMeshParams(L"fur1",       8, 0.1f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"breast left", L"breast right");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"amanda_heavy", info);
}

void InitAmandaLight(void)
{
 // not a lara model
 XNAClassicModel info;
 info.laracroft = false;

 // mesh properties
 info.AddMeshParams(L"bands1",     1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"bands2",     1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"bands3",     1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"boots",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"trousers",   1, 0.15f, 10.0f, 10.0f);
 info.AddMeshParams(L"tshirt",     1, 0.15f, 16.0f, 32.0f);
 info.AddMeshParams(L"arms",       2, 0.10f);
 info.AddMeshParams(L"body",       2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"hand1left",  2, 0.10f);
 info.AddMeshParams(L"hand1right", 2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"eyes",       4, 0.10f);
 info.AddMeshParams(L"metal1",     4, 0.60f);
 info.AddMeshParams(L"metal2",     4, 0.60f);
 info.AddMeshParams(L"metal4",     4, 0.60f);
 info.AddMeshParams(L"zipper1",    4, 0.10f);
 info.AddMeshParams(L"zipper2",    4, 0.10f);
 info.AddMeshParams(L"hair1",      5, 0.10f);
 info.AddMeshParams(L"hair3",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"hair2left",  7);
 info.AddMeshParams(L"hair2right", 7);
 info.AddMeshParams(L"jewelry",    7);
 info.AddMeshParams(L"scar",       7);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"breast left", L"breast right");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"amanda_light", info);
}

void InitAmelia(void)
{
}

void InitBraid(void)
{
}

void InitCroftManorBack(void)
{
}

void InitCroftManorHall(void)
{
}

void InitCroftManorHallLQ(void)
{
}

void InitDiverCaptain(void)
{
}

void InitDoppelganger(void)
{
}

void InitDummy(void)
{
}

void InitExcalibur(void)
{
}

void InitExternObj(void)
{
}

void InitFoliageTree1(void)
{
}

void InitFoliageTree2(void)
{
}

void InitFoliageTree3(void)
{
}

void InitFoliageTree4(void)
{
}

void InitGlassCage(void)
{
}

void InitGlasses(void)
{
}

void InitGoLBruteMinion(void)
{
}

void InitGoLChompy(void)
{
}

void InitGoLDoppelganger(void)
{
}

void InitGoLFireLizard(void)
{
}

void InitGoLFireShaman(void)
{
}

void InitGoLLara(void)
{
}

void InitGoLLaraBiker(void)
{
}

void InitGoLLaraJungleHeavy(void)
{
}

void InitGoLLaraJungleShorts(void)
{
}

void InitGoLLaraLegend(void)
{
}

void InitGoLPoisonGatekeeper(void)
{
}

void InitGoLPoisonLizard(void)
{
}

void InitGoLPoisonMinion(void)
{
}

void InitGoLPoisonShaman(void)
{
}

void InitGoLShieldDemon(void)
{
}

void InitGoLShieldGatekeeper(void)
{
}

void InitGoLSkeleton(void)
{
}

void InitGoLSmallMinion(void)
{
}

void InitGoLTotec(void)
{
}

void InitGoLTRex(void)
{
}

void InitGoLXolotl(void)
{
}

void InitGrapple(void)
{
}

void InitHairAlister(void)
{
}

void InitHairAmanda(void)
{
}

void InitHairNatla(void)
{
}

void InitHelicopter(void)
{
}

void InitJellyfish(void)
{
}

void InitKnightThrall(void)
{
}

void InitKraken(void)
{
}

void InitLara(void)
{
}

void InitLaraBathingSuit(void)
{
 // not a lara model
 XNAClassicModel info;
 info.laracroft = true;

 // mesh properties
 info.AddMeshParams(L"backpack",   1, 0.1f, 12.0f, 12.0f);
 info.AddMeshParams(L"belts",      1, 0.1f, 10.0f, 20.0f);
 info.AddMeshParams(L"collar",     1, 0.1f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear2",      1, 0.1f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear3",      1, 0.1f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear6",      1, 0.1f, 12.0f, 12.0f);
 info.AddMeshParams(L"suit",       1, 0.3f, 24.0f, 48.0f);
 info.AddMeshParams(L"arms",       2, 0.1f);
 info.AddMeshParams(L"body",       2, 0.1f);
 info.AddMeshParams(L"eyeducts",   2, 0.1f);
 info.AddMeshParams(L"face",       2, 0.1f);
 info.AddMeshParams(L"feet",       2, 0.1f);
 info.AddMeshParams(L"hands",      2, 0.1f);
 info.AddMeshParams(L"legs",       2, 0.1f);
 info.AddMeshParams(L"light",      2, 0.1f);
 info.AddMeshParams(L"mouth",      2, 0.1f);
 info.AddMeshParams(L"ribbon",     2, 0.1f);
 info.AddMeshParams(L"backring",   3, 0.1f);
 info.AddMeshParams(L"eyeirises",  4, 0.1f);
 info.AddMeshParams(L"gear5",      4, 0.1f);
 info.AddMeshParams(L"hair1",      4, 0.1f);
 info.AddMeshParams(L"metal",      4, 0.1f);
 info.AddMeshParams(L"zipper1",    4, 0.1f);
 info.AddMeshParams(L"zipper2",    4, 0.1f);
 info.AddMeshParams(L"eyewhites",  5, 0.1f);
 info.AddMeshParams(L"gear1",      5, 0.1f);
 info.AddMeshParams(L"hair2",      6, 0.1f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"gear4",      7);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"breast left", L"breast right");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"lara_bathing_suit", info);
}

void InitLaraBathingSuitNoGear(void)
{
}

void InitLaraCasual(void)
{
}

void InitLaraDLCBikiniBlackWhite(void)
{
}

void InitLaraDLCBikiniBlue(void)
{
}

void InitLaraDLCBikiniCamo(void)
{
}

void InitLaraDLCBikiniYellow(void)
{
}

void InitLaraDLCCasualExplorer(void)
{
}

void InitLaraDLCClassic(void)
{
}

void InitLaraDLCDesignerFashionWinner(void)
{
}

void InitLaraDLCDesignerPeoplesChoice(void)
{
}

void InitLaraDLCDrySuitRed(void)
{
}

void InitLaraDLCJungleShortsGrey(void)
{
}

void InitLaraDLCLegend(void)
{
}

void InitLaraDLCWetSuitBlue(void)
{
}

void InitLaraDrySuit(void)
{
}

void InitLaraJungleHeavy(void)
{
}

void InitLaraJunglePants(void)
{
}

void InitLaraJungleShorts(void)
{
}

void InitLaraSnowHeavy(void)
{
}

void InitLaraSnowLight(void)
{
}

void InitLaraWetSuit(void)
{
}

void InitLaraWetSuitNoGear(void)
{
}

void InitMayanThrall(void)
{
}

void InitMercenary(void)
{
}

void InitMjolnir(void)
{
}

void InitMotorcycle(void)
{
}

void InitNagaGreen(void)
{
}

void InitNagaRed(void)
{
}

void InitNatla(void)
{
}

void InitNatlaNoWings(void)
{
}

void InitPanther(void)
{
}

void InitPantherThrall(void)
{
}

void InitPoacher(void)
{
}

void InitPonytail(void)
{
}

void InitRecorder(void)
{
}

void InitScion1(void)
{
}

void InitScion2(void)
{
}

void InitScion3(void)
{
}

void InitSharkBlue(void)
{
}

void InitSharkGreenland(void)
{
}

void InitShip(void)
{
}

void InitShipNatlaRoom(void)
{
}

void InitSkyDomeThailandSea(void)
{
}

void InitThailandMirrorStatueLeft(void)
{
}

void InitThailandMirrorStatueRight(void)
{
}

void InitTiger1(void)
{
}

void InitTiger2(void)
{
}

void InitVikingThrall(void)
{
}

void InitWeaponAKRifle(void)
{
}

void InitWeaponAssaultRifle(void)
{
}

void InitWeaponGrenade(void)
{
}

void InitWeaponHandgun(void)
{
}

void InitWeaponShotgun(void)
{
}

void InitWeaponSpeargun(void)
{
}

void InitWeaponTranquilizer(void)
{
}

void InitWeaponUzi(void)
{
}

void InitWings(void)
{
}

void InitWinston(void)
{
}

void InitWraithStone(void)
{
}

void InitYacht(void)
{
}

void InitYetiThrall(void)
{
}

void InitYetiThrallNoChains(void)
{
}

void InitZip(void)
{
}

#pragma endregion CLASSIC_DEFINITIONS