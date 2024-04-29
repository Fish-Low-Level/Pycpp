#include "array.hpp"
#include "../base/error.hpp"
#include "../base/algo.hpp"

namespace pycpp
{

#pragma region Definition : Private Array

	#pragma region __Array

		#pragma region Constructors & Destructors

	__Array::__Array() : data(nullptr), capacity(0u), len(0u) {}

	__Array::__Array(std::initializer_list<Object*> list)
	{
		len = (u32)list.size();
		u32 best_choice_of_capacity = __Array::find_capacity(len);
		init_with_capacity(len);

		auto head = list.begin();
		for (u32 i = 0u; i < len; ++i)
		{
			INREF(head[i]);
			data[i] = head[i];
		}
	}

	__Array::__Array(const __Array* source) : __Array()
	{
		init_from_array(source);	// Don't need SAFE(source)
	}

	__Array::__Array(Iterable* iterable)
	{
		// Don't need to SAFE(iterable).

		// Try cast to __Array
		auto __array = dynamic_cast<__Array*>(iterable);
		if (__array)
		{
			init_from_array(__array);
			return;
		}

		// Try cast to __HashTable
		auto __hashtable = dynamic_cast<__HashTable*>(iterable);
		if (__hashtable)
		{
			init_from_hashtable(__hashtable);
			return;
		}

		extend_from_iterable(iterable);
	}

	__Array::~__Array()
	{
		if (data == nullptr)
			return;

		for (u32 i = 0u; i < len; ++i)
		{
			DEREF(data[i]);
			SAFE(data[i]);
		}

		delete[] data;
	}

		#pragma endregion

		#pragma region Initialize Methods

	void __Array::init_with_capacity(u32 n_capacity)
	{
		// Before using this methods, this should be an empty :
		// data = nullptr | capacity = 0 | size = 0

		capacity = n_capacity;
		data = new Object * [capacity];
	}

	void __Array::init_from_array(const __Array* source)
	{
		// Before using this methods, this should be an empty :
		// data = nullptr | capacity = 0 | size = 0

		if (source->len == 0u)
			return;

		init_with_capacity(source->len);
		len = source->len;

		for (u32 i = 0u; i < len; ++i)
		{
			data[i] = source->data[i];
			INREF(data[i]);
		}

		SAFE(source);
	}

	void __Array::init_from_hashtable(const __HashTable* __hashtable)
	{
		// Before using this methods, this should be an empty :
		// data = nullptr | capacity = 0 | size = 0

		if (__hashtable->len == 0u)
			return;

		len = __hashtable->len;
		init_with_capacity(__Array::find_capacity(len));

		Object* cur = nullptr;
		for (u32 i = 0u; i < len; ++i)
		{
			cur = __hashtable->data[i]->ref;

			if (cur != nullptr)
			{
				data[i] = cur;
				INREF(data[i]);
			}
		}

		SAFE(__hashtable);
	}

		#pragma endregion

		#pragma region Inline Methods

	inline void __Array::grow_when_empty()
	{
		data = new Object * [1]{ nullptr };
		capacity = 1u;
	}

	inline void __Array::shrink_to_empty()
	{
		if (data[0] != nullptr)
		{
			DEREF(data[0]);
			SAFE(data[0]);
		}

		delete[] data;
		data = nullptr;
		capacity = len = 0u;
	}

		#pragma endregion

		#pragma region Allocation

	void __Array::grow()
	{
		// Case 1 : array is empty
		if (capacity == 0u)
		{
			grow_when_empty();
			return;
		}

		// Case 2 : capacity is capacity * 2 + 1
		auto tmp = data;
		capacity = capacity << 1u | 1u;
		data = new Object * [capacity];

		// Move & free
		for (u32 i = 0u; i < len; ++i)
			data[i] = tmp[i];
		delete[] tmp;
	}

