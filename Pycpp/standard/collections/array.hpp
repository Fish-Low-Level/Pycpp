#pragma once

#include "../base/object.hpp"
#include "../base/algo.hpp"
#include "iterable.hpp"
#include "arrange.hpp"
#include "private_hashtable.hpp"
#include "deque.hpp"

namespace pycpp
{
	class __Array;
	class __HashTable;
	class Deque;

#pragma region Declaration : Private Array

	/*
	Tổng quan về Array :
		- Lưu trữ một dãy phần tử liên tiếp nhau được đánh số bắt đầu từ 0.

		- Cấp phát động cho phù hợp với kích thước mảng,
			với dung lượng mảng `capacity`, số lượng phần tử `len`
			+ Khi mảng rỗng : data = nullptr, len = capacity = 0.
			+ Mảng với 1 phần tử : capacity = 1.
			+ Mảng tăng tiến bình thường : capacity có dạng 2^k - 1.

		- Vị trí phần tử đầu tiên là 0, vị trí phần tử cuối cùng là -1,
			khoảng hợp lệ của mảng : [-len, len)

	Table of Constructors :

	+----------------------------------------+----------------------------------------+
	| ()                                     | Khởi tạo rỗng.                         |
	+----------------------------------------+----------------------------------------+
	| (u32 n_capacity)                       | Khởi tạo mảng với dung lượng cho trước.|
	|                                        | Thường được dùng để thêm số lượng lớn  |
	|                                        | phần tử, tránh chi phí gia tăng mảng.  |
	+----------------------------------------+----------------------------------------+
	| (std::initializer_list<Object*>)       | Khởi tạo mảng từ danh sách.            |
	+----------------------------------------+----------------------------------------+
	| (const __Array*)                       | Khởi tạo mảng từ Array khác.           |
	+----------------------------------------+----------------------------------------+
	| (Iterable* iterable)                   | Khởi tạo mảng từ Iterable.             |
	|                                        | Nó sẽ cố ép sang các kiểu __Array,     |
	|                                        | __HashTable, ... nếu có thể.           |
	+----------------------------------------+----------------------------------------+
	| virtual ~__Array()                     | Hàm hủy.                               |
	+----------------------------------------+----------------------------------------+

	Table of Methods :
		Các phương thức đề cập dưới đây là thao tác cơ bản của mảng,
			những phương thức khác chỉ bổ trợ cho chúng.

	+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
	| Standard Methods                       |                                        |
	+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
	| void output() const                    | Xuất mảng.                             |
	+----------------------------------------+----------------------------------------+
	| u64 hash() const                       | Tính giá trị băm của mảng.             |
	+----------------------------------------+----------------------------------------+
	| void clear()                           | Xóa toàn bộ phần tử, mảng trở thành    |
	|                                        | rỗng : data = nullptr.                 |
	+----------------------------------------+----------------------------------------+
	| void reverse()                         | Đảo mảng.                              |
	+----------------------------------------+----------------------------------------+
	| Object* get(i32 index)                 | Trả về phần tử tại `index`.            |
	|                                        | Hàm sẽ ném lỗi runtime nếu `index`     |
	|                                        | không hợp lệ.                          |
	+----------------------------------------+----------------------------------------+
	| void set(i32 index, Object* item)      | Gán `item` tại `index`.                |
	|                                        | Hàm sẽ ném lỗi runtime nếu `index`     |
	|                                        | không hợp lệ.                          |
	+----------------------------------------+----------------------------------------+
	| void push(Object* item)                | Thêm phần tử vào cuối.                 |
	+----------------------------------------+----------------------------------------+
	| void insert(i32 index, Object* item)   | Chèn phần tử vào vị trí `index`.       |
	|                                        | `index` có thể âm.                     |
	|                                        | Hàm sẽ ném lỗi runtime nếu `index`     |
	|                                        | không hợp lệ.                          |
	+----------------------------------------+----------------------------------------+
	| Object* pop()                          | Xóa và trả về phần tử cuối.            |
	|                                        | Hàm sẽ ném lỗi runtime nếu Array rỗng. |
	+----------------------------------------+----------------------------------------+
	| Object* pop(i32 index)                 | Xóa và trả về phần tử ở `index`.       |
	|                                        | Hàm sẽ ném lỗi runtime nếu `index`     |
	|                                        | không hợp lệ.                          |
	+----------------------------------------+----------------------------------------+
	| void remove(i32 index)                 | Xóa và phần tử tử ở `index`.           |
	|                                        | Hàm sẽ ném lỗi runtime nếu `index`     |
	|                                        | không hợp lệ.                          |
	+----------------------------------------+----------------------------------------+
	| void remove_in(i32 left, i32 right)    | Xóa các phần tử trong đoạn             |
	|                                        | [left, right] ( hoặc [right, left] nếu |
	|                                        | right < left ).                        |
	|                                        | Hàm sẽ ném lỗi runtime nếu `left` hoặc |
	|                                        | `right` không hợp lệ.                  |
	+----------------------------------------+----------------------------------------+
	| void extend(Iterable*)                 | Thêm một tập phần tử từ Iterable.      |
	+----------------------------------------+----------------------------------------+
	| void shrink_to_fit()                   | Giảm dung lượng mảng khớp với số lượng |
	|                                        | phần tử.                               |
	+----------------------------------------+----------------------------------------+
	| u32 count(const Object* item)          | Đếm số phần tử bằng với `item`, bằng   |
	|                                        | cách sử dụng hàm `eq`.                 |
	+----------------------------------------+----------------------------------------+
	| bool contains(const Object* item)      | True nếu `item` tồn tại.               |
	+----------------------------------------+----------------------------------------+
	| i32 find(const Object* item)           | Tìm vị trí phần tử bằng với `item`,    |
	|                                        | bằng cách sử dụng hàm `eq`.            |
	|                                        | Nếu không tìm thấy, trả về -1.         |
	+----------------------------------------+----------------------------------------+
	| void sort()                            | Sắp xếp mảng tăng dần.                 |
	+----------------------------------------+----------------------------------------+

	*/

	class __Array
	{
		friend __HashTable;
		friend Deque;

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