#include "stdafx.h"
#include "RT_math.h"
#include "RT_dds.h"
#include "RT_bvh.h"
#include "RT_depth.h"
#include "RT_lights.h"
#include "RT_texture.h"
#include "RT_win32.h"
#include "RT_xnalara.h"
#include "RT_lights.h"
#include "RT_bvh.h"
#include "RT_raytrace.h"
using namespace std;

#define TIME_TEST

bool GenerateMultiJitteredUnitSquareSamples(std::vector<vector2D<float>>& dst, size_t m, size_t n);
//bool ConvertModel(XNAModel& src, RTMergedModel& dst);
//bool ConvertToOBJ(const RTMergedModel& model);

void render(void)
{
 // load model into ray tracer
 if(!LoadModel(L"C:\\Users\\semory\\Desktop\\DESKTOP\\xnalara\\Alcina Dimitrescu\\xps.xps"))
    return;

 // initialize default camera and lights
 InitCamera();
 InitLights2();

 // set render target dimensions
 SetClearColor(vector4D<float>(0.0f, 0.0f, 0.0f, 0.0f));
 SetTargetDims(1920, 1080);
 SetJitterDims(8);

 // render
 Trace(L"test.bmp");
}

int main()
{
 render();
 return 0;

 // time testing
 #ifdef TIME_TEST
 __int64 hz;
 __int64 t0, t1;
 if(!QueryPerformanceFrequency((LARGE_INTEGER*)&hz))
    return (int)error("Performance counter not available.", __FILE__, __LINE__);
 #endif

 // load model
 // Alcina Dimitrescu -> 240x180 -> 446.748 seconds (step006)
 // Alcina Dimitrescu -> 240x180 ->  68.197 seconds (step007)
 XNAModel model;
 if(!LoadXPSMeshBin(L"C:\\Users\\semory\\Desktop\\DESKTOP\\xnalara\\Alcina Dimitrescu\\xps.xps", &model)) {
    cout << "Failed to load mesh binary." << endl;
    return 0;
   }
 cout << "Number of meshes = " << model.n_mesh << endl;

 // construct BVH
 RTDynamicBVH bvh;
 bvh.construct(&model);
 cout << "BVH nodes = " << bvh.nodes() << endl;

 // initialize lights
 InitLights();

 // create image
 unsigned int image_dx = 1920; // 800;
 unsigned int image_dy = 1080; // 600;
 unsigned int image_pitch = image_dx*4;
 unsigned int image_size = image_pitch*image_dy;
 unique_ptr<unsigned char[]> image(new unsigned char[image_size]);

 // time test
 #ifdef TIME_TEST
 QueryPerformanceCounter((LARGE_INTEGER*)&t0);
 #endif

 // camera position + orientation
 float E[3] = { 0.0f, 1.4f, 8.0f }; // eye position vector (origin)
 float D[3] = { 0.0f, 0.0f,-1.0f }; // look-at direction (x-axis)
 float L[3] = {-1.0f, 0.0f, 0.0f }; // left direction (y-axis)
 float U[3] = { 0.0f, 1.0f, 0.0f }; // up direction (z-axis)

 // camera properties
 float nplane = 0.05f;
 float fplane = 150.0f;
 float pratio = fplane/nplane;
 float fovy = 30.0f;
 float aspect = static_cast<float>(image_dx)/static_cast<float>(image_dy);

 // near plane half-dimensions
 float frame_hy = nplane*std::tan(radians(fovy/2.0f));
 float frame_hx = frame_hy*aspect;

 // near plane dimensions
 float frame_dy = 2.0f*frame_hy;
 float frame_dx = 2.0f*frame_hx;

 // spacing between pixels
 float spacing_dx = frame_dx/static_cast<float>(image_dx);
 float spacing_dy = frame_dy/static_cast<float>(image_dy);

 // compute reference point for image origin
 float n1[3] = {
  E[0] + nplane*D[0] + frame_hx*L[0] + frame_hy*U[0],
  E[1] + nplane*D[1] + frame_hx*L[1] + frame_hy*U[1],
  E[2] + nplane*D[2] + frame_hx*L[2] + frame_hy*U[2]
 };

 // ray tracing variables
 vector4D<float> clearcolor;
 clearcolor[0] = 0.0f;
 clearcolor[1] = 0.0f;
 clearcolor[2] = 0.0f;
 clearcolor[3] = 1.0f;

 // sampling variables
 size_t m = 4;
 size_t n = 4;
 size_t n_samples = m*n;

 // for each pixel in the image
 int total_pixels = image_dx*image_dy;
 int curr_pixel = 0;
 for(unsigned int r = 0; r < image_dy; r++)
    {
     for(unsigned int c = 0; c < image_dx; c++)
        {
         // generate samples
         std::vector<vector2D<float>> pointlist;
         GenerateMultiJitteredUnitSquareSamples(pointlist, m, n);

         // sample accumulator
         vector4D<float> sample_color;
         sample_color[0] = 0.0f;
         sample_color[1] = 0.0f;
         sample_color[2] = 0.0f;
         sample_color[3] = 0.0f;

         // for each sample
         for(const vector2D<float>& point : pointlist)
            {
             // compute direction vector
             float scale_L = (c + point[0])*spacing_dx;
             float scale_U = (r + point[1])*spacing_dy;
             float screen_pnt[3] = {
              n1[0] - (scale_L*L[0] + scale_U*U[0]),
              n1[1] - (scale_L*L[1] + scale_U*U[1]),
              n1[2] - (scale_L*L[2] + scale_U*U[2])
             };
             float screen_dir[3] = {
              screen_pnt[0] - E[0],
              screen_pnt[1] - E[1],
              screen_pnt[2] - E[2]
             };
             float tn = vector3D_normalize(&screen_dir[0]);
             float tf = tn*pratio;

             // initialize depth test
             DepthTest dt;
             dt.Update(clearcolor, tf);

             // initialize ray
             ray3D ray;
             ray.origin[0] = screen_pnt[0];
             ray.origin[1] = screen_pnt[1];
             ray.origin[2] = screen_pnt[2];
             ray.direction[0] = screen_dir[0];
             ray.direction[1] = screen_dir[1];
             ray.direction[2] = screen_dir[2];
             ray.inverse[0] = (zerotest(screen_dir[0]) ? std::numeric_limits<float>::infinity() : 1.0f/screen_dir[0]);
             ray.inverse[1] = (zerotest(screen_dir[1]) ? std::numeric_limits<float>::infinity() : 1.0f/screen_dir[1]);
             ray.inverse[2] = (zerotest(screen_dir[2]) ? std::numeric_limits<float>::infinity() : 1.0f/screen_dir[2]);

             // test ray
             bvh.set_depth_test(&dt);
             bvh.raytest(ray);

             // accumulate color
             auto output = dt.blend();
             sample_color[0] += output[0];
             sample_color[1] += output[1];
             sample_color[2] += output[2];
             sample_color[3] += output[3];
            }

         // set pixel color based on depth test
         unsigned int index = (image_dy - r - 1)*image_pitch + 4*c;
         unsigned char* ptr = reinterpret_cast<unsigned char*>(image.get()) + index;
         ptr[0] = static_cast<uint8_t>(255.0f*sample_color[2]/n_samples);
         ptr[1] = static_cast<uint8_t>(255.0f*sample_color[1]/n_samples);
         ptr[2] = static_cast<uint8_t>(255.0f*sample_color[0]/n_samples);
         ptr[3] = static_cast<uint8_t>(255.0f*sample_color[3]/n_samples);

         curr_pixel++;
         #ifndef TIME_TEST
         std::cout << "Percent done = " << (100.0f*(float)curr_pixel)/(float)total_pixels << "%" << std::endl;
         #endif
        }
    }

 // time test
 #ifdef TIME_TEST
 QueryPerformanceCounter((LARGE_INTEGER*)&t1);
 double elapsed = (double)(t1 - t0)/(double)hz;
 std::cout << "Image creation time: " << elapsed << " seconds." << std::endl;
 #endif

 // create image file
 SaveDibFile32(L"test.bmp", image_dx, image_dy, image.get());
 return 0;
}

