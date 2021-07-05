#include "stdafx.h"
#include "RT_math.h"
#include "RT_win32.h"
#include "RT_texture.h"
#include "RT_raytrace.h"
#include "RT_xnalara.h"

bool XNAReadBinaryString(std::ifstream& ifile, STDSTRINGW& out)
{
 // read length
 // TODO: make sure this doesn't go beyond 4 or 5 iterations!
 unsigned int value = 0;
 unsigned int shift = 0;
 while(true) {
       unsigned char byte = 0;
       ifile.read((char*)&byte, 1);
       if(ifile.fail()) return false;
       value |= (((unsigned int)(byte & 0x7F)) << shift);
       if(!(byte & 0x80)) break; // high bit is 1
       shift += 7;
      }

 // XNALara strings should not be longer than 512 bytes
 if(value > 512) return false;

 // read and convert UTF-8 string
 unsigned int size = value;
 if(size) {
    char data[512];
    ifile.read(&data[0], size);
    if(ifile.fail()) return false;
    data[size] = '\0';
    out = Unicode08ToUnicode16(data);
   }
 else out = L"";

 return true;
}

bool XNAReadUint16(std::ifstream& ifile, unsigned short& value)
{
 ifile.read((char*)&value, sizeof(value));
 if(ifile.fail()) return false;
 return true; 
}

bool XNAReadUint32(std::ifstream& ifile, unsigned int& value)
{
 ifile.read((char*)&value, sizeof(value));
 if(ifile.fail()) return false;
 return true; 
}

bool XNAReadUint32Array(std::ifstream& ifile, unsigned int* value, int n)
{
 if(!n) return false;
 ifile.read((char*)value, n*sizeof(unsigned int));
 if(ifile.fail()) return false;
 return true; 
}

bool XNAReadVector2D(std::ifstream& ifile, float* value)
{
 ifile.read((char*)value, 2*sizeof(float));
 if(ifile.fail()) return false;
 return true; 
}

bool XNAReadVector3D(std::ifstream& ifile, float* value)
{
 ifile.read((char*)value, 3*sizeof(float));
 if(ifile.fail()) return false;
 return true; 
}

bool XNAReadVector4D(std::ifstream& ifile, float* value)
{
 ifile.read((char*)value, 4*sizeof(float));
 if(ifile.fail()) return false;
 return true; 
}

bool XNAReadColor4D(std::ifstream& ifile, float* value)
{
 unsigned char temp[4];
 ifile.read((char*)temp, 4);
 if(ifile.fail()) return false;
 value[0] = static_cast<float>(temp[0])/255.0f;
 value[1] = static_cast<float>(temp[1])/255.0f;
 value[2] = static_cast<float>(temp[2])/255.0f;
 value[3] = static_cast<float>(temp[3])/255.0f;
 return true; 
}

bool XNAReadBlendIndices(std::ifstream& ifile, unsigned short* value)
{
 ifile.read((char*)value, 4*sizeof(unsigned short));
 if(ifile.fail()) return false;
 return true; 
}

bool XNAReadFace(std::ifstream& ifile, unsigned int* value)
{
 ifile.read((char*)value, 3*sizeof(unsigned int));
 if(ifile.fail()) return false;
 return true; 
}

#pragma region Extra Utilities

std::vector<std::wstring> XNAParseMeshName(const std::wstring& src)
{
 // returns a list of strings separated by '_'
 std::vector<std::wstring> retval;
 std::wstring item;
 for(size_t i = 0; i < src.length(); i++) {
     if(src[i] == L'_') {
        if(item.length()) {
           retval.push_back(item);
           item.erase();
          }
       }
     else
        item += src[i];
    }
 if(item.length()) retval.push_back(item);
 return retval;
}

