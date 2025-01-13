#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

#define DELETE 8
#define ENTER 13
#define ESC 27

using namespace std;
using namespace sf;

class TextBox {
public:
	TextBox() {

	}

	TextBox(bool sel) {
		isSelected = true;

		if (sel) {
			textbox.setString("_");
		}
		else
			textbox.setString("");
	}

	void setFont(Font& font) {
		textbox.setFont(font);
	}

	void setPosition(Vector2f pos) {
		textbox.setPosition(pos);
	}

	void setSelected(bool sel) {
		isSelected = sel;
		if (!sel) {
			string t = text.str();
			string newT = "";
			for (int i = 0; i < t.size(); i++) {
				newT += t[i];
			}
			textbox.setString(newT);
		}
	}

	string getText() {
		return text.str();
	}

	void drawTo(RenderWindow& window) {
		window.draw(textbox);
	}

	void typedOn(Event input) {
		if (isSelected) {
			int charTyped = input.text.unicode;
			if (charTyped < 128) {
				if (continueInput) {
					inputLogic(charTyped);
				}
			}
		}
	}

	bool isEnterPressed() { 
		return enterPressed; 
	} 
	
	void modify() {
		enterPressed = false;
	}

	bool leaveWindow() {
		return leave;
	}

	char* convertToChar() {
		char final[100];
		int i;
		for (i = 0; i < saveSave.length(); i++) {
			final[i] = saveSave[i];
		}
		final[i] = '\0';
		return final;
	}

	void buttonPressed() {
		deleteAll();
	}	

	string getSavedText() {
		savedText = text.str();
		continueInput = false;
		if (savedText.length() != 0) {
			saveSave = savedText;
		}
		return saveSave;
	}

private:
	
	ostringstream text;
	Text textbox;
	Font font;
	bool isSelected = false;
	bool enterPressed = false;
	bool leave = false;
	bool continueInput = true;
	string savedText = "";
	string inputFunction = "";
	string saveSave = "";

	void inputLogic(int charTyped) {
		if (charTyped != DELETE && charTyped != ENTER && charTyped != ESC)
			text << static_cast<char>(charTyped);
		else if (charTyped == DELETE) {
			if (text.str().length() > 0) {
				deleteLastChar();
			}
		}
		else if (charTyped == ENTER) { 
			enterPressed = true; 
			savedText = text.str(); 
			continueInput = false;
		}
		else if (charTyped == ESC) {
			leave = true;
		}
		textbox.setString(text.str() + "_");
		if (continueInput == false) {
			deleteAll();
		}
	}

	void deleteLastChar() {
		string t = text.str();
		string newT = "";
		for (int i = 0; i < t.size() - 1; i++) {
			newT += t[i];
		}
		text.str("");
		text << newT;

		textbox.setString(text.str());
	}

	void deleteAll() {
		savedText = text.str();
		text.str("");
		textbox.setString(text.str());
	}
};