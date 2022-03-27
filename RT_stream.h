#ifndef __RT_STREAM_H
#define __RT_STREAM_H

class binary_stream {
 public :
  typedef std::unique_ptr<char[]> data_type;
  typedef size_t size_type;
  typedef ptrdiff_t diff_type;
  static const size_type npos;
 private :
  data_type data;
  size_type elem;
  size_type position;
 public :
  char& operator [](size_t index) { return data.get()[index]; }
  const char& operator [](size_t index)const { return data.get()[index]; }
 public :
  size_type size(void)const { return elem; }
  const char* c_str(void)const { return data.get(); }
  const char* c_str(size_type pos)const { return data.get() + pos; }
  const char* c_pos(void)const { return data.get() + position; }
  size_type tell(void)const { return position; }
  bool at_begin(void)const { return position == 0; }
  bool at_end(void)const { return position == elem; }
  bool can_seek(size_type pos)const;
  bool can_move(diff_type pos)const;
  bool fail(void)const { return position == npos; }
  bool good(void)const { return position != npos; }
 public :
  void seek(size_type pos);
  void seek_reverse(size_type pos);
  void move(diff_type pos);
  void move(diff_type pos, size_type from);
  void reset(void);
 public :
  void read(char* ptr, size_type n);
  void read_reverse(char* ptr, size_type n);
  std::string read_string(void);
  std::string read_string(int alignment);
 // template functions
 private :
  template<class T>
  bool peek_value(T& value) {
   if(fail() || !(position < (npos - sizeof(T)))) return false;
   size_type newpos = position + sizeof(T);
   if(!can_seek(newpos)) return false; // don't fail
   value = *reinterpret_cast<const T*>(c_pos());
   return true;
  }
  template<class T>
  bool read_value(T& value) {
   if(fail() || !(position < (npos - sizeof(T)))) return false;
   size_type newpos = position + sizeof(T);
   if(!can_seek(newpos)) { position = npos; return false; }
   T r = *reinterpret_cast<const T*>(c_pos());
   position = newpos;
   value = r;
   return true;
  }
  template<class T>
  bool peek_value(T& value, size_type pos) {
   if(fail() || !(pos < (npos - sizeof(T)))) return false;
   size_type newpos = pos + sizeof(T);
   if(!can_seek(newpos)) return false; // don't fail
   value = *reinterpret_cast<const T*>(c_str(pos));
   return true;
  }
  template<class T>
  bool read_value(T& value, size_type pos) {
   if(fail() || !(pos < (npos - sizeof(T)))) return false;
   size_type newpos = pos + sizeof(T);
   if(!can_seek(newpos)) { position = npos; return false; }
   T r = *reinterpret_cast<const T*>(c_str(pos));
   position = newpos;
   value = r;
   return true;
  }
 // array functions
 public :
  template<class T>
  void read_array(T* ptr, size_type n) {
   if(fail() || !ptr || !n) return;
   size_type offset = n*sizeof(T);
   size_type newpos = position + offset;
   if(!can_seek(newpos)) { position = npos; return; }
   memmove(ptr, c_pos(), offset);
   position = newpos;
  }
  template<class T>
  void read_array(T* ptr, size_type n, size_type pos) {
   if(fail() || !ptr || !n) return;
   size_type offset = n*sizeof(T);
   if(!(pos < npos - offset)) { position = npos; return; }
   size_type newpos = pos + offset;
   if(!can_seek(newpos)) { position = npos; return; }
   memmove(ptr, c_str(pos), offset);
  }
 public :
  binary_stream& operator =(const binary_stream& other) = delete;
  binary_stream& operator =(binary_stream&& other)noexcept;
 public :
  binary_stream();
  binary_stream(std::unique_ptr<char[]>&& ptr, size_type n);
  explicit binary_stream(const char* filename);
  explicit binary_stream(const wchar_t* filename);
  binary_stream(const binary_stream& bs) = delete;
  binary_stream(binary_stream&& other)noexcept;
 ~binary_stream();
};

#endif
