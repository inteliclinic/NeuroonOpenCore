#include <gtest/gtest.h>
#include "InMsg.h"

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

  auto msg = InMsg("msg");
  auto str = msg.string_value();
  EXPECT_EQ(str,"msg");

  auto msg1 = InMsg(2.5,10);
  auto d = msg1.double_value();
  EXPECT_EQ(d,2.5);

  auto msg2 = InMsg(1,10);
  auto i = msg2.int_value();
  EXPECT_EQ(i,1);
}

TEST_F(TempTest, inmsg_copy_constructor) {
  // length of a window
  // trivial
  {
  InMsg msg = InMsg("msg",10);
  ASSERT_NO_THROW({
      auto msgc = InMsg(msg);
    });
  }
  {
    InMsg msg = InMsg(1,10);
    ASSERT_NO_THROW({
        auto msgc = InMsg(msg);
      });
  }
  {
    InMsg msg = InMsg(2.5,10);
    ASSERT_NO_THROW({
        auto msgc = InMsg(msg);
      });
  }
}
