#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define MAX 100
struct Nod {
    char info[MAX];
    struct Nod* stg = NULL, * drp = NULL;
};

struct Stiva {
    struct Nod* noduri[100];
    int top;
};

void initStiva(struct Stiva* s) {
    s->top = -1;
}

int isEmpty(struct Stiva* s) {
    return s->top == -1;
}

void push(struct Stiva* s, struct Nod* node) {
    s->noduri[++(s->top)] = node;
}

struct Nod* pop(struct Stiva* s) {
    if (!isEmpty(s)) {
        return s->noduri[(s->top)--];
    }
    return NULL;
}

Nod* creareNod(const char* valoare) {
    struct Nod* newNode = (struct Nod*)malloc(sizeof(struct Nod));
    if (newNode == NULL) {
        printf("Eroare la alocarea memoriei\n");
        exit(1);
    }
    strncpy(newNode->info, valoare, MAX - 1);
    newNode->info[MAX - 1] = '\0';
    newNode->stg = newNode->drp = NULL;
    return newNode;
}

int verificaFunctieTrigonometrica(const char* postfix) {
    const char* functiiTrigonometrice[] = { "sin","cos","tg","ctg","arcsin","arccos","arctg","arcctg","sqrt","ln" };
    int numarFunctii = sizeof(functiiTrigonometrice) / sizeof(functiiTrigonometrice[0]);
    for (int i = 0;i < numarFunctii;i++) {
        if (strcmp(postfix, functiiTrigonometrice[i]) == 0)
            return 1;
    }
    return 0;
}

int esteOperator(char c) {
    if (c == '+' || c == '^' || c == '-' || c == '/' || c == '*')
        return 1;
    else return 0;
}

int EsteOperator(char* c) {
    if (c == "+" || c == "^" || c == "-" || c == "/" || c == "*")
        return 1;
    else return 0;
}

int precedenta(char* op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        return 2;
    if ((strcmp(op, "^") == 0))
        return 3;
    if (verificaFunctieTrigonometrica(op))
        return 4;
    return 0;
}

struct Nod* creareArborePostfix(char* postfix) {
    struct Stiva stiva;
    initStiva(&stiva);

    char buffer[10];
    int j = 0;
    int asteptamNumar = 1; // Flag pentru a ști daca urmatorul '-' este parte dintr-un numar

    for (int i = 0; postfix[i] != '\0'; i++) {
        if (isspace(postfix[i])) {
            if (j > 0) {
                buffer[j] = '\0';
                j = 0;

                if (verificaFunctieTrigonometrica(buffer)) {
                    struct Nod* node = creareNod(buffer);
                    node->drp = pop(&stiva);
                    node->stg = NULL;
                    push(&stiva, node);
                    asteptamNumar = 1;
                }
                else if (buffer[0] == '-' && strlen(buffer) > 1) {
                    // Este un numar negativ
                    struct Nod* node = creareNod(buffer);
                    push(&stiva, node);
                    asteptamNumar = 0;
                }
                else if (isalnum(buffer[0]) || (buffer[0] == '-' && isdigit(buffer[1]))) {
                    // Operanzi (numere/variabile), inclusiv numere negative
                    struct Nod* node = creareNod(buffer);
                    push(&stiva, node);
                    asteptamNumar = 0;
                }
                else { // Operator binar
                    struct Nod* node = creareNod(buffer);
                    node->drp = pop(&stiva);
                    node->stg = pop(&stiva);
                    push(&stiva, node);
                    asteptamNumar = 1;
                }
            }
        }
        else {
            // Tratam cazul special pentru minus
            if (postfix[i] == '-' && asteptamNumar && j == 0) {
                // Este un minus care face parte dintr-un numar negativ
                buffer[j++] = postfix[i];
            }
            else {
                buffer[j++] = postfix[i];
            }
        }
    }

    // Verificam daca mai sunt elementeIn buffer
    if (j > 0) {
        buffer[j] = '\0';
        if (verificaFunctieTrigonometrica(buffer)) {
            struct Nod* node = creareNod(buffer);
            node->drp = pop(&stiva);
            node->stg = NULL;
            push(&stiva, node);
        }
        else if (buffer[0] == '-' && strlen(buffer) > 1) {
            // Este un numar negativ
            struct Nod* node = creareNod(buffer);
            push(&stiva, node);
        }
        else if (isalnum(buffer[0]) || (buffer[0] == '-' && isdigit(buffer[1]))) {
            struct Nod* node = creareNod(buffer);
            push(&stiva, node);
        }
        else {
            struct Nod* node = creareNod(buffer);
            node->drp = pop(&stiva);
            node->stg = pop(&stiva);
            push(&stiva, node);
        }
    }

