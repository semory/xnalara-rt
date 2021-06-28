#ifndef __STDAFX_H
#define __STDAFX_H

#define NOMINMAX
#include<windows.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<deque>
#include<stack>
#include<vector>
#include<memory>
#include<random>
#include<climits>
#include<functional>
#include<algorithm>

#ifndef RC_INVOKED

inline bool error(const char* message)
{
 std::cout << message << std::endl;
 return false;
}

inline bool error(const char* message, const char* file, int line)
{
 std::cout << "Error: " << message << std::endl;
 std::cout << "  File: " << file << std::endl;
 std::cout << "  Line: " << line << std::endl;
 return false;
}

#endif

#ifndef RC_INVOKED

inline uint16_t StringToUint16(const char* str) { return static_cast<uint16_t>(atoi(str)); }
inline uint16_t StringToUint16(const wchar_t* str) { return static_cast<uint16_t>(_wtoi(str)); }
inline uint32_t StringToUint32(const char* str) { return static_cast<uint32_t>(strtoul(str, nullptr, 10)); }
inline uint32_t StringToUint32(const wchar_t* str) { return static_cast<uint32_t>(wcstoul(str, nullptr, 10)); }
inline float StringToReal32(const char* str) {  return strtof(str, nullptr); }
inline float StringToReal32(const wchar_t* str) { return wcstof(str, nullptr); }

#endif

#endif
