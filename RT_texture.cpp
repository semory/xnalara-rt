#include "stdafx.h"
#include "RT_math.h"
#include "RT_texture.h"
#include "RT_win32.h"

//
// SAMPLERS
//
#pragma region SAMPLERS

auto sampler_BGRA = [](XNATextureData* ptr, float u, float v)
{
 // strip off to clamp (u,v) to [-1, +1]
 float intpart;
 u = modff(u, &intpart);
 v = modff(v, &intpart);

 // u and v should range from [0, 1]
 if(u < 0.0f) u += 1.0f;
 if(v < 0.0f) v += 1.0f;

 // obtain image coordinates from UV
 float x = (ptr->dx - 1)*u;
 float y = (ptr->dy - 1)*v;

 // convert to integer coords
 unsigned int x0 = static_cast<unsigned int>(x);
 unsigned int y0 = static_cast<unsigned int>(y);
 unsigned int x1 = x0 + 1;
 unsigned int y1 = y0 + 1;
 if(x1 == ptr->dx) x1 = x0;
 if(y1 == ptr->dy) y1 = y0;

 // modify x and y to unit variables
 float x_int = 0.0f;
 float y_int = 0.0f;
 x = std::modf(x, &x_int);
 y = std::modf(y, &y_int);
 float xy = x*y;

 // cached values
 unsigned int x0_offset = 4*x0;
 unsigned int x1_offset = 4*x1;
 unsigned int y0_offset = y0*ptr->pitch;
 unsigned int y1_offset = y1*ptr->pitch;

 // index of coordinate
 unsigned int i11 = y0_offset + x0_offset; // p11
 unsigned int i12 = y1_offset + x0_offset; // p12
 unsigned int i21 = y0_offset + x1_offset; // p21
 unsigned int i22 = y1_offset + x1_offset; // p22

 // sample image
 vector4D<unsigned char> result;

 // B channel
 float p11 = ptr->data[i11++];
 float p12 = ptr->data[i12++];
 float p21 = ptr->data[i21++];
 float p22 = ptr->data[i22++];
 float a = p21 - p11;
 float b = p12 - p11;
 float c = p22 - p12 - a;
 result.b = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // G channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.g = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // R channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.r = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // A channel
 p11 = ptr->data[i11];
 p12 = ptr->data[i12];
 p21 = ptr->data[i21];
 p22 = ptr->data[i22];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.a = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 return result;
};

auto sampler_BGRX = [](XNATextureData* ptr, float u, float v)
{
 // strip off to clamp (u,v) to [-1, +1]
 float intpart;
 u = modff(u, &intpart);
 v = modff(v, &intpart);

 // u and v should range from [0, 1]
 if(u < 0.0f) u += 1.0f;
 if(v < 0.0f) v += 1.0f;

 // obtain image coordinates from UV
 float x = (ptr->dx - 1)*u;
 float y = (ptr->dy - 1)*v;

 // convert to integer coords
 unsigned int x0 = static_cast<unsigned int>(x);
 unsigned int y0 = static_cast<unsigned int>(y);
 unsigned int x1 = x0 + 1;
 unsigned int y1 = y0 + 1;
 if(x1 == ptr->dx) x1 = x0;
 if(y1 == ptr->dy) y1 = y0;

 // modify x and y to unit variables
 float x_int = 0.0f;
 float y_int = 0.0f;
 x = std::modf(x, &x_int);
 y = std::modf(y, &y_int);
 float xy = x*y;

 // cached values
 unsigned int x0_offset = 4*x0;
 unsigned int x1_offset = 4*x1;
 unsigned int y0_offset = y0*ptr->pitch;
 unsigned int y1_offset = y1*ptr->pitch;

 // index of coordinate
 unsigned int i11 = y0_offset + x0_offset; // p11
 unsigned int i12 = y1_offset + x0_offset; // p12
 unsigned int i21 = y0_offset + x1_offset; // p21
 unsigned int i22 = y1_offset + x1_offset; // p22

 // sample image
 vector4D<unsigned char> result;

 // B channel
 float p11 = ptr->data[i11++];
 float p12 = ptr->data[i12++];
 float p21 = ptr->data[i21++];
 float p22 = ptr->data[i22++];
 float a = p21 - p11;
 float b = p12 - p11;
 float c = p22 - p12 - a;
 result.b = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // G channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.g = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // R channel
 p11 = ptr->data[i11];
 p12 = ptr->data[i12];
 p21 = ptr->data[i21];
 p22 = ptr->data[i22];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.r = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // A channel
 result.a = 255;
 return result;

 // // obtain image coordinates from UV
 // unsigned int x = static_cast<unsigned int>((ptr->dx - 1)*u);
 // unsigned int y = static_cast<unsigned int>((ptr->dy - 1)*v);
 // 
 // // index of coordinate
 // unsigned int index = y*ptr->pitch + 4*x;
 // 
 // // sample image
 // vector4D<unsigned char> result;
 // result.b = ptr->data[index++];
 // result.g = ptr->data[index++];
 // result.r = ptr->data[index];
 // result.a = 255;
 // return result;
};

