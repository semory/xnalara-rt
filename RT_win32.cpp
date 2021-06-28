#include "stdafx.h"
#include "RT_win32.h"

//
// Unicode Functions
//

#pragma region Unicode Functions

STDSTRINGA Unicode16ToUnicode08(LPCWSTR str)
{
 // number of characters in string including null
 if(!str) return STDSTRINGA();
 size_t n = wcslen(str) + 1;
 if(n == 1) return STDSTRINGA();

 // fail if invalid input character is encountered
 #if (WINVER >= 0x0600)
 DWORD flags = WC_ERR_INVALID_CHARS;
 #else
 DWORD flags = 0;
 #endif

 // determine size of destination buffer including null
 int size = WideCharToMultiByte(CP_UTF8, flags, str, static_cast<int>(n), NULL, 0, NULL, NULL);
 if(size < 2) return STDSTRINGA();

 // convert and return result
 STDSTRINGA retval;
 retval.resize(size); 
 if(!WideCharToMultiByte(CP_UTF8, flags, str, static_cast<int>(n), &retval[0], size, NULL, NULL)) return STDSTRINGA();
 return retval;
}

STDSTRINGW Unicode08ToUnicode16(LPCSTR str)
{
 // number of characters in string including null
 if(!str) return STDSTRINGW();
 size_t n = strlen(str) + 1;
 if(n == 1) return STDSTRINGW();

 // determine size of destination buffer including null
 int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
 if(size < 2) return STDSTRINGW();

 // allocate string and convert
 STDSTRINGW retval;
 retval.resize(size - 1); 
 if(!MultiByteToWideChar(CP_UTF8, 0, str, -1, &retval[0], size)) return STDSTRINGW();
 return retval;
}

#pragma endregion

//
// Filename Functions
//

#pragma region Filename Functions

STDSTRINGA GetShortFilenameA(LPCSTR filename)
{
 // validate filename
 if(!filename) return STDSTRINGA();
 if(!strlen(filename)) return STDSTRINGA();

 // extract components
 CHAR c_param1[MAX_PATH];
 CHAR c_param2[MAX_PATH];
 CHAR c_param3[MAX_PATH];
 CHAR c_param4[MAX_PATH];
 c_param1[0] = static_cast<CHAR>(0);
 c_param2[0] = static_cast<CHAR>(0);
 c_param3[0] = static_cast<CHAR>(0);
 c_param4[0] = static_cast<CHAR>(0);
 _splitpath_s<MAX_PATH,MAX_PATH,MAX_PATH,MAX_PATH>(filename, c_param1, c_param2, c_param3, c_param4); 

 // return result
 STDSTRINGA retval(c_param3);
 retval += c_param4;
 return retval;
}

STDSTRINGW GetShortFilenameW(LPCWSTR filename)
{
 // validate filename
 if(!filename) return STDSTRINGW();
 if(!wcslen(filename)) return STDSTRINGW();

 // extract components
 WCHAR c_param1[MAX_PATH];
 WCHAR c_param2[MAX_PATH];
 WCHAR c_param3[MAX_PATH];
 WCHAR c_param4[MAX_PATH];
 c_param1[0] = static_cast<WCHAR>(0);
 c_param2[0] = static_cast<WCHAR>(0);
 c_param3[0] = static_cast<WCHAR>(0);
 c_param4[0] = static_cast<WCHAR>(0);
 _wsplitpath_s<MAX_PATH,MAX_PATH,MAX_PATH,MAX_PATH>(filename, c_param1, c_param2, c_param3, c_param4); 

 // return result
 STDSTRINGW retval(c_param3);
 retval += c_param4;
 return retval;
}

STDSTRINGA GetShortFilenameWithoutExtensionA(LPCSTR filename)
{
 // validate filename
 if(!filename) return STDSTRINGA();
 if(!strlen(filename)) return STDSTRINGA();

 // extract components
 CHAR c_param1[MAX_PATH];
 CHAR c_param2[MAX_PATH];
 CHAR c_param3[MAX_PATH];
 CHAR c_param4[MAX_PATH];
 c_param1[0] = static_cast<CHAR>(0);
 c_param2[0] = static_cast<CHAR>(0);
 c_param3[0] = static_cast<CHAR>(0);
 c_param4[0] = static_cast<CHAR>(0);
 _splitpath_s<MAX_PATH,MAX_PATH,MAX_PATH,MAX_PATH>(filename, c_param1, c_param2, c_param3, c_param4); 

 // return result
 return STDSTRINGA(c_param3);
}

