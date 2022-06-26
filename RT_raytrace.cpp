#include "stdafx.h"
#include "RT_win32.h"
#include "RT_math.h"
#include "RT_texture.h" // must come before RT_xnalara.h
#include "RT_xnalara.h"
#include "RT_bvh.h"
#include "RT_raytrace.h"

// timing variables
static bool timetest = true;
static __int64 hz = 0;
static __int64 t0 = 0, t1 = 0;

// image variables
static uint32_t image_dx = 0;
static uint32_t image_dy = 0;
static uint32_t image_pitch = 0;
static uint32_t image_bytes = 0;
static std::unique_ptr<uint8_t[]> image;

// camera variables
static float E[3], D[3];
static float L[3], U[3];
static float nplane = 0.0f, fplane = 0.0f;
static float pratio = 0.0f;
static float fovy = 0.0f;

// frame variables
static float aspect = 0.0f;
static float frame_hx = 0.0f, frame_hy = 0.0f;
static float frame_dx = 0.0f, frame_dy = 0.0f;
static float pixel_dx = 0.0f, pixel_dy = 0.0f;
static float nearref[3] = { 0.0f, 0.0f, 0.0f };

// lighting variables
static DirectionalLight lightlist[3];
static bool enable_shading = true;
static bool enable_shadows = true;
static bool k_conservation = true;
static float ambient_co[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static float ka = 0.33f;
static float ks = 0.10f; // this is a material property, only use with render groups that support specular

// sampling variables
static uint32_t n_samples_xy = 0;
static uint32_t n_samples = 0;
static std::vector<vector2D<float>> sampling_matrix;

// rendering variables
static vector4D<float> clearcolor;
static bool enable_DM = true;
static bool enable_BM = true;
static bool enable_LM = true;
static bool enable_SM = true;
static bool enable_EM = true;
static bool enable_GM = true;
static bool debug_shader = false;

// depth testing variables
struct DepthPixel {
 vector4D<float> color;
 float t;
 uint32_t mesh_index;
};
static const uint32_t MAXPIXELDEPTH = 1024; //128;
static DepthPixel depth_data[MAXPIXELDEPTH];
static uint32_t depth_elem = 0;
static float depth_tmin = 0.0f;
static float depth_tmax = 0.0f;

// sample scene
static XNAModel model;
static RTDynamicBVH bvh;

// this is thread local storage for ray testing
static size_t trace_node_stack[65536ul];
static uint32_t trace_node_elem = 0;

// this is thread local storage for shadow ray testing
static size_t shadow_node_stack[65536ul];
static uint32_t shadow_node_elem = 0;

bool SetTraceTiming(bool state)
{
 if(state == timetest) return timetest;
 bool previous = timetest;
 if(state) {
    t0 = t1 = 0;
    if(QueryPerformanceFrequency((LARGE_INTEGER*)&hz)) timetest = true;
    else {
       timetest = false;
       hz = 0;
      }
   }
 return previous;
}

bool GetTraceTiming(void)
{
 return timetest;
}

#pragma region CAMERA

void InitCamera(void)
{
 E[0] =  0.0f; // eye position
 E[1] =  1.4f;
 E[2] =  8.0f;
 D[0] =  0.0f; // D = camera x-axis
 D[1] =  0.0f;
 D[2] = -1.0f;
 L[0] = -1.0f; // L = camera y-axis
 L[1] =  0.0f;
 L[2] =  0.0f;
 U[0] =  0.0f; // U = camera z-axis
 U[1] =  1.0f;
 U[2] =  0.0f;

 // camera properties
 nplane = 0.050f;
 fplane = 150.0f;
 pratio = fplane/nplane;
 fovy = 30.0f;
}

void SetCameraPosition(float x, float y, float z)
{
 E[0] = x;
 E[1] = y;
 E[2] = z;
}

#pragma endregion CAMERA

#pragma region LIGHTS

void ComputeLightDirection(float* D, float horz, float vert)
{
 float r1 = radians(horz);
 float r2 = radians(vert);
 float m[16];
 RotationMatrix4D_YZ(m, r1, r2); // order is Z (vert) then Y (horz)
 float v[3] = { 1.0f, 0.0f, 0.0f };
 float x[3];
 Matrix4DVector3DMul(x, m, v);
 vector3D_normalize(x);
 D[0] = -x[0];
 D[1] = -x[1];
 D[2] = -x[2];
}

void InitLights(void)
{
 // light #1 (XNALara 9.7.8 default)
 lightlist[0].enabled = true;
 lightlist[0].angle_horz = 315.0f;
 lightlist[0].angle_vert =  35.0f;
 lightlist[0].color[0] = lightlist[0].color[1] = lightlist[0].color[2] = lightlist[0].color[3] = 1.0f;
 lightlist[0].intensity = 1.1f;
 lightlist[0].shadow_depth = 0.4f;
 ComputeLightDirection(lightlist[0].direction, lightlist[0].angle_horz, lightlist[0].angle_vert);

 // light #2 (XNALara 9.7.8 default)
 // XNALara 9.7.8 has all lights enabled by default; disabled lights have intensity set to 0.
 lightlist[1].enabled = false;
 lightlist[1].angle_horz = 225.0f;
 lightlist[1].angle_vert = 35.0f;
 lightlist[1].color[0] = lightlist[1].color[1] = lightlist[1].color[2] = lightlist[1].color[3] = 1.0f;
 lightlist[1].intensity = 0.0f;
 lightlist[1].shadow_depth = 0.4f;
 ComputeLightDirection(lightlist[1].direction, lightlist[1].angle_horz, lightlist[1].angle_vert);

 // light #3 (XNALara 9.7.8 default)
 // XNALara 9.7.8 has all lights enabled by default; disabled lights have intensity set to 0.
 lightlist[2].enabled = false;
 lightlist[2].angle_horz = 180.0f;
 lightlist[2].angle_vert = 90.0f;
 lightlist[2].color[0] = lightlist[2].color[1] = lightlist[2].color[2] = lightlist[2].color[3] = 1.0f;
 lightlist[2].intensity = 0.0f;
 lightlist[2].shadow_depth = 0.4f;
 ComputeLightDirection(lightlist[2].direction, lightlist[2].angle_horz, lightlist[2].angle_vert);
}

void ResetLight(int index)
{
 // light #1 (XNALara 9.7.8 default)
 if(index == 0) {
    lightlist[0].enabled = true;
    lightlist[0].angle_horz = 315.0f;
    lightlist[0].angle_vert =  35.0f;
    lightlist[0].color[0] = lightlist[0].color[1] = lightlist[0].color[2] = lightlist[0].color[3] = 1.0f;
    lightlist[0].intensity = 1.1f;
    lightlist[0].shadow_depth = 0.4f;
    ComputeLightDirection(lightlist[0].direction, lightlist[0].angle_horz, lightlist[0].angle_vert);
   }

 // light #2 (XNALara 9.7.8 default)
 // XNALara 9.7.8 has all lights enabled by default; disabled lights have intensity set to 0.
 if(index == 1) {
    lightlist[1].enabled = false;
    lightlist[1].angle_horz = 225.0f;
    lightlist[1].angle_vert = 35.0f;
    lightlist[1].color[0] = lightlist[1].color[1] = lightlist[1].color[2] = lightlist[1].color[3] = 1.0f;
    lightlist[1].intensity = 0.0f;
    lightlist[1].shadow_depth = 0.4f;
    ComputeLightDirection(lightlist[1].direction, lightlist[1].angle_horz, lightlist[1].angle_vert);
   }

 // light #3 (XNALara 9.7.8 default)
 // XNALara 9.7.8 has all lights enabled by default; disabled lights have intensity set to 0.
 if(index == 2) {
    lightlist[2].enabled = false;
    lightlist[2].angle_horz = 180.0f;
    lightlist[2].angle_vert = 90.0f;
    lightlist[2].color[0] = lightlist[2].color[1] = lightlist[2].color[2] = lightlist[2].color[3] = 1.0f;
    lightlist[2].intensity = 0.0f;
    lightlist[2].shadow_depth = 0.4f;
    ComputeLightDirection(lightlist[2].direction, lightlist[2].angle_horz, lightlist[2].angle_vert);
   }
}

#pragma endregion LIGHTS

#pragma region SHADING

bool SetShadingState(bool state)
{
 bool previous = enable_shading;
 enable_shading = state;
 return previous;
}

bool GetShadingState(void)
{
 return enable_shading;
}

bool SetShadowsState(bool state)
{
 bool previous = enable_shadows;
 enable_shadows = state;
 return previous;
}

bool GetShadowsState(void)
{
 return enable_shadows;
}


#pragma endregion SHADING

#pragma region MODEL_FUNCTIONS

bool LoadModel(const wchar_t* filename)
{
 // validate
 if(!filename) return error(__FILE__, __LINE__);

 // get file extension
 auto fullpath = std::filesystem::path(filename);
 STDSTRINGW extension = fullpath.extension();
 if(!extension.c_str()) return error(__FILE__, __LINE__);

 // OBJ
 if(_wcsicmp(extension.c_str(), L".obj") == 0) {
    return error("OBJ format is not supported yet.", __FILE__, __LINE__);
   }
 // MESH ASCII
 else if(_wcsicmp(extension.c_str(), L".mesh.ascii") == 0) {
    return error("mesh.ascii format is not supported yet.", __FILE__, __LINE__);
   }
 // MESH
 else if(_wcsicmp(extension.c_str(), L".mesh") == 0) {
    if(!LoadXNAMeshBin(filename, &model))
       return error("Failed to load mesh binary.", __FILE__, __LINE__);
   }
 // XPS
 else if(_wcsicmp(extension.c_str(), L".xps") == 0) {
    if(!LoadXPSMeshBin(filename, &model))
       return error("Failed to load XPS binary.", __FILE__, __LINE__);
   }
 else return false;

 // construct model
 bvh.construct(&model);

 // save OBJ?
 // ConvertToOBJ(&model, L"output.obj");
 return true;
}

const XNAModel* GetModel(void)
{
 return &model;
}

#pragma endregion MODEL_FUNCTIONS

#pragma region RAYTRACING

void SetClearColor(const vector4D<float>& color)
{
 clearcolor.r = color.r;
 clearcolor.g = color.g;
 clearcolor.b = color.b;
 clearcolor.a = color.a;
}

void SetTargetDims(uint32_t dx, uint32_t dy)
{
 // no need to resize
 if(image && image_dx == dx && image_dy == dy) return;

 // clear or resize
 if(dx == 0 || dy == 0) {
    image_dx = 0;
    image_dy = 0;
    image_pitch = 0;
    image_bytes = 0;
    image.reset(nullptr);
    return;
   }
 else {
    image_dx = dx;
    image_dy = dy;
    image_pitch = 4*dx;
    image_bytes = 4*dx*dy;
    image.reset(new uint8_t[image_bytes]);
   }
}

void SetJitterDims(uint32_t n)
{
 n_samples_xy = (n > 0 && n <= 16 ? n : 1);
 n_samples = n_samples_xy*n_samples_xy;
 sampling_matrix.resize(n_samples);
}

bool Trace(const wchar_t* filename)
{
 // must have image
 if(!image) return error("Cannot ray trace without an image.", __FILE__, __LINE__);
 if(!n_samples) return error("Cannot ray trace without setting sampling information.", __FILE__, __LINE__);

 // set aspect ratio
 aspect = static_cast<float>(image_dx)/static_cast<float>(image_dy);

 // near plane half-dimensions
 frame_hy = nplane*std::tan(radians(fovy/2.0f));
 frame_hx = frame_hy*aspect;

 // near plane dimensions
 frame_dy = 2.0f*frame_hy;
 frame_dx = 2.0f*frame_hx;

 // spacing between pixels
 pixel_dx = frame_dx/static_cast<float>(image_dx);
 pixel_dy = frame_dy/static_cast<float>(image_dy);

 // compute reference point for image origin
 nearref[0] = E[0] + nplane*D[0] + frame_hx*L[0] + frame_hy*U[0];
 nearref[1] = E[1] + nplane*D[1] + frame_hx*L[1] + frame_hy*U[1];
 nearref[2] = E[2] + nplane*D[2] + frame_hx*L[2] + frame_hy*U[2];

 // time testing
 if(timetest) {
    if(!QueryPerformanceFrequency((LARGE_INTEGER*)&hz)) return error("Performance counter not available.", __FILE__, __LINE__);
    QueryPerformanceCounter((LARGE_INTEGER*)&t0);
   }

 // for each pixel in the image
 int total_pixels = image_dx*image_dy;
 int curr_pixel = 0;
 for(uint32_t r = 0; r < image_dy; r++) {
 for(uint32_t c = 0; c < image_dx; c++)
    {
     // re generate samples
     RefreshJitterMatrix();

     // sample accumulator
     vector4D<float> sample_color;
     sample_color[0] = 0.0f;
     sample_color[1] = 0.0f;
     sample_color[2] = 0.0f;
     sample_color[3] = 0.0f;

     // for each sample
     for(const vector2D<float>& point : sampling_matrix)
        {
         // compute direction vector
         float scale_L = (c + point[0])*pixel_dx;
         float scale_U = (r + point[1])*pixel_dy;
         float screen_pnt[3] = {
          nearref[0] - (scale_L*L[0] + scale_U*U[0]),
          nearref[1] - (scale_L*L[1] + scale_U*U[1]),
          nearref[2] - (scale_L*L[2] + scale_U*U[2])
         };
         float screen_dir[3] = {
          screen_pnt[0] - E[0],
          screen_pnt[1] - E[1],
          screen_pnt[2] - E[2]
         };
         float tn = vector3D_normalize(screen_dir);
         float tf = tn*pratio;

         // initialize depth test
         // note that because ray origin is a point on the near plane, the
         // depth range is from [tn - tn, tf - tn] = [0.0, tf - tn]
         InitDepthTest(0.0f, (tf - tn));

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

         // place root node index on stack
         trace_node_stack[0] = 0;
         trace_node_elem = 1;

         // while there are node indices on the stack
         while(trace_node_elem)
              {
               // pop stack
               size_t node_index = trace_node_stack[trace_node_elem - 1];
               trace_node_elem--;
         
               // if node is contains faces
               const RTDynamicBVH::RTNode& node = bvh.tree[node_index];
               if(node.faces)
                 {
                  for(uint32_t i = 0; i < node.faces; i++)
                     {
                      // face must be visible
                      const XNAGlobalFace& face = bvh.model->facelist[node.face_index + i];
                      if(bvh.model->meshlist[face.mesh_index].params.optional[1] == false) continue;

                      // must have shader
                      auto shader = bvh.model->meshlist[face.mesh_index].params.shader;
                      if(!shader) continue;

                      // intersect test
                      ray3D_triangle3D_intersect_result result;
                      const XNAVertex& v0 = bvh.model->meshlist[face.mesh_index].verts[face.refs[0]];
                      const XNAVertex& v1 = bvh.model->meshlist[face.mesh_index].verts[face.refs[1]];
                      const XNAVertex& v2 = bvh.model->meshlist[face.mesh_index].verts[face.refs[2]];
                      ray3D_triangle3D_intersect(result, ray, v0.position, v1.position, v2.position, false);
                      if(!result.intersect) continue;

                      // shade face
                      XNAShaderData sd;
                      sd.model = bvh.model;
                      sd.face = &face;
                      sd.v0 = &v0;
                      sd.v1 = &v1;
                      sd.v2 = &v2;
                      sd.ray = &ray;
                      sd.t = result.t;
                      sd.u = result.u;
                      sd.v = result.v;
                      sd.w = 1.0f - result.u - result.v;
                      auto color = (debug_shader ? RGDB(&sd) : (*shader)(&sd));
                      UpdateDepthTest(color, result.t, face.mesh_index, bvh.model->meshlist[face.mesh_index].params.alpha);
                     }
                 }
               // put children on stack
               else {
                  uint32_t child1 = node.node_index;
                  uint32_t child2 = node.node_index + 1;
                  if(ray3D_AABB(ray, node.bbmin, node.bbmax, 0.0f, (tf - tn))) trace_node_stack[trace_node_elem++] = child1;
                  if(ray3D_AABB(ray, node.bbmin, node.bbmax, 0.0f, (tf - tn))) trace_node_stack[trace_node_elem++] = child2;
                 }
              }

         // accumulate color
         auto output = BlendPixels();
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
     if(!timetest) std::cout << "Percent done = " << (100.0f*(float)curr_pixel)/(float)total_pixels << "%" << std::endl;
    }}

 // time test
 if(timetest) {
    QueryPerformanceCounter((LARGE_INTEGER*)&t1);
    double elapsed = (double)(t1 - t0)/(double)hz;
    std::cout << "Image creation time: " << elapsed << " seconds." << std::endl;
   }

 // create image file
 SaveDibFile32(filename, image_dx, image_dy, image.get());
 return true;
}

void RefreshJitterMatrix(void)
{
 // must have at least a 2x2 square
 if(n_samples == 1) {
    sampling_matrix[0].x = 0.5f;
    sampling_matrix[0].y = 0.5f;
    return;
   }

 // random number generation
 std::random_device rd;
 std::mt19937 gen(rd());
 std::uniform_real_distribution<float> urd(0.0f, 1.0f);

 // generate canonical pattern
 float subcell_dim = 1.0f/static_cast<float>(n_samples);
 size_t index = 0;
 for(size_t r = 0; r < n_samples_xy; r++) {
     for(size_t c = 0; c < n_samples_xy; c++) {
         vector2D<float> point;
         point.x = (r*n_samples_xy + c + urd(gen))*subcell_dim;
         point.y = (c*n_samples_xy + r + urd(gen))*subcell_dim;
         sampling_matrix[index++] = point;
        }
    }

 // shuffle x and y
 for(size_t i = 0; i < n_samples_xy; i++) {
     for(size_t j = 0; j < n_samples_xy; j++) {
         std::uniform_int_distribution<size_t> uid(j, n_samples_xy - 1);
         size_t k = uid(gen);
         std::swap(sampling_matrix[i*n_samples_xy + j].x, sampling_matrix[i*n_samples_xy + k].x);
         k = uid(gen);
         std::swap(sampling_matrix[j*n_samples_xy + i].y, sampling_matrix[k*n_samples_xy + i].y);
        }
    }
}

void InitDepthTest(float tn, float tf)
{
 depth_elem = 0;
 depth_tmin = tn;
 depth_tmax = tf;
}

void UpdateDepthTest(const vector4D<float>& color, float dt, uint32_t mesh_index, uint8_t alpha)
{
 // CASE #1: INVISIBLE PIXEL
 if(color.a == 0) return;

 // CASE #2: FIRST PIXEL IS SIMPLY INSERTED
 if(!depth_elem) {
    depth_data[depth_elem].color = color;
    depth_data[depth_elem].t = dt;
    depth_data[depth_elem].mesh_index = mesh_index;
    depth_elem = 1;
    return;
   }

 constexpr float epsilon = 1.0e-6f;
 uint32_t j = depth_elem - 1;
 uint32_t k = depth_elem;

 // CASE #3: ANYTHING BEHIND LAST OPAQUE PIXEL IS HIDDEN
 if(depth_data[j].color.a == 1.0f && !(dt < depth_data[j].t))
    return;

 // CASE #4: HAVEN'T REACHED LIMIT
 if(depth_elem < MAXPIXELDEPTH)
   {
    // shift data until dt is greater than depth_data[j]
    for(size_t i = 0; i < depth_elem; i++) {
        if(dt < depth_data[j].t) {
           depth_data[k] = depth_data[j];
           j--;
           k--;
          }
        else
           break;
       }
    // insert new data
    depth_data[k].color = color;
    depth_data[k].t = dt;
    depth_data[k].mesh_index = mesh_index;
    // set number of colors
    if(color.a == 1.0f) depth_elem = k + 1;
    else depth_elem++;
   }
 // CASE #5: REACHED LIMIT
 else
   {
    // keep going if less
    if((dt < depth_data[j].t) || (dt == depth_data[j].t && color.a < 1.0f)) {
       j--;
       k--;
      }
    // otherwise quit
    else return;

    // stop shifting when new t-value is greater than depth_data[j]
    for(size_t i = 1; i < depth_elem; i++) {
        // break when certainly true
        // dt is considerably larger: (30 - 20) > epsilon = true
        if((dt - depth_data[j].t) > epsilon) break;
        // shift when certainly true
        // dt is considerably smaller: (30 - 20) > epsilon = true
        if((depth_data[j].t - dt) > epsilon) {
           depth_data[k] = depth_data[j];
           j--;
           k--;
          }
        // dt is really close, must resolve overlapping meshes, let alpha meshes go first
        else if(alpha) {
           depth_data[k] = depth_data[j];
           j--;
           k--;
          }
        else break;
       }
    depth_data[k].color = color;
    depth_data[k].t = dt;
    depth_data[k].mesh_index = mesh_index;
    if(color.a == 1.0f) depth_elem = k + 1;
   }
}

void UpdateDepthTest_TESTING(const vector4D<float>& color, float dt, uint32_t mesh_index, uint8_t alpha)
{
 // NOTES:
 // To solve the overlapping mesh problem, I currently shift or break during
 // the insertion sort until things are considerably over or under. Everything
 // else is considered close. For anything considered close, I put alpha meshes
 // over non-alpha meshes. Though a better way to do this is to also save the
 // mesh index and sort anything close by mesh index instead. This would simulate
 // the mesh rendering order in XNALara/XPS where you put overlapping alpha meshes 
 // later in the file.

 // CASE #1: INVISIBLE PIXEL
 if(color.a == 0) return;

 // CASE #2: FIRST PIXEL IS SIMPLY INSERTED
 if(!depth_elem) {
    depth_data[depth_elem].color = color;
    depth_data[depth_elem].t = dt;
    depth_data[depth_elem].mesh_index = mesh_index;
    depth_elem = 1;
    return;
   }

 constexpr float epsilon = 1.0e-6f;
 uint32_t j = depth_elem - 1;
 uint32_t k = depth_elem;

 // CASE #3: ANYTHING CONSIDERABLY BEHIND LAST OPAQUE PIXEL IS HIDDEN
 if(depth_data[j].color.a == 1.0f && ((dt - depth_data[j].t) > epsilon))
    return;

 // CASE #4: HAVEN'T REACHED LIMIT
 if(depth_elem < MAXPIXELDEPTH)
   {
    // stop shifting when new t-value is greater than depth_data[j]
    for(size_t i = 0; i < depth_elem; i++) {
        // break when certainly true
        // dt is considerably larger: (30 - 20) > epsilon = true
        if((dt - depth_data[j].t) > epsilon) break;
        // shift when certainly true
        // dt is considerably smaller: (30 - 20) > epsilon = true
        if((depth_data[j].t - dt) > epsilon) {
           depth_data[k] = depth_data[j];
           j--;
           k--;
          }
        // dt is really close, must resolve overlapping meshes, let alpha meshes go first
        // else if(alpha) {
        else if(depth_data[j].mesh_index < mesh_index) {
           depth_data[k] = depth_data[j];
           j--;
           k--;
          }
        else break;
       }
    // insert new data
    depth_data[k].color = color;
    depth_data[k].t = dt;
    depth_data[k].mesh_index = mesh_index;
    // set number of colors
    if(color.a == 1.0f) depth_elem = k + 1;
    else depth_elem++;
   }
 // CASE #5: REACHED LIMIT
 else
   {
    // keep going if less
    if((dt < depth_data[j].t) || (dt == depth_data[j].t && color.a < 1.0f)) {
       j--;
       k--;
      }
    // otherwise quit
    else return;

    // stop shifting when new t-value is greater than depth_data[j]
    for(size_t i = 1; i < depth_elem; i++) {
        // break when certainly true
        // dt is considerably larger: (30 - 20) > epsilon = true
        if((dt - depth_data[j].t) > epsilon) break;
        // shift when certainly true
        // dt is considerably smaller: (30 - 20) > epsilon = true
        if((depth_data[j].t - dt) > epsilon) {
           depth_data[k] = depth_data[j];
           j--;
           k--;
          }
        // dt is really close, must resolve overlapping meshes, let alpha meshes go first
        else if(alpha) {
           depth_data[k] = depth_data[j];
           j--;
           k--;
          }
        else break;
       }
    depth_data[k].color = color;
    depth_data[k].t = dt;
    depth_data[k].mesh_index = mesh_index;
    if(color.a == 1.0f) depth_elem = k + 1;
   }
}

vector4D<float> BlendPixels(void)
{
 // blend sorted pixels back-to-front
 vector4D<float> back = clearcolor;
 for(uint32_t i = 0; i < depth_elem; i++) {
     uint32_t j = depth_elem - i - 1;
     vector4D<float>& front = depth_data[j].color;
     float ONE_MINUS_ALPHA = 1.0f - front.a;
     back.r = front.a*depth_data[j].color.r + ONE_MINUS_ALPHA*back.r*back.a;
     back.g = front.a*depth_data[j].color.g + ONE_MINUS_ALPHA*back.g*back.a;
     back.b = front.a*depth_data[j].color.b + ONE_MINUS_ALPHA*back.b*back.a;
     back.a = front.a + ONE_MINUS_ALPHA*back.a;
    }
 return back;
}

#pragma endregion RAYTRACING

#pragma region HELPER_FUNCTIONS

inline vector4D<float> SampleTexture(const XNATexture& texture, float u, float v)
{
 return texture.data->sampler(texture.data, u, v);
}

template<uint32_t rows, uint32_t cols>
inline vector4D<float> SampleTexture(const XNAMesh& mesh, uint32_t index, const float (&uv)[rows][cols])
{
 // sample texture given mesh, texture index, and interpolated UVs
 const XNATexture& texture = mesh.textures[index];
 return texture.data->sampler(texture.data, uv[texture.channel][0], uv[texture.channel][1]);
}

template<uint32_t rows, uint32_t cols>
inline vector4D<float> SampleTexture(const XNAMesh& mesh, uint32_t index, const float (&uv)[rows][cols], float scale)
{
 // sample texture given mesh, texture index, and interpolated UVs
 const XNATexture& texture = mesh.textures[index];
 return texture.data->sampler(texture.data, scale*uv[texture.channel][0], scale*uv[texture.channel][1]);
}

template<uint32_t rows, uint32_t cols>
inline vector4D<float> SampleDiffuse(const XNAMesh& mesh, uint32_t index, const float (&uv)[rows][cols])
{
 if(enable_DM) return SampleTexture(mesh, index, uv);
 vector4D<float> sample;
 sample.r = sample.g = sample.b = 0.75f;
 sample.a = 1.0f;
 return sample;
}

template<uint32_t rows, uint32_t cols>
inline vector4D<float> SampleDiffuse(const XNAMesh& mesh, uint32_t index, const float (&color)[4], const float (&uv)[rows][cols])
{
 vector4D<float> sample;
 if(enable_DM) sample = SampleTexture(mesh, index, uv);
 else {
    sample.r = sample.g = sample.b = 0.75f;
    sample.a = 1.0f;
   }
 sample[0] *= color[0];
 sample[1] *= color[1];
 sample[2] *= color[2];
 sample[3] *= color[3]; // OK, always 1.0f
 return sample;
}

inline float Saturate(float value)
{
 if(value < 0.0f) return 0.0f;
 if(value > 1.0f) return 1.0f;
 return value;
}

#pragma endregion HELPER_FUNCTIONS

#pragma region SHADOWS

float ShadowTest(const RTDynamicBVH& bvh, const float* hp, const float* ld)
{
 // disabled shadows
 if(!enable_shadows) return 1.0f;

 // initialize shadow ray
 ray3D ray;
 ray.origin[0] = hp[0];
 ray.origin[1] = hp[1];
 ray.origin[2] = hp[2];
 ray.direction[0] = ld[0];
 ray.direction[1] = ld[1];
 ray.direction[2] = ld[2];
 ray.inverse[0] = (zerotest(ld[0]) ? std::numeric_limits<float>::infinity() : 1.0f/ld[0]);
 ray.inverse[1] = (zerotest(ld[1]) ? std::numeric_limits<float>::infinity() : 1.0f/ld[1]);
 ray.inverse[2] = (zerotest(ld[2]) ? std::numeric_limits<float>::infinity() : 1.0f/ld[2]);

 // initialize shadow multiplier
 float scale = 1.0f;

 // place root node index on stack
 shadow_node_stack[0] = 0;
 shadow_node_elem = 1;

 // while there are node indices on the stack
 while(shadow_node_elem)
      {
       // pop stack
       size_t node_index = shadow_node_stack[shadow_node_elem - 1];
       shadow_node_elem--;

       // if node is contains faces
       const RTDynamicBVH::RTNode& node = bvh.tree[node_index];
       if(node.faces)
         {
          for(uint32_t i = 0; i < node.faces; i++)
             {
              // face must be visible
              const XNAGlobalFace& face = bvh.model->facelist[node.face_index + i];
              if(bvh.model->meshlist[face.mesh_index].params.optional[1] == false) continue;

              // intersect test
              ray3D_triangle3D_intersect_result result;
              const XNAVertex& v1 = bvh.model->meshlist[face.mesh_index].verts[face.refs[0]];
              const XNAVertex& v2 = bvh.model->meshlist[face.mesh_index].verts[face.refs[1]];
              const XNAVertex& v3 = bvh.model->meshlist[face.mesh_index].verts[face.refs[2]];
              ray3D_triangle3D_intersect(result, ray, v1.position, v2.position, v3.position, false);
              if(!result.intersect) continue;

              // let this be greater than 0 so that it doesn't intersect the same triangle
              // or any other triangle overlapping it
              if(result.t < 1.0e-4f) continue; // technically needs to be not the same face
  
              // barycentric coordinates
              float u = result.u;
              float v = result.v;
              float w = 1.0f - u - v;

              // interpolated UV coordinates
              float uv[2] = {    
               u*v1.uv[0][0] + v*v2.uv[0][0] + w*v3.uv[0][0],
               u*v1.uv[0][1] + v*v2.uv[0][1] + w*v3.uv[0][1]
              };

              // diffuse alpha channel sample and compute percentage
              if(!debug_shader) {
                 auto DM_sample = SampleTexture(bvh.model->meshlist[face.mesh_index].textures[0], uv[0], uv[1]);
                 scale *= (1.0f - DM_sample.a);
                }
              else scale = 0.0f;
             }
         }
       // put children on stack
       else {
          uint32_t child1 = node.node_index;
          uint32_t child2 = node.node_index + 1;
          if(ray3D_AABB(ray, node.bbmin, node.bbmax, 0.0f, 1000.0f)) shadow_node_stack[shadow_node_elem++] = child1;
          if(ray3D_AABB(ray, node.bbmin, node.bbmax, 0.0f, 1000.0f)) shadow_node_stack[shadow_node_elem++] = child2;
         }
      }

 return scale;
}

#pragma endregion SHADOWS

#pragma region RENDERGROUP_SHADERS

inline void InterpolatePosition(const XNAShaderData* input, float* P)
{
 float u = input->u;
 float v = input->v;
 float w = input->w;
 P[0] = u*input->v0->position[0] + v*input->v1->position[0] + w*input->v2->position[0];
 P[1] = u*input->v0->position[1] + v*input->v1->position[1] + w*input->v2->position[1];
 P[2] = u*input->v0->position[2] + v*input->v1->position[2] + w*input->v2->position[2];
}

inline float InterpolateNormal(const XNAShaderData* input, float* N)
{
 float u = input->u;
 float v = input->v;
 float w = input->w;
 N[0] = u*input->v0->normal[0] + v*input->v1->normal[0] + w*input->v2->normal[0];
 N[1] = u*input->v0->normal[1] + v*input->v1->normal[1] + w*input->v2->normal[1];
 N[2] = u*input->v0->normal[2] + v*input->v1->normal[2] + w*input->v2->normal[2];
 return vector3D_normalize(N);
}

inline float InterpolateTangent(const XNAShaderData* input, uint32_t channel, float* N, float* T)
{
 float u = input->u;
 float v = input->v;
 float w = input->w;
 T[0] = u*input->v0->tangent[channel][0] + v*input->v1->tangent[channel][0] + w*input->v2->tangent[channel][0];
 T[1] = u*input->v0->tangent[channel][1] + v*input->v1->tangent[channel][1] + w*input->v2->tangent[channel][1];
 T[2] = u*input->v0->tangent[channel][2] + v*input->v1->tangent[channel][2] + w*input->v2->tangent[channel][2];
 T[3] = u*input->v0->tangent[channel][3] + v*input->v1->tangent[channel][3] + w*input->v2->tangent[channel][3];
 vector3D_orthogonalize(T, &N[0], &T[0]); // interpolation destroys orthogonality
 return vector3D_normalize(T);
}

// Interpolate UVs from a specific channel
inline void InterpolateUV(const XNAShaderData* input, uint32_t channel, float* uv)
{
 // interpolate UV coordinates
 // do not floor UVs here as texture sampler is responsible for texture wrapping
 uv[0] = input->u * input->v0->uv[channel][0] + input->v * input->v1->uv[channel][0] + input->w * input->v2->uv[channel][0];
 uv[1] = input->u * input->v0->uv[channel][1] + input->v * input->v1->uv[channel][1] + input->w * input->v2->uv[channel][1];
}

// Interpolate UVs on all channels
template<uint32_t rows, uint32_t cols>
inline void InterpolateUV(const XNAShaderData* input, float (&uv)[rows][cols])
{
 // interpolate UV coordinates
 // do not floor UVs here as texture sampler is responsible for texture wrapping
 for(uint32_t i = 0; i < input->model->meshlist[input->face->mesh_index].n_channels; i++) {
     uv[i][0] = input->u*input->v0->uv[i][0] + input->v*input->v1->uv[i][0] + input->w*input->v2->uv[i][0];
     uv[i][1] = input->u*input->v0->uv[i][1] + input->v*input->v1->uv[i][1] + input->w*input->v2->uv[i][1];
    }
}

inline void InterpolateColor4D(const XNAShaderData* input, float* color)
{
 color[0] = input->u*input->v0->color[0] + input->v*input->v1->color[0] + input->w*input->v2->color[0];
 color[1] = input->u*input->v0->color[1] + input->v*input->v1->color[1] + input->w*input->v2->color[1];
 color[2] = input->u*input->v0->color[2] + input->v*input->v1->color[2] + input->w*input->v2->color[2];
 color[3] = input->u*input->v0->color[3] + input->v*input->v1->color[3] + input->w*input->v2->color[3];
}

template<uint32_t rows, uint32_t cols>
void NormalMapping1(const XNAShaderData* input, uint32_t BM_index, const float (&uv)[rows][cols], float (&N)[3])
{
 // normal mapping disabled
 if(!enable_BM) return;

 // face UVs and edges
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 uint32_t channel = input->model->meshlist[input->face->mesh_index].textures[BM_index].channel;
 vector2D<float> uv0(input->v0->uv[channel]);
 vector2D<float> uv1(input->v1->uv[channel]);
 vector2D<float> uv2(input->v2->uv[channel]);
 vector2D<float> tea = uv1 - uv0;
 vector2D<float> teb = uv2 - uv0;

 // you can't bump map degenerate UVs
 float scale = (tea[0]*teb[1] - teb[0]*tea[1]);
 if(std::abs(scale) < 1.0e-6f) return;

 // interpolated tangent
 float T[4];
 float T_norm = InterpolateTangent(input, channel, N, T);

 // N = T cross B (Z = X x Y)
 // T = B cross N (X = Y x Z)
 // B = N cross T (Y = Z x X)
 float B[3];
 vector3D_vector_product(B, N, T);
 B[0] *= T[3]; // for mirrored UVs
 B[1] *= T[3]; // for mirrored UVs
 B[2] *= T[3]; // for mirrored UVs
 vector3D_normalize(B);

 // remap sample to <[-1, +1],[-1, +1],[0, +1]>
 auto NM_sample = SampleTexture(mesh, BM_index, uv);
 float N_unperturbed[3] = {
  NM_sample[0]*2.0f - 1.0f, // r -> [0.0,1.0] so this maps to [-1.0,+1.0]
  NM_sample[1]*2.0f - 1.0f, // g -> [0.0,1.0] so this maps to [-1.0,+1.0]
  NM_sample[2]              // b -> [0.0,1.0] so unchanged as we cannot have negative normal (means invisible)
 };
 vector3D_normalize(N_unperturbed);

 // rotate normal
 // T[0] B[0] N[0] * N[0]
 // T[1] B[1] B[1] * N[1]
 // T[2] B[2] N[2] * N[2e]
 N[0] = N_unperturbed[0]*T[0] + N_unperturbed[1]*B[0] + N_unperturbed[2]*N[0];
 N[1] = N_unperturbed[0]*T[1] + N_unperturbed[1]*B[1] + N_unperturbed[2]*N[1];
 N[2] = N_unperturbed[0]*T[2] + N_unperturbed[1]*B[2] + N_unperturbed[2]*N[2];
 vector3D_normalize(N);
}

template<uint32_t rows, uint32_t cols>
void NormalMapping3(const XNAShaderData* input, uint32_t BM_index, uint32_t MM_index, uint32_t R1_index, float s1, uint32_t R2_index, float s2, const float (&uv)[rows][cols], float (&N)[3])
{
 // normal mapping disabled
 if(!enable_BM) return;

 // face UVs and edges
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 uint32_t channel = input->model->meshlist[input->face->mesh_index].textures[BM_index].channel;
 vector2D<float> uv0(input->v0->uv[channel]);
 vector2D<float> uv1(input->v1->uv[channel]);
 vector2D<float> uv2(input->v2->uv[channel]);
 vector2D<float> tea = uv1 - uv0;
 vector2D<float> teb = uv2 - uv0;

 // you can't bump map degenerate UVs
 float scale = (tea[0]*teb[1] - teb[0]*tea[1]);
 if(std::abs(scale) < 1.0e-6f) return;

 // interpolated tangent
 float T[4];
 float T_norm = InterpolateTangent(input, channel, N, T);

 // N = T cross B (Z = X x Y)
 // T = B cross N (X = Y x Z)
 // B = N cross T (Y = Z x X)
 float B[3];
 vector3D_vector_product(B, N, T);
 B[0] *= T[3]; // for mirrored UVs
 B[1] *= T[3]; // for mirrored UVs
 B[2] *= T[3]; // for mirrored UVs
 vector3D_normalize(B);

 // sample textures
 auto NM_sample = SampleTexture(mesh, BM_index, uv);
 auto MM_sample = SampleTexture(mesh, MM_index, uv);
 auto R1_sample = SampleTexture(mesh, R1_index, uv, s1);
 auto R2_sample = SampleTexture(mesh, R2_index, uv, s2);

 // combine bumpmap colors
 // note that NM, R1, and R2 can use different UV layers, but the tangent space
 // normal mapping uses the UV layer specified by the NM texture.
 float final_bump[3];
 final_bump[0] = NM_sample.r + (R1_sample.r - 0.5f)*MM_sample.r + (R2_sample.r - 0.5f)*MM_sample.g;
 final_bump[1] = NM_sample.g + (R1_sample.g - 0.5f)*MM_sample.r + (R2_sample.g - 0.5f)*MM_sample.g;
 final_bump[2] = NM_sample.b + (R1_sample.b - 0.5f)*MM_sample.r + (R2_sample.b - 0.5f)*MM_sample.g;

 // remap sample to <[-1, +1],[-1, +1],[0, +1]>
 float N_unperturbed[3] = {
  final_bump[0]*2.0f - 1.0f, // r -> [0.0,1.0] so this maps to [-1.0,+1.0]
  final_bump[1]*2.0f - 1.0f, // g -> [0.0,1.0] so this maps to [-1.0,+1.0]
  final_bump[2]              // b -> [0.0,1.0] so unchanged as we cannot have negative normal (means invisible)
 };
 vector3D_normalize(N_unperturbed);

 // rotate normal
 // T[0] B[0] N[0] * N[0]
 // T[1] B[1] B[1] * N[1]
 // T[2] B[2] N[2] * N[2]
 N[0] = N_unperturbed[0]*T[0] + N_unperturbed[1]*B[0] + N_unperturbed[2]*N[0];
 N[1] = N_unperturbed[0]*T[1] + N_unperturbed[1]*B[1] + N_unperturbed[2]*N[1];
 N[2] = N_unperturbed[0]*T[2] + N_unperturbed[1]*B[2] + N_unperturbed[2]*N[2];
 vector3D_normalize(N);
}

vector4D<float> RGDB(const XNAShaderData* input)
{
 // initialize sample to flat color
 vector4D<float> sample;
 sample[0] = 0.75f;
 sample[1] = 0.75f;
 sample[2] = 0.75f;
 sample[3] = 1.00f;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // face normal
 float A[3], B[3];
 vector3D_sub(A, v1->position, v0->position);
 vector3D_sub(B, v2->position, v0->position);
 vector3D_vector_product(N, B, A);
 N_norm = vector3D_normalize(N);

 // // interpolated UV coordinates
 // float uv[2][2];
 // InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 // shading
 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f);
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color component
              float factor = scale*dot1;
              float scaleC = kd*factor*dlight.intensity;
              float scaleS = ks*factor*specular_intensity;
              float ccd[3] = { dlight.color[0]*scaleC, dlight.color[1]*scaleC, dlight.color[2]*scaleC };
              float ccs[3] = { dlight.color[0]*scaleS, dlight.color[1]*scaleS, dlight.color[2]*scaleS };
           
              // sum components
              L[0] += (sample[0]*ccd[0] + specular[0]*ccs[0]);
              L[1] += (sample[1]*ccd[1] + specular[1]*ccs[1]);
              L[2] += (sample[2]*ccd[2] + specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    sample[0] = Saturate((ka*sample.r*ambient_co[0]) + L[0]);
    sample[1] = Saturate((ka*sample.g*ambient_co[1]) + L[1]);
    sample[2] = Saturate((ka*sample.b*ambient_co[2]) + L[2]);
   }

 return sample;
}

vector4D<float> RG00(const XNAShaderData* input)
{
 // if there is a texture, sample the first one
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 if(mesh.n_textures) {
    uint32_t channel = mesh.textures[0].channel;
    float uv[2];
    InterpolateUV(input, channel, uv);
    const XNATexture& texture = mesh.textures[0];
    return texture.data->sampler(texture.data, uv[0], uv[1]);
   }

 // otherwise
 vector4D<float> sample;
 sample[0] = 0.75f;
 sample[1] = 0.75f;
 sample[2] = 0.75f;
 sample[3] = 1.00f;
 return sample;
}

vector4D<float> RG01(const XNAShaderData* input)
{
 // same as RG20 but without alpha
 vector4D<float> rv = RG20(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG02(const XNAShaderData* input)
{
 // same as RG08 but without alpha
 vector4D<float> rv = RG08(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG03(const XNAShaderData* input)
{
 // same as RG09 but without alpha
 vector4D<float> rv = RG09(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG04(const XNAShaderData* input)
{
 // same as RG06 but without alpha
 vector4D<float> rv = RG06(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG05(const XNAShaderData* input)
{
 // same as RG07 but without alpha
 vector4D<float> rv = RG07(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG06(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f);
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG07(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // for each light
    float kd = (k_conservation ? 1.0f - ka : 1.0f);
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // sum components
              L[0] += scale*(dlight.color[0]*dot1*dlight.intensity);
              L[1] += scale*(dlight.color[1]*dot1*dlight.intensity);
              L[2] += scale*(dlight.color[2]*dot1*dlight.intensity);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate(DM_sample.r*(ka*ambient_co[0] + kd*L[0]));
    DM_sample.g = Saturate(DM_sample.g*(ka*ambient_co[1] + kd*L[1]));
    DM_sample.b = Saturate(DM_sample.b*(ka*ambient_co[2] + kd*L[2]));
   }

 // finished
 return DM_sample;
}

vector4D<float> RG08(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int LM_index = 1;
 constexpr int BM_index = 2;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f);
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
    if(enable_LM) {
       auto LM_sample = SampleTexture(mesh, LM_index, uv);
       DM_sample.r *= LM_sample.r;
       DM_sample.g *= LM_sample.g;
       DM_sample.b *= LM_sample.b;
      }
   }

 // finished
 return DM_sample;
}

vector4D<float> RG09(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int LM_index = 1;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors (no specular = no ks)
    float kd = (k_conservation ? 1.0f - ka : 1.0f);

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
    if(enable_LM) {
       auto LM_sample = SampleTexture(mesh, LM_index, uv);
       DM_sample.r *= LM_sample.r;
       DM_sample.g *= LM_sample.g;
       DM_sample.b *= LM_sample.b;
      }
   }

 // finished
 return DM_sample;
}

vector4D<float> RG10(const XNAShaderData* input)
{
 // same as RG21 but without alpha
 vector4D<float> rv = RG21(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG11(const XNAShaderData* input)
{
 // same as RG12 but without alpha
 vector4D<float> rv = RG12(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG12(const XNAShaderData* input)
{
 // unsure about this one, assume it is the
 // same as RG06 but without posing
 return RG06(input);
}

vector4D<float> RG13(const XNAShaderData* input)
{
 // same as RG21 but without alpha and posing
 vector4D<float> rv = RG21(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG14(const XNAShaderData* input)
{
 // same as RG15 but without alpha
 vector4D<float> rv = RG15(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG15(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 return DM_sample;
}

vector4D<float> RG16(const XNAShaderData* input)
{
 // same as RG18 but without alpha
 vector4D<float> rv = RG18(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG17(const XNAShaderData* input)
{
 // same as RG19 but without alpha
 vector4D<float> rv = RG19(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG18(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, uv);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // for each light
    float kd = (k_conservation ? 1.0f - ka : 1.0f);
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // interpolate between [1.0f, (N dot -L)]
        float shading = Saturate(vector3D_scalar_product(N, &wi[0]));
        shading = 1.0f*(1.0f - dlight.shadow_depth) + shading*dlight.shadow_depth;

        // if light intensity is not zero
        if(shading > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale) {
              L[0] += scale*(kd*DM_sample[0]*dlight.color[0]*shading)*dlight.intensity;
              L[1] += scale*(kd*DM_sample[1]*dlight.color[1]*shading)*dlight.intensity;
              L[2] += scale*(kd*DM_sample[2]*dlight.color[2]*shading)*dlight.intensity;
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG19(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int LM_index = 1;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, uv);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka : 1.0f);

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // interpolate between [1.0f, (N dot -L)]
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        dot1 = 1.0f - dlight.shadow_depth*(1.0f - dot1); // lerp (shadow depth = 0 = no shading. shadow depth = 1 = normal shading)

        // if light intensity is not zero
        if(dot1 > 0.0f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           // sum components
           if(scale) {
              L[0] += scale*(dlight.color[0]*dot1*dlight.intensity);
              L[1] += scale*(dlight.color[1]*dot1*dlight.intensity);
              L[2] += scale*(dlight.color[2]*dot1*dlight.intensity);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r *= (ka*ambient_co[0] + kd*L[0]);
    DM_sample.g *= (ka*ambient_co[1] + kd*L[1]);
    DM_sample.b *= (ka*ambient_co[2] + kd*L[2]);
   }

 // multiply by lightmap data
 if(enable_LM) {
    vector4D<float> LM_sample = SampleTexture(mesh, LM_index, uv);
    DM_sample.r *= LM_sample.r;
    DM_sample.g *= LM_sample.g;
    DM_sample.b *= LM_sample.b;
    DM_sample.a *= LM_sample.a;
   }

 // finished
 return DM_sample;
}

vector4D<float> RG20(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int LM_index = 1;
 constexpr int BM_index = 2;
 constexpr int MM_index = 3;
 constexpr int R1_index = 4;
 constexpr int R2_index = 5;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping3(input, BM_index, MM_index, R1_index, mesh.params.params[1], R2_index, mesh.params.params[2], uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1) // > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              float hitpoint[3];
              InterpolatePosition(input, &hitpoint[0]);
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
    if(enable_LM) {
       auto LM_sample = SampleTexture(mesh, LM_index, uv);
       DM_sample.r *= LM_sample.r;
       DM_sample.g *= LM_sample.g;
       DM_sample.b *= LM_sample.b;
      }
   }

 // finished
 return DM_sample;
}

vector4D<float> RG21(const XNAShaderData* input)
{
 // SHADLESS RENDER GROUPS
 // texture indices
 constexpr int DM_index = 0;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 return SampleDiffuse(mesh, DM_index, uv);
}

vector4D<float> RG22(const XNAShaderData* input)
{
 // same as RG23 but without alpha
 vector4D<float> rv = RG23(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG23(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int LM_index = 1;
 constexpr int BM_index = 2;
 constexpr int MM_index = 3;
 constexpr int R1_index = 4;
 constexpr int R2_index = 5;
 constexpr int SM_index = 6;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping3(input, BM_index, MM_index, R1_index, mesh.params.params[1], R2_index, mesh.params.params[2], uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              auto SM_sample = SampleTexture(mesh, SM_index, uv);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.x);
              specular[1] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.y);
              specular[2] = Saturate(std::pow(reflection_angle, specular_power)*SM_sample.z);
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
    if(enable_LM) {
       auto LM_sample = SampleTexture(mesh, LM_index, uv);
       DM_sample.r *= LM_sample.r;
       DM_sample.g *= LM_sample.g;
       DM_sample.b *= LM_sample.b;
      }
   }

 // finished
 return DM_sample;
}

vector4D<float> RG24(const XNAShaderData* input)
{
 // same as RG25 but without alpha
 vector4D<float> rv = RG25(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG25(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int LM_index = 1;
 constexpr int BM_index = 2;
 constexpr int SM_index = 3;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              float factor = Saturate(std::pow(reflection_angle, specular_power));

              // specular map
              auto SM_sample = SampleTexture(mesh, SM_index, uv);
              specular.r = factor*SM_sample.r;
              specular.g = factor*SM_sample.g;
              specular.b = factor*SM_sample.b;
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
    if(enable_LM) {
       auto LM_sample = SampleTexture(mesh, LM_index, uv);
       DM_sample.r *= LM_sample.r;
       DM_sample.g *= LM_sample.g;
       DM_sample.b *= LM_sample.b;
      }
   }

 // finished
 return DM_sample;
}

vector4D<float> RG26(const XNAShaderData* input)
{
 // same as RG27 but without alpha
 vector4D<float> rv = RG27(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG27(const XNAShaderData* input)
{
 // NOTE: In the original XNALara "MetallicPS" shader, they do lighting first and
 // then multiply the diffuse color by the resulting phong shading color second.
 // Normal and reflection mapping come in as the 3rd and 4th steps, respectively.
 // For now, I am going to do this in normal order... diffuse, normal, shading,
 // and then reflection.

 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;
 constexpr int EM_index = 2;
 constexpr int MM_index = 3;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = (enable_EM ? 1.0f : mesh.params.params[0]);
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;

        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 //
 // ENVIRONMENT MAPPING
 //

 if(enable_EM)
   {
    // compute sphere map UV
    float scale = vector3D_scalar_product(input->ray->direction, N);
    float reflection[3] = {
     input->ray->direction[0] - 2.0f*scale*N[0],
     input->ray->direction[1] - 2.0f*scale*N[1],
     input->ray->direction[2] - 2.0f*scale*N[2]
    };
    vector3D_normalize(reflection);
    float denom = sqrt(vector3D_squared_norm(reflection) + 2.0f*reflection[2] + 1.0f);
    float tu = 0.5f*reflection[0]/denom + 0.5f;
    float tv = 0.5f*reflection[1]/denom - 0.5f;

    // sample texture using computed sphere map UV
    auto EM_sample = SampleTexture(mesh.textures[EM_index], tu, tv);
    auto MM_sample = SampleTexture(mesh, MM_index, uv);

    // interpolate
    float kr = mesh.params.params[0]; // 0.0 = no reflection, 1.0 = full reflection
    DM_sample.r = DM_sample.r + kr*(EM_sample.r - DM_sample.r)*MM_sample.r;
    DM_sample.g = DM_sample.g + kr*(EM_sample.g - DM_sample.g)*MM_sample.g;
    DM_sample.b = DM_sample.b + kr*(EM_sample.b - DM_sample.b)*MM_sample.b;
   }

 // finished
 return DM_sample;
}

vector4D<float> RG28(const XNAShaderData* input)
{
 // same as RG29 but without alpha
 vector4D<float> rv = RG29(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG29(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;
 constexpr int MM_index = 2;
 constexpr int R1_index = 3;
 constexpr int R2_index = 4;
 constexpr int EM_index = 5;

 // RG29
 //	6 textures: diffuse, bumpmap, maskmap, repeater, repeater, environment map
 //	Alpha: Y
 // Posing: Y
 // Shading: Y
 // Parameters:	ReflectionAmount, BumpSpecularAmount, BumpUVScale
 // Notes: Same as RG28 but with alpha
 // XNALaraShader: ?

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping3(input, BM_index, MM_index, R1_index, mesh.params.params[2], R2_index, mesh.params.params[2], uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[1];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;

        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              float specular = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 //
 // ENVIRONMENT MAPPING
 //

 if(enable_EM)
   {
    // compute sphere map UV and sample texture
    float scale = vector3D_scalar_product(input->ray->direction, N);
    float reflection[3] = {
     input->ray->direction[0] - 2.0f*scale*N[0],
     input->ray->direction[1] - 2.0f*scale*N[1],
     input->ray->direction[2] - 2.0f*scale*N[2]
    };
    vector3D_normalize(reflection);
    float denom = sqrt(vector3D_squared_norm(reflection) + 2.0f*reflection[2] + 1.0f);
    float tu = 0.5f*reflection[0]/denom + 0.5f;
    float tv = 0.5f*reflection[1]/denom - 0.5f;
    auto EM_sample = SampleTexture(mesh.textures[EM_index], tu, tv);

    // interpolate
    float kr = mesh.params.params[0]; // 0.0 = no reflection, 1.0 = full reflection
    DM_sample.r = DM_sample.r + kr*(EM_sample.r - DM_sample.r);
    DM_sample.g = DM_sample.g + kr*(EM_sample.g - DM_sample.g);
    DM_sample.b = DM_sample.b + kr*(EM_sample.b - DM_sample.b);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG30(const XNAShaderData* input)
{
 // same as RG31 but without alpha
 vector4D<float> rv = RG31(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG31(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;
 constexpr int GM_index = 2;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f);
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 //
 // EMISSIVE MAPS
 //

 // extremely simple shader
 if(enable_GM) {
    auto GM_sample = SampleTexture(mesh, GM_index, uv);
    DM_sample.r = Saturate(DM_sample.r + GM_sample.r);
    DM_sample.g = Saturate(DM_sample.g + GM_sample.g);
    DM_sample.b = Saturate(DM_sample.b + GM_sample.b);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG32(const XNAShaderData* input)
{
 // same as RG33 but without alpha
 vector4D<float> rv = RG33(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG33(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f);
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = 1.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG34(const XNAShaderData* input)
{
 // same as RG35 but without alpha
 vector4D<float> rv = RG35(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG35(const XNAShaderData* input)
{
 return RG00(input);
}

vector4D<float> RG36(const XNAShaderData* input)
{
 // same as RG37 but without alpha
 vector4D<float> rv = RG37(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG37(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;
 constexpr int GM_index = 2;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f);
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        const DirectionalLight& dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              specular[0] = Saturate(std::pow(reflection_angle, specular_power));
              specular[1] = Saturate(std::pow(reflection_angle, specular_power));
              specular[2] = Saturate(std::pow(reflection_angle, specular_power));
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 //
 // EMISSIVE MAPS
 //

 // extremely simple shader
 if(enable_GM) {
    auto GM_sample = SampleTexture(mesh, GM_index, uv, mesh.params.params[1]);
    DM_sample.r = Saturate(DM_sample.r + GM_sample.r);
    DM_sample.g = Saturate(DM_sample.g + GM_sample.g);
    DM_sample.b = Saturate(DM_sample.b + GM_sample.b);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG38(const XNAShaderData* input)
{
 // same as RG39 but without alpha
 vector4D<float> rv = RG39(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG39(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;
 constexpr int SM_index = 2;
 constexpr int GM_index = 3;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              float factor = Saturate(std::pow(reflection_angle, specular_power));

              // specular map
              auto SM_sample = SampleTexture(mesh, SM_index, uv);
              specular.r = factor*SM_sample.r;
              specular.g = factor*SM_sample.g;
              specular.b = factor*SM_sample.b;
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 //
 // EMISSIVE MAPS
 //

 // extremely simple shader
 if(enable_GM) {
    auto GM_sample = SampleTexture(mesh, GM_index, uv);
    DM_sample.r = Saturate(DM_sample.r + GM_sample.r);
    DM_sample.g = Saturate(DM_sample.g + GM_sample.g);
    DM_sample.b = Saturate(DM_sample.b + GM_sample.b);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG40(const XNAShaderData* input)
{
 // same as RG41 but without alpha
 vector4D<float> rv = RG41(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG41(const XNAShaderData* input)
{
 // NOTE: For RG41, the normal map format is different.

 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;
 constexpr int SM_index = 2;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              float factor = Saturate(std::pow(reflection_angle, specular_power));

              // specular map
              auto SM_sample = SampleTexture(mesh, SM_index, uv);
              specular.r = factor*SM_sample.r;
              specular.g = factor*SM_sample.g;
              specular.b = factor*SM_sample.b;
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG42(const XNAShaderData* input)
{
 // same as RG43 but without alpha
 vector4D<float> rv = RG43(input);
 rv.a = 1.0f;
 return rv;
}

vector4D<float> RG43(const XNAShaderData* input)
{
 // texture indices
 constexpr int DM_index = 0;
 constexpr int BM_index = 1;
 constexpr int SM_index = 2;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated normal
 float N[3];
 float N_norm = InterpolateNormal(input, N);

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 // interpolated color
 float color[4];
 InterpolateColor4D(input, color);

 //
 // DIFFUSE MAPPING
 //

 // diffuse sample
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, color, uv);

 //
 // NORMAL MAPPING
 //

 NormalMapping1(input, BM_index, uv, N);

 //
 // LIGHTING
 //

 if(enable_shading)
   {
    // wo is "from hit-point to camera"
    vector3D<float> wo;
    wo[0] = -input->ray->direction[0];
    wo[1] = -input->ray->direction[1];
    wo[2] = -input->ray->direction[2];

    vector3D<float> L;
    L[0] = 0.0f;
    L[1] = 0.0f;
    L[2] = 0.0f;

    // factors
    float kd = (k_conservation ? 1.0f - ka - ks : 1.0f); // energy conserving
    float specular_power = 10.0f; // XNALara uses 10.0f for BumpSpecularGloss
    float specular_intensity = mesh.params.params[0];
    if(!enable_SM) specular_intensity = 0.0f;

    // for each light
    for(uint32_t i = 0; i < 3; i++)
       {
        // light disabled
        auto dlight = lightlist[i];
        if(!dlight.enabled) continue;
    
        // wi is normalized vector "from hit point to light"
        vector3D<float> wi;
        wi[0] = -dlight.direction[0];
        wi[1] = -dlight.direction[1];
        wi[2] = -dlight.direction[2];
    
        // dot products
        float dot1 = vector3D_scalar_product(N, &wi[0]); // in XNALara, this is PhongShadingFactor
        float dot2 = 2.0f*dot1;
    
        // if light intensity is not zero
        if(dot1 > 1.0e-6f)
          {
           // shadow test
           float scale = 1.0f;
           if(enable_shadows) {
              // interpolated hitpoint
              float hitpoint[3] = {
               input->u*v0->position[0] + input->v*v1->position[0] + input->w*v2->position[0],
               input->u*v0->position[1] + input->v*v1->position[1] + input->w*v2->position[1],
               input->u*v0->position[2] + input->v*v1->position[2] + input->w*v2->position[2]
              };
              scale = ShadowTest(bvh, &hitpoint[0], &wi[0]);
             }
           if(scale)
             {
              // wr is normalized reflection vector
              vector3D<float> wr;
              wr[0] = -wi[0] + dot2*N[0];
              wr[1] = -wi[1] + dot2*N[1];
              wr[2] = -wi[2] + dot2*N[2];
              vector3D_normalize(&wr[0]);
           
              // compute specular component
              vector3D<float> specular;
              float reflection_angle = vector3D_scalar_product(&wo[0], &wr[0]);
              float factor = Saturate(std::pow(reflection_angle, specular_power));

              // specular map
              auto SM_sample = SampleTexture(mesh, SM_index, uv, mesh.params.params[1]);
              specular.r = factor*SM_sample.r;
              specular.g = factor*SM_sample.g;
              specular.b = factor*SM_sample.b;
           
              // color components
              float ccd[3] = { // diffuse
               dlight.color[0]*dot1*dlight.intensity,
               dlight.color[1]*dot1*dlight.intensity,
               dlight.color[2]*dot1*dlight.intensity
              };
              float ccs[3] = { // specular
               dlight.color[0]*dot1*specular_intensity,
               dlight.color[1]*dot1*specular_intensity,
               dlight.color[2]*dot1*specular_intensity
              };
           
              // sum components (page 282, equations 15.7 and 15.8 in book)
              L[0] += scale*(kd*DM_sample.x*ccd[0] + ks*specular[0]*ccs[0]);
              L[1] += scale*(kd*DM_sample.y*ccd[1] + ks*specular[1]*ccs[1]);
              L[2] += scale*(kd*DM_sample.z*ccd[2] + ks*specular[2]*ccs[2]);
             }
          }
       }

    // ambient + all light contributions
    DM_sample.r = Saturate((ka*DM_sample.r*ambient_co[0]) + L[0]);
    DM_sample.g = Saturate((ka*DM_sample.g*ambient_co[1]) + L[1]);
    DM_sample.b = Saturate((ka*DM_sample.b*ambient_co[2]) + L[2]);
   }

 // finished
 return DM_sample;
}

vector4D<float> RG_thorgear(const XNAShaderData* input)
{
 // same as RG02
 return RG02(input);
}

vector4D<float> RG_thorglow(const XNAShaderData* input)
{
 // NOTE: It's a little more complicated than this... looks like two passes
 // followed by a billboard pass.
 // This is pass #1:
 //     graphicsDevice.RenderState.SourceBlend = Blend.BlendFactor;
 //     graphicsDevice.RenderState.BlendFactor = new Color(2.0f, 2.0f, 2.0f, 1.0f);
 //     graphicsDevice.RenderState.DestinationBlend = Blend.InverseSourceAlpha;
 //     effectsArmature.CurrentTechnique = effectsArmature.Techniques["ThorGlow1"];
 //     foreach (Mesh mesh in model.GetMeshGroup(MeshGroupNames.ThorGlowAll)) {
 //         effectsArmature.Parameters["BoneMatrices"].SetValue(armature.GetBoneMatrices(mesh));
 //         mesh.RenderSinglePass(effectsArmature, "DiffuseTexture");
 //     }
 // This is pass #2:
 //     graphicsDevice.RenderState.SourceBlend = Blend.SourceAlpha;
 //     graphicsDevice.RenderState.DestinationBlend = Blend.One;
 //     effectsArmature.CurrentTechnique = effectsArmature.Techniques["ThorGlow2"];
 //     foreach (Mesh mesh in model.GetMeshGroup(MeshGroupNames.ThorGlowAll)) {
 //         effectsArmature.Parameters["BoneMatrices"].SetValue(armature.GetBoneMatrices(mesh));
 //         mesh.RenderSinglePass(effectsArmature, "DiffuseTexture");
 //     }
 // And this is the billboard pass:
 //     graphicsDevice.RenderState.SourceBlend = Blend.SourceColor;
 //     graphicsDevice.RenderState.DestinationBlend = Blend.One;
 //     RenderGauntletGlowBillboards(item, isThorGlowGauntletLeftVisible, isThorGlowGauntletRightVisible, false);

 // NOTE: Notice that first function gets full color and multiplies it by a strong blue color.
 // Then notice the second function sets the blue color but keeps the alpha channel.
 // float4 ThorGlow1PS(BasicVertexShaderOutput input) : COLOR0
 // {
 // 	return GetDiffuseColor(input.TexCoord) * float4(0.25, 0.5, 1.0, 1.0);
 // }
 // float4 ThorGlow2PS(BasicVertexShaderOutput input) : COLOR0
 // {
 // 	float4 color = GetDiffuseColor(input.TexCoord);
 // 	color.r = 0.25;
 // 	color.g = 0.5;
 // 	color.b = 1.0;
 // 	return color;
 // }

 // texture indices
 constexpr int DM_index = 0;

 // mesh and triangle vertices
 const XNAMesh& mesh = input->model->meshlist[input->face->mesh_index];
 const XNAVertex* v0 = input->v0;
 const XNAVertex* v1 = input->v1;
 const XNAVertex* v2 = input->v2;

 // interpolated UV coordinates
 float uv[2][2];
 InterpolateUV(input, uv);

 //
 // DIFFUSE MAPPING
 //

 // multiply diffuse by emissive blue color
 vector4D<float> DM_sample = SampleDiffuse(mesh, DM_index, uv);
 DM_sample.r = Saturate(DM_sample.r*0.25f);
 DM_sample.g = Saturate(DM_sample.g*0.50f);
 DM_sample.b = Saturate(DM_sample.b*0.75f);
 return DM_sample;
}

vector4D<float> RG_handguns(const XNAShaderData* input)
{
 // same as RG06
 return RG06(input);
}

#pragma endregion RENDERGROUP_SHADERS