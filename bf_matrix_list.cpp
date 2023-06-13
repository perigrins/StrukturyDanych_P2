#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <conio.h>
#include <cmath>
#include <ctime>

using namespace std;

const static double MAXINT = 2147483647;

class GeneratorG
{

private:
    int maxWaga, minEdges, edges, verticles;
    double maxEdges;
    int** wynik;

public:
    GeneratorG();
    GeneratorG(int);
    ~GeneratorG();
};


class Opcje
{
private:
    struct ElementListy
    {
        ElementListy* next;
        int n;
        int waga;
    };

    int v;          // verticles
    int e;          // edges

    int** Graph;
    int** Graph2;
    int** resultsList;

    int edge2;

    ElementListy **ListaN;
    ElementListy** Lista, * pom;

public:
    int poczatek, koniec;        // początkowy i końcowy wierzchołek

    Opcje();

    void createGraph();     // graf skierowany z pliku
    void createGraph2();    // graf nieskierowany
    //void createRandom();


    void makeNeighbourList();
    void makeNeighbourList2();
    void printNeighbourList();
    void printNeighbourList2();

    void BFList(int);

    bool readFile();

    ~Opcje();
};

// wczytujemy graf z pliku i wybieramy, co chcemy zrobic -> MENU
void readGraph()
{
    bool sukces;
    Opcje graf;
    do
    {
        sukces = graf.readFile();
    } while (sukces != 1);

    graf.createGraph();
    graf.createGraph2();
    graf.makeNeighbourList();
    graf.makeNeighbourList2();

    int wybor = 0;
    do {
        cout << endl;
        cout << "Wybierz opcje: " << endl;
        cout << "1. algorytm bellmana - forda dla postaci listy" << endl;
        cout << "2. wyswietl graf skierowany" << endl;
        cout << "3. wyswietl graf nieskikerowany" << endl;
        cout << "4. wyjdz" << endl << endl;
        cin >> wybor;

        switch (wybor)
        {
        case 1:
            graf.BFList(graf.poczatek);
            break;
        case 2:
            graf.printNeighbourList();
            break;
        case 3:
            graf.printNeighbourList2();
            break;
        default:
            cout << "niepoprawna opcja" << endl;
            break;
        }
    } while (wybor != 4);
}

// tworzenie grafu losowo -> MENU
void randomGraph() {
    GeneratorG generator(5);
    Opcje graf;
    graf.createGraph();
    graf.createGraph2();
    graf.makeNeighbourList();
    graf.makeNeighbourList2();

    int wybor = 0;
    do {
        cout << endl;
        cout << "Wybierz opcje: " << endl;
        cout << "1. algorytm bellmana - forda dla postaci listy" << endl;
        cout << "2. wyswietl graf skierowany" << endl;
        cout << "3. wyswietl graf nieskikerowany" << endl;
        cout << "4. wyjdz" << endl << endl;
        cin >> wybor;

        switch (wybor)
        {
        case 1:
            graf.BFList(graf.poczatek);
            break;
        case 2:
            graf.printNeighbourList();
            break;
        case 3:
            graf.printNeighbourList2();
            break;
        default:
            cout << "niepoprawna opcja" << endl;
            break;
        }
    } while (wybor != 4);
}

// tworzenie grafu skierowanego (z pliku)
void Opcje::createGraph()
{
    ifstream wejscie;
    wejscie.open("graf_0.txt");

    if (wejscie.is_open())
    {
        wejscie >> e;
        wejscie >> v;
        wejscie >> poczatek;
        wejscie >> koniec;
        Graph = new int* [e];

        if (wejscie.fail())
        {
            cout << "blad" << endl;
        }
        else
        {
            int source, destination, localWeight;
            for (int i = 0; i < e; i++)
            {
                wejscie >> source;
                wejscie >> destination;
                wejscie >> localWeight;

                if (wejscie.fail())
                {
                    cout << "blad" << endl;
                    break;
                }
                else
                {
                    Graph[i] = new int[3];
                    Graph[i][0] = source;
                    Graph[i][1] = destination;
                    Graph[i][2] = localWeight;
                }
            }
        }
        wejscie.close();
    }
    else
    {
        cout << "blad" << endl;
    }
}

