#include "object.hpp"

namespace pycpp
{

#pragma region Declaration : Str

	class Str final : public Object
	{

	#pragma region Str
		std::string string;

	public:
		TYPE_ID(Str);

		#pragma region Constructors & Destructors

		Str();
		Str(Str& source);
		Str(Object* source);
		Str(const char* content);

		~Str() = default;

		#pragma endregion

		#pragma region Standard Methods



		#pragma endregion

		#pragma region Override Object

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		void info() override;
		void info(std::string&) override;
		u64 hash() override;
		u32 bytes() override;
		Object* copy() override;

		// So sánh Str với Str.
		bool equal(Object*) override;
		i32 rich_compare(Object*) override;

		#pragma endregion

		#pragma region Override Iterable



		#pragma endregion

	#pragma endregion

	};

#pragma endregion

}