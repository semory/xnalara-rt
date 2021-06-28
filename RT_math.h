#ifndef __RT_MATH_H
#define __RT_MATH_H

#define PI_OVER_180      0.01745329252f
#define PI_OVER_180_INV 57.29577951308f

//
// GENERAL FUNCTIONS
//

inline float inv(float x)
{
 return 1.0f/x;
}

//
// TRIGNOMETRY FUNCTIONS
//

inline float radians(float d) { return d*PI_OVER_180; }
inline float degrees(float r) { return r*PI_OVER_180_INV; }

//
// BINARY TRAITS
//

inline float positive_infinity(void)
{
 // 0-11111111-00000000000000000000000
 unsigned int temp = 0x7F800000u;
 return *reinterpret_cast<float*>(&temp);
}

inline float negative_infinity(void)
{
 // 1-11111111-00000000000000000000000
 unsigned int temp = 0xFF800000u;
 return *reinterpret_cast<float*>(&temp);
}

//
// TESTING VALUES
//

inline bool zerotest(float x)
{
 return std::abs(x) < 1.0e-6f;
}

//
// 2D VECTORS
//

template<class T>
struct vector2D {
 T v[2];
 vector2D() {}
 vector2D(const vector2D<T>& obj) {
  v[0] = obj.v[0];
  v[1] = obj.v[1];
 }
 explicit vector2D(const T* ptr) {
  v[0] = ptr[0];
  v[1] = ptr[1];
 }
 T& operator [](size_t index) { return v[index]; }
 const T& operator [](size_t index)const { return v[index]; }
 vector2D<T>& operator +=(const vector2D<T>& other) {
  v[0] += other.v[0];
  v[1] += other.v[1];
  return *this;
 }
 vector2D<T>& operator -=(const vector2D<T>& other) {
  v[0] -= other.v[0];
  v[1] -= other.v[1];
  return *this;
 }
};

template<class T>
inline vector2D<T> operator +(const vector2D<T>& v1, const vector2D<T>& v2) {
 vector2D<T> temp;
 temp.v[0] = v1.v[0] + v2.v[0];
 temp.v[1] = v1.v[1] + v2.v[1];
 return temp;
}

template<class T>
inline vector2D<T> operator -(const vector2D<T>& v1, const vector2D<T>& v2) {
 vector2D<T> temp;
 temp.v[0] = v1.v[0] - v2.v[0];
 temp.v[1] = v1.v[1] - v2.v[1];
 return temp;
}

//
// 3D VECTORS
//

template<class T>
struct vector3D {
 T v[3];
 vector3D() {}
 vector3D(const T& p1, const T& p2, const T& p3) {
  v[0] = p1;
  v[1] = p2;
  v[2] = p3;
 }
 vector3D(const vector3D<T>& obj) {
  v[0] = obj.v[0];
  v[1] = obj.v[1];
  v[2] = obj.v[2];
 }
 explicit vector3D(const T* ptr) {
  v[0] = ptr[0];
  v[1] = ptr[1];
  v[2] = ptr[2];
 }
 T& operator [](size_t index) { return v[index]; }
 const T& operator [](size_t index)const { return v[index]; }
 vector3D<T>& operator +=(const vector3D<T>& other) {
  v[0] += other.v[0];
  v[1] += other.v[1];
  v[2] += other.v[2];
  return *this;
 }
 vector3D<T>& operator -=(const vector3D<T>& other) {
  v[0] -= other.v[0];
  v[1] -= other.v[1];
  v[2] -= other.v[2];
  return *this;
 }
};

template<class T>
inline vector3D<T> operator +(const vector3D<T>& v1, const vector3D<T>& v2) {
 vector3D<T> temp;
 temp.v[0] = v1.v[0] + v2.v[0];
 temp.v[1] = v1.v[1] + v2.v[1];
 temp.v[2] = v1.v[2] + v2.v[2];
 return temp;
}

template<class T>
inline vector3D<T> operator -(const vector3D<T>& v1, const vector3D<T>& v2) {
 vector3D<T> temp;
 temp.v[0] = v1.v[0] - v2.v[0];
 temp.v[1] = v1.v[1] - v2.v[1];
 temp.v[2] = v1.v[2] - v2.v[2];
 return temp;
}

