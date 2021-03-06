#ifndef __RT_OBSOLETE_H
#define __RT_OBSOLETE_H

/*
** The original XNALara mesh format had models that were hardcoded into the
** program. For example, rendering parameters were not stored in the mesh
** binaries but rather coded into .NET classes. It was a horrible model
** format at the time and the generic_item format ended up superceding it.
** However, support must be included for these files. Many of these older
** files also contain unused meshes -- like 'eyeballs' -- that must be
** processed but ultimately ignored.
*/

struct XNAClassicMeshParams {
 uint16_t render_group;
 float params[3];
 bool visible;
};

struct XNAClassicModel {
 std::map<STDSTRINGW, XNAClassicMeshParams> params;
 std::map<STDSTRINGW, XNACameraTarget> targets;
 void AddMeshParams(const STDSTRINGW& name, uint16_t rg, bool visible = true);
 void AddMeshParams(const STDSTRINGW& name, uint16_t rg, float a, bool visible = true);
 void AddMeshParams(const STDSTRINGW& name, uint16_t rg, float a, float b, float c, bool visible = true);
 void AddCameraTarget(const STDSTRINGW& name, const STDSTRINGW& b1);
 void AddCameraTarget(const STDSTRINGW& name, const STDSTRINGW& b1, const STDSTRINGW& b2);
 void AddCameraTargetStandardUpperF(void);
 void AddCameraTargetStandardUpperM(void);
 void AddCameraTargetStandardLower(void);
};

void InitClassicModels(void);
const XNAClassicModel* GetClassicModel(const STDSTRINGW& name);

#endif
