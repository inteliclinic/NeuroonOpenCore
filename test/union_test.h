#include <gtest/gtest.h>
#include "CommonTypedefs.h"
#include "InValue.h"

struct TempTest : public ::testing::Test {


	virtual void SetUp() {
    ;
	}

	void TearDown() {
    ;
	}
};


TEST_F(TempTest, inmsg_getvalue) {
  // length of a window
  // trivial

  auto msg = InValue("msg");
  auto str = msg.string_value();
  EXPECT_EQ(str,"msg");

  auto msg1 = InValue(2.5);
  auto d = msg1.double_value();
  EXPECT_EQ(d,2.5);

  auto msg2 = InValue((llong)1);
  auto i = msg2.llong_value();
  EXPECT_EQ(i,1);
}

TEST_F(TempTest, inmsg_copy_constructor) {
  // length of a window
  // trivial
  {
  InValue msg = InValue("msg");
  ASSERT_NO_THROW({
      auto msgc = InValue(msg);
    });
  }
  {
    InValue msg = InValue(1LL);
    ASSERT_NO_THROW({
        auto msgc = InValue(msg);
      });
  }
  {
    InValue msg = InValue(2.5);
    ASSERT_NO_THROW({
        auto msgc = InValue(msg);
      });
  }
}