	void __Array::grow_with_capacity(u32 n_capacity)
	{
		auto tmp = data;
		capacity = n_capacity;
		data = new Object * [capacity];

		// Move & free
		for (u32 i = 0u; i < len; ++i)
			data[i] = tmp[i];
		delete[] tmp;
	}

	void __Array::grow_and_insert(u32 index, Object* object)
	{
		INREF(object);

		// Case 1 : array is empty
		if (capacity == 0u)
		{
			grow_when_empty();
			data[0] = object;
			return;
		}

		// Case 2 : capacity is capacity * 2 + 1
		auto tmp = data;
		capacity = capacity << 1u | 0x1u;
		data = new Object * [capacity];

		// Move & free
		for (u32 i = 0u; i < index; ++i)
			data[i] = tmp[i];

		for (u32 i = index; i < len; ++i)
			data[i + 1] = tmp[i];

		data[index] = object;
		++len;
		delete[] tmp;
	}

	void __Array::shrink()
	{
		// Case 1
		if (capacity == 1u)
		{
			shrink_to_empty();
			return;
		}

		// Case 2 : capacity is capacity / 2
		auto tmp = data;
		capacity >>= 1;
		data = new Object * [capacity];

		// Move & free
		for (u32 i = 0u; i < len; ++i)
			data[i] = tmp[i];
		delete[] tmp;
	}

	Object* __Array::shrink_and_remove(u32 index)
	{
		auto deleting = data[index];
		data[index] = nullptr;
		DEREF(deleting);
		
		// Case 1
		if (capacity == 1u)
		{
			shrink_to_empty();
			return deleting;
		}

		// Case 2 : capacity is capacity / 2
		auto tmp = data;
		capacity >>= 1;
		data = new Object * [capacity];

		// Move & free
		for (u32 i = 0u; i < index; ++i)
			data[i] = tmp[i];
		for (u32 i = index + 1; i < len; ++i)
			data[i - 1] = tmp[i];

		--len;
		delete[] tmp;
		return deleting;
	}

		#pragma endregion

		#pragma region Standard Methods

	Iterator* __Array::first()
	{
		return new __Array::iterator(this, 0);
	}

	Iterator* __Array::last()
	{
		return new __Array::iterator(this, len - 1u);
	}


	void __Array::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void __Array::output(std::string& out)
	{
		out.clear();
		if (len == 0u)
			return;

		bool first = true;
		Object* __this = dynamic_cast<Object*>(this);
		std::string str_cur;
		for (u32 i = 0u; i < len; ++i)
		{
			auto cur = data[i];
			if (cur == __this)
				str_cur = "this";
			else
				cur->output(out);

			if(first == false)
			str_cur = ", " + str_cur;
			first = false;

			out += str_cur;
		}
	}

	void __Array::clear()
	{
		for (u32 i = 0u; i < len; ++i)
		{
			DEREF(data[i]);
			SAFE(data[i]);
		}

		delete[] data;
		data = nullptr;
		capacity = len = 0u;
	}

	void __Array::reverse()
	{
		if (len <= 1)
			return;

		for (u32 i = 0u, j = len - 1; i < j; ++i, --j)
			pycpp::swap(data[i], data[j]);
	}

	void __Array::sort()
	{
		pycpp::shell_sort(data, len);
	}

	void __Array::shrink_to_fit()
	{
		grow_with_capacity(len);
	}

	bool __Array::contains(Object* elem)
	{
		bool result = false;
		for(u32 i = 0u; i < len; ++i)
			if (elem->equal(data[i]))
			{
				result = true;
				break;
			}

		SAFE(elem);
		return result;
	}