auto sampler_BGR = [](XNATextureData* ptr, float u, float v)
{
 // strip off to clamp (u,v) to [-1, +1]
 float intpart;
 u = modff(u, &intpart);
 v = modff(v, &intpart);

 // u and v should range from [0, 1]
 if(u < 0.0f) u += 1.0f;
 if(v < 0.0f) v += 1.0f;

 // obtain image coordinates from UV
 float x = (ptr->dx - 1)*u;
 float y = (ptr->dy - 1)*v;

 // convert to integer coords
 unsigned int x0 = static_cast<unsigned int>(x);
 unsigned int y0 = static_cast<unsigned int>(y);
 unsigned int x1 = x0 + 1;
 unsigned int y1 = y0 + 1;
 if(x1 == ptr->dx) x1 = x0;
 if(y1 == ptr->dy) y1 = y0;

 // modify x and y to unit variables
 float x_int = 0.0f;
 float y_int = 0.0f;
 x = std::modf(x, &x_int);
 y = std::modf(y, &y_int);
 float xy = x*y;

 // cached values
 unsigned int x0_offset = 3*x0;
 unsigned int x1_offset = 3*x1;
 unsigned int y0_offset = y0*ptr->pitch;
 unsigned int y1_offset = y1*ptr->pitch;

 // index of coordinate
 unsigned int i11 = y0_offset + x0_offset; // p11
 unsigned int i12 = y1_offset + x0_offset; // p12
 unsigned int i21 = y0_offset + x1_offset; // p21
 unsigned int i22 = y1_offset + x1_offset; // p22

 // sample image
 vector4D<unsigned char> result;

 // B channel
 float p11 = ptr->data[i11++];
 float p12 = ptr->data[i12++];
 float p21 = ptr->data[i21++];
 float p22 = ptr->data[i22++];
 float a = p21 - p11;
 float b = p12 - p11;
 float c = p22 - p12 - a;
 result.b = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // G channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.g = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // R channel
 p11 = ptr->data[i11];
 p12 = ptr->data[i12];
 p21 = ptr->data[i21];
 p22 = ptr->data[i22];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.r = static_cast<unsigned char>(a*x + b*y + c*xy + p11);

 // A channel
 result.a = 255;
 return result;

 // // obtain image coordinates from UV
 // unsigned int x = static_cast<unsigned int>((ptr->dx - 1)*u);
 // unsigned int y = static_cast<unsigned int>((ptr->dy - 1)*v);
 // 
 // // index of coordinate
 // unsigned int index = y*ptr->pitch + 3*x;
 // 
 // // sample image
 // vector4D<unsigned char> result;
 // result.b = ptr->data[index++];
 // result.g = ptr->data[index++];
 // result.r = ptr->data[index];
 // result.a = 255;
 // return result;
};

