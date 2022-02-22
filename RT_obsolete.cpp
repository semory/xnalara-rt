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
void InitExternObj(const STDSTRINGW& name);
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
void InitLara(XNAClassicModel& info);
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

void XNAClassicModel::AddMeshParams(const STDSTRINGW& name, uint16_t rg, bool visible)
{
 XNAClassicMeshParams cmp;
 cmp.render_group = rg;
 cmp.params[0] = 0.0f;
 cmp.params[1] = 0.0f;
 cmp.params[2] = 0.0f;
 cmp.visible = visible;
 params.insert_or_assign(name, cmp);
}

void XNAClassicModel::AddMeshParams(const STDSTRINGW& name, uint16_t rg, float a, bool visible)
{
 XNAClassicMeshParams cmp;
 cmp.render_group = rg;
 cmp.params[0] = a;
 cmp.params[1] = 0.0f;
 cmp.params[2] = 0.0f;
 cmp.visible = visible;
 params.insert_or_assign(name, cmp);
}

void XNAClassicModel::AddMeshParams(const STDSTRINGW& name, uint16_t rg, float a, float b, float c, bool visible)
{
 XNAClassicMeshParams cmp;
 cmp.render_group = rg;
 cmp.params[0] = a;
 cmp.params[1] = b;
 cmp.params[2] = c;
 cmp.visible = visible;
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
 // mesh properties
 XNAClassicModel info;
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
 // mesh properties
 XNAClassicModel info;
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
 // mesh properties
 XNAClassicModel info;
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
 info.AddMeshParams(L"hair1",      5);
 info.AddMeshParams(L"fur2",       5);
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
 // mesh properties
 XNAClassicModel info;
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
 info.AddMeshParams(L"hair1",      5);
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
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 info.AddMeshParams(L"skirtleft",  1, 0.00f, 16.0f, 16.0f);
 info.AddMeshParams(L"skirtright", 1, 0.00f, 16.0f, 16.0f);
 info.AddMeshParams(L"body1",      2, 0.10f);
 info.AddMeshParams(L"body2",      2, 0.10f);
 info.AddMeshParams(L"coat2",      2, 0.10f);
 info.AddMeshParams(L"hands",      2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"legs",       2, 0.10f);
 info.AddMeshParams(L"mouth",      3);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"hair2",      5);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"organs 1", L"organs 3");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"amelia", info);
}

void InitBraid(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"braid",   2, 0.2f);
 info.AddMeshParams(L"leather", 2, 0.2f);
 info.AddMeshParams(L"hair",    6, 0.2f);

 // set camera targets
 info.AddCameraTarget(L"braid", L"braid 1");

 // add model information
 modelmap.insert_or_assign(L"braid", info);
}

void InitCroftManorBack(void)
{
 // mesh properties
 XNAClassicModel info;
 for(int i = 1; i <= 66; i++) {
     WCHAR buffer[256];
     wsprintf(buffer, L"part%d", i);
     switch(i) {
       case(13) : info.AddMeshParams(buffer, 12, 0.05f); break;
       case(14) : info.AddMeshParams(buffer, 12, 0.05f); break;
       case(17) : info.AddMeshParams(buffer, 12, 0.05f); break;
       case(27) : info.AddMeshParams(buffer, 12, 0.05f); break;
       case(65) : info.AddMeshParams(buffer, 12, 0.05f); break;
       default : info.AddMeshParams(buffer, 11, 0.05f);
      }
    }

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"croft_manor_back", info);
}

void InitCroftManorHall(void)
{
 // mesh properties
 XNAClassicModel info;
 for(int i = 1; i <= 116; i++) {
     WCHAR buffer[256];
     wsprintf(buffer, L"Group%d", i);
     switch(i) {
       case( 35) : info.AddMeshParams(buffer, 12, 0.1f); break;
       case( 36) : info.AddMeshParams(buffer, 12, 0.1f); break;
       case( 37) : info.AddMeshParams(buffer, 12, 0.1f); break;
       case(112) : info.AddMeshParams(buffer, 12, 0.1f); break;
       case( 10) : info.AddMeshParams(buffer, 13); break;
       case( 91) : info.AddMeshParams(buffer, 13); break;
       case( 92) : info.AddMeshParams(buffer, 13); break;
       default : info.AddMeshParams(buffer, 11, 0.1f);
      }
    }

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"croft_manor_hall", info);
}

