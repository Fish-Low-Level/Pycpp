#include "deque.hpp"

namespace pycpp
{

#pragma region Definition : Deque

	#pragma region Deque

		#pragma region Constructors & Destructor

	Deque::Deque() : head(nullptr), tail(nullptr), len(0u) {}

	Deque::Deque(std::initializer_list<Object*> list) : len(list.size())
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

	inline Object* Deque::get_left(u32 index)
	{
		auto iter = head;
		while (index != 0)
		{
			iter = iter->next;
			--index;
		}

		return iter->ref;
	}

	inline Object* Deque::get_right(u32 index)
	{
		auto iter = tail;
		while (index != 0)
		{
			iter = iter->prev;
			--index;
		}

		return iter->ref;
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
		// TODO
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
		if (index < (len >> 1))
			return Deque::get_left(u32(index));
		return Deque::get_right(len - u32(index) - 1u);
	}

	bool Deque::equal(Arguments* arguments)
	{
		// TODO
	}

	i32 Deque::rich_compare(Arguments* arguments)
	{
		// TODO
	}

		#pragma endregion

		#pragma region Private Methods

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

		#pragma endregion

	#pragma endregion

	#pragma region Iterator



	#pragma endregion

#pragma endregion

}