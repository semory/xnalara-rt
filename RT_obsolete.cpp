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

void XNAClassicModel::AddCameraTargetStandardUpperF(void)
{
 AddCameraTarget(L"head", L"head jaw");
 AddCameraTarget(L"body upper", L"breast left", L"breast right");
}

void XNAClassicModel::AddCameraTargetStandardUpperM(void)
{
 AddCameraTarget(L"head", L"head jaw");
 AddCameraTarget(L"body upper", L"spine upper");
}

void XNAClassicModel::AddCameraTargetStandardLower(void)
{
 AddCameraTarget(L"body lower", L"pelvis");
 AddCameraTarget(L"hand left", L"arm left wrist");
 AddCameraTarget(L"hand right", L"arm right wrist");
 AddCameraTarget(L"knee left", L"leg left knee");
 AddCameraTarget(L"knee right", L"leg right knee");
 AddCameraTarget(L"foot left", L"leg left ankle");
 AddCameraTarget(L"foot right", L"leg right ankle");
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
 info.AddMeshParams(L"mesh001", 4, 0.3f);
 info.AddMeshParams(L"mesh002", 4, 0.3f);
 info.AddMeshParams(L"mesh003", 4, 0.3f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"tail", L"tail 5");

 // add model information
 modelmap.insert_or_assign(L"gol_fire_lizard", info);
}

void InitGoLFireShaman(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_fire_shaman", info);
}

void InitGoLLara(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh002",      2, 0.1f);
 info.AddMeshParams(L"mesh001",      4, 0.2f);
 info.AddMeshParams(L"mesh003",      4, 0.1f);
 info.AddMeshParams(L"mesh004left",  4, 0.1f);
 info.AddMeshParams(L"mesh004right", 4, 0.1f);
 info.AddMeshParams(L"mesh006",      4, 0.1f);
 info.AddMeshParams(L"mesh005",      7);

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
 modelmap.insert_or_assign(L"gol_lara", info);
}

void InitGoLLaraBiker(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 2, 0.1f);
 info.AddMeshParams(L"mesh002", 4, 0.3f);
 info.AddMeshParams(L"mesh003", 4, 0.1f);
 info.AddMeshParams(L"mesh004", 4, 0.1f);
 info.AddMeshParams(L"mesh005", 4, 0.1f);
 info.AddMeshParams(L"mesh006", 4, 0.2f);
 info.AddMeshParams(L"mesh007", 5, 0.3f);
 info.AddMeshParams(L"mesh008", 7);

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
 modelmap.insert_or_assign(L"gol_lara_biker", info);
}

void InitGoLLaraJungleHeavy(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh002", 2, 0.1f);
 info.AddMeshParams(L"mesh001", 4, 0.3f);
 info.AddMeshParams(L"mesh003", 4, 0.1f);
 info.AddMeshParams(L"mesh004", 4, 0.1f);
 info.AddMeshParams(L"mesh005", 4, 0.1f);
 info.AddMeshParams(L"mesh007", 4, 0.1f);
 info.AddMeshParams(L"mesh008", 4, 0.1f);
 info.AddMeshParams(L"mesh010", 4, 0.1f);
 info.AddMeshParams(L"mesh011", 4, 0.3f);
 info.AddMeshParams(L"mesh012", 4, 0.1f);
 info.AddMeshParams(L"mesh013", 4, 0.1f);
 info.AddMeshParams(L"mesh009", 5, 0.1f);
 info.AddMeshParams(L"mesh006", 7);

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
 modelmap.insert_or_assign(L"gol_lara_jungle_heavy", info);
}

void InitGoLLaraJungleShorts(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh006",      2, 0.1f);
 info.AddMeshParams(L"mesh001left",  4, 0.1f);
 info.AddMeshParams(L"mesh001right", 4, 0.1f);
 info.AddMeshParams(L"mesh002",      4, 0.1f);
 info.AddMeshParams(L"mesh003",      4, 0.1f);
 info.AddMeshParams(L"mesh004",      4, 0.1f);
 info.AddMeshParams(L"mesh005",      4, 0.1f);
 info.AddMeshParams(L"mesh007",      4, 0.1f);
 info.AddMeshParams(L"mesh008",      4, 0.2f);
 info.AddMeshParams(L"mesh009",      4, 0.3f);
 info.AddMeshParams(L"mesh010",      4, 0.1f);
 info.AddMeshParams(L"mesh011",      5, 0.1f);
 info.AddMeshParams(L"mesh012",      7);

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
 modelmap.insert_or_assign(L"gol_lara_jungle_shorts", info);
}

void InitGoLLaraLegend(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh003",      2, 0.1f);
 info.AddMeshParams(L"mesh001",      4, 0.3f);
 info.AddMeshParams(L"mesh004",      4, 0.1f);
 info.AddMeshParams(L"mesh005left",  4, 0.1f);
 info.AddMeshParams(L"mesh005right", 4, 0.1f);
 info.AddMeshParams(L"mesh006",      4, 0.1f);
 info.AddMeshParams(L"mesh008",      4, 0.1f);
 info.AddMeshParams(L"mesh002",      5, 0.1f);
 info.AddMeshParams(L"mesh007",      7);

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
 modelmap.insert_or_assign(L"gol_lara_legend", info);
}

void InitGoLPoisonGatekeeper(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);
 info.AddMeshParams(L"mesh004", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_poison_gatekeeper", info);
}

void InitGoLPoisonLizard(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.3f);
 info.AddMeshParams(L"mesh002", 4, 0.3f);
 info.AddMeshParams(L"mesh003", 4, 0.3f);
 info.AddMeshParams(L"mesh004", 4, 0.3f);
 info.AddMeshParams(L"mesh005", 4, 0.3f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"tail", L"tail 5");

 // add model information
 modelmap.insert_or_assign(L"gol_poison_lizard", info);
}

void InitGoLPoisonMinion(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_poison_minion", info);
}

void InitGoLPoisonShaman(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_poison_shaman", info);
}

void InitGoLShieldDemon(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);
 info.AddMeshParams(L"mesh004", 4, 0.2f);
 info.AddMeshParams(L"mesh005", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_shield_demon", info);
}

void InitGoLShieldGatekeeper(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);
 info.AddMeshParams(L"mesh004", 4, 0.2f);
 info.AddMeshParams(L"mesh005", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_shield_gatekeeper", info);
}

void InitGoLSkeleton(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);
 info.AddMeshParams(L"mesh004", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_skeleton", info);
}

void InitGoLSmallMinion(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);

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
 modelmap.insert_or_assign(L"gol_small_minion", info);
}

void InitGoLTotec(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh003", 4, 0.2f);
 info.AddMeshParams(L"mesh004", 4, 0.2f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head neck upper");
 info.AddCameraTarget(L"body upper", L"spine upper");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"gol_totec", info);
}

void InitGoLTRex(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001", 4, 0.2f);
 info.AddMeshParams(L"mesh002", 4, 0.2f);
 info.AddMeshParams(L"mesh004", 4, 0.2f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 5");
 info.AddCameraTarget(L"tail", L"tail 4");
 info.AddCameraTarget(L"leg hind left", L"leg hind left 4");
 info.AddCameraTarget(L"leg hind right", L"leg hind right 4");

 // add model information
 modelmap.insert_or_assign(L"gol_trex", info);
}

void InitGoLXolotl(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh001",      4, 0.2f);
 info.AddMeshParams(L"mesh002",      4, 0.2f);
 info.AddMeshParams(L"mesh003",      4, 0.2f);
 info.AddMeshParams(L"mesh004left",  4, 0.2f);
 info.AddMeshParams(L"mesh004right", 4, 0.2f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head neck upper");
 info.AddCameraTarget(L"body upper", L"spine upper");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"gol_xolotl", info);
}

void InitGrapple(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh1", 4, 0.50f);
 info.AddMeshParams(L"mesh2", 4, 0.30f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"grapple", info);
}

void InitHairAlister(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"hair1", 5);
 info.AddMeshParams(L"hair2", 6, 0.10f);
 info.AddMeshParams(L"hair3", 6, 0.10f);

 // set camera targets
 info.AddCameraTarget(L"hair", L"head neck upper");

 // add model information
 modelmap.insert_or_assign(L"hair_alister", info);
}

