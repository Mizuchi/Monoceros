#include "sbl/chain.hpp"
#include "gtest/gtest.h"

TEST(chain, replace_and_top) {
  auto chain = sbl::make_chain<int, sbl::Replace, sbl::Compare<>::Top>();
  static_assert(chain.has<sbl::Replace>(), "");
  static_assert(chain.has<sbl::Compare<>::Top>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  EXPECT_EQ(chain.top(2, 8), 7);
  chain.replace(5, 8, 4);
  EXPECT_EQ(chain.top(2, 8), 5);
}

TEST(chain, replace_and_max_element) {
  auto chain = sbl::make_chain<int, sbl::Replace, sbl::Compare<>::MaxElement>();
  static_assert(chain.has<sbl::Replace>(), "");
  static_assert(chain.has<sbl::Compare<>::MaxElement>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  EXPECT_EQ(chain.max_element(2, 8), 6);
  chain.replace(5, 8, 4);
  EXPECT_EQ(chain.max_element(2, 8), 3);
}

TEST(chain, replace_and_sum) {
  auto chain = sbl::make_chain<int, sbl::Replace, sbl::Sum>();
  static_assert(chain.has<sbl::Replace>(), "");
  static_assert(chain.has<sbl::Sum>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  EXPECT_EQ(chain.sum(2, 8), 22);
  chain.replace(5, 8, 4);
  EXPECT_EQ(chain.sum(2, 8), 20);
}

