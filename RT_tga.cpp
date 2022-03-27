#include "stdafx.h"
#include "RT_win32.h"
#include "RT_stream.h"
#include "RT_imgproc.h"
#include "RT_tga.h"

#pragma region FORWARD_DECLARATIONS

struct TGAHEADER {
 BYTE id_length;        // 0
 BYTE color_map_type;   // 1
 BYTE image_type;       // 2
 WORD color_map_origin; // 3
 WORD color_map_length; // 5
 BYTE color_map_bits;   // 7
 WORD xorigin;          // 8
 WORD yorigin;          // 10
 WORD dx;               // 12
 WORD dy;               // 14
 BYTE pixel_depth;      // 16
 BYTE image_descriptor; // 17
};

struct TGAFOOTER {
 DWORD eao;          // 00-03: The Extension Area Offset
 DWORD ddo;          // 04-07: The Developer Directory Offset
 char signature[18]; // 08-25: "TRUEVISION-XFILE.\0"
};

// Explict Handling
BOOL TGAImageType01(std::stringstream& ss, const TGAHEADER& header, LPIMGDATA out);
BOOL TGAImageType02(std::stringstream& ss, const TGAHEADER& header, LPIMGDATA out);
BOOL TGAImageType03(std::stringstream& ss, const TGAHEADER& header, LPIMGDATA out);
BOOL TGAImageType09(std::stringstream& ss, const TGAHEADER& header, LPIMGDATA out);
BOOL TGAImageType10(std::stringstream& ss, const TGAHEADER& header, LPIMGDATA out);
BOOL TGAImageType11(std::stringstream& ss, const TGAHEADER& header, LPIMGDATA out);

#pragma endregion FORWARD_DECLARATIONS

bool LoadTGA(LPCWSTR filename, LPIMGDATA out)
{
 //
 // DATA
 //

 TGAHEADER header;
 TGAFOOTER footer;
 std::unique_ptr<BYTE[]> filedata;

 // constexpr
 constexpr DWORD header_size = 18;
 constexpr DWORD footer_size = 26;

 // open file for reading
 if(!filename) return error(__FILE__, __LINE__);
 HANDLE handle = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(!handle) return error(__FILE__, __LINE__);

 // get filesize
 DWORD filesize = 0;
 if(!GetFileSize(handle, &filesize)) return error(__FILE__, __LINE__);
 if(filesize < 18) return error(__FILE__, __LINE__); // size of TGA header

 // read file data
 filedata.reset(new BYTE[filesize]);
 DWORD bytes_read = 0;
 if(!ReadFile(handle, (LPVOID)filedata.get(), filesize, &bytes_read, NULL)) return error(__FILE__, __LINE__);

 //
 // READ HEADER
 //

 // read file header
 BYTE* ptr = filedata.get();
 header.id_length = *reinterpret_cast<BYTE*>(ptr); ptr++;
 header.color_map_type = *reinterpret_cast<BYTE*>(ptr); ptr++;
 header.image_type = *reinterpret_cast<BYTE*>(ptr); ptr++;
 header.color_map_origin = *reinterpret_cast<WORD*>(ptr); ptr += 2;
 header.color_map_length = *reinterpret_cast<WORD*>(ptr); ptr += 2;
 header.color_map_bits = *reinterpret_cast<BYTE*>(ptr); ptr++;
 header.xorigin = *reinterpret_cast<WORD*>(ptr); ptr += 2;
 header.yorigin = *reinterpret_cast<WORD*>(ptr); ptr += 2;
 header.dx = *reinterpret_cast<WORD*>(ptr); ptr += 2;
 header.dy = *reinterpret_cast<WORD*>(ptr); ptr += 2;
 header.pixel_depth = *reinterpret_cast<BYTE*>(ptr); ptr++;
 header.image_descriptor = *reinterpret_cast<BYTE*>(ptr); ptr++;

 // validate header
 if(header.color_map_type > 1) return error(__FILE__, __LINE__);
 switch(header.image_type) {
    case(0x0): case(0x1): case(0x2): case(0x3): break;
    case(0x9): case(0xA): case(0xB) : break;
    default: return error(__FILE__, __LINE__);
   }
 if(header.color_map_type == 1 && header.color_map_length) {
    switch(header.color_map_bits) {
      case(0x08): case(0x0F): case(0x10): case(0x18): case(0x20): break;
      default: return error(__FILE__, __LINE__);
     }
   }

 //
 // READ FOOTER
 //

 // determine presence of footer
 DWORD min_filesize_with_footer = header_size + footer_size + static_cast<DWORD>(header.id_length);
 bool has_footer = false;
 if(!(filesize < min_filesize_with_footer)) {
    BYTE* ptr = filedata.get() + (filesize - footer_size + 8);
    if(strcmp(reinterpret_cast<const char*>(ptr), "TRUEVISION-XFILE.") == 0) has_footer = true;
   }

 // read footer
 DWORD eao = 0;
 DWORD ddo = 0;
 if(has_footer) {
    // read footer
    BYTE* ptr = filedata.get() + (filesize - footer_size);
    eao = *reinterpret_cast<DWORD*>(ptr); ptr += 4;
    ddo = *reinterpret_cast<DWORD*>(ptr); ptr += 4;
    // validate footer
   }

 //
 // VALIDATION?
 //

 //
 // LAMBDAS
 //

 auto GetAlphaBits = [&](void) { return (header.image_descriptor & 0xF); };
 auto GetHorzTransferOrder = [&](void) { return ((header.image_descriptor & 0x10) >> 4); };
 auto GetVertTransferOrder = [&](void) { return ((header.image_descriptor & 0x20) >> 5); };
 auto IsCompressed = [&](void) { return ((header.image_type == 9) || (header.image_type == 10) || (header.image_type == 11)); };

 auto TGARLE32 = [&](const BYTE* src, BYTE* dst, BYTE header, UINT count) {
  // compressed, read item and repeat 'count' times
  if(header & 0b10000000) {
     DWORD offset = 0;
     for(UINT i = 0; i < count; i++) {
         dst[offset++] = src[0]; // B
         dst[offset++] = src[1]; // G
         dst[offset++] = src[2]; // R
         dst[offset++] = src[3]; // A
        }
    }
  // uncompressed, read 'count' items
  else {
     DWORD offset = 0;
     for(UINT i = 0; i < count; i++) {
         dst[offset] = src[offset]; offset++; // B
         dst[offset] = src[offset]; offset++; // G
         dst[offset] = src[offset]; offset++; // R
         dst[offset] = src[offset]; offset++; // A
        }
    }
 };

 //
 // DECOMPRESSION
 //

 if(IsCompressed())
   {
    // decompress data
    DWORD n_pixels = ((DWORD)header.dx)*((DWORD)header.dy);
    DWORD curr = 0;
    BYTE* data = filedata.get() + header_size + static_cast<DWORD>(header.id_length);
    while(curr < n_pixels) {
          // read packet header
          BYTE ph = *reinterpret_cast<BYTE*>(data);
          data++;
          // 'count' colors can be added (clamp 'count' to prevent crashing)
          UINT count = (ph & 0x7F) + 1;
          if(!(curr + count < n_pixels)) count = n_pixels - curr;
          // RLE packet processing
          //if(!op(ss, dst, ph, count)) return FALSE;
          curr += count;

          // // RLE packet (read a single index and reuse it 'count' times)
          // if(ph & 0x80) {
          //    T value;
          //    ss.read((char*)&value, sizeof(value));
          //    if(ss.fail()) return FALSE;
          //    for(UINT i = 0; i < count; i++) dst[curr + i] = value;
          //   }
          // // RAW packet (read 'count' indices)
          // else {
          //    ss.read((char*)&dst[curr], count*sizeof(T)); 
          //    if(ss.fail()) return FALSE;
          //   }
         }
   }
 else
   {
   }

 //
 //
 //
 if(header.image_type == 0x0)
    
 if(header.image_type == 0x1 || header.image_type == 0x9) // COLOR-MAPPED
   {
    return true;
   }
 if(header.image_type == 0x2 || header.image_type == 0xA) // RGB
   {
    return true;
   }
 if(header.image_type == 0x3 || header.image_type == 0xB) // GRAYSCALE
   {
    return true;
   }

 return false;
}