    return pop(&stiva);
}

void stergeArbore(struct Nod* nod) {
    if (nod == NULL) {
        return;
    }
    stergeArbore(nod->drp);
    stergeArbore(nod->stg);
    free(nod);
}

void Postordine(struct Nod* a) {
    if (a == NULL) {
        return;
    }
    Postordine(a->stg);
    Postordine(a->drp);
    printf("%s", a->info);
}

int verificaExpresie(const char* expresie) {
    int paranteze = 0;
    int asteaptaOperand = 1;
    int lungime = strlen(expresie);
    char token[MAX];
    int j = 0;
    int potFiNumereNegative = 1; // LaInceput sau dupa operator/paranteza deschisa putem avea numere negative

    for (int i = 0; i < lungime; i++) {
        if (isspace(expresie[i])) {
            continue;
        }

        if (expresie[i] == '(') {
            paranteze++;
            asteaptaOperand = 1;
            potFiNumereNegative = 1; // Dupa paranteza deschisa putem avea numar negativ
        }
        else if (expresie[i] == ')') {
            paranteze--;
            if (paranteze < 0 || asteaptaOperand) {
                return 0;
            }
            asteaptaOperand = 0;
            potFiNumereNegative = 0; // Dupa parantezaInchisa nu putem avea numar negativ
        }
        else if (isalpha(expresie[i])) {
            j = 0;
            while (i < lungime && isalpha(expresie[i])) {
                token[j++] = expresie[i++];
            }
            token[j] = '\0';
            i--;

            if (verificaFunctieTrigonometrica(token)) {
                if (expresie[i + 1] != '(') {
                    return 0;
                }
                potFiNumereNegative = 1; // Dupa funcție (și paranteza ei) putem avea numar negativ
            }
            else if (strlen(token) > 1) {
                return 0;
            }

            asteaptaOperand = 0;
            potFiNumereNegative = 0;
        }
        else if (isdigit(expresie[i]) || expresie[i] == '.') {
            if (!asteaptaOperand && !potFiNumereNegative) {
                return 0;
            }
            while (i < lungime && (isdigit(expresie[i]) || expresie[i] == '.')) {
                i++;
            }
            i--;
            asteaptaOperand = 0;
            potFiNumereNegative = 0;
        }
        else if (expresie[i] == '-') {
            if (potFiNumereNegative) {
                // Este un minus unar (numar negativ)
                i++;
                if (i >= lungime || (!isdigit(expresie[i]) && expresie[i] != '.')) {
                    return 0; // Dupa minus trebuie sa urmeze un numar
                }
                while (i < lungime && (isdigit(expresie[i]) || expresie[i] == '.')) {
                    i++;
                }
                i--;
                asteaptaOperand = 0;
                potFiNumereNegative = 0;
            }
            else {
                // Este operator binar de scadere
                if (asteaptaOperand) {
                    return 0;
                }
                asteaptaOperand = 1;
                potFiNumereNegative = 1;
            }
        }
        else if (esteOperator(expresie[i])) {
            if (expresie[i] == '/' && expresie[i + 1] == '0') {
                return 0;
            }
            if (asteaptaOperand) {
                return 0;
            }
            asteaptaOperand = 1;
            potFiNumereNegative = 1; // Dupa operator putem avea numar negativ
        }
        else {
            return 0;
        }
    }

    return paranteze == 0 && !asteaptaOperand;
}


void infixToPostfix(char* infix, char* postfix) {
    struct Stiva operatori;
    initStiva(&operatori);

    char token[MAX] = { 0 };
    int j = 0;
    int k = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        if (isspace(infix[i])) continue;  //Daca sunt spatii le ignoram

        if (isalpha(infix[i])) {
            k = 0;
            while (isalpha(infix[i])) {//citim pana cand se termina alfanumericele
                token[k++] = infix[i++];
            }
            token[k] = '\0';
            i--;
            //Verificam daca e sqrt sau o functie trigonometrica
            if (verificaFunctieTrigonometrica(token)) {
                push(&operatori, creareNod(token)); //Daca da adaugam in stiva nodul
            }
            else {
                sprintf(postfix + j, "%s ", token);
                j += strlen(token) + 1;
            }
        }
        else if (isalnum(infix[i])) {
            k = 0;
            while (isalnum(infix[i])) {
                token[k++] = infix[i++];
            }
            token[k] = '\0';
            i--;
            sprintf(postfix + j, "%s ", token);
            j += strlen(token) + 1;
        }
        //Daca este paranteza deschisa o adaugam in stiva
        else if (infix[i] == '(') {
            push(&operatori, creareNod("("));
        }
        //Daca e paranteza inchisa scoatem toate nodurile din stiva
        else if (infix[i] == ')') {
            while (!isEmpty(&operatori) && strcmp(operatori.noduri[operatori.top]->info, "(") != 0) {
                j += sprintf(postfix + j, "%s ", operatori.noduri[operatori.top]->info);
                pop(&operatori);
            }
            pop(&operatori);
            //Mai verificam daca e sau nu o functie trigonometrica
            if (!isEmpty(&operatori) && verificaFunctieTrigonometrica(operatori.noduri[operatori.top]->info)) {
                j += sprintf(postfix + j, "%s ", operatori.noduri[operatori.top]->info);
                pop(&operatori);
            }
        }
        else {
            char op[2] = { infix[i], '\0' };
            while (!isEmpty(&operatori) && precedenta(operatori.noduri[operatori.top]->info) >= precedenta(op)) {
                j += sprintf(postfix + j, "%s ", operatori.noduri[operatori.top]->info);
                pop(&operatori);
            }
            push(&operatori, creareNod(op));
        }
    }
    //Daca au ramas operatori in stiva;
    while (!isEmpty(&operatori)) {
        j += sprintf(postfix + j, "%s ", operatori.noduri[operatori.top]->info);
        pop(&operatori);
    }

    postfix[j] = '\0';
}

