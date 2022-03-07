#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <unordered_map>
#include <stack>
#include <fstream>
#include <deque>
#include <queue>
#include <csignal>
using namespace std;

ifstream fin("a.txt");

int MaxAge = 20;//la alegerea progrmatorrului
double constGivingBirth = 0.2;
double constGettingOld = 0.125;

class Individ {
    int i, j;
    int varsta;
    double energie;
    unsigned char viu;
    char tip;
    //metode private
    friend void feed(int, int, Individ**&);
    void adauga(Individ&);
    void die();
    void getsOld();

public:
    Individ(int, int, bool);
    Individ();

    friend void giving_birth(int, int, Individ**&);
    void update();
    bool isAlive();

};

//constructori
Individ::Individ() {
    i = 0;
    j = 0;
    varsta = 0;
    energie = 0; // valoare constanta aleasa de programator
    viu = '0';
    //trebuie aplicata functia alocare de la tabelSpecii
};

Individ::Individ(int pi, int pj, bool type) {
    i = pi;
    j = pj;
    varsta = 0;
    energie = 3; // valoare constanta aleasa de programator
    viu = '1';
    //trebuie aplicata functia alocare de la tabelSpecii
};

//metode

void Individ::adauga(Individ& adauga) {
    i = adauga.i;
    j = adauga.j;
    varsta = adauga.varsta;
    energie = adauga.energie;
    viu = adauga.viu;
    tip = adauga.tip;
}

void Individ::die() {
    viu = 0;
}

void Individ::getsOld() {
    varsta++;
    energie -= constGettingOld;
    if (energie < 0 || varsta >= MaxAge)
        die();
}

void Individ::update() {
    feed;
    giving_birth;
    //de pus ataca
    getsOld;
}

bool Individ::isAlive() {
    return int(viu);
}

//functii pentru ciclu
void doDelay(int number)
{
    std::cout << "delay " << number << '\n';

    volatile unsigned int iii = 0;
    while (iii < 100000000) {
        iii++;
    }
}

void doDelayLoop()
{
    for (int iii = 0; iii < 10; ++iii) {
        doDelay(iii);
    }
}

bool flagLoop = true;
void raiseFlag(int param)
{
    flagLoop = false;
}


// functii in afara
void feed(int x, int y, Individ**& mat) {
    int aportEnergetic = 1;
    int i = x, j = y;
    while (j++ < 71 && int(mat[i][j].viu))
        aportEnergetic++;
    j = y;
    while (j-- >0 && int(mat[i][j].viu))
        aportEnergetic++;

    mat[i][j].energie = aportEnergetic * (1 - abs(MaxAge / 2 - mat[i][j].varsta) ); // formula pentru energie: aport * ( 1 - (mijlocul_varstei_maxime - varsta actuala) )
}

void giving_birth(int x, int y, Individ**& mat) {
    int FreeSpaces = 0, nrSons = 0;

    int i = x, j = y;
    while (j++ < 71 && int(mat[i][j].viu))
        FreeSpaces++;
    j = y;
    while (j-- > 0 && int(mat[i][j].viu))
        FreeSpaces++;

    nrSons = mat[i][j].energie * (1 - abs(MaxAge / 2 - mat[i][j].varsta));  // formuala pentru numarul de fii: energie * (1 - |enegiaMax/2 - varsta| )
    nrSons = min(FreeSpaces, nrSons);                                           // numarul de fii nu poate fi mai mare decat numarul de spatii libere
    int MaxNumberOfSonsBeforeDeath = min(nrSons * constGivingBirth, floor(mat[i][j].energie / constGivingBirth));//daca da nastere la prea multi risca sa ramana fara energie
    nrSons = min(nrSons, MaxNumberOfSonsBeforeDeath);
    mat[i][j].energie -= nrSons * constGivingBirth;            // pentru fiecare fiu energia scade cu o consntanta
    i = x; j = y;
    while (nrSons-- && j++ < 71 && int(mat[i][j].viu)) {
        Individ aux(i, j, 1);
        mat[i][j] = aux;            // un fiu ia nastere(e apelat prin constructor)
    }

    j = y;
    while (nrSons-- && j-- > 0 && int(mat[i][j].viu)) {
        Individ aux(i, j, 1);
        mat[i][j] = aux;            // un fiu ia nastere(e apelat prin constructor)
    }
}


int main() {
    Individ** mat = new Individ * [71];
    for (int i = 0; i < 71; ++i)            //matricea alocata dinamic
        mat[i] = new Individ[21];

    //signal(SIGINT, raiseFlag);

    ////Keeping track of the current cycle
    //int iteration = 0;

    ////Infinite loop of the program
    //while (flagLoop == true) {
    //    std::cout << "Iteration: " << iteration++ << '\n';
    //    doDelayLoop();
    //}
}

/*
Tema 16. (0.3 pct bonus) (TODO: indentare)
Clasa "Individ", avand:
- metode private:
•	ataca = pentru fiecare individ x invecinat si de alt tip, daca energia proprie este mai mare decat energia lui x, din energia proprie se scade energia lui x iar x este omorat (i se aplica metoda"die");
- metode publice:
•	gettip = returneaza tipul.
Program care genereaza aleator o tabela 20X70 de indivizi, majoritatea pozitiilor fiind insa goale, apoi intr-un ciclu, la fiecare iteratie, parcurge indivizii (intr-o anumita ordine), pe cei vii ii actualizeaza, pe cei morti ii elimina din tabela, apoi afisaza tabla. Ciclarea dureaza pana se apasa o tasta sau pana nu mai ramane nici un individ pe tabla. Propunere: tabla sa fie ea insasi un obiect avand metode pentru initializare aleatoare, afisare, actualizare, test daca mai exista indivizi, alocarea unui nou element pe o pozitie data, eliminarea unui element de pe o pozitie data, etc.

*/