	Object* __Array::get(i32 index)
	{
		if (index < -i32(len) || index >= i32(len))
		{
			char block_location[32];
			std::sprintf(block_location, "%s::get(i32)",
				dynamic_cast<Object*>(this)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
			return nullptr;
		}

		if (index >= 0)
			return data[index];
		return data[index + i32(len)];
	}

	void __Array::assign(i32 index, Object* item)
	{
		if (index < -i32(len) || index >= i32(len))
		{
			char block_location[32];
			std::sprintf(block_location, "%s::assign(i32, Object)",
				dynamic_cast<Object*>(this)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
			return;
		}

		Object** ptr;
		if (index >= 0)
			ptr = data + index;
		else
			ptr = data + index + len;

		Object* deleting = *ptr;
		*ptr = item;

		DEREF(deleting);
		SAFE(deleting);
	}

	void __Array::multi_assign(Range* range, Iterable* iterable)
	{
		auto iter = iterable->first();

		Object* cur = nullptr;
		for (i32 i : *range)
		{
			if (iter->is_valid() == false)
				goto special_error;

			cur = iter->get();
			assign(i, cur);

			iter->next();
		}
	
		// Range kết thúc, nhưng Iterable vẫn có thể tiếp tục => Runtime error
		if (iter->is_valid() == true)
		{
		special_error:
			char content_error[128];
			std::sprintf(content_error,
				"Error in %s::multi_assign(Range*, Iterable*) : "
				"the number of elements of Range and Iterables must be equal !",
				dynamic_cast<Object*>(this)->class_name().c_str());
			std::cout << content_error << std::endl;

			throw std::exception("Error : the number of elements of two Iterables must be equal !");
			return;
		}

		SAFE(iter);
		SAFE(iterable);
		SAFE(range);
	}

	i32 __Array::find(Object* item)
	{
		i32 index = -1;

		for (u32 i = 0u; i < len; ++i)
			if (item->equal(data[i]))
			{
				index = i;
				break;
			}

		SAFE(item);
		return index;
	}


	void __Array::push(Object* item)
	{
		if (len == capacity)
			grow();

		INREF(item);
		data[len++] = item;
	}

	void __Array::insert(i32 index, Object* item)
	{
		// Error : out of bound
		if (index < -i32(len) || index >= i32(len))
		{
			char block_location[64];
			std::sprintf(block_location,
				"%s::insert", dynamic_cast<Object*>(this)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
		}

		if (index < 0)
			index += len;

		if (len == capacity)
		{
			// Don't need INREF(item)
			grow_and_insert(index, item);
			return;
		}

		INREF(item);
		for (u32 i = len; i > u32(index); --i)
			data[i] = data[i - 1u];
		data[index] = item;
		++len;
	}

	Object* __Array::pop(i32 index)
	{
		// Error : out of bound
		if (index < -i32(len) || index >= i32(len))
		{
			char block_location[64];
			std::sprintf(block_location,
				"%s::pop", dynamic_cast<Object*>(this)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
		}

		if (index < 0)
			index += len;

		if (len - 1u <= (capacity >> 1))
			return shrink_and_remove(index);
		
		auto deleting = data[index];
		DEREF(deleting);

		// Move
		for (u32 i = index + 1; i < len; ++i)
			data[i - 1] = data[i];

		--len;
		return deleting;
	}

	void __Array::remove(i32 index)
	{
		// Error : out of bound
		if (index < -i32(len) || index >= i32(len))
		{
			char block_location[64];
			std::sprintf(block_location,
				"%s::remove", dynamic_cast<Object*>(this)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
		}

		if (index < 0)
			index += len;

		if (len - 1u <= (capacity >> 1))
		{
			auto deleting = shrink_and_remove(index);
			SAFE(deleting);
			return;
		}

		auto deleting = data[index];
		DEREF(deleting);
		SAFE(deleting);

		// Move
		for (u32 i = index + 1; i < len; ++i)
			data[i - 1] = data[i];
		--len;
	}

	void __Array::extend(Iterable* iterable)
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


		#pragma endregion

		#pragma region Private Methods

	bool __Array::equal(__Array* other)
	{
		if (this == other)
			return true;

		if (len != other->len)
		{
			SAFE(other);
			return false;
		}

		for (u32 i = 0u; i < len; ++i)
		{
			auto a = data[i];
			auto b = other->data[i];

			if (a->equal(b) == false)
			{
				SAFE(other);
				return false;
			}
		}

		SAFE(other);
		return true;
	}

	i32 __Array::rich_compare(__Array* other)
	{
		if (this == other)
			return 0;

		auto min_len = pycpp::min(len, other->len);
		i32 result = 0;

		for (u32 i = 0u; i < min_len; ++i)
		{
			auto a = data[i];
			auto b = other->data[i];

			result += a->rich_compare(b);
			if (result != 0)
				break;
		}

		if(result == 0)
			result += len - other->len;
		return result;
	}


	void __Array::multiply_with_int(u32 multiplier)
	{
		if (len == 0u || multiplier == 1u)
			return;

		if (multiplier == 0u)
		{
			clear();
			return;
		}

		u32 n_capacity = __Array::find_capacity(len * multiplier);
		grow_with_capacity(n_capacity);

		for (u32 i = 1u; i < multiplier; ++i)
		{
			u32 begin = i * len;

			for (u32 j = 0u; j < len; ++j)
				data[begin + j] = data[j];
		}

		len *= multiplier;
	}

	void __Array::extend_from_iterable(Iterable* iterable)
	{
		Object* cur = nullptr;
		auto iter = iterable->first();

		for (; iter->is_valid(); iter->next())
		{
			if (len == capacity)
				grow();

			cur = iter->get();
			INREF(cur);
			data[len++] = cur;
		}

		SAFE(iter);
		SAFE(iterable);
	}

	void __Array::extend_from_array(const __Array* other)
	{
		// Duplicate
		if (this == other)
		{
			multiply_with_int(2u);
			return;
		}

		// "other" rỗng
		if (other->len == 0u)
		{
			SAFE(other);
			return;
		}

		// Growing
		u32 n_len = len + other->len;
		u32 n_capacity = __Array::find_capacity(n_len);

		if (n_capacity != capacity)
			grow_with_capacity(n_capacity);

		// Moving
		Object* cur = nullptr;
		for (u32 i = len; i < n_len; ++i)
		{
			cur = other->data[i];
			INREF(cur);
			data[i] = cur;
		}

		len = n_len;
		SAFE(other);
	}

	void __Array::extend_from_hashtable(const __HashTable* source)
	{
		// TODO
	}

		#pragma endregion

	#pragma endregion

	#pragma region Iterator

	__Array::iterator::iterator(__Array* source, i32 index) :
		source(source), index(index)
	{
		INREF(source);
	}

	__Array::iterator::~iterator()
	{
		DEREF(source);
		SAFE(source);
	}


	i64 __Array::iterator::type()
	{
		return __Array::iterator::type_id;
	}

	std::string __Array::iterator::class_name()
	{
		return "__Array::iterator";
	}

	u32 __Array::iterator::bytes()
	{
		return sizeof(__Array::iterator);
	}


	bool __Array::iterator::is_valid()
	{
		return index >= 0 && index < i32(source->len);
	}

	void __Array::iterator::next()
	{
		if (index >= i32(source->len))
		{
			char block_location[64];
			std::sprintf(block_location, "%s::iterator::next",
				dynamic_cast<Object*>(source)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
			return;
		}

		++index;
	}

	void __Array::iterator::prev()
	{
		if (index < 0)
		{
			char block_location[64];
			std::sprintf(block_location, "%s::iterator::prev",
				dynamic_cast<Object*>(source)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
			return;
		}

		--index;
	}

	Object* __Array::iterator::get()
	{
		if (is_valid() == false)
		{
			char block_location[64];
			std::sprintf(block_location, "%s::iterator::get",
				dynamic_cast<Object*>(source)->class_name().c_str());

			__pycpp_error_out_of_bound_error(block_location);
			return nullptr;
		}

		return source->data[index];
	}


	#pragma endregion

#pragma endregion

}