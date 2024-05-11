#include "private_hashtable.hpp"

namespace pycpp
{

#pragma region Definition : Entry

	Entry::Entry(Object* ref) :
		ref(ref), hash_value(ref->hash()), next(nullptr)
	{
		INREF(ref);
	}

	Entry::Entry(Object* ref, Entry* pointer) :
		ref(ref), hash_value(ref->hash()), next(pointer)
	{
		INREF(ref);
	}

	Entry::Entry(Object* ref, u64 hash_value) :
		ref(ref), hash_value(hash_value), next(nullptr)
	{
		INREF(ref);
	}

	Entry::Entry(Object* ref, u64 hash_value, Entry* pointer) :
		ref(ref), hash_value(hash_value), next(pointer)
	{
		INREF(ref);
	}

	Entry::Entry(Entry& source) :
		ref(source.ref), hash_value(source.hash_value), next(nullptr)
	{
		INREF(ref);
	}

	Entry::Entry(Entry& source, Entry* pointer) :
		ref(source.ref), hash_value(source.hash_value), next(pointer)
	{
		INREF(ref);
	}

	Entry::~Entry()
	{
		if (ref)
		{
			DEREF(ref);
			SAFE(ref);
		}
	}


	inline Object* Entry::pop()
	{
		DEREF(ref);

		auto result = ref;
		ref = nullptr;
		return result;
	}

#pragma endregion

#pragma region Definition : Private HashTable

	#pragma region Private HashTable

		#pragma region Constructors & Destructors

	__HashTable::__HashTable() :
		data(nullptr), capacity(0u), len(0u) {}

	__HashTable::__HashTable(std::initializer_list<Object*> list)
	{
		capacity = find_capacity((u32) list.size());
		data = new Entry * [capacity];
		pycpp::fill<Entry*>(data, capacity, nullptr);

		only_extend(list);	// Hàm có gán "len"
	}

	__HashTable::__HashTable(__HashTable& source)
	{
		extend_from_hashtable(&source);
	}

	__HashTable::__HashTable(Iterable* iterable)
	{
		auto __array = dynamic_cast<__Array*>(iterable);
		if (__array)
		{
			extend_from_array(__array);
			return;
		}

		auto __hashtable = dynamic_cast<__HashTable*>(iterable);
		if (__hashtable)
		{
			extend_from_hashtable(__hashtable);
			return;
		}

		extend_from_iterable(iterable);

	}

	__HashTable::~__HashTable()
	{
		clear();
	}

		#pragma endregion

		#pragma region Inline Methods

	inline u32 __HashTable::index_hashing(u64 hash_value)
	{
		return (hash_value & capacity) - 1u;
	}

	inline u32 __HashTable::find_capacity(u32 n_len)
	{
		auto power_of_2 = pycpp::find_next_power_of_two(n_len);
		return (power_of_2 << 1u) - 1u;
	}


	inline i32 __HashTable::first_entry()
	{
		u32 i = 0u;
		for (; i < capacity; ++i)
			if (data[i] != nullptr)
				return i;
		return -1;
	}

	inline i32 __HashTable::next_entry(u32 prev_entry)
	{
		u32 i = prev_entry + 1u;
		for (; i < capacity; ++i)
			if (data[i] != nullptr)
				return i;
		return -1;
	}

	inline i32 __HashTable::last_entry()
	{
		i32 i = capacity - 1;
		for (; i >= 0; --i)
			if (data[i] != nullptr)
				return i;
		return -1;
	}


	inline bool __HashTable::exist_in_entry(Entry* head, Object* item, u64 hash_value)
	{
		for (; head; head = head->next)
			if (hash_value == head->hash_value &&
				item->equal(head->ref))
				break;

		return head != nullptr;
	}


	inline void __HashTable::grow_when_empty()
	{
		data = new Entry * [3]{ nullptr, nullptr, nullptr };
		capacity = 3;
	}

	inline void __HashTable::shrink_to_empty()
	{
		Entry* elem = nullptr;
		if (data[0] != nullptr)
			elem = data[0];
		else if (data[1] != nullptr)
			elem = data[1];
		else if (data[2] != nullptr)
			elem = data[2];

		if (elem != nullptr)
		{
			// Giải phóng phần tử
			DEREF(elem->ref);
			SAFE(elem->ref);

			// Giải phóng nút
			delete elem;
		}

		delete[] data;
		data = nullptr;
		capacity = 0u;
	}


