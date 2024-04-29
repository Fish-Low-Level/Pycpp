#pragma once

#include "object.hpp"

namespace pycpp
{

#pragma region Supported Basic Algorith

	template <typename T>
	inline void swap(T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	template <typename T>
	inline T min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <typename T>
	inline T max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	/// <summary>
	/// Trả về giá trị result = 2^k nhỏ nhất mà result > n.
	/// (1) : chỉ sử dụng cho số nguyên 32 bit.
	/// </summary>
	constexpr inline u32 find_next_power_of_two(u32 n)
	{
		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;
		return n + 1u;
	}

#pragma endregion

#pragma region Supported Array

	template <typename T>
	inline void fill(T* __array, u32 len, T fill_value)
	{
		for (u32 i = 0u; i < len; ++i)
			__array[i] = fill_value;
	}

	// Don't delete any object
	inline i32 upper_bound(Object** __array, u32 len, Object* value)
	{
		i32 left = 0, right = i32(len) - 1;
		i32 mid, result_of_rich_compare;

		while (left <= right)
		{
			mid = (left + right) >> 1;
			result_of_rich_compare = value->rich_compare(__array[mid]);

			// result_of_rich_compare >= 0 : value >= array[mid]
			if (result_of_rich_compare >= 0)
				left = mid + 1;
			else // value < array[mid]
				right = mid - 1;
		}

		// [0, 1, 1, 1, 2, 2, 2, 5, 7]
		// value = 1	=> return 4
		// value = 2	=> return 7
		// value = -1	=> return 0

		return left;
	}

	void binary_insertion_sort(Object** __array, u32 len);

	void shell_sort(Object** __array, u32 len);

	u64 hash_array(Object** __array, u32 len);

#pragma endregion



}