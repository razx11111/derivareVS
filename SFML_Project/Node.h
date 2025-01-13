#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Node {
public:
	
	Node() {  }

	Node(Color textColor, int textSize, Font& textFont, Color nodColor, int nodRaza) {
		text.setFillColor(textColor);
		text.setCharacterSize(textSize);
		text.setFont(textFont);

		nod.setRadius(nodRaza);
		nod.setFillColor(nodColor);
		nod.setOutlineColor(Color(192, 192, 192)); 
		nod.setOutlineThickness(2);
	}

	void setNodePosition(Vector2f pos) {
		nod.setPosition(pos);
		
		FloatRect textBounds = text.getLocalBounds();
		FloatRect nodeBounds = nod.getLocalBounds();
		Vector2f nodePosition = nod.getPosition();

		float xPos = nodePosition.x + (nodeBounds.width / 2.0f) - (textBounds.width / 2.0f) - textBounds.left;
		float yPos = nodePosition.y + (nodeBounds.height / 2.0f) - (textBounds.height / 2.0f) - textBounds.top;
		
		text.setPosition(xPos, yPos);
	}	

	void setText(string t) {
		text.setString(t);
	}

	void drawTo(RenderWindow& window) {
		window.draw(nod);
		window.draw(text);
	}

	string charToString(char* c) {
		if (c == nullptr) {
			return "";
		}
		string s = "";
		for (int i = 0; i < strlen(c); i++) {
			s += c[i];
		}
		text.setString(s);
		return s;
	}

private:
	Text text;
	CircleShape nod;
};