#pragma once

#include "array.hpp"

namespace pycpp
{

#pragma region Declaration : Array List

	class ArrayList final : public List, public __Array
	{

	public:
		TYPE_ID(ArrayList);

	#pragma region Array List

		#pragma region Constructors & Destructors

		ArrayList() = default;
		ArrayList(std::initializer_list<Object*>);
		ArrayList(ArrayList&);
		ArrayList(Iterable*);

		~ArrayList() = default;

		#pragma endregion

		#pragma region Override Object

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		u32 bytes() override;
		u64 hash() override;

		bool equal(Object*) override;
		i32 rich_compare(Object*) override;

		// List + Iterable.
		// => "this" thêm toàn bộ phần tử từ Iterable và trả về "this".
		Object* add(Object*);

		// List * Integer ( với Integer >= 0 ).
		// => Số phần tử trong "this" gấp Integer lần và trả về "this".
		// Example : [0, 1] * 2 = [0, 1, 0, 1]
		Object* mul(Object*);

		#pragma endregion

		#pragma region Override Iterable

		Iterator* first() override;
		Iterator* last() override;

		#pragma endregion

		#pragma region Override Arguments

		bool is_empty() override;
		u32 size() override;
		Object* get(i32 index) override;

		#pragma endregion

		#pragma region Override Container

		void clear() override;
		bool contains(Object*) override;
		bool push(Object*) override;
		Object* pop() override;
		void extend(Iterable*) override;

		#pragma endregion

		#pragma region Override List

		// Xem dung lượng của List tại thời điểm gọi hàm.
		u32 capacity() override;

		// Gán mới phần tử tại vị trí "index".
		void set(i32 index, Object* item) override;

		// Chèn một phần tử vào vị trí "index".
		void insert(i32 index, Object* item) override;

		// Xóa và trả về phần tử tại vị trí "index".
		Object* pop(i32 index) override;

		// Xóa một phần tử tại vị trí "index".
		void remove(i32 index) override;

		// Sắp xếp tăng dần
		void sort() override;

		#pragma endregion

	#pragma endregion

	};

#pragma endregion

}