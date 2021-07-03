#ifndef __RT_DEPTH_H
#define __RT_DEPTH_H

#include "RT_math.h"

struct AlphaPixel {
 vector4D<float> color;
 float distance;
};

struct DepthTest {
 public :
  static const unsigned int max_size = 64;
  AlphaPixel pixellist[max_size];
  unsigned int n;
 public :
  DepthTest() : n(0)
  {
  }
 ~DepthTest()
  {
  }
 public :
  vector4D<float> blend(void)const
  {
   if(n == 0) return pixellist[0].color;
   vector4D<float> dst = pixellist[n - 1].color;
   for(unsigned int i = 1; i < n; i++) {
       const vector4D<float>& src = pixellist[n - 1 - i].color;
       float a1 = src[3];
       float a2 = 1.0f - src[3];
       dst[0] = src[0]*a1 + dst[0]*a2;
       dst[1] = src[1]*a1 + dst[1]*a2;
       dst[2] = src[2]*a1 + dst[2]*a2;
       dst[3] = 1.0f;
      }
   return dst;
  }
  void Update(const vector4D<float>& color, float distance)
  {
   // CASE #1
   // FIRST PIXEL IS ALWAYS OPAQUE
   if(!n) {
      pixellist[0].color = color;
      pixellist[0].color[3] = 1.0f;
      pixellist[0].distance = distance;
      n = 1;
      return;
     }

   // CASE #2
   // ANYTHING BEHIND OPAQUE PIXEL ALWAYS FAILS
   if(!((distance + 1.0e-6f) < pixellist[n - 1].distance)) return;

   // CASE #3
   // OPAQUE PIXELS
   if(color[3] == 1.0f)
     {
      if(n == max_size)
        {
         // distance must be less than farthest transparent pixel
         size_t index = n - 2;
         for(size_t i = 1; i < n; i++) {
             if((distance + 1.0e-6f) < pixellist[index].distance) pixellist[index + 1] = pixellist[index];
             else break;
             index--;
            }
         pixellist[index + 1].color = color;
         pixellist[index + 1].distance = distance;
         n = index + 2;
        }
      else
        {
         // distance is already less than opaque pixel
         size_t index = n - 1;
         for(size_t i = 0; i < n; i++) {
             if((distance + 1.0e-6f) < pixellist[index].distance) pixellist[index + 1] = pixellist[index];
             else break;
             index--;
            }
         pixellist[index + 1].color = color;
         pixellist[index + 1].distance = distance;
         n = index + 2;
        }
     }
   // CASE #4
   // TRANSPARENT PIXELS
   else
     {
      if(n == max_size)
        {
         // distance must be less than farthest transparent pixel
         size_t index = n - 2;
         if((distance + 1.0e-6f) < pixellist[index].distance) {
            index--;
            for(size_t i = 2; i < n; i++) {
                if((distance + 1.0e-6f) < pixellist[index].distance) pixellist[index + 1] = pixellist[index];
                else break;
                index--;
               }
            pixellist[index + 1].color = color;
            pixellist[index + 1].distance = distance;
           }
        }
      else
        {
         // distance is already less than opaque pixel
         size_t index = n - 1;
         for(size_t i = 0; i < n; i++) {
             if((distance + 1.0e-6f) < pixellist[index].distance) pixellist[index + 1] = pixellist[index];
             else break;
             index--;
            }
         pixellist[index + 1].color = color;
         pixellist[index + 1].distance = distance;
         n++;
        }
     }
  }
};

#endif