#pragma region SAMPLING_FUNCTIONS

bool GenerateMultiJitteredUnitSquareSamples(std::vector<vector2D<float>>& dst, size_t m, size_t n)
{
 // must have at least a 2x2 square
 if(m == 1 && n == 1) {
    vector2D<float> v;
    v[0] = 0.5f;
    v[1] = 0.5f;
    dst.push_back(v);
    return true;
   }

 // resize sample list if necessary
 size_t n_elem = m*n;
 if(dst.size() != n_elem) dst.resize(n_elem);
  
 // random number generation
 std::random_device rd;
 std::mt19937 gen(rd());
 std::uniform_real_distribution<float> urd(0.0f, 1.0f);

 // generate canonical pattern
 float subcell_dim = 1.0f/static_cast<float>(m*n);
 size_t index = 0;
 for(size_t r = 0; r < m; r++) {
     for(size_t c = 0; c < n; c++) {
         vector2D<float> point;
         point[0] = (c*m + r)*subcell_dim;
         point[1] = (r*n + c)*subcell_dim;
         dst[index++] = point;
        }
    }

 // shuffle along x
 for(size_t r = 0; r < m; r++) {
     for(size_t c = 0; c < n; c++) {
         std::uniform_int_distribution<size_t> uid(c, n - 1);
         size_t j = uid(gen);
         std::swap(dst[r*n + c][0], dst[r*n + j][0]);
        }
    }

 // shuffle along y
 for(size_t c = 0; c < n; c++) {
     for(size_t r = 0; r < m; r++) {
         size_t j = r;
         std::uniform_int_distribution<size_t> uid(j, m - 1);
         size_t i = uid(gen);
         std::swap(dst[i*n + c][0], dst[j*n + c][0]);
        }
    }

 return true;
}

