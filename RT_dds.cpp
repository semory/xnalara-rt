#include "stdafx.h"
#include "RT_win32.h"
#include "RT_dds.h"

bool ConvertBMPToDDS(const wchar_t* src)
{
 // load BMP file
 using namespace std;
 BITMAPFILEDATA info;
 if(!LoadDibFileW(src, &info))
    return error("LoadDibFileW() failed.", __FILE__, __LINE__);

 // only BI_RGB, BI_BITFIELDS is common with GIMP
 // it is better to use Photoshop since by default it saves to BI_RGB
 if(info.bih.biCompression != BI_RGB)
    return error("Only BI_RGB BMP files are supported.", __FILE__, __LINE__);

 // only 24-bit BGR and 32-bit BGRA
 bool has_alpha = false;
 switch(info.bih.biBitCount) {
   case(24) : break;
   case(32) : has_alpha = true; break;
   default : return error("Only 24-bit and 32-bit BMP files are supported.", __FILE__, __LINE__);
  }

 // must have dimensions
 if(!info.bih.biWidth) return error("Invalid BMP width.", __FILE__, __LINE__);
 if(!info.bih.biHeight) return error("Invalid BMP height.", __FILE__, __LINE__);

 // filename information
 STDSTRINGW pathname = GetPathnameFromFilenameW(src);
 STDSTRINGW shrtname = GetShortFilenameWithoutExtensionW(src);

 // create DDS file
 STDSTRINGSTREAMW ss;
 ss << pathname << shrtname << L".dds";
 ofstream ofile(ss.str().c_str(), ios::binary);
 if(!ofile) return error("Failed to create DDS file.", __FILE__, __LINE__);

 // save DDS header
 DWORD dwarr[32] = {
  0x20534444,                      // [ 0] magic
  124,                             // [ 1] size
  0x100F,                          // [ 2] flags
  info.bih.biWidth,                // [ 3] dx
  info.bih.biHeight,               // [ 4] dy
  GetDibPitchBytes(info.bih),      // [ 5] pitch bytes
  0,                               // [ 6] not a volume texture
  0,                               // [ 7] no mipmaps
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // [ 8] DWORD reserved[11]
  32,                              // [19] sizeof(DDS_PIXELFORMAT)
  (has_alpha ? 0x41 : 0x40),       // [20] DDPF_RGB | DDPF_ALPHAPIXELS (0x41) or DDPF_RGB (0x40)
  0,                               // [21] no FourCC
  info.bih.biBitCount,             // [22] RGBA bitcount
  0x00FF0000,                      // [23] R mask
  0x0000FF00,                      // [24] G mask
  0x000000FF,                      // [25] B mask
  0xFF000000,                      // [26] A mask
  0x1000,                          // [27] capabilities1 = DDSCAPS_TEXTURE
  0,                               // [28] capabilities2 = 0
  0,                               // [29] capabilities3 = unused
  0,                               // [30] capabilities4 = unused
  0                                // [31] reserved
 };
 ofile.write((char*)&dwarr, 32*sizeof(DWORD));
 if(ofile.fail()) return error("Write failure.", __FILE__, __LINE__);

 // bitmap properties
 DWORD pitch_bytes = GetDibPitchBytes(info.bih);
 DWORD total_bytes = GetDibTotalBytes(info.bih);

 // bitmaps are upside-down; flip data for DDS
 BYTE* itop = reinterpret_cast<BYTE*>(&info.data[0]);
 BYTE* ibot = reinterpret_cast<BYTE*>(&info.data[0] + (total_bytes - pitch_bytes));
 DWORD mid = info.bih.biHeight/2;
 for(DWORD r = 0; r < mid; r++) {
     for(DWORD i = 0; i < pitch_bytes; i++) std::swap(itop[i], ibot[i]);
     itop += pitch_bytes;
     ibot -= pitch_bytes;
    }

 // save data
 ofile.write(&info.data[0], total_bytes);
 if(ofile.fail()) return error("Write failure.", __FILE__, __LINE__);

 return true;
}