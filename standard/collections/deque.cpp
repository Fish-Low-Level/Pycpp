#include "deque.hpp"

// #define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

namespace pycpp
{

#pragma region Definition : Deque

	#pragma region Deque

		#pragma region Constructors & Destructor

	Deque::Deque() : head(nullptr), tail(nullptr), len(0u) {}

	Deque::Deque(std::initializer_list<Object*> list) :
		len(u32(list.size()))
	{
		if (len == 0u)
		{
			head = tail = nullptr;
			return;
		}

		auto head_list = list.begin();

		// First node
		head = tail = new __DLNode(head_list[0]);

		for (u32 i = 1u; i < len; ++i)
		{
			tail->next = new __DLNode(head_list[i]);
			tail = tail->next;
		}
	}

	Deque::Deque(Deque& source) : len(source.len)
	{
		if (len == 0u)
		{
			head = tail = nullptr;
			return;
		}

		auto head_source = source.head;
		// First node
		head = tail = new __DLNode(head_source->ref);
		head_source = head_source->next;

		for (; head_source; head_source = head_source->next)
		{
			tail->next = new __DLNode(head->ref);
			tail = tail->next;
		}

	}

	Deque::Deque(Iterable* iterable) : len(0u)
	{
		auto iter = iterable->first();
		if (iter->is_valid() == false)
		{
			head = tail = nullptr;

			SAFE(iter);
			SAFE(iterable);
			return;
		}

		// First node
		head = tail = new __DLNode(iter->get());
		iter->next();
		++len;

		for (; iter->is_valid(); iter->next())
		{
			tail->next = new __DLNode(iter->get());
			tail = tail->next;
			++len;
		}

		SAFE(iter);
		SAFE(iterable);
	}

	Deque::~Deque()
	{
		clear();
	}

		#pragma endregion

		#pragma region Inline Methods

	inline __DLNode* Deque::get_left(u32 index)
	{
		auto iter = head;
		while (index != 0)
		{
			iter = iter->next;
			--index;
		}

		return iter;
	}

	inline __DLNode* Deque::get_right(u32 index)
	{
		auto iter = tail;
		while (index != 0)
		{
			iter = iter->prev;
			--index;
		}

		return iter;
	}

		#pragma endregion

		#pragma region Override Object

	i64 Deque::type()
	{
		return Deque::type_id;
	}

	std::string Deque::class_name()
	{
		return "Deque";
	}

	void Deque::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void Deque::output(std::string& out)
	{
		if (len == 0u)
		{
			out = "[]";
			return;
		}

		out = "[";

		bool first = true;
		std::string str_current;
		for (auto iter = head; iter; iter = iter->next)
		{
			str_current.clear();

			if (this == iter->ref)
				str_current = "this";
			else
				iter->ref->output(str_current);

			if (first == false)
				str_current += " ~ ";
			first = false;

			out += str_current;
		}

		out += "]";
	}

	u64 Deque::hash()
	{
		return hash_value();
	}

	u32 Deque::bytes()
	{
		u32 size_of_this = sizeof(Deque);
		u32 size_of_nodes = sizeof(__DLNode) * len;

		u32 size_of_elements = 0u;
		for (auto iter = head; iter; iter = iter->next)
			size_of_elements += iter->ref->bytes();

		return size_of_this + size_of_nodes + size_of_elements;
	}

	Object* Deque::copy()
	{
		return new Deque(*this);
	}


	bool Deque::equal(Object* other)
	{
		auto arguments = dynamic_cast<Arguments*>(other);
		if (arguments == nullptr)
			return false;

		return equal(arguments);
	}

	i32 Deque::rich_compare(Object* other)
	{
		auto arguments = dynamic_cast<Arguments*>(other);
		if (arguments == nullptr)
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in Deque::rich_compare : "
				"the compare operator cannot be used to compare Deque with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error: comparing Deque with a different data type other than Arguments");