	inline void __HashTable::only_extend(std::initializer_list<Object*> list)
	{
		u32 list_size = (u32) list.size();
		auto iter = list.begin();

		// Duyệt "list"
		for (u32 i = 0u; i < list_size; ++i)
		{
			// Tìm đến Entry
			auto item = iter[i];
			u64 hash_value = item->hash();
			auto& entry_head = data[index_hashing(hash_value)];

			// Kiểm tra tồn tại
			if (exist_in_entry(entry_head, item, hash_value))
			{
				SAFE(item);
				continue;
			}

			// Tạo nút Entry
			++len;
			auto entry = new Entry(iter[i], hash_value);

			if (entry_head == nullptr)
				entry_head = entry;
			else {
				entry->next = entry_head;
				entry_head = entry;
			}
		}
	}

		#pragma endregion

		#pragma region Allocation

	void __HashTable::grow()
	{
		if (len == 0u)
		{
			grow_when_empty();
			return;
		}

		auto head = connect_entry();

		// Tăng dung lượng
		delete[] data;

		capacity = capacity << 1u | 1u;
		data = new Entry * [capacity];
		pycpp::fill<Entry*>(data, capacity, nullptr);

		// Tái cấu trúc
		restructure(head);
	}

	void __HashTable::shrink()
	{
		if (capacity == 3u)
		{
			shrink_to_empty();
			return;
		}

		auto head = connect_entry();

		// Giảm dung lượng
		delete[] data;

		capacity >>= 1u;
		data = new Entry * [capacity];
		pycpp::fill<Entry*>(data, capacity, nullptr);

		// Tái cấu trúc
		restructure(head);
	}

	void __HashTable::resize_based_on_len(u32 n_len)
	{
		auto n_capacity = find_capacity(n_len);
		if (capacity == n_capacity)
			return;

		Entry* head = nullptr;
		if (data != nullptr && len != 0u)
		{
			head = connect_entry();
			delete[] data;
		}

		// Tạo mảng băm phù hợp
		capacity = n_capacity;
		data = new Entry * [capacity];
		pycpp::fill<Entry*>(data, capacity, nullptr);

		// Tái cấu trúc
		if(head)
			restructure(head);
	}

		#pragma endregion

		#pragma region Standard Methods

	void __HashTable::clear()
	{
		if (data == nullptr)
			return;

		for (u32 i = 0u; i < capacity; ++i)
		{
			auto head = data[i];
			if (head == nullptr)
				continue;

			for (; head;)
			{
				// Tách nút đầu
				auto cur = head;
				head = head->next;

				// Giải phóng Entry
				delete cur;
			}
		}

		delete[] data;
		data = nullptr;
		capacity = len = 0u;
	}

	void __HashTable::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void __HashTable::output(std::string& out)
	{
		out.clear();
		if (len == 0u)
			return;

		auto this_object = dynamic_cast<Object*>(this);
		bool first_print = true;
		auto __entry_index = first_entry();
		auto head = data[__entry_index];

		std::string str_current;
		while (true)
		{
			// "head" duyệt hết Entry
			for (; head; head = head->next)
			{
				if (this_object == head->ref)
					str_current = "this";
				else
					head->ref->output(str_current);

				if (first_print == false)
					str_current = ", " + str_current;
				first_print = false;

				out += str_current;
			}

			__entry_index = next_entry(__entry_index);
			if (__entry_index == -1)
				break;

			// Nhảy tới Entry kế
			head = data[__entry_index];
		}
	}

	Iterator* __HashTable::first()
	{
		if (len == 0)
			return new __HashTable::iterator(this, 0);

		return new __HashTable::iterator(this, first_entry(), true);
	}

	Iterator* __HashTable::last()
	{
		if (len == 0)
			return new __HashTable::iterator(this, -1);

		return new __HashTable::iterator(this, last_entry(), false);
	}