template<class T>
inline vector3D<T> vector_product(const vector3D<T>& v1, const vector3D<T>& v2)
{
 return vector3D<T>(
  v1.v[1]*v2.v[2] - v1.v[2]*v2.v[1],
  v1.v[2]*v2.v[0] - v1.v[0]*v2.v[2],
  v1.v[0]*v2.v[1] - v1.v[1]*v2.v[0]
 );
}

template<class T>
inline T scalar_product(const vector3D<T>& v1, const vector3D<T>& v2)
{
 return (v1.v[0]*v2.v[0]) + (v1.v[1]*v2.v[1]) + (v1.v[2]*v2.v[2]);
}

template<class T>
inline void orthogonalize(vector3D<T>& dst, const vector3D<T>& u, const vector3D<T>& v)
{
 T scale = scalar_product(u, v);
 dst[0] = v[0] - u[0]*scale;
 dst[1] = v[1] - u[1]*scale;
 dst[2] = v[2] - u[2]*scale;
}

inline float vector3D_squared_norm(const float* v)
{
 return (v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]);
}

inline float vector3D_norm(const float* v)
{
 return sqrt(vector3D_squared_norm(v));
}

inline float vector3D_normalize(float* v)
{
 float norm = vector3D_norm(v);
 if(norm < 1.0e-6f) return norm;
 float scalar = inv(norm);
 v[0] *= scalar;
 v[1] *= scalar;
 v[2] *= scalar;
 return norm;
}

inline float vector3D_normalize(float* x, const float* v)
{
 float norm = vector3D_norm(v);
 if(norm < 1.0e-6f) return norm;
 float scalar = inv(norm);
 x[0] = v[0]*scalar;
 x[1] = v[1]*scalar;
 x[2] = v[2]*scalar;
 return norm;
}

inline float vector3D_scalar_product(const float* v1, const float* v2)
{
 return (v1[0]*v2[0]) + (v1[1]*v2[1]) + (v1[2]*v2[2]);
}

inline void vector3D_orthogonalize(float* dst, const float* u, const float* v)
{
 float scale = vector3D_scalar_product(u, v);
 dst[0] = v[0] - u[0]*scale;
 dst[1] = v[1] - u[1]*scale;
 dst[2] = v[2] - u[2]*scale;
}

//
// 4D VECTORS
//

template<class T>
struct vector4D {
 union {
  T v[4];
  struct { T r, g, b, a; };
  struct { T x, y, z, w; };
 };
 T& operator [](size_t index) { return v[index]; }
 const T& operator [](size_t index)const { return v[index]; }
};

//
// TRIANGLES
//

struct triangle3D {
 float A[3];
 float B[3];
 float C[3];
 float color_A[3];
 float color_B[3];
 float color_C[3];
 float uv_A[2];
 float uv_B[2];
 float uv_C[2];
};

//
// MATRIX FUNCTIONS
// 
#pragma region MATRIX_FUNCTIONS

inline void Matrix4DVector3DMul(float* X, const float* A, const float* B)
{
 float R[3] = {
  A[0x0]*B[0x0] + A[0x1]*B[0x1] + A[0x2]*B[0x2] + A[0x3],
  A[0x4]*B[0x0] + A[0x5]*B[0x1] + A[0x6]*B[0x2] + A[0x7],
  A[0x8]*B[0x0] + A[0x9]*B[0x1] + A[0xA]*B[0x2] + A[0xB] 
 };
 X[0x0] = R[0x0];
 X[0x1] = R[0x1];
 X[0x2] = R[0x2];
}

inline void RotationMatrix4D_YZ(float* m, float r1, float r2)
{
 float zer = 0.0f;
 float one = 1.0f;
 float c1 = std::cos(r1); float c2 = std::cos(r2);
 float s1 = std::sin(r1); float s2 = std::sin(r2);
 m[0x0] =  c1*c2; m[0x1] = -c1*s2; m[0x2] =  s1; m[0x3] = zer;
 m[0x4] =     s2; m[0x5] =     c2; m[0x6] = zer; m[0x7] = zer;
 m[0x8] = -s1*c2; m[0x9] =  s1*s2; m[0xA] =  c1; m[0xB] = zer;
 m[0xC] =    zer; m[0xD] =    zer; m[0xE] = zer; m[0xF] = one;
}