bool XNAExtractMeshParams(const std::wstring& src, XNAMeshParams& out)
{
 // nothing to do
 if(!src.length()) return error("Invalid argument.", __FILE__, __LINE__);

 // separate string with '_' delimiter
 std::vector<std::wstring> parts = XNAParseMeshName(src);
 if(!parts.size()) return error("Invalid XNALara mesh name.", __FILE__, __LINE__);

 // useful lambda functions
 auto thorglow = [](const std::wstring& str) {
  if(str == L"thorglowbelt") return true;
  if(str == L"thorglowgauntletleft") return true;
  if(str == L"thorglowgauntletright") return true;
  return false;
 };
 auto thorgear = [](const std::wstring& str) {
  if(str == L"thorbelt1") return true;
  if(str == L"thorbelt2") return true;
  if(str == L"thorgauntletleft") return true;
  if(str == L"thorgauntletright") return true;
  return false;
 };
 auto handguns = [](const std::wstring& str) {
  if(str == L"handgunhandleft") return true;
  if(str == L"handgunhandright") return true;
  if(str == L"handgunholsterleft") return true;
  if(str == L"handgunholsterright") return true;
  return false;
 };

 // initialize output parameters
 out.render_group = 0;
 out.optional[0] = false;
 out.optional[1] = false;
 out.fullname = src;
 out.name.clear();
 out.params[0] = 1.0f;
 out.params[1] = 1.0f;
 out.params[2] = 1.0f;
 out.pivots.clear();
 out.shader = nullptr;

 // part is PROTECTED
 size_t index = 0;
 if(parts[index] == L"P" || parts[index] == L"p")
    return error("Model contains a protected render group.", __FILE__, __LINE__);

 // part is special render group
 if(thorglow(parts[index])) {
    out.render_group = 0xFFFDu; // thorglow
    out.name = parts[index++];
    out.shader = RG_thorglow;
   }
 else if(thorgear(parts[index])) {
    out.render_group = 0xFFFEu; // thorgear
    out.name = parts[index++];
    out.shader = RG_thorgear;
   }
 else if(handguns(parts[index])) {
    out.render_group = 0xFFFFu; // handguns
    out.name = parts[index++];
    out.shader = RG_handguns;
   }
 // part must be <render group>_<name> or <render group>_<optional><name>
 else {
   if(parts.size() == 1) return error("Invalid XNA mesh parameters.", __FILE__, __LINE__);
    out.render_group = StringToUint16(parts[index++].c_str()); 
    out.name = parts[index++];
    if(out.name[0] == L'+') {
       if(out.name.length() < 2) return error("Invalid XNA mesh parameters.", __FILE__, __LINE__);
       out.name = out.name.substr(1);
       out.optional[0] = true; // optional
       out.optional[1] = true; // visible
      }
    else if(out.name[0] == L'-') {
       if(out.name.length() < 2) return error("Invalid XNA mesh parameters.", __FILE__, __LINE__);
       out.name = out.name.substr(1);
       out.optional[0] = true;  // optional
       out.optional[1] = false; // not visible
      }
    else {
       out.optional[0] = false; // optional
       out.optional[1] = true;  // visible
      }
    // set shader
    switch(out.render_group) {
      case( 0) : out.shader = RG00; out.alpha = 0; break;
      case( 1) : out.shader = RG01; out.alpha = 0; break;
      case( 2) : out.shader = RG02; out.alpha = 0; break;
      case( 3) : out.shader = RG03; out.alpha = 0; break;
      case( 4) : out.shader = RG04; out.alpha = 0; break;
      case( 5) : out.shader = RG05; out.alpha = 0; break;
      case( 6) : out.shader = RG06; out.alpha = 1; break;
      case( 7) : out.shader = RG07; out.alpha = 1; break;
      case( 8) : out.shader = RG08; out.alpha = 1; break;
      case( 9) : out.shader = RG09; out.alpha = 1; break;
      case(10) : out.shader = RG10; out.alpha = 0; break;
      case(11) : out.shader = RG11; out.alpha = 0; break;
      case(12) : out.shader = RG12; out.alpha = 1; break;
      case(13) : out.shader = RG13; out.alpha = 0; break;
      case(14) : out.shader = RG14; out.alpha = 0; break;
      case(15) : out.shader = RG15; out.alpha = 1; break;
      case(16) : out.shader = RG16; out.alpha = 0; break;
      case(17) : out.shader = RG17; out.alpha = 0; break;
      case(18) : out.shader = RG18; out.alpha = 1; break;
      case(19) : out.shader = RG19; out.alpha = 1; break;
      case(20) : out.shader = RG20; out.alpha = 1; break;
      case(21) : out.shader = RG21; out.alpha = 1; break;
      case(22) : out.shader = RG22; out.alpha = 0; break;
      case(23) : out.shader = RG23; out.alpha = 1; break;
      case(24) : out.shader = RG24; out.alpha = 0; break;
      case(25) : out.shader = RG25; out.alpha = 1; break;
      case(26) : out.shader = RG26; out.alpha = 0; break;
      case(27) : out.shader = RG27; out.alpha = 1; break;
      case(28) : out.shader = RG28; out.alpha = 0; break;
      case(29) : out.shader = RG29; out.alpha = 1; break;
      case(30) : out.shader = RG30; out.alpha = 0; break;
      case(31) : out.shader = RG31; out.alpha = 1; break;
      case(32) : out.shader = RG32; out.alpha = 0; break;
      case(33) : out.shader = RG33; out.alpha = 1; break;
      case(34) : out.shader = RG34; out.alpha = 0; break;
      case(35) : out.shader = RG35; out.alpha = 1; break;
      case(36) : out.shader = RG36; out.alpha = 0; break;
      case(37) : out.shader = RG37; out.alpha = 1; break;
      case(38) : out.shader = RG38; out.alpha = 0; break;
      case(39) : out.shader = RG39; out.alpha = 1; break;
      case(40) : out.shader = RG40; out.alpha = 0; break;
      case(41) : out.shader = RG41; out.alpha = 1; break;
      case(42) : out.shader = RG42; out.alpha = 0; break;
      case(43) : out.shader = RG43; out.alpha = 1; break;
      default : return error("Invalid render group.", __FILE__, __LINE__);
     }
   }

 // read 1st parameter
 out.params[0] = StringToReal32(parts[index++].c_str());
 if(index == src.size()) return true;

 // read 2nd parameter
 out.params[1] = StringToReal32(parts[index++].c_str());
 if(index == src.size()) return true;

 // read 3rd parameter
 out.params[2] = StringToReal32(parts[index++].c_str());
 if(index == src.size()) return true;

 // read pivots
 for(size_t i = index; i < parts.size(); i++)
     out.pivots.push_back(parts[i]);

 return true;
}

