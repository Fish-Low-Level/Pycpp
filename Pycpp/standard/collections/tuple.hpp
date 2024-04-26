#pragma once

#include "array.hpp"

namespace pycpp
{

#pragma region Declaration : Tuple

	class Tuple final : public Iterable, public Arguments, public __Array
	{

	#pragma region Tuple
		const u64 hash_value;

	public:
		TYPE_ID(Tuple);

		#pragma region Constructors & Destructors

		Tuple();
		Tuple(Iterable*);
		Tuple(const Tuple&);
		Tuple(std::initializer_list<Object*>);

		~Tuple() = default;

		#pragma endregion

		#pragma region Override : Object

		i64 type() override;
		const char* class_name() override;
		u64 hash() override;
		u32 bytes() override;
		void output() override;

		bool eq(Object*) override;

		#pragma endregion

		#pragma region Override : Iterable

		bool is_empty() override;
		u32 size() override;

		Iterator* first() override;
		Iterator* last() override;

		#pragma endregion

		#pragma region Override : Arguments

		Object* get(i32 index) override;
		bool contains(Object*) override;

		#pragma endregion


	#pragma endregion

	};

#pragma endregion

}