#include "TextBox.h"
#include "SwitchButton.h"
#include "JustText.h"
#include "Node.h"
#include "derivaref.h"
#include "derivaref.cpp"

using namespace sf;
using namespace std;

int calculateMaxDepth(Nod* radacina) {
    if (radacina == NULL) {
        return 0;
    }
    return 1 + max(calculateMaxDepth(radacina->stg), calculateMaxDepth(radacina->drp));
}

void iarParcurgere(Nod* bunic, Nod* tata, Nod* actual, RenderWindow& window, float x_axis, float y_axis, Font& arial, bool stgdrp) {
    if (actual == NULL) {
        return;
    }

    Node nod(Color::Black, 26, arial, Color::White, 30);
    nod.setText(nod.charToString(actual->info));
    nod.setNodePosition({ x_axis, y_axis });
    nod.drawTo(window);
    float horizontalOffset = 90;

    if (actual->stg != NULL) {
        VertexArray line(Lines, 2);
        line[0].position = { x_axis + 30, y_axis + 30 };
        line[0].color = Color::White;
        line[1].color = Color::White;

        if (!stgdrp && tata != NULL) {
            if (tata->stg != NULL && tata->stg->drp != NULL) {
                line[1].position = { x_axis - horizontalOffset / 2 + 30, y_axis + 70 + 30};
                window.draw(line);
                iarParcurgere(tata, actual, actual->stg, window, x_axis - 45, y_axis + 70, arial, true);
            }
            else {
                line[1].position = { x_axis - horizontalOffset + 30, y_axis + 70 + 30};
                window.draw(line);
                iarParcurgere(tata, actual, actual->stg, window, x_axis - 90, y_axis + 70, arial, true);
            }
        }
        else {
            line[1].position = { x_axis - horizontalOffset + 30, y_axis + 70 + 30};
            window.draw(line);
            iarParcurgere(tata, actual, actual->stg, window, x_axis - 90, y_axis + 70, arial, true);
        }
    }

    if (actual->drp != NULL) {
        VertexArray line(Lines, 2);
        line[0].position = { x_axis + 30, y_axis + 30 };
        line[0].color = Color::White;
        line[1].color = Color::White;

        if (stgdrp && tata != NULL) {
            if (tata->drp != NULL && tata->drp->stg != NULL) {
                line[1].position = { x_axis + horizontalOffset / 2 + 30, y_axis + 70 + 30};
                window.draw(line);
                iarParcurgere(tata, actual, actual->drp, window, x_axis + 45, y_axis + 70, arial, false);
            }
            else {
                line[1].position = { x_axis + horizontalOffset + 30, y_axis + 70 + 30};
                window.draw(line);
                iarParcurgere(tata, actual, actual->drp, window, x_axis + 90, y_axis + 70, arial, false);
            }
        }
        else {
            line[1].position = { x_axis + horizontalOffset + 30, y_axis + 70 + 30 };
            window.draw(line);
            iarParcurgere(tata, actual, actual->drp, window, x_axis + 90, y_axis + 70, arial, false);
        }
    }
}


void parcurgere(Nod* radacina, Vector2f coordonate, Font& arial, RenderWindow& window, float horizontalOffset, float verticalOffset, int depth, int maxDepth) {
    if (radacina == NULL) {
        return;
    }

    Node nod1(Color::Black, 30, arial, Color::White, 30);
    nod1.setText(nod1.charToString(radacina->info));
    nod1.setNodePosition(coordonate);
    nod1.drawTo(window);

    float nodeRadius = 30.0f; 
    float adjustedHorizontalOffset = horizontalOffset * ((maxDepth - depth)/1.1); 

    if (radacina->stg != NULL) {
        Vector2f leftChildPos = coordonate;
        leftChildPos.x -= adjustedHorizontalOffset;
        leftChildPos.y += verticalOffset; 

        // Draw line to left child
        sf::VertexArray line(Lines, 2);
        line[0].position = { coordonate.x + nodeRadius, coordonate.y + nodeRadius};
        line[0].color = Color::White;
        line[1].position = { leftChildPos.x + nodeRadius, leftChildPos.y + nodeRadius};
        line[1].color = Color::White;
        window.draw(line);

        parcurgere(radacina->stg, leftChildPos, arial, window, horizontalOffset, verticalOffset, depth + 1, maxDepth);
    }

    if (radacina->drp != NULL) {
        Vector2f rightChildPos = coordonate;
        rightChildPos.x += adjustedHorizontalOffset;
        rightChildPos.y += verticalOffset; 

        sf::VertexArray line(Lines, 2);
        line[0].position = { coordonate.x + nodeRadius, coordonate.y + nodeRadius};
        line[0].color = Color::White;
        line[1].position = { rightChildPos.x + nodeRadius, rightChildPos.y + nodeRadius};
        line[1].color = Color::White;
        window.draw(line);

        parcurgere(radacina->drp, rightChildPos, arial, window, horizontalOffset, verticalOffset, depth + 1, maxDepth);
    }
}

