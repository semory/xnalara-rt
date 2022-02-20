#include "stdafx.h"
#include "RT_win32.h"
#include "RT_dds.h"

// DWORD[ 0] = 0x20534444 (DDS);
// DWORD[ 1] = dwSize;
// DWORD[ 2] = dwFlags;
// DWORD[ 3] = dwHeight;
// DWORD[ 4] = dwWidth;
// DWORD[ 5] = dwPitchOrLinearSize;
// DWORD[ 6] = dwDepth;
// DWORD[ 7] = dwMipMapCount;
// DWORD[ 8] = dwReserved1[0];
// DWORD[ 9] = dwReserved1[1];
// DWORD[10] = dwReserved1[2];
// DWORD[11] = dwReserved1[3];
// DWORD[12] = dwReserved1[4];
// DWORD[13] = dwReserved1[5];
// DWORD[14] = dwReserved1[6];
// DWORD[15] = dwReserved1[7];
// DWORD[16] = dwReserved1[8];
// DWORD[17] = dwReserved1[9];
// DWORD[18] = dwReserved1[10];
// DWORD[19] = DDS_PIXELFORMAT::dwSize;
// DWORD[20] = DDS_PIXELFORMAT::dwFlags;
// DWORD[21] = DDS_PIXELFORMAT::dwFourCC;
// DWORD[22] = DDS_PIXELFORMAT::dwRGBBitCount;
// DWORD[23] = DDS_PIXELFORMAT::dwRBitMask;
// DWORD[24] = DDS_PIXELFORMAT::dwGBitMask;
// DWORD[25] = DDS_PIXELFORMAT::dwBBitMask;
// DWORD[26] = DDS_PIXELFORMAT::dwABitMask;
// DWORD[27] = dwCaps;
// DWORD[28] = dwCaps2;
// DWORD[29] = dwCaps3;
// DWORD[30] = dwCaps4;
// DWORD[31] = dwReserved2;

bool LoadDDS(LPCWSTR filename, LPDDSDATA out)
{
 // open DDS file
 using namespace std;
 ifstream ifile(filename, ios::binary);
 if(!ifile) {
   std::wcout << filename << std::endl;
    return error(__FILE__, __LINE__);
   }

 // compute filesize
 ifile.seekg(0, ios::end);
 size_t filesize = (size_t)ifile.tellg();
 ifile.seekg(0, ios::beg);
 if(ifile.fail()) return error(__FILE__, __LINE__);

 // must have at least a header
 if(filesize < 0x20) return error(__FILE__, __LINE__);

 // read DDS signature + header
 DWORD ddsh[32];
 ifile.read((char*)&ddsh[0], 32*sizeof(DWORD));
 if(ifile.fail()) return error(__FILE__, __LINE__);

 // validate header
 if(ddsh[0] != 0x20534444ul) return error(__FILE__, __LINE__);
 if(!ddsh[3] || !ddsh[4]) return error(__FILE__, __LINE__); // allow this?
 if(!ddsh[5]) return error(__FILE__, __LINE__);

 // read data
 size_t datasize = filesize - 128ul; // filesize - DDS_header_size
 unique_ptr<BYTE[]> data(new BYTE[datasize]);
 ifile.read((char*)data.get(), datasize);
 if(ifile.fail()) return error(__FILE__, __LINE__);

 // determine texture format
 // 0x40 = DDPF_RGB
 // 0x41 = DDPF_RGB | DDPF_ALPHAPIXELS
 DWORD bitcount = ddsh[22]; // [22] = DDS_PIXELFORMAT::dwRGBBitCount
 DWORD flags = ddsh[20];    // [20] = DDS_PIXELFORMAT::dwFlags
 DWORD bpp = 0;
 BOOL alpha = FALSE;
 if(bitcount == 24) {
    if(flags == 0x40) bpp = 24;
    else return error("", __FILE__, __LINE__);
   }
 else if(bitcount == 32) {
    bpp = 32;
    if(flags == 0x40) alpha = FALSE;
    else if(flags == 0x41) alpha = TRUE;
    else return error("", __FILE__, __LINE__);
   }
 else return error("", __FILE__, __LINE__);

 // set data
 out->dx = ddsh[4];
 out->dy = ddsh[3];
 out->bpp = bpp; // 24 or 32 for now
 out->pitch_bytes = ddsh[5];
 out->total_bytes = (DWORD)datasize;
 out->data = std::move(data);
 out->has_alpha = alpha;

 return true;
}

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
  0x20534444,                       // [ 0] magic
  124,                              // [ 1] size
  0x100F,                           // [ 2] flags
  (DWORD)info.bih.biWidth,          // [ 3] dx
  (DWORD)info.bih.biHeight,         // [ 4] dy
  GetDibPitchBytes(info.bih),       // [ 5] pitch bytes
  0,                                // [ 6] not a volume texture
  0,                                // [ 7] no mipmaps
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // [ 8] DWORD reserved[11]
  32,                               // [19] sizeof(DDS_PIXELFORMAT)
  (DWORD)(has_alpha ? 0x41 : 0x40), // [20] DDPF_RGB | DDPF_ALPHAPIXELS (0x41) or DDPF_RGB (0x40)
  0,                                // [21] no FourCC
  info.bih.biBitCount,              // [22] RGBA bitcount
  0x00FF0000,                       // [23] R mask
  0x0000FF00,                       // [24] G mask
  0x000000FF,                       // [25] B mask
  0xFF000000,                       // [26] A mask
  0x1000,                           // [27] capabilities1 = DDSCAPS_TEXTURE
  0,                                // [28] capabilities2 = 0
  0,                                // [29] capabilities3 = unused
  0,                                // [30] capabilities4 = unused
  0                                 // [31] reserved
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