void InitCroftManorHallLQ(void)
{
 // mesh properties
 XNAClassicModel info;
 for(int i = 1; i <= 116; i++) {
     WCHAR buffer[256];
     wsprintf(buffer, L"Group%d", i);
     switch(i) {
       case( 35) : info.AddMeshParams(buffer, 15, 0.1f); break;
       case( 36) : info.AddMeshParams(buffer, 15, 0.1f); break;
       case( 37) : info.AddMeshParams(buffer, 15, 0.1f); break;
       case(112) : info.AddMeshParams(buffer, 15, 0.1f); break;
       case( 10) : info.AddMeshParams(buffer, 13); break;
       case( 91) : info.AddMeshParams(buffer, 13); break;
       case( 92) : info.AddMeshParams(buffer, 13); break;
       default : info.AddMeshParams(buffer, 14, 0.1f);
      }
    }

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"croft_manor_hall_lq", info);
}

void InitDiverCaptain(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"head",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"suit1",      1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"suit2left",  1, 0.15f,  8.0f,  8.0f);
 info.AddMeshParams(L"suit2right", 1, 0.15f,  8.0f,  8.0f);
 info.AddMeshParams(L"suit3",      1, 0.15f, 16.0f, 16.0f);
 info.AddMeshParams(L"suit4",      1, 0.15f,  8.0f,  8.0f);
 info.AddMeshParams(L"suit5",      1, 0.15f, 16.0f, 16.0f);
 info.AddMeshParams(L"gear1",      2, 0.10f);
 info.AddMeshParams(L"gear2",      2, 0.10f);
 info.AddMeshParams(L"eyes",       4, 0.10f);
 info.AddMeshParams(L"gun",        4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.60f);
 info.AddMeshParams(L"mouth",      4, 0.10f);
 info.AddMeshParams(L"hair",       6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"spine 1");
 info.AddCameraTarget(L"body lower", L"spine 4");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"diver_captain", info);
}

void InitDoppelganger(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"belts1",      1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"belts2",      1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"boots",       1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"gloves",      1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"jacket1",     1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"jacket2",     1, 0.20f,  8.0f,  8.0f);
 info.AddMeshParams(L"ribbons",     1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"trousers1",   1, 0.15f, 32.0f, 32.0f);
 info.AddMeshParams(L"trousers2",   1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"zipper3",     1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"arms",        2, 0.10f);
 info.AddMeshParams(L"body",        2, 0.10f);
 info.AddMeshParams(L"braid",       2, 0.10f);
 info.AddMeshParams(L"face",        2, 0.10f);
 info.AddMeshParams(L"fingers",     2, 0.10f);
 info.AddMeshParams(L"mouth",       2, 0.10f);
 info.AddMeshParams(L"eyeirises",   4, 0.10f);
 info.AddMeshParams(L"hair1",       4, 0.10f);
 info.AddMeshParams(L"metal",       4, 0.60f);
 info.AddMeshParams(L"zipper1",     4, 0.10f);
 info.AddMeshParams(L"zipper2",     4, 0.10f);
 info.AddMeshParams(L"zipper4",     4, 0.10f);
 info.AddMeshParams(L"eyewhites",   5);
 info.AddMeshParams(L"hair2",       6, 0.10f);
 info.AddMeshParams(L"hair3",       6, 0.10f);
 info.AddMeshParams(L"eyebrows",    7);
 info.AddMeshParams(L"eyelashes",   7);
 info.AddMeshParams(L"eyeshading",  7);
 info.AddMeshParams(L"eyepupils",  10);
 info.AddMeshParams(L"handgunhandleft",     6, 0.1f, true); // HANDGUNS = RG 6 (with alpha)
 info.AddMeshParams(L"handgunhandright",    6, 0.1f, true);
 info.AddMeshParams(L"handgunholsterleft",  6, 0.1f, true);
 info.AddMeshParams(L"handgunholsterright", 6, 0.1f, true);

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
 modelmap.insert_or_assign(L"doppelganger", info);
}

void InitDummy(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"dummy", 10);

 // set camera targets
 info.AddCameraTarget(L"dummy", L"dummy");

 // add model information
 modelmap.insert_or_assign(L"dummy", info);
}

void InitExcalibur(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"excalibur", 4, 0.3f);

 // set camera targets
 info.AddCameraTarget(L"excalibur", L"excalibur");

 // add model information
 modelmap.insert_or_assign(L"excalibur", info);
}

void InitExternObj(const STDSTRINGW& name)
{
 // NOTE: Some classic models like the GHALALI_KEY are not MESH files.
 // These are OBJ files instead and must be called from LoadOBJFile.
 // Only the following render groups are used. Model is always given a
 // single root bone.
 // a. RG10 (opaque, shadeless) (diffuse)
 // b. RG24 (opaque) (diffuse, light, bump, specular)
 // c. RG25 (transparent) (diffuse, light, bump, specular)

 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(name.c_str(), info);
}