	Object* __HashTable::get(Object* elem)
	{
		u64 hash_value = elem->hash();
		Object* result = nullptr;

		// Tìm "elem"
		auto head = data[index_hashing(hash_value)];
		for (; head; head = head->next)
			if (hash_value == head->hash_value &&
				elem->equal(head->ref))
			{
				result = head->ref;
				break;
			}

		// Runtime error
		if (result == nullptr)
		{
			auto __class_name = dynamic_cast<Object*>(this)->class_name();
			char content_error[128];
			sprintf_s(content_error,
				"Error in %s::get(Object) : unable to get an element"
				" in a %s when it does not exist !",
				__class_name.c_str(), __class_name.c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : trying to retrieve an object that does not exist in the Set");
			return nullptr;
		}

		// Không cần kiểm tra "elem" trùng địa chỉ với "result"
		SAFE(elem);	
		return result;
	}

	bool __HashTable::contains(Object* elem)
	{
		return contains(elem, elem->hash());
	}


	bool __HashTable::push(Object* item)
	{
		return push(item, item->hash());
	}

	Object* __HashTable::pop()
	{
		// Error : retrieve from an empty Container
		if (len == 0u)
		{
			auto object = dynamic_cast<Object*>(this);
			char block_location[32];
			sprintf_s(block_location, "%s::pop()",
				object->class_name().c_str());

			__pycpp_error_retrieve_from_empty_container(
				block_location, object->class_name().c_str());
			return nullptr;
		}

		i32 index = first_entry();
		Entry* entry = data[index];
		
		// Tách Entry khỏi "this"
		data[index] = entry->next;
		--len;

		// Giảm dung lượng nếu có thể
		if (len <= (capacity >> 2u))
			shrink();

		// Giải phóng Entry
		auto result = entry->pop();
		delete entry;
		return result;
	}


	void __HashTable::extend(Iterable* iterable)
	{
		auto __array = dynamic_cast<__Array*>(iterable);
		if (__array)
		{
			extend_from_array(__array);
			return;
		}

		auto __hashtable = dynamic_cast<__HashTable*>(iterable);
		if (__hashtable)
		{
			extend_from_hashtable(__hashtable);
			return;
		}

		extend_from_iterable(iterable);
	}

	void __HashTable::extend(std::initializer_list<Object*> list)
	{
		u32 list_size = (u32) list.size();
		if (list_size == 0u)
			return;

		// Tăng dung lượng nếu có thể
		resize_based_on_len(len + list_size);

		only_extend(list);
	}

		#pragma endregion

		#pragma region Private Methods

	Entry* __HashTable::connect_entry()
	{
		if (len == 0u)
			return nullptr;

		auto __entry_index = first_entry();
		auto head = data[__entry_index];
		auto cur = head;

		// Nối các nút
		while (true)
		{
			__entry_index = next_entry(__entry_index);
			if (__entry_index == -1)
				break;

			// "cur" đi đến nút cuối của "head"
			while (cur->next)
				cur = cur->next;

			// Nối
			cur->next = data[__entry_index];
		}

		return head;
	}

	void __HashTable::restructure(Entry* head)
	{
		Entry* cur = nullptr;
		u32 index;

		for (; head != nullptr;)
		{
			// Tách nút đầu khỏi danh sách
			cur = head;
			head = head->next;
			cur->next = nullptr;

			// Băm
			index = index_hashing(cur->hash_value);

			if (data[index] == nullptr)
				data[index] = cur;
			else {
				cur->next = data[index];
				data[index] = cur;
			}
		}
	}


	bool __HashTable::contains(Object* elem, u64 hash_value)
	{
		if (len == 0u)
		{
			SAFE(elem);
			return false;
		}

		auto* head = data[index_hashing(hash_value)];
		bool result = exist_in_entry(head, elem, hash_value);

		SAFE(elem);
		return result;
	}


	bool __HashTable::push(Object* item, u64 hash_value)
	{
		// Nếu "this" rỗng
		if (len == 0u)
		{
			grow_when_empty();
			data[index_hashing(hash_value)] = new Entry(item, hash_value);
			len = 1u;
			return true;
		}

		auto& head = data[index_hashing(hash_value)];
		if (exist_in_entry(head, item, hash_value))
		{
			SAFE(item);
			return false;
		}

		// Thêm Entry vào mảng băm
		if (head == nullptr)
			head = new Entry(item, hash_value);
		else
			head = new Entry(item, hash_value, head);
		++len;

		// Tăng trưởng nếu vượt ngưỡng
		if (len > (capacity >> 1u))
			grow();

		return true;
	}

	void __HashTable::extend_from_array(__Array* source)
	{
		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		auto source_len = source->len;
		// Tăng dung lượng mảng băm nếu cần thiết
		resize_based_on_len(len + source_len);

		// Mở rộng từ Array
		Object* current = nullptr;
		for (u32 i = 0u; i < source_len; ++i)
		{
			current = source->data[i];
			auto hash_value = current->hash();
			auto& head = data[index_hashing(hash_value)];

			if (exist_in_entry(head, current, hash_value))
				continue;

			++len;
			if (head == nullptr)
				head = new Entry(source->data[i], hash_value);
			else
				head = new Entry(source->data[i], hash_value, head);
		}

		SAFE(source);
	}

	void __HashTable::extend_from_hashtable(__HashTable* source)
	{
		if (this == source)
			return;

		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		// Tăng dung lượng mảng băm nếu cần thiết
		resize_based_on_len(len + source->len);

		for (u32 i = 0u; i < capacity; ++i)
		{
			auto head = source->data[i];
			if (head == nullptr)
				continue;

			for (; head; head = head->next)	// Duyệt Entry của "source"
			{
				auto& entry = data[index_hashing(head->hash_value)];

				// Tồn tại => bỏ qua
				if (exist_in_entry(entry, head->ref, head->hash_value))
					continue;

				++len;
				if (entry == nullptr)
					entry = new Entry(*head);
				else
					entry = new Entry(head->ref, head->hash_value, entry);
			}
		}

		SAFE(source);
	}

	void __HashTable::extend_from_iterable(Iterable* iterable)
	{
		if (iterable->is_empty())
		{
			SAFE(iterable);
			return;
		}

		// Tăng dung lượng mảng băm nếu cần thiết
		auto iterable_size = iterable->size();
		resize_based_on_len(len + iterable_size);

		auto iter = iterable->first();

		for (; iter->is_valid(); iter->next())
		{
			auto current = iter->get();
			auto hash_value = current->hash();

			auto& entry = data[index_hashing(hash_value)];
			if (exist_in_entry(entry, current, hash_value))
			{
				SAFE(current);
				continue;
			}

			++len;
			if (entry == nullptr)
				entry = new Entry(current, hash_value);
			else
				entry = new Entry(current, hash_value, entry);
		}

		SAFE(iter);
		SAFE(iterable);
	}

		#pragma endregion

	#pragma endregion

	#pragma region Iterator

	__HashTable::iterator::iterator(__HashTable* container, i32 entry_index, bool first) :
		container(container), entry_index(entry_index), entry(nullptr)
	{
		INREF(container);

		if (entry_index == -1 || entry_index == container->capacity)
			return;

		entry = container->data[entry_index];

		if (first == false)	// last
			while (entry->next)	// Tìm đến nút cuối
				entry = entry->next;
	}


	i64 __HashTable::iterator::type()
	{
		return __HashTable::iterator::type_id;
	}

	std::string __HashTable::iterator::class_name()
	{
		return "__HashTable::iterator";
	}

	u32 __HashTable::iterator::bytes()
	{
		return sizeof(__HashTable::iterator);
	}


	bool __HashTable::iterator::is_valid()
	{
		return entry_index == -1 || entry_index == container->capacity;
	}

	void __HashTable::iterator::next()
	{
		if (entry_index == container->capacity)
		{
			char block_location[64];
			sprintf_s(block_location, "%s::iterator::next",
				dynamic_cast<Object*>(container)->class_name().c_str());

			__pycpp_error_out_of_bound(block_location);
			return;
		}

		entry = entry->next;
		if (entry == nullptr)
		{
			// Tìm đến Entry kế tiếp
			entry_index = container->next_entry(entry_index);

			if (entry_index == -1)	// Điểm kết thúc cuối
				entry_index = container->capacity;
			else
				entry = container->data[entry_index];
		}
	}

	Object* __HashTable::iterator::get()
	{
		if (is_valid() == false)
		{
			char block_location[64];
			sprintf_s(block_location, "%s::iterator::get",
				dynamic_cast<Object*>(container)->class_name().c_str());

			__pycpp_error_out_of_bound(block_location);
			return nullptr;
		}

		return entry->ref;
	}

	#pragma endregion

#pragma endregion

}