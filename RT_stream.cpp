#include "stdafx.h"
#include "RT_stream.h"

const binary_stream::size_type binary_stream::npos = std::numeric_limits<size_type>::max();

binary_stream::binary_stream()
{
 elem = 0;
 position = npos;
}

binary_stream::binary_stream(std::unique_ptr<char[]>&& ptr, size_type n)
{
 if(ptr && n) {
    data = std::move(ptr);
    elem = n;
    position = 0;
   }
 else {
    elem = 0;
    position = npos;
   }
}

binary_stream::binary_stream(const char* filename) : elem(0), position(npos)
{
 // open file for reading
 HANDLE handle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(!handle) return;

 // get filesize
 DWORD filesize = 0;
 if(!GetFileSize(handle, &filesize)) return;

 // read file data
 std::unique_ptr<char[]> temp(new char[filesize]);
 DWORD bytes_read = 0;
 if(!ReadFile(handle, (LPVOID)temp.get(), filesize, &bytes_read, NULL)) return;

 // move file data
 data = std::move(temp);
 elem = filesize;
 position = 0;
}

binary_stream::binary_stream(const wchar_t* filename)
{
 // open file for reading
 HANDLE handle = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(!handle) return;

 // get filesize
 DWORD filesize = 0;
 if(!GetFileSize(handle, &filesize)) return;

 // read file data
 std::unique_ptr<char[]> temp(new char[filesize]);
 DWORD bytes_read = 0;
 if(!ReadFile(handle, (LPVOID)temp.get(), filesize, &bytes_read, NULL)) return;

 // move file data
 data = std::move(temp);
 elem = filesize;
 position = 0;
}

binary_stream::binary_stream(binary_stream&& other)noexcept
{
 // move data
 data = std::move(other.data);
 elem = other.elem;
 position = other.position;

 // replace data
 other.elem = 0;
 other.position = npos;
}

binary_stream::~binary_stream()
{
}

binary_stream& binary_stream::operator =(binary_stream&& other)noexcept
{
 // move data
 if(this == &other) return *this;
 data = std::move(other.data);
 elem = other.elem;
 position = other.position;

 // replace data
 other.elem = 0;
 other.position = npos;
 return *this;
}

bool binary_stream::can_seek(size_type pos)const
{
 // validate
 if(position == npos) return false; // current position must be valid
 if(pos == npos) return false; // seek position must bevalid
 return !(elem < pos);
}

bool binary_stream::can_move(diff_type pos)const
{
 // validate
 if(position == npos) return false; // current position must be valid
 if(pos == npos) return false; // offset must be valid

 // offset is negative
 if(pos < 0) {
    size_type amount = (size_type)std::abs(pos);
    return !(position < amount);
   }

 // offset is positive
 if(pos > 0) {
    size_type newpos = position + pos;
    return !(elem < newpos);
   }

 return true;
}

void binary_stream::seek(size_type pos)
{
 if(can_seek(pos)) position = pos;
 else position = npos;
}

void binary_stream::seek_reverse(size_type pos)
{
 if(can_seek(pos)) position = elem - pos;
 else position = npos;
}

void binary_stream::move(diff_type pos)
{
 if(pos == 0) return;
 else if(pos > 0) seek(position + pos);
 else if(pos < 0) {
    size_type off = (size_type)std::abs(pos);
    if(!(position < off)) position -= off;
    else position = npos;
   }
}

void binary_stream::move(diff_type pos, size_type from)
{
 if(fail()) return;
 seek(from);
 if(good()) move(pos);
}

void binary_stream::reset(void)
{
 position = 0;
}

void binary_stream::read(char* ptr, size_type n)
{
 if(fail() || !ptr || !n) return;
 size_type newpos = position + n;
 if(!can_move(n)) { position = npos; return; }
 memmove(ptr, c_pos(), n);
 position = newpos;
}

void binary_stream::read_reverse(char* ptr, size_type n)
{
 read(ptr, n);
 if(good()) std::reverse(ptr, ptr + n);
}

std::string binary_stream::read_string(void)
{
 std::string retval;
 for(;;) {
     char c;
     read(&c, 1);
     if(fail()) break;
     if(c == '\0') break;
     retval += c;
    }
 return retval;
}

std::string binary_stream::read_string(int alignment)
{
 std::string retval = read_string();
 if(alignment == 0) return retval;

 // variables
 size_t bytes_read = retval.length() + 1;
 size_t bytes_full = 0;

 // compute number of bytes to read
 if(alignment == 2) bytes_full = ((bytes_read + 0x01) & (~0x01));
 else if(alignment == 4) bytes_full = ((bytes_read + 0x03) & (~0x03));
 else if(alignment == 8) bytes_full = ((bytes_read + 0x07) & (~0x07));
 else if(alignment == 16) bytes_full = ((bytes_read + 0x0F) & (~0x0F));
 else if(alignment == 32) bytes_full = ((bytes_read + 0x1F) & (~0x1F));
 else if(alignment == 64) bytes_full = ((bytes_read + 0x3F) & (~0x3F));
 else return retval; // assume no alignment

 // skip past the difference
 if(bytes_read < bytes_full) move(bytes_full - bytes_read);
 return retval;
}