			return 0;
		}

		return rich_compare(arguments);
	}


	Object* Deque::add(Object* other)
	{
		auto iterable = dynamic_cast<Iterable*>(other);
		if (iterable == nullptr)
		{
			char content_error[128];
			std::sprintf(content_error,
				"Error in Deque::add : the add operator cannot be used"
				"to add Deque with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : cannot concatenate a Deque with an Object that is not Iterable!");

			return nullptr;
		}

		Deque::extend(iterable);
		// Don't need to SAFE "iterable"
		return this;
	}

	Object* Deque::mul(Object* other)
	{
		i64 other_type = other->type();
		i32 num;

		switch (other_type)
		{
		case I32::type_id:
			num = dynamic_cast<I32*>(other)->value;
			break;

		case I64::type_id:
			num = (i32) dynamic_cast<I64*>(other)->value;
			break;

		default:
		{
			char content_error[128];
			std::sprintf(content_error,
				"Error in Deque::mul : the mul operator cannot be used"
				"to multiply Deque with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}
		}

		// Error : multiplying Deque with by a negative number
		if (num < 0)
		{
			char content_error[128];
			std::sprintf(content_error,
				"Error in Deque::mul : the mul operator cannot be used"
				"to multiply Deque with negative number of %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : multiplying a list by a negative number");
		}

		Deque::multiply_with_int(u32(num));

		SAFE(other);
		return this;
	}

		#pragma endregion

		#pragma region Override Iterable

	Iterator* Deque::first()
	{
		return new Deque::iterator(this, head);
	}

	Iterator* Deque::last()
	{
		return new Deque::iterator(this, tail);
	}

		#pragma endregion

		#pragma region Override Container

	void Deque::clear()
	{
		if (len == 0u)
			return;

		for (tail = head; tail; tail = head)
		{
			head = head->next;
			delete tail;
		}

		head = tail = nullptr;
		len = 0u;
	}

	bool Deque::contains(Object* item)
	{
		auto cur = head;
		for (; cur; cur = cur->next)
			if (item->equal(cur->ref))
				break;

		SAFE(item);
		return cur != nullptr;
	}

	bool Deque::push(Object* item)
	{
		if (len == 0u)
		{
			head = tail = new __DLNode(item);
			len = 1u;
			return true;
		}

		tail->next = new __DLNode(item);
		++len;
		return true;
	}

	Object* Deque::pop()
	{
		if (len == 0u)
		{
			__pycpp_error_retrieve_from_empty_container("Deque::pop", "Deque");
			return nullptr;
		}

		if (len == 1u)
		{
			auto result = head->pop();
			delete head;
			head = tail = nullptr;
			len = 0u;
			return result;
		}

		// Tách nút cuối
		auto tail_node = tail;
		tail = tail->prev;
		tail->next = nullptr;

		// Tách tham chiếu
		auto result = tail_node->pop();
		delete tail_node;
		--len;

		return result;
	}

	void Deque::extend(Iterable* iterable)
	{
		// Don't need SAFE(iterable)

		// Try to cast "iterable" to "__Array"
		auto __array = dynamic_cast<__Array*>(iterable);
		if (__array)
		{
			extend_from_array(__array);
			return;
		}

		// Try to cast "iterable" to "__Array"
		auto __hashtable = dynamic_cast<__HashTable*>(iterable);
		if (__hashtable)
		{
			extend_from_hashtable(__hashtable);
			return;
		}

		extend_from_iterable(iterable);
	}

	// Thêm các phần tử trong {Object ..} vào Deque.
	// void Deque::extend(std::initializer_list<Object*> list) override;

		#pragma endregion

		#pragma region Override Arguments

	bool Deque::is_empty()
	{
		return len == 0u;
	}

	u32 Deque::size()
	{
		return len;
	}

	Object* Deque::get(i32 index)
	{
		if (index < -i32(len) || index >= i32(len))
		{
			__pycpp_error_out_of_bound("Deque::get(i32 index)");
			return nullptr;
		}

		if (index < 0)
			index += i32(len);

		// Phần tử thuộc nửa trái
		if (u32(index) < (len >> 1))
			return Deque::get_left(u32(index))->ref;
		return Deque::get_right(len - u32(index) - 1u)->ref;
	}

	bool Deque::equal(Arguments* arguments)
	{
		auto deque = dynamic_cast<Deque*>(arguments);
		if (deque)
			return equal_with_deque(deque);

		auto __array = dynamic_cast<__Array*>(arguments);
		if (__array)
			return equal_with_array(__array);

		return equal_with_arguments(arguments);
	}

	i32 Deque::rich_compare(Arguments* arguments)
	{
		auto deque = dynamic_cast<Deque*>(arguments);
		if (deque)
			return rich_compare_with_deque(deque);

		auto __array = dynamic_cast<__Array*>(arguments);
		if (__array)
			return rich_compare_with_array(__array);

		return rich_compare_with_arguments(arguments);
	}

		#pragma endregion

		#pragma region Override List

	u32 Deque::capacity()
	{
		return len;
	}

	void Deque::set(i32 index, Object* item)
	{
		if (index < -i32(len) || index >= i32(len))
		{
			__pycpp_error_out_of_bound("ArrayList::set(i32 index, Object item)");
			return;
		}

		if (index < 0)
			index += len;

		auto selected_node = (index < (len >> 1)) ?
			get_left(index) : get_right(len - u32(index) - 1u);
		auto deleting = selected_node->pop();
		DEREF(deleting);
		SAFE(deleting);

		INREF(item);
		selected_node->ref = item;
	}

	void Deque::insert(i32 index, Object* item)
	{
		// Error : out of bound
		if (index < -i32(len) || index > i32(len))
		{
			__pycpp_error_out_of_bound("Deque::insert(i32 index, Object item)");
			return;
		}

		if (index < 0)
			index += len;

		auto element_node = new __DLNode(item);
		++len;

		// Insert head
		if (index == 0)
		{
			element_node->next = head;
			head = element_node;
			return;
		}

		// Insert tail->next
		if (index == i32(len))
		{
			tail->next = element_node;
			tail = element_node;
			return;
		}

		auto specified_node = (u32(index) < (len >> 1)) ?
			get_left(u32(index)) : get_right(len - u32(index) - 1u);
		auto previous_node = specified_node->prev;

		// Linking
		previous_node->next = element_node;
		element_node->prev = previous_node;
		element_node->next = specified_node;
		specified_node->prev = element_node;
	}

	Object* Deque::pop(i32 index)
	{
		// Error : out of bound
		if (index < -i32(len) || index >= i32(len))
		{
			__pycpp_error_out_of_bound("Deque::pop(i32 index)");
			return nullptr;
		}

		if (index < 0)
			index += len;

		__DLNode* delete_node = nullptr;
		Object* result = nullptr;
		--len;
		
		if (len == 1u)	// Pop and Deque becomes empty
		{
			delete_node = head;
			head = tail = nullptr;
		}
		else if (index == 0)	// Pop head
		{
			delete_node = head;
			head = head->next;
		}
		else if (u32(index) == len - 1)	// Pop tail
		{
			delete_node = tail;
			tail = tail->prev;
			tail->next = nullptr;
		}
		else
		{
			delete_node = (u32(index) < (len >> 1)) ?
				get_left(u32(index)) : get_right(len - u32(index) - 1u);
			auto previous_node = delete_node->prev;
			auto next_node = delete_node->next;

			// Remove "delete_node"
			previous_node->next = next_node;
			next_node->prev = previous_node;
		}

		result = delete_node->pop();
		delete delete_node;
		return result;
	}

	void Deque::remove(i32 index)
	{
		// Error : out of bound
		if (index < -i32(len) || index >= i32(len))
		{
			__pycpp_error_out_of_bound("Deque::remove(i32 index)");
			return;
		}

		if (index < 0)
			index += len;

		__DLNode* delete_node = nullptr;
		--len;

		if (len == 1u)	// Pop and Deque becomes empty
		{
			delete_node = head;
			head = tail = nullptr;
		}
		else if (index == 0)	// Pop head
		{
			delete_node = head;
			head = head->next;
		}
		else if (u32(index) == len - 1)	// Pop tail
		{
			delete_node = tail;
			tail = tail->prev;
			tail->next = nullptr;
		}
		else
		{
			delete_node = (u32(index) < (len >> 1)) ?
				get_left(u32(index)) : get_right(len - u32(index) - 1u);
			auto previous_node = delete_node->prev;
			auto next_node = delete_node->next;

			// Remove "delete_node"
			previous_node->next = next_node;
			next_node->prev = previous_node;
		}

		delete delete_node;
	}

	void Deque::sort()
	{
		Deque::merge_sort(head, tail, len);
	}

		#pragma endregion

		#pragma region Private Methods

	u64 Deque::hash_value()
	{
		u64 base = 0b1110'1101'1011'0111ull;

		if (len == 0u)
			return base;

		u64 hash_value = 0u;
		u64 seed, hash_item;
		for (auto i = head; i; i = i->next)
		{
			seed = hash_value;
			hash_item = i->ref->hash();

			seed ^= hash_item + base + (seed >> 2);
			hash_value = seed;
		}

		return hash_value;
	}


	bool Deque::equal_with_deque(Deque* source)
	{
		if (this == source)
			return true;

		if (len != source->len)
		{
			SAFE(source);
			return false;
		}

		auto iter = head;
		auto iter_source = source->head;
		while (iter)
		{
			if (iter->ref->equal(iter_source->ref) == false)
			{
				SAFE(source);
				return false;
			}

			iter = iter->next;
			iter_source = iter_source->next;
		}

		SAFE(source);
		return true;
	}

	bool Deque::equal_with_array(__Array* source)
	{
		if (len != source->len)
		{
			SAFE(source);
			return false;
		}

		auto iter_node = head;
		u32 i = 0u;
		while (iter_node)
		{
			if (iter_node->ref->equal(source->data[i]) == false)
			{
				SAFE(source);
				return false;
			}

			iter_node = iter_node->next;
			++i;
		}

		SAFE(source);
		return true;
	}

	bool Deque::equal_with_arguments(Arguments* arguments)
	{
		if (len != arguments->size())
		{
			SAFE(arguments);
			return false;
		}

		auto iter_node = head;
		u32 i = 0u;
		while (iter_node)
		{
			if (iter_node->ref->equal(arguments->get(i)) == false)
			{
				SAFE(arguments);
				return false;
			}

			iter_node = iter_node->next;
			++i;
		}

		SAFE(arguments);
		return true;
	}

	i32 Deque::rich_compare_with_deque(Deque* source)
	{
		if (this == source)
			return 0;

		auto iter = head;
		auto iter_source = source->head;
		i32 result = 0;
		while (iter && iter_source)
		{
			result += iter->ref->rich_compare(iter_source->ref);
			if (result != 0)
			{
				SAFE(source);
				return result;
			}

			iter = iter->next;
			iter_source = iter_source->next;
		}

		i32 rich_compare_len = i32(len) - i32(source->len);
		SAFE(source);
		return result + rich_compare_len;

	}

	i32 Deque::rich_compare_with_array(__Array* source)
	{
		auto iter = head;
		u32 i = 0u;
		i32 result = 0;

		while (iter)
		{
			result += iter->ref->rich_compare(source->data[i]);
			if (result != 0)
			{
				SAFE(source);
				return result;
			}

			iter = iter->next;
			++i;
		}

		i32 rich_compare_len = i32(len) - i32(source->len);
		SAFE(source);
		return result + rich_compare_len;
	}

	i32 Deque::rich_compare_with_arguments(Arguments* arguments)
	{
		auto iter = head;
		u32 i = 0u;
		i32 result = 0;

		while (iter)
		{
			result += iter->ref->rich_compare(arguments->get(i));
			if (result != 0)
			{
				SAFE(arguments);
				return result;
			}

			iter = iter->next;
			++i;
		}

		i32 rich_compare_len = i32(len) - i32(arguments->size());
		SAFE(arguments);
		return result + rich_compare_len;
	}



	void Deque::multiply_with_int(u32 multiplier)
	{
		if (len == 0u || multiplier == 1u)
			return;

		if (multiplier == 0u)
		{
			clear();
			return;
		}

		__DLNode* new_tail = tail;
		for (; multiplier != 1u; --multiplier)
			for (auto current = head; current != tail->next; current = current->next)
			{
				new_tail->next = new __DLNode(current->ref);
				new_tail = new_tail->next;
			}

		tail = new_tail;
		len *= multiplier;
	}


	void Deque::extend_from_array(__Array* source)
	{
		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		__DLNode tmp(NONE);
		if (len == 0u)
			head = tail = &tmp;

		for (u32 i = 0u; i < source->len; ++i)
		{
			tail->next = new __DLNode(source->data[i]);
			tail = tail->next;
		}

		if (len == 0u)
			head = head->next;

		len += source->len;
		SAFE(source);
	}

	void Deque::extend_from_hashtable(__HashTable* source)
	{
		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		__DLNode tmp(NONE);
		if (len == 0u)
			head = tail = &tmp;

		for (u32 i = 0u; i < source->capacity; ++i)
		{
			auto head = source->data[i];
			if (head == nullptr)
				continue;

			for (; head; head = head->next)
			{
				tail->next = new __DLNode(head->ref);
				tail = tail->next;
			}
		}

		if (len == 0u)
			head = head->next;

		len += source->len;
		SAFE(source);
	}

	void Deque::extend_from_iterable(Iterable* iterable)
	{
		auto iter = iterable->first();
		if (iter->is_valid() == false)
		{
			SAFE(iter);
			SAFE(iterable);
			return;
		}

		__DLNode tmp(NONE);
		if (len == 0u)
			head = tail = &tmp;

		for (; iter->is_valid(); iter->next())
		{
			tail->next = new __DLNode(iter->get());
			tail = tail->next;
			++len;
		}

		if (len == 0u)
			head = head->next;

		SAFE(iter);
		SAFE(iterable);
	}


	void Deque::insertion_sort(__DLNode* head, __DLNode* tail)
	{
		if (head == tail)
			return;

		for (auto i = head->next; i; i = i->next)
		{
			auto remember = i->ref;
			auto j = i->prev;

			while (j && j->ref->rich_compare(remember) > 0)
			{
				j->next->ref = j->ref;
				j = j->prev;
			}

			if (j == nullptr)
				head->ref = remember;
			else
				j->next->ref = remember;
		}
	}

	void Deque::merge_sort(__DLNode*& head, __DLNode*& tail, u32 number)
	{
		if (number <= 32)
		{
			insertion_sort(head, tail);
			return;
		}

		// Divide
		u32 mid = number >> 1u;
		u32 len_left = mid;
		u32 len_right = number - len_left;

		auto tail_a = head;
		for (; mid != 1u; --mid)
			tail_a = tail_a->next;

		auto head_b = tail_a->next;

		// Sort left - right
		merge_sort(head, tail_a, len_left);
		merge_sort(head_b, tail, len_right);

		// Merge
		merge(head, tail_a, head_b, tail);
	}

	void Deque::merge(__DLNode*& head_a, __DLNode* tail_a, __DLNode* head_b, __DLNode*& tail_b)
	{
		if (tail_a->ref->rich_compare(head_b->ref) <= 0)
		{
			tail_a->next = head_b;
			head_b->prev = tail_a;
			return;
		}

		__DLNode tmp(NONE);
		__DLNode* new_head = &tmp;
		__DLNode* new_tail = &tmp;
		__DLNode* current = nullptr;

		while (head_a && head_b)
		{
			if (head_a->ref->rich_compare(head_b->ref) <= 0)
			{
				current = head_a;
				head_a = head_a->next;
			}
			else
			{
				current = head_b;
				head_b = head_b->next;
			}

			new_tail->next = current;
			current->prev = new_tail;
			new_tail = new_tail->next;
		}

		if (head_a)
		{
			new_tail->next = head_a;
			head_a->prev = new_tail;
			new_tail = tail_a;
		}
		else
		{
			new_tail->next = head_b;
			head_b->prev = new_tail;
			new_tail = tail_b;
		}

		new_head = new_head->next;	// Bỏ qua nút "tmp"
		new_head->prev = nullptr;
		new_tail->next = nullptr;
		head_a = new_head;
		tail_b = new_tail;
	}

		#pragma endregion

	#pragma endregion

	#pragma region Iterator

	Deque::iterator::iterator(Deque* container, __DLNode* node) :
		container(container), node(node)
	{
		INREF(container);
	}

	Deque::iterator::~iterator()
	{
		DEREF(container);
		SAFE(container);
	}


	i64 Deque::iterator::type()
	{
		return Deque::iterator::type_id;
	}

	std::string Deque::iterator::class_name()
	{
		return "Deque::iterator";
	}

	u32 Deque::iterator::bytes()
	{
		return sizeof(Deque::iterator);
	}


	bool Deque::iterator::is_valid()
	{
		return node != nullptr;
	}

	void Deque::iterator::next()
	{
		if (node == nullptr)
		{
			__pycpp_error_out_of_bound("Deque::iterator::next()");
			return;
		}

		node = node->next;
	}

	void Deque::iterator::prev()
	{
		if (node == nullptr)
		{
			__pycpp_error_out_of_bound("Deque::iterator::next()");
			return;
		}

		node = node->prev;
	}

	Object* Deque::iterator::get()
	{
		return node->ref;
	}

	#pragma endregion

#pragma endregion

}