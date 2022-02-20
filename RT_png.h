#ifndef __RT_PNG_H
#define __RT_PNG_H

typedef struct tagPNGDATA {
 DWORD dx, dy, bpp;
 DWORD pitch_bytes;
 DWORD total_bytes;
 BOOL has_alpha;
 std::unique_ptr<BYTE[]> data;
} PNGDATA, *LPPNGDATA, *PPNGDATA;

bool LoadPNG(LPCWSTR filename, LPPNGDATA out);

#endif
