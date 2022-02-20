#ifndef __STDXNA_H
#define __STDXNA_H

struct XNACameraTarget {
 public :
  STDSTRINGW p1;
  STDSTRINGW p2;
 public :
  XNACameraTarget() {}
  XNACameraTarget(const STDSTRINGW& p1) {
   this->p1 = p1;
  }
  XNACameraTarget(const STDSTRINGW& p1, const STDSTRINGW& p2) {
   this->p1 = p1;
   this->p2 = p2;
  }
};

#endif
