#ifndef __NC_SERIALIZATION_UTILS__
#define __NC_SERIALIZATION_UTILS__

#include <cstring>
#include <sstream>

template <class T>
std::size_t serialize(T &data, char *out_buffer, std::size_t sz) {
  auto actual_sz = sizeof(T);
  if (actual_sz > sz) {
    return actual_sz;
  }
  memcpy(out_buffer, reinterpret_cast<char *>(&data), actual_sz);
  return 0;
};

template <class T>
std::size_t deserialize(T &data, const char *in_buffer, std::size_t sz) {
  if (sz < sizeof(T))
    return sizeof(T);
  memcpy(reinterpret_cast<char *>(&data), in_buffer, sizeof(T));
  return 0;
};

#endif


// printf("ASDASDASD\n");
// for (int i = 0; i < sz; i++) {
//   printf("%d", in_buffer[i]);
//  }
// std::stringstream ss(in_buffer);
// if (sz < sizeof(T))
//   return sizeof(T);
// ss.read(reinterpret_cast<char *>(&data), sizeof(T));
// return 0;
