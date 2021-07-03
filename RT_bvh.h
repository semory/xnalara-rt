#ifndef __RT_BVH_H
#define __RT_BVH_H

struct DepthTest;
struct XNAModel;
struct XNATexture;

class RTDynamicBVH {
 private :
  constexpr static uint32_t max_faces = 4;
  constexpr static uint32_t n_bins = 8;
  constexpr static float binning_epsilon = 1.0e-6f;
 public :
  XNAModel* model; // can't be constant -> must sort index buffer
  struct RTNode {
   float bbmin[3];
   union {
    uint32_t node_index;
    uint32_t face_index;
   };
   float bbmax[3];
   uint16_t faces;
   uint8_t flags1;
   uint8_t flags2;
  };
  std::vector<RTNode> tree;
 private :
  ray3D test;
  DepthTest* dt;
  void traverse(const RTNode& node)const;
 public :
  void construct(XNAModel* src);
  void rebuild(void);
  void clear(void);
 public :
  void set_depth_test(DepthTest* ptr) { dt = ptr; }
  bool raytest(const ray3D& ray);
 public :
  void traverse2(const RTNode& node)const;
  bool raytest2(const ray3D& ray);
 public :
  size_t nodes(void)const { return tree.size(); }
 public :
  RTDynamicBVH();
  RTDynamicBVH(const RTDynamicBVH&) = delete;
  void operator =(const RTDynamicBVH&) = delete;
 ~RTDynamicBVH();
};

#endif
