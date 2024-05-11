#pragma once

#include "../base/object.hpp"
#include "../base/funtor.hpp"
#include "../base/var.hpp"

namespace pycpp
{

#pragma region Declaration : Iterator

	/*
	Iterator : sử dụng để lặp qua lần lượt các phần tử trong Container.

	Table of Methods :

	+----------------------------------------+----------------------------------------+
	| bool is_valid() = 0                    | Kiểm tra Iterator có hợp lệ không.     |
	|                                        | Nếu giá trị là True, có thể gọi các    |
	|                                        | phương thức 'next', 'get' mà không ném |
	|                                        | lỗi runtime.                           |
	+----------------------------------------+----------------------------------------+
	| void next() = 0                        | Trỏ đến phần tử kế tiếp.               |
	|                                        | Cần gọi 'is_valid' trước để kiểm tra   |
	|                                        | Iterator hợp lệ.                       |
	+----------------------------------------+----------------------------------------+
	| void prev()                            | Trỏ đến phần tử trước đó.              |
	|                                        | Cần gọi 'is_valid' trước để kiểm tra   |
	|                                        | Iterator hợp lệ.                       |
	|                                        | (!) : một vài Container không thể      |
	|                                        | duyệt lùi, hàm có thể ném lỗi runtime. |
	+----------------------------------------+----------------------------------------+
	| Object* get() = 0                      | Lấy ra phần tử nó đang trỏ đến.        |
	|                                        | Cần gọi 'is_valid' trước để kiểm tra   |
	|                                        | Iterator hợp lệ.                       |
	+----------------------------------------+----------------------------------------+

	*/

	// Iterator : interface.
	class Iterator
	{
	public:
		virtual ~Iterator() = default;

		/// <summary>
		/// Kiểm tra Iterator có hợp lệ không trước khi gọi các phương thức khác.
		/// </summary>
		virtual bool is_valid() = 0;

		/// <summary>
		/// Trỏ đến phần tử kế tiếp trong Container.
		/// Sau đó cần gọi lại phương thức 'is_valid' để kiểm tra hợp lệ không.
		/// </summary>
		virtual void next() = 0;

		/// <summary>
		/// Trỏ đến phần tử đứng trước trong Container.
		/// Sau đó cần gọi lại phương thức 'is_valid' để kiểm tra hợp lệ không.
		/// Phương thức sẽ ném lỗi runtime nếu Container của nó không thể duyệt lùi.
		/// </summary>
		virtual void prev();

		/// <summary>
		/// Lấy ra phần tử nó đang trỏ đến.
		/// </summary>
		virtual Object* get() = 0;

	};

#pragma endregion

#pragma region Declaration : Iterable

	/// <summary>
	/// Iterable : public Object
	/// Đại diện cho các tập hợp liên tiếp.
	/// </summary>
	class Iterable : public Object
	{
public:
		virtual ~Iterable() = default;

		//==========     Standard Methods     ==========//

		virtual bool is_empty();
		virtual u32 size();

		virtual Iterator* first() = 0;
		virtual Iterator* last();
	};

#pragma endregion

#pragma region Declaration : Arguments

	// Arguments : interface.
	class Arguments
	{
	public:
		virtual ~Arguments() = default;

		virtual bool is_empty() = 0;
		virtual u32 size() = 0;
		virtual Object* get(i32 index) = 0;

		virtual u64 hash();

		// Trả về "true" nếu "item" có trong Container.
		virtual bool contains(Object*);

		virtual bool equal(Arguments*);
		virtual i32 rich_compare(Arguments*);
	};

#pragma endregion

#pragma region Declaration : Container

	// Container : abstract class.
	class Container : public Iterable
	{
	public:
		Container() = default;
		virtual ~Container() = default;

		///==========     Standard Methods     ==========///

		// Container trở thành rỗng.
		virtual void clear() = 0;

		// Trả về "true" nếu "item" có trong Container.
		virtual bool contains(Object* item) = 0;

		// Thêm phần tử vào Container, trả về "true" nếu thêm thành công.
		virtual bool push(Object*) = 0;

		// Lấy ra một phần tử.
		// Tùy loại Container mà nó sẽ quyết định vị trí phần tử cần lấy.
		virtual Object* pop() = 0;

		// Thêm các phần tử trong Iterable vào Container.
		virtual void extend(Iterable*);

