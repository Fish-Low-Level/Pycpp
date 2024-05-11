#pragma once

#include <initializer_list>

#include "object.hpp"
#include "../collections/map.hpp"

namespace pycpp
{

	class Map;

#pragma region Declaration : Pair

	class Pair final : public Object
	{
		friend Map;

		Object* const __key;
		Object* __value;

	public:
		TYPE_ID(Pair);

#pragma region Constructors & Destructor

		Pair();
		Pair(Object* key);
		Pair(Object* key, Object* value);

		~Pair();

#pragma endregion

#pragma region Override : Object

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		u32 bytes() override;
		u64 hash() override;
		Object* copy() override;

		bool equal(Object*) override;
		i32 rich_compare(Object*) override;

#pragma endregion

#pragma region Standard Methods

		void set(Object* value);
		Object* key();
		Object* value();

#pragma endregion

	};

#pragma endregion

}