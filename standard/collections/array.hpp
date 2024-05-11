#pragma once

#include "../base/object.hpp"
#include "../base/algo.hpp"
#include "iterable.hpp"
#include "arrange.hpp"
#include "private_hashtable.hpp"
#include "deque.hpp"
#include "map.hpp"

namespace pycpp
{
	class __Array;
	class __HashTable;
	class Deque;
	class OrderedMap;

#pragma region Declaration : Private Array

	class __Array
	{
		friend __HashTable;
		friend Deque;
		friend OrderedMap;

	#pragma region __Array

	protected:
		Object** data;
		u32 capacity;
		u32 len;

		#pragma region Constructor & Destructor

		__Array();
		__Array(std::initializer_list<Object*>);

		__Array(const __Array*);
		__Array(Iterable*);

		virtual ~__Array();

		#pragma endregion

		#pragma region Initialize Methods

		void init_with_capacity(u32 n_capacity);
		void init_from_array(const __Array*);
		void init_from_hashtable(const __HashTable*);

		#pragma endregion

		#pragma region Inline Methods

		inline void grow_when_empty();
		inline void shrink_to_empty();

		// Tìm "capacity" hợp lệ với "n_len"
		static inline u32 find_capacity(u32 n_len)
		{
			// "capacity" phù hợp có dạng {2^k - 1}
			// và có giá trị nhỏ nhất sao cho "capacity" >= "n_len"

			u32 next_power_of_2 = pycpp::find_next_power_of_two(n_len);
			return next_power_of_2 - 1u;
		}

		#pragma endregion

		#pragma region Allocation

		// Tăng dung lượng lên "n_capacity".
		// Điều kiện : n_capacity >= len.
		void grow_with_capacity(u32 n_capacity);

		// Tăng dung lượng.
		void grow();

		// Điều kiện : "index" hợp lệ.
		void grow_and_insert(u32 index, Object* object);

		// Giảm dung lượng.
		void shrink();

		// Điều kiện : "index" hợp lệ.
		Object* shrink_and_remove(u32 index);

		#pragma endregion

		#pragma region Standard Methods

		Iterator* first();
		Iterator* last();

		void output();
		void output(std::string& out);
		void clear();
		void reverse();
		void sort();
		void shrink_to_fit();
		bool contains(Object*);


		Object* get(i32 index);
		void assign(i32 index, Object*);
		void multi_assign(Range*, Iterable*);
		i32 find(Object*);

		void push(Object*);
		void insert(i32 index, Object*);
		Object* pop(i32 index);
		void remove(i32 index);
		
		void extend(Iterable*);

		#pragma endregion

		#pragma region Private Methods

		// Hàm chỉ so sánh hai Array
		bool equal(__Array*);

		// Hàm chỉ so sánh hai Array
		i32 rich_compare(__Array*);

		void multiply_with_int(u32 multiplier);

		void extend_from_iterable(Iterable*);
		void extend_from_array(const __Array* source);
		void extend_from_hashtable(const __HashTable* source);

		#pragma endregion

	#pragma endregion

	public:

	#pragma region Iterator

		class iterator final : public Iterator, public Object
		{
			__Array* source;
			i32 index;

			friend __Array;
			iterator(__Array* source, i32 index);
			~iterator();

		public:
			TYPE_ID(__Array::iterator);

			i64 type() override;
			std::string class_name() override;
			u32 bytes() override;

			bool is_valid() override;
			void next() override;
			void prev() override;
			Object* get() override;

		};

	#pragma endregion

	};

#pragma endregion

}