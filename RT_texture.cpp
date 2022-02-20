#include "stdafx.h"
#include "RT_win32.h"
#include "RT_math.h"
#include "RT_dds.h"
#include "RT_png.h"
#include "RT_texture.h"

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
 vector4D<float> result;

 // B channel
 float p11 = ptr->data[i11++];
 float p12 = ptr->data[i12++];
 float p21 = ptr->data[i21++];
 float p22 = ptr->data[i22++];
 float a = p21 - p11;
 float b = p12 - p11;
 float c = p22 - p12 - a;
 result.b = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // G channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.g = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // R channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.r = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // A channel
 p11 = ptr->data[i11];
 p12 = ptr->data[i12];
 p21 = ptr->data[i21];
 p22 = ptr->data[i22];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.a = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 return result;
};

auto sampler_BGRX = [](XNATextureData* ptr, float u, float v)
{
/*
 // strip off to clamp (u,v) to [-1, +1]
 float intpart;
 u = modff(u, &intpart);
 v = modff(v, &intpart);

 // u and v should range from [0, 1]
 if(u < 0.0f) u += 1.0f;
 if(v < 0.0f) v += 1.0f;

 // obtain image coordinates from UV
 unsigned int x = static_cast<unsigned int>((ptr->dx - 1)*u);
 unsigned int y = static_cast<unsigned int>((ptr->dy - 1)*v);
 
 // index of coordinate
 unsigned int index = y*ptr->pitch + 4*x;
 
 // sample image
 vector4D<float> result;
 result.b = ptr->data[index++]/255.0f;
 result.g = ptr->data[index++]/255.0f;
 result.r = ptr->data[index]/255.0f;
 result.a = 1.0f;
 return result;
*/

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
 vector4D<float> result;

 // B channel
 float p11 = ptr->data[i11++];
 float p12 = ptr->data[i12++];
 float p21 = ptr->data[i21++];
 float p22 = ptr->data[i22++];
 float a = p21 - p11;
 float b = p12 - p11;
 float c = p22 - p12 - a;
 result.b = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // G channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.g = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // R channel
 p11 = ptr->data[i11];
 p12 = ptr->data[i12];
 p21 = ptr->data[i21];
 p22 = ptr->data[i22];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.r = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // A channel
 result.a = 1.0f;
 return result;
};