#pragma endregion MATRIX_FUNCTIONS

//
// BOUNDING BOXES
// 
struct MinMaxAABB {
 public :
  float bbmin[3];
  float bbmax[3];
 // QUERY FUNCTIONS
 public :
  float dx(void)const { return bbmax[0] - bbmin[0]; }
  float dy(void)const { return bbmax[1] - bbmin[1]; }
  float dz(void)const { return bbmax[2] - bbmin[2]; }
  vector3D<float> dimensions(void)const {
   return vector3D<float>(this->dx(), this->dy(), this->dz());
  }
  int dominator(void)const {
   float cx = this->dx();
   float cy = this->dy();
   float cz = this->dz();
   if(cx < cy) return (cy < cz ? 2 : 1); // not dx
   return (cx < cz ? 2 : 0); // not dy
  }
  int submissor(void)const {
   float cx = this->dx();
   float cy = this->dy();
   float cz = this->dz();
   if(cz < cy) return (cz < cx ? 2 : 0); // not dy
   return (cy < cx ? 1 : 0); // not dz
  }
 public :
  void set(const float* data) {
   bbmin[0] = bbmax[0] = data[0];
   bbmin[1] = bbmax[1] = data[1];
   bbmin[2] = bbmax[2] = data[2];
  }
  void set(float a, float b) {
   bbmin[0] = bbmin[1] = bbmin[2] = a;
   bbmax[0] = bbmax[1] = bbmax[2] = b;
  }
  void set(const float* a, const float* b) {
   bbmin[0] = a[0]; bbmin[1] = a[1]; bbmin[2] = a[2];
   bbmax[0] = b[0]; bbmax[1] = b[1]; bbmax[2] = b[2];
  }
 public :
  // grow from single point
  void grow(const float* data) {
   if(data[0] < bbmin[0]) bbmin[0] = data[0]; else if(bbmax[0] < data[0]) bbmax[0] = data[0];
   if(data[1] < bbmin[1]) bbmin[1] = data[1]; else if(bbmax[1] < data[1]) bbmax[1] = data[1];
   if(data[2] < bbmin[2]) bbmin[2] = data[2]; else if(bbmax[2] < data[2]) bbmax[2] = data[2];
  }
  void grow(const vector3D<float>& v) {
   if(v[0] < bbmin[0]) bbmin[0] = v[0]; else if(bbmax[0] < v[0]) bbmax[0] = v[0];
   if(v[1] < bbmin[1]) bbmin[1] = v[1]; else if(bbmax[1] < v[1]) bbmax[1] = v[1];
   if(v[2] < bbmin[2]) bbmin[2] = v[2]; else if(bbmax[2] < v[2]) bbmax[2] = v[2];
  }
  // grow from another AABB
  void grow(const MinMaxAABB& other) {
   // update min bounds
   if(other.bbmin[0] < bbmin[0]) bbmin[0] = other.bbmin[0];
   if(other.bbmin[1] < bbmin[1]) bbmin[1] = other.bbmin[1];
   if(other.bbmin[2] < bbmin[2]) bbmin[2] = other.bbmin[2];
   // update max bounds
   if(bbmax[0] < other.bbmax[0]) bbmax[0] = other.bbmax[0];
   if(bbmax[1] < other.bbmax[1]) bbmax[1] = other.bbmax[1];
   if(bbmax[2] < other.bbmax[2]) bbmax[2] = other.bbmax[2];
  }
 public :
  // create from three points
  void from(const float* va, const float* vb, const float* vc) {
   bbmin[0] = bbmax[0] = va[0];
   bbmin[1] = bbmax[1] = va[1];
   bbmin[2] = bbmax[2] = va[2];
   grow(vb);
   grow(vc);
  }
  // create from one AABB
  void from(const MinMaxAABB& A) {
   bbmin[0] = A.bbmin[0]; bbmin[1] = A.bbmin[1]; bbmin[2] = A.bbmin[2];
   bbmax[0] = A.bbmax[0]; bbmax[1] = A.bbmax[1]; bbmax[2] = A.bbmax[2];
  }
  // create from two AABB
  void from(const MinMaxAABB& A, const MinMaxAABB& B) {
   // compare minimums                                                          |
   bbmin[0] = (A.bbmin[0] < B.bbmin[0]) ? A.bbmin[0] : B.bbmin[0];
   bbmin[1] = (A.bbmin[1] < B.bbmin[1]) ? A.bbmin[1] : B.bbmin[1];
   bbmin[2] = (A.bbmin[2] < B.bbmin[2]) ? A.bbmin[2] : B.bbmin[2];
   // compare maximums
   bbmax[0] = (A.bbmax[0] < B.bbmax[0]) ? B.bbmax[0] : A.bbmax[0];
   bbmax[1] = (A.bbmax[1] < B.bbmax[1]) ? B.bbmax[1] : A.bbmax[1];
   bbmax[2] = (A.bbmax[2] < B.bbmax[2]) ? B.bbmax[2] : A.bbmax[2];
  }
 public :
  MinMaxAABB() {}
  explicit MinMaxAABB(const float* data) {
   bbmin[0] = bbmax[0] = data[0];
   bbmin[1] = bbmax[1] = data[1];
   bbmin[2] = bbmax[2] = data[2];
  }
  explicit MinMaxAABB(const vector3D<float>& v) {
   bbmin[0] = bbmax[0] = v[0];
   bbmin[1] = bbmax[1] = v[1];
   bbmin[2] = bbmax[2] = v[2];
  }
  MinMaxAABB(float a, float b) {
   bbmin[0] = bbmin[1] = bbmin[2] = a;
   bbmax[0] = bbmax[1] = bbmax[2] = b;
  }
  MinMaxAABB(const float* a, const float* b) {
   bbmin[0] = a[0]; bbmin[1] = a[1]; bbmin[2] = a[2];
   bbmax[0] = b[0]; bbmax[1] = b[1]; bbmax[2] = b[2];
  }
};