void copiazaSir(Nod* nod, char* sir) {
    if (nod != NULL) {
        copiazaSir(nod->stg, sir);

        strcat(sir, nod->info);

        copiazaSir(nod->drp, sir);
    }
}

void Inordine(Nod* nod) {
    if (nod != NULL) {
        Inordine(nod->stg);
        printf("%s", nod->info);
        Inordine(nod->drp);
    }
}

Nod* prelucreazaSir(Nod* nod) {
    if (nod == NULL) return NULL;

    nod->stg = prelucreazaSir(nod->stg);
    nod->drp = prelucreazaSir(nod->drp);

    if (strcmp(nod->info, "-") == 0) {
        if (nod->stg != NULL && strcmp(nod->stg->info, "0") == 0) {
            //In loc sa pastram "0-expresie", transformamIn "-expresie"
            Nod* temp = nod->drp;
            free(nod->stg);  // Eliberam nodul cu "0"
            free(nod);

            // Cream un nou nod pentru minus
            Nod* nouNod = (Nod*)malloc(sizeof(Nod));
            strcpy(nouNod->info, "-");
            nouNod->stg = NULL;  // Operatorul unar minus nu are copil stâng
            nouNod->drp = temp;  // Copilul drept este expresia originala
            return nouNod;
        }

        // Restul cazurilor pentru scadere ramân neschimbate
        if (nod->drp != NULL && strcmp(nod->drp->info, "0") == 0) {
            Nod* temp = nod->stg;
            free(nod->drp);
            free(nod);
            return temp;
        }
    }

    // Cazul 1: Adunarea cu 0
    if (strcmp(nod->info, "+") == 0) {
        // Daca operandul stâng este 0, returnam operandul drept
        if (nod->stg != NULL && strcmp(nod->stg->info, "0") == 0) {
            Nod* temp = nod->drp;
            free(nod->stg);
            free(nod);
            return temp;
        }
        // Daca operandul drept este 0, returnam operandul stâng
        if (nod->drp != NULL && strcmp(nod->drp->info, "0") == 0) {
            Nod* temp = nod->stg;
            free(nod->drp);
            free(nod);
            return temp;
        }
    }

    // Cazul 2: Scaderea cu 0
    if (strcmp(nod->info, "-") == 0) {
        // Daca operandul drept este 0, returnam operandul stâng
        if (nod->drp != NULL && strcmp(nod->drp->info, "0") == 0) {
            Nod* temp = nod->stg;
            free(nod->drp);
            free(nod);
            return temp;
        }
    }

    // Cazul 3:Inmultirea cu 0
    if (strcmp(nod->info, "*") == 0) {
        // Daca unul din operanzi este 0, returnam 0
        if ((nod->stg != NULL && strcmp(nod->stg->info, "0") == 0) ||
            (nod->drp != NULL && strcmp(nod->drp->info, "0") == 0)) {
            stergeArbore(nod->stg);
            stergeArbore(nod->drp);
            strcpy(nod->info, "0");
            nod->stg = NULL;
            nod->drp = NULL;
            return nod;
        }
        //Inmultirea cu 1
        if (nod->stg != NULL && strcmp(nod->stg->info, "1") == 0) {
            Nod* temp = nod->drp;
            free(nod->stg);
            free(nod);
            return temp;
        }
        if (nod->drp != NULL && strcmp(nod->drp->info, "1") == 0) {
            Nod* temp = nod->stg;
            free(nod->drp);
            free(nod);
            return temp;
        }
    }

    // Cazul 4:Impartirea cu 1
    if (strcmp(nod->info, "/") == 0) {
        if (nod->drp != NULL && strcmp(nod->drp->info, "1") == 0) {
            Nod* temp = nod->stg;
            free(nod->drp);
            free(nod);
            return temp;
        }
    }

    // Cazul 5: Puterea la 1
    if (strcmp(nod->info, "^") == 0) {
        if (nod->drp != NULL && strcmp(nod->drp->info, "1") == 0) {
            Nod* temp = nod->stg;
            free(nod->drp);
            free(nod);
            return temp;
        }
        // Puterea la 0
        if (nod->drp != NULL && strcmp(nod->drp->info, "0") == 0) {
            stergeArbore(nod->stg);
            stergeArbore(nod->drp);
            strcpy(nod->info, "1");
            nod->stg = NULL;
            nod->drp = NULL;
            return nod;
        }
    }

    return nod;
}
Nod* copiazaNod(Nod* sursa) {
    if (sursa == NULL) return NULL;

    Nod* nou = creareNod(sursa->info);

    // Alocam memorie pentru subnoduri
    nou->stg = copiazaNod(sursa->stg);
    nou->drp = copiazaNod(sursa->drp);

    return nou;
}

