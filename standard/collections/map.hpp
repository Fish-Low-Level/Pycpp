#pragma once

#include "iterable.hpp"
#include "array.hpp"
#include "private_hashtable.hpp"
#include "deque.hpp"
#include "../base/pair.hpp"

namespace pycpp
{
	class __Array;
	struct Entry;
	class __HashTable;
	struct Node;
	class Deque;


#pragma region Declaration : EntryPair

	struct EntryPair
	{
		Object* key;
		Object* value;
		u64 hash_value;
		i32 entry_index;
		i32 mapping;

		inline EntryPair() = default;

		inline EntryPair(Object* key, Object* value) :
			key(key), value(value), hash_value(key->hash()), entry_index(0u), mapping(0u) {}

		inline EntryPair(Object* key, Object* value, u64 hash_of_key) :
			key(key), value(value), hash_value(hash_of_key),
			entry_index(0u), mapping(0u) {}

		inline EntryPair(Object* key, Object* value, u64 hash_of_key,
			i32 entry_index, i32 mapping) :
			key(key), value(value), hash_value(hash_of_key),
			entry_index(entry_index), mapping(mapping) {}

		// Gán và tăng tham chiếu.
		inline void set(Object* key, Object* value)
		{
			INREF(key);
			INREF(value);
			this->key = key;
			this->value = value;
		}

		inline Pair* get_pair()
		{
			return new Pair(key, value);
		}

		inline void clear()
		{
			if (key == nullptr)
				return;
			
			DEREF(key);
			SAFE(key);
			DEREF(value);
			SAFE(value);

			key = nullptr;
			value = nullptr;
		}

		inline void operator=(const EntryPair& source)
		{
			key = source.key;
			value = source.value;
			mapping = source.mapping;
		}

	};

#pragma endregion

#pragma region Declaration : Map

	class OrderedMap final : public Dictionary, public Arguments
	{
		friend Pair;
		friend __Array;
		friend __HashTable;
		friend Deque;

	#pragma region Map

		EntryPair* ordered_entries;
		u32 capacity;
		u32 len;
		u32 len_of_deleted;

	public:
		class iterator;
		friend iterator;

		TYPE_ID(Map);

		#pragma region Constructors & Destructor

		OrderedMap();
		OrderedMap(OrderedMap& source);
		OrderedMap(std::initializer_list<Object*> list);
		OrderedMap(Iterable* iterable);

		~OrderedMap();

		#pragma endregion

		#pragma region Inline Funtion

		inline u32 inline_find_capacity(u32 n_len)
		{
			auto power_of_2 = pycpp::find_next_power_of_two(n_len);
			return (power_of_2 << 1u) - 1u;
		}

		inline u32 inline_probing(u64 hash_value, u32 number)
		{
			u32 squared = number * number;
			u64 probing_value = hash_value + (squared ^ number) + (squared ^ hash_value);
			return (probing_value & capacity) - 1u;
		}

		inline void inline_fill_entry_index()
		{
			for (u32 i = 0u; i < capacity; ++i)
				ordered_entries[i].entry_index = INT32_MAX;
		}

		// Kiểm tra xem số lượng "phần tử bị đánh dấu là xóa" có nhiều đến mức cần cấu trúc lại không.
		//Trả về True nếu "len + len_of_deleted > 2/3 capacity".
		inline bool inline_is_needed_restructure()
		{
			return (len + len_of_deleted) > ((capacity << 1u) / 3u);
		}

		// Tái cấu trúc "ordered_entries", xóa bỏ "len_of_deleted".
		// Chỉ gọi khi "len_of_deleted" khác 0.
		inline void inline_restructured_ordered_entries(EntryPair* old_entries)
		{
			u32 counter = 0u;
			auto len_of_entries = len + len_of_deleted;
			for (u32 i = 0u; i < len_of_entries; ++i)
			{
				if (old_entries[i].key == nullptr)
				{
					++counter;
					continue;
				}

				ordered_entries[i - counter] = old_entries[i];
			}

			len_of_deleted = 0u;
		}

