#pragma once

#include "../base/object.hpp"
#include "iterable.hpp"
#include "array.hpp"
#include "deque.hpp"

#include <initializer_list>

namespace pycpp
{
	class __Array;
	class __HashTable;
	class Deque;

#pragma region Declaration : struct Entry

	struct Entry final
	{
		Object* ref;
		u64 hash_value;
		Entry* next;

		Entry(Object* ref);
		Entry(Object* ref, Entry* pointer);
		Entry(Object* ref, u64 hash_value);
		Entry(Object* ref, u64 hash_value, Entry* pointer);
		Entry(Entry&);
		Entry(Entry&, Entry* pointer);

		Entry(Entry&&) = delete;
		
		~Entry();

		// Trả về Object và giảm tham chiếu, xóa Object khỏi Entry.
		// (!) : chỉ gọi hàm khi Entry còn tham chiếu đến Object.
		inline Object* pop();

	};

#pragma endregion

#pragma region Declaration : Private HashTable

	class __HashTable
	{
		friend __Array;
		friend Deque;

	public:
		class iterator;

	#pragma region Private HashTable

	protected:
		Entry** data;
		u32 capacity;
		u32 len;

		#pragma region Constructors & Destructors

		__HashTable();
		__HashTable(std::initializer_list<Object*>);
		__HashTable(__HashTable&);
		__HashTable(Iterable*);

		virtual ~__HashTable();

		#pragma endregion

		#pragma region Inline Methods

		inline u32 index_hashing(u64 hash_value);

		inline u32 find_capacity(u32 n_len);


		// Tìm vị trí Entry đầu tiên có phần tử.
		// Nếu không tìm thấy, trả về -1.
		inline i32 first_entry();

		// Tìm đến Entry kế tiếp có phần tử ngay sau vị trí "prev_entry".
		// Nếu không tìm thấy, trả về -1.
		inline i32 next_entry(u32 prev_entry);

		// Tìm vị trí Entry cuối cùng có phần tử.
		// Nếu không tìm thấy, trả về -1.
		inline i32 last_entry();


		// Kiểm tra trong một danh sách liên kết các nút Entry, có tồn tại "item" trong đó hay không.
		// (!) : hàm không giải phóng bất kì Object nào
		inline bool exist_in_entry(Entry* head, Object* item, u64 hash_value);


		// Tăng dung lượng mảng lên capacity = 3.
		inline void grow_when_empty();

		// Giảm dung lượng mảng về 0.
		// Yêu cầu : capacity = 3, và len = 0 hoặc len = 1.
		inline void shrink_to_empty();


		// Mở rộng từ danh sách.
		// (!) : hàm không thay đổi mảng băm
		inline void only_extend(std::initializer_list<Object*>);

		#pragma endregion

		#pragma region Allocation

		// Tăng dung lượng mảng
		void grow();

		// Giảm dung lượng mảng
		void shrink();

		// Thay đổi dung lượng mảng băm phù hợp với "n_len"
		// (!) : hàm không xóa phần tử, "n_len" == 0 không hợp lệ
		void resize_based_on_len(u32 n_len);

		#pragma endregion

		#pragma region Standard Methods

		void clear();
		void output();

		Iterator* first();
		Iterator* last();

		// Lấy ra phần tử trong HashTable.
		// (!) : hàm ném lỗi runtime nếu "elem" không tồn tại.
		Object* get(Object* elem);

		bool contains(Object* elem);

		bool push(Object*);
		Object* pop();
		

		void extend(Iterable*);
		void extend(std::initializer_list<Object*>);

		#pragma endregion

		#pragma region Private Methods

		// Liên kết toàn bộ Entry, trả về nút đầu của danh sách.
		// (!) : trả về nullptr nếu "this" rỗng.
		Entry* connect_entry();

		// Tái cấu trúc danh sách liên kết Entry.
		void restructure(Entry* head);

		bool contains(Object* elem, u64 hash_value);

		bool push(Object*, u64 hash_value);


		void extend_from_array(__Array* source);
		void extend_from_hashtable(__HashTable* source);
		void extend_from_iterable(Iterable* iterable);

		#pragma endregion

	#pragma endregion

	public:

	#pragma region Iterator

		class iterator final : public Iterator, public Object
		{
			friend __HashTable;

			__HashTable* container;
			i32 entry_index;
			Entry* entry;

			iterator(__HashTable* container, i32 entry_index, bool first=true);
		public:
			TYPE_ID(__HashTable::iterator);

			///==========     Override Object     ==========///

			i64 type() override;
			std::string class_name() override;
			u32 bytes() override;

			///==========     Override Iterator     ==========///

			bool is_valid() override;
			void next() override;
			Object* get() override;

		};

	#pragma endregion

	};

#pragma endregion

}