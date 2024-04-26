#pragma once

#include "primitive.hpp"
#include "../collections/iterable.hpp"

namespace pycpp
{
	class Arguments;

#pragma region Declaration : Funtor

	class Funtor : public Object
	{
	public:
		virtual ~Funtor() = default;

		virtual Object* call(std::initializer_list<Object*>) = 0;
	};

#pragma endregion

#pragma region Declaration : Lambda

	class Lambda final : public Funtor
	{
		using lambda_t = Object* (*)(Arguments*);
		lambda_t lambda;

	public:
		Lambda() = delete;
		Lambda(lambda_t);
		~Lambda() = default;

		Object* call(std::initializer_list<Object*>) override;
	};

#pragma endregion

#pragma region Declaration : Predicate

	class Predicate final : public Funtor
	{
		using lambda_t = bool (*)(Object*);
		lambda_t lambda;

	public:
		Predicate() = delete;
		Predicate(lambda_t lambda);
		~Predicate() = default;

		bool call(Object*);
		Object* call(std::initializer_list<Object*>) override;
	};

#pragma endregion

#pragma region Declaration : Mapping

	class Mapping final : public Funtor
	{
		using lambda_t = Object* (*)(Object*);
		lambda_t lambda;

	public:
		Mapping() = delete;
		Mapping(lambda_t lambda);
		~Mapping() = default;

		Object* call(Object*);
		Object* call(std::initializer_list<Object*>) override;
	};

#pragma endregion

#pragma region Declaration : Compare

	class Comparer final : public Funtor
	{
		using lambda_t = i32 (*)(Object*, Object*);
		lambda_t lambda;

	public:
		Comparer() = delete;
		Comparer(lambda_t lambda);
		~Comparer() = default;

		i32 call(Object*, Object*);
		Object* call(std::initializer_list<Object*>) override;
	};

#pragma endregion

}