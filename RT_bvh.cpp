#include "stdafx.h"
#include "RT_math.h"
#include "RT_xnalara.h"
#include "RT_bvh.h"

RTDynamicBVH::RTDynamicBVH() : model(nullptr)
{
}

RTDynamicBVH::~RTDynamicBVH()
{
}

void RTDynamicBVH::construct(XNAModel* src)
{
 // this algorithm is based on the journal article by Ingo Wald:
 // [2007] On Fast Construction of SAH-based Bounding Volume Hierarchies
 if(!src) return;
 model = src;
 rebuild();
}

void RTDynamicBVH::rebuild(void)
{
 //
 // PHASE #0: VALIDATION
 // If a model has neither vertices nor faces, there is nothing to do!
 // 

 // nothing to do
 if(!model) return;
 if(!model->meshlist.size()) return;
 if(!model->facelist.size()) return;

 //
 // PHASE #1: INITIALIZATION
 //

 // clear previous
 tree.clear();

 // per-bin data
 MinMaxAABB binlist[n_bins];
 uint32_t bintris[n_bins];

 // per-partition data
 const uint32_t n_part = n_bins - 1;
 float costs[n_part];
 uint32_t NL[n_part];
 uint32_t NR[n_part];
 MinMaxAABB BL[n_part];
 MinMaxAABB BR[n_part];

 //
 // PHASE #1
 // PROCESS PER-FACE DATA
 // For each triangle, compute its min-max AABB and its centroid. The centroid
 // is used to place triangles into bins and the AABBs are used "loosen" boxes
 // so that they enclose all of their binned triangles.
 //

 // temporary per-face data
 std::unique_ptr<MinMaxAABB[]> blist(new MinMaxAABB[model->facelist.size()]);
 std::unique_ptr<vector3D<float>[]> clist(new vector3D<float>[model->facelist.size()]);

 // compute per-face data
 for(size_t i = 0; i < model->facelist.size(); i++) {
     uint32_t mi = model->facelist[i].mesh_index;
     uint32_t fa = model->facelist[i].refs[0];
     uint32_t fb = model->facelist[i].refs[1];
     uint32_t fc = model->facelist[i].refs[2];
     const float* va = model->meshlist[mi].verts[fa].position;
     const float* vb = model->meshlist[mi].verts[fb].position;
     const float* vc = model->meshlist[mi].verts[fc].position;
     blist[i].from(va, vb, vc); // AABB from triangle
     clist[i] = centroid(blist[i]); // centroid of triangle AABB
    }

 //
 // PHASE #2
 // NON-RECURSIVE SPACIAL PARITIONING
 //

 // add root node to tree
 tree.push_back(RTNode());

  // add full model information to stack
 struct BVHSTACKITEM {
  uint32_t tree_index;
  uint32_t face_index[2];
 };
 std::stack<BVHSTACKITEM, std::vector<BVHSTACKITEM>> stack;

 BVHSTACKITEM item;
 item.tree_index = 0;
 item.face_index[0] = 0;
 item.face_index[1] = static_cast<uint32_t>(model->facelist.size());
 stack.emplace(item);

 // non-recursive partitioning
 while(stack.size())
      {
       //
       // STEP #1
       // CALCULATE PARTITION BOUNDS
       // tbounds = AABB for all faces (for ray-AABB collision detection)
       // cbounds = AABB for all centroids (for binning)
       // tbounds will contain cbounds
       //

       // get top partition
       BVHSTACKITEM item;
       item.tree_index = stack.top().tree_index;
       item.face_index[0] = stack.top().face_index[0];
       item.face_index[1] = stack.top().face_index[1];
       stack.pop();

       // per-partition data (all triangle bounds, all centroid bounds)
       MinMaxAABB tbounds(blist[item.face_index[0]]);
       MinMaxAABB cbounds(clist[item.face_index[0]]);
       for(uint32_t i = (item.face_index[0] + 1); i < item.face_index[1]; i++) {
           tbounds.grow(blist[i]);
           cbounds.grow(clist[i]);
          }

       // set triangle bounds for this node
       tree[item.tree_index].bbmin[0] = tbounds.bbmin[0];
       tree[item.tree_index].bbmin[1] = tbounds.bbmin[1];
       tree[item.tree_index].bbmin[2] = tbounds.bbmin[2];
       tree[item.tree_index].bbmax[0] = tbounds.bbmax[0];
       tree[item.tree_index].bbmax[1] = tbounds.bbmax[1];
       tree[item.tree_index].bbmax[2] = tbounds.bbmax[2];

       // set flags for this node
       tree[item.tree_index].flags1 = 0;
       tree[item.tree_index].flags2 = 0;

       // this is an inner node -> must partition
       uint32_t n_face = item.face_index[1] - item.face_index[0];
       if(max_faces < n_face) {
          tree[item.tree_index].node_index = static_cast<uint32_t>(tree.size());
          tree[item.tree_index].faces = 0;
         }
       // this is an outer node -> skip partition
       else {
          tree[item.tree_index].face_index = item.face_index[0];
          tree[item.tree_index].faces = item.face_index[1] - item.face_index[0];
          continue;
         }       

       //
       // STEP #2
       // DETERMINE DOMINANT AXIS
       //

       // choose dominant "longest" axis from centroid bounds
       vector3D<float> dv = cbounds.dimensions();
       unsigned int axis = cbounds.dominator();

       // dominant axis is too small, so change this to a leaf node
       if((dv[axis] < 1.0e-6f) && !(16 < n_face)) {
          tree[item.tree_index].face_index = item.face_index[0];
          tree[item.tree_index].faces = item.face_index[1] - item.face_index[0];
          continue;
         }

       //
       // STEP #3
       // COMPUTE BINS
       //

       // binning constants
       const float k0 = cbounds.bbmin[axis];
       const float k1 = n_bins*(1.0f - binning_epsilon)/dv[axis];

       // lambdas
       auto BIN_INDEX_FROM_FACE = [&](uint32_t face) { return static_cast<uint32_t>(k1*(clist[face][axis] - k0)); };
       auto BIN_LIMIT_TEST = [](uint32_t n) { return !(max_faces < n); };

       // initialize per-bin data
       for(uint32_t i = 0; i < n_bins; i++) {
           bintris[i] = 0;
           binlist[i].set(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
          }

       // count number of triangles in bins and grow the bin AABBs to fit the triangles that are in each bin
       for(uint32_t i = item.face_index[0]; i < item.face_index[1]; i++) {
           uint32_t binindex = BIN_INDEX_FROM_FACE(i);
           bintris[binindex]++;
           binlist[binindex].grow(blist[i]); // grow by triangle bounds
          }

       //
       // STEP #4
       // PARTITIONING
       //

       // L-side computation
       NL[0] = bintris[0];
       BL[0] = binlist[0];
       for(uint32_t i = 1; i < n_part; i++) {
           NL[i] = NL[i - 1] + bintris[i];
           BL[i].from(BL[i - 1], binlist[i]);
          }

       // R-side computation
       uint32_t j = n_part - 1;
       uint32_t k = n_part;
       NR[j] = bintris[k];
       BR[j] = binlist[k];
       for(uint32_t i = 1; i < n_part; i++) {
           j--;
           k--;           
           NR[j] = NR[k] + bintris[k];
           BR[j].from(BR[k], binlist[k]);
          }

       // surface area lambda
       auto SURFACE_AREA = [](const MinMaxAABB& box) {
        float dx = box.dx();
        float dy = box.dy();
        float dz = box.dz();
        return dx*dy + dx*dz + dy*dz;
       };

       // compute index of best partition
       uint32_t best_index = 0;
       costs[0] = NL[0]*SURFACE_AREA(BL[0]) + NR[0]*SURFACE_AREA(BR[0]);
       for(uint32_t i = 1; i < n_part; i++) {
           costs[i] = NL[i]*SURFACE_AREA(BL[i]) + NR[i]*SURFACE_AREA(BR[i]);
           if(costs[i] < costs[best_index]) best_index = i;
          }

       //
       // STEP #5
       // INDEX BUFFER SORTING
       //

       // sort index buffer
       uint32_t L_count = 0;
       uint32_t L_pivot = item.face_index[0];
       uint32_t R_pivot = item.face_index[1] - 1;

       // stop when L_count matches with the number of faces on the L-side
       while(L_count < NL[best_index])
            {
             // bin for L_pivot is on R-side
             uint32_t i = BIN_INDEX_FROM_FACE(L_pivot);
             if(best_index < i)
               {
                for(;;)
                   {
                    // bin for R_pivot is on R-side (just move R_pivot over)
                    uint32_t j = BIN_INDEX_FROM_FACE(R_pivot);
                    if(best_index < j) R_pivot--;
                    // bin for R_pivot is on L-side
                    else
                      {
                       // swap faces and per-face data
                       std::swap(model->facelist[L_pivot], model->facelist[R_pivot]);
                       std::swap(blist[L_pivot], blist[R_pivot]);
                       std::swap(clist[L_pivot], clist[R_pivot]);
                       // move pivots
                       L_pivot++;
                       R_pivot--;
                       L_count++;
                       break;
                      }
                   }
               }
             // bin for L_pivot is on L-side (just move L_pivot over)
             else {
                L_pivot++;
                L_count++;
               }
            }

       //
       // STEP #6
       // DIVIDE AND CONQUER
       //

       auto SET_OUTER_NODE = [](RTNode& node, const MinMaxAABB& aabb, uint32_t index, uint32_t n) {
        node.bbmin[0] = aabb.bbmin[0];
        node.bbmin[1] = aabb.bbmin[1];
        node.bbmin[2] = aabb.bbmin[2];
        node.face_index = index;
        node.bbmax[0] = aabb.bbmax[0];
        node.bbmax[1] = aabb.bbmax[1];
        node.bbmax[2] = aabb.bbmax[2];
        node.faces = n;
        node.flags1 = 0;
        node.flags2 = 0;
       };

       // L = leaf
       tree.push_back(RTNode());
       if(BIN_LIMIT_TEST(NL[best_index])) {
          SET_OUTER_NODE(tree.back(), BL[best_index], item.face_index[0], NL[best_index]);
         }
       // L = node
       else {
          BVHSTACKITEM next;
          next.tree_index = static_cast<uint32_t>(tree.size() - 1);
          next.face_index[0] = item.face_index[0];
          next.face_index[1] = item.face_index[0] + L_count;
          stack.emplace(next);
         }

       // R = leaf
       tree.push_back(RTNode());
       if(BIN_LIMIT_TEST(NR[best_index])) {
          SET_OUTER_NODE(tree.back(), BR[best_index], item.face_index[0] + L_count, NR[best_index]);
         }
       // R = node
       else {
          BVHSTACKITEM next;
          next.tree_index = static_cast<uint32_t>(tree.size() - 1);
          next.face_index[0] = item.face_index[0] + L_count;
          next.face_index[1] = item.face_index[1];
          stack.emplace(next);
         }
      }
}

void RTDynamicBVH::clear(void)
{
 tree.clear();
}