inline void centroid(const MinMaxAABB& aabb, float* out) {
 if(out) {
    out[0] = (aabb.bbmin[0] + aabb.bbmax[0])*0.5f;
    out[1] = (aabb.bbmin[1] + aabb.bbmax[1])*0.5f;
    out[2] = (aabb.bbmin[2] + aabb.bbmax[2])*0.5f;
   }
}
inline vector3D<float> centroid(const MinMaxAABB& aabb) {
 return vector3D<float>(
  (aabb.bbmin[0] + aabb.bbmax[0])*0.5f,
  (aabb.bbmin[1] + aabb.bbmax[1])*0.5f,
  (aabb.bbmin[2] + aabb.bbmax[2])*0.5f);
}

//
// 3D RAYS
//

struct ray3D {
 float origin[3];
 float direction[3];
 float inverse[3];
};

struct ray3D_triangle3D_intersect_result {
 bool intersect;
 float t;
 float u;
 float v;
};

//
// RAY TRIANGLE INTERSECTION
//

inline void ray3D_triangle3D_intersect(ray3D_triangle3D_intersect_result& result, const ray3D& ray, const float* A, const float* B, const float* C, bool cull)
{
 // compute E = C - A
 // compute F = C - B
 float E[3] = { C[0] - A[0], C[1] - A[1], C[2] - A[2] };
 float F[3] = { C[0] - B[0], C[1] - B[1], C[2] - B[2] };

 // compute F cross D
 float F_cross_D[3] = {
  (F[1]*ray.direction[2] - F[2]*ray.direction[1]),
  (F[2]*ray.direction[0] - F[0]*ray.direction[2]),
  (F[0]*ray.direction[1] - F[1]*ray.direction[0])
 };

 // compute determinant
 // if this dot product is zero, then the ray and normal vector of the triangle are orthogonal,
 // which means that the ray and triangle edge vectors are parallel
 float determinant = E[0]*F_cross_D[0] + E[1]*F_cross_D[1] + E[2]*F_cross_D[2];
 if(zerotest(determinant))
   {
    // compute L = C - O
    float L[3] = {
     C[0] - ray.origin[0],
     C[1] - ray.origin[1],
     C[2] - ray.origin[2]
    };

    // compute (L dot (F cross D))
    // if this triple scalar product is zero, the ray and triangle are in the same plane
    float L_dot_F_cross_D = (L[0]*F_cross_D[0] + L[1]*F_cross_D[1] + L[2]*F_cross_D[2]);
    if(zerotest(L_dot_F_cross_D))
      {
       // may or may not intersect, but just say no anyways
       result.intersect = false;
      }
    else
       result.intersect = false;

    return;
   }

 // 
 if(cull)
   {
   }
 // not culling
 else
   {
    // compute L = C - O
    float inverse_determinant = inv(determinant);
    float L[3] = {
     C[0] - ray.origin[0],
     C[1] - ray.origin[1],
     C[2] - ray.origin[2]
    };

    // compute u = (L dot (F cross D))/determinant
    float u = (L[0]*F_cross_D[0] + L[1]*F_cross_D[1] + L[2]*F_cross_D[2])*inverse_determinant;
    if(u < 0.0f || u > 1.0f) {
       result.intersect = false;
       return;
      }

    // compute E cross L
    float E_cross_L[3] = {
     (E[1]*L[2] - E[2]*L[1]),
     (E[2]*L[0] - E[0]*L[2]),
     (E[0]*L[1] - E[1]*L[0])
    };

    // compute v = (D dot (E cross L))/determinant
    float v = (ray.direction[0]*E_cross_L[0] + ray.direction[1]*E_cross_L[1] + ray.direction[2]*E_cross_L[2])*inverse_determinant;
    if((v < 0.0f) || ((u + v) > 1.0f)) {
       result.intersect = false;
       return;
      }

    // compute t = (F dot (L cross E))/determinant
    result.intersect = true;
    result.t = -(F[0]*E_cross_L[0] + F[1]*E_cross_L[1] + F[2]*E_cross_L[2])*inverse_determinant;
    result.u = u;
    result.v = v;
   }
}

