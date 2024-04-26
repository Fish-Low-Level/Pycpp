#pragma once

#include "object.hpp"

namespace pycpp {

#pragma region I32

	class I32 final : public Object
	{
	public:
		TYPE_ID(I32);
		const i32 value;

		//==========     Initialize     ==========//

		I32();
		I32(i32 value);
		I32(const I32& source);
		I32(Object* source);
		~I32() = default;

		i32 init_from_object(Object* source);

		//==========     Methods Object     ==========//

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		u64 hash() override;
		u32 bytes() override;
		Object* copy() override;

		bool equal(Object* other) override;
		i32 rich_compare(Object* other) override;

		//==========     Methods Operator     ==========//

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I32 + Boolean | I32 -> I32 },
		/// { I32 + I64 -> I64 },
		/// { I32 + F32 -> F32 }
		/// { I32 + F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* add(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I32 - Boolean | I32 -> I32 },
		/// { I32 - I64 -> I64 }
		/// { I32 - F32 -> F32 }
		/// { I32 - F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* sub(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I32 * Boolean | I32 -> I32 },
		/// { I32 * I64 -> I64 }
		/// { I32 * F32 -> F32 }
		/// { I32 * F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* mul(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I32 / Boolean -> I32 },
		/// { I32 / I32 | I64 | F32 -> F32 },
		/// { I32 / F64 -> F64 }.
		/// Chia cho False hoặc giá trị 0 sẽ ném lỗi runtime.
		/// Gọi phương thức với đối tượng không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64</param>
		Object* div(Object* other) override;

	};

#pragma endregion

#pragma region I64

	class I64 final : public Object {
	public:
		TYPE_ID(I64);
		const i64 value;

		//==========     Initialize     ==========//

		I64();
		I64(i64 value);
		I64(Object* source);
		I64(const I64& source);
		~I64() = default;

		i64 init_from_object(Object* source);

		//==========     Methods Object     ==========//

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		u64 hash() override;
		u32 bytes() override;
		Object* copy() override;

		bool equal(Object* other) override;
		i32 rich_compare(Object* other) override;

		//==========     Methods Operator     ==========//

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I64 + Boolean | I32 | I64 -> I64 },
		/// { I64 + F32 -> F32 }
		/// { I64 + F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* add(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I64 - Boolean | I32 |I64 -> I64 },
		/// { I64 - F32 -> F32 }
		/// { I64 - F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* sub(Object* other) override;
		
		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I64 * Boolean | I32 | I64 -> I64 },
		/// { I64 * F32 -> F32 }
		/// { I64 * F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* mul(Object* other) override;
		
		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { I64 / Boolean -> I64 },
		/// { I64 / I32 | I64 | F32 -> F32 },
		/// { I64 / F64 -> F64 }.
		/// Chia cho False hoặc giá trị 0 sẽ ném lỗi runtime.
		/// Gọi phương thức với đối tượng không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64</param>
		Object* div(Object* other) override;

	};

#pragma endregion

#pragma region F32

	class F32 final : public Object {
	public:
		TYPE_ID(F32);
		const f32 value;

		//==========     Initialize     ==========//

		F32();
		F32(f32 value);
		F32(Object* source);
		F32(const F32& source);
		~F32() = default;

		f32 init_from_object(Object* source);

		//==========     Methods Object     ==========//

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		u32 bytes() override;
		u64 hash() override;
		Object* copy() override;


		bool equal(Object* other) override;
		i32 rich_compare(Object* other) override;

		//==========     Methods Operator     ==========//

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F32 + Boolean | I32 | I64 | F32 -> F32 },
		/// { F32 + F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* add(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F32 - Boolean | I32 | I64 | F32 -> F32 },
		/// { F32 - F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* sub(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F32 * Boolean | I32 | I64 | F32 -> F32 },
		/// { F32 * F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* mul(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F32 / Boolean | I32 | I64 | F32 -> F32 },
		/// { F32 / F64 -> F64 }.
		/// Chia cho False hoặc giá trị 0 sẽ ném lỗi runtime.
		/// Gọi phương thức với đối tượng không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64</param>
		Object* div(Object* other) override;

	};

#pragma endregion

#pragma region F64

	class F64 final : public Object {
	public:
		TYPE_ID(F64);
		const f64 value;

		//==========     Initialize     ==========//

		F64();
		F64(f64 value);
		F64(Object* source);
		F64(const F64& source);
		~F64() = default;

		f64 init_from_object(Object* source);

		//==========     Methods Object     ==========//

		i64 type() override;
		std::string class_name() override;
		void output() override;
		void output(std::string&) override;
		u32 bytes() override;
		u64 hash() override;
		Object* copy() override;

		bool equal(Object* other) override;
		i32 rich_compare(Object* other) override;

		//==========     Methods Operator     ==========//

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F64 + Boolean | I32 | I64 | F32 | F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* add(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F64 - Boolean | I32 | I64 | F32 | F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* sub(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F64 * Boolean | I32 | I64 | F32 | F64 -> F64 }.
		/// Gọi phương thức với Object không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64.</param>
		Object* mul(Object* other) override;

		/// <summary>
		/// Trả về đối tượng bao quát nhất :
		/// { F64 / Boolean | I32 | I64 | F32 | F64 -> F64 }.
		/// Chia cho False hoặc giá trị 0 sẽ ném lỗi runtime.
		/// Gọi phương thức với đối tượng không hợp lệ sẽ ném lỗi runtime.
		/// </summary>
		/// <param name="other">: Boolean, I32, I64, F32, F64</param>
		Object* div(Object* other) override;

	};

#pragma endregion

}