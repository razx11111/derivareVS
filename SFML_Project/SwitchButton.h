#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class SwitchButton {
public:
    SwitchButton() {}

    SwitchButton(Vector2f size, Color buttonColor, Color textColor, int charSize) {
        text.setFillColor(textColor);
        text.setCharacterSize(charSize);

        button.setSize(size);
        button.setFillColor(buttonColor);
    }

    void assignText(string t) {
        text.setString(t);
        centerText();
    }

    void setFont(Font& font) {
        text.setFont(font);
        centerText();
    }

    void setColors(Color buttonColor, Color textColor) {
        button.setFillColor(buttonColor);
        text.setFillColor(textColor);
    }

    void setButtonPostionion(Vector2f pos) {
        button.setPosition(pos);
        centerText();
    }

    void drawTo(RenderWindow& window) {
        window.draw(button);
        window.draw(text);
    }

    bool isMouseOver(RenderWindow& window) {
        float mouseX = Mouse::getPosition(window).x;
        float mouseY = Mouse::getPosition(window).y;

        float btnPosX = button.getPosition().x;
        float btnPosY = button.getPosition().y;

        float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
        float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
            return true;
        }
        return false;
    }

    void wasPressed() {
        isPressed = true;
    }

    bool isButtonPressed() {
        if (isPressed) {
            isPressed = false;
            return true;
        }
        return false;
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

    void modify() {
        isPressed = false;
    }

private:
    RectangleShape button;
    Text text;
    bool isPressed = false;

    void centerText() {
        FloatRect textBounds = text.getLocalBounds();
        FloatRect buttonBounds = button.getLocalBounds();
        Vector2f buttonPosition = button.getPosition();

        float xPos = buttonPosition.x + (buttonBounds.width / 2.0f) - (textBounds.width / 2.0f) - textBounds.left;
        float yPos = buttonPosition.y + (buttonBounds.height / 2.0f) - (textBounds.height / 2.0f) - textBounds.top;
        text.setPosition(xPos, yPos);
    }
};