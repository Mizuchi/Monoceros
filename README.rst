#########
Monoceros
#########

A C++14 library presenting intrusive binary tree data structure. It can be used to build other variant of the binary tree, such as order statistic tree and splay tree.

USAGE: sbl::chain
=====================

sbl::chain is a std::vector like data structure which is built on the top of monoceros. 
It is designed for efficient operation that involves a sequence elements as whole.
Operations such as insert, erase and get the max element in the sub-array from i-th to j-th element
take time that is logarithmic time of the length of the container's size.

Here is a list of all operations and the equivalent of std::vector


+-------------+---------------------+-----------------+---------------------------------------------+-----------------+
| operation   | sbl::chain<T> a;    | time complexity | std::vector<T> a;                           | time complexity |
+=============+=====================+=================+=============================================+=================+
| insert      | a.insert(i, j)      | O(logN)         | a.insert(begin(a) + i, j)                   | O(N)            |
+-------------+---------------------+                 +---------------------------------------------+                 +
| erase       | a.erase(i, j)       |                 | a.erase(begin(a) + i, begin(a) + j)         |                 |
+-------------+---------------------+                 +---------------------------------------------+                 +
| replace     | a.replace(i, j, k)  |                 | fill(begin(a) + i, begin(a) + j, k)         |                 |
+-------------+---------------------+                 +---------------------------------------------+                 +
| sum         | a.sum(i, j)         |                 | accumulate(begin(a) + i, begin(a) + j, T()) |                 |
+-------------+---------------------+                 +---------------------------------------------+                 +
| reverse     | a.reverse(i, j)     |                 | reverse(begin(a) + i, begin(a) + j)         |                 |
+-------------+---------------------+                 +---------------------------------------------+                 +
| max_element | a.max_element(i, j) |                 | max_element(begin(a) + i, begin(a) + j)     |                 |
+-------------+---------------------+                 +---------------------------------------------+                 +
| lcp         | a.lcp(i, j)         |                 | Longest common prefix                       |                 |
+-------------+---------------------+                 +---------------------------------------------+                 +
| max_sum     | a.max_sum(i, j)     |                 | Maximum subarray problem                    |                 |
+-------------+---------------------+                 +---------------------------------------------+-----------------+
| at          | a[i]                |                 | a[i]                                        | O(1)            |
+-------------+---------------------+-----------------+---------------------------------------------+-----------------+


Code Example
-------------

.. code-block:: cpp

        #include "sbl/chain.hpp"
        #include "gtest/gtest.h"

        TEST(chain, example) {
          auto chain = sbl::make_chain<int, 
                                       sbl::Sum, 
                                       sbl::Compare<>::MaxSum,
                                       sbl::Replace, 
                                       sbl::Reverse>();
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


License
=======

Code is licensed under Reciprocal Public License 1.5 (RPL-1.5): https://opensource.org/licenses/RPL-1.5
