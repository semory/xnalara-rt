#ifndef __RT_WIN32_H
#define __RT_WIN32_H

typedef std::basic_string<CHAR> STDSTRINGA;
typedef std::basic_string<WCHAR> STDSTRINGW;
typedef std::basic_stringstream<CHAR> STDSTRINGSTREAMA;
typedef std::basic_stringstream<WCHAR> STDSTRINGSTREAMW;

//
// Unicode Functions
//

STDSTRINGA Unicode16ToUnicode08(LPCWSTR str);
STDSTRINGW Unicode08ToUnicode16(LPCSTR str);

//
// Filename Functions
//

#ifdef UNICODE
#define GetShortFilename GetShortFilenameW
#else
#define GetShortFilename GetShortFilenameA
#endif
STDSTRINGA GetShortFilenameA(LPCSTR filename);
STDSTRINGW GetShortFilenameW(LPCWSTR filename);

#ifdef UNICODE
#define GetShortFilenameWithoutExtension GetShortFilenameWithoutExtensionW
#else
#define GetShortFilenameWithoutExtension GetShortFilenameWithoutExtensionA
#endif
STDSTRINGA GetShortFilenameWithoutExtensionA(LPCSTR filename);
STDSTRINGW GetShortFilenameWithoutExtensionW(LPCWSTR filename);

#ifdef UNICODE
#define GetPathnameFromFilename GetPathnameFromFilenameW
#else
#define GetPathnameFromFilename GetPathnameFromFilenameA
#endif
STDSTRINGA GetPathnameFromFilenameA(LPCSTR filename);
STDSTRINGW GetPathnameFromFilenameW(LPCWSTR filename);

//
// Bitmap Functions
//

struct BITMAPFILEDATA {
 BITMAPFILEHEADER bfh;
 BITMAPINFOHEADER bih;
 DWORD masks[3];
 std::vector<RGBQUAD> palette;
 std::vector<char> data;
};

BOOL LoadDibFileW(const wchar_t* filename, BITMAPFILEDATA* bfd);
BOOL SaveDibFile32(const wchar_t* filename, LONG dx, LONG dy, const unsigned char* data);

DWORD GetDibMaskBytes(const BITMAPINFOHEADER& bih);
DWORD GetDibPaletteBytes(const BITMAPINFOHEADER& bih);
DWORD GetDibPitchBytes(const BITMAPINFOHEADER& bih);
DWORD GetDibTotalBytes(const BITMAPINFOHEADER& bih);

#endif