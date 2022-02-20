#ifndef __RT_DDS_H
#define __RT_DDS_H

typedef struct tagDDSDATA {
 DWORD dx, dy, bpp;
 DWORD pitch_bytes;
 DWORD total_bytes;
 BOOL has_alpha;
 std::unique_ptr<BYTE[]> data;
} DDSDATA, *LPDDSDATA, *PDDSDATA;

bool LoadDDS(LPCWSTR filename, LPDDSDATA out);
bool ConvertBMPToDDS(const wchar_t* src);

#endif