// tablica bez powtorzonych wierzcholkow (potrzebna do grafu nieskierowanego)
void Opcje::createGraph2() {
    bool* duplicate;
    duplicate = new bool[e];
    for (int i = 0; i < e; i++)
        duplicate[i] = false;
    Graph2 = new int* [e];
    for (int i = 0; i < e; i++)
        Graph2[i] = NULL;

    for (int i = 0; i < e; i++) {
        for (int j = 0; j < i; j++) {
            if ((Graph[i][0] == Graph[j][0] && Graph[i][1] == Graph[j][1]) || (Graph[i][1] == Graph[j][0] && Graph[i][0] == Graph[j][1])) {
                duplicate[i] = false;
                break;
            }
            duplicate[i] = true;
        }
    }

    duplicate[0] = true;
    edge2 = 0;
    for (int i = 0; i < e; i++) {
        if (duplicate[i] == true) {
            Graph2[edge2] = new int[3];
            Graph2[edge2][0] = Graph[i][0];
            Graph2[edge2][1] = Graph[i][1];
            Graph2[edge2][2] = Graph[i][2];
            edge2++;
        }
    }

    delete[] duplicate;
}


// lista sąsiedztwa 
void Opcje::makeNeighbourList()
{
    Lista = new ElementListy * [v];
    for (int i = 0; i < v; i++)
    {
        Lista[i] = NULL;
    }
    
    for (int i = 0; i < e; i++)
    {
        pom = new ElementListy;    // Tworzymy nowy element
        pom->n = Graph[i][1];          // Numerujemy go
        pom->waga = Graph[i][2];
        pom->next = Lista[Graph[i][0]];    // Dodajemy go na poczatek listy
        Lista[Graph[i][0]] = pom;
    }
}

// lista sąsiedztwa - wyświetlenie
void Opcje::printNeighbourList() 
{
    cout << "[nr_wierzcholka] = " << setw(3) << "nr_wierzcholka(waga_krawdzi)" << endl;
    for (int i = 0; i < v; i++)
    {
        cout << "[" << i << "] = ";
        pom = Lista[i];
        while (pom)
        {
            cout << setw(3) << pom->n << "(" << pom->waga << ")";
            pom = pom->next;
        }
        cout << endl;
    }
}

// lisa sąsiedztwa (graf nieskierowany)
void Opcje::makeNeighbourList2()
{
    ListaN = new ElementListy * [v];
    for (int i = 0; i < v; i++)
    {
        ListaN[i] = NULL;
    }
    for (int i = 0; i < edge2; i++)
    {
        pom = new ElementListy;
        pom->n = Graph2[i][1];
        pom->waga = Graph2[i][2];
        pom->next = ListaN[Graph2[i][0]];
        ListaN[Graph2[i][0]] = pom;

        pom = new ElementListy;
        pom->n = Graph2[i][0];
        pom->waga = Graph2[i][2];
        pom->next = ListaN[Graph2[i][1]];
        ListaN[Graph2[i][1]] = pom;
    }
}

// lista sąsiedztwa (graf nieskierowany) - wyświetlenie
void Opcje::printNeighbourList2()
{
    cout << "[nr_wierzcholka] = " << setw(3) << "nr_wierzcholka(waga_krawdzi)" << endl;
    for (int i = 0; i < v; i++) {
        cout << "[" << i << "] =";
        pom = ListaN[i];
        while (pom) {
            cout << setw(3) << pom->n << "(" << pom->waga << ")";
            pom = pom->next;
        }
        cout << endl;
    }
}