bool XNAComputeTangents(XNAModel* model)
{
 // http://www.crytek.com/download/Triangle_mesh_tangent_space_calculation.pdf

 if(!model) return false;

 // for each mesh
 for(unsigned int mesh_index = 0; mesh_index < model->n_mesh; mesh_index++)
    {
     // initialize tangents and bitangents
     XNAMesh& mesh = model->meshlist[mesh_index];
     std::vector<vector3D<float>> T_list(mesh.n_vert);
     std::vector<vector3D<float>> B_list(mesh.n_vert);

     // for each texture channel
     for(unsigned int channel = 0; channel < mesh.n_channels; channel++)
        {
         // initialize T and B
         for(unsigned int i = 0; i < mesh.n_vert; i++) {
             T_list[i][0] = T_list[i][1] = T_list[i][2] = 0.0f;
             B_list[i][0] = B_list[i][1] = B_list[i][2] = 0.0f;
            }

         // for each face
         unsigned int degens = 0;
         for(unsigned int face_index = 0; face_index < mesh.n_face; face_index++)
            {
             // face vertices
             unsigned int i1 = mesh.faces[face_index].refs[0];
             unsigned int i2 = mesh.faces[face_index].refs[1];
             unsigned int i3 = mesh.faces[face_index].refs[2];
             vector3D<float> vx1(mesh.verts[i1].position);
             vector3D<float> vx2(mesh.verts[i2].position);
             vector3D<float> vx3(mesh.verts[i3].position);

             // face edges (position)
             vector3D<float> ve1 = vx2 - vx1;
             vector3D<float> ve2 = vx3 - vx1;

             // face UVs
             vector2D<float> uv1(mesh.verts[i1].uv[channel]);
             vector2D<float> uv2(mesh.verts[i2].uv[channel]);
             vector2D<float> uv3(mesh.verts[i3].uv[channel]);

             // face edges (uv)
             vector2D<float> te1 = uv2 - uv1; // x2 - x1
             vector2D<float> te2 = uv3 - uv1; // x3 - x1

             // must have valid UVs
             // if UV is degenerate, you can't normal map this face
             float scale = (te1[0]*te2[1] - te2[0]*te1[1]); // proportional to signed area
             if(std::abs(scale) < 1.0e-6f) {
                degens++;
                continue;
               }
             scale = 1.0f/scale;

             // compute T
             vector3D<float> s_dir;
             s_dir[0] = (te2[1]*ve1[0] - te1[1]*ve2[0])*scale;
             s_dir[1] = (te2[1]*ve1[1] - te1[1]*ve2[1])*scale;
             s_dir[2] = (te2[1]*ve1[2] - te1[1]*ve2[2])*scale;

             // compute B
             vector3D<float> t_dir;
             t_dir[0] = (te1[0]*ve2[0] - te2[0]*ve1[0])*scale;
             t_dir[1] = (te1[0]*ve2[1] - te2[0]*ve1[1])*scale;
             t_dir[2] = (te1[0]*ve2[2] - te2[0]*ve1[2])*scale;

             // average T
             T_list[i1] += s_dir;
             T_list[i2] += s_dir;
             T_list[i3] += s_dir;

             // average B
             B_list[i1] += t_dir;
             B_list[i2] += t_dir;
             B_list[i3] += t_dir;
            }

         // form an orthogonal frame
         degens = 0;
         for(size_t i = 0; i < mesh.n_vert; i++)
            {
             // normalize T
             vector3D<float> n(mesh.verts[i].normal);
             vector3D<float> t = T_list[i];

             // Gram–Schmidt orthonormalization
             vector3D_orthogonalize(mesh.verts[i].tangent[channel], &n[0], &t[0]);
             if(vector3D_normalize(mesh.verts[i].tangent[channel]) < 1.0e-6f) {
                mesh.verts[i].tangent[channel][0] = 0.0f;
                mesh.verts[i].tangent[channel][1] = 0.0f;
                mesh.verts[i].tangent[channel][2] = 0.0f;
                mesh.verts[i].tangent[channel][3] = 0.0f;
                degens++;
                continue;
               }

             // Y = Z cross X (B = N cross T)
             if(scalar_product(vector_product(n, t), B_list[i]) < 0.0f) mesh.verts[i].tangent[channel][3] = -1.0f;
             else mesh.verts[i].tangent[channel][3] = +1.0f;
            }
        }
    }

 return true;
}

