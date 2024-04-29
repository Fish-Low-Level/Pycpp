#include "iterable.hpp"
#include "array.hpp"
#include "private_hashtable.hpp"

namespace pycpp
{
	class __Array;
	class __HashTable;
	class Deque;

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

	

	class Deque final : public Container, public Arguments
	{
		friend __Array;
		friend __HashTable;

		__DLNode* head;
		__DLNode* tail;
		u32 len;

	public:
		TYPE_ID(Deque);

	#pragma region Deque

		#pragma region Constructors & Destructor

		Deque();
		Deque(std::initializer_list<Object*> list);
		Deque(Deque& source);
		Deque(Iterable* iterable);

		virtual ~Deque();

		#pragma endregion

		#pragma region Inline Methods

		inline Object* get_left(u32 index);
		inline Object* get_right(u32 index);

		#pragma endregion

		#pragma region Override Object

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string& out) override;
		u64 hash() override;
		u32 bytes() override;
		Object* copy() override;

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

		#pragma region Standard Methods



		#pragma endregion

		#pragma region Private Methods

		void multiply_with_int(u32 multiplier);

		void extend_from_array(__Array* source);
		void extend_from_hashtable(__HashTable* source);
		void extend_from_iterable(Iterable* iterable);

		#pragma endregion

	#pragma endregion

	#pragma region Iterator



	#pragma endregion

	};

#pragma endregion

}