Nod* derivare(struct Nod* nod);

int esteVariabila(const char* info) {
    return (strcmp(info, "x") == 0);
}

bool alfanumeric(Nod* tata)
{
    int yuh = 0;
    if (strchr(tata->stg->info, '*') != NULL || strchr(tata->stg->info, '/') != NULL || strchr(tata->stg->info, '^') != NULL || strstr(tata->stg->info, "sqrt") != NULL)
        yuh++;
    if (strchr(tata->drp->info, '*') != NULL || strchr(tata->drp->info, '/') != NULL || strchr(tata->drp->info, '^') != NULL || strstr(tata->drp->info, "sqrt") != NULL)
        yuh++;
    if (yuh > 0)
        return false;
    return true;
}

Nod* constant(Nod* functie)
{
    struct Nod* rezultat = creareNod("0");
    return rezultat;
}

Nod* x_noConstant(Nod* functie)
{
    struct Nod* rezultat = creareNod("1");
    return rezultat;
}

Nod* suma(struct Nod* nod) {
    // cream un nod pt suma derivatelor
    Nod* rezultat = creareNod("+");

    // Derivam operandul stang
    rezultat->stg = derivare(nod->stg);

    // Derivam operandul drept
    rezultat->drp = derivare(nod->drp);

    return rezultat;
}

Nod* scadere(struct Nod* nod) {
    Nod* rezultat = creareNod("-");
    rezultat->stg = derivare(nod->stg);
    rezultat->drp = derivare(nod->drp);

    return rezultat;
}

Nod* inmultire(struct Nod* nod) {
    // Cream un nod pentru suma derivatelor
    Nod* rezultat = creareNod("+");

    // Derivam operandul stâng
    Nod* derivataStanga = derivare(nod->stg); //f'
    Nod* operandDrept = copiazaNod(nod->drp); //g

    // Primul termen: f' * g
    Nod* termen1 = creareNod("*");
    termen1->stg = derivataStanga;
    termen1->drp = operandDrept;

    // Derivam operandul drept
    Nod* derivataDreapta = derivare(nod->drp);//g'
    Nod* operandStang = copiazaNod(nod->stg); //f

    // Al doilea termen: f * g'
    Nod* termen2 = creareNod("*");
    termen2->stg = operandStang;
    termen2->drp = derivataDreapta;

    // Adaugam cei doi termeniIn nodul rezultat
    rezultat->stg = termen1;
    rezultat->drp = termen2;

    return rezultat;
}


Nod* impartire(struct Nod* nod) {
    // Cream un nod pentru rezultatul derivarii
    Nod* rezultat = creareNod("/");

    // Numitorul (g^2)
    Nod* numitor = creareNod("^");
    numitor->stg = copiazaNod(nod->drp); // g
    numitor->drp = creareNod("2"); // 2

    // Numaratorul (f' * g - f * g')
    Nod* numarator = creareNod("-");

    // Derivam operandul stang (f)
    Nod* derivataStanga = derivare(nod->stg); // f'
    Nod* operandDrept = copiazaNod(nod->drp); // g

    // Primul termen f' * g
    Nod* termen1 = creareNod("*");
    termen1->stg = derivataStanga;
    termen1->drp = operandDrept;

    // Derivam operandul drept (g)
    Nod* derivataDreapta = derivare(nod->drp);//g'
    Nod* operandStang = copiazaNod(nod->stg); // f

    // Al doilea termen f * g'
    Nod* termen2 = creareNod("*");
    termen2->stg = operandStang;
    termen2->drp = derivataDreapta;

    // Adaugam cei doi termeni la numarator
    numarator->stg = termen1;
    numarator->drp = termen2;

    // Adaugam numitorul si numaratorul la rezultat
    rezultat->stg = numarator;
    rezultat->drp = numitor;

    return rezultat;
}

