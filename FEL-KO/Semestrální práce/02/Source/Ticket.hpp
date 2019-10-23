using namespace std;

struct Ticket {
    string from;
    string to;
    int day;
    int price;
    
    string name() const;
};

ostream & operator << (ostream & os, const Ticket & ticket) {
    os << ticket.name();
    return os;
}

string Ticket::name() const {
    return from + " " + to + " " + to_string(day) + " " + to_string(price);
}
