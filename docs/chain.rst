sbl::Chain
###########

User Guide
============

Reference
----------

.. cpp:function:: size_t sbl::Chain::size() const noexcept

   Returns the number of elements in the container.
   返回容器中元素个数

.. cpp:function:: void sbl::Chain::replace(size_t left, size_t right, T value)

   :param left: left bound index 左边界
   :param right: right bound index 右边界
   :param value: given value 给定值
   :Complexity: Logarithmic in the size of the container.
   :Precondition: left < right. 
   :时间复杂度: 容器内元素个数的对数。
   :前至条件: left < right. 

   Replaces all elements in the range [first, last) with another value.
   将一个范围中所有元素替换成给定元素。

.. cpp:function:: void sbl::Chain::reverse(size_t left, size_t right)

   :param left: left bound index 左边界
   :param right: right bound index 右边界
   :Complexity: Logarithmic in the size of the container.
   :Precondition: left < right. 
   :时间复杂度: 容器内元素个数的对数。
   :前至条件: left < right. 

   Reverses the order of the elements in the range [left, right). 
   将一个范围中所有元素的顺序反转。

.. cpp:function:: size_t sbl::Chain::max_element(size_t left, size_t right) const

   :param left: left bound index 左边界
   :param right: right bound index 右边界
   :return: index of maximum element in the range [left, right)
   :Complexity: Logarithmic in the size of the container.
   :Precondition: left < right. 
   :时间复杂度: 容器内元素个数的对数。
   :前至条件: left < right. 

   Finds the greatest element in the range [first, last)
   寻找范围内的最大元素的下标。

.. cpp:function:: T sbl::Chain::top(size_t left, size_t right) const

   :param left: left bound index 左边界
   :param right: right bound index 右边界
   :return: maximum element in the range [left, right)
   :Complexity: Logarithmic in the size of the container.
   :Precondition: left < right. 
   :时间复杂度: 容器内元素个数的对数。
   :前至条件: left < right. 

   Similar to :cpp:func:`sbl::Chain::max_element`\ ,
   but return the value, not the index.
   和 :cpp:func:`sbl::Chain::max_element` 类似，
   但是返回的不是下标，而是最大值。
   
.. cpp:function:: T sbl::Chain::max_sum(size_t left, size_t right) const

   :param left: left bound index 左边界
   :param right: right bound index 右边界
   :return: maximum sum of sequence element
   :Complexity: Logarithmic in the size of the container.
   :Precondition: left < right. 
   :时间复杂度: 容器内元素个数的对数。
   :前至条件: left < right. 

   Finding the contiguous subarray within the range [left, right) which has the
   largest sum. Similar to Maximum subarray problem.
   求出区间中和最大的连续子序列，类似于最大子序列问题。
   
.. cpp:function:: size_t sbl::Chain::lcp(size_t first, size_t second) const

   :param first: first index 第一个下标
   :param second: second index 第二个下标
   :return: length of longest common prefix
   :Complexity: square of logarithmic in the size of the container.
   :Precondition: left < right. 
   :Postcondition: maximum ret 
                   satisfy [first, first+ret) equal to [second, second+ret)
   :时间复杂度: 容器内元素个数的对数。
   :前至条件: left < right. 
   :后置条件: 最大的ret满足 [first, first+ret) 等于 [second, second+ret)

   Finding the lengtho of longest common prefix start with first and second
   index.
   求出从first和second开始的最长公共前缀的长度。

.. cpp:function:: T sbl::Chain::sum(size_t left, size_t right) const

   :param left: left bound index 左边界
   :param right: right bound index 右边界
   :return: sum of elements in the range [left, right)
   :Complexity: Logarithmic in the size of the container.
   :Precondition: left < right. 
   :时间复杂度: 容器内元素个数的对数。
   :前至条件: left < right. 

   Computes the sum of the elements in the range [left, right). 
   对一个范围中所有元素求和。