Nod* exponent_x(Nod* nod)
{
    //n*u^(n-1)*u'

    Nod* derivata = creareNod("*");

    Nod* xDerivat = derivare(nod->stg);

    Nod* rezultat = creareNod("*");

    int n = atoi(nod->drp->info);

    Nod* factorN = creareNod("n");
    _itoa(n, factorN->info, 10);

    rezultat->stg = factorN;

    Nod* putere = creareNod("^");
    putere->stg = copiazaNod(nod->stg);

    Nod* exponentNou = creareNod("n-1");
    n = n - 1;
    _itoa(n, exponentNou->info, 10);  // Salvam n-1In nodul exponentului

    putere->drp = exponentNou;

    rezultat->drp = putere;

    derivata->stg = rezultat;
    derivata->drp = xDerivat;

    return derivata;
}


Nod* radical(Nod* nod)
{
    // 1/2sqrt(u) * u'
    Nod* rezultat = creareNod("*");

    Nod* impartire = creareNod("/");
    impartire->stg = creareNod("1");

    Nod* derivataFunctie = derivare(nod->drp);//u'
    Nod* functie = copiazaNod(nod->drp);//u

    Nod* inmultit = creareNod("*");
    inmultit->stg = creareNod("2");

    Nod* radical = creareNod("sqrt");
    radical->drp = copiazaNod(functie);

    rezultat->stg = impartire;
    rezultat->drp = derivataFunctie;
    impartire->drp = inmultit;
    inmultit->drp = radical;

    return rezultat;

}

Nod* e_to_x(Nod* nod)
{
    // e^x * x'
    Nod* rezultat = creareNod("*");
    rezultat->stg = copiazaNod(nod);
    rezultat->drp = derivare(nod->drp);
    return rezultat;
}

Nod* a_to_x(Nod* nod)
{
    // a^x * ln(a) * x'
    Nod* rezultat = creareNod("*");
    Nod* xDerivat = derivare(nod->drp);
    rezultat->drp = xDerivat;

    Nod* inmultit = creareNod("*");
    inmultit->stg = copiazaNod(nod);

    Nod* logaritm = creareNod("ln");
    logaritm->drp = copiazaNod(nod->stg);

    rezultat->stg = inmultit;
    inmultit->drp = logaritm;

    return rezultat;
}

Nod* ln_x(Nod* nod)
{   //1/u * u'
    Nod* rezultat = creareNod("*");
    Nod* impartit = creareNod("/");
    impartit->stg = creareNod("1");
    impartit->drp = copiazaNod(nod->drp);
    rezultat->stg = impartit;
    rezultat->drp = derivare(nod->drp);

    return rezultat;
}

Nod* sin_x(Nod* functie) {
    // cos(u) * u'
    Nod* rezultat = creareNod("*");
    Nod* cosinus = creareNod("cos");
    cosinus->drp = copiazaNod(functie->drp);
    rezultat->drp = derivare(functie->drp); //u'
    rezultat->stg = cosinus;
    return rezultat;
}
//nu merge bine nush de ce
Nod* cos_x(Nod* functie) {
    //-sin(x)*x'
    Nod* rezultat = creareNod("*");

    Nod* inmultit = creareNod("*");
    inmultit->stg = creareNod("-1");

    Nod* sinus = creareNod("sin");
    sinus->drp = copiazaNod(functie->drp);

    inmultit->drp = sinus;

    rezultat->stg = inmultit;
    rezultat->drp = derivare(functie->drp);

    return rezultat;
}

Nod* tg_x(Nod* functie)
{
    //1/cos^2 u * u'
    Nod* rezultat = creareNod("*");
    rezultat->drp = derivare(functie->drp);

    Nod* impartit = creareNod("/");
    impartit->stg = creareNod("1");

    Nod* cosul = creareNod("cos");
    cosul->drp = copiazaNod(functie->drp);
    Nod* putere = creareNod("^");

    impartit->drp = putere;
    putere->drp = creareNod("2");
    putere->stg = cosul;

    rezultat->stg = impartit;

    return rezultat;

}

