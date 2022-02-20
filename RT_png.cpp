#include "stdafx.h"
#include "RT_win32.h"
#include "RT_png.h"

bool LoadPNG(LPCWSTR filename, LPPNGDATA out)
{
 // validate
 if(!filename) return error(__FILE__, __LINE__);
 if(!out) return error(__FILE__, __LINE__);

 // create WIC factory
 CComPtr<IWICImagingFactory> wicf;
 HRESULT result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicf);
 if(FAILED(result)) return error(__FILE__, __LINE__);

 // create WIC PNG decoder
 CComPtr<IWICBitmapDecoder> wicd;
 result = wicf->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &wicd);
 if(FAILED(result)) return error(__FILE__, __LINE__);

 // must have at least one frame
 UINT count = 0;
 result = wicd->GetFrameCount(&count);
 if(FAILED(result) || (count == 0))
    return error(__FILE__, __LINE__);

 // get frame (PNG files have only one frame)
 CComPtr<IWICBitmapFrameDecode> frame = NULL;
 if(FAILED(wicd->GetFrame(0, &frame)))
   return error(__FILE__, __LINE__);

 // get pixel format
 WICPixelFormatGUID pfguid;
 if(FAILED(frame->GetPixelFormat(&pfguid))) return error(__FILE__, __LINE__);

 // the only alpha formats
 bool has_alpha = FALSE;
 if(IsEqualGUID(pfguid, GUID_WICPixelFormat32bppBGRA)) has_alpha = TRUE;
 else if(IsEqualGUID(pfguid, GUID_WICPixelFormat64bppRGBA)) has_alpha = TRUE;

 // create format converter
 CComPtr<IWICFormatConverter> converter = NULL;
 if(FAILED(wicf->CreateFormatConverter(&converter)))
    return error(__FILE__, __LINE__);

 // initialize format converter (always convert to 32 bpp)
 // if you need pre-multiplied alpha, use GUID_WICPixelFormat32bppPBGRA
 result = converter->Initialize(frame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
 if(FAILED(result)) return error(__FILE__, __LINE__);

 // retrieve dimensions
 UINT dx = 0;
 UINT dy = 0;
 if(FAILED(converter->GetSize(&dx, &dy)) || (!dx || !dy))
    return error(__FILE__, __LINE__);

 // copy image
 UINT pitch_bytes = 4*dx;
 UINT total_bytes = pitch_bytes*dy;
 std::unique_ptr<BYTE[]> buffer(new BYTE[total_bytes]);
 if(FAILED(converter->CopyPixels(NULL, pitch_bytes, total_bytes, buffer.get())))
    return error(__FILE__, __LINE__);

 // set data
 out->dx = dx;
 out->dy = dy;
 out->bpp = 32;
 out->pitch_bytes = pitch_bytes;
 out->total_bytes = total_bytes;
 out->has_alpha = has_alpha;
 out->data = std::move(buffer);

 // success
 return true;
}