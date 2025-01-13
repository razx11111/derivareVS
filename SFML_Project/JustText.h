#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class JustText {
public:
	JustText() {  }

	JustText(Vector2f pos, Color textColor, int size, Font& font) {
		text.setFillColor(textColor);
		text.setPosition(pos);
		text.setCharacterSize(size);
		text.setFont(font);
	}

	void setText(string t) {
		text.setString(t);
	}

	void drawTo(RenderWindow& window) {
		window.draw(text);
	}

private:
	Text text;
};