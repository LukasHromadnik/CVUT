#include <iostream>
#include <vector>
#include <ctime>
#include <unistd.h>

using namespace std;

string toLower(string s) {
    for(size_t i = 0; i <= s.size(); i++) { 
        if (s[i] >= 97 && s[i] <= 122) {
            s[i] -= 32;
        }
    }
    return s;
}

struct LibraryItem {
    using publisher_t = string;
    using genre_t = string;
    using created_t = time_t;

    string name;
    publisher_t publisher;
    int year;
    genre_t genre;
    created_t created;

    LibraryItem(string name, publisher_t publisher, int year, genre_t genre) :
        name(name),
        publisher(publisher),
        year(year),
        genre(genre),
        created(0)
        { }

    virtual ~LibraryItem() { };

    virtual void print(ostream & os) const = 0;
    virtual bool match(char letter) const = 0;
};

ostream & operator << (ostream & os, const LibraryItem & item) {
    item.print(os);
    return os;     
}

struct Book: public LibraryItem {
    using author_t = string;
    
    author_t author;

    Book(string name, author_t author, publisher_t publisher, int year, genre_t genre) :
        LibraryItem(name, publisher, year, genre),
        author(author)
        { }
    
    virtual void print(ostream & os) const {
        string sep = " | ";
        
        os << "[BOOK] ";
        os << name << sep;
        os << author << sep;
        os << publisher << sep;
        os << year << sep;
        os << genre << sep;
        os << created;
    }

    virtual bool match(char letter) const {
        return toLower(name)[0] == letter || toLower(author)[0] == letter;
    }
};

struct Magazine: public LibraryItem {
    using catalog_t = int;
    
    catalog_t catalog;

    Magazine(string name, publisher_t publisher, catalog_t catalog, int year, genre_t genre) :
        LibraryItem(name, publisher, year, genre),
        catalog(catalog)
        { }
    
    virtual void print(ostream & os) const {
        string sep = " | ";
        
        os << "[MAGAZINE] ";
        os << name << sep;
        os << publisher << sep;
        os << catalog << sep;
        os << year << sep;
        os << genre << sep;
        os << created;
    }

    virtual bool match(char letter) const {
        return toLower(name)[0] == letter;
    }
};

using library_t = vector<LibraryItem *>;

void addItem(LibraryItem * item, library_t & library) {
    item->created = time(0);
    library.push_back(item);
}

ostream & operator << (ostream & os, const library_t & library) {
    if (library.empty()) {
        os << "Knihovna je prázdná";
    } else {
        for (auto item : library) {
            os << *item << endl;
        }
    }   
    return os;
}

void inputLine(string & input) {
    cin.clear();
    cin.sync();
    getline(cin, input);
}

void addBook(library_t & library) {
    string input;
    
    string name;
    Book::author_t author;
    Book::publisher_t publisher;
    int year;
    LibraryItem::genre_t genre;

    cin.ignore();

    cout << "Zadejte název knihy" << endl;
    cout << "> ";
    inputLine(name);

    cout << "Zadejte autora" << endl;
    cout << "> ";
    inputLine(author);

    cout << "Zadejte vydavatele" << endl;
    cout << "> ";
    inputLine(publisher);

    cout << "Zadejte rok" << endl;
    cout << "> ";
    cin >> input;
    try {
        year = stoi(input);
    } catch (invalid_argument) {
        cout << "[!] Vstup by měl být číslo" << endl;
        return;
    }

    cin.ignore();
    
    cout << "Zadejte žánr" << endl;
    cout << "> ";
    inputLine(genre);

    auto book = new Book(name, author, publisher, year, genre);
    addItem(book, library);

    cout << endl << "[!] Kniha přidána" << endl;
}

void addMagazine(library_t & library) {
    string input;

    string name;
    Book::publisher_t publisher;
    int number;
    int year;
    LibraryItem::genre_t genre;

    cin.ignore();

    cout << "Zadejte název magazínu" << endl;
    cout << "> ";
    inputLine(name);

    cout << "Zadejte vydavatele" << endl;
    cout << "> ";
    inputLine(publisher);

    cout << "Zadejte číslo časopisu" << endl;
    cout << "> ";
    cin >> input;
    try {
        number = stoi(input);
    } catch (invalid_argument) {
        cout << "[!] Vstup by měl být číslo" << endl;
        return;
    }
    
    cout << "Zadejte rok" << endl;
    cout << "> ";
    cin >> input;
    try {
        year = stoi(input);
    } catch (invalid_argument) {
        cout << "[!] Vstup by měl být číslo" << endl;
        return;
    }

    cin.ignore();
    
    cout << "Zadejte žánr" << endl;
    cout << "> ";
    inputLine(genre);

    auto magazine = new Magazine(name, publisher, number, year, genre);
    addItem(magazine, library);

    cout << endl << "[!] Magazín přidán" << endl;
}

void selectItemToErase(library_t & library) {
    if (library.empty()) {
        cout << "[!] Knihovna je prázdná" << endl;
        return;
    }
    
    cout << "[?] Zadejte ID řádku, který chcete smazat" << endl << endl;

    for (size_t i = 0; i < library.size(); i++) {
        cout << "[" << i << "] " << *library.at(i) << endl;
    }

    string input;

    cout << "> ";
    cin >> input;
    int index;
    try {
        index = stoi(input);
    } catch (invalid_argument) {
        cout << "[!] Vstup by měl být číslo" << endl;
        return;
    }
    if (index < 0 || index >= library.size()) {
        cout << "[!] Nevalidní index" << endl;
        return;
    }
    
    delete library.at(index);
    library.erase(library.begin() + index);

    cout << "[!] Objekt na řádku " << index << " smazán" << endl;
}

void filterItems(library_t & library) {
    cout << "Zadejte první písmeno názvu nebo autora" << endl;
    cout << "> ";
    string letter;
    cin >> letter;
    letter = toLower(letter);
    
    for (auto item : library) {
        if (item->match(letter[0])) {
            cout << *item << endl;
        }
    }
}

int main() {
    auto library = library_t();
    auto shouldEnd = false;

    // Test data
    auto book = new Book("Bible", "Mojžíš", "Albatros", 100, "Náročná literatura");
    addItem(book, library);
    auto magazine = new Magazine("Top Gear", "BBC", 1826343, 2019, "Motorismus");
    addItem(magazine, library);

    cout << "Knihovna" << endl;
    cout << "========" << endl;

    while (shouldEnd == false) {
        string input;
        cout << "Vyberte akci" << endl;
        cout << " 1. Přidání knihy" << endl;
        cout << " 2. Přidání magazínu" << endl;
        cout << " 3. Smazání záznamu" << endl;
        cout << " 4. Vypsání záznamů" << endl;
        cout << " 5. Filtrování záznamů" << endl;
        cout << " 6. Ukončení programu" << endl;
        cout << "> ";
        cin >> input;
        int convertedInput;
        try {
            convertedInput = stoi(input);
        } catch (std::invalid_argument) {
            convertedInput = 0;
        }

        cout << endl;
        
        switch (convertedInput) {
        case 1:
            addBook(library);
            break;
        case 2:
            addMagazine(library);
            break;
        case 3:
            selectItemToErase(library);
            break;
        case 4:
            cout << endl << library << endl;
            break;
        case 5:
            filterItems(library);
            break;
        case 6:
            shouldEnd = true;
            break;
        default:
            cout << endl << "[!] Neplatná akce" << endl;
            break;
        }

        if (shouldEnd == false) {
            cout << endl;
            usleep(1000000);
        }
    }

    for (auto item : library)
        delete item;
    
    return 0;
}
