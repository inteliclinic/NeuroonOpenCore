#ifdef DESKTOP_BUILD

#ifndef __VECTOR_VIEW__
#define __VECTOR_VIEW__

#include <iterator>
#include <vector>
#include <array>

template <typename T>
class VectorView {
  typedef typename std::vector<T>::const_iterator cit;
  cit _begin;
  cit _end;
public:
  // ...

  VectorView():
    _begin(0), _end(0) {}
  VectorView(cit begin, cit end):
    _begin(begin), _end(end) {}
  VectorView(const std::vector<T> & v):
    _begin(v.begin()), _end(v.end()) {}

  cit begin() const { return this->_begin; }
  cit end()   const { return this->_end; }
  const T& operator[](std::size_t index) const { return this->_begin[index]; }

  std::size_t size() const{ return _end - _begin;}
};

template <typename T>
class ArrayView {
  std::iterator<std::random_access_iterator_tag, T> _begin;
  std::iterator<std::random_access_iterator_tag, T> _end;
public:
  // ...
  ArrayView(std::iterator<std::random_access_iterator_tag, T> begin,
            std::iterator<std::random_access_iterator_tag, T> end):
    _begin(begin), _end(end) {}

  const std::iterator<std::random_access_iterator_tag, T> begin() const { return this->_begin; }
  const std::iterator<std::random_access_iterator_tag, T> end() const   { return this->_end; }
  const typename std::iterator_traits< std::iterator<std::random_access_iterator_tag, T> >::reference
  operator[](std::size_t index) { return this->_begin[index]; }
  std::size_t size() const{ return _end - _begin;}
};

#endif

#endif