Nod* ctg_x(Nod* functie)
{
    //-1/cos^2 u * u'
    Nod* rezultat = creareNod("*");
    rezultat->drp = derivare(functie->drp);

    Nod* impartit = creareNod("/");
    impartit->stg = creareNod("-1");

    Nod* sinus = creareNod("sin");
    sinus->drp = copiazaNod(functie->drp);
    Nod* putere = creareNod("^");

    impartit->drp = putere;
    putere->drp = creareNod("2");
    putere->stg = sinus;

    rezultat->stg = impartit;

    return rezultat;

}
Nod* arcsin_x(Nod* functie)
{
    Nod* rezultat = creareNod("*");
    rezultat->drp = derivare(functie->drp);

    Nod* impartit = creareNod("/");
    rezultat->stg = impartit;
    impartit->stg = creareNod("1");

    Nod* sqrt = creareNod("sqrt");
    impartit->drp = sqrt;

    Nod* minus = creareNod("-");
    sqrt->drp = minus;
    minus->stg = creareNod("1");

    Nod* putere = creareNod("^");
    minus->drp = putere;
    putere->drp = creareNod("2");
    putere->stg = copiazaNod(functie->drp);

    return rezultat;
}

Nod* arccos_x(Nod* functie)
{
    Nod* rezultat = creareNod("*");
    rezultat->drp = derivare(functie->drp);

    Nod* impartit = creareNod("/");
    rezultat->stg = impartit;
    impartit->stg = creareNod("-1");

    Nod* sqrt = creareNod("sqrt");
    impartit->drp = sqrt;

    Nod* minus = creareNod("-");
    sqrt->drp = minus;
    minus->stg = creareNod("1");

    Nod* putere = creareNod("^");
    minus->drp = putere;
    putere->drp = creareNod("2");
    putere->stg = copiazaNod(functie->drp);

    return rezultat;
}

Nod* arctg_x(Nod* functie)
{   // 1/(u^2 +1) * u'
    Nod* rezultat = creareNod("*");
    rezultat->drp = derivare(functie->drp);

    Nod* impartit = creareNod("/");
    rezultat->stg = impartit;
    impartit->stg = creareNod("1");

    Nod* adunare = creareNod("+");
    impartit->drp = adunare;
    adunare->drp = creareNod("1");

    Nod* putere = creareNod("^");
    adunare->stg = putere;
    putere->stg = copiazaNod(functie->drp);
    putere->drp = creareNod("2");

    return rezultat;
}

Nod* arcctg_x(Nod* functie)
{   // -1/(u^2 +1) * u'
    Nod* rezultat = creareNod("*");
    rezultat->drp = derivare(functie->drp);

    Nod* impartit = creareNod("/");
    rezultat->stg = impartit;
    impartit->stg = creareNod("-1");

    Nod* adunare = creareNod("+");
    impartit->drp = adunare;
    adunare->drp = creareNod("1");

    Nod* putere = creareNod("^");
    adunare->stg = putere;
    putere->stg = copiazaNod(functie->drp);
    putere->drp = creareNod("2");

    return rezultat;
}

Nod* putere_generala(Nod* nod) {
    // Verificam daca este cazul ln(x)^n
    if (strcmp(nod->stg->info, "ln") == 0 && esteNumar(nod->drp->info) == 0) {
        Nod* prod = creareNod("*");

        // n
        prod->stg = copiazaNod(nod->drp);

        // ln(x)^(n-1)
        Nod* putere = creareNod("^");
        putere->stg = copiazaNod(nod->stg);
        putere->stg->drp = copiazaNod(nod->stg->drp);  // x esteIn dreapta pentru ln

        char buf[MAX];
        snprintf(buf, MAX - 1, "%d", atoi(nod->drp->info) - 1);
        putere->drp = creareNod(buf);

        // 1/x
        Nod* div = creareNod("/");
        div->stg = creareNod("1");
        div->drp = copiazaNod(nod->stg->drp);  // x din ln(x)

        Nod* prod2 = creareNod("*");
        prod2->stg = putere;
        prod2->drp = div;

        prod->drp = prod2;
        return prod;
    }

    Nod* prod_final = creareNod("*");
    prod_final->stg = copiazaNod(nod);  // f(x)^g(x)

    Nod* plus = creareNod("+");

    // Primul termen: g(x) * f'(x)/f(x)
    Nod* prod1 = creareNod("*");
    prod1->stg = copiazaNod(nod->drp);  // g(x)

    Nod* div = creareNod("/");
    div->stg = derivare(nod->stg);      // f'(x)
    div->drp = copiazaNod(nod->stg);    // f(x)

    prod1->drp = div;

    // Al doilea termen: g'(x) * ln(f(x))
    Nod* prod2 = creareNod("*");
    prod2->stg = derivare(nod->drp);    // g'(x)

    Nod* ln = creareNod("ln");
    ln->drp = copiazaNod(nod->stg);     // ln(f(x))
    prod2->drp = ln;

    plus->stg = prod1;
    plus->drp = prod2;

    prod_final->drp = plus;
    return prod_final;
}