#pragma endregion Extra Utilities

bool LoadXNAMeshBin(const wchar_t* filename, XNAModel* model)
{
 //
 // PHASE #1
 // READ FILE DATA
 //

 // open file
 using namespace std;
 if(!filename || !model) return false;
 ifstream ifile(filename, ios::binary);
 if(!ifile) return false;

 // pathname of file
 STDSTRINGW pathname = GetPathnameFromFilenameW(filename);
 STDSTRINGW shrtname = GetShortFilenameWithoutExtensionW(filename);

 // obtain filesize
 ifile.seekg(0, ios::end);
 size_t filesize = (size_t)ifile.tellg();
 ifile.seekg(0, ios::beg);
 if(ifile.fail()) return false;

 //
 // PHASE #2
 // READ SKELETON
 //

 // read number of joints
 unsigned int n_jnts;
 if(!XNAReadUint32(ifile, n_jnts)) return false;

 // create bone list
 std::vector<XNABone> bonelist;
 if(n_jnts) bonelist.resize(n_jnts);
 for(size_t i = 0; i < n_jnts; i++) {
     bonelist[i].name = L"";
     bonelist[i].parent = 0xFFFF;
     bonelist[i].position[0] = 0.0f;
     bonelist[i].position[1] = 0.0f;
     bonelist[i].position[2] = 0.0f;
    }

 // read joints
 for(unsigned int i = 0; i < n_jnts; i++) {
     if(!XNAReadBinaryString(ifile, bonelist[i].name)) return false;
     if(!XNAReadUint16(ifile, bonelist[i].parent)) return false;
     if(!XNAReadVector3D(ifile, bonelist[i].position)) return false;
    }

 //
 // PHASE #3
 // READ MESH LIST
 //

 // read number of meshes
 unsigned int n_mesh;
 if(!XNAReadUint32(ifile, n_mesh)) return false;

 // create mesh data
 std::vector<XNAMesh> meshlist;
 if(n_mesh) meshlist.resize(n_mesh);
 for(size_t i = 0; i < n_mesh; i++) {
     meshlist[i].n_channels = 0;
     meshlist[i].n_textures = 0;
     meshlist[i].n_vert = 0;
     meshlist[i].n_face = 0;
    }

 // read meshes
 uint32_t total_faces = 0;
 for(unsigned int i = 0; i < n_mesh; i++)
    {
     // read name
     XNAMesh& mesh = meshlist[i];
     std::wstring name;
     if(!XNAReadBinaryString(ifile, name)) return false;
     if(!XNAExtractMeshParams(name, mesh.params)) return false;

     // read number of UV channels
     // render groups 22, 23, 24, 25 are only ones that support multiple UV channels in XPS and only for AO maps
     if(!XNAReadUint32(ifile, mesh.n_channels)) return false;

     // read number of textures
     if(!XNAReadUint32(ifile, mesh.n_textures)) return false;

     // create texture data
     if(mesh.n_textures) mesh.textures.resize(mesh.n_textures);
     for(unsigned int j = 0; j < mesh.n_textures; j++)  {
         XNATexture& texture = mesh.textures[j];
         texture.name = L"";
         texture.channel = 0;
         texture.data = nullptr;
        }

     // read textures
     for(unsigned int j = 0; j < mesh.n_textures; j++) {
         // read texture filename
         XNATexture& texture = mesh.textures[j];
         if(!XNAReadBinaryString(ifile, texture.name)) return false;
         // read texture channels
         if(!XNAReadUint32(ifile, texture.channel)) return false;
         if(!(texture.channel < mesh.n_channels)) return false;
         // strip pathname
         texture.name = GetShortFilenameW(texture.name.c_str());
         // load texture
         STDSTRINGSTREAMW ss;
         ss << pathname << texture.name.c_str();
         if(!LoadTexture(ss.str().c_str(), &texture.data)) return false;
        }

     // read number of vertices
     if(!XNAReadUint32(ifile, mesh.n_vert)) return false;

     // read vertices
     if(mesh.n_vert) mesh.verts.resize(mesh.n_vert);
     for(unsigned int j = 0; j < mesh.n_vert; j++)
        {
         if(!XNAReadVector3D(ifile, mesh.verts[j].position)) return false;
         if(!XNAReadVector3D(ifile, mesh.verts[j].normal)) return false;
         if(!XNAReadColor4D(ifile, mesh.verts[j].color)) return false;
         for(unsigned int k = 0; k < mesh.n_channels; k++) if(!XNAReadVector2D(ifile, mesh.verts[j].uv[k])) return false;
         for(unsigned int k = 0; k < mesh.n_channels; k++) if(!XNAReadVector4D(ifile, mesh.verts[j].tangent[k])) return false;

         // read blend indices and blend weights
         if(n_jnts)
           {
            // read data
            if(!XNAReadBlendIndices(ifile, mesh.verts[j].blendindices)) return false;
            if(!XNAReadVector4D(ifile, mesh.verts[j].blendweights)) return false;

            // sum blendweights
            float sum = mesh.verts[j].blendweights[0];
            sum += mesh.verts[j].blendweights[1];
            sum += mesh.verts[j].blendweights[2];
            sum += mesh.verts[j].blendweights[3];

            // fix blendweights
            if(sum == 0.0f) {
               mesh.verts[j].blendweights[0] = 1.0f;
               mesh.verts[j].blendweights[1] = 0.0f;
               mesh.verts[j].blendweights[2] = 0.0f;
               mesh.verts[j].blendweights[3] = 0.0f;
              }
            else if(sum != 1.0f) {
               mesh.verts[j].blendweights[0] /= sum;
               mesh.verts[j].blendweights[1] /= sum;
               mesh.verts[j].blendweights[2] /= sum;
               mesh.verts[j].blendweights[3] /= sum;
              }
           }
        }

     // read number of faces
     if(!XNAReadUint32(ifile, mesh.n_face)) return false;

     // read faces
     mesh.faces.resize(mesh.n_face);
     for(unsigned int j = 0; j < mesh.n_face; j++)
         if(!XNAReadFace(ifile, mesh.faces[j].refs)) return false;

     // accumulate faces
     total_faces += mesh.n_face;
    }

 // accumulate faces
 std::vector<XNAGlobalFace> facelist(total_faces);
 uint32_t foffset = 0;
 for(uint32_t i = 0; i < meshlist.size(); i++) {
     for(uint32_t j = 0; j < meshlist[i].n_face; j++) {
         uint32_t findex = foffset + j;
         facelist[findex].refs[0] = meshlist[i].faces[j].refs[0];
         facelist[findex].refs[1] = meshlist[i].faces[j].refs[1];
         facelist[findex].refs[2] = meshlist[i].faces[j].refs[2];
         facelist[findex].mesh_index = i;
        }
     foffset += meshlist[i].n_face;
    }

 // assign model
 model->isXPS = false;
 model->header = XNAHeader();
 model->n_jnts = n_jnts;
 model->n_mesh = n_mesh;
 model->bonelist = std::move(bonelist);
 model->meshlist = std::move(meshlist);
 model->facelist = std::move(facelist);

 // success
 return true;
}

