#include <iostream>

#include "gtest/gtest.h"

TEST(dummy_test_case, dummy_test)
{
  ASSERT_TRUE(true) << "C++ is broken if this test fails!";
}

class DummyFixture : public testing::Test
{
protected:
  DummyFixture() = default;
  virtual ~DummyFixture() = default;

  virtual void SetUp() override
  {
    m_bool = true;
  }

  virtual void TearDown() override
  {
    m_bool = false;
  }

protected:
  bool m_bool = false;
};

TEST_F(DummyFixture, dummy_fixture_test)
{
  EXPECT_TRUE(m_bool);
}
