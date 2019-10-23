#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <stdexcept>

using namespace std;

class Object2D {
public:
    virtual double vypoctiPlochu() = 0;
    virtual double vypoctiObvod() = 0;
    virtual ~Object2D() { };
    virtual string print() = 0;
};

class Obdelnik: public Object2D {
protected:
    int sideA;
    int sideB;
public:
    Obdelnik(int sideA, int sideB) : sideA(sideA), sideB(sideB) { }
    virtual double vypoctiPlochu() { return sideA * sideB; }
    virtual double vypoctiObvod() { return 2 * (sideA + sideB); }
    virtual string print() { return "Obdélník, a = " + to_string(sideA) + ", b = " + to_string(sideB); }
};

class Ctverec: public Obdelnik {
public:
    Ctverec(int side) : Obdelnik(side, side) { }
    virtual string print() { return "Čtverec, a = " + to_string(sideA); }
};

class Trojuhelnik: public Object2D {
    int sideA;
    int sideB;
    int sideC;
public:
    Trojuhelnik(int sideA, int sideB, int sideC) : sideA(sideA), sideB(sideB), sideC(sideC) { };
    virtual double vypoctiPlochu() {
        double cosGamma = (pow(sideA, 2) + pow(sideB, 2) - pow(sideC, 2)) / (2 * sideA * sideB);
        double sinGamma = sqrt(1 - pow(cosGamma, 2));
        double vyskaA = sideB * sinGamma;
        return sideA * vyskaA / 2;
    }
    virtual double vypoctiObvod() { return sideA + sideB + sideC; }
    virtual string print() { return "Trojúhelník, a = " + to_string(sideA) + ", b = " + to_string(sideB) + ", c = " + to_string(sideC); }
};

class Kruh: public Object2D {
    int radius;
public:
    Kruh(int radius) : radius(radius) { };
    virtual double vypoctiObvod() { return 2 * M_PI * radius; }
    virtual double vypoctiPlochu() { return M_PI * radius * radius; }
    virtual string print() { return "Kruh, r = " + to_string(radius); }
};

enum ObjectType { square, rectangle, triangle, circle };

vector<int> loadLineNumbers() {
    string line;
    cout << "> ";
    getline(cin, line);
    stringstream ss(line);
    string word;
    vector<int> tokens;
    while (ss >> word) {
        tokens.push_back(stoi(word));
    }

    return tokens;
}

struct unknown_object: public exception { };
struct not_enough_arguments: public exception { };

int main() {
    cout << "=== APLIKACE PRO VÝPOČET OBSAHU A OBVODU OBRAZCŮ ===" << endl;
    while (true) {
        cout << "Se kterým obrazcem chcete pracovat?" << endl;
        cout << " 1 čtverec" << endl;
        cout << " 2 obdélník" << endl;
        cout << " 3 trojúhelník" << endl;
        cout << " 4 kruh" << endl;

        ObjectType ot;
        vector<int> tokens = loadLineNumbers();
        switch (tokens[0]) {
            case 1:
                ot = ObjectType::square;
                cout << endl << "Zadejte délku strany čtverce:" << endl;
                break;
            case 2:
                ot = ObjectType::rectangle;
                cout << endl << "Zadejte délku stran obdélníku:" << endl;
                break;
            case 3:
                ot = ObjectType::triangle;
                cout << endl << "Zadejte délku stran trojúhelníku:" << endl;
                break;
            case 4:
                ot = ObjectType::circle;
                cout << endl << "Zadejte poloměr kruhu:" << endl;
                break;
            default:
                throw unknown_object();
        }

        tokens = loadLineNumbers();

        unique_ptr<Object2D> object;

        switch (ot) {
            case ObjectType::square:
                if (tokens.size() < 1)
                    throw not_enough_arguments();
                object = make_unique<Ctverec>(tokens[0]);
                break;
            case ObjectType::rectangle:
                if (tokens.size() < 2)
                    throw not_enough_arguments();
                object = make_unique<Obdelnik>(tokens[0], tokens[1]);
                break;
            case ObjectType::triangle:
                if (tokens.size() < 3)
                    throw not_enough_arguments();
                object = make_unique<Trojuhelnik>(tokens[0], tokens[1], tokens[2]);
                break;
            case ObjectType::circle:
                if (tokens.size() < 1)
                    throw not_enough_arguments();
                object = make_unique<Kruh>(tokens[0]);
                break;
        }

        cout << object->print() << endl;
        cout << "Obvod: " << object->vypoctiObvod() << endl;
        cout << "Obsah: " << object->vypoctiPlochu() << endl;
        cout << endl;
    }

    return 0;
}