		// Thêm các phần tử trong {Object ..} vào Container.
		// virtual void extend(std::initializer_list<Object*> list);
	};

#pragma endregion

#pragma region Declaration : List

	// List : abstract class.
	class List : public Container, public Arguments
	{
	public:
		virtual ~List() = default;

		///==========     Standard Methods     ==========///

		// Xem dung lượng của List tại thời điểm gọi hàm.
		virtual u32 capacity() = 0;

		// Gán mới phần tử tại vị trí "index".
		virtual void set(i32 index, Object* item) = 0;

		// Chèn một phần tử vào vị trí "index".
		virtual void insert(i32 index, Object* item) = 0;

		// Xóa và trả về phần tử tại vị trí "index".
		virtual Object* pop(i32 index) = 0;

		// Xóa một phần tử tại vị trí "index".
		virtual void remove(i32 index) = 0;

		// Sắp xếp tăng dần
		virtual void sort() = 0;

		///==========     Override Object     ==========///

		/*
		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string& out) override;
		u32 bytes() override;
		u64 hash() override;

		virtual bool eq(Object*) override;
		virtual bool lt(Object*) override;
		virtual bool lte(Object*) override;

		virtual Object* add(Object*) override;
		virtual Object* mul(Object*) override;
		*/

		///==========     Override Iterable     ==========///

		/*
		virtual Iterator* first() override;
		virtual Iterator* last() override;
		*/

		///==========     Override Container     ==========///

		/*
		virtual void clear() override;
		virtual bool push(Object*) override;
		virtual Object* pop() override;
		virtual void extend(Iterable*) override;
		*/

	};

#pragma endregion

#pragma region Declaration : Set

	class Set : public Container
	{
	public:
		virtual ~Set() = default;

		///==========     Standard Methods     ==========///

		// Lấy ra phần tử trong Set.
		// Có thể sử dụng Set kết hợp Pair cho ra một Dictionary.
		virtual Object* get(Object* elem) = 0;

		//// Trả về "true" nếu Set là tập con của Iterable.
		//virtual bool is_subset(Iterable* iterable) = 0;

		//// Trả về "true" nếu Iterable là tập con của Set.
		//virtual bool is_superset(Iterable* iterable) = 0;

		///==========     Override Object     ==========///

		/*
		virtual i64 type() override;
		virtual const char* class_name() override;
		virtual u32 bytes() override;
		virtual u64 hash() override;
		virtual void output() override;

		virtual bool eq(Object*) override;

		virtual Object* add(Object*) override;
		*/

		///==========     Override Iterable     ==========///

		/*
		virtual Iterator* first() override;
		virtual Iterator* last() override;
		*/

		///==========     Override Container     ==========///

		/*
		virtual void clear() override;
		virtual bool push(Object*) override;
		virtual Object* pop() override;
		virtual void extend(Iterable*) override;
		*/

	};

#pragma endregion

#pragma region Declaration : Dictionary

	class Dictionary : public Set
	{

	public:

		///==========     Standard Methods     ==========///

		// Gán "value" vào "key" tương ứng.
		// Nếu "key" chưa tồn tại, thêm cặp "key-value" vào Dictionary.
		// Nếu "key" đã tồn tại, thay thế "value" mới của nó.
		virtual void set(Object* key, Object* value) = 0;

		// Xóa và trả về Pair "key-value" ra khỏi Dictionary.
		virtual Object* pop(Object* key) = 0;

		// Xóa "key-value" ra khỏi Dictionary.
		virtual void remove(Object* key) = 0;

		// Duyệt qua các "key".
		virtual Iterable* keys() = 0;

		// Duyệt qua các "value".
		virtual Iterable* values() = 0;

		// Duyệt qua các cặp "key-value", mỗi cặp nằm trong một Pair.
		virtual Iterable* items() = 0;

		///==========     Override Container     ==========///

		// Trả về "true" nếu "key" có trong Dictionary.
		virtual bool contains(Object* key) = 0;




		///==========     Override Set     ==========///

		// Lấy ra "value" tương ứng với "key".
		virtual Object* get(Object* key) = 0;

		// Lấy ra phần tử ngẫu nhiên hoặc có thứ tự ( tùy cách triển khai ) trong Dictionary.
		virtual Object* pop() = 0;

	};

#pragma endregion

}