#include "algo.hpp"

namespace pycpp
{

#pragma region Supported Array

	void binary_insertion_sort(Object** __array, u32 len)
	{
		if (len <= 1)
			return;

		i32 location;
		Object* selected;

		for (u32 i = 1u; i < len; ++i)
		{
			selected = __array[i];
			location = upper_bound(__array, i, selected);

			for (u32 j = i - 1u; j >= location; --j)
				__array[j + 1] = __array[j];
			__array[location] = selected;
		}
	}

	void shell_sort(Object** __array, u32 len)
	{
		if (len <= 1)
			return;

		// Tìm khoảng
		u32 interval = 1u;
		while (interval < len / 3)
			interval = interval * 3 + 1;

		// Shell
		for (; interval > 1; interval = (interval - 1u) / 3u)
		{
			for (u32 outer = interval; outer < len; ++outer)
			{
				Object* value_to_insert = __array[outer];
				u32 inner = outer;

				// Move to right
				while (inner >= interval && __array[inner - interval]->rich_compare(value_to_insert) > 0)
				{
					__array[inner] = __array[inner - interval];
					inner -= interval;
				}

				__array[inner] = value_to_insert;
			}
		}
		
		// Insertion sort
		binary_insertion_sort(__array, len);
	}

	u64 hash_array(Object** __array, u32 len)
	{
		u64 base = 0b1110'1101'1011'0111ull;

		if (len == 0u)
			return base;

		u64 hash_value = 0u;
		u64 seed, hash_item;
		for (u32 i = 0; i < len; ++i)
		{
			seed = hash_value;
			hash_item = __array[i]->hash();

			seed ^= hash_item + base + (seed >> 2);
			hash_value = seed;
		}

		return hash_value;
	}

#pragma endregion

}