void InitHairAmanda(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"hair2",      5);
 info.AddMeshParams(L"hair3",      6, 0.10f);
 info.AddMeshParams(L"hair2left",  7);
 info.AddMeshParams(L"hair2right", 7);

 // set camera targets
 info.AddCameraTarget(L"hair", L"head neck upper");

 // add model information
 modelmap.insert_or_assign(L"hair_amanda", info);
}

void InitHairNatla(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"hair1", 2, 0.10f);
 info.AddMeshParams(L"hair2", 7);
 info.AddMeshParams(L"hair3", 8, 0.10f);
 info.AddMeshParams(L"hair4", 8, 0.10f);
 info.AddMeshParams(L"hair5", 9);

 // set camera targets
 info.AddCameraTarget(L"hair", L"head neck upper");

 // add model information
 modelmap.insert_or_assign(L"hair_natla", info);
}

void InitHelicopter(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.25f);
 info.AddMeshParams(L"part2", 4, 0.25f);
 info.AddMeshParams(L"part3", 4, 0.25f);
 info.AddMeshParams(L"part4", 4, 0.25f);
 info.AddMeshParams(L"part5", 7);
 info.AddMeshParams(L"part6", 7);

 // set camera targets
 info.AddCameraTarget(L"root", L"root ground");

 // add model information
 modelmap.insert_or_assign(L"helicopter", info);
}

void InitJellyfish(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 7);
 info.AddMeshParams(L"part2", 7);
 info.AddMeshParams(L"part3", 7);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"jellyfish", info);
}

void InitKnightThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"Mesh001", 20, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"Mesh002", 20, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"Mesh003", 20, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"Mesh004", 20, 0.10f, 16.0f, 16.0f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw 2");
 info.AddCameraTarget(L"body upper", L"arm left shoulder 1", L"arm right shoulder 1");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"knight_thrall", info);
}

void InitKraken(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part2",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part4",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part5",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part6",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part7",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part8",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part9",  1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part10", 1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part12", 1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part11", 2, 0.20f);
 info.AddMeshParams(L"part3",  4, 0.20f);

 // set camera targets
 info.AddCameraTarget(L"head", L"body 1");

 // add model information
 modelmap.insert_or_assign(L"kraken", info);
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
 info.AddCameraTargetStandardUpperF();
 info.AddCameraTargetStandardLower();
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
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.1f);
 info.AddMeshParams(L"gear5",      4, 0.1f);
 info.AddMeshParams(L"hair1",      4, 0.1f);
 info.AddMeshParams(L"metal",      4, 0.1f);
 info.AddMeshParams(L"zipper1",    4, 0.1f);
 info.AddMeshParams(L"zipper2",    4, 0.1f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"gear1",      5);
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
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"collar",     1, 0.1f, 12.0f, 12.0f);
 info.AddMeshParams(L"suit",       1, 0.3f, 24.0f, 48.0f);
 info.AddMeshParams(L"arms",       2, 0.1f);
 info.AddMeshParams(L"body",       2, 0.1f);
 info.AddMeshParams(L"eyeducts",   2, 0.1f);
 info.AddMeshParams(L"face",       2, 0.1f);
 info.AddMeshParams(L"feet",       2, 0.1f);
 info.AddMeshParams(L"hands",      2, 0.1f);
 info.AddMeshParams(L"legs",       2, 0.1f);
 info.AddMeshParams(L"mouth",      2, 0.1f);
 info.AddMeshParams(L"ribbon",     2, 0.1f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.1f);
 info.AddMeshParams(L"hair1",      4, 0.1f);
 info.AddMeshParams(L"zipper1",    4, 0.1f);
 info.AddMeshParams(L"zipper2",    4, 0.1f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"hair2",      6, 0.1f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_bathing_suit_nogear", info);
}

void InitLaraCasual(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 20.0f);
 info.AddMeshParams(L"boots1",     1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"sweater",    1, 0.03f, 12.0f, 30.0f);
 info.AddMeshParams(L"trousers",   1, 0.05f, 8.0f, 8.0f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"hands",      2, 0.10f);
 info.AddMeshParams(L"light1",     2, 0.10f);
 info.AddMeshParams(L"light2",     2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"neck",       2, 0.10f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"boots2",     4, 0.60f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.60f);
 info.AddMeshParams(L"zipper1",    4, 0.10f);
 info.AddMeshParams(L"zipper2",    4, 0.10f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_casual", info);
}

void InitLaraDLCBikiniBlackWhite(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh013", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh015", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh022", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh023", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh024", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh001", 2, 0.1f);
 info.AddMeshParams(L"mesh008", 2, 0.1f);
 info.AddMeshParams(L"mesh010", 2, 0.1f);
 info.AddMeshParams(L"mesh017", 2, 0.1f);
 info.AddMeshParams(L"mesh018", 2, 0.1f);
 info.AddMeshParams(L"mesh019", 2, 0.1f);
 info.AddMeshParams(L"mesh020", 2, 0.1f);
 info.AddMeshParams(L"mesh021", 2, 0.1f);
 info.AddMeshParams(L"mesh014", 3);
 info.AddMeshParams(L"mesh009", 4, 0.1f);
 info.AddMeshParams(L"mesh012", 4, 0.6f);
 info.AddMeshParams(L"mesh004", 5);
 info.AddMeshParams(L"mesh005", 5);
 info.AddMeshParams(L"mesh016", 5);
 info.AddMeshParams(L"mesh011", 6, 0.1f);
 info.AddMeshParams(L"mesh003", 7);
 info.AddMeshParams(L"mesh006", 7);
 info.AddMeshParams(L"mesh007", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_bikini_blackwhite", info);
}

void InitLaraDLCBikiniBlue(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh013", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh015", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh022", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh023", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh027", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh001", 2, 0.1f);
 info.AddMeshParams(L"mesh008", 2, 0.1f);
 info.AddMeshParams(L"mesh010", 2, 0.1f);
 info.AddMeshParams(L"mesh017", 2, 0.1f);
 info.AddMeshParams(L"mesh018", 2, 0.1f);
 info.AddMeshParams(L"mesh019", 2, 0.1f);
 info.AddMeshParams(L"mesh020", 2, 0.1f);
 info.AddMeshParams(L"mesh021", 2, 0.1f);
 info.AddMeshParams(L"mesh024", 2, 0.1f);
 info.AddMeshParams(L"mesh025", 2, 0.1f);
 info.AddMeshParams(L"mesh014", 3);
 info.AddMeshParams(L"mesh009", 4, 0.1f);
 info.AddMeshParams(L"mesh012", 4, 0.6f);
 info.AddMeshParams(L"mesh004", 5);
 info.AddMeshParams(L"mesh005", 5);
 info.AddMeshParams(L"mesh016", 5);
 info.AddMeshParams(L"mesh011", 6, 0.1f);
 info.AddMeshParams(L"mesh003", 7);
 info.AddMeshParams(L"mesh006", 7);
 info.AddMeshParams(L"mesh007", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_bikini_blue", info);
}

