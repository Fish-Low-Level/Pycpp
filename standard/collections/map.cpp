#include "map.hpp"
#include "../base/algo.hpp"
#include "tuple.hpp"
#include "arraylist.hpp"
#include "hashtable.hpp"

namespace pycpp
{

#pragma region Definition : OrderedMap

#pragma region OrderedMap

		#pragma region Constructors & Destructor

	OrderedMap::OrderedMap() : ordered_entries(nullptr),
		capacity(0u), len(0u), len_of_deleted(0u) {}

	OrderedMap::OrderedMap(OrderedMap& source) : OrderedMap()
	{
		init_from_ordered_dictionary(&source);
	}

	OrderedMap::OrderedMap(std::initializer_list<Object*> list) : OrderedMap()
	{
		extend(list);
	}

	OrderedMap::OrderedMap(Iterable* iterable) : OrderedMap()
	{
		extend(iterable);
	}

	OrderedMap::~OrderedMap()
	{
		if (len == 0u)
			return;

		u32 len_of_entries = len + len_of_deleted;
		for (u32 i = 0u; i < len_of_entries; ++i)
			if (ordered_entries[i].key != nullptr)
				ordered_entries[i].clear();
		delete ordered_entries;
	}

		#pragma endregion

		#pragma region Initialize Methods

	void OrderedMap::init_from_hashtable(__HashTable* source)
	{
		if (source->len)
		{
			SAFE(source);
			return;
		}

		allocation_resize_based_on_len(source->len);

		// Adding all elements to "ordered_entries"
		for (u32 i = 0u; i < source->capacity; ++i)
			for (auto head = source->data[i]; head; head = head->next)
			{
				// Gán vào "ordered_entries"
				INREF(head->ref);
				INREF(NONE);
				ordered_entries[len] = { head->ref, NONE, head->hash_value };
				++len;
			}

		// Restructuring "array_hash"
		inline_restructure_entry_index();

		SAFE(source);
	}

	void OrderedMap::init_from_ordered_dictionary(OrderedMap* source)
	{
		if (source->len)
		{
			SAFE(source);
			return;
		}

		// Sao chép những gì "source" có.
		capacity = source->capacity;
		len = source->len;
		len_of_deleted = source->len_of_deleted;

		ordered_entries = new EntryPair[capacity];
		for (u32 i = 0u; i < capacity; ++i)
		{
			ordered_entries[i] = source->ordered_entries[i];
			INREF(ordered_entries[i].key);
			INREF(ordered_entries[i].value);
		}

		SAFE(source);
	}

		#pragma endregion

		#pragma region Allocation

	void OrderedMap::allocation_grow()
	{
		if (capacity == 0u)
		{
			allocation_grow_when_empty();
			return;
		}

		allocation_resize_capacity(capacity << 1u | 1u);
	}

	void OrderedMap::allocation_shrink()
	{
		if (capacity == 3u)
		{
			allocation_shrink_to_emptry();
			return;
		}

		allocation_resize_capacity(capacity >> 1u);
	}

	void OrderedMap::allocation_resize_based_on_len(u32 n_len)
	{
		auto n_capacity = inline_find_capacity(n_len);
		allocation_resize_capacity(n_capacity);
	}

	void OrderedMap::allocation_resize_capacity(u32 n_capacity)
	{
		if (capacity = 0u)
		{
			capacity = n_capacity;
			ordered_entries = new EntryPair[capacity];
			inline_fill_entry_index();
			return;
		}

		// Grow "ordered_entries"
		capacity = n_capacity;
		auto ordered_entries_tmp = ordered_entries;
		ordered_entries = new EntryPair[capacity];

		// Move "ordered_entries"
		if (len_of_deleted = 0u)
		{
			for (u32 i = 0u; i < len; ++i)
				ordered_entries[i] = ordered_entries_tmp[i];
		}
		else
			inline_restructured_ordered_entries(ordered_entries_tmp);

		delete[] ordered_entries_tmp;

		// Grow "array_hash"
		inline_fill_entry_index();
		inline_restructure_entry_index();
	}

	void OrderedMap::allocation_restructure()
	{
		if (len_of_deleted == 0u)
			return;

		// Xử lý "ordered_entries"
		inline_restructured_ordered_entries(ordered_entries);

		// Xử lý bảng băm
		inline_fill_entry_index();
		inline_restructure_entry_index();
	}

		#pragma endregion

		#pragma region Private Methods

	void OrderedMap::private_extend_from_array(__Array* source)
	{
		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		// Adding all elements
		for (u32 i = 0u; i < source->len; ++i)
		{
			auto current = source->data[i];
			inline_push_pair({ current, NONE, current->hash() });
		}

		SAFE(source);
	}

	void OrderedMap::private_extend_from_hashtable(__HashTable* source)
	{
		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		// Adding all elements
		for (u32 i = 0u; i < source->capacity; ++i)
			for(auto head = source->data[i]; head; head = head->next)
				inline_push_pair({ head->ref, NONE, head->hash_value });

		SAFE(source);
	}

