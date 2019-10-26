#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

typedef struct Data {
    string name;
    string addr;
    int income;
    int expense;
} DATA;

class CTaxCiziten {
    
};


class CTaxRegister {
public:
    CTaxRegister (void);
    ~CTaxRegister (void);
    bool Birth (const string & name, const string & addr);
    bool Death (const string & name, const string & addr);
    bool Income (const string & name, const string & addr, int amount);
    bool Expense (const string & name, const string & addr, int amount);
    bool Audit (const string & name, const string & addr, int & sumIncome, int & sumExpense) const;
    void print (void);
    void Sort(void);
    vector<DATA> items;
    vector<CTaxCiziten> citizens;

private:
        int count;
    int size;
};

CTaxRegister::CTaxRegister(void) {
    this->size = 1000;
    this->count = 0;
    this->items.resize(this->size);
}

CTaxRegister::~CTaxRegister(void) {
    this->items.clear();
}

bool CTaxRegister::Income(const string & name, const string & addr, int amount) {
    int id = -1;
    for (int i = 0; i < this->count; i++) {
        if (this->items[i].name == name && this->items[i].addr == addr) {
            if (id > -1)
                return false;
            else
                id = i;
        }
    }
    
    if (id > -1) {
        this->items[id].income += amount;
        return true;
    }
    
    return false;
}

bool CTaxRegister::Expense(const string & name, const string & addr, int amount) {
    int id = -1;
    for (int i = 0; i < this->count; i++) {
        if (this->items[i].name == name && this->items[i].addr == addr) {
            if (id > -1)
                return false;
            else
                id = i;
        }
    }
    
    if (id > -1) {
        this->items[id].expense += amount;
        return true;
    }

    
    return false;
}

bool CTaxRegister::Birth(const string & name, const string & addr) {
    for (int i = 0; i < this->count; i++) {
        if (this->items[i].name == name && this->items[i].addr == addr) {
            return false;
        }
    }

    this->items[count].name = name;
    this->items[count].addr = addr;
    this->count++;
    
    if (this->count == this->size) {
        this->size *= 2;
        this->items.resize(this->size);
    }
    
    return true;
}

bool CTaxRegister::Death(const string & name, const string & addr) {
    int id = -1;
    for (int i = 0; i < this->count; i++) {
        if (this->items[i].name == name && this->items[i].addr == addr) {
            if (id > -1)
                return false;
            else
                id = i;
        }
    }
    
    if (id > -1) {
        this->items.erase(this->items.begin() + id);
        this->count--;
        
        return true;
    }
    
    return false;
}

bool CTaxRegister::Audit (const string & name, const string & addr, int & sumIncome, int & sumExpense) const {
    int id = -1;
    for (int i = 0; i < this->count; i++) {
        if (this->items[i].name == name && this->items[i].addr == addr) {
            if (id > -1)
                return false;
            else
                id = i;
        }
    }
    
    if (id > -1) {
        sumIncome = this->items[id].income;
        sumExpense = this->items[id].expense;
        return true;
    }
    
    return false;
}

void CTaxRegister::print() {
    for (int i = 0; i < this->count; i++) {
        cout << this->items[i].name << ", " << this->items[i].addr << ", " << this->items[i].income << ", "
        << this->items[i].expense << endl;
    }
}

bool my_cmp(DATA a, DATA b) {
    return strcmp(a.name.c_str(), b.name.c_str());
}

void CTaxRegister::Sort() {
    sort(this->items.begin(), this->items.end(), my_cmp);
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] ) {
    CTaxRegister a;
    a.Birth("Alena", "Doma");
    a.Birth("Lukáš", "Doma");
    a.Birth("Petr", "Doma");
    //    a.Sort();
    auto pos = lower_bound(a.items.name.begin(), a.items.name.end(), "Filip");
    a.print();
//    bool   status;
//    int    sumIncome, sumExpense;
//    
//    CTaxRegister b1;
//    status = b1 . Birth ( "John Smith", "Oak Road 23" );
//    // status = true
//    cout << ((status) ? "true" : "false") << endl;
//    status = b1 . Birth ( "Jane Hacker", "Main Street 17" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Birth ( "Peter Hacker", "Main Street 17" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Birth ( "John Smith", "Main Street 17" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Income ( "Peter Hacker", "Main Street 17", 2000 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Income ( "Jane Hacker", "Main Street 17", 3000 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Income ( "Peter Hacker", "Main Street 17", 4000 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Expense ( "Jane Hacker", "Main Street 17", 2000 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Expense ( "John Smith", "Main Street 17", 500 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Expense ( "Jane Hacker", "Main Street 17", 100 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b1 . Audit ( "John Smith", "Oak Road 23", sumIncome, sumExpense );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true, sumIncome=0, sumExpense=0
//    status = b1 . Audit ( "Jane Hacker", "Main Street 17", sumIncome, sumExpense );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true, sumIncome=3000, sumExpense=2100
//    status = b1 . Audit ( "Peter Hacker", "Main Street 17", sumIncome, sumExpense );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true, sumIncome=6000, sumExpense=0
//    status = b1 . Audit ( "John Smith", "Main Street 17", sumIncome, sumExpense );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true, sumIncome=0, sumExpense=500
//    status = b1 . Death ( "John Smith", "Main Street 17" );
//    // status = true
//    cout << "------------------------------" << endl;
//    CTaxRegister b2;
//    status = b2 . Birth ( "John Smith", "Oak Road 23" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b2 . Birth ( "Jane Hacker", "Main Street 17" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b2 . Income ( "Jane Hacker", "Oak Road 10", 4000 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = false
//    status = b2 . Expense ( "John Smith", "Main Street 18", 500 );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = false
//    status = b2 . Audit ( "John Nowak", "Second Street 23", sumIncome, sumExpense );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = false
//    status = b2 . Death ( "Peter Nowak", "5-th Avenue" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = false
//    status = b2 . Birth ( "Jane Hacker", "Main Street 17" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = false
//    status = b2 . Birth ( "Joe Hacker", "Elm Street 23" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b2 . Birth ( "Joe Hacker", "Elm Street 23" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = false
//    status = b2 . Death ( "Jane Hacker", "Main Street 17" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true
//    status = b2 . Audit ( "Jane Hacker", "Main Street 17", sumIncome, sumExpense );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = false
//    status = b2 . Birth ( "Jane Hacker", "Main Street 17" );
//    cout << ((status) ? "true" : "false") << endl;
//    // status = true

    return 0;
}
#endif /* __PROGTEST__ */
