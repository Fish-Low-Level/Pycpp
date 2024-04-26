#include "algo.hpp"

namespace pycpp
{

#pragma region Supported Array

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