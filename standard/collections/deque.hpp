#pragma once

#include "iterable.hpp"
#include "array.hpp"
#include "private_hashtable.hpp"
#include "map.hpp"

namespace pycpp
{
	class __Array;
	class __HashTable;
	class OrderedMap;

#pragma region Declaration : Node

	struct __DLNode
	{
		Object* ref;
		__DLNode* next;
		__DLNode* prev;

		__DLNode() = delete;

		inline __DLNode(Object* reference) :
			ref(reference), next(nullptr), prev(nullptr)
		{
			INREF(ref);
		}

		inline Object* pop()
		{
			auto result = ref;
			DEREF(ref);
			ref = nullptr;
			return result;
		}

		~__DLNode()
		{
			if (ref)
			{
				DEREF(ref);
				SAFE(ref);
			}
		}
	};

#pragma endregion

#pragma region Declaration : Deque

	class Deque final : public List
	{
		friend __Array;
		friend __HashTable;
		friend OrderedMap;

		__DLNode* head;
		__DLNode* tail;
		u32 len;

	public:
		TYPE_ID(Deque);

		class iterator;
		friend iterator;

	#pragma region Deque

		#pragma region Constructors & Destructor

		Deque();
		Deque(std::initializer_list<Object*> list);
		Deque(Deque& source);
		Deque(Iterable* iterable);

		virtual ~Deque();

		#pragma endregion

		#pragma region Inline Methods

		inline __DLNode* get_left(u32 index);
		inline __DLNode* get_right(u32 index);

		#pragma endregion

		#pragma region Override Object

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string& out) override;
		u64 hash() override;
		u32 bytes() override;
		Object* copy() override;

		bool equal(Object* other) override;
		i32 rich_compare(Object* other) override;

		Object* add(Object* other) override;
		Object* mul(Object* other) override;

		#pragma endregion

		#pragma region Override Iterable

		Iterator* first() override;
		Iterator* last() override;

		#pragma endregion

		#pragma region Override Container

		// Deque trở thành rỗng.
		void clear() override;

		// Trả về "true" nếu "item" có trong Deque.
		bool contains(Object* item) override;

		// Thêm phần tử vào cuối Deque, luôn trả về "true".
		bool push(Object* item) override;

		// Xóa và trả về phần tử ở cuối.
		Object* pop() override;

		// Thêm các phần tử trong Iterable vào Deque.
		void extend(Iterable*) override;

		// Thêm các phần tử trong {Object ..} vào Deque.
		// void extend(std::initializer_list<Object*> list) override;

		#pragma endregion

		#pragma region Override Arguments

		bool is_empty() override;
		u32 size() override;
		Object* get(i32 index) override;

		bool equal(Arguments* arguments) override;
		i32 rich_compare(Arguments* arguments) override;

		#pragma endregion

		#pragma region Override List

		// Xem dung lượng của Deque.
		// Hàm trả về tương tự hàm "size()".
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

		#pragma region Standard Methods



		#pragma endregion

		#pragma region Private Methods
	private:

		u64 hash_value();

		// So sánh Deque với Deque.
		bool equal_with_deque(Deque* source);
		// So sánh Deque với __Array.
		bool equal_with_array(__Array* source);
		// So sánh Deque với Arguments.
		// (!) : chỉ gọi hàm của Arguments.
		bool equal_with_arguments(Arguments* arguments);

		// So sánh Deque với Deque.
		i32 rich_compare_with_deque(Deque* source);
		// So sánh Deque với __Array.
		i32 rich_compare_with_array(__Array* source);
		// So sánh Deque với Arguments.
		// (!) : chỉ gọi hàm của Arguments.
		i32 rich_compare_with_arguments(Arguments* arguments);

		void multiply_with_int(u32 multiplier);

		void extend_from_array(__Array* source);
		void extend_from_hashtable(__HashTable* source);
		void extend_from_iterable(Iterable* iterable);

		static void insertion_sort(__DLNode* head, __DLNode* tail);
		static void merge_sort(__DLNode*& head, __DLNode*& tail, u32 number);
		static void merge(__DLNode*& head_a, __DLNode* tail_a, __DLNode* head_b, __DLNode*& tail_b);

		#pragma endregion

	#pragma endregion

	#pragma region Iterator
	public:
		class iterator final : public Iterator, public Object
		{
			friend Deque;

			Deque* container;
			__DLNode* node;

			iterator(Deque* container, __DLNode* node);
		public:
			~iterator();

			TYPE_ID(Deque::iterator);

			//==========     Override Object     ==========//

			i64 type() override;
			std::string class_name() override;
			u32 bytes() override;

			//==========     Override Iterator     ==========//

			bool is_valid() override;
			void next() override;
			void prev() override;
			Object* get() override;

		};

	#pragma endregion

	};

#pragma endregion

}