STDSTRINGW GetShortFilenameWithoutExtensionW(LPCWSTR filename)
{
 // validate filename
 if(!filename) return STDSTRINGW();
 if(!wcslen(filename)) return STDSTRINGW();

 // extract components
 WCHAR c_param1[MAX_PATH];
 WCHAR c_param2[MAX_PATH];
 WCHAR c_param3[MAX_PATH];
 WCHAR c_param4[MAX_PATH];
 c_param1[0] = static_cast<WCHAR>(0);
 c_param2[0] = static_cast<WCHAR>(0);
 c_param3[0] = static_cast<WCHAR>(0);
 c_param4[0] = static_cast<WCHAR>(0);
 _wsplitpath_s<MAX_PATH,MAX_PATH,MAX_PATH,MAX_PATH>(filename, c_param1, c_param2, c_param3, c_param4); 

 // return result
 return STDSTRINGW(c_param3);
}

STDSTRINGA GetPathnameFromFilenameA(LPCSTR filename)
{
 // validate filename
 if(!filename) return STDSTRINGA();
 if(!strlen(filename)) return STDSTRINGA();

 // extract components
 CHAR c_param1[MAX_PATH];
 CHAR c_param2[MAX_PATH];
 CHAR c_param3[MAX_PATH];
 CHAR c_param4[MAX_PATH];

 // extract components
 c_param1[0] = static_cast<CHAR>(0);
 c_param2[0] = static_cast<CHAR>(0);
 c_param3[0] = static_cast<CHAR>(0);
 c_param4[0] = static_cast<CHAR>(0);
 _splitpath_s<MAX_PATH,MAX_PATH,MAX_PATH,MAX_PATH>(filename, c_param1, c_param2, c_param3, c_param4); 

 // append components
 STDSTRINGA retval = c_param1;
 retval += c_param2;
 return retval;
}

STDSTRINGW GetPathnameFromFilenameW(LPCWSTR filename)
{
 // validate filename
 if(!filename) return STDSTRINGW();
 if(!wcslen(filename)) return STDSTRINGW();

 // extract components
 WCHAR c_param1[MAX_PATH];
 WCHAR c_param2[MAX_PATH];
 WCHAR c_param3[MAX_PATH];
 WCHAR c_param4[MAX_PATH];
 c_param1[0] = static_cast<WCHAR>(0);
 c_param2[0] = static_cast<WCHAR>(0);
 c_param3[0] = static_cast<WCHAR>(0);
 c_param4[0] = static_cast<WCHAR>(0);
 _wsplitpath_s<MAX_PATH,MAX_PATH,MAX_PATH,MAX_PATH>(filename, c_param1, c_param2, c_param3, c_param4); 

 // append components
 STDSTRINGW retval = c_param1;
 retval += c_param2;
 return retval;
}

#pragma endregion

//
// Bitmap Functions
//

#pragma region Bitmap Functions

