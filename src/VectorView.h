#ifndef __VECTOR_VIEW__
#define __VECTOR_VIEW__

#include <iterator>
#include <vector>

template <typename T>
class VectorView {
  typedef typename std::vector<T>::const_iterator cit;
  cit _begin;
  cit _end;
public:
  // ...
  VectorView(cit begin, cit end):
    _begin(begin), _end(end) {}

  cit begin() const { return this->_begin; }
  cit end()   const { return this->_end; }
  const T& operator[](std::size_t index) { return this->_begin[index]; }
};

// template <typename T>
// class ArrayView {
//   std::iterator<std::random_access_iterator_tag, T> _begin;
//   std::iterator<std::random_access_iterator_tag, T> _end;
// public:
//   // ...
//   ArrayView(std::iterator<std::random_access_iterator_tag, T> begin,
//             std::iterator<std::random_access_iterator_tag, T> end):
//     _begin(begin), _end(end) {}

//   std::iterator<std::random_access_iterator_tag, T> begin() const { return this->_begin; }
//   std::iterator<std::random_access_iterator_tag, T> end() const   { return this->_end; }
//   typename std::iterator_traits< std::iterator<std::random_access_iterator_tag, T> >::reference
//   operator[](std::size_t index) { return this->_begin[index]; }
// };

#endif