/*
#pragma region FORWARD_DECLARATIONS

// Explict Handling
BOOL TGAImageType01(std::stringstream& ss, const TGAHEADER& header, LPTGADATA out); // BELIEVE TO BE DONE
BOOL TGAImageType02(std::stringstream& ss, const TGAHEADER& header, LPTGADATA out); // BELIEVE TO BE DONE
BOOL TGAImageType03(std::stringstream& ss, const TGAHEADER& header, LPTGADATA out); // BELIEVE TO BE DONE
BOOL TGAImageType09(std::stringstream& ss, const TGAHEADER& header, LPTGADATA out); // BELIEVE TO BE DONE
BOOL TGAImageType10(std::stringstream& ss, const TGAHEADER& header, LPTGADATA out); // BELIEVE TO BE DONE
BOOL TGAImageType11(std::stringstream& ss, const TGAHEADER& header, LPTGADATA out); // BELIEVE TO BE DONE

#pragma endregion FORWARD_DECLARATIONS

#pragma region TGA_COLOR_MAPPING_FUNCTION_OBJECTS

static int C5_map[32] = {
 0x00, 0x08, 0x10, 0x19, 0x21, 0x29, 0x31, 0x3a,
 0x42, 0x4a, 0x52, 0x5a, 0x63, 0x6b, 0x73, 0x7b,
 0x84, 0x8c, 0x94, 0x9c, 0xa5, 0xad, 0xb5, 0xbd,
 0xc5, 0xce, 0xd6, 0xde, 0xe6, 0xef, 0xf7, 0xff,
};

// 24-bit Color Mapping Functions
struct TGA15_to_D3D24 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
 }
};
struct TGA16_to_D3D24 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
 }
};
struct TGA24_to_D3D24 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
 }
};
struct TGA32_to_D3D24 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
 }
};

// 32-bit Color Mapping Functions
struct TGA15_to_D3D32 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
  const WORD* ptr = reinterpret_cast<const WORD*>(colormap);
  dst[0] = C5_map[(ptr[index] & 0x001F)];
  dst[1] = C5_map[(ptr[index] & 0x03E0) >>  5];
  dst[2] = C5_map[(ptr[index] & 0x7C00) >> 10];
  dst[3] = 255;
 }
};
struct TGA16_to_D3D32 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
  const WORD* ptr = reinterpret_cast<const WORD*>(colormap);
  dst[0] = C5_map[(ptr[index] & 0x001F)];
  dst[1] = C5_map[(ptr[index] & 0x03E0) >>  5];
  dst[2] = C5_map[(ptr[index] & 0x7C00) >> 10];
  dst[3] = ((ptr[index] & 0x8000) ? 255 : 0);
 }
};
struct TGA24_to_D3D32 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
  size_t offset = 3*index;
  dst[0] = colormap[offset++];
  dst[1] = colormap[offset++];
  dst[2] = colormap[offset];
  dst[3] = 255;
 }
};
struct TGA32_to_D3D32 { 
 void operator ()(const BYTE* colormap, size_t index, BYTE* dst)const {
  size_t offset = 4*index;
  dst[0] = colormap[offset + 0];
  dst[1] = colormap[offset + 1];
  dst[2] = colormap[offset + 2];
  dst[3] = colormap[offset + 3];
 }
};

template<class ConversionOp>
BOOL TGACopyColorMappedImage(DWORD dx, DWORD dy, const BYTE* colormap, const BYTE* src, UINT bpp, BYTE* dst, ConversionOp cop)
{
 DWORD pitch_bytes = 4*dx*dy;
 if(bpp == 8) {
    for(size_t r = 0; r < dy; r++) {
        size_t s0 = r*dx;
        size_t d0 = r*pitch_bytes;
        for(size_t c = 0; c < dx; c++) {
            size_t i = src[s0 + c];
            size_t j = d0 + 4*c;
            cop(colormap, i, &dst[j]);
           }
       }
   }
 else if(bpp == 16) {
    const WORD* srcptr = reinterpret_cast<const WORD*>(src);
    for(size_t r = 0; r < dy; r++) {
        size_t s0 = r*dx;
        size_t d0 = r*pitch_bytes;
        for(size_t c = 0; c < dx; c++) {
            size_t i = srcptr[s0 + c];
            size_t j = d0 + 4*c;
            cop(colormap, i, &dst[j]);
           }
       }
   }
 else return FALSE;
 return TRUE;
}

#pragma endregion TGA_COLOR_MAPPING_FUNCTION_OBJECTS

#pragma region TGA_COLOR_FUNCTION_OBJECTS

BOOL TGAMono08::operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst)
{
 // pitch bytes
 DWORD src_pitch = dx*src_bpp;
 DWORD dst_pitch = dx*dst_bpp;
 // copy src to dst buffer
 for(size_t r = 0; r < dy; r++) {
     size_t src_start = r*src_pitch;
     size_t dst_start = r*dst_pitch;
     for(size_t c = 0; c < dx; c++) {
         BYTE color = src[src_start + c*src_bpp];
         size_t j = dst_start + c*dst_bpp;
         dst[j + 0] = color;
         dst[j + 1] = color;
         dst[j + 2] = color;
         dst[j + 3] = 0;
        }
    }
 return TRUE;
}

BOOL TGAMono16::operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst)
{
 // copy src to dst buffer
 DWORD src_pitch = dx*src_bpp;
 DWORD dst_pitch = dx*dst_bpp;
 for(size_t r = 0; r < dy; r++) {
     size_t src_start = r*src_pitch;
     size_t dst_start = r*dst_pitch;
     for(size_t c = 0; c < dx; c++) {
         // src components
         size_t i = src_start + c*src_bpp;
         BYTE color = src[i];
         BYTE alpha = src[i + 1];
         // dst components
         size_t j = dst_start + c*dst_bpp;
         dst[j + 0] = color;
         dst[j + 1] = color;
         dst[j + 2] = color;
         dst[j + 3] = alpha;
        }
    }
 return TRUE;
}

BOOL TGAColor15::operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst)
{
 // pitch bytes
 DWORD src_pitch = dx*src_bpp;
 DWORD dst_pitch = dx*dst_bpp;
 // copy src to dst buffer
 for(size_t r = 0; r < dy; r++) {
     size_t src_start = r*src_pitch;
     size_t dst_start = r*dst_pitch;
     for(size_t c = 0; c < dx; c++) {
         size_t i = src_start + c*src_bpp;
         size_t j = dst_start + c*dst_bpp;
         WORD color = *reinterpret_cast<const WORD*>(&src[i]);
         dst[j + 0] = C5_map[(color & 0x001F)];
         dst[j + 1] = C5_map[(color & 0x03E0) >>  5];
         dst[j + 2] = C5_map[(color & 0x7C00) >> 10];
         dst[j + 3] = 0;
        }
    }
 return TRUE;
}

BOOL TGAColor16::operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst)
{
 // pitch bytes
 DWORD src_pitch = dx*src_bpp;
 DWORD dst_pitch = dx*dst_bpp;
 // copy src to dst buffer
 for(size_t r = 0; r < dy; r++) {
     size_t src_start = r*src_pitch;
     size_t dst_start = r*dst_pitch;
     for(size_t c = 0; c < dx; c++) {
         size_t i = src_start + c*src_bpp;
         size_t j = dst_start + c*dst_bpp;
         WORD color = *reinterpret_cast<const WORD*>(&src[i]);
         dst[j + 0] = C5_map[(color & 0x001F)];
         dst[j + 1] = C5_map[(color & 0x03E0) >>  5];
         dst[j + 2] = C5_map[(color & 0x7C00) >> 10];
         dst[j + 3] = ((color & 0x8000) ? 255 : 0);
        }
    }
 return TRUE;
}

BOOL TGAColor24::operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst)
{
 // pitch bytes
 DWORD src_pitch = dx*src_bpp;
 DWORD dst_pitch = dx*dst_bpp;
 // copy src to dst buffer
 for(size_t r = 0; r < dy; r++) {
     size_t src_start = r*src_pitch;
     size_t dst_start = r*dst_pitch;
     for(size_t c = 0; c < dx; c++) {
         size_t i = src_start + c*src_bpp;
         size_t j = dst_start + c*dst_bpp;
         dst[j + 0] = src[i + 0];
         dst[j + 1] = src[i + 1];
         dst[j + 2] = src[i + 2];
         dst[j + 3] = 0;
        }
    }
 return TRUE;
}

BOOL TGAColor32::operator ()(DWORD dx, DWORD dy, BYTE* src, BYTE* dst)
{
 // pitch bytes
 DWORD src_pitch = dx*src_bpp;
 DWORD dst_pitch = dx*dst_bpp;
 // copy src to dst buffer
 for(size_t r = 0; r < dy; r++) {
     size_t src_start = r*src_pitch;
     size_t dst_start = r*dst_pitch;
     for(size_t c = 0; c < dx; c++) {
         size_t i = src_start + c*src_bpp;
         size_t j = dst_start + c*dst_bpp;
         dst[j + 0] = src[i + 0];
         dst[j + 1] = src[i + 1];
         dst[j + 2] = src[i + 2];
         dst[j + 3] = src[i + 3];
        }
    }
 return TRUE;
}

#pragma endregion TGA_COLOR_FUNCTION_OBJECTS

#pragma region TGA_RLE_FUNCTION_OBJECTS

BOOL TGAMonoRLE08::operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count)
{
 // read single byte and repeat 'count' times
 if(header & 0x80) {
    BYTE value;
    ss.read((char*)&value, sizeof(BYTE));
    if(ss.fail()) return FALSE;
    for(UINT i = 0; i < count; i++) {
        dst[offset++] = value; // B
        dst[offset++] = value; // G
        dst[offset++] = value; // R
        dst[offset++] = 0;     // A
       }
   }
 // read 'count' bytes
 else {
    BYTE value;
    for(UINT i = 0; i < count; i++) {
        ss.read((char*)&value, sizeof(BYTE)); 
        if(ss.fail()) return FALSE;
        dst[offset++] = value; // B
        dst[offset++] = value; // G
        dst[offset++] = value; // R
        dst[offset++] = 0;     // A
       }
   }
 return TRUE;
}

BOOL TGAMonoRLE16::operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count)
{
 // read single byte and repeat 'count' times
 BYTE value[2];
 if(header & 0x80) {
    ss.read((char*)&value[0], sizeof(WORD));
    if(ss.fail()) return FALSE;
    for(UINT i = 0; i < count; i++) {
        dst[offset++] = value[0]; // B
        dst[offset++] = value[0]; // G
        dst[offset++] = value[0]; // R
        dst[offset++] = value[1]; // A
       }
   }
 // read 'count' bytes
 else {
    for(UINT i = 0; i < count; i++) {
        ss.read((char*)&value[0], sizeof(WORD)); 
        if(ss.fail()) return FALSE;
        dst[offset++] = value[0]; // B
        dst[offset++] = value[0]; // G
        dst[offset++] = value[0]; // R
        dst[offset++] = value[1]; // A
       }
   }
 return TRUE;
}

BOOL TGAColorRLE15::operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count)
{
 // read single WORD and repeat 'count' times
 WORD value;
 if(header & 0x80) {
    ss.read((char*)&value, sizeof(WORD));
    if(ss.fail()) return FALSE;
    for(UINT i = 0; i < count; i++) {
        dst[offset++] = C5_map[(value & 0x001F)];
        dst[offset++] = C5_map[(value & 0x03E0) >>  5];
        dst[offset++] = C5_map[(value & 0x7C00) >> 10];
        dst[offset++] = 0;
       }
   }
 // read 'count' WORDs
 else {
    for(UINT i = 0; i < count; i++) {
        ss.read((char*)&value, sizeof(WORD));
        if(ss.fail()) return FALSE;
        dst[offset++] = C5_map[(value & 0x001F)];
        dst[offset++] = C5_map[(value & 0x03E0) >>  5];
        dst[offset++] = C5_map[(value & 0x7C00) >> 10];
        dst[offset++] = 0;
       }
   }
 return TRUE;
}

BOOL TGAColorRLE16::operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count)
{
 // read single WORD and repeat 'count' times
 WORD value;
 if(header & 0x80) {
    ss.read((char*)&value, sizeof(WORD));
    if(ss.fail()) return FALSE;
    for(UINT i = 0; i < count; i++) {
        dst[offset++] = C5_map[(value & 0x001F)];
        dst[offset++] = C5_map[(value & 0x03E0) >>  5];
        dst[offset++] = C5_map[(value & 0x7C00) >> 10];
        dst[offset++] = ((value & 0x8000) ? 255 : 0);
       }
   }
 // read 'count' WORDs
 else {
    for(UINT i = 0; i < count; i++) {
        ss.read((char*)&value, sizeof(WORD));
        if(ss.fail()) return FALSE;
        dst[offset++] = C5_map[(value & 0x001F)];
        dst[offset++] = C5_map[(value & 0x03E0) >>  5];
        dst[offset++] = C5_map[(value & 0x7C00) >> 10];
        dst[offset++] = ((value & 0x8000) ? 255 : 0);
       }
   }
 return TRUE;
}

BOOL TGAColorRLE24::operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count)
{
 // read BYTE[3] and repeat 'count' times
 BYTE cache[3];
 if(header & 0x80) {
    ss.read((char*)&cache[0], 3*sizeof(BYTE));
    if(ss.fail()) return FALSE;
    for(UINT i = 0; i < count; i++) {
        dst[offset++] = cache[0]; // B
        dst[offset++] = cache[1]; // G
        dst[offset++] = cache[2]; // R
        dst[offset++] = 0;        // A
       }
   }
 // read 'count' BYTE[3]s
 else {
    for(UINT i = 0; i < count; i++) {
        ss.read((char*)&cache[0], 3*sizeof(BYTE));
        if(ss.fail()) return FALSE;
        dst[offset++] = cache[0]; // B
        dst[offset++] = cache[1]; // G
        dst[offset++] = cache[2]; // R
        dst[offset++] = 0;        // A
       }
   }
 return TRUE;
}

BOOL TGAColorRLE32::operator ()(std::stringstream& ss, BYTE* dst, BYTE header, UINT count)
{
 // read BYTE[4] and repeat 'count' times
 BYTE cache[4];
 if(header & 0x80) {
    ss.read((char*)&cache[0], 4*sizeof(BYTE));
    if(ss.fail()) return FALSE;
    for(UINT i = 0; i < count; i++) {
        dst[offset++] = cache[0]; // B
        dst[offset++] = cache[1]; // G
        dst[offset++] = cache[2]; // R
        dst[offset++] = cache[3]; // A
       }
   }
 // read 'count' BYTE[4]s
 else {
    for(UINT i = 0; i < count; i++) {
        ss.read((char*)&cache[0], 4*sizeof(BYTE));
        if(ss.fail()) return FALSE;
        dst[offset++] = cache[0]; // B
        dst[offset++] = cache[1]; // G
        dst[offset++] = cache[2]; // R
        dst[offset++] = cache[3]; // A
       }
   }
 return TRUE;
}

#pragma endregion TGA_RLE_FUNCTION_OBJECTS

#pragma region TGA_RLE_FUNCTIONS

template<class T>
BOOL TGADecompressMappedRLE(std::stringstream& ss, T* dst, DWORD n_pixels)
{
 // read and decompress data
 DWORD curr = 0;
 while(curr < n_pixels)
      {
       // read packet header
       BYTE ph;
       ss.read((char*)&ph, 1);
       if(ss.fail()) return FALSE;

       // no matter what, up to 'count' indices will be added
       // clamp 'count' to prevent crashing
       UINT count = (ph & 0x7F) + 1;
       if(!(curr + count < n_pixels)) count = n_pixels - curr;

       // RLE packet (read a single index and reuse it 'count' times)
       if(ph & 0x80) {
          T value;
          ss.read((char*)&value, sizeof(value));
          if(ss.fail()) return FALSE;
          for(UINT i = 0; i < count; i++) dst[curr + i] = value;
         }
       // RAW packet (read 'count' indices)
       else {
          ss.read((char*)&dst[curr], count*sizeof(T)); 
          if(ss.fail()) return FALSE;
         }

       curr += count;
      }

 return TRUE;
}

template<class ReadOp>
BOOL TGADecompressRLE(std::stringstream& ss, const D3DImageInfo& info, BYTE* dst, ReadOp op)
{
 // decompress data
 DWORD n_pixels = info.dx*info.dy;
 DWORD curr = 0;
 while(curr < n_pixels) {
       // read packet header
       BYTE ph;
       ss.read((char*)&ph, 1);
       if(ss.fail()) return FALSE;
       // 'count' colors can be added (clamp to prevent crashing)
       UINT count = (ph & 0x7F) + 1;
       if(!(curr + count < n_pixels)) count = n_pixels - curr;
       // RLE packet processing
       if(!op(ss, dst, ph, count)) return FALSE;
       curr += count;
      }
 return TRUE;
}

#pragma endregion TGA_RLE_FUNCTIONS

#pragma region TGA_EXPLICIT_HANDLING

/// @brief Load uncompressed, color-mapped TGA data.
/// @details Called from LoadTGA to process TGA type-1 data, which represents uncompressed, indexed
///  color data. Index data must be 8-bit or 16-bit indices. Color map data must be 15, 16, 24, or
///  32 bits.
/// @param ss Reference to the std::stringstream containing file data. The current position of the
///  stream must point to the beginning of the TGA color map data before calling this function.
/// @param header TGA header data.
/// @param out TGA color-mapped data converted to either 24-bit BGR or 32-bit BGRA format.
/// @returns TRUE on successful loading and conversion of data; FALSE otherwise.
BOOL TGAImageType01(std::stringstream& ss, const TGAHEADER& header, LPTGADATA out)
{
 // validate color map and pixel depth
 if(header.color_map_type != 1) return FALSE;
 if(!(header.pixel_depth == 8 || header.pixel_depth == 16)) return FALSE;

 // determine size of color map
 DWORD map_bytes = 0;
 switch(header.color_map_bits) {
   case(15) : map_bytes = 2*header.color_map_length; break;
   case(16) : map_bytes = 2*header.color_map_length; break;
   case(24) : map_bytes = 3*header.color_map_length; break;
   case(32) : map_bytes = 4*header.color_map_length; break;
   default : return FALSE;
  }

 // create and read color map
 std::unique_ptr<BYTE[]> colormap(new BYTE[map_bytes]);
 ss.read((char*)colormap.get(), map_bytes);
 if(ss.fail()) return FALSE;

 // read index data
 DWORD n_pixels = header.dx*header.dy;
 DWORD datasize = n_pixels*header.pixel_depth;
 std::unique_ptr<BYTE[]> src_data(new BYTE[datasize]);
 ss.read((char*)src_data.get(), datasize);
 if(ss.fail()) return FALSE;

 // create destination data
 TGADATA dst;
 dst.dx = header.dx;
 dst.dy = header.dy;
 dst.pitch_bytes = dst.dx*4;
 dst.total_bytes = dst.dy*dst.pitch_bytes;
 std::unique_ptr<BYTE[]> dst_data(new BYTE[dst.total_bytes]);

 // convert TGA data to D3D data
 BOOL success = FALSE;
 switch(header.color_map_bits) {
   case(15) : success = TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), src_data.get(), header.pixel_depth, dst_data.get(), TGA15_to_D3D32); break; // X1R5G5B5
   case(16) : success = TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), src_data.get(), header.pixel_depth, dst_data.get(), TGA16_to_D3D32); break; // A1R5G5B5
   case(24) : success = TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), src_data.get(), header.pixel_depth, dst_data.get(), TGA24_to_D3D32); break; // B8G8R8
   case(32) : success = TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), src_data.get(), header.pixel_depth, dst_data.get(), TGA32_to_D3D32); break; // B8G8R8A8
  }
 if(!success) return FALSE;

 // move data
 out->dx = dst.dx;
 out->dy = dst.dy;
 out->bpp = 32;
 out->pitch_bytes = dst.pitch_bytes;
 out->total_bytes = dst.total_bytes;
 out->data = std::move(dst_data);
 return TRUE;
}

BOOL TGAImageType02(std::stringstream& ss, const TGAHEADER& header, D3DImageData& imgref)
{
 // skip any color map data if stored in file
 if(header.color_map_type != 0) {
    if(header.color_map_bits == 15) ss.seekg(header.color_map_length*2, std::ios::cur);
    else if(header.color_map_bits == 16) ss.seekg(header.color_map_length*2, std::ios::cur);
    else if(header.color_map_bits == 24) ss.seekg(header.color_map_length*3, std::ios::cur);
    else if(header.color_map_bits == 32) ss.seekg(header.color_map_length*4, std::ios::cur);
    else return FALSE;    
   }

 // extract number of alpha bits
 BYTE alpha_bits = (header.image_descriptor & 0x0F);

 // compute source pitch bytes
 DWORD pitch_bytes = 0;
 if(header.pixel_depth == 8) pitch_bytes = header.dx;
 else if(header.pixel_depth == 15) pitch_bytes = 2*header.dx;
 else if(header.pixel_depth == 16) pitch_bytes = 2*header.dx;
 else if(header.pixel_depth == 24) pitch_bytes = 3*header.dx;
 else if(header.pixel_depth == 32) pitch_bytes = 4*header.dx;
 else return FALSE;

 // initialize src data
 D3DImageInfo src;
 src.dx = header.dx;
 src.dy = header.dy;
 src.pitch_bytes = pitch_bytes;
 src.total_bytes = pitch_bytes*src.dy;

 // create src data and read data
 std::unique_ptr<BYTE[]> src_data(new BYTE[src.total_bytes]);
 ss.read((char*)src_data.get(), src.total_bytes);
 if(ss.fail()) return FALSE;

 // initialize and create dst data
 D3DImageInfo dst;
 dst.dx = header.dx;
 dst.dy = header.dy;
 dst.pitch_bytes = dst.dx*4;
 dst.total_bytes = dst.dy*dst.pitch_bytes;
 std::unique_ptr<BYTE[]> dst_data(new BYTE[dst.total_bytes]);

 // TGA M8 (mono formats shouldn't be here, but just in case)
 if(header.pixel_depth == 8 && !alpha_bits) {
    TGAMono08 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // TGA M8A8 (mono formats shouldn't be here, but just in case)
 else if(header.pixel_depth == 16 && alpha_bits == 8) {
    TGAMono16 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // TGA X1R5G5B5
 else if(header.pixel_depth == 15) {
    TGAColor15 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // TGA A1R5G5B5
 else if(header.pixel_depth == 16 && (alpha_bits == 0 || alpha_bits == 1)) {
    TGAColor16 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // TGA B8G8R8
 else if(header.pixel_depth == 24) {
    TGAColor24 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // TGA B8G8R8A8
 else if(header.pixel_depth == 32) {
    TGAColor32 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }

 // move data
 imgref.info.dx = dst.dx;
 imgref.info.dy = dst.dy;
 imgref.info.pitch_bytes = dst.pitch_bytes;
 imgref.info.total_bytes = dst.total_bytes;
 imgref.format = DXGI_FORMAT_B8G8R8A8_UNORM;
 imgref.data = std::move(dst_data);
 return TRUE;
}

BOOL TGAImageType03(std::stringstream& ss, const TGAHEADER& header, D3DImageData& imgref)
{
 // skip any color map data if stored in file
 if(header.color_map_type != 0) {
    if(header.color_map_bits == 8) ss.seekg(header.color_map_length, std::ios::cur);
    else if(header.color_map_bits == 16) ss.seekg(header.color_map_length*2, std::ios::cur);
    else return FALSE;    
   }

 // extract number of alpha bits
 BYTE alpha_bits = (header.image_descriptor & 0x0F);

 // compute source pitch bytes
 DWORD pitch_bytes = 0;
 if(header.pixel_depth == 8) pitch_bytes = header.dx;
 else return FALSE;

 // initialize src data
 D3DImageInfo src;
 src.dx = header.dx;
 src.dy = header.dy;
 src.pitch_bytes = pitch_bytes;
 src.total_bytes = pitch_bytes*header.dy;

 // create src data and read data
 std::unique_ptr<BYTE[]> src_data(new BYTE[src.total_bytes]);
 ss.read((char*)src_data.get(), src.total_bytes);
 if(ss.fail()) return FALSE;

 // create dst data
 D3DImageInfo dst;
 dst.dx = header.dx;
 dst.dy = header.dy;
 dst.pitch_bytes = dst.dx*4;
 dst.total_bytes = dst.dy*dst.pitch_bytes;
 std::unique_ptr<BYTE[]> dst_data(new BYTE[dst.total_bytes]);

 // TGA M8
 if(header.pixel_depth == 8 && !alpha_bits) {
    TGAMono08 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // TGA A8
 else if(header.pixel_depth == 8 && alpha_bits == 8) {
    TGAMono08 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // TGA M8A8
 else if(header.pixel_depth == 16 && alpha_bits == 8) {
    TGAMono16 obj;
    obj(dst.dx, dst.dy, src_data.get(), dst_data.get());
   }
 // UNSUPPORTED
 else
    return FALSE;

 // move data
 imgref.info.dx = dst.dx;
 imgref.info.dy = dst.dy;
 imgref.info.pitch_bytes = dst.pitch_bytes;
 imgref.info.total_bytes = dst.total_bytes;
 imgref.format = DXGI_FORMAT_B8G8R8A8_UNORM;
 imgref.data = std::move(dst_data);
 return TRUE;
}

BOOL TGAImageType09(std::stringstream& ss, const TGAHEADER& header, D3DImageData& imgref)
{
 // must have color map, pixel depth must be 0x08 or 0x10
 if(header.color_map_type != 1) return FALSE;
 if(!(header.pixel_depth == 8 || header.pixel_depth == 16)) return FALSE;

 // determine size of color map
 DWORD map_bytes = 0;
 if(header.color_map_bits == 15) map_bytes = 2*header.color_map_length;
 else if(header.color_map_bits == 16) map_bytes = 2*header.color_map_length;
 else if(header.color_map_bits == 24) map_bytes = 3*header.color_map_length;
 else if(header.color_map_bits == 32) map_bytes = 4*header.color_map_length;
 else return FALSE;

 // create and read color map
 std::unique_ptr<BYTE[]> colormap(new BYTE[map_bytes]);
 ss.read((char*)colormap.get(), map_bytes);
 if(ss.fail()) return FALSE;

 // create index data
 DWORD n_pixels = header.dx*header.dy;
 DWORD datasize = n_pixels*header.pixel_depth;
 std::unique_ptr<BYTE[]> indexed_data(new BYTE[datasize]);

 // decompress index data
 if(header.pixel_depth == 8) {
    BYTE* ptr = indexed_data.get();
    if(!TGADecompressMappedRLE(ss, ptr, n_pixels)) return FALSE;
   }
 else {
    WORD* ptr = reinterpret_cast<WORD*>(indexed_data.get());
    if(!TGADecompressMappedRLE(ss, ptr, n_pixels)) return FALSE;
   }

 // create destination data
 D3DImageInfo dst;
 dst.dx = header.dx;
 dst.dy = header.dy;
 dst.pitch_bytes = dst.dx*4;
 dst.total_bytes = dst.dy*dst.pitch_bytes;
 std::unique_ptr<BYTE[]> dst_data(new BYTE[dst.total_bytes]);

 // X1R5G5B5
 if(header.color_map_bits == 15) {
    if(!TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), indexed_data.get(), header.pixel_depth, dst_data.get(), TGA15_to_D3D32))
       return FALSE;
   }
 // A1R5G5B5
 else if(header.color_map_bits == 16) {
    if(!TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), indexed_data.get(), header.pixel_depth, dst_data.get(), TGA16_to_D3D32))
       return FALSE;
   }
 // B8G8R8
 else if(header.color_map_bits == 24) {
    if(!TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), indexed_data.get(), header.pixel_depth, dst_data.get(), TGA24_to_D3D32))
       return FALSE;
   }
 // B8G8R8A8
 else if(header.color_map_bits == 32) {
    if(!TGACopyColorMappedImage(dst.dx, dst.dy, colormap.get(), indexed_data.get(), header.pixel_depth, dst_data.get(), TGA32_to_D3D32))
       return FALSE;
   }
 // UNSUPPORTED
 else
    return FALSE;

 // move data
 imgref.info.dx = dst.dx;
 imgref.info.dy = dst.dy;
 imgref.info.pitch_bytes = dst.pitch_bytes;
 imgref.info.total_bytes = dst.total_bytes;
 imgref.format = DXGI_FORMAT_B8G8R8A8_UNORM;
 imgref.data = std::move(dst_data);
 return TRUE;
}

BOOL TGAImageType10(std::stringstream& ss, const TGAHEADER& header, D3DImageData& imgref)
{
 // skip any color map data if stored in file
 if(header.color_map_type != 0) {
    if(header.color_map_bits == 15) ss.seekg(header.color_map_length*2, std::ios::cur);
    else if(header.color_map_bits == 16) ss.seekg(header.color_map_length*2, std::ios::cur);
    else if(header.color_map_bits == 24) ss.seekg(header.color_map_length*3, std::ios::cur);
    else if(header.color_map_bits == 32) ss.seekg(header.color_map_length*4, std::ios::cur);
    else return FALSE;    
   }

 // extract number of alpha bits
 BYTE alpha_bits = (header.image_descriptor & 0x0F);

 // initialize destination data
 D3DImageInfo dst_info;
 dst_info.dx = header.dx;
 dst_info.dy = header.dy;
 dst_info.pitch_bytes = dst_info.dx*4;
 dst_info.total_bytes = dst_info.dy*dst_info.pitch_bytes;
 std::unique_ptr<BYTE[]> dst_data(new BYTE[dst_info.total_bytes]);

 // TGA M8
 if(header.pixel_depth == 8 && !alpha_bits) {
    if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAMonoRLE08()))
       return FALSE;
   }
 // TGA M8A8
 else if(header.pixel_depth == 16 && alpha_bits == 8) {
    if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAMonoRLE16()))
       return FALSE;
   }
 // TGA X1R5G5B5
 else if(header.pixel_depth == 15) {
    if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAColorRLE15()))
       return FALSE;
   }
 // TGA A1R5G5B5
 else if(header.pixel_depth == 16 && (alpha_bits == 0 || alpha_bits == 1)) {
    if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAColorRLE16()))
       return FALSE;
   }
 // TGA B8G8R8
 else if(header.pixel_depth == 24) {
    if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAColorRLE24()))
       return FALSE;
   }
 // TGA B8G8R8A8
 else if(header.pixel_depth == 32) {
    if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAColorRLE32()))
       return FALSE;
   }
 // UNSUPPORTED
 else
    return FALSE;

 // move data
 imgref.info.dx = dst_info.dx;
 imgref.info.dy = dst_info.dy;
 imgref.info.pitch_bytes = dst_info.pitch_bytes;
 imgref.info.total_bytes = dst_info.total_bytes;
 imgref.format = DXGI_FORMAT_B8G8R8A8_UNORM;
 imgref.data = std::move(dst_data);
 return TRUE;
}

BOOL TGAImageType11(std::stringstream& ss, const TGAHEADER& header, D3DImageData& imgref)
{
 // skip any color map data if stored in file
 if(header.color_map_type != 0) {
    if(header.color_map_bits == 15) ss.seekg(header.color_map_length*2, std::ios::cur);
    else if(header.color_map_bits == 16) ss.seekg(header.color_map_length*2, std::ios::cur);
    else if(header.color_map_bits == 24) ss.seekg(header.color_map_length*3, std::ios::cur);
    else if(header.color_map_bits == 32) ss.seekg(header.color_map_length*4, std::ios::cur);
    else return FALSE;    
   }

 // extract number of alpha bits
 BYTE alpha_bits = (header.image_descriptor & 0x0F);

 // initialize destination data
 D3DImageInfo dst_info;
 dst_info.dx = header.dx;
 dst_info.dy = header.dy;
 dst_info.pitch_bytes = dst_info.dx*4;
 dst_info.total_bytes = dst_info.dy*dst_info.pitch_bytes;
 std::unique_ptr<BYTE[]> dst_data(new BYTE[dst_info.total_bytes]);

 // M8 or A8
 if(header.pixel_depth == 8) {
    if(!alpha_bits || alpha_bits == 8) {
       if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAMonoRLE08()))
          return FALSE;
      }
    else
       return FALSE;
   }
 // TGA R8A8
 else if(header.pixel_depth == 16 && alpha_bits == 8) {
    if(!TGADecompressRLE(ss, dst_info, dst_data.get(), TGAMonoRLE16()))
       return FALSE;
   }
 // UNSUPPORTED
 else
    return FALSE;

 // move data
 imgref.info.dx = dst_info.dx;
 imgref.info.dy = dst_info.dy;
 imgref.info.pitch_bytes = dst_info.pitch_bytes;
 imgref.info.total_bytes = dst_info.total_bytes;
 imgref.format = DXGI_FORMAT_B8G8R8A8_UNORM;
 imgref.data = std::move(dst_data);
 return TRUE;
}

#pragma endregion TGA_EXPLICIT_HANDLING

#pragma region TGA_LOADSAVE

bool LoadTGA(const LPCWSTR filename, LPTGADATA data)
{
 // must have valid filename
 if(!filename) return error(__FILE__, __LINE__);

 // open file
 using namespace std;
 ifstream ifile(filename, ios::binary);
 if(!ifile) return error(__FILE__, __LINE__);

 // get filesize
 ifile.seekg(0, ios::end);
 DWORD filesize = (DWORD)ifile.tellg();
 ifile.seekg(0, ios::beg);
 if(ifile.fail()) return error(__FILE__, __LINE__);

 // read into stringstream
 std::stringstream ss;
 ss << ifile.rdbuf() << ends;
 ifile.close();

 // read header
 TGAHEADER header;
 ss.read((char*)&header.imageID, sizeof(header.imageID));
 ss.read((char*)&header.color_map_type, sizeof(header.color_map_type));
 ss.read((char*)&header.image_type, sizeof(header.image_type));
 ss.read((char*)&header.color_map_origin, sizeof(header.color_map_origin));
 ss.read((char*)&header.color_map_length, sizeof(header.color_map_length));
 ss.read((char*)&header.color_map_bits, sizeof(header.color_map_bits));
 ss.read((char*)&header.xorigin, sizeof(header.xorigin));
 ss.read((char*)&header.yorigin, sizeof(header.yorigin));
 ss.read((char*)&header.dx, sizeof(header.dx));
 ss.read((char*)&header.dy, sizeof(header.dy));
 ss.read((char*)&header.pixel_depth, sizeof(header.pixel_depth));
 ss.read((char*)&header.image_descriptor, sizeof(header.image_descriptor));
 if(ss.fail()) return error(__FILE__, __LINE__);

 // special case #1: TGA file contains no data
 if(header.image_type == 0x00)
    return error(__FILE__, __LINE__);

 // skip past image identification
 if(header.imageID) ss.seekg(header.imageID, ios_base::cur);
 if(ifile.fail()) return error(__FILE__, __LINE__);

 // read image data
 BOOL retval = FALSE;
 switch(header.image_type) {
   case(0x01): retval = TGAImageType01(ss, header, data); break; // UNCOMPRESSED COLOR-MAPPED
   case(0x02): retval = TGAImageType02(ss, header, data); break; // UNCOMPRESSED RGB
   case(0x03): retval = TGAImageType03(ss, header, data); break; // UNCOMPRESSED GRAYSCALE
   case(0x09): retval = TGAImageType09(ss, header, data); break; // RLE-COMPRESSED COLOR-MAPPED
   case(0x0A): retval = TGAImageType10(ss, header, data); break; // RLE-COMPRESSED RGB
   case(0x0B): retval = TGAImageType11(ss, header, data); break; // RLE-COMPRESSED GRAYSCALE
   default: return error(__FILE__, __LINE__);
  }

 // // image must be flipped
 // if(header.yorigin == 0x00) {
 //    // 32-bit BGRA
 //    if(data->format == DXGI_FORMAT_B8G8R8A8_UNORM) {
 //       DWORD src_pitch = data->dx * 4;
 //       DWORD src_total = data->dy * src_pitch;
 //       UINT* upper = reinterpret_cast<UINT*>(data->data[0]);
 //       UINT* lower = reinterpret_cast<UINT*>(data->data[src_total - src_pitch]);
 //       UINT elem = src_pitch/4;
 //       while(upper < lower) {
 //             for(UINT i = 0; i < elem; i++) std::swap(upper[i], lower[i]);
 //             upper += elem;
 //             lower -= elem;
 //            }
 //      }
 //   }

 return true;
}

#pragma endregion TGA_LOADSAVE
*/