#pragma endregion SAMPLING_FUNCTIONS

/*
bool ConvertModel(XNAModel& src, RTMergedModel& dst)
{
 using namespace std;

 // count points and faces
 uint32_t total_verts = 0;
 uint32_t total_faces = 0;
 for(size_t i = 0; i < src.meshlist.size(); i++) {
     total_verts += src.meshlist[i].n_vert;
     total_faces += src.meshlist[i].n_face;
    }
 cout << "Total verts = " << total_verts << endl; 
 cout << "Total faces = " << total_faces << endl;

 // allocate model
 RTMergedModel temp;
 temp.n_vert = total_verts;
 temp.n_face = total_faces;
 temp.pos0.reset(new vector3D<float>[temp.n_vert]);
 temp.pos1.reset(new vector3D<float>[temp.n_vert]);
 temp.uv[0].reset(new vector2D<float>[temp.n_vert]);
 temp.uv[1].reset(new vector2D<float>[temp.n_vert]);
 temp.facelist.reset(new RTFace[temp.n_face]);

 // copy data over
 uint32_t voffset = 0;
 uint32_t foffset = 0;
 for(uint32_t i = 0; i < src.meshlist.size(); i++) {
     // copy vertex data
     uint32_t n_channels = (src.meshlist[i].n_channels > 2 ? 2 : src.meshlist[i].n_channels);
     for(uint32_t j = 0; j < src.meshlist[i].n_vert; j++) {
         uint32_t vindex = voffset + j;
         temp.pos0[vindex].v[0] = src.meshlist[i].verts[j].position[0];
         temp.pos0[vindex].v[1] = src.meshlist[i].verts[j].position[1];
         temp.pos0[vindex].v[2] = src.meshlist[i].verts[j].position[2];
         temp.pos1[vindex].v[0] = src.meshlist[i].verts[j].position[0];
         temp.pos1[vindex].v[1] = src.meshlist[i].verts[j].position[1];
         temp.pos1[vindex].v[2] = src.meshlist[i].verts[j].position[2];
         for(size_t k = 0; k < n_channels; k++) {
             temp.uv[k][vindex].v[0] = src.meshlist[i].verts[j].uv[k][0];
             temp.uv[k][vindex].v[1] = src.meshlist[i].verts[j].uv[k][1];
            }
        }
     // copy face data
     for(uint32_t j = 0; j < src.meshlist[i].n_face; j++) {
         uint32_t findex = foffset + j;
         temp.facelist[findex].a = src.meshlist[i].faces[j].refs[0] + voffset;
         temp.facelist[findex].b = src.meshlist[i].faces[j].refs[1] + voffset;
         temp.facelist[findex].c = src.meshlist[i].faces[j].refs[2] + voffset;
         temp.facelist[findex].mesh_index = i;
        }

     // increment offsets
     voffset += src.meshlist[i].n_vert;
     foffset += src.meshlist[i].n_face;
    }

 // move data
 dst.n_vert = temp.n_vert;
 dst.pos0 = std::move(temp.pos0);
 dst.pos1 = std::move(temp.pos1);
 dst.uv[0] = std::move(temp.uv[0]);
 dst.uv[1] = std::move(temp.uv[1]);
 dst.n_face = temp.n_face;
 dst.facelist = std::move(temp.facelist);

 return true;
}

bool ConvertToOBJ(const RTMergedModel& model)
{
 using namespace std;
 ofstream ofile("output.obj");
 if(!ofile) return false;

 ofile << "o output" << endl;
 for(size_t i = 0; i < model.n_vert; i++)
     ofile << "v " << model.pos0[i][0] << " " << model.pos0[i][1] << " " << model.pos0[i][2] << endl;

 for(size_t i = 0; i < model.n_face; i++) {
     uint32_t a = model.facelist[i].a + 1;
     uint32_t b = model.facelist[i].b + 1;
     uint32_t c = model.facelist[i].c + 1;
     ofile << "f " << a << " " << b << " " << c << endl;
    }

 return true;
}
*/