		// Thực hiện băm lại từ vào "array_hash".
		// (!) : chỉ sử dụng hàm khi "len_of_deleted" = 0.
		// (!) : chỉ sử dụng khi toàn bộ "array_hash" là INT32_MAX.
		inline void inline_restructure_entry_index()
		{
			u64 hash_value;
			u32 index;
			for (u32 i = 0u; i < len; ++i)
			{
				hash_value = ordered_entries[i].hash_value;

				for (u32 number = 1;; ++number)
				{
					index = inline_probing(hash_value, number);

					// Collision
					if (ordered_entries[index].entry_index != INT32_MAX)
						continue;

					// Index at entry_index is empty.
					ordered_entries[index].entry_index = i;
					ordered_entries[i].entry_index = index;
				}
			}
		}


		// Nếu "key" có tồn tại trong Map, trả về -1.
		// Nếu Map rỗng, trả về -2.
		// Nếu không tồn tại, trả về vị trí tiếp theo của nó trong bảng băm.
		// (!) : hàm không giải phóng bất kỳ.
		inline i32 inline_find_entry_index(Object* key, u64 key_hash)
		{
			if (len == 0u)
				return -2;

			u32 index;
			i32 mapping;
			i32 entry_index_is_deleted = -1;
			for (u32 i = 1u;; ++i)
			{
				index = inline_probing(key_hash, i);
				mapping = ordered_entries[index].entry_index;

				// Empty -> not contains
				if (mapping == INT32_MAX)
				{
					if (entry_index_is_deleted != -1)
						return entry_index_is_deleted;
					return i32(index);
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
				if (key->equal(ordered_entries[mapping].key) == false)
					continue;

				break;
			}

			return -1;	// Exist
		}

		// Tìm Entry từ bên trái với vị trí "index".
		// Chỉ dùng khi "len_of_deleted" khác 0.
		// (!) : giá trị "index" hợp lệ là [0, len).
		inline i32 inline_find_from_left(i32 index)
		{
			i32 counter = -1, i = 0;
			for (; counter != index; ++i)
				if (ordered_entries[i].key != nullptr)
					++counter;
			return i - 1;
		}

		// Tìm Entry từ bên phải với vị trí "index".
		// Chỉ dùng khi "len_of_deleted" khác 0.
		// (!) : giá trị "index" hợp lệ là [0, len)
		inline i32 inline_find_from_right(i32 index)
		{
			i32 counter = -1, i = len + len_of_deleted;
			for (; counter != index; --i)
				if (ordered_entries[i].key != nullptr)
					++counter;
			return i + 1;
		}


		// Thêm cặp "key-value" vào Map, trả về "true" nếu thêm thành công.
		// (!) : hàm không giải phóng bất kỳ.
		inline bool inline_push_pair(EntryPair entry)
		{
			auto entry_index = inline_find_entry_index(entry.key, entry.hash_value);
			if (entry_index == -1)	// "key" is exist
				return false;
			if (entry_index == -2)	// Map rỗng
				allocation_grow_when_empty();

			// Thêm phần tử
			auto index = len + len_of_deleted;
			ordered_entries[index].set(entry.key, entry.value);
			ordered_entries[index].mapping = entry_index;
			ordered_entries[entry_index].entry_index = index;
			++len;

			// Tăng trưởng
			if (len >= (capacity >> 1))
				allocation_grow();
			return true;
		}

		#pragma endregion

		#pragma region Initialize Methods

		void init_from_hashtable(__HashTable* source);
		void init_from_ordered_dictionary(OrderedMap* source);

		#pragma endregion

		#pragma region Allocation

		// Từ Map rỗng, tăng trưởng thành capacity = 3
		inline void allocation_grow_when_empty()
		{
			capacity = 3u;
			ordered_entries = new EntryPair[3u]
			{
				{ nullptr, nullptr, 0u, INT32_MAX, 0u },
				{ nullptr, nullptr, 0u, INT32_MAX, 0u },
				{ nullptr, nullptr, 0u, INT32_MAX, 0u }
			};
		}

		// Map trở thành rỗng.
		// (!) : có thể có 1 hoặc không có phần tử, hàm kiểm tra và xóa nó.
		inline void allocation_shrink_to_emptry()
		{
			// Delete element if have
			if (len == 0u)
			{
				if (ordered_entries[0].key)
					ordered_entries[0].clear();
				else if (ordered_entries[1].key)
					ordered_entries[1].clear();
				else if (ordered_entries[2].key)
					ordered_entries[2].clear();
			}

			delete[] ordered_entries;
			ordered_entries = nullptr;
			capacity = len = len_of_deleted = 0u;
		}

		// Tăng trưởng gấp đôi dung lượng
		void allocation_grow();

		// Giảm gấp đôi dung lượng
		void allocation_shrink();

		// Thay đổi dung lượng dựa vào "n_len"
		// (!) : nếu "n_len" quá nhỏ dẫn đến "capacity < len" sẽ gây lỗi.
		void allocation_resize_based_on_len(u32 n_len);

		// Thay đổi dung lượng thành "n_capacity".
		// (!) : "n_capacity" không phù hợp có thể gây ra lỗi.
		// (!) : hàm không thực hiện bước trở thành rỗng.
		void allocation_resize_capacity(u32 n_capacity);

		// Xóa những đánh dấu về "phần tử đã bị xóa".
		void allocation_restructure();

		#pragma endregion

		#pragma region Standard Methods



		#pragma endregion

		#pragma region Private Methods

		void private_extend_from_array(__Array* source);
		void private_extend_from_hashtable(__HashTable* source);
		void private_extend_from_deque(Deque* source);
		void private_extend_from_ordered_dictionary(OrderedMap* source);
		void private_extend_from_iterable(Iterable* iterable);

		#pragma endregion

		#pragma region Override Arguments

		bool is_empty() override;
		u32 size() override;
		Object* get(i32 index) override;

		#pragma endregion

		#pragma region Override Iterable

		Iterator* first() override;
		Iterator* last() override;

		#pragma endregion

		#pragma region Override Container

		void clear() override;
		bool contains(Object* element) override;

		// Thêm một "item" vào Map với "item-None" làm "key-value".
		// Trả về "true" nếu thêm thành công.
		bool push(Object* item) override;

		// Xóa và trả về một Pair cuối cùng ( LIFO ) chứa "key-value".
		// (!) : hàm ném lỗi runtime nếu Map rỗng.
		Object* pop() override;

		void extend(std::initializer_list<Object*> list);
		void extend(Iterable* iterable) override;

		#pragma endregion

		#pragma region Override Set

		// Trả về "value" tương ứng với "key" có tồn tại.
		// (!) : hàm ném lỗi runtime nếu "key" không tồn tại.
		Object* get(Object* key) override;

		#pragma endregion

		#pragma region Override Dictionary

		// Gán "value" vào "key" tương ứng.
		// Nếu "key" chưa tồn tại, thêm cặp "key-value" vào Dictionary.
		// Nếu "key" đã tồn tại, thay thế "value" mới của nó.
		void set(Object* key, Object* value) override;

		// Xóa và trả về Pair "key-value" ra khỏi Dictionary.
		// (!) : hàm ném lỗi runtime nếu "key" không tồn tại.
		Object* pop(Object* key) override;

		// Xóa cặp "key-value" ra khỏi OrderedMap.
		void remove(Object* key);

		#pragma endregion

	#pragma endregion

	#pragma region Iterator

		class iterator : public Iterator, public Object
		{
			friend OrderedMap;

			OrderedMap* container;
			i32 index_of_entry;

			iterator(OrderedMap* container, i32 index_of_entry);

		public:
			TYPE_ID(OrderedMap::iterator);

			~iterator();

			///==========     Override Object     ==========///

			i64 type() override;
			std::string class_name() override;
			u32 bytes() override;

			///==========     Override Iterator     ==========///

			bool is_valid() override;
			void next() override;
			void prev() override;
			Object* get() override;

		};

	#pragma endregion

	};

#pragma endregion

}