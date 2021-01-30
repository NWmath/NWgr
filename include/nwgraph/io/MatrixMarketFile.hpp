// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//     Kevin Deweese	
//

#pragma once

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_WINDOWS)
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <io.h>
#endif

extern "C" {
#include "mmio_nist.h"
}

#if defined(_MSC_VER)
// Windows has no mmap(), let's emulate it here
#include <windows.h>

#define PROT_READ 1
#define PROT_WRITE 2
#define MAP_SHARED 1
#define MAP_PRIVATE 2
#define MAP_FIXED 4
#define MAP_NORESERVE 8
#define MAP_FAILED ((void*)-1)

inline void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
  void* retval = MAP_FAILED;

  // recode PROT and FLAGS into desired access
  int iAccess = 0;
  int iProt   = 0;
  int iFlags  = 0;
  if (prot & PROT_READ) {
    iAccess = iAccess | GENERIC_READ;
    iProt   = PAGE_READONLY; // READONLY assumed
    iFlags  = FILE_MAP_READ; // and FLAGS are ignored
  }
  if (prot & PROT_WRITE) {
    iAccess = iAccess | GENERIC_WRITE;
    if (flags & MAP_PRIVATE) { // Write changes won't be shared
      iProt  = PAGE_WRITECOPY;
      iFlags = FILE_MAP_COPY;
    } else if (flags & MAP_SHARED) { // Write changes will be shared
      iProt  = PAGE_READWRITE;
      iFlags = FILE_MAP_WRITE;
    }
  }
  if ((iProt != 0) && (iFlags != 0)) { // valid PROT, FLAGS options passed

    HANDLE file_handle = (HANDLE)_get_osfhandle(fd);
    if (file_handle == NULL) return retval; // "Could not open file"

    HANDLE file_mapping = CreateFileMapping(file_handle, NULL, iProt, 0, 0, NULL);
    if (file_mapping == NULL) return retval; // "Could not create file mapping"

    void* staddr = MapViewOfFileEx(file_mapping, iFlags, 0, offset, length, (LPVOID)addr);
    if (staddr == nullptr) return retval;    // "Could not map the file"

    if (!CloseHandle(file_mapping)) return retval; // "Could not close file mapping"
    if (!CloseHandle(file_handle)) return retval;  // "Could not close file handle"

    retval = staddr;    //  return address of mapped memory
  }
  return retval;
}

inline int munmap(void* start, size_t length) { return (int)(UnmapViewOfFile((LPVOID)start)); }
#endif

namespace mmio {
template <typename It>
class Range {
  It begin_;
  It end_;

public:
  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }
};

class MatrixMarketFile final {
  int fd_  = -1;    // .mtx file descriptor
  int n_   = 0;     // number of rows
  int m_   = 0;     // number of columns
  int nnz_ = 0;     // number of edges

  char* base_ = nullptr;    // base pointer to mmap-ed file
  long  i_    = 0;          // byte offset of the first edge
  long  e_    = 0;          // bytes in the mmap-ed file

  MM_typecode type_;

public:
  MatrixMarketFile(std::filesystem::path path) : fd_(open(path.c_str(), O_RDONLY)) {
    if (fd_ < 0) {
      fprintf(stderr, "open failed, %d: %s\n", errno, strerror(errno));
      std::terminate();
    }

    FILE* f = fdopen(fd_, "r");
    if (f == nullptr) {
      fprintf(stderr, "fdopen failed, %d: %s\n", errno, strerror(errno));
      goto error;
    }

    switch (mm_read_banner(f, &type_)) {
      case MM_PREMATURE_EOF:       // if all items are not present on first line of file.
      case MM_NO_HEADER:           // if the file does not begin with "%%MatrixMarket".
      case MM_UNSUPPORTED_TYPE:    // if not recongizable description.
        goto error;
    }

    if (!mm_is_coordinate(type_)) {
      goto error;
    }

    switch (mm_read_mtx_crd_size(f, &n_, &m_, &nnz_)) {
      case MM_PREMATURE_EOF:    // if an end-of-file is encountered before processing these three values.
        goto error;
    }

    i_ = ftell(f);
    fseek(f, 0L, SEEK_END);
    e_ = ftell(f);

    base_ = static_cast<char*>(mmap(nullptr, e_, PROT_READ, MAP_PRIVATE, fd_, 0));
    if (base_ == MAP_FAILED) {
      fprintf(stderr, "mmap failed, %d: %s\n", errno, strerror(errno));
      goto error;
    }

    return;

  error:
    close(fd_);
    std::terminate();
  };

  ~MatrixMarketFile() { release(); };

  /// Release the memory mapping and file descriptor early.
  void release() {
    if (base_ && munmap(base_, e_)) {
      fprintf(stderr, "munmap failed, %d: %s\n", errno, strerror(errno));
    }
    base_ = nullptr;

    if (fd_ != -1 && close(fd_)) {
      fprintf(stderr, "close(fd) failed, %d: %s\n", errno, strerror(errno));
    }
    fd_ = -1;
  };

  /// ADL
  friend void release(MatrixMarketFile& mm) { mm.release(); }

  int getNRows() const { return n_; }

  int getNCols() const { return m_; }

  int getNEdges() const { return nnz_; }

  bool isPattern() const { return mm_is_pattern(type_); }

  bool isSymmetric() const { return mm_is_symmetric(type_); }

  // Iterator over edges in the file.
  template <typename... Vs>
  class iterator {
    const char* i_;

    /// Read the next token in the stream as a U, and update the pointer.
    ///
    /// This horrible code is required because normal stream processing is very
    /// slow, while this tokenized version is just sort of slow.
    template <typename U>
    static constexpr U get(const char*(&i)) {
      U     v;
      char* e = nullptr;
      if constexpr (std::is_same_v<U, int>) {
        v = std::strtol(i, &e, 10);
      } else {
        v = std::strtod(i, &e);
      }
      i = e;
      return v;
    }

  public:
    iterator(const char* i) : i_(i) {}

    std::tuple<int, int, Vs...> operator*() const {
      const char* i = i_;
      int         u = get<int>(i) - 1;
      int         v = get<int>(i) - 1;
      return std::tuple(u, v, get<Vs>(i)...);
    }

    iterator& operator++() {
      ++(i_ = std::strchr(i_, '\n'));
      return *this;
    }

    bool operator!=(const iterator& b) const { return i_ != b.i_; }
  };

  template <typename... Vs>
  iterator<Vs...> begin() const {
    return {base_ + i_};
  }

  template <typename... Vs>
  iterator<Vs...> end() const {
    return {base_ + e_};
  }

  template <typename... Vs>
  iterator<Vs...> at(long edge) const {
    if (edge == 0) return begin<Vs...>();
    if (edge == nnz_) return end<Vs...>();

    // Use the edge id to come up with an approximation of the byte to start
    // searching at, and then search backward to find the byte offset of the
    // nearest edge.
    long approx = (double(edge) / double(nnz_)) * (e_ - i_) + i_;
    while (i_ <= approx && base_[approx] != '\n') {
      --approx;
    }
    ++approx;
    return {base_ + approx};
  }
};

template <typename... Vs>
auto edges(const MatrixMarketFile& mm, int j, int k) {
  return Range(mm.template at<Vs...>(j), mm.template at<Vs...>(k));
}
}    // namespace mmio
