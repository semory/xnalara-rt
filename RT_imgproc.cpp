#include "stdafx.h"
#include "RT_win32.h"
#include "RT_imgproc.h"

// Color Mapping Function Objects
typedef void (*ColorMapConversionOp)(const BYTE* colormap, size_t index, BYTE* dst);

void ColorMapToTrueColor(const BYTE* colormap, int srcbpp, size_t index, BYTE* dst, int dstbpp);