#pragma endregion

//
// TEXTURE MANAGEMENT OBJECT
//
#pragma region TEXTURE_MANAGEMENT_OBJECT

class XNATextureManager {
 private :
  struct XNATextureRef {
   XNATextureData data;
   unsigned int refs;
  };
  typedef std::unordered_map<std::wstring, XNATextureRef> hashmap_type;
  hashmap_type hashmap;
 public :
  bool LoadTexture(const wchar_t* filename, XNATextureData** out);
  bool FreeTexture(const wchar_t* filename);
  const XNATextureData* FindTexture(const wchar_t* filename)const;
 public :
  XNATextureManager();
 ~XNATextureManager();
 private :
  XNATextureManager(const XNATextureManager&) = delete;
  void operator =(const XNATextureManager&) = delete;
};

XNATextureManager::XNATextureManager()
{
}

XNATextureManager::~XNATextureManager()
{
}

bool XNATextureManager::LoadTexture(const wchar_t* filename, XNATextureData** out)
{
 // lookup filename in hash table
 if(!filename || !out) return error("", __FILE__, __LINE__);
 auto entry = hashmap.find(filename);

 // filename is not in hash table, create new texture
 if(entry == std::end(hashmap))
   {
    // open image file
    using namespace std;
    ifstream ifile(filename, ios::binary);
    if(!ifile) {
      std::wcout << filename << std::endl;
       return error("", __FILE__, __LINE__);
      }

    // compute filesize
    ifile.seekg(0, ios::end);
    size_t filesize = (size_t)ifile.tellg();
    ifile.seekg(0, ios::beg);
    if(ifile.fail()) return error("", __FILE__, __LINE__);

    // must have at least a header
    if(filesize < 0x20) return error("", __FILE__, __LINE__);

    // read DDS signature + header
    DWORD ddsh[32];
    ifile.read((char*)&ddsh[0], 32*sizeof(DWORD));
    if(ifile.fail()) return error("", __FILE__, __LINE__);

    // validate header
    if(ddsh[0] != 0x20534444ul) return error("", __FILE__, __LINE__);
    if(!ddsh[3] || !ddsh[4]) return error("", __FILE__, __LINE__); // allow this?
    if(!ddsh[5]) return error("", __FILE__, __LINE__);

    // read data
    size_t datasize = filesize - 128ul; // filesize - DDS_header_size
    unique_ptr<unsigned char[]> data(new unsigned char[datasize]);
    ifile.read((char*)data.get(), datasize);
    if(ifile.fail()) return error("", __FILE__, __LINE__);

    // determine texture format
    XNATextureFormat format;
    if(ddsh[22] == 24) {
       if(ddsh[20] == 0x40) format = XNATextureFormat::BGR;
       else return error("", __FILE__, __LINE__);
      }
    else if(ddsh[22] == 32) {
       if(ddsh[20] == 0x40) format = XNATextureFormat::BGRX;
       else if(ddsh[20] == 0x41) format = XNATextureFormat::BGRA;
       else return error("", __FILE__, __LINE__);
      }
    else return error("", __FILE__, __LINE__);

    // assign sampler
    std::function<vector4D<unsigned char>(XNATextureData*, float, float)> sampler;
    switch(format) {
      case(XNATextureFormat::BGR)  : sampler = sampler_BGR;  break;
      case(XNATextureFormat::BGRX) : sampler = sampler_BGRX; break;
      case(XNATextureFormat::BGRA) : sampler = sampler_BGRA; break;
      default : return error("", __FILE__, __LINE__);
     }

    // switch(header.format) {
    //   case(XNATextureFormat::BGRA) : sampler = sampler_BGRA; break;
    //   case(XNATextureFormat::BGRX) : sampler = sampler_BGRX; break;
    //   case(XNATextureFormat::BGR) :  sampler = sampler_BGR; break;
    //   case(0x01) : break; // DXT1 = DXGI_FORMAT_BC1_UNORM (71)
    //   case(0x02) : break; // DXT3 = DXGI_FORMAT_BC2_UNORM (74)
    //   case(0x03) : break; // DXT5 = DXGI_FORMAT_BC3_UNORM (77)
    //   case(0x04) : break; // ATI1 = DXGI_FORMAT_BC4_UNORM (80)
    //   case(0x05) : break; // ATI2 = DXGI_FORMAT_BC5_UNORM (83)
    //   case(0x06) : break; // B5_G6_R5    = DXGI_FORMAT_B5G6R5_UNORM (85)
    //   case(0x07) : break; // B5_G5_R5_A1 = DXGI_FORMAT_B5G5R5A1_UNORM (86)
    //   case(0x08) : break; // B8_G8_R8_A8 = DXGI_FORMAT_B8G8R8A8_UNORM (87) (IMPORTANT)
    //   case(0x09) : break; // B8_G8_R8_X8 = DXGI_FORMAT_B8G8R8X8_UNORM (88) (IMPORTANT)
    //   case(0x0A) : break; // BC6H = DXGI_FORMAT_BC6H_UF16 (95)
    //   case(0x0B) : break; // BC7_ = DXGI_FORMAT_BC7_UNORM (98)
    //   case(0x0C) : break; // R8_G8_B8_A8 = DXGI_FORMAT_R8G8B8A8_UNORM (28)
    //   case(0x0D) : break;
    //   case(0x0E) : break;
    //   case(0x0F) : break;
    //   default : return error("", __FILE__, __LINE__);
    //  }

    // create texture reference
    XNATextureRef entry;
    entry.data.dx = ddsh[3];
    entry.data.dy = ddsh[4];
    entry.data.pitch = ddsh[5];
    entry.data.type = 0;
    entry.data.mipmap = 0;
    entry.data.format = format;
    entry.data.data = std::move(data);
    entry.data.sampler = sampler;
    entry.refs = 1;

    // insert and assign texture
    auto iter = hashmap.insert(hashmap_type::value_type(filename, std::move(entry)));
    if(iter.second == false) return error("", __FILE__, __LINE__);

    // pass ownership of unique_ptr to texture manager
    data.release();
    *out = &iter.first->second.data;
   }
 // reuse texture
 else {
    if(entry->second.refs == 0) return error("", __FILE__, __LINE__);
    entry->second.refs++;
    *out = &entry->second.data;
   }

 return true;
}