void InitLaraDLCBikiniCamo(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh013", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh015", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh022", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh023", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh025", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh001", 2, 0.1f);
 info.AddMeshParams(L"mesh008", 2, 0.1f);
 info.AddMeshParams(L"mesh010", 2, 0.1f);
 info.AddMeshParams(L"mesh017", 2, 0.1f);
 info.AddMeshParams(L"mesh018", 2, 0.1f);
 info.AddMeshParams(L"mesh019", 2, 0.1f);
 info.AddMeshParams(L"mesh020", 2, 0.1f);
 info.AddMeshParams(L"mesh021", 2, 0.1f);
 info.AddMeshParams(L"mesh014", 3);
 info.AddMeshParams(L"mesh009", 4, 0.1f);
 info.AddMeshParams(L"mesh012", 4, 0.6f);
 info.AddMeshParams(L"mesh004", 5);
 info.AddMeshParams(L"mesh005", 5);
 info.AddMeshParams(L"mesh016", 5);
 info.AddMeshParams(L"mesh011", 6, 0.1f);
 info.AddMeshParams(L"mesh003", 7);
 info.AddMeshParams(L"mesh006", 7);
 info.AddMeshParams(L"mesh007", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_bikini_camo", info);
}

void InitLaraDLCBikiniYellow(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh013", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh015", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh022", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh023", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh024", 1, 0.6f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh001", 2, 0.1f);
 info.AddMeshParams(L"mesh008", 2, 0.1f);
 info.AddMeshParams(L"mesh010", 2, 0.1f);
 info.AddMeshParams(L"mesh017", 2, 0.1f);
 info.AddMeshParams(L"mesh018", 2, 0.1f);
 info.AddMeshParams(L"mesh019", 2, 0.1f);
 info.AddMeshParams(L"mesh020", 2, 0.1f);
 info.AddMeshParams(L"mesh021", 2, 0.1f);
 info.AddMeshParams(L"mesh014", 3);
 info.AddMeshParams(L"mesh009", 4, 0.1f);
 info.AddMeshParams(L"mesh012", 4, 0.6f);
 info.AddMeshParams(L"mesh004", 5);
 info.AddMeshParams(L"mesh005", 5);
 info.AddMeshParams(L"mesh016", 5);
 info.AddMeshParams(L"mesh011", 6, 0.1f);
 info.AddMeshParams(L"mesh003", 7);
 info.AddMeshParams(L"mesh006", 7);
 info.AddMeshParams(L"mesh007", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_bikini_yellow", info);
}

void InitLaraDLCCasualExplorer(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh003", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh006", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh007", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh011", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh016", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh017", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh018", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh022", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh024", 1, 0.1f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh002", 2, 0.10f);
 info.AddMeshParams(L"mesh005", 2, 0.10f);
 info.AddMeshParams(L"mesh009", 2, 0.10f);
 info.AddMeshParams(L"mesh013", 2, 0.10f);
 info.AddMeshParams(L"mesh014", 2, 0.10f);
 info.AddMeshParams(L"mesh015", 2, 0.10f);
 info.AddMeshParams(L"mesh023", 3);
 info.AddMeshParams(L"mesh010", 4, 0.10f);
 info.AddMeshParams(L"mesh012", 4, 0.10f);
 info.AddMeshParams(L"mesh020", 4, 0.60f);
 info.AddMeshParams(L"mesh021", 4, 0.60f);
 info.AddMeshParams(L"mesh001", 5);
 info.AddMeshParams(L"mesh004", 5);
 info.AddMeshParams(L"mesh026", 5);
 info.AddMeshParams(L"mesh027", 5);
 info.AddMeshParams(L"mesh008", 6, 0.10f);
 info.AddMeshParams(L"mesh025", 7);
 info.AddMeshParams(L"mesh029", 7);
 info.AddMeshParams(L"mesh030", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_casual_explorer", info);
}

void InitLaraDLCClassic(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh001", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh004", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh007", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh009", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh010", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh011", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh018", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh002", 2, 0.10f);
 info.AddMeshParams(L"mesh005", 2, 0.10f);
 info.AddMeshParams(L"mesh013", 2, 0.10f);
 info.AddMeshParams(L"mesh015", 2, 0.10f);
 info.AddMeshParams(L"mesh016", 2, 0.10f);
 info.AddMeshParams(L"mesh017", 2, 0.10f);
 info.AddMeshParams(L"mesh020", 2, 0.10f);
 info.AddMeshParams(L"mesh008", 4, 0.60f);
 info.AddMeshParams(L"mesh012", 4, 0.10f);
 info.AddMeshParams(L"mesh019", 5);
 info.AddMeshParams(L"mesh023", 5);
 info.AddMeshParams(L"mesh014", 6, 0.10f);
 info.AddMeshParams(L"mesh006", 7);
 info.AddMeshParams(L"mesh021", 7);
 info.AddMeshParams(L"mesh024", 7);

 // add model information (two models)
 modelmap.insert_or_assign(L"lara_dlc_classic_fixed", info);
}

void InitLaraDLCDesignerFashionWinner(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh014", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh016", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh017", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh018", 1, 0.10f, 20.0f, 24.0f);
 info.AddMeshParams(L"mesh024", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh025", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh028", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh029", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh030", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh032", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh001", 2, 0.10f);
 info.AddMeshParams(L"mesh008", 2, 0.10f);
 info.AddMeshParams(L"mesh011", 2, 0.10f);
 info.AddMeshParams(L"mesh019", 2, 0.10f);
 info.AddMeshParams(L"mesh020", 2, 0.10f);
 info.AddMeshParams(L"mesh022", 2, 0.10f);
 info.AddMeshParams(L"mesh023", 2, 0.10f);
 info.AddMeshParams(L"mesh015", 3);
 info.AddMeshParams(L"mesh009", 4, 0.60f);
 info.AddMeshParams(L"mesh010", 4, 0.10f);
 info.AddMeshParams(L"mesh013", 4, 0.60f);
 info.AddMeshParams(L"mesh021", 4, 0.10f);
 info.AddMeshParams(L"mesh027", 4, 0.10f);
 info.AddMeshParams(L"mesh033", 4, 0.10f);
 info.AddMeshParams(L"mesh004", 5);
 info.AddMeshParams(L"mesh005", 5);
 info.AddMeshParams(L"mesh031", 5);
 info.AddMeshParams(L"mesh012", 6, 0.10f);
 info.AddMeshParams(L"mesh003", 7);
 info.AddMeshParams(L"mesh006", 7);
 info.AddMeshParams(L"mesh007", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_designer_fashion_winner", info);
}

void InitLaraDLCDesignerPeoplesChoice(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh018", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh020", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh023", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh025", 1, 0.30f, 24.0f, 24.0f);
 info.AddMeshParams(L"mesh026", 1, 0.30f, 24.0f, 24.0f);
 info.AddMeshParams(L"mesh027", 1, 0.30f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh003", 2, 0.10f);
 info.AddMeshParams(L"mesh010", 2, 0.10f);
 info.AddMeshParams(L"mesh012", 2, 0.10f);
 info.AddMeshParams(L"mesh014", 2, 0.10f);
 info.AddMeshParams(L"mesh015", 2, 0.10f);
 info.AddMeshParams(L"mesh016", 2, 0.10f);
 info.AddMeshParams(L"mesh019", 2, 0.10f);
 info.AddMeshParams(L"mesh024", 3);
 info.AddMeshParams(L"mesh007", 4, 0.10f);
 info.AddMeshParams(L"mesh008", 4, 0.10f);
 info.AddMeshParams(L"mesh009", 4, 0.10f);
 info.AddMeshParams(L"mesh022", 4, 0.60f);
 info.AddMeshParams(L"mesh002", 5);
 info.AddMeshParams(L"mesh006", 5);
 info.AddMeshParams(L"mesh017", 5);
 info.AddMeshParams(L"mesh011", 6, 0.10f);
 info.AddMeshParams(L"mesh001", 7);
 info.AddMeshParams(L"mesh004", 7);
 info.AddMeshParams(L"mesh013", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_designer_peoples_choice", info);
}

void InitLaraDLCDrySuitRed(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"arms",       1, 0.30f, 20.0f, 20.0f);
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 10.0f);
 info.AddMeshParams(L"boots1",     1, 0.10f,  8.0f, 16.0f);
 info.AddMeshParams(L"boots2",     1, 0.10f,  8.0f,  8.0f);
 info.AddMeshParams(L"gear2",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear3",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear6",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"legs",       1, 0.30f, 20.0f, 40.0f);
 info.AddMeshParams(L"suit",       1, 0.30f, 24.0f, 48.0f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"hands",      2, 0.10f);
 info.AddMeshParams(L"light",      2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"neck",       2, 0.10f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"gear5",      4, 0.10f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.20f);
 info.AddMeshParams(L"zipper",     4, 0.10f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"gear1",      5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"gear4",      7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_drysuit_red", info);
}