	void OrderedMap::private_extend_from_deque(Deque* source)
	{
		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		// Adding all elements
		for (auto head = source->head; head; head = head->next)
			inline_push_pair({ head->ref, NONE });

		SAFE(source);
	}
	
	void OrderedMap::private_extend_from_ordered_dictionary(OrderedMap* source)
	{
		if (source->len == 0u)
		{
			SAFE(source);
			return;
		}

		// Adding all elements
		u32 len_of_source_entries = source->len + source->len_of_deleted;
		for (u32 i = 0u; i < len_of_source_entries; ++i)
		{
			auto& entry_pair = source->ordered_entries[i];
			if (entry_pair.key != nullptr)
				inline_push_pair(entry_pair);
		}

		SAFE(source);
	}

	void OrderedMap::private_extend_from_iterable(Iterable* iterable)
	{
		auto iter = iterable->first();
		for (; iter->is_valid(); iter->next())
			inline_push_pair({ iter->get(), NONE });

		SAFE(iter);
		SAFE(iterable);
	}

		#pragma endregion

		#pragma region Override Arguments

	bool OrderedMap::is_empty()
	{
		return len == 0u;
	}

	u32 OrderedMap::size()
	{
		return len;
	}

	Object* OrderedMap::get(i32 index)
	{
		if (index < -i32(len) || index >= i32(len))
			__pycpp_error_out_of_bound("OrderedMap::get(i32 index)");

		if (index < 0)
			index += i32(len);

		if (len_of_deleted == 0u)
			return ordered_entries[index].get_pair();

		// Tìm vị trí Entry với "index".
		i32 index_of_entry;
		if (index < (len >> 1))
			index_of_entry = inline_find_from_left(index);
		else
			index_of_entry = inline_find_from_right(i32(len) - index - 1);

		return ordered_entries[index_of_entry].get_pair();
	}

		#pragma endregion

		#pragma region Override Iterable

	Iterator* OrderedMap::first()
	{
		auto first_index = inline_find_from_left(0);
		return new OrderedMap::iterator(this, first_index);
	}

	Iterator* OrderedMap::last()
	{
		auto last_index = inline_find_from_right(0);
		return new OrderedMap::iterator(this, last_index);
	}

		#pragma endregion

		#pragma region Override Container

	void OrderedMap::clear()
	{
		if (len == 0u)
			return;

		auto len_of_entries = len + len_of_deleted;
		for (u32 i = 0u; i < len_of_entries; ++i)
			if (ordered_entries[i].key)
				ordered_entries[i].clear();
		delete ordered_entries;

		ordered_entries = nullptr;
		capacity = len = len_of_deleted = 0u;
	}

	bool OrderedMap::contains(Object* element)
	{
		bool result = inline_find_entry_index(element, element->hash());
		SAFE(element);
		return result < 0;
	}


	bool OrderedMap::push(Object* item)
	{
		auto result = inline_push_pair({ item, NONE });

		if(result == false)
			SAFE(item);
		return result;
	}

	Object* OrderedMap::pop()
	{
		// Error : retrieve from empty OrderedMap
		if (len == 0u)
			__pycpp_error_retrieve_from_empty_container("OrderedMap::pop", "OrderedMap");

		// Tìm vị trí Entry cuối cùng ( không bị đánh dấu )
		i32 index = len + len_of_deleted - 1u;
		if (len_of_deleted != 0u)
			for (; ordered_entries[index].key == nullptr; --index);

		auto result = ordered_entries[index].get_pair();
		ordered_entries[index].clear();

		// Đánh dấu "đã xóa" tại "entry_index"
		ordered_entries[ordered_entries[index].mapping].entry_index = -1;

		return result;
	}

	void OrderedMap::extend(std::initializer_list<Object*> list)
	{
		u32 list_len = list.size();
		auto iter = list.begin();

		for (u32 i = 0u; i < list_len; ++i)
			inline_push_pair({ iter[i], NONE });
	}

	void OrderedMap::extend(Iterable* iterable)
	{
		switch (iterable->type())
		{
		case Tuple::type_id:
		case ArrayList::type_id:
			private_extend_from_array(dynamic_cast<__Array*>(iterable));
			break;

		case HashTable::type_id:
			private_extend_from_hashtable(dynamic_cast<__HashTable*>(iterable));
			break;

		case OrderedMap::type_id:
			private_extend_from_ordered_dictionary(dynamic_cast<OrderedMap*>(iterable));

		case Deque::type_id:
			private_extend_from_deque(dynamic_cast<Deque*>(iterable));
			break;

		default:
			private_extend_from_iterable(iterable);
		}
	}

		#pragma endregion

		#pragma region Override Set