// algorytm bellmana forda - lista sąsiedztwa
void Opcje::BFList(int start)
{
    int* koszt;
    int* sciezka;
    int* Stos;
    int wskaznikStosu;
    int i, j, u;
    bool* czy_odwiedzone;

    koszt = new int[v];
    sciezka = new int[v];
    czy_odwiedzone = new bool[v];
    Stos = new int[v];
    wskaznikStosu = 0;

    for (int i = 0; i < v; i++)
    {
        koszt[i] = MAXINT;
        sciezka[i] = -1;
        czy_odwiedzone[i] = 0;
    }

    cout << endl;
    koszt[start] = 0;

    for (int i = 0; i < v; i++)
    {
        for (j = 0; czy_odwiedzone[j]; j++);
        for (u = j++; j < v; j++)
        {
            if (!czy_odwiedzone[j] && (koszt[j] < koszt[u]))
            {
                u = j;
            }
        }

        czy_odwiedzone[u] = 1;

        for (pom = Lista[u]; pom; pom = pom->next)
        {
            if (!czy_odwiedzone[pom->n] && (koszt[pom->n] > koszt[u] + pom->waga))
            {
                koszt[pom->n] = koszt[u] + pom->waga;
                sciezka[pom->n] = u;
            }
        }
    }

    cout << "Wierzchołek startowy: " << poczatek << endl;
    cout << "Wierzcholek | Waga | Sciezka" << endl;

    for (i = 0; i < v; i++)
    {
        cout << i << ":     ";
        for (j = i; j > -1; j = sciezka[j])
        {
            Stos[wskaznikStosu++] = j;
        }
        cout << "   | " << koszt[i] << " |   ";
        while (wskaznikStosu)
        {
            cout << Stos[--wskaznikStosu] << " ";
        }
        cout << endl;
    }

    delete[] koszt;
    delete[] sciezka;
    delete[] Stos;
    delete[] czy_odwiedzone;
}

// wczytanie wartości zmiennych z pliku do tablicy i utworzenie grafu
bool Opcje::readFile()
{
    bool sukces = 0;
    
    ifstream wejscie;
    wejscie.open("graf_10.txt");

    if (wejscie.is_open())
    {
        wejscie >> e;               // liczba krawedzi
        wejscie >> v;               // liczba wierzcholkow
        wejscie >> poczatek;        // wierzcholek poczatkowy
        wejscie >> koniec;          // wierzcholek koncowy 

        Graph = new int* [e];

        if (wejscie.fail())
        {
            cout << "blad" << endl;
        }
        else
        {
            int source, destination, weight;
            for (int i = 0; i < e; i++)
            {
                wejscie >> source;
                wejscie >> destination;
                wejscie >> weight;

                if (wejscie.fail())
                {
                    cout << "blad" << endl;
                    break;
                }
                else
                {
                    // Wczytanie wartości do tablicy wskaźników
                    Graph[i] = new int[3];
                    Graph[i][0] = source;
                    Graph[i][1] = destination;
                    Graph[i][2] = weight;
                }
            }
        }
        wejscie.close();
        sukces = 1;
    }
    else
    {
        cout << "blad" << endl;
        sukces = 0;
    }

    return sukces;
}

// konstruktor
Opcje::Opcje()
{

}

// destruktor
Opcje::~Opcje()
{

    for (int i = 0; i < e; i++)
    {
        delete[] Graph[i];
        delete[] Graph2[i];
    }
    Graph = NULL;
    Graph2 = NULL;
}