void InitLaraDLCJungleShortsGrey(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh008", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh009", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh010", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh017", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh021", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh022", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh024", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh001", 2, 0.10f);
 info.AddMeshParams(L"mesh004", 2, 0.10f);
 info.AddMeshParams(L"mesh013", 2, 0.10f);
 info.AddMeshParams(L"mesh015", 2, 0.10f);
 info.AddMeshParams(L"mesh019", 2, 0.10f);
 info.AddMeshParams(L"mesh020", 2, 0.10f);
 info.AddMeshParams(L"mesh023", 2, 0.10f);
 info.AddMeshParams(L"mesh018", 3);
 info.AddMeshParams(L"mesh011", 4, 0.60f);
 info.AddMeshParams(L"mesh014", 4, 0.10f);
 info.AddMeshParams(L"mesh002", 5);
 info.AddMeshParams(L"mesh003", 5);
 info.AddMeshParams(L"mesh026", 5);
 info.AddMeshParams(L"mesh016", 6, 0.10f);
 info.AddMeshParams(L"mesh005", 7);
 info.AddMeshParams(L"mesh007", 7);
 info.AddMeshParams(L"mesh012", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_jungle_shorts_grey", info);
}

void InitLaraDLCLegend(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"mesh008", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh009", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh016", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh018", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh019", 1, 0.01f, 24.0f, 32.0f);
 info.AddMeshParams(L"mesh020", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh021", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh025", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh028", 1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"mesh001", 2, 0.10f);
 info.AddMeshParams(L"mesh004", 2, 0.10f);
 info.AddMeshParams(L"mesh011", 2, 0.10f);
 info.AddMeshParams(L"mesh014", 2, 0.10f);
 info.AddMeshParams(L"mesh022", 2, 0.10f);
 info.AddMeshParams(L"mesh023", 2, 0.10f);
 info.AddMeshParams(L"mesh024", 2, 0.10f);
 info.AddMeshParams(L"mesh017", 3);
 info.AddMeshParams(L"mesh010", 4, 0.60f);
 info.AddMeshParams(L"mesh013", 4, 0.10f);
 info.AddMeshParams(L"mesh002", 5);
 info.AddMeshParams(L"mesh003", 5);
 info.AddMeshParams(L"mesh027", 5);
 info.AddMeshParams(L"mesh015", 6, 0.10f);
 info.AddMeshParams(L"mesh005", 7);
 info.AddMeshParams(L"mesh007", 7);
 info.AddMeshParams(L"mesh012", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_legend_fixed", info);
}

void InitLaraDLCWetSuitBlue(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"arms",         1, 0.30f, 20.0f, 20.0f);
 info.AddMeshParams(L"backpack",     1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"belts",        1, 0.10f, 10.0f, 20.0f);
 info.AddMeshParams(L"collar",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear2",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear3",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear6",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"suit",         1, 0.30f, 24.0f, 48.0f);
 info.AddMeshParams(L"suitstripes1", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"suitstripes2", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"body",         2, 0.10f);
 info.AddMeshParams(L"eyeducts",     2, 0.10f);
 info.AddMeshParams(L"face",         2, 0.10f);
 info.AddMeshParams(L"feet",         2, 0.10f);
 info.AddMeshParams(L"hands",        2, 0.10f);
 info.AddMeshParams(L"legs",         2, 0.10f);
 info.AddMeshParams(L"light",        2, 0.10f);
 info.AddMeshParams(L"mouth",        2, 0.10f);
 info.AddMeshParams(L"ribbon",       2, 0.10f);
 info.AddMeshParams(L"backring",     3);
 info.AddMeshParams(L"eyeirises",    4, 0.10f);
 info.AddMeshParams(L"gear5",        4, 0.10f);
 info.AddMeshParams(L"hair1",        4, 0.10f);
 info.AddMeshParams(L"metal",        4, 0.20f);
 info.AddMeshParams(L"zipper1",      4, 0.10f);
 info.AddMeshParams(L"zipper2",      4, 0.10f);
 info.AddMeshParams(L"eyewhites",    5);
 info.AddMeshParams(L"gear1",        5);
 info.AddMeshParams(L"hair2",        6, 0.10f);
 info.AddMeshParams(L"eyebrows",     7);
 info.AddMeshParams(L"eyelashes",    7);
 info.AddMeshParams(L"eyeshading",   7);
 info.AddMeshParams(L"gear4",        7);

 // add model information
 modelmap.insert_or_assign(L"lara_dlc_wetsuit_blue", info);
}

void InitLaraDrySuit(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"arms",       1, 0.30f, 20.0f, 20.0f);
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 10.0f);
 info.AddMeshParams(L"boots1",     1, 0.10f,  8.0f, 16.0f);
 info.AddMeshParams(L"boots2",     1, 0.10f,  8.0f,  8.0f);
 info.AddMeshParams(L"gear2",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear3",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear6",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"legs",       1, 0.30f, 20.0f, 40.0f);
 info.AddMeshParams(L"suit",       1, 0.30f, 24.0f, 48.0f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"hands",      2, 0.10f);
 info.AddMeshParams(L"light",      2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"neck",       2, 0.10f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"gear5",      4, 0.10f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.20f);
 info.AddMeshParams(L"zipper",     4, 0.10f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"gear1",      5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"gear4",      7);

 // add model information
 modelmap.insert_or_assign(L"lara_drysuit", info);
}

void InitLaraJungleHeavy(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 20.0f);
 info.AddMeshParams(L"boots",      1, 0.10f, 15.0f, 30.0f);
 info.AddMeshParams(L"gloves",     1, 0.10f,  5.0f,  7.0f);
 info.AddMeshParams(L"jacket",     1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"trousers",   1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"body",       2, 0.10f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"fingers",    2, 0.10f);
 info.AddMeshParams(L"light1",     2, 0.10f);
 info.AddMeshParams(L"light2",     2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.60f);
 info.AddMeshParams(L"zipper1",    4, 0.10f);
 info.AddMeshParams(L"zipper2",    4, 0.10f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_jungle_heavy", info);
}

void InitLaraJunglePants(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 20.0f);
 info.AddMeshParams(L"boots",      1, 0.10f, 15.0f, 30.0f);
 info.AddMeshParams(L"gloves",     1, 0.10f,  5.0f,  7.0f);
 info.AddMeshParams(L"trousers",   1, 0.10f, 24.0f, 24.0f);
 info.AddMeshParams(L"tshirt",     1, 0.10f, 24.0f, 48.0f);
 info.AddMeshParams(L"body",       2, 0.10f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"fingers",    2, 0.10f);
 info.AddMeshParams(L"light1",     2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"xxxx",       3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.60f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_jungle_pants", info);
}