bool XNATextureManager::FreeTexture(const wchar_t* filename)
{
 // lookup filename in hash table
 auto entry = hashmap.find(std::wstring(filename));
 if(entry == std::end(hashmap)) return false;

 // a free request should NEVER have a reference count of zero
 XNATextureRef& resource = entry->second;
 if(resource.refs == 0) return false;

 // delete texture if no longer referenced
 resource.refs--;
 if(resource.refs == 0) {
    // if(resource.data.data) delete[] resource.data.data;
    // resource.data.data = nullptr;
    // resource.refs = 0;
    hashmap.erase(entry);
   }

 return true;
}

const XNATextureData* XNATextureManager::FindTexture(const wchar_t* filename)const
{
 auto entry = hashmap.find(filename);
 if(entry == std::end(hashmap)) return nullptr;
 return &entry->second.data;
}

#pragma endregion

//
// TEXTURE MANAGMENT FUNCTIONS
//
#pragma region TEXTURE_MANAGEMENT

static XNATextureManager globalTextureManager;

bool LoadTexture(const wchar_t* filename, XNATextureData** out)
{
 return globalTextureManager.LoadTexture(filename, out);
}

bool FreeTexture(const wchar_t* filename)
{
 return globalTextureManager.FreeTexture(filename);
}

const XNATextureData* FindTexture(const wchar_t* filename)
{
 return globalTextureManager.FindTexture(filename);
}

#pragma endregion