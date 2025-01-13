#include <string>
struct Nod;
struct Stiva;
bool alfanumeric(Nod* tata);
Nod* constant(Nod* functie);
Nod* x_noConstant(Nod* functie);
Nod* exponent_x(Nod* tata);
Nod* radical(Nod* tata);
Nod* e_to_x(Nod* tata);
Nod* a_to_x(Nod* tata);
Nod* ln_x(Nod* functie);
Nod* arctg_x(Nod* functie);
Nod* arcctg_x(Nod* functie);
Nod* arcsin_x(Nod* functie);
Nod* arccos_x(Nod* functie);
Nod* sin_x(Nod* functie);
Nod* cos_x(Nod* functie);
Nod* tg_x(Nod* functie);
Nod* ctg_x(Nod* functie);
Nod* x_plus_minus_a(Nod* tata);
Nod* a_minus_x(Nod* tata);
void initStiva(struct Stiva* s);
int isEmpty(struct Stiva* s);
void push(struct Stiva* s, struct Nod* node);
struct Nod* pop(struct Stiva* s);
Nod* creareNod(const char* valoare);
int verificaFunctieTrigonometrica(const char* postfix);
int esteOperator(char c);
int precedenta(char* op);
struct Nod* creareArborePostfix(char* postfix);
void stergeArbore(struct Nod* nod);
void Postordine(struct Nod* a);
int verificaExpresie(const char* expresie);
void infixToPostfix(const char* infix, char* postfix);
struct Nod* copiazaNod(struct Nod* sursa);
void genereazaExpresie(Nod* nod, char* expresie, int* pozitie);
Nod* prelucreazaSir(Nod* nod);
void copiazaSir(Nod* nod, char* sir);
Nod* putere_generala(Nod* nod);
bool esteNumar(const char* str);
void genereazaExpresie(Nod* nod, char* expresie, int* pozitie);
char* genereazaExpresiaFinala(Nod* nod);
char* stringToChar(std::string s);
void genereazaExpresieS(struct Nod* nod, std::string& expresie, int prioritateParinte);
