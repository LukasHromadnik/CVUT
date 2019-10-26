#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <list>
#include <stdint.h>

using namespace std;
#endif /* __PROGTEST__ */

///////////////////////////////////////////////////////////////////////////////////////////////////

class Node;
class Cell;
class Table;
class SearchNode;

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

enum NodeType { Operator, Value, Key };
enum CellType { Undefined, Valid, Cycle, Zero, Invalid };
enum SearchType { Fresh, Open, Closed };

bool isOperator(string key) {
    return (key == "+" || key == "-" || key == "*" || key == "/");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class Node {
public:
    Node(Cell *cell);
    string build(string formula);
    int getValue();
    
    friend ostream& operator<< (ostream& stream, const Node& node);
    
    string m_Operator;
    int m_Value;
    string m_Key;
    Cell *m_Cell;
    NodeType m_Type;
    Node *m_Left;
    Node *m_Right;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class Cell {
public:
    Cell(string identifier, Table *table);
    
    void nodeFoundKey(Node *node, string key);
    void addObserver(Cell *cell);
    void setValueForKey(int value, string key, CellType type);
    void parseFormula(string formula);
    int getValue();
    void setValue();
    bool hasValue() const;
    void notifyObserversError();
    
    friend ostream& operator<< (ostream& stream, const Cell& cell);
    
    string m_Identifier;
    map<string, vector<Node *> > m_Observing;
    vector<Cell *> m_Observers;
    Node *m_Formula;
    Table *m_Table;
    CellType m_Type;
    SearchType m_SearchType;
    int m_Value;
    bool m_HasValue;
    int m_Index;
    int m_LowLink;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class Table {
public:
    void addObserverForKey(Cell *cell, string key);
    void cellHasValue(Cell *cell, int value);
    void parseInput(string text);
    void notifyObservers();
    void checkCycles();
    // void checkValue();
    
    friend ostream& operator<< (ostream& stream, const Table& table);
    
    map<string, Cell *> m_Cells;
    SearchNode *m_SearchNode;
    vector<string> m_Keys;
    vector<Cell *> m_Ready;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class Stack {
public:
    void push(Cell *cell);
    Cell * pop();
    bool contains(Cell *cell);
    
private:
    vector<Cell *> m_Data;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

void Stack::push(Cell *cell) {
    m_Data.push_back(cell);
}

Cell * Stack::pop() {
    Cell *cell = m_Data.back();
    m_Data.pop_back();
    
    return cell;
}

bool Stack::contains(Cell *cell) {
    return find(m_Data.begin(), m_Data.end(), cell) != m_Data.end();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class Component {
public:
    void add(Cell *cell);
    vector<Cell *> m_Data;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

void Component::add(Cell *cell) {
    m_Data.push_back(cell);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class List {
public:
    void push(Component *component);
    vector<Component *> m_Data;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

void List::push(Component *component) {
    m_Data.push_back(component);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void tarjan(Cell *cell, Stack *stack, List *components, int index = 1) {
    cell->m_Index = index;
    cell->m_LowLink = index;
    
    stack->push(cell);
    
    for (auto item : cell->m_Observing) {
        Cell *offspring = cell->m_Table->m_Cells[item.first];
        if (offspring->m_Index == -1) {
            tarjan(offspring, stack, components, index + 1);
            cell->m_LowLink = min(cell->m_LowLink, offspring->m_LowLink);
        } else if (stack->contains(offspring)) {
            cell->m_LowLink = min(cell->m_LowLink, offspring->m_Index);
        }
    }
    
    if (cell->m_Index == cell->m_LowLink) {
        Cell *item = NULL;
        Component *component = new Component();
        
        do {
            item = stack->pop();
            component->add(item);
        } while (item->m_Identifier != cell->m_Identifier);
        
        components->push(component);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Node::Node(Cell *cell) {
    m_Cell = cell;
}

string Node::build(string formula) {
    string value;
    string remaining = "";
    for (int i = 0; i < (int)formula.length(); i++) {
        if (formula[i] == ' ') {
            value = formula.substr(0, i);
            remaining = formula.substr(i + 1, formula.length());
            break;
        } else if (i + 1 == (int)formula.length()) {
            value = formula;
            break;
        }
    }
    
    if (isOperator(value)) {
        m_Type = Operator;
        m_Operator = value;
    } else if (value.find(":") != string::npos) {
        m_Type = Key;
        m_Key = value;
        m_Cell->nodeFoundKey(this, value);
    } else {
        m_Type = Value;
        m_Value = stoi(value);
    }
    
    if (isOperator(value)) {
        m_Left = new Node(m_Cell);
        remaining = m_Left->build(remaining);
        
        m_Right = new Node(m_Cell);
        remaining = m_Right->build(remaining);
    }
    
    return remaining;
}

int Node::getValue() {
    if (m_Type == Operator) {
        int left = m_Left->getValue();
        int right = m_Right->getValue();
        
        string op = m_Operator;
        if (op == "+") {
            return left + right;
        } else if (op == "-") {
            return left - right;
        } else if (op == "/") {
            if (right == 0) {
                m_Cell->m_Type = Zero;
                return 0;
            } else {
                double d_Left = left;
                double d_Right = right;
                double d_Res = d_Left / d_Right;
                if (d_Res < 0) {
                    d_Res--;
                }
//                cout << d_Left / d_Right << endl;
                return (int)d_Res;
            }
        } else if (op == "*") {
            return left * right;
        }
        
        return 0;
    } else {
        return m_Value;
    }
}
ostream& operator<< (ostream& stream, const Node& node) {
    if (node.m_Type == Operator) {
        stream << *(node.m_Left) << " ";
    }
    
    switch (node.m_Type) {
        case Operator: stream << node.m_Operator; break;
        case Key: stream << node.m_Key; break;
        case Value: stream << node.m_Value; break;
    }
    
    if (node.m_Type == Operator) {
        stream << " " << *(node.m_Right);
    }
    
    return stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Cell::Cell(string identifier, Table *table) {
    m_Identifier = identifier;
    m_Table = table;
    m_Type = Undefined;
    m_SearchType = Fresh;
    m_HasValue = false;
    m_Index = -1;
    m_LowLink = -1;
}

void Cell::nodeFoundKey(Node *node, string key) {
    if (m_Observing.find(key) == m_Observing.end()) {
        m_Observing[key] = vector<Node *>();
        m_Table->addObserverForKey(this, key);
    }
    m_Observing[key].push_back(node);
    
}

void Cell::addObserver(Cell *cell) {
    m_Observers.push_back(cell);
}

void Cell::setValueForKey(int value, string key, CellType type) {
    if (type != Valid && m_Type == Undefined) {
        m_Type = Invalid;
        for (auto it : m_Observers) {
            it->setValueForKey(0, m_Identifier, m_Type);
        }
    } else {
        vector<Node *> nodes = m_Observing[key];
        for (auto it : nodes) {
            it->m_Value = value;
            it->m_Type = Value;
        }
        m_Observing.erase(key);
        
        if (m_Observing.size() <= 0) {
            getValue();
            if (m_Type == Undefined) {
                m_Type = Valid;
            }
            for (auto it : m_Observers) {
                it->setValueForKey(m_Value, m_Identifier, m_Type);
            }
        }
    }
}

void Cell::parseFormula(string formula) {
    m_Formula = new Node(this);
    m_Formula->build(formula);
    if (m_Observing.size() == 0) {
        m_Type = Valid;
        m_Table->m_Ready.push_back(this);
    }
}

void Cell::setValue() {
    m_Value = m_Formula->getValue();
    m_HasValue = true;
}

bool Cell::hasValue() const {
    return m_Observing.size() <= 0 && m_Type == Valid;
}

void Cell::notifyObserversError() {
    for (auto it : m_Observers) {
        if (it->m_Type == Valid) {
            it->m_Type = Invalid;
            it->notifyObserversError();
        }
    }
}

int Cell::getValue() {
    if ( ! m_HasValue) {
        setValue();
    }
    
    return m_Value;
}

ostream& operator<< (ostream& stream, const Cell& cell) {
    stream << cell.m_Identifier << " = ";
    switch (cell.m_Type) {
        case Valid: stream << cell.m_Value; break;
        case Invalid: stream << "#invalid"; break;
        case Zero: stream << "#zero"; break;
        case Cycle: stream << "#cycle"; break;
        case Undefined: stream << "#invalid"; break;
    }
    
    return stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Table::addObserverForKey(Cell *cell, string key) {
    if (m_Cells.find(key) == m_Cells.end()) {
        m_Cells[key] = new Cell(key, this);
    }
    m_Cells[key]->addObserver(cell);
}

void Table::cellHasValue(Cell *cell, int value) {
    for (auto it : m_Cells) {
        it.second->setValueForKey(value, cell->m_Identifier, cell->m_Type);
    }
}

void Table::parseInput(string text) {
    bool newCell = false;
    string lastKey;
    int lastIndex = 0;
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] == '\n') {
            if (lastKey.length() != 0) {
                string formula = text.substr(lastIndex, i - lastIndex);
                
                Cell *cell = m_Cells[lastKey];
                if (cell == NULL) {
                    m_Cells[lastKey] = new Cell(lastKey, this);
                    cell = m_Cells[lastKey];
                }
                cell->parseFormula(formula);
            }
            newCell = true;
            lastIndex = i + 1;
        }
        
        if (text[i] == '=' && newCell) {
            newCell = false;
            lastKey = text.substr(lastIndex, i - lastIndex - 1);
            m_Keys.push_back(lastKey);
            lastIndex = i + 2;
        }
    }
}

void Table::notifyObservers() {
    for (auto cell : m_Ready) {
        cell->m_Type = Valid;
        cell->getValue();
        for (auto observer : cell->m_Observers) {
            observer->setValueForKey(cell->m_Value, cell->m_Identifier, cell->m_Type);
        }
    }
}

void Table::checkCycles() {
    Stack *stack = new Stack();
    List *components = new List();
    
    for (auto it : m_Cells) {
        Cell *cell = it.second;
        
        if (cell->m_Index == -1) {
            tarjan(cell, stack, components);
        }
    }
    
    for (auto component : components->m_Data) {
        if (component->m_Data.size() > 1) {
            for (auto cell : component->m_Data) {
                cell->m_Type = Cycle;
            }
        }
    }
}

ostream& operator<< (ostream& stream, const Table& table) {
    for (auto it : table.m_Cells) {
        stream << *(it.second) << endl;
    }
    
    return stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

string loadInput(const char *inFile) {
    string line, text;
    ifstream in(inFile);
    
    while (getline(in, line)) {
        text += line + "\n";
    }
    
    return text;
}

void audit(const char *inFile, const char *outFile) {
    Table *table = new Table();
    
    string text = loadInput(inFile);
    table->parseInput(text);
    table->checkCycles();
    table->notifyObservers();
    
    ofstream out(outFile);
    
    for (auto it : table->m_Keys) {
        out << *table->m_Cells[it] << endl;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PROGTEST__

int main(int argc, char * argv [])
{
   if (argc != 3) {
       cout << "usage: binary inFile outFile" << endl;
       return 1;
   }
    
   audit(argv[1], argv[2]);
     // audit("/Users/Luke/Documents/FIT/BI-GRA/Progtest/01/test/01_in.txt", "/Users/Luke/Documents/FIT/BI-GRA/Progtest/01/test/01_my.txt");
    
    return 0;
}

#endif /* __PROGTEST__ */
