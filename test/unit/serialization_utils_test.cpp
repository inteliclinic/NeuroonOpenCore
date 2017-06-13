#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "serialization_utils.h"
#include <cmath>

TEST(SerializationUtilsTest, serialization_simple_1) {

  unsigned long long asd = 10e10L;

  auto sz = sizeof(unsigned long long);
  char buf[sz];

  auto actual_sz = serialize(asd, buf, sz);

  EXPECT_EQ(0, actual_sz);

  unsigned long long bsd;

  actual_sz = deserialize(bsd, buf, sz);

  EXPECT_EQ(0, actual_sz);

  EXPECT_EQ(asd, bsd);
}

TEST(SerializationUtilsTest, serialization_simple_2) {

  struct Test {
    int i;
    std::string str;
  } asd, bsd;


  asd.str = "test";
  asd.i = -69;

  auto sz = sizeof(asd);

  std::cout << sizeof(asd);
  std::fflush(stdout);

  char buf[sz];

  auto actual_sz = serialize(asd, buf, sz);

  EXPECT_EQ(0, actual_sz);

  actual_sz = deserialize(bsd, buf, sz);

  EXPECT_EQ(0, actual_sz);

  EXPECT_EQ(asd.i, bsd.i);

  std::cout << asd.str<< ":" << bsd.str;
  std::fflush(stdout);

  EXPECT_STREQ(asd.str.c_str(), bsd.str.c_str());
}
