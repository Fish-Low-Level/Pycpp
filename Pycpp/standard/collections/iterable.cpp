#include "iterable.hpp"
#include "../base/algo.hpp"
#include "../base/error.hpp"

namespace pycpp
{

#pragma region Definition : Iterator

	void Iterator::prev()
	{
		// Note : mọi class đều phải kế thừa từ Object
		auto object = dynamic_cast<Object*>(this)->class_name();
		char block_location[32];
		std::sprintf(block_location, "%s::prev", object.c_str());

		__pycpp_error_dont_support_method(block_location);
	}

#pragma endregion

#pragma region Definition : Iterable

	bool Iterable::is_empty()
	{
		Iterator* iter = first();
		bool result = iter->is_valid();

		SAFE(iter);
		return result;
	}

	u32 Iterable::size()
	{
		Iterator* iter = first();
		u32 count = 0u;

		for (; iter->is_valid(); iter->next())
			++count;

		SAFE(iter);
		return count;
	}

	Iterator* Iterable::last()
	{
		char block_location[64];
		std::sprintf(block_location, "%s::last", class_name().c_str());

		__pycpp_error_dont_support_method(block_location);
		return nullptr;
	}

#pragma endregion

#pragma region Definition : Arguments

	u64 Arguments::hash()
	{
		u64 base = 0b1110'1101'1011'0111ull;
		auto len = size();
		if (len == 0u)
			return base;

		u64 hash_value = 0u;
		u64 seed, hash_item;
		for (u32 i = 0; i < len; ++i)
		{
			seed = hash_value;
			hash_item = get(i)->hash();

			seed ^= hash_item + base + (seed >> 2);
			hash_value = seed;
		}

		return hash_value;
	}


	bool Arguments::contains(Object* elem)
	{
		bool result = false;

		u32 arguments_len = size();
		Object* cur = nullptr;
		for (u32 i = 0u; i < arguments_len; ++i)
		{
			cur = get(i);
			if (elem->equal(cur))
			{
				result = true;
				break;
			}
		}

		SAFE(elem);
		return result;
	}


	bool Arguments::equal(Arguments* other)
	{
		if (this == other)
			return true;

		auto __size = size();
		if (__size != other->size())
		{
			SAFE(other);
			return false;
		}

		for (u32 i = 0u; i < __size; ++i)
		{
			auto a = get(i);
			auto b = other->get(i);

			if (a->equal(b) == false)
			{
				SAFE(other);
				return false;
			}
		}

		SAFE(other);
		return true;
	}

	i32 Arguments::rich_compare(Arguments* other)
	{
		if (this == other)
			return 0;

		auto __len = size();
		auto __other_len = other->size();
		auto __min_len = pycpp::min(__len, __other_len);
		i32 result = 0;

		for (u32 i = 0u; i < __min_len; ++i)
		{
			auto a = get(i);
			auto b = other->get(i);
			result += a->rich_compare(b);

			if (result != 0)
			{
				SAFE(other);
				return result;
			}
		}

		return __len - __other_len;
	}

#pragma endregion

#pragma region Definition : Container

	void Container::extend(Iterable* iterable)
	{
		auto iter = iterable->first();

		Object* cur = nullptr;
		for (; iter->is_valid(); iter->next())
		{
			cur = iter->get();
			push(cur);
		}

		SAFE(iter);
		SAFE(iterable);
	}

#pragma endregion

}