void InitLaraJungleShorts(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 20.0f);
 info.AddMeshParams(L"boots",      1, 0.10f, 15.0f, 30.0f);
 info.AddMeshParams(L"gloves",     1, 0.10f,  5.0f,  7.0f);
 info.AddMeshParams(L"shorts",     1, 0.10f, 24.0f, 36.0f);
 info.AddMeshParams(L"tshirt",     1, 0.10f, 24.0f, 48.0f);
 info.AddMeshParams(L"body",       2, 0.10f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"fingers",    2, 0.10f);
 info.AddMeshParams(L"light1",     2, 0.10f);
 info.AddMeshParams(L"light2",     2, 0.10f);
 info.AddMeshParams(L"legs",       2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.60f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);

 // add model information
 modelmap.insert_or_assign(L"lara_jungle_shorts", info);
}

void InitLaraSnowHeavy(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 20.0f);
 info.AddMeshParams(L"boots1",     1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"boots3",     1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"gloves",     1, 0.10f, 20.0f, 20.0f);
 info.AddMeshParams(L"jacket",     1, 0.25f, 32.0f, 32.0f);
 info.AddMeshParams(L"sleeves",    1, 0.25f, 20.0f, 20.0f);
 info.AddMeshParams(L"trousers",   1, 0.05f, 16.0f, 16.0f);
 info.AddMeshParams(L"body",       2, 0.10f);
 info.AddMeshParams(L"boots2",     2, 0.60f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"light1",     2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"sweater",    2, 0.10f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"light2",     4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.60f);
 info.AddMeshParams(L"zipper1",    4, 0.10f);
 info.AddMeshParams(L"zipper2",    4, 0.10f);
 info.AddMeshParams(L"boots4",     5);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"fur1",       5);
 info.AddMeshParams(L"fur2",       6, 0.10f);
 info.AddMeshParams(L"fur3",       6, 0.10f);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"boots5",     9);

 // add model information
 modelmap.insert_or_assign(L"lara_snow_heavy", info);
}

void InitLaraSnowLight(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"backpack",   1, 0.10f, 12.0f, 16.0f);
 info.AddMeshParams(L"belts",      1, 0.10f, 10.0f, 20.0f);
 info.AddMeshParams(L"boots1",     1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"boots3",     1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"gloves",     1, 0.10f, 20.0f, 20.0f);
 info.AddMeshParams(L"jacket",     1, 0.25f, 32.0f, 32.0f);
 info.AddMeshParams(L"sleeves",    1, 0.25f, 20.0f, 20.0f);
 info.AddMeshParams(L"trousers",   1, 0.05f, 16.0f, 16.0f);
 info.AddMeshParams(L"eyeducts",   2, 0.10f);
 info.AddMeshParams(L"light2",     2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"body",       2, 0.10f);
 info.AddMeshParams(L"boots2",     2, 0.60f);
 info.AddMeshParams(L"ribbon",     2, 0.10f);
 info.AddMeshParams(L"backring",   3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"light1",     4, 0.10f);
 info.AddMeshParams(L"zipper1",    4, 0.10f);
 info.AddMeshParams(L"zipper2",    4, 0.10f);
 info.AddMeshParams(L"hair1",      4, 0.10f);
 info.AddMeshParams(L"metal",      4, 0.60f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"boots4",     5);
 info.AddMeshParams(L"hair2",      6, 0.10f);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"boots5",     9);

 // add model information
 modelmap.insert_or_assign(L"lara_snow_light", info);
}

void InitLaraWetSuit(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"arms",         1, 0.30f, 20.0f, 20.0f);
 info.AddMeshParams(L"backpack",     1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"belts",        1, 0.10f, 10.0f, 10.0f);
 info.AddMeshParams(L"collar",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear2",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear3",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"gear6",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"suit",         1, 0.30f, 24.0f, 48.0f);
 info.AddMeshParams(L"suitstripes1", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"suitstripes2", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"body",         2, 0.10f);
 info.AddMeshParams(L"eyeducts",     2, 0.10f);
 info.AddMeshParams(L"face",         2, 0.10f);
 info.AddMeshParams(L"feet",         2, 0.10f);
 info.AddMeshParams(L"hands",        2, 0.10f);
 info.AddMeshParams(L"legs",         2, 0.10f);
 info.AddMeshParams(L"light",        2, 0.10f);
 info.AddMeshParams(L"mouth",        2, 0.10f);
 info.AddMeshParams(L"ribbon",       2, 0.10f);
 info.AddMeshParams(L"backring",     3);
 info.AddMeshParams(L"eyeirises",    4, 0.10f);
 info.AddMeshParams(L"gear5",        4, 0.10f);
 info.AddMeshParams(L"hair1",        4, 0.10f);
 info.AddMeshParams(L"metal",        4, 0.20f);
 info.AddMeshParams(L"zipper1",      4, 0.10f);
 info.AddMeshParams(L"zipper2",      4, 0.10f);
 info.AddMeshParams(L"eyewhites",    5);
 info.AddMeshParams(L"gear1",        5);
 info.AddMeshParams(L"hair2",        6, 0.10f);
 info.AddMeshParams(L"eyebrows",     7);
 info.AddMeshParams(L"eyelashes",    7);
 info.AddMeshParams(L"eyeshading",   7);
 info.AddMeshParams(L"gear4",        7);

 // add model information
 modelmap.insert_or_assign(L"lara_wetsuit", info);
}

void InitLaraWetSuitNoGear(void)
{
 // base mesh properties
 XNAClassicModel info;
 InitLara(info);

 // mesh properties
 info.AddMeshParams(L"arms",         1, 0.30f, 20.0f, 20.0f);
 info.AddMeshParams(L"collar",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"suit",         1, 0.30f, 24.0f, 48.0f);
 info.AddMeshParams(L"suitstripes1", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"suitstripes2", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"body",         2, 0.10f);
 info.AddMeshParams(L"eyeducts",     2, 0.10f);
 info.AddMeshParams(L"face",         2, 0.10f);
 info.AddMeshParams(L"feet",         2, 0.10f);
 info.AddMeshParams(L"hands",        2, 0.10f);
 info.AddMeshParams(L"legs",         2, 0.10f);
 info.AddMeshParams(L"mouth",        2, 0.10f);
 info.AddMeshParams(L"ribbon",       2, 0.10f);
 info.AddMeshParams(L"eyeirises",    4, 0.10f);
 info.AddMeshParams(L"hair1",        4, 0.10f);
 info.AddMeshParams(L"zipper1",      4, 0.10f);
 info.AddMeshParams(L"zipper2",      4, 0.10f);
 info.AddMeshParams(L"eyewhites",    5);
 info.AddMeshParams(L"hair2",        6, 0.10f);
 info.AddMeshParams(L"eyebrows",     7);
 info.AddMeshParams(L"eyelashes",    7);
 info.AddMeshParams(L"eyeshading",   7);

 // add model information
 modelmap.insert_or_assign(L"lara_wetsuit_nogear", info);
}

void InitMayanThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh1a", 2, 0.20f);
 info.AddMeshParams(L"mesh1b", 2, 0.20f);
 info.AddMeshParams(L"mesh2",  2, 0.20f);
 info.AddMeshParams(L"mesh5",  2, 0.20f);
 info.AddMeshParams(L"mesh6",  2, 0.20f);
 info.AddMeshParams(L"mesh8",  2, 0.20f);
 info.AddMeshParams(L"mesh3",  4, 0.20f);
 info.AddMeshParams(L"mesh7",  6, 0.20f);
 info.AddMeshParams(L"mesh4",  7);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw 2");
 info.AddCameraTarget(L"body upper", L"arm left shoulder 1", L"arm right shoulder 1");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"mayan_thrall", info);
}