auto sampler_BGR = [](XNATextureData* ptr, float u, float v)
{
/*
 // strip off to clamp (u,v) to [-1, +1]
 float intpart;
 u = modff(u, &intpart);
 v = modff(v, &intpart);

 // u and v should range from [0, 1]
 if(u < 0.0f) u += 1.0f;
 if(v < 0.0f) v += 1.0f;

 // obtain image coordinates from UV
 unsigned int x = static_cast<unsigned int>((ptr->dx - 1)*u);
 unsigned int y = static_cast<unsigned int>((ptr->dy - 1)*v);
 
 // index of coordinate
 unsigned int index = y*ptr->pitch + 3*x;
 
 // sample image
 vector4D<float> result;
 result.b = ptr->data[index++]/255.0f;
 result.g = ptr->data[index++]/255.0f;
 result.r = ptr->data[index]/255.0f;
 result.a = 1.0f;
 return result;
*/

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
 vector4D<float> result;

 // B channel
 float p11 = ptr->data[i11++];
 float p12 = ptr->data[i12++];
 float p21 = ptr->data[i21++];
 float p22 = ptr->data[i22++];
 float a = p21 - p11;
 float b = p12 - p11;
 float c = p22 - p12 - a;
 result.b = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // G channel
 p11 = ptr->data[i11++];
 p12 = ptr->data[i12++];
 p21 = ptr->data[i21++];
 p22 = ptr->data[i22++];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.g = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // R channel
 p11 = ptr->data[i11];
 p12 = ptr->data[i12];
 p21 = ptr->data[i21];
 p22 = ptr->data[i22];
 a = p21 - p11;
 b = p12 - p11;
 c = p22 - p12 - a;
 result.r = (static_cast<unsigned char>(a*x + b*y + c*xy + p11))/255.0f;

 // A channel
 result.a = 1.0f;
 return result;
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
    // extract extension
    using namespace std;
    STDSTRINGW extension = std::filesystem::path(filename).extension();

    // DDS
    if(_wcsicmp(extension.c_str(), L".dds") == 0)
      {
       // load DDS file
       DDSDATA ddsd;
       if(!LoadDDS(filename, &ddsd)) return error(__FILE__, __LINE__);

       // determine format
       XNATextureFormat format;
       if(ddsd.bpp == 24) format = XNATextureFormat::BGR;
       else if(ddsd.bpp == 32) {
          if(ddsd.has_alpha) format = XNATextureFormat::BGRA;
          else format = XNATextureFormat::BGRX;
         }
       else return error(__FILE__, __LINE__);

       // assign sampler
       std::function<vector4D<float>(XNATextureData*, float, float)> sampler;
       switch(format) {
         case(XNATextureFormat::BGR)  : sampler = sampler_BGR;  break;
         case(XNATextureFormat::BGRX) : sampler = sampler_BGRX; break;
         case(XNATextureFormat::BGRA) : sampler = sampler_BGRA; break;
         default : return error(__FILE__, __LINE__);
        }

       // create texture reference
       XNATextureRef entry;
       entry.data.dx = ddsd.dx;
       entry.data.dy = ddsd.dy;
       entry.data.pitch = ddsd.pitch_bytes;
       entry.data.type = 0;
       entry.data.mipmap = 0;
       entry.data.format = format;
       entry.data.data = ddsd.data.get();
       entry.data.sampler = sampler;
       entry.refs = 1;

       // insert and assign texture
       auto iter = hashmap.insert(hashmap_type::value_type(filename, std::move(entry)));
       if(iter.second == false) return error(__FILE__, __LINE__);

       // pass ownership of unique_ptr data to texture manager
       ddsd.data.release();
       *out = &iter.first->second.data;
      }
    // PNG
    else if(_wcsicmp(extension.c_str(), L".png") == 0)
      {
       PNGDATA pngdata;
       if(!LoadPNG(filename, &pngdata)) return error(__FILE__, __LINE__);

       // determine format
       XNATextureFormat format;
       if(pngdata.bpp == 24) format = XNATextureFormat::BGR;
       else if(pngdata.bpp == 32) {
          if(pngdata.has_alpha) format = XNATextureFormat::BGRA;
          else format = XNATextureFormat::BGRX;
         }
       else return error(__FILE__, __LINE__);

       // assign sampler
       std::function<vector4D<float>(XNATextureData*, float, float)> sampler;
       switch(format) {
         case(XNATextureFormat::BGR)  : sampler = sampler_BGR;  break;
         case(XNATextureFormat::BGRX) : sampler = sampler_BGRX; break;
         case(XNATextureFormat::BGRA) : sampler = sampler_BGRA; break;
         default : return error(__FILE__, __LINE__);
        }

       // create texture reference
       XNATextureRef entry;
       entry.data.dx = pngdata.dx;
       entry.data.dy = pngdata.dy;
       entry.data.pitch = pngdata.pitch_bytes;
       entry.data.type = 0;
       entry.data.mipmap = 0;
       entry.data.format = format;
       entry.data.data = pngdata.data.get();
       entry.data.sampler = sampler;
       entry.refs = 1;

       // insert and assign texture
       auto iter = hashmap.insert(hashmap_type::value_type(filename, std::move(entry)));
       if(iter.second == false) return error(__FILE__, __LINE__);

       // pass ownership of unique_ptr data to texture manager
       pngdata.data.release();
       *out = &iter.first->second.data;
      }
    // other
    else
       return error(__FILE__, __LINE__);
   }
 // reuse texture
 else {
    if(entry->second.refs == 0) return error(__FILE__, __LINE__);
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
    if(resource.data.data) delete[] resource.data.data;
    resource.data.data = nullptr;
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