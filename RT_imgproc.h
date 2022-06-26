#ifndef __RT_IMGPROC_H
#define __RT_IMGPROC_H

typedef struct tagIMGDATA {
 DWORD dx, dy, bpp;
 DWORD pitch_bytes;
 DWORD total_bytes;
 BOOL has_alpha;
 std::unique_ptr<BYTE[]> data;
} IMGDATA, *LPIMGDATA, *PIMGDATA;


template<class IndexType>
struct ColorMapConversion_B8G8R8_to_B8G8R8A8 { 
 void operator ()(const BYTE* colormap, int entries, const IndexType* data, BYTE* dst, int dx, int dy)const
 {
  // size_t offset = 3*index;
  // dst[0] = colormap[offset++];
  // dst[1] = colormap[offset++];
  // dst[2] = colormap[offset];
  // dst[3] = 255;
 }
};

template<class IndexType>
struct ColorMapConversion_BGRA32_to_BGRA32 { 
 void operator ()(const BYTE* colormap, int entries, const IndexType* data, BYTE* dst, int dx, int dy)const
 {
  size_t offset = 0;
  int n = dx*dy;
  for(int i = 0; i < n; i++) {
      size_t index = static_cast<size_t>(sizeof(IndexType)*data[i]);
      dst[offset++] = colormap[index++];
      dst[offset++] = colormap[index++];
      dst[offset++] = colormap[index++];
      dst[offset++] = colormap[index];
     }
 }
};

#endif