BOOL LoadDibFileW(const wchar_t* filename, BITMAPFILEDATA* bfd)
{
 // open file
 std::ifstream ifile(filename, std::ios::binary);
 if(!ifile) return FALSE;

 // get filesize
 ifile.seekg(0, std::ios::end);
 DWORD filesize = (DWORD)ifile.tellg();
 ifile.seekg(0, std::ios::beg);
 if(ifile.fail()) return FALSE;

 // read BITMAPFILEHEADER
 BITMAPFILEHEADER bfh;
 ifile.read((char*)&bfh.bfType, sizeof(bfh.bfType));
 ifile.read((char*)&bfh.bfSize, sizeof(bfh.bfSize));
 ifile.read((char*)&bfh.bfReserved1, sizeof(bfh.bfReserved1));
 ifile.read((char*)&bfh.bfReserved2, sizeof(bfh.bfReserved2));
 ifile.read((char*)&bfh.bfOffBits, sizeof(bfh.bfOffBits));
 if(ifile.fail()) return FALSE;

 // validate BITMAPFILEHEADER
 if(bfh.bfType != 0x4D42) return FALSE;

 // read BITMAPINFOHEADER
 BITMAPINFOHEADER bih;
 ifile.read((char*)&bih.biSize, sizeof(bih.biSize));
 ifile.read((char*)&bih.biWidth, sizeof(bih.biWidth));
 ifile.read((char*)&bih.biHeight, sizeof(bih.biHeight));
 ifile.read((char*)&bih.biPlanes, sizeof(bih.biPlanes));
 ifile.read((char*)&bih.biBitCount, sizeof(bih.biBitCount));
 ifile.read((char*)&bih.biCompression, sizeof(bih.biCompression));
 ifile.read((char*)&bih.biSizeImage, sizeof(bih.biSizeImage));
 ifile.read((char*)&bih.biXPelsPerMeter, sizeof(bih.biXPelsPerMeter));
 ifile.read((char*)&bih.biYPelsPerMeter, sizeof(bih.biYPelsPerMeter));
 ifile.read((char*)&bih.biClrUsed, sizeof(bih.biClrUsed));
 ifile.read((char*)&bih.biClrImportant, sizeof(bih.biClrImportant));
 if(ifile.fail()) return false;

 // validate BITMAPINFOHEADER
 // size for BI_RGB must be 40, BI_BITFIELDS must be 56
 if(!(bih.biSize == 40 || bih.biSize == 56)) return FALSE;

 // validate BITMAPINFOHEADER
 if(bih.biWidth < 1 || bih.biHeight < 1)  {
    error("Invalid bitmap dimensions.", __FILE__, __LINE__);
    return FALSE;
   }
 if(bih.biPlanes != 1)  {
    error("Invalid number of planes.", __FILE__, __LINE__);
    return FALSE;
   }

 // JPEG and PNG are not supported
 if(bih.biCompression == BI_JPEG) return FALSE;
 if(bih.biCompression == BI_PNG) return FALSE;

 // palette information
 DWORD masks[3] = { 0, 0, 0 };
 DWORD palette_offset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 std::vector<RGBQUAD> biPalette;

 // depending on the bit count
 switch(bih.biBitCount) {
    // not supported
    case(0) : return FALSE;
    // 1-bit per pixel
    // requires RGBQUAD[2]
    case(1) : {
         biPalette.resize(2);
         ifile.read((char*)&biPalette[0], sizeof(RGBQUAD)*2);
         if(ifile.fail()) return FALSE;
         break;
        }
    // 4-bit per pixel
    // requires RGBQUAD[1] to RGBQUAD[16]
    case(4) : {
         DWORD n = (bih.biClrUsed ? std::min(bih.biClrUsed, static_cast<DWORD>(16)) : 16);
         biPalette.resize(n);
         ifile.read((char*)&biPalette[0], sizeof(RGBQUAD)*n);
         if(ifile.fail()) return FALSE;
         break;
        }
    // 8-bit per pixel
    // requires RGBQUAD[1] to RGBQUAD[256]
    case(8) : {
         DWORD n = (bih.biClrUsed ? std::min(bih.biClrUsed, static_cast<DWORD>(16)) : 16);
         biPalette.resize(n);
         ifile.read((char*)&biPalette[0], sizeof(RGBQUAD)*n);
         if(ifile.fail()) return FALSE;
         break;
        }
    // 16-bit per pixel
    // optional masking data + optional palette data
    case(16) : {
         // optional masking data
         if(bih.biCompression == BI_BITFIELDS) {
            ifile.read((char*)&masks[0], sizeof(DWORD)*3);
            if(ifile.fail()) return FALSE;
           }
         // optional palette data
         if(bih.biClrUsed) {
            DWORD n = bih.biClrUsed;
            biPalette.resize(n);
            ifile.read((char*)&biPalette[0], sizeof(RGBQUAD)*n);
            if(ifile.fail()) return FALSE;
           }
         break;
        }
    // 24-bit per pixel
    // optional palette data
    case(24) : {
         if(bih.biClrUsed) {
            DWORD n = bih.biClrUsed;
            biPalette.resize(n);
            ifile.read((char*)&biPalette[0], sizeof(RGBQUAD)*n);
            if(ifile.fail()) return FALSE;
           }
         break;
        }
    // 32-bit per pixel
    // optional masking data + optional palette data
    case(32) : {
         // optional masking data
         if(bih.biCompression == BI_BITFIELDS) {
            if(bfh.bfOffBits < palette_offset) return FALSE;
            ifile.read((char*)&masks[0], sizeof(DWORD)*3);
            if(ifile.fail()) return FALSE;
           }
         // optional palette data
         if(bih.biClrUsed) {
            DWORD n = bih.biClrUsed;
            biPalette.resize(n);
            ifile.read((char*)&biPalette[0], sizeof(RGBQUAD)*n);
            if(ifile.fail()) return FALSE;
           }
         break;
        }
    default : return FALSE; // not supported
   }

 // (re)-compute data size
 DWORD biDataSizeActual = filesize - (DWORD)ifile.tellg();
 DWORD biDataSizeNeeded = GetDibTotalBytes(bih);
 if(biDataSizeActual < biDataSizeNeeded) return FALSE;
 bih.biSizeImage = biDataSizeNeeded;

 // read data
 std::vector<char> biData(biDataSizeNeeded);
 ifile.read(&biData[0], biDataSizeNeeded);
 if(ifile.fail()) return FALSE;

 // set data
 bfd->bfh = bfh;
 bfd->bih = bih;
 bfd->masks[0] = masks[0];
 bfd->masks[1] = masks[1];
 bfd->masks[2] = masks[2];
 bfd->palette = biPalette;
 bfd->data = std::move(biData);

 return TRUE;
}