void InitMercenary(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"face",     2, 0.10f);
 info.AddMeshParams(L"belts",    2, 0.10f);
 info.AddMeshParams(L"tshirt",   2, 0.05f);
 info.AddMeshParams(L"arms",     2, 0.10f);
 info.AddMeshParams(L"trousers", 2, 0.10f);
 info.AddMeshParams(L"boots",    2, 0.10f);
 info.AddMeshParams(L"hair1",    8, 0.10f);
 info.AddMeshParams(L"hair2",    8, 0.10f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"arm left shoulder 1", L"arm right shoulder 1");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"mercenary", info);
}

void InitMjolnir(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mjolnir", 5);

 // set camera targets
 info.AddCameraTarget(L"mjolnir", L"mjolnir");

 // add model information
 modelmap.insert_or_assign(L"mjolnir", info);
}

void InitMotorcycle(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh1", 4, 0.50f);
 info.AddMeshParams(L"mesh2", 4, 0.50f);
 info.AddMeshParams(L"mesh3", 4, 0.20f);
 info.AddMeshParams(L"mesh6", 4, 0.20f);
 info.AddMeshParams(L"mesh4", 5);
 info.AddMeshParams(L"mesh5", 6, 1.00f);

 // set camera targets
 info.AddCameraTarget(L"body", L"body");
 info.AddCameraTarget(L"headlight", L"fork front");
 info.AddCameraTarget(L"wheel front", L"wheel front");
 info.AddCameraTarget(L"wheel back", L"wheel back");

 // add model information
 modelmap.insert_or_assign(L"motorcycle", info);
}

void InitNagaGreen(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"teeth", 2, 0.20f);
 info.AddMeshParams(L"body",  2, 0.20f);
 info.AddMeshParams(L"legs",  2, 0.20f);
 info.AddMeshParams(L"mouth", 2, 0.20f);
 info.AddMeshParams(L"eyes",  5);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"pelvis", L"pelvis");
 info.AddCameraTarget(L"tail", L"tail 4");

 // add model information
 modelmap.insert_or_assign(L"naga_green", info);
}

void InitNagaRed(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"teeth", 2, 0.20f);
 info.AddMeshParams(L"body",  2, 0.20f);
 info.AddMeshParams(L"legs",  2, 0.20f);
 info.AddMeshParams(L"mouth", 2, 0.20f);
 info.AddMeshParams(L"eyes",  5);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"pelvis", L"pelvis");
 info.AddCameraTarget(L"tail", L"tail 4");

 // add model information
 modelmap.insert_or_assign(L"naga_red", info);
}

void InitNatla(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"arms",           1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"detail3",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"jacket",         1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"trousers",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"wingbonesleft",  1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"wingbonesright", 1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"wingleft",       1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"wingright",      1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"detail1",        2, 0.10f);
 info.AddMeshParams(L"detail2",        2, 0.10f);
 info.AddMeshParams(L"detail4",        2, 0.10f);
 info.AddMeshParams(L"detail5",        2, 0.10f);
 info.AddMeshParams(L"face",           2, 0.10f);
 info.AddMeshParams(L"feet",           2, 0.10f);
 info.AddMeshParams(L"hair1",          2, 0.10f);
 info.AddMeshParams(L"handleft",       2, 0.10f);
 info.AddMeshParams(L"handright",      2, 0.10f);
 info.AddMeshParams(L"mouth",          4, 0.10f);
 info.AddMeshParams(L"zipper1",        4, 0.10f);
 info.AddMeshParams(L"zipper2",        4, 0.10f);
 info.AddMeshParams(L"eyes",           5);
 info.AddMeshParams(L"label",          5);
 info.AddMeshParams(L"eyebrows",       7);
 info.AddMeshParams(L"eyelashes",      7);
 info.AddMeshParams(L"eyeshading",     7);
 info.AddMeshParams(L"hair2",          7);
 info.AddMeshParams(L"hair3",          8, 0.10f);
 info.AddMeshParams(L"hair4",          8, 0.10f);
 info.AddMeshParams(L"hair5",          9);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"unused007");
 info.AddCameraTargetStandardLower();
 info.AddCameraTarget(L"wing left", L"wing left 7c");
 info.AddCameraTarget(L"wing right", L"wing right 7c");

 // add model information
 modelmap.insert_or_assign(L"natla", info);
}

void InitNatlaNoWings(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"arms",           1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"detail3",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"jacket",         1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"trousers",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"detail1",        2, 0.10f);
 info.AddMeshParams(L"detail2",        2, 0.10f);
 info.AddMeshParams(L"detail4",        2, 0.10f);
 info.AddMeshParams(L"detail5",        2, 0.10f);
 info.AddMeshParams(L"face",           2, 0.10f);
 info.AddMeshParams(L"feet",           2, 0.10f);
 info.AddMeshParams(L"hair1",          2, 0.10f);
 info.AddMeshParams(L"handleft",       2, 0.10f);
 info.AddMeshParams(L"handright",      2, 0.10f);
 info.AddMeshParams(L"mouth",          4, 0.10f);
 info.AddMeshParams(L"zipper1",        4, 0.10f);
 info.AddMeshParams(L"zipper2",        4, 0.10f);
 info.AddMeshParams(L"eyes",           5);
 info.AddMeshParams(L"label",          5);
 info.AddMeshParams(L"eyebrows",       7);
 info.AddMeshParams(L"eyelashes",      7);
 info.AddMeshParams(L"eyeshading",     7);
 info.AddMeshParams(L"hair2",          7);
 info.AddMeshParams(L"hair3",          8, 0.10f);
 info.AddMeshParams(L"hair4",          8, 0.10f);
 info.AddMeshParams(L"hair5",          9);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body upper", L"unused007");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"natla_nowings", info);
}

void InitPanther(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part4", 1, 0.15f, 32.0f, 32.0f);
 info.AddMeshParams(L"part2", 2, 0.15f);
 info.AddMeshParams(L"part3", 2, 0.15f);
 info.AddMeshParams(L"part1", 4, 0.15f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"pelvis", L"pelvis");
 info.AddCameraTarget(L"tail", L"tail 3");

 // add model information
 modelmap.insert_or_assign(L"panther", info);
}

void InitPantherThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 1, 0.20f, 8.0f, 8.0f);
 info.AddMeshParams(L"part3", 1, 0.20f, 8.0f, 8.0f);
 info.AddMeshParams(L"part2", 6, 0.20f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"pelvis", L"pelvis");
 info.AddCameraTarget(L"tail", L"tail 3");

 // add model information
 modelmap.insert_or_assign(L"panther_thrall", info);
}

void InitPoacher(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"clothes1",    1, 0.05f,  8.0f,  8.0f);
 info.AddMeshParams(L"gear",        1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"belt2",       1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"clothes3",    1, 0.05f,  8.0f,  8.0f);
 info.AddMeshParams(L"hat",         1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"neckerchief", 1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"gloves",      1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"trousers",    1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"clothleft",   1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"clothright",  1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"body",        2, 0.10f);
 info.AddMeshParams(L"metal",       2, 0.40f);
 info.AddMeshParams(L"zipper",      2, 0.10f);
 info.AddMeshParams(L"belt1",       2, 0.10f);
 info.AddMeshParams(L"hair1",       5);
 info.AddMeshParams(L"eyes",        5);
 info.AddMeshParams(L"clothes2",    5);
 info.AddMeshParams(L"hair2",       7);
 info.AddMeshParams(L"eyebrows",    7);
 info.AddMeshParams(L"eyelashes",   7);
 info.AddMeshParams(L"beard",       7);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw 2");
 info.AddCameraTarget(L"body upper", L"arm left shoulder 1", L"arm right shoulder 1");
 info.AddCameraTarget(L"body lower", L"pelvis");
 info.AddCameraTarget(L"hand left", L"arm left wrist");
 info.AddCameraTarget(L"hand right", L"arm right wrist");
 info.AddCameraTarget(L"knee left", L"leg left knee");
 info.AddCameraTarget(L"knee right", L"leg right knee");
 info.AddCameraTarget(L"foot left", L"leg left ankle");
 info.AddCameraTarget(L"foot right", L"leg right ankle");

 // add model information
 modelmap.insert_or_assign(L"poacher", info);
}