// konstruktor
GeneratorG::GeneratorG()
{
}
// generator grafu
GeneratorG::GeneratorG(int x)
{
    int v2 = 0;
    float gestosc = 0;

    do
    {
        cout << "podaj liczbe wierzcholkow: ";
        cin >> v2;
    } while (v2 < 2);

    this->verticles = v2;
    maxWaga = 9;
    minEdges = verticles - 1;	//minimalna ilosc krawedzi dla grafu spojnego
    maxEdges = ((verticles - 1) * verticles) / 2;

    int mingestosc = ceil((minEdges / maxEdges) * 100);

    do
    {
        cout << "podaj gestosc (minimum " << mingestosc << "%) " << endl;
        cin >> gestosc;
    } while (gestosc < mingestosc || gestosc >= 100);

    edges = ceil(maxEdges * (gestosc / 100));	    //pożądana liczba krawedzi
    int weight;
    wynik = new int* [2 * edges];

    //drzewo
    vector <int> tab;                               //vector pamietajacy czy wierzcholek byl juz odwiedzony
    for (int i = 0; i < verticles; i++)	tab.push_back(i);

    srand(time(NULL));
    int next, previous;
    previous = rand() % verticles;
    tab[previous] = -1;
    for (int i = 1; i < minEdges + 1; i++)
    {
        while (true)
        {
            next = rand() % verticles;
            if (tab[next] >= 0)
                break;
        }

        wynik[i - 1] = new int[3];
        wynik[i - 1][0] = previous;
        wynik[i - 1][1] = next;
        weight = (rand() % maxWaga) + 1;
        wynik[i - 1][2] = weight;
        tab[next] = -1;
        previous = next;
    }

    bool czyJest = false;
    for (int i = minEdges; i < edges; i++)
    {		// dodanie losowych krawedzi do drzewa
        while (true)
        {
            czyJest = false;
            previous = rand() % verticles;
            next = rand() % verticles;
            for (int j = 0; j < i; j++)
            {
                if ((wynik[j][0] == previous && wynik[j][1] == next) || (wynik[j][0] == next && wynik[j][1] == previous) || (next == previous))
                {
                    czyJest = true;
                    break;
                }
            }
            if (czyJest == false)
            {
                wynik[i] = new int[3];
                wynik[i][0] = previous;
                wynik[i][1] = next;
                weight = rand() % maxWaga + 1;
                wynik[i][2] = weight;
                break;
            }
        }

    }

    for (int i = edges; i < edges * 2; i++)
    {

        while (true)
        {
            czyJest = false;
            previous = rand() % verticles;
            next = rand() % verticles;
            for (int j = 0; j < i; j++)
            {

                if ((wynik[j][0] == previous && wynik[j][1] == next) || (next == previous))
                {
                    czyJest = true;
                    break;
                }

            }
            if (czyJest == false)
            {
                wynik[i] = new int[3];
                wynik[i][0] = previous;
                wynik[i][1] = next;
                weight = rand() % maxWaga + 1;
                wynik[i][2] = weight;
                break;
            }
        }

    }

    ofstream wyjscie;
    wyjscie.open("graf_0.txt");
    wyjscie << 2 * edges << " " << verticles << " " << wynik[0][0] << " " << wynik[verticles - 1][0] << endl;
    for (int i = 0; i < 2 * edges; i++)
    {
        wyjscie << wynik[i][0] << " " << wynik[i][1] << " " << wynik[i][2] << endl;
    }
    wyjscie.close();
 
}

// destruktor
GeneratorG::~GeneratorG()
{
    for (int i = 0; i < 2 * edges; i++)
    {
        delete wynik[i];
    }
    wynik = NULL;
}

int main()
{
    // MENU

    /*int wybor = 0;
    do
    {
        cout << "wygeneruj graf: " << endl;
        cout << "1. z pliku" << endl;
        cout << "2. losowo" << endl;
        cout << "3. wyjdz" << endl << endl;
        cin >> wybor;

        switch (wybor)
        {
        case 1:
            // Wczytaniu grafu z pliku
            readGraph();
            break;
        case 2:
            //wczytanie grafu losowo
            randomGraph();
        case 3:
            // Wyjście z programu
            break;
        default:
            cout << "bledny wybor" << endl;
            break;
        }
    } while (wybor != 3);*/

    // *****************
    // porownanie czasow
   

    clock_t start = clock();
    GeneratorG generator(5);
    Opcje graf;
    graf.createGraph();
    graf.createGraph2();
    graf.makeNeighbourList();
    graf.makeNeighbourList2();
    graf.BFList(graf.poczatek);
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "\n\nczas dla n wierzcholkow grafu: " << elapsed << endl;

    //system("PAUSE");
    return 0;
}