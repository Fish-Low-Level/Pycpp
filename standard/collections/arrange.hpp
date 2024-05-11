#pragma once

#include "iterable.hpp"
#include "../base/primitive.hpp"

namespace pycpp
{

#pragma region Declaration : Range

	class Range final : public Iterable
	{
	public:
		class iterator;

	private:

	#pragma region Range

		i32 _start, _step, _end;

	public:
		TYPE_ID(Range);

		#pragma region Constructors & Destructors

		Range(i32 end);
		Range(i32 start, i32 end, i32 step = 1);
		Range(const Range& source) = default;

		~Range() = default;

		#pragma endregion

		#pragma region Standard Methods

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		u64 hash() override;
		u32 bytes() override;
		Object* copy() override;

		Iterator* first() override;
		Iterator* last() override;

		#pragma endregion

		#pragma region C++ Standard Methods

		iterator begin();
		iterator end();
		iterator rbegin();
		iterator rend();

		#pragma endregion

		#pragma region Private Methods

		i32 end_item();
		i32 last_item();

		#pragma endregion

	#pragma endregion

	#pragma region Iterator

		class iterator final : public Iterator, public Object
		{
			i32 current, start, step, end;
			I32* cur;

		public:
			TYPE_ID(Range::iterator);

			iterator(i32 current, i32 start, i32 end, i32 step);
			~iterator();
			iterator(const iterator&) = default;

			//==========     Methods Object     ==========//

			i64 type() override;
			std::string class_name() override;
			void output() override;
			void output(std::string&) override;
			u64 hash() override;
			u32 bytes() override;

			//==========     Standard Methods     ==========//

			bool is_valid() override;
			void next() override;
			void prev() override;
			Object* get() override;

			//==========     C++ Standard Methods     ==========//

			i32 operator*();
			void operator++();
			void operator--();
			bool operator!=(const iterator& other);

		};

	#pragma endregion

	};

#pragma endregion

}