void InitPonytail(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"ponytail", 6, 0.20f);

 // set camera targets
 info.AddCameraTarget(L"ponytail", L"ponytail 1");

 // add model information
 modelmap.insert_or_assign(L"ponytail", info);
}

void InitRecorder(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"recorder", 4, 0.20f);
 info.AddMeshParams(L"glass",    4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"recorder", info);
}

void InitScion1(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"scion1a", 5);
 info.AddMeshParams(L"scion1b", 5);
 info.AddMeshParams(L"scion1c", 7);

 // set camera targets
 info.AddCameraTarget(L"scion1", L"root");

 // add model information
 modelmap.insert_or_assign(L"scion1", info);
}

void InitScion2(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"scion2a", 5);
 info.AddMeshParams(L"scion2b", 5);
 info.AddMeshParams(L"scion2c", 5);
 info.AddMeshParams(L"scion2d", 5);
 info.AddMeshParams(L"scion2e", 7);

 // set camera targets
 info.AddCameraTarget(L"scion2", L"root");

 // add model information
 modelmap.insert_or_assign(L"scion2", info);
}

void InitScion3(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"scion3a", 5);
 info.AddMeshParams(L"scion3b", 5);
 info.AddMeshParams(L"scion3c", 5);
 info.AddMeshParams(L"scion3d", 7);

 // set camera targets
 info.AddCameraTarget(L"scion3", L"root");

 // add model information
 modelmap.insert_or_assign(L"scion3", info);
}

void InitSharkBlue(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"body",   1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"mouth",  2, 0.10f);
 info.AddMeshParams(L"eyes",   4, 0.40f);
 info.AddMeshParams(L"teeth",  5);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 4");
 info.AddCameraTarget(L"fin left", L"fin left 2");
 info.AddCameraTarget(L"fin right", L"fin right 2");
 info.AddCameraTarget(L"fin back", L"fin back root");

 // add model information
 modelmap.insert_or_assign(L"shark_blue", info);
}

void InitSharkGreenland(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"body",   1, 0.20f, 12.0f, 12.0f);
 info.AddMeshParams(L"eyes",   4, 0.40f);
 info.AddMeshParams(L"teeth",  5);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 4");
 info.AddCameraTarget(L"fin left", L"fin left 2");
 info.AddCameraTarget(L"fin right", L"fin right 2");
 info.AddCameraTarget(L"fin back", L"fin back root");

 // add model information
 modelmap.insert_or_assign(L"shark_greenland", info);
}

void InitShip(void)
{
 // mesh properties
 XNAClassicModel info;
 for(int i = 1; i <= 244; i++) {
     std::wstringstream ss;
     ss << L"part" << std::setfill(L'0') << std::setw(3) << i;
     switch(i) {
       case(96) : case(151) : case(239) : case(240) : case(242) : case(243) : case(244) : {
            info.AddMeshParams(ss.str().c_str(), 16);
            break;
           }
       case(69) : case(81): case(241) : {
            info.AddMeshParams(ss.str().c_str(), 18);
            break;
           }
      case(101) : case(102) : case(104) : case(108) : case(109) : case(112) : case(113) :
      case(114) : case(117) : case(118) : case(124) : case(125) : case(127) : case(155) :
      case(156) : case(160) : case(161) : case(165) : case(171) : case(172) : case(182) :
      case(189) : case(190) : case(198) : case(199) : case(213) : case(214) : case(226) : {
            info.AddMeshParams(ss.str().c_str(), 19);
            break;
           }
       default : {
            info.AddMeshParams(ss.str().c_str(), 17);
            break;
           }
      }
    }
 

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"ship", info);
}

void InitShipNatlaRoom(void)
{
 // mesh properties
 XNAClassicModel info;
 for(int i = 1; i <= 55; i++) {
     std::wstringstream ss;
     ss << L"Mesh" << std::setfill(L'0') << std::setw(3) << i;
     switch(i) {
       case(31) : case(51) : case(52) : case(53) : {
            info.AddMeshParams(ss.str().c_str(), 16);
            break;
           }
       case( 5) : case( 6) : case(33) : case(50) : {
            info.AddMeshParams(ss.str().c_str(), 19);
            break;
           }
       case(47) : case(48) : case(49) : { // these do not exist
            break;
           }
       default : {
            info.AddMeshParams(ss.str().c_str(), 17);
            break;
           }
      }
    }

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"ship_natla_room", info);
}

void InitSkyDomeThailandSea(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"dome1",      7); // half sphere map render group? RG_DOME
 info.AddMeshParams(L"dome2",      7); // half sphere map render group?
 info.AddMeshParams(L"dome3",      7); // half sphere map render group?
 info.AddMeshParams(L"dome4",      7); // half sphere map render group?
 info.AddMeshParams(L"clouds",     7); // what render group? RG_CLOUDS

 // set camera targets
 // none

 // add model information
 modelmap.insert_or_assign(L"skydome_thailand_sea", info);
}

void InitThailandMirrorStatueLeft(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"body",  17);
 info.AddMeshParams(L"fan1",  17);
 info.AddMeshParams(L"fan2",  17);
 info.AddMeshParams(L"cloth", 17);
 info.AddMeshParams(L"deco",  17);
 info.AddMeshParams(L"block", 17);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"thailand_mirror_statue_left", info);
}

void InitThailandMirrorStatueRight(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"body",  17);
 info.AddMeshParams(L"fan1",  17);
 info.AddMeshParams(L"fan2",  17);
 info.AddMeshParams(L"cloth", 17);
 info.AddMeshParams(L"deco",  17);
 info.AddMeshParams(L"block", 17);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"thailand_mirror_statue_right", info);
}

void InitTiger1(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh1", 1, 0.10f,  4.0f,  4.0f);
 info.AddMeshParams(L"mesh2", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"mesh4", 2, 0.10f);
 info.AddMeshParams(L"mesh3", 8, 0.10f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"pelvis", L"pelvis");
 info.AddCameraTarget(L"tail", L"tail 4");

 // add model information
 modelmap.insert_or_assign(L"tiger1", info);
}

void InitTiger2(void)
{
 // NOTE: In the original XNALara, there is no tiger2 model. The source
 // code actually loads tiger1 model if you try to load a model named tiger2.

 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"mesh1", 1, 0.10f,  4.0f,  4.0f);
 info.AddMeshParams(L"mesh2", 1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"mesh4", 2, 0.10f);
 info.AddMeshParams(L"mesh3", 8, 0.10f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw");
 info.AddCameraTarget(L"body", L"spine 2");
 info.AddCameraTarget(L"pelvis", L"pelvis");
 info.AddCameraTarget(L"tail", L"tail 4");

 // add model information
 modelmap.insert_or_assign(L"tiger2", info);
}

void InitVikingThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1",      1, 0.50f,  8.0f,  8.0f);
 info.AddMeshParams(L"part2",      1, 0.50f,  8.0f,  8.0f);
 info.AddMeshParams(L"part3",      1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part5",      1, 0.20f, 16.0f, 16.0f);
 info.AddMeshParams(L"part8",      1, 0.20f, 32.0f, 32.0f);
 info.AddMeshParams(L"part9",      1, 0.20f, 32.0f, 32.0f);
 info.AddMeshParams(L"part10",     1, 0.20f, 32.0f, 32.0f);
 info.AddMeshParams(L"part12",     7);
 info.AddMeshParams(L"part4",      8, 0.05f);
 info.AddMeshParams(L"part6",      8, 0.05f);
 info.AddMeshParams(L"part7",      8, 0.05f);
 info.AddMeshParams(L"part11",     8, 0.05f);
 
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
 modelmap.insert_or_assign(L"viking_thrall", info);
}