bool esteNumar(const char* str) {
    if (str == NULL) return false;

    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

Nod* derivare(struct Nod* nod) {
    if (nod == NULL) return NULL;

    // Operatori de baza (+, -)
    if (strcmp(nod->info, "+") == 0) {
        return suma(nod); // derivata sumei este suma derivatelor
    }
    if (strcmp(nod->info, "-") == 0) {
        return scadere(nod); // derivata diferentei este diferenta derivatelor
    }

    // Operator deInmultire (*)
    if (strcmp(nod->info, "*") == 0) {
        return inmultire(nod); // derivata produsului: f'g + fg'
    }

    // Operator deImpartire (/)
    if (strcmp(nod->info, "/") == 0) {
        return impartire(nod); // derivataImpartirii: (f'g - fg')/g^2
    }

    // Operator de putere (^)
    // Operator de putere (^)
    if (strcmp(nod->info, "^") == 0) {
        // Verificăm mai întâi cazurile speciale

        // e^x -> e^x
        if (strcmp(nod->stg->info, "e") == 0 && strcmp(nod->drp->info, "x") == 0) {
            return e_to_x(nod);
        }
        // x^n unde n este număr
        else if (strcmp(nod->stg->info, "x") == 0 && esteNumar(nod->drp->info)) {
            return exponent_x(nod);
        }
        // a^x unde a este număr
        else if (esteNumar(nod->stg->info) && strcmp(nod->drp->info, "x") == 0) {
            return a_to_x(nod);
        }
        // Pentru ORICE alt caz (inclusiv x^sin(x), x^sqrt(x), etc)
        else {
            return putere_generala(nod);
        }
    }
    // Functii trigonometrice
    if (strcmp(nod->info, "sin") == 0) {
        return sin_x(nod); // d/dx(sin(x)) = cos(x)
    }
    if (strcmp(nod->info, "cos") == 0) {
        return cos_x(nod); // d/dx(cos(x)) = -sin(x)
    }
    if (strcmp(nod->info, "tg") == 0) {
        return tg_x(nod); // d/dx(tg(x)) = 1/cos^2(x)
    }
    if (strcmp(nod->info, "ctg") == 0) {
        return ctg_x(nod); // d/dx(ctg(x)) = -1/sin^2(x)
    }

    // Functii trigonometrice inverse
    if (strcmp(nod->info, "arcsin") == 0) {
        return arcsin_x(nod); // d/dx(arcsin(x)) = 1/sqrt(1-x^2)
    }
    if (strcmp(nod->info, "arccos") == 0) {
        return arccos_x(nod); // d/dx(arccos(x)) = -1/sqrt(1-x^2)
    }
    if (strcmp(nod->info, "arctg") == 0) {
        return arctg_x(nod); // d/dx(arctg(x)) = 1/(1+x^2)
    }
    if (strcmp(nod->info, "arcctg") == 0) {
        return arcctg_x(nod); // d/dx(arcctg(x)) = -1/(1+x^2)
    }

    // Functii logaritmice și exponentiale
    if (strcmp(nod->info, "ln") == 0) {
        return ln_x(nod); // d/dx(ln(x)) = 1/x
    }

    // Functia radical
    if (strcmp(nod->info, "sqrt") == 0) {
        return radical(nod); // d/dx(sqrt(x)) = 1/(2*sqrt(x))
    }

    // Variabila x și constante
    if (strcmp(nod->info, "x") == 0) {
        return x_noConstant(nod); // d/dx(x) = 1
    }
    if (esteNumar(nod->info)) {
        return constant(nod); // d/dx(c) = 0
    }
    // Cazul default pentru orice alta situatie neacoperita
    return NULL;
}

int getPrioritate(const char* op) {
    if (strcmp(op, "^") == 0) return 4;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 3;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 2;
    return 1; // pentru funcții și operanzi
}

// Verifică dacă este funcție matematică
int esteFunctie(const char* str) {
    return strcmp(str, "sin") == 0 || strcmp(str, "cos") == 0 ||
        strcmp(str, "tg") == 0 || strcmp(str, "ctg") == 0 ||
        strcmp(str, "arcsin") == 0 || strcmp(str, "arccos") == 0 ||
        strcmp(str, "arctg") == 0 || strcmp(str, "arcctg") == 0 ||
        strcmp(str, "ln") == 0 || strcmp(str, "sqrt") == 0 ||
        strcmp(str, "log") == 0;
}

// Verifică dacă un operator necesită paranteze pentru operanzi
bool necesitaParantezeOperand(const char* parinte, const char* copil, bool eDreapta) {
    // Dacă copilul nu e operator, nu necesită paranteze
    if (strchr("+-*/^", copil[0]) == NULL) return false;

    int prioritateParinte = getPrioritate(parinte);
    int prioritateCopil = getPrioritate(copil);

    // Pentru exponentiere
    if (strcmp(parinte, "^") == 0) {
        if (eDreapta) return true;  // Mereu punem paranteze pentru exponentul complex
        return prioritateCopil < prioritateParinte;
    }

    // Pentru înmulțire și împărțire
    if (strcmp(parinte, "*") == 0 || strcmp(parinte, "/") == 0) {
        if (prioritateCopil < prioritateParinte) return true;
        if (eDreapta && strcmp(parinte, "/") == 0) return true;
        return false;
    }

    // Pentru adunare și scădere
    if (prioritateCopil <= prioritateParinte) return true;

    return false;
}

void genereazaExpresie(Nod* nod, char* expresie, int* pozitie) {
    if (nod == NULL) return;

    // Tratăm funcțiile matematice
    if (esteFunctie(nod->info)) {
        *pozitie += sprintf(expresie + *pozitie, "%s(", nod->info);
        genereazaExpresie(nod->drp, expresie, pozitie);
        *pozitie += sprintf(expresie + *pozitie, ")");
        return;
    }

    bool esteOperator = strchr("+-*/^", nod->info[0]) != NULL;
    bool necesitaParantezeStg = nod->stg && esteOperator &&
        necesitaParantezeOperand(nod->info, nod->stg->info, false);
    bool necesitaParantezeDrp = nod->drp && esteOperator &&
        necesitaParantezeOperand(nod->info, nod->drp->info, true);

    // Cazuri speciale pentru derivate complexe
    if (strcmp(nod->info, "*") == 0 || strcmp(nod->info, "+") == 0) {
        // Verificăm dacă avem o expresie de tipul f(x)^g(x) * derivată
        if (nod->stg != NULL && strcmp(nod->stg->info, "^") == 0) {
            necesitaParantezeStg = true;
        }
        // Verificăm dacă avem operații complexe în dreapta
        if (nod->drp != NULL && strchr("+-*/", nod->drp->info[0]) != NULL) {
            necesitaParantezeDrp = true;
        }
    }

    // Procesăm partea stângă
    if (nod->stg != NULL) {
        if (necesitaParantezeStg) *pozitie += sprintf(expresie + *pozitie, "(");
        genereazaExpresie(nod->stg, expresie, pozitie);
        if (necesitaParantezeStg) *pozitie += sprintf(expresie + *pozitie, ")");
    }

    // Adăugăm operatorul
    if (esteOperator) {
        *pozitie += sprintf(expresie + *pozitie, "%s", nod->info);
    }
    else if (!esteFunctie(nod->info)) {
        *pozitie += sprintf(expresie + *pozitie, "%s", nod->info);
    }

    // Procesăm partea dreaptă
    if (nod->drp != NULL && !esteFunctie(nod->info)) {
        if (necesitaParantezeDrp) *pozitie += sprintf(expresie + *pozitie, "(");
        genereazaExpresie(nod->drp, expresie, pozitie);
        if (necesitaParantezeDrp) *pozitie += sprintf(expresie + *pozitie, ")");
    }
}

char* genereazaExpresiaFinala(Nod* nod) {
    char* expresie = (char*)malloc(100000 * sizeof(char));
    int pozitie = 0;
    genereazaExpresie(nod, expresie, &pozitie);
    expresie[pozitie] = '\0';
    return expresie;
}

void genereazaExpresieS(struct Nod* nod, std::string& expresie, int prioritateParinte) {
    if (!nod) return;

    // Daca este un operand,Il adaugam direct
    if (!nod->stg && !nod->drp) {
        expresie += nod->info;
        return;
    }

    int prioritateCurenta = precedenta(nod->info);
    int necesitaParanteze = prioritateCurenta < prioritateParinte;

    if (necesitaParanteze) expresie += '(';

    // Procesam copilul stâng
    genereazaExpresieS(nod->stg, expresie, prioritateCurenta);

    // Adaugam operatorul sau functia
    expresie += nod->info;

    // Procesam copilul drept
    genereazaExpresieS(nod->drp, expresie, prioritateCurenta);

    if (necesitaParanteze)  expresie += ')';
}

char* stringToChar(string s) {
    char c[100];
    int i;
    for (i = 0; i < s.size(); i++) {
        c[i] = s[i];
    }
    c[i] = '\0';
    return c;
}