int main() {
    char infixExpr[100];
    char postfix[100];
    char* expresiefinala = nullptr;
    int functieArboreSwitch = -1;
    bool wasPressedOnce = false;
	string enteredText = "";
    string rezultat;
    bool doNotRepeat = false;
    Nod* arbore = new(Nod);
    Nod* arboreDerivat = new(Nod);
    bool intervine = false;
    int maxHorizontalLength;
    int v[2] = { 0 };

    RenderWindow window(VideoMode(1800, 1200), "DERIVARE");
    Vector2i centerWindow((VideoMode::getDesktopMode().width / 2) - 900, (VideoMode::getDesktopMode().height / 2) - 600);
    window.setPosition(centerWindow);
    window.setKeyRepeatEnabled(true);

    Font arial;
    arial.loadFromFile("ARIAL.ttf");
    
    TextBox textbox1(false);
    textbox1.setFont(arial);
    textbox1.setPosition({ 695, 100 });

    JustText functie({100, 40}, Color::Red, 30, arial), functieDerivata({100, 40}, Color::Blue, 30, arial);

    SwitchButton button1({ 200, 50 }, Color::Red, Color::White, 30);
	button1.setFont(arial);
	button1.setButtonPostionion({ 695, 200 });
	button1.assignText("Deriveaza");

    SwitchButton button2({ 200, 50 }, Color::Blue, Color::White, 30);
    button2.setFont(arial);
    button2.setButtonPostionion({ 905, 200 });
    button2.assignText("f''(x)");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::TextEntered) {
                textbox1.typedOn(event); 
            }
            if (event.type == Event::MouseMoved) {
                if (button1.isMouseOver(window)) {
                    button1.setColors(Color::Red, Color::White);
				}
                else {
                    button1.setColors(Color::White, Color::Red);
                }
                if (button2.isMouseOver(window)) {
                    button2.setColors(Color::Blue, Color::White);
                }
                else {
                    button2.setColors(Color::White, Color::Red);
                }
            }
            if (event.type == Event::MouseButtonPressed) {
                if (button1.isMouseOver(window)) {
					button1.setButtonPostionion({ 695, 100 });
					button2.setButtonPostionion({ 905, 100 });
                    wasPressedOnce = true;
                    button1.wasPressed();
					if (functieArboreSwitch % 2 == 0) 
                        button1.assignText("functie");
                    else
						button1.assignText("derivata");				
                }
                if (button2.isMouseOver(window)) {
                    button2.wasPressed();
                    wasPressedOnce = true;
                    enteredText = string(expresiefinala);
                    strcpy(postfix, "");
                    strcpy(infixExpr, expresiefinala);
                    intervine = true;
                    stergeArbore(arbore);
                    arbore = creareArborePostfix(postfix); 
                    stergeArbore(arboreDerivat);
                    arboreDerivat = derivare(arbore); 
                    arboreDerivat = prelucreazaSir(arboreDerivat);
                }
            }
        }
        
        if (wasPressedOnce) {
            wasPressedOnce = false;
            if (!intervine) {
                enteredText = textbox1.getSavedText();
                strcpy(infixExpr, textbox1.convertToChar());
            }
            if (!verificaExpresie(infixExpr)) {
				cout << infixExpr << endl;
                printf("Eroare: Functie incorecta\n");
                window.close();
            }
            infixToPostfix(infixExpr, postfix);
			cout << postfix << endl;
            arbore = creareArborePostfix(postfix);
            functie.setText("f(x) = " + enteredText);
            arboreDerivat = derivare(arbore);
            arboreDerivat = prelucreazaSir(arboreDerivat);
            expresiefinala = genereazaExpresiaFinala(arboreDerivat);
            functieDerivata.setText("f'(x) = " + string(expresiefinala));
			textbox1.modify();
			textbox1.buttonPressed();
        }

		if (button1.isButtonPressed()) {
            functieArboreSwitch++;
		}

        window.clear();
		button1.drawTo(window);
		button2.drawTo(window);
        
        if (functieArboreSwitch % 2 == 0) {
            functie.drawTo(window);
			int maxDepth = calculateMaxDepth(arbore); 
			iarParcurgere(NULL, NULL, arbore, window, 900, 200, arial, true);
        //    parcurgere(arbore, { 625, 300 }, arial, window, 35, 70, 0, maxDepth);  
        }
        else if (functieArboreSwitch != -1){
            functieDerivata.drawTo(window);
            int maxDepth = calculateMaxDepth(arboreDerivat);
			iarParcurgere(NULL, NULL, arboreDerivat, window, 900, 200, arial, true);
        //    parcurgere(arboreDerivat, { 625, 300 }, arial, window, 35, 70, 0, maxDepth);
        }
        
        textbox1.drawTo(window);
        window.display();
    }

    return 0;
}
