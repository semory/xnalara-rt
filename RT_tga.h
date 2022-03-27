#ifndef __TGA_H
#define __TGA_H

bool LoadTGA(LPCWSTR filename, LPIMGDATA out);

/*

// Color Mapping Function Objects
// typedef void (*ColorMapConversionOp)(const BYTE* colormap, size_t index, BYTE* dst);
// void TGA15_to_D3D32(const BYTE* colormap, size_t index, BYTE* dst);
// void TGA16_to_D3D32(const BYTE* colormap, size_t index, BYTE* dst);
// void TGA24_to_D3D32(const BYTE* colormap, size_t index, BYTE* dst);
// void TGA32_to_D3D32(const BYTE* colormap, size_t index, BYTE* dst);
// BOOL TGACopyColorMappedImage(DWORD dx, DWORD dy, const BYTE* colormap, const BYTE* src, UINT bpp, BYTE* dst, ColorMapConversionOp cop);

// Color Function Objects
struct TGAMono08 {
 static const DWORD src_bpp = 1;
 static const DWORD dst_bpp = 4;
 BOOL operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst);
};
struct TGAMono16 {
 static const DWORD src_bpp = 1;
 static const DWORD dst_bpp = 4;
 BOOL operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst);
};
struct TGAColor15 {
 static const DWORD src_bpp = 2;
 static const DWORD dst_bpp = 4;
 BOOL operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst);
};
struct TGAColor16 {
 static const DWORD src_bpp = 2;
 static const DWORD dst_bpp = 4;
 BOOL operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst);
};
struct TGAColor24 {
 static const DWORD src_bpp = 3;
 static const DWORD dst_bpp = 4;
 BOOL operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst);
};
struct TGAColor32 {
 static const DWORD src_bpp = 4;
 static const DWORD dst_bpp = 4;
 BOOL operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst);
};

// RLE Function Objects
struct TGABaseRLE {
 DWORD offset;
 TGABaseRLE() : offset(0) {}
};
struct TGAMonoRLE08 : TGABaseRLE {
 BOOL operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count);
};
struct TGAMonoRLE16 : TGABaseRLE {
 BOOL operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count);
};
struct TGAColorRLE15 : TGABaseRLE {
 BOOL operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count);
};
struct TGAColorRLE16 : TGABaseRLE {
 BOOL operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count);
};
struct TGAColorRLE24 : TGABaseRLE {
 BOOL operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count);
};
struct TGAColorRLE32 : TGABaseRLE {
 BOOL operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count);
};
*/

bool LoadTGA(const LPCWSTR filename, LPIMGDATA data);

#endif