BOOL SaveDibFile32(const wchar_t* filename, LONG dx, LONG dy, const unsigned char* data)
{
 // BITMAPFILEHEADER
 BITMAPFILEHEADER bfh;
 bfh.bfType = 0x4D42;
 bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 4*dx*dy;
 bfh.bfReserved1 = 0;
 bfh.bfReserved2 = 0;
 bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

 // BITMAPINFOHEADER
 BITMAPINFOHEADER bih;
 bih.biSize = sizeof(BITMAPINFOHEADER);
 bih.biWidth = dx;
 bih.biHeight = dy;
 bih.biPlanes = 1;
 bih.biBitCount = 32;
 bih.biCompression = BI_RGB;
 bih.biSizeImage = 4*dx*dy;
 bih.biXPelsPerMeter = 0;
 bih.biYPelsPerMeter = 0;
 bih.biClrUsed = 0;
 bih.biClrImportant = 0;

 // create image file
 std::ofstream ofile(filename, std::ios::binary);
 if(!ofile) return FALSE;

 // save BITMAPFILEHEADER
 ofile.write((char*)&bfh.bfType, sizeof(bfh.bfType));
 ofile.write((char*)&bfh.bfSize, sizeof(bfh.bfSize));
 ofile.write((char*)&bfh.bfReserved1, sizeof(bfh.bfReserved1));
 ofile.write((char*)&bfh.bfReserved2, sizeof(bfh.bfReserved2));
 ofile.write((char*)&bfh.bfOffBits, sizeof(bfh.bfOffBits));

 // save BITMAPINFOHEADER
 ofile.write((char*)&bih.biSize, sizeof(bih.biSize));
 ofile.write((char*)&bih.biWidth, sizeof(bih.biWidth));
 ofile.write((char*)&bih.biHeight, sizeof(bih.biHeight));
 ofile.write((char*)&bih.biPlanes, sizeof(bih.biPlanes));
 ofile.write((char*)&bih.biBitCount, sizeof(bih.biBitCount));
 ofile.write((char*)&bih.biCompression, sizeof(bih.biCompression));
 ofile.write((char*)&bih.biSizeImage, sizeof(bih.biSizeImage));
 ofile.write((char*)&bih.biXPelsPerMeter, sizeof(bih.biXPelsPerMeter));
 ofile.write((char*)&bih.biYPelsPerMeter, sizeof(bih.biYPelsPerMeter));
 ofile.write((char*)&bih.biClrUsed, sizeof(bih.biClrUsed));
 ofile.write((char*)&bih.biClrImportant, sizeof(bih.biClrImportant));

 // save image data
 ofile.write((const char*)data, 4*dx*dy);
 if(ofile.fail()) return FALSE;

 return TRUE;
}

DWORD GetDibMaskBytes(const BITMAPINFOHEADER& bih)
{
 switch(bih.biBitCount) {
    case(0x10) : return (bih.biCompression == BI_BITFIELDS ? 24 : 0);
    case(0x20) : return (bih.biCompression == BI_BITFIELDS ? 24 : 0);
    default : return 0;
   }
 return 0;
}

DWORD GetDibPaletteBytes(const BITMAPINFOHEADER& bih)
{
 switch(bih.biBitCount) {
    case(0x01) : return sizeof(RGBQUAD)*(2);
    case(0x04) : return sizeof(RGBQUAD)*(bih.biClrUsed ? std::min(bih.biClrUsed, static_cast<DWORD>(16)) : 16);
    case(0x08) : return sizeof(RGBQUAD)*(bih.biClrUsed ? std::min(bih.biClrUsed, static_cast<DWORD>(256)) : 256);
    case(0x10) : return sizeof(RGBQUAD)*(bih.biClrUsed);
    case(0x18) : return sizeof(RGBQUAD)*(bih.biClrUsed);
    case(0x20) : return sizeof(RGBQUAD)*(bih.biClrUsed);
    default : return 0;
   }
 return 0;
}

DWORD GetDibPitchBytes(const BITMAPINFOHEADER& bih)
{
 return ((bih.biBitCount*bih.biWidth + 0x1F) & (~0x1F))/8;
}

DWORD GetDibTotalBytes(const BITMAPINFOHEADER& bih)
{
 return GetDibPitchBytes(bih)*std::abs(bih.biHeight);
}

#pragma endregion