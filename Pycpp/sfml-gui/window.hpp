#include "../standard/base/object.hpp"

#include "SFML/Graphics.hpp"

namespace pycpp
{

#pragma region Declaration : Window

	class Window : public Object
	{

#pragma region Window

		sf::RenderWindow render_window;

	public:
		TYPE_ID(Window);

#pragma region Constructors & Destructor

		Window(sf::VideoMode, const sf::String&,
			u32 style = sf::Style::Default,
			const sf::ContextSettings& context_setting = sf::ContextSettings());

#pragma endregion

#pragma region Standard Methods

		void update();

#pragma endregion

#pragma region Override Object

		virtual i64 type() override;
		virtual const char* class_name() override;
		virtual u32 bytes() override;

#pragma endregion

#pragma endregion	

	};

#pragma endregion

}