	Object* OrderedMap::get(Object* key)
	{
		auto entry_index = inline_find_entry_index(key, key->hash());

		// Error : "key" doesn't exist
		if (entry_index < 0)
		{
			std::cout << "Error in OrderedMap::get(Object key) : "
				"\"key\" doesn't exist !" << std::endl;

			throw std::exception("Error : get \"value\" with \"key\" doesn't exist in Dictionary");
		}

		auto mapping = ordered_entries[entry_index].entry_index;
		auto& entry_pair = ordered_entries[mapping];
		return entry_pair.value;
	}

		#pragma endregion

		#pragma region Override Dictionary

	void OrderedMap::set(Object* key, Object* value)
	{
		if (len == 0u)
			allocation_grow_when_empty();

		auto key_hash = key->hash();

		u32 index;
		i32 mapping;
		i32 entry_index_is_deleted = -1;
		for (u32 i = 1u;; ++i)
		{
			index = inline_probing(key_hash, i);
			mapping = ordered_entries[index].entry_index;

			// "key" doesn't exist
			if (mapping == INT32_MAX)
			{
				if (entry_index_is_deleted != -1)
					index = entry_index_is_deleted;
				break;
			}

			if (mapping < 0)	// Bị đánh dấu "deleted"
			{
				entry_index_is_deleted = index;
				continue;
			}

			// Kiểm tra giá trị băm
			if (key_hash != ordered_entries[mapping].hash_value)
				continue;

			// So sánh "equal"
			if (key->equal(ordered_entries[mapping].key))
				break;
		}

		// Thêm mới "key-value"
		if (mapping == INT32_MAX)
		{
			auto last_entry_index = len + len_of_deleted;
			ordered_entries[last_entry_index] = { key, value, key_hash, 0u, index };
			ordered_entries[index].entry_index = last_entry_index;
			++len;
			INREF(key);
			INREF(value);

			if (len >= (capacity >> 1))
				allocation_grow();
			return;
		}

		// Chèn "value" mới vào "key" có sẵn
		auto& entry_pair = ordered_entries[index];
		DEREF(entry_pair.value);
		SAFE(entry_pair.value);

		entry_pair.value = value;
		INREF(value);

		SAFE(key);
	}

	Object* OrderedMap::pop(Object* key)
	{
		auto entry_index = inline_find_entry_index(key, key->hash());

		// Not exist
		if (entry_index < 0)
			__pycpp_error_retrieve_from_empty_container("OrderedMap::pop(Object key)",
				"OrderedMap");

		auto& mapping = ordered_entries[entry_index].entry_index;
		auto& entry_pair = ordered_entries[mapping];

		auto result = entry_pair.get_pair();
		entry_pair.clear();

		// Đánh dấu "đã xóa"
		mapping = -1;
		++len_of_deleted;
		--len;

		if (len < (capacity >> 2))
			allocation_shrink();

		SAFE(key);
		return result;
	}

	void OrderedMap::remove(Object* key)
	{
		auto entry_index = inline_find_entry_index(key, key->hash());

		// Not exist
		if (entry_index < 0)
			return;

		auto& mapping = ordered_entries[entry_index].entry_index;
		auto& entry_pair = ordered_entries[mapping];
		entry_pair.clear();

		// Đánh dấu "đã xóa"
		mapping = -1;
		++len_of_deleted;
		--len;

		if (len < (capacity >> 2))
			allocation_shrink();

		SAFE(key);
	}

		#pragma endregion

#pragma endregion

#pragma region Iterator

	OrderedMap::iterator::iterator(OrderedMap* container, i32 index_of_entry) :
		container(container), index_of_entry(index_of_entry)
	{
		INREF(container);
	}

	OrderedMap::iterator::~iterator()
	{
		DEREF(container);
		SAFE(container);
	}


	i64 OrderedMap::iterator::type()
	{
		return OrderedMap::iterator::type_id;
	}

	std::string OrderedMap::iterator::class_name()
	{
		return "OrderedMap::iterator";
	}

	u32 OrderedMap::iterator::bytes()
	{
		return sizeof(OrderedMap::iterator);
	}


	bool OrderedMap::iterator::is_valid()
	{
		return index_of_entry != -1 || 
			index_of_entry != container->len + container->len_of_deleted;
	}

	void OrderedMap::iterator::next()
	{
		auto len_of_entries = container->len + container->len_of_deleted;
		if (index_of_entry == len_of_entries)
			__pycpp_error_out_of_bound("OrderedMap::iterator::next()");

		while (index_of_entry < len_of_entries ||
			container->ordered_entries[++index_of_entry].key == nullptr);
	}

	void OrderedMap::iterator::prev()
	{
		if (index_of_entry == -1)
			__pycpp_error_out_of_bound("OrderedMap::iterator::prev()");

		while (index_of_entry >= 0 ||
			container->ordered_entries[--index_of_entry].key == nullptr);
	}

	Object* OrderedMap::iterator::get()
	{
		if (is_valid() == false)
			__pycpp_error_out_of_bound("OrderedMap::iterator::get()");

		return container->ordered_entries[index_of_entry].get_pair();
	}


#pragma endregion

#pragma endregion

}