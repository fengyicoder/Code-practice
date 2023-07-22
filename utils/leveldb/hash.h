#include <cstddef>
#include <cstdint>

#ifndef FALLTHROUGH_INTENDED
#define FALLTHROUGH_INTENDED \
  do {                       \
  } while (0)
#endif

uint32_t Hash(const char* data, size_t n, uint32_t seed);