inline bool ray3D_AABB(const ray3D& ray, const MinMaxAABB& aabb, float tmin, float tmax)
{
 // x-axis
 float t0 = (aabb.bbmin[0] - ray.origin[0])*ray.inverse[0]; // new tmin value
 float t1 = (aabb.bbmax[0] - ray.origin[0])*ray.inverse[0]; // new tmax value
 tmin = std::max(std::min(t0, t1), tmin);
 tmax = std::min(std::max(t0, t1), tmax);
 if(tmax < tmin) return false;

 // y-axis
 t0 = (aabb.bbmin[1] - ray.origin[1])*ray.inverse[1];
 t1 = (aabb.bbmax[1] - ray.origin[1])*ray.inverse[1];
 tmin = std::max(std::min(t0, t1), tmin);
 tmax = std::min(std::max(t0, t1), tmax);
 if(tmax < tmin) return false;

 // z-axis
 t0 = (aabb.bbmin[2] - ray.origin[2])*ray.inverse[2];
 t1 = (aabb.bbmax[2] - ray.origin[2])*ray.inverse[2];
 tmin = std::max(std::min(t0, t1), tmin);
 tmax = std::min(std::max(t0, t1), tmax);
 if(tmax < tmin) return false;
 return true;
}

inline bool ray3D_AABB(const ray3D& ray, const float* bbmin, const float* bbmax, float tmin, float tmax)
{
 // x-axis
 float t0 = (bbmin[0] - ray.origin[0])*ray.inverse[0]; // new tmin value
 float t1 = (bbmax[0] - ray.origin[0])*ray.inverse[0]; // new tmax value
 tmin = std::max(std::min(t0, t1), tmin);
 tmax = std::min(std::max(t0, t1), tmax);
 if(tmax < tmin) return false;

 // y-axis
 t0 = (bbmin[1] - ray.origin[1])*ray.inverse[1];
 t1 = (bbmax[1] - ray.origin[1])*ray.inverse[1];
 tmin = std::max(std::min(t0, t1), tmin);
 tmax = std::min(std::max(t0, t1), tmax);
 if(tmax < tmin) return false;

 // z-axis
 t0 = (bbmin[2] - ray.origin[2])*ray.inverse[2];
 t1 = (bbmax[2] - ray.origin[2])*ray.inverse[2];
 tmin = std::max(std::min(t0, t1), tmin);
 tmax = std::min(std::max(t0, t1), tmax);
 if(tmax < tmin) return false;
 return true;
}

#endif