void InitWeaponAKRifle(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_ak_rifle", info);
}

void InitWeaponAssaultRifle(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part2",      4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_assault_rifle", info);
}

void InitWeaponGrenade(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 1, 0.30f, 4.0f, 4.0f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_grenade", info);
}

void InitWeaponHandgun(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_handgun", info);
}

void InitWeaponShotgun(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.40f);
 info.AddMeshParams(L"part2", 4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_shotgun", info);
}

void InitWeaponSpeargun(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.40f);
 info.AddMeshParams(L"part2", 4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_speargun", info);
}

void InitWeaponTranquilizer(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.40f);
 info.AddMeshParams(L"part2", 4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_tranquilizer", info);
}

void InitWeaponUzi(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.40f);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"weapon_uzi", info);
}

void InitWings(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"wingbonesleft",  1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"wingbonesright", 1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"wingleft",       1, 0.10f, 16.0f, 16.0f);
 info.AddMeshParams(L"wingright",      1, 0.10f, 16.0f, 16.0f);

 // set camera targets
 info.AddCameraTarget(L"wing left", L"wing left 7c");
 info.AddCameraTarget(L"wing right", L"wing right 7c");

 // add model information
 modelmap.insert_or_assign(L"wings", info);
}

void InitWinston(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"trousers",   1, 0.05f, 16.0f, 16.0f);
 info.AddMeshParams(L"vest",       1, 0.05f,  8.0f,  8.0f);
 info.AddMeshParams(L"suitleft",   1, 0.05f, 16.0f, 16.0f);
 info.AddMeshParams(L"suitright",  1, 0.05f, 16.0f, 16.0f);
 info.AddMeshParams(L"shoes",      1, 0.10f,  4.0f,  4.0f);
 info.AddMeshParams(L"hands",      1, 0.10f,  8.0f,  8.0f);
 info.AddMeshParams(L"shirt",      1, 0.05f,  8.0f,  8.0f);
 info.AddMeshParams(L"sleeves",    1, 0.05f, 12.0f, 12.0f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"hair1",      2, 0.10f);
 info.AddMeshParams(L"face",       3);
 info.AddMeshParams(L"eyes",       4, 0.10f);
 info.AddMeshParams(L"eyeshading", 7);
 info.AddMeshParams(L"eyebrows",   7);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"hair2",      9);

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
 modelmap.insert_or_assign(L"winston", info);
}

void InitWraithStone(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1", 4, 0.20f);
 info.AddMeshParams(L"part2", 7);

 // set camera targets
 info.AddCameraTarget(L"stone", L"stone");

 // add model information
 modelmap.insert_or_assign(L"wraith_stone", info);
}

void InitYacht(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part1",  16);
 info.AddMeshParams(L"part2",  16);
 info.AddMeshParams(L"part3",  16);
 info.AddMeshParams(L"part4",  16);
 info.AddMeshParams(L"part12", 16);
 info.AddMeshParams(L"part23", 16);
 info.AddMeshParams(L"part24", 16);
 info.AddMeshParams(L"part25", 16);
 info.AddMeshParams(L"part44", 16);
 info.AddMeshParams(L"part5",  17);
 info.AddMeshParams(L"part6",  17);
 info.AddMeshParams(L"part7",  17);
 info.AddMeshParams(L"part8",  17);
 info.AddMeshParams(L"part9",  17);
 info.AddMeshParams(L"part10", 17);
 info.AddMeshParams(L"part11", 17);
 info.AddMeshParams(L"part14", 17);
 info.AddMeshParams(L"part15", 17);
 info.AddMeshParams(L"part16", 17);
 info.AddMeshParams(L"part17", 17);
 info.AddMeshParams(L"part18", 17);
 info.AddMeshParams(L"part19", 17);
 info.AddMeshParams(L"part20", 17);
 info.AddMeshParams(L"part21", 17);
 info.AddMeshParams(L"part26", 17);
 info.AddMeshParams(L"part27", 17);
 info.AddMeshParams(L"part28", 17);
 info.AddMeshParams(L"part29", 17);
 info.AddMeshParams(L"part30", 17);
 info.AddMeshParams(L"part31", 17);
 info.AddMeshParams(L"part32", 17);
 info.AddMeshParams(L"part33", 17);
 info.AddMeshParams(L"part35", 17);
 info.AddMeshParams(L"part36", 17);
 info.AddMeshParams(L"part37", 17);
 info.AddMeshParams(L"part38", 17);
 info.AddMeshParams(L"part39", 17);
 info.AddMeshParams(L"part40", 17);
 info.AddMeshParams(L"part13", 18);
 info.AddMeshParams(L"part22", 18);
 info.AddMeshParams(L"part41", 18);
 info.AddMeshParams(L"part42", 18);

 // set camera targets
 info.AddCameraTarget(L"root", L"root");

 // add model information
 modelmap.insert_or_assign(L"yacht", info);
}

void InitYetiThrall(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part3",  1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part4",  1, 0.50f,  8.0f,  8.0f);
 info.AddMeshParams(L"part5a", 1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part5b", 1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part6",  1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part2",  2, 0.30f);
 info.AddMeshParams(L"part7",  2, 0.10f);
 info.AddMeshParams(L"part1a", 8, 0.10f);
 info.AddMeshParams(L"part1b", 8, 0.10f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw 2");
 info.AddCameraTarget(L"body upper", L"arm left shoulder 1", L"arm right shoulder 1");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"yeti_thrall", info);
}

void InitYetiThrallNoChains(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"part3",  1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part4",  1, 0.50f,  8.0f,  8.0f);
 info.AddMeshParams(L"part5a", 1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part5b", 1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part6",  1, 0.15f, 12.0f, 12.0f);
 info.AddMeshParams(L"part2",  2, 0.30f);
 info.AddMeshParams(L"part7",  2, 0.10f);
 info.AddMeshParams(L"part1a", 8, 0.10f);
 info.AddMeshParams(L"part1b", 8, 0.10f);

 // set camera targets
 info.AddCameraTarget(L"head", L"head jaw 2");
 info.AddCameraTarget(L"body upper", L"arm left shoulder 1", L"arm right shoulder 1");
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"yeti_thrall_nochains", info);
}

void InitZip(void)
{
 // mesh properties
 XNAClassicModel info;
 info.AddMeshParams(L"band",       1, 0.10f,  4.0f,  4.0f);
 info.AddMeshParams(L"hair",       1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"jacket",     1, 0.03f, 16.0f, 16.0f);
 info.AddMeshParams(L"shoes",      1, 0.10f, 12.0f, 12.0f);
 info.AddMeshParams(L"trousers",   1, 0.03f,  8.0f,  8.0f);
 info.AddMeshParams(L"face",       2, 0.10f);
 info.AddMeshParams(L"handleft",   2, 0.10f);
 info.AddMeshParams(L"handright",  2, 0.10f);
 info.AddMeshParams(L"mouth",      2, 0.10f);
 info.AddMeshParams(L"body",       3);
 info.AddMeshParams(L"eyeirises",  4, 0.10f);
 info.AddMeshParams(L"necklaces",  4, 0.10f);
 info.AddMeshParams(L"eyewhites",  5);
 info.AddMeshParams(L"watch",      5);
 info.AddMeshParams(L"eyelashes",  7);
 info.AddMeshParams(L"eyeshading", 7);

 // set camera targets
 info.AddCameraTargetStandardUpperM();
 info.AddCameraTargetStandardLower();

 // add model information
 modelmap.insert_or_assign(L"zip", info);
}

#pragma endregion CLASSIC_DEFINITIONS