bool LoadXPSMeshBin(const wchar_t* filename, XNAModel* model)
{
 // look class XNALara.LoadOptionalInformations
 // lookup function WriteHeader()
 // lookup function XNALara.Model LoadModel
 // lookup function XNALara.MeshDesc LoadMesh()

 //
 // PHASE #1
 // READ FILE DATA
 //

 // open file
 using namespace std;
 if(!filename || !model) return false;
 ifstream ifile(filename, ios::binary);
 if(!ifile) return false;

 // pathname of file
 STDSTRINGW pathname = GetPathnameFromFilenameW(filename);
 STDSTRINGW shrtname = GetShortFilenameWithoutExtensionW(filename);

 // obtain filesize
 ifile.seekg(0, ios::end);
 size_t filesize = (size_t)ifile.tellg();
 ifile.seekg(0, ios::beg);
 if(ifile.fail()) return false;

 //
 // PHASE #2
 // READ XPS HEADER
 //

 // read magic number
 XNAHeader header;
 if(!XNAReadUint32(ifile, header.magic)) return false;
 if(header.magic != 0x04EEA0) return false;

 // read version
 if(!XNAReadUint16(ifile, header.version[0])) return false;
 if(!XNAReadUint16(ifile, header.version[1])) return false;

 // read XNAaraL
 if(!XNAReadBinaryString(ifile, header.creator)) return false;
 if(header.creator != L"XNAaraL") return false;

 // read unknown
 // appears to be size of data that follows
 if(!XNAReadUint32(ifile, header.n_item)) return false;

 // read strings
 if(!XNAReadBinaryString(ifile, header.rn)) return false;
 if(!XNAReadBinaryString(ifile, header.un)) return false;
 if(!XNAReadBinaryString(ifile, header.filename)) return false;

 // read optional information
 if(header.n_item) {
    header.optional.resize(header.n_item);
    if(!XNAReadUint32Array(ifile, &header.optional[0], header.n_item)) return false;
   }

 //
 // PHASE #3
 // READ SKELETON
 //

 // read number of joints
 unsigned int n_jnts = 0;
 if(!XNAReadUint32(ifile, n_jnts)) return false;

 // create bone list
 std::vector<XNABone> bonelist;
 if(n_jnts) bonelist.resize(n_jnts);
 for(size_t i = 0; i < n_jnts; i++) {
     bonelist[i].name = L"";
     bonelist[i].parent = 0xFFFF;
     bonelist[i].position[0] = 0.0f;
     bonelist[i].position[1] = 0.0f;
     bonelist[i].position[2] = 0.0f;
    }

 // read joints
 for(unsigned int i = 0; i < n_jnts; i++) {
     if(!XNAReadBinaryString(ifile, bonelist[i].name)) return false;
     if(!XNAReadUint16(ifile, bonelist[i].parent)) return false;
     if(!XNAReadVector3D(ifile, bonelist[i].position)) return false;
    }

 //
 // PHASE #4
 // READ MESH LIST
 //

 // read number of meshes
 unsigned int n_mesh;
 if(!XNAReadUint32(ifile, n_mesh)) return false;

 // create mesh data
 std::vector<XNAMesh> meshlist;
 if(n_mesh) meshlist.resize(n_mesh);
 for(size_t i = 0; i < n_mesh; i++) {
     meshlist[i].n_channels = 0;
     meshlist[i].n_textures = 0;
     meshlist[i].n_vert = 0;
     meshlist[i].n_face = 0;
    }

 // read meshes
 uint32_t total_faces = 0;
 for(unsigned int i = 0; i < n_mesh; i++)
    {
     // read name
     XNAMesh& mesh = meshlist[i];
     std::wstring name;
     if(!XNAReadBinaryString(ifile, name)) return error("", __FILE__, __LINE__);
     if(!XNAExtractMeshParams(name, mesh.params)) return error("", __FILE__, __LINE__);

     // read number of UV channels
     // render groups 22, 23, 24, 25 are only ones that support multiple UV channels in XPS and only for AO maps
     if(!XNAReadUint32(ifile, mesh.n_channels)) return error("", __FILE__, __LINE__);

     // read number of textures
     if(!XNAReadUint32(ifile, mesh.n_textures)) return error("", __FILE__, __LINE__);

     // create texture data
     if(mesh.n_textures) mesh.textures.resize(mesh.n_textures);
     for(unsigned int j = 0; j < mesh.n_textures; j++)  {
         XNATexture& texture = mesh.textures[j];
         texture.name = L"";
         texture.channel = 0;
         texture.data = nullptr;
        }

     // read textures
     for(unsigned int j = 0; j < mesh.n_textures; j++) {
         // read filename
         XNATexture& texture = mesh.textures[j];
         if(!XNAReadBinaryString(ifile, texture.name)) return error("", __FILE__, __LINE__);
         // read UV channel
         if(!XNAReadUint32(ifile, texture.channel)) return error("", __FILE__, __LINE__);
         if(!(texture.channel < mesh.n_channels)) return error("", __FILE__, __LINE__);
         // strip pathname
         texture.name = GetShortFilenameW(texture.name.c_str());
         // read texture data
         STDSTRINGSTREAMW ss;
         ss << pathname << texture.name.c_str();
         if(!LoadTexture(ss.str().c_str(), &texture.data)) return error("", __FILE__, __LINE__);
        }

     // read number of vertices
     if(!XNAReadUint32(ifile, mesh.n_vert)) return error("", __FILE__, __LINE__);

     // read vertices
     if(mesh.n_vert) mesh.verts.resize(mesh.n_vert);
     for(unsigned int j = 0; j < mesh.n_vert; j++)
        {
         if(!XNAReadVector3D(ifile, mesh.verts[j].position)) return error("", __FILE__, __LINE__);
         if(!XNAReadVector3D(ifile, mesh.verts[j].normal)) return error("", __FILE__, __LINE__);
         if(!XNAReadColor4D(ifile, mesh.verts[j].color)) return error("", __FILE__, __LINE__);
         for(unsigned int k = 0; k < mesh.n_channels; k++)
             if(!XNAReadVector2D(ifile, mesh.verts[j].uv[k]))
                return error("", __FILE__, __LINE__);

         // read tangents
         // NOTE: XPS files do not contain tangents. Just initialize to zero for now.
         for(unsigned int k = 0; k < mesh.n_channels; k++) {
             mesh.verts[j].tangent[k][0] = 0.0f;
             mesh.verts[j].tangent[k][1] = 0.0f;
             mesh.verts[j].tangent[k][2] = 0.0f;
             mesh.verts[j].tangent[k][3] = 0.0f;
            }

         // read blend indices and blend weights
         if(n_jnts)
           {
            // read data
            if(!XNAReadBlendIndices(ifile, &mesh.verts[j].blendindices[0])) return error("", __FILE__, __LINE__);
            if(!XNAReadVector4D(ifile, &mesh.verts[j].blendweights[0])) return error("", __FILE__, __LINE__);

            // sum blendweights
            float sum = mesh.verts[j].blendweights[0];
            sum += mesh.verts[j].blendweights[1];
            sum += mesh.verts[j].blendweights[2];
            sum += mesh.verts[j].blendweights[3];

            // fix blendweights
            if(sum == 0.0f) {
               mesh.verts[j].blendweights[0] = 1.0f;
               mesh.verts[j].blendweights[1] = 0.0f;
               mesh.verts[j].blendweights[2] = 0.0f;
               mesh.verts[j].blendweights[3] = 0.0f;
              }
            else if(sum != 1.0f) {
               mesh.verts[j].blendweights[0] /= sum;
               mesh.verts[j].blendweights[1] /= sum;
               mesh.verts[j].blendweights[2] /= sum;
               mesh.verts[j].blendweights[3] /= sum;
              }
           }
        }

     // read number of faces
     if(!XNAReadUint32(ifile, mesh.n_face)) return error("", __FILE__, __LINE__);

     // read faces
     mesh.faces.resize(mesh.n_face);
     for(unsigned int j = 0; j < mesh.n_face; j++)
         if(!XNAReadFace(ifile, &mesh.faces[j].refs[0])) return error("", __FILE__, __LINE__);

     // accumulate faces
     total_faces += mesh.n_face;
    }

 // accumulate faces
 std::vector<XNAGlobalFace> facelist(total_faces);
 uint32_t foffset = 0;
 for(uint32_t i = 0; i < meshlist.size(); i++) {
     for(uint32_t j = 0; j < meshlist[i].n_face; j++) {
         uint32_t findex = foffset + j;
         facelist[findex].refs[0] = meshlist[i].faces[j].refs[0];
         facelist[findex].refs[1] = meshlist[i].faces[j].refs[1];
         facelist[findex].refs[2] = meshlist[i].faces[j].refs[2];
         facelist[findex].mesh_index = i;
        }
     foffset += meshlist[i].n_face;
    }

 // assign model
 model->isXPS = true;
 model->header = header;
 model->n_jnts = n_jnts;
 model->n_mesh = n_mesh;
 model->bonelist = std::move(bonelist);
 model->meshlist = std::move(meshlist);
 model->facelist = std::move(facelist);

 // compute tangents
 if(!XNAComputeTangents(model)) return false;

 // success
 return true;
}

bool FreeXNAMesh(XNAModel* model)
{
 // there is nothing to do
 if(!model) return false;

 // clear header
 model->isXPS = false;
 model->header.magic = 0;
 model->header.version[0] = 0;
 model->header.version[1] = 0;
 model->header.creator = L"";
 model->header.n_item = 0;
 model->header.rn = L"";
 model->header.un = L"";
 model->header.filename = L"";
 model->header.optional.clear();

 // clear and free textures
 for(unsigned int i = 0; i < model->n_mesh; i++)
     for(unsigned int j = 0; j < model->meshlist[i].n_textures; j++)
         FreeTexture(model->meshlist[i].textures[j].name.c_str());

 // clear mesh data
 model->n_jnts = 0;
 model->n_mesh = 0;
 model->bonelist.clear();
 model->meshlist.clear();
 model->facelist.clear();

 return true;
}
