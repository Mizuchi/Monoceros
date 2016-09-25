#include "sbl/chain.hpp"
#include "gtest/gtest.h"

TEST(chain, example) {
  auto chain = sbl::make_chain<int, sbl::Sum, sbl::Compare<>::MaxSum,
                               sbl::Replace, sbl::Reverse>();
  static_assert(chain.has<sbl::Replace>(), "");
  static_assert(chain.has<sbl::Reverse>(), "");
  static_assert(chain.has<sbl::Sum>(), "");
  static_assert(chain.has<sbl::Compare<>::MaxSum>(), "");
  chain += { 2, -6, 3, 5, 1, -5, -3, 6, 3 };
  EXPECT_EQ(chain.sum(4, 8), -1);
  EXPECT_EQ(chain.max_sum(0, 9), 10);
  chain.insert(8, -5);
  chain.insert(9, 7);
  chain.insert(10, 2);
  chain.erase(11, 12);
  chain.replace(2, 5, 2);
  chain.reverse(2, 8);
  EXPECT_EQ(chain.sum(4, 8), 1);
  EXPECT_EQ(chain.max_sum(0, chain.size()), 10);
}