void InitFoliageTree1(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"trunk",  17);
 info.AddMeshParams(L"leaves", 19);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"foliage_tree1", info);
}

void InitFoliageTree2(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"trunk",  17);
 info.AddMeshParams(L"leaves", 19);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"foliage_tree2", info);
}

void InitFoliageTree3(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"trunk",  17);
 info.AddMeshParams(L"leaves", 19);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"foliage_tree3", info);
}

void InitFoliageTree4(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"trunk",  17);
 info.AddMeshParams(L"leaves", 19);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"foliage_tree4", info);
}

void InitGlassCage(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"metal",    4, 0.25f);
 info.AddMeshParams(L"glass",    7);
 info.AddMeshParams(L"lights1", 10);
 info.AddMeshParams(L"lights2", 10);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"glass_cage", info);
}

void InitGlasses(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"frames", 5);
 info.AddMeshParams(L"glass",  7);

 // set camera targets
 info.AddCameraTarget(L"glasses", L"root");

 // add model information
 modelmap.insert_or_assign(L"glasses", info);
}

void InitGoLBruteMinion(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001",      4, 0.20f);
 info.AddMeshParams(L"mesh002",      4, 0.20f);
 info.AddMeshParams(L"mesh003left",  4, 0.20f);
 info.AddMeshParams(L"mesh003right", 4, 0.20f);
 info.AddMeshParams(L"mesh004",      4, 0.20f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head neck upper");
 info.AddCameraTarget(L"body upper", L"spine upper");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"gol_brute_minion", info);
}

void InitGoLChompy(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.3f);
 info.AddMeshParams(L"mesh002", 4, 0.3f);
 info.AddMeshParams(L"mesh003", 4, 0.3f);
 info.AddMeshParams(L"mesh004", 4, 0.3f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw 1");
 info.AddCameraTarget(L"body", L"spine 2", L"spine 3");
 info.AddCameraTarget(L"tail", L"spine 6");

 // add model information
 modelmap.insert_or_assign(L"gol_chompy", info);
}

void InitGoLDoppelganger(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh003",      2, 0.1f);
 info.AddMeshParams(L"mesh005",      2, 0.1f);
 info.AddMeshParams(L"mesh001",      4, 0.3f);
 info.AddMeshParams(L"mesh002left",  4, 0.1f);
 info.AddMeshParams(L"mesh002right", 4, 0.1f);
 info.AddMeshParams(L"mesh004",      4, 0.3f);
 info.AddMeshParams(L"mesh007",      4, 0.3f);
 info.AddMeshParams(L"mesh008",      4, 0.1f);
 info.AddMeshParams(L"mesh009",      4, 0.1f);
 info.AddMeshParams(L"mesh010",      4, 0.1f);
 info.AddMeshParams(L"mesh006",      7);

 // set camera targets
 info.AddCameraTarget(L"head", L"head neck upper");
 info.AddCameraTarget(L"body upper", L"spine upper");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"gol_doppelganger", info);
}

void InitGoLFireLizard(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLFireLizard", info);
}

void InitGoLFireShaman(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLFireShaman", info);
}

void InitGoLLara(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLLara", info);
}

void InitGoLLaraBiker(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLLaraBiker", info);
}

void InitGoLLaraJungleHeavy(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLLaraJungleHeavy", info);
}

void InitGoLLaraJungleShorts(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLLaraJungleShorts", info);
}

void InitGoLLaraLegend(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLLaraLegend", info);
}

void InitGoLPoisonGatekeeper(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLPoisonGatekeeper", info);
}

void InitGoLPoisonLizard(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLPoisonLizard", info);
}

void InitGoLPoisonMinion(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLPoisonMinion", info);
}

void InitGoLPoisonShaman(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLPoisonShaman", info);
}

void InitGoLShieldDemon(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLShieldDemon", info);
}

void InitGoLShieldGatekeeper(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLShieldGatekeeper", info);
}

void InitGoLSkeleton(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLSkeleton", info);
}

void InitGoLSmallMinion(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLSmallMinion", info);
}

void InitGoLTotec(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLTotec", info);
}

void InitGoLTRex(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLTRex", info);
}

void InitGoLXolotl(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"GoLXolotl", info);
}

void InitGrapple(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Grapple", info);
}

void InitHairAlister(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"HairAlister", info);
}

void InitHairAmanda(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"HairAmanda", info);
}

void InitHairNatla(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"HairNatla", info);
}

