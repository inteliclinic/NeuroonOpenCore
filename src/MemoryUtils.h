#ifndef __MEM_UTILS__
#define __MEM_UTILS__

#include <memory>

template <class T, class U>
std::weak_ptr<T> static_pointer_cast(std::weak_ptr<U> const &r) {
  return std::static_pointer_cast<T>(std::shared_ptr<U>(r.lock()));
}

#endif
