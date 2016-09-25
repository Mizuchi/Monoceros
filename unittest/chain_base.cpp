#include "sbl/chain.hpp"
#include "gtest/gtest.h"

TEST(chain, insert) {
  auto chain = sbl::make_chain<int>();
  chain.insert(0, 0);
  chain.insert(1, 1);
  chain.insert(2, 2);
  chain.insert(3, 3);
  chain.insert(4, 4);
  EXPECT_EQ(chain[0], 0);
  EXPECT_EQ(chain[1], 1);
  EXPECT_EQ(chain[2], 2);
  EXPECT_EQ(chain[3], 3);
  EXPECT_EQ(chain[4], 4);
  EXPECT_EQ(chain.size(), 5);

  chain.insert(2, 5);
  chain.insert(5, 7);
  chain.insert(1, 9);
  EXPECT_EQ(chain[0], 0);
  EXPECT_EQ(chain[1], 9);
  EXPECT_EQ(chain[2], 1);
  EXPECT_EQ(chain[3], 5);
  EXPECT_EQ(chain[4], 2);
  EXPECT_EQ(chain[5], 3);
  EXPECT_EQ(chain[6], 7);
  EXPECT_EQ(chain[7], 4);
  EXPECT_EQ(chain.size(), 8);

  chain.erase(3, 6);
  EXPECT_EQ(chain[0], 0);
  EXPECT_EQ(chain[1], 9);
  EXPECT_EQ(chain[2], 1);
  EXPECT_EQ(chain[3], 7);
  EXPECT_EQ(chain[4], 4);
  EXPECT_EQ(chain.size(), 5);
}

TEST(chain, replace) {
  auto temp = sbl::make_chain<int>();
  static_assert(not temp.has<sbl::Replace>(), "");

  auto chain = sbl::make_chain<int, sbl::Replace>();
  static_assert(chain.has<sbl::Replace>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  chain.replace(3, 6, 9);
  EXPECT_EQ(chain[0], 0);
  EXPECT_EQ(chain[1], 9);
  EXPECT_EQ(chain[2], 1);
  EXPECT_EQ(chain[3], 9);
  EXPECT_EQ(chain[4], 9);
  EXPECT_EQ(chain[5], 9);
  EXPECT_EQ(chain[6], 7);
  EXPECT_EQ(chain[7], 4);
  EXPECT_EQ(chain.size(), 8);
}

TEST(chain, reverse) {
  auto temp = sbl::make_chain<int>();
  static_assert(not temp.has<sbl::Reverse>(), "");

  auto chain = sbl::make_chain<int, sbl::Reverse>();
  static_assert(chain.has<sbl::Reverse>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  chain.reverse(3, 6);
  EXPECT_EQ(chain[0], 0);
  EXPECT_EQ(chain[1], 9);
  EXPECT_EQ(chain[2], 1);
  EXPECT_EQ(chain[5], 5);
  EXPECT_EQ(chain[4], 2);
  EXPECT_EQ(chain[3], 3);
  EXPECT_EQ(chain[6], 7);
  EXPECT_EQ(chain[7], 4);
  EXPECT_EQ(chain.size(), 8);
}

TEST(chain, top) {
  auto temp = sbl::make_chain<int>();
  static_assert(not temp.has<sbl::Compare<>::Top>(), "");

  auto chain = sbl::make_chain<int, sbl::Compare<>::Top>();
  static_assert(chain.has<sbl::Compare<>::Top>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  EXPECT_EQ(chain.top(0, 2), 9);
  EXPECT_EQ(chain.top(3, 4), 5);
  EXPECT_EQ(chain.top(2, 4), 5);
  EXPECT_EQ(chain.top(3, 8), 7);
  EXPECT_EQ(chain.top(2, 6), 5);
}

TEST(chain, sum) {
  auto temp = sbl::make_chain<int>();
  static_assert(not temp.has<sbl::Sum>(), "");

  auto chain = sbl::make_chain<int, sbl::Sum>();
  static_assert(chain.has<sbl::Sum>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  EXPECT_EQ(chain.sum(0, 2), 9);
  EXPECT_EQ(chain.sum(3, 4), 5);
  EXPECT_EQ(chain.sum(2, 4), 6);
  EXPECT_EQ(chain.sum(3, 8), 21);
  EXPECT_EQ(chain.sum(2, 6), 11);
}

TEST(chain, max_element) {
  auto temp = sbl::make_chain<int>();
  static_assert(not temp.has<sbl::Compare<>::MaxElement>(), "");

  auto chain = sbl::make_chain<int, sbl::Compare<>::MaxElement>();
  static_assert(chain.has<sbl::Compare<>::MaxElement>(), "");
  chain += { 0, 9, 1, 5, 2, 3, 7, 4 };
  EXPECT_EQ(chain.max_element(0, 2), 1);
  EXPECT_EQ(chain.max_element(3, 4), 3);
  EXPECT_EQ(chain.max_element(2, 4), 3);
  EXPECT_EQ(chain.max_element(3, 8), 6);
  EXPECT_EQ(chain.max_element(2, 6), 3);
}