void InitHelicopter(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Helicopter", info);
}

void InitJellyfish(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Jellyfish", info);
}

void InitKnightThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"KnightThrall", info);
}

void InitKraken(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Kraken", info);
}

void InitLara(XNAClassicModel& info)
{
 // base mesh properties
 info.AddMeshParams(L"thorbelt1",              2, 0.5f, true); // THORGEAR = RG 2 (no alpha)
 info.AddMeshParams(L"thorbelt2",              2, 0.5f, true);
 info.AddMeshParams(L"thorgauntletleft",       2, 0.5f, true);
 info.AddMeshParams(L"thorgauntletright",      2, 0.5f, true);
 info.AddMeshParams(L"handgunhandleft",        6, 0.4f, true); // HANDGUNS = RG 6 (with alpha)
 info.AddMeshParams(L"handgunhandright",       6, 0.4f, true);
 info.AddMeshParams(L"handgunholsterleft",     6, 0.4f, true);
 info.AddMeshParams(L"handgunholsterright",    6, 0.4f, true);
 info.AddMeshParams(L"thorglowbelt",          21, true); // THORGLOW = RG 21 but with glow billboard
 info.AddMeshParams(L"thorglowgauntletleft",  21, true); 
 info.AddMeshParams(L"thorglowgauntletright", 21, true); 
 info.AddMeshParams(L"thorwireframe",         21, true); // should be separated into the three thorglows

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
}

void InitLaraBathingSuit(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

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
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"MayanThrall", info);
}

void InitMercenary(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Mercenary", info);
}

void InitMjolnir(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Mjolnir", info);
}

void InitMotorcycle(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Motorcycle", info);
}

void InitNagaGreen(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"NagaGreen", info);
}

void InitNagaRed(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"NagaRed", info);
}

void InitNatla(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Natla", info);
}

void InitNatlaNoWings(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"NatlaNoWings", info);
}

void InitPanther(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Panther", info);
}

void InitPantherThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"PantherThrall", info);
}

void InitPoacher(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Poacher", info);
}

void InitPonytail(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Ponytail", info);
}

void InitRecorder(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Recorder", info);
}

void InitScion1(void)
{
 // mesh properties
 XNAClassicModel info;

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Scion1", info);
}

void InitScion2(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Scion2", info);
}

void InitScion3(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Scion3", info);
}

void InitSharkBlue(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"SharkBlue", info);
}

void InitSharkGreenland(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"SharkGreenland", info);
}

void InitShip(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Ship", info);
}

void InitShipNatlaRoom(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"ShipNatlaRoom", info);
}

void InitSkyDomeThailandSea(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"SkyDomeThailandSea", info);
}

void InitThailandMirrorStatueLeft(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"ThailandMirrorStatueLeft", info);
}

void InitThailandMirrorStatueRight(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"ThailandMirrorStatueRight", info);
}

void InitTiger1(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Tiger1", info);
}

void InitTiger2(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Tiger2", info);
}

void InitVikingThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"VikingThrall", info);
}

void InitWeaponAKRifle(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponAKRifle", info);
}

void InitWeaponAssaultRifle(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponAssaultRifle", info);
}

void InitWeaponGrenade(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponGrenade", info);
}

void InitWeaponHandgun(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponHandgun", info);
}

void InitWeaponShotgun(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponShotgun", info);
}

void InitWeaponSpeargun(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponSpeargun", info);
}

void InitWeaponTranquilizer(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponTranquilizer", info);
}

void InitWeaponUzi(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WeaponUzi", info);
}

void InitWings(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Wings", info);
}

void InitWinston(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Winston", info);
}

void InitWraithStone(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"WraithStone", info);
}

void InitYacht(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Yacht", info);
}

void InitYetiThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"YetiThrall", info);
}

void InitYetiThrallNoChains(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"YetiThrallNoChains", info);
}

void InitZip(void)
{
 // mesh properties
 XNAClassicModel info;
 //info.AddMeshParams(L"coat1",      1, 0.05f, 16.0f, 16.0f);
 //info.AddMeshParams(L"legs",       2, 0.10f);
 //info.AddMeshParams(L"mouth",      3, 0.10f);
 //info.AddMeshParams(L"hair1",      4, 0.10f);
 //info.AddMeshParams(L"hair2",      5, 0.10f);
 //info.AddMeshParams(L"hair2",      6, 0.10f);
 //info.AddMeshParams(L"hair2",      7);

 // set camera targets

 // add model information
 modelmap.insert_or_assign(L"Zip", info);
}

#pragma endregion CLASSIC_DEFINITIONS