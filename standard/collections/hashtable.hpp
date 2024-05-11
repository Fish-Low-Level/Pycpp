#pragma once

#include "iterable.hpp"
#include "private_hashtable.hpp"

namespace pycpp
{

#pragma region Declaration : HashTable

	class HashTable final : public Set, public __HashTable
	{
	public:
		TYPE_ID(HashTable);

	#pragma region Constructors & Destructors

		HashTable() = default;
		HashTable(std::initializer_list<Object*>);
		HashTable(HashTable&);
		HashTable(Iterable*);

		~HashTable() = default;

	#pragma endregion

	#pragma region Override Object

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string& out) override;
		u32 bytes() override;
		u64 hash() override;
		Object* copy() override;

		bool equal(Object*);

		Object* add(Object*);

	#pragma endregion

	#pragma region Override Iterable

		Iterator* first();
		Iterator* last();

	#pragma endregion

	#pragma region Override Container

		void clear();
		bool contains(Object*);
		bool push(Object*);
		Object* pop();
		void extend(Iterable*);
		void extend(std::initializer_list<Object*>);

	#pragma endregion

	#pragma region Override Set

		// Lấy ra phần tử trong HashTable.
		// Có thể sử dụng Set kết hợp Pair cho ra một Dictionary.
		// (!) : hàm ném lỗi runtime nếu "elem" không tồn tại.
		Object* get(Object* elem);

	#pragma endregion

	};

#pragma endregion

}