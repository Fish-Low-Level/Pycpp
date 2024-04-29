#include "../base/object.hpp"
#include "../collections/array.hpp"
#include "../collections/private_hashtable.hpp"

namespace pycpp
{

#pragma region Declaration : Private Fixed Array

	class __Array;
	struct Entry;
	class __HashTable;
	class Arguments;

	template<u32 Capacity, std::enable_if_t<(Capacity > 0u), bool> = true>
	class __FixedArray
	{
		class iterator;

	protected:
		Object* data[Capacity];
		u32 len;

	#pragma region Private Fixed Array

		#pragma region Constructors & Destructor

		__FixedArray() : len(0u) {}

		__FixedArray(std::initializer_list<Object*> list)
		{
			u32 list_size = (u32)list.size();
			len = pycpp::min(Capacity, list_size);

			auto head = list.begin();
			for (u32 i = 0u; i < len; ++i)
			{
				INREF(head[i]);
				data[i] = head[i];
			}

			if (len < list_size)
				for (u32 i = len; i < list_size; ++i)
					SAFE(head[i]);
		}

		__FixedArray(__FixedArray& source) : len(source.len)
		{
			for (u32 i = 0u; i < len; ++i)
			{
				data[i] = source.data[i];
				INREF(data[i]);
			}
		}

		__FixedArray(Iterable* iterable)
		{
			auto __array = dynamic_cast<__Array*>(iterable);
			if (__array)
			{
				init_from_array(__array);
				return;
			}

			auto __hashtable = dynamic_cast<__HashTable*>(iterable);
			if (__hashtable)
			{
				init_from_hashtable(__hashtable);
				return;
			}

			auto arguments = dynamic_cast<Arguments*>(iterable);
			if (arguments)
			{
				init_from_arguments(arguments);
				return;
			}

			init_from_iterable(iterable);
		}

		virtual ~__FixedArray()
		{
			for (u32 i = 0u; i < len; ++i)
			{
				DEREF(data[i]);
				SAFE(data[i]);
			}
		}

		#pragma endregion

		#pragma region Initialize Methods

		void init_from_array(__Array* source)
		{
			len = pycpp::min(Capacity, source.len);
			for (u32 i = 0u; i < len; ++i)
			{
				data[i] = source->data[i];
				INREF(data[i]);
			}

			SAFE(source);
		}

		void init_from_arguments(Arguments* arguments)
		{
			len = pycpp::min(Capacity, arguments->size());
			for (u32 i = 0u; i < len; ++i)
			{
				data[i] = arguments->get(i);
				INREF(data[i]);
			}

			SAFE(arguments);
		}

		void init_from_hashtable(__HashTable* source)
		{
			len = pycpp::min(Capacity, source->len);

			u32 increase_index = 0u;
			for (u32 i = 0u; i < source->capacity; ++i)
			{
				auto head = source->data[i];
				if (head == nullptr)
					continue;

				for (; head; head = head->next)
				{
					data[increase_index] = head->ref;
					INREF(data[increase_index]);
					++increase_index;
					if (increase_index == Capacity)
					{
						SAFE(source);
						return;
					}
				}
			}

			SAFE(source);
		}

		void init_from_iterable(Iterable* iterable)
		{
			len = pycpp::min(Capacity, iterable->size());
			auto iter = iterable->first();

			u32 increase_index = 0u;
			for (; iter->is_valid(); iter->next())
			{
				data[increase_index] = iter->get();
				INREF(data[increase_index]);
				++increase_index;
				if (increase_index == Capacity)
					break;
			}

			SAFE(iter);
			SAFE(iterable);
		}

		#pragma endregion

		#pragma region Inline Methods

		#pragma endregion

		#pragma region Allocation

		#pragma endregion

		#pragma region Standard Methods

		Iterator* first()
		{
			return iterator(this, 0);
		}

		Iterator* last()
		{
			return iterator(this, i32(len) - 1u);
		}

		void output()
		{
			std::string content;
			output(content);
			std::cout << content;
		}

		void output(std::string& out)
		{
			out.clear();
			bool first = true;

			std::string str_cur;
			for (u32 i = 0u; i < len; ++i)
			{
				data[i]->output(str_cur);

				if (first == false)
					str_cur = ", " + str_cur;
				first = false;

				out += str_cur;
			}
		}

		void clear()
		{
			for (u32 i = 0u; i < len; ++i)
			{
				DEREF(data[i]);
				SAFE(data[i]);
			}
			len = 0u;
		}

		void reverse()
		{
			if (len < 1u)
				return;

			for (u32 i = 0, j = len - 1u; i < j; ++i, --j)
				pycpp::swap(data[i], data[j]);
		}

		void sort()
		{
			pycpp::shell_sort(data, len);
		}

		bool push(Object* item)
		{
			if (len == Capacity)
			{
				SAFE(item);
				return false;
			}

			INREF(item);
			data[len++] = item;
			return true;
		}

		void insert(i32 index, Object* item)
		{
			if (len == Capacity)
			{
				SAFE(item);
				return;
			}

			// Error : out of bound
			if (index < -i32(len) || index >= i32(len))
			{
				char block_location[32];
				std::sprintf(block_location, "%s::insert",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			if (index < 0)
				index += len;

			for (i32 i = (i32)len; i > index; --i)
				data[i] = data[i - 1];
			INREF(item);
			data[index] = item;
			++len;
		}

		Object* pop()
		{
			// Error : retrieve from an empty Container
			if (len == 0u)
			{
				std::string __class_name = dynamic_cast<Object*>(this);
				char block_location[32];
				std::sprintf(block_location, "%s::pop", __class_name.c_str());

				__pycpp_error_retrieve_from_empty_container(block_location, __class_name.c_str());
			}

			--len;
			DEREF(data[len]);
			return data[len];
		}

		Object* pop(i32 index)
		{
			// Error : retrieve from an empty Container
			if (len == 0u)
			{
				std::string __class_name = dynamic_cast<Object*>(this);
				char block_location[32];
				std::sprintf(block_location, "%s::pop", __class_name.c_str());

				__pycpp_error_retrieve_from_empty_container(block_location, __class_name.c_str());
			}

			// Error : out of bound
			if (index < -i32(len) || index >= i32(len))
			{
				char block_location[32];
				std::sprintf(block_location, "%s::pop(i32 index)",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			if (index < 0)
				index += len;

			Object* result = data[index];
			DEREF(result);

			for (i32 i = index + 1; i < len; ++i)
				data[i - 1] = data[i];
			--len;
			return result;
		}

		void remove(i32 index)
		{
			// Error : out of bound
			if (index < -i32(len) || index >= i32(len))
			{
				char block_location[32];
				std::sprintf(block_location, "%s::pop(i32 index)",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			if (index < 0)
				index += len;

			DEREF(data[index]);
			SAFE(data[index]);

			for (i32 i = index + 1; i < len; ++i)
				data[i - 1] = data[i];
			--len;
		}

		void extend(Iterable* iterable)
		{
			auto arguments = dynamic_cast<Arguments*>(iterable);
			if (arguments)
			{
				extend_from_arguments(arguments);
				return;
			}

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

		bool contains(Object* elem)
		{
			for(u32 i = 0u; i < len; ++i)
				if (elem->equal(data[i]))
				{
					SAFE(elem);
					return true;
				}

			SAFE(elem);
			return false;
		}

		#pragma endregion

		#pragma region Private Methods

		void multiply_with_int(u32 multiplier)
		{
			if (len == 0u || multiplier == 1u)
				return;

			if (multiplier == 0u)
			{
				clear();
				return;
			}

			// Error : out of bound.
			if (len * multiplier > Capacity)
			{
				char block_location[64];
				std::sprintf(block_location, "%s::mul",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			for (u32 i = 1u; i < multiplier; ++i)
			{
				u32 begin = i * len;

				for (u32 j = 0u; j < len; ++j)
					data[begin + j] = data[j];
			}

			len *= multiplier;
		}


		void extend_from_array(__Array* source)
		{
			if (source->len == 0u)
			{
				SAFE(source);
				return;
			}

			// Error : out of bound.
			if (len + source->len > Capacity)
			{
				char block_location[64];
				std::sprintf(block_location, "%s::extend",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			Object* cur;
			for (u32 i = 0u; i < source->len; ++i)
			{
				cur = source->data[i];
				INREF(cur);
				data[i + len] = cur;
			}

			len += source->len;
			SAFE(source);
		}

		void extend_from_arguments(Arguments* arguments)
		{
			auto arguments_len = arguments->size();
			if (arguments_len == 0u)
			{
				SAFE(arguments_len);
				return;
			}

			// Error : out of bound.
			if (len + arguments_len > Capacity)
			{
				char block_location[64];
				std::sprintf(block_location, "%s::extend",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			Object* cur;
			for (u32 i = 0u; i < arguments_len; ++i)
			{
				cur = arguments->get(i);
				INREF(cur);
				data[i + len] = cur;
			}

			len += arguments_len;
			SAFE(arguments);
		}

		void extend_from_hashtable(__HashTable* source)
		{
			if (source->len == 0u)
			{
				SAFE(source);
				return;
			}

			// Error : out of bound.
			if (len + source->len > Capacity)
			{
				char block_location[64];
				std::sprintf(block_location, "%s::extend",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			Entry* head;
			u32 increase_counter = 0u;
			for (u32 i = 0u; i < source->capacity; ++i)
			{
				head = source;
				if (head == nullptr)
					continue;

				for (; head; head = head->next)
				{
					INREF(head->ref);
					data[increase_counter + len] = head->ref;
					++increase_counter;
				}
			}

			len += source->len;
			SAFE(source);
		}

		void extend_from_iterable(Iterable* iterable)
		{
			auto iterable_len = iterable->size();
			if (iterable_len == 0u)
			{
				SAFE(iterable);
				return;
			}

			// Error : out of bound.
			if (len + iterable_len > Capacity)
			{
				char block_location[64];
				std::sprintf(block_location, "%s::extend",
					dynamic_cast<Object*>(this)->class_name().c_str());

				__pycpp_error_out_of_bound_error(block_location);
			}

			auto iter = iterable->first();
			Object* cur;
			for (; iter->is_valid(); iter->next())
			{
				cur = iter->get();
				INREF(cur);
				data[i + len] = cur;
			}

			len += source->len;
			SAFE(iter);
			SAFE(iterable);
		}

		#pragma endregion

	#pragma endregion

	#pragma region Iterator
	public:

		class iterator final : public Iterator, public Object
		{
			__FixedArray* container;
			i32 index;

			friend __FixedArray;
			iterator(__FixedArray* container, i32 index) :
				container(container), index(index)
			{
				INREF(container);
			}

			~iterator()
			{
				DEREF(container);
				SAFE(container);
			}

		public:
			TYPE_ID(__FixedArray::iterator);

			i64 type() override
			{
				return type_id;
			}

			std::string class_name() override
			{
				return typeid(__FixedArray::iterator).name();
			}

			u32 bytes() override
			{
				return sizeof(__FixedArray::iterator);
			}

			bool is_valid() override
			{
				return index >= 0 && index < i32(container->len);
			}

			void next() override
			{
				if (index >= i32(container->len))
				{
					char block_location[64];
					std::sprintf(block_location, "%s::iterator::next",
						dynamic_cast<Object*>(container)->class_name().c_str());

					__pycpp_error_out_of_bound_error(block_location);
					return;
				}

				++index;
			}

			void prev() override
			{
				if (index < 0)
				{
					char block_location[64];
					std::sprintf(block_location, "%s::iterator::prev",
						dynamic_cast<Object*>(container)->class_name().c_str());

					__pycpp_error_out_of_bound_error(block_location);
					return;
				}

				--index;
			}

			Object* get() override
			{
				if (is_valid() == false)
				{
					char block_location[64];
					std::sprintf(block_location, "%s::iterator::get",
						dynamic_cast<Object*>(container)->class_name().c_str());

					__pycpp_error_out_of_bound_error(block_location);
					return nullptr;
				}

				return container->data[index];
			}


		};

	#pragma endregion

	};

#pragma endregion

}