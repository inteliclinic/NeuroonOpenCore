#ifndef __TEST_UTILS__
#define __TEST_UTILS__

#include <vector>
#include <memory>
#include <gtest/gtest.h>
#include "../../src/VectorView.h"


template<typename T>
void EXPECT_EQ_VECTORS(const std::vector<T> & v1,const std::vector<T> v2){
  std::vector<double>::size_type i = 0;
  /* EXPECT_EQ(v1.size(),v2.size()); */
  EXPECT_TRUE(v1.size()==v2.size());
  for(const auto& v : v1){
    EXPECT_EQ(v, v2[i]);
    i++;
  }
}


template<typename T>
void EXPECT_EQ_VECTORS(const std::vector<T> & v1,std::shared_ptr< const std::vector<T> > v2){
  std::vector<double>::size_type i = 0;
  EXPECT_TRUE(v1.size()==v2->size());
  for(const auto& v : v1){
    EXPECT_EQ(v, (*v2)[i]);
    i++;
  }
}

template<typename T>
void EXPECT_EQ_VECTORS(const std::vector<T> & v1,VectorView<T> v2){
  int i = 0;
  EXPECT_TRUE(v1.size()==v2.size());
  for(const auto& v : v1){
    EXPECT_EQ(v, v2[i]);
    i++;
  }
}

#endif
