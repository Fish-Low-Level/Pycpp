#pragma once

#include <iostream>
#include <string>

namespace pycpp
{

	typedef int32_t i32;
	typedef int64_t i64;
	typedef uint32_t u32;
	typedef uint64_t u64;
	typedef float f32;
	typedef double f64;

#pragma region Object

	// Object : lớp nguyên mẫu, cần phải kế thừa Object để có thể sử dụng các tính năng của thư viện.
	class Object {
	protected:
		Object();

	public:
		mutable u32 ref_count;

		virtual ~Object() = default;

		/// <summary>
		/// Giá trị định danh của lớp đối tượng.
		/// </summary>
		virtual i64 type();

		/// <summary>
		/// Tên của lớp đối tượng.
		/// </summary>
		virtual std::string class_name();

		/// <summary>
		/// Xuất Object ra Console.
		/// </summary>
		virtual void output();

		/// <summary>
		/// Xuất Object ra chuỗi.
		/// </summary>
		virtual void output(std::string&);

		/// <summary>
		/// Xuất toàn bộ thông tin Object ra Console.
		/// </summary>
		virtual void info();

		/// <summary>
		/// Xuất toàn bộ thông tin Object ra chuỗi.
		/// </summary>
		virtual void info(std::string&);

		// Tính số bytes của Object, có thể bao gồm các Object mà nó tham chiếu tới.
		// (!) Một số trường hợp có thể không chính xác.
		virtual u32 bytes();

		/// <summary>
		/// Giá trị băm.
		/// </summary>
		virtual u64 hash();

		/// <summary>
		/// Tạo mới bản sao của đối tượng.
		/// </summary>
		virtual Object* copy();

		/// <summary>
		/// True nếu bằng với other hoặc trùng địa chỉ.
		/// `this` và `other` có thể khác kiểu dữ liệu, nhưng vẫn có thể bằng nhau.
		/// Nếu hai Object khác kiểu dữ liệu và không thể so sánh với nhau, trả về False.
		/// </summary>
		virtual bool equal(Object* other);

		// So sánh với "other" và trả về Integer.
		// Nếu âm : "this" < "other",
		// Nếu dương : "this" > "other",
		// Bằng 0 : "this" == "other".
		virtual i32 rich_compare(Object* other);

		/// <summary>
		/// Trả về Object mang giá trị `this + other`.
		/// Nếu instance của lớp là hằng số ( Object không thể thay đổi giá trị ),
		/// hàm sẽ trả về Object mới.
		/// Nếu instance của lớp có thể thay đổi giá trị, hàm trả về chính nó.
		/// </summary>
		virtual Object* add(Object*);

		/// <summary>
		/// Trả về Object mang giá trị `this - other`.
		/// Nếu instance của lớp là hằng số ( Object không thể thay đổi giá trị ),
		/// hàm sẽ trả về Object mới.
		/// Nếu instance của lớp có thể thay đổi giá trị, hàm trả về chính nó.
		/// </summary>
		virtual Object* sub(Object*);

		/// <summary>
		/// Trả về Object mang giá trị `this * other`.
		/// Nếu instance của lớp là hằng số ( Object không thể thay đổi giá trị ),
		/// hàm sẽ trả về Object mới.
		/// Nếu instance của lớp có thể thay đổi giá trị, hàm trả về chính nó.
		/// </summary>
		virtual Object* mul(Object*);

		/// <summary>
		/// Trả về Object mang giá trị `this / other`.
		/// Nếu instance của lớp là hằng số ( Object không thể thay đổi giá trị ),
		/// hàm sẽ trả về Object mới.
		/// Nếu instance của lớp có thể thay đổi giá trị, hàm trả về chính nó.
		/// </summary>
		virtual Object* div(Object*);

	};

#pragma endregion

#pragma region Macro

	i64 constexpr pycpp_type_id(const char* class_name, i64 __len__)
	{
		i64 __sum = 0;

		u32 i = 0u;
		for (; class_name[i]; ++i)
			__sum += i64(class_name[i]) << i;
		return __sum + (__len__ << i);
	}

#define UNIQUE_ID(Type)	pycpp_type_id(#Type, __LINE__)

#define TYPE_ID(Type)	\
	static constexpr const i64 type_id = UNIQUE_ID(Type)

#define NONE &None::instance
#define TRUE &Boolean::True
#define FALSE &Boolean::False

	inline bool IS(const Object* obj, const Object* __constant)
	{
		// T is base of Object
		return obj == __constant;
	}

	template <typename T, typename std::enable_if_t<
		!std::is_base_of_v<Object, T>
		, bool> = true>
	inline bool IS(const T* obj, const Object* __constant)
	{
		// T is not base of Object
		return dynamic_cast<const Object*>(obj) == __constant;
	}

#pragma endregion

#pragma region Boolean

	class Boolean final : public Object {

		Boolean();
		Boolean(Boolean&) = delete;
		Boolean(const Boolean&) = delete;
		Boolean(Boolean&&) = delete;
		~Boolean() = default;

	public:
		TYPE_ID(Boolean);

		static Boolean True;
		static Boolean False;

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		void info() override;
		void info(std::string&) override;
		u32 bytes() override;
		u64 hash() override;

		bool equal(Object*) override;
		i32 rich_compare(Object*) override;

		Object* add(Object*) override;
		Object* sub(Object*) override;
		Object* mul(Object*) override;
		Object* div(Object*) override;

	};

#pragma endregion

#pragma region None

	class None final : public Object {

		None();
		None(const None&) = delete;
		None(None&&) = delete;
		~None() = default;

	public:
		TYPE_ID(None);

		static None instance;

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		void info() override;
		void info(std::string&) override;
		u32 bytes() override;
		u64 hash() override;

		bool equal(Object*) override;

		// None có thể so sánh với tất cả các Object, và kết quả luôn là -1 nếu khác kiểu.
		// None là nhỏ nhất.
		i32 rich_compare(Object*) override;
	};

#pragma endregion

}