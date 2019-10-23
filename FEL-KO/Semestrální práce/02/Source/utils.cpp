#include <vector>
#include <set>
#include <fstream>
#include <sstream>

#include "Ticket.hpp"

using namespace std;

struct Input {
    vector<Ticket *> tickets;
    string startCity;
    int numberOfDays;
    set<string> cities;
    int timeLimit;
};

Input loadInput(const char * argv[]) {

    Input input = Input();

    int lineNumber = 0;
    string line;

    // Open input file
    ifstream inFile(argv[1]);

    // Iterate over all input lines
    while (getline(inFile, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);
        
        // Count number of words in one input line
        int wordNumber = 0;
            
        // Create new ticket
        Ticket *ticket = new Ticket();

        string word;

        // Iterate over all "token"
        while (ss >> word) {

            if (lineNumber == 0) {
                input.startCity = word;
            } else {
                switch (wordNumber) {
                case 0:
                    ticket->from = word;
                    input.cities.insert(word);
                    break;
                case 1:
                    ticket->to = word;
                    input.cities.insert(word);
                    break;
                case 2:
                    ticket->day = stoi(word);
                    input.numberOfDays = max(input.numberOfDays, stoi(word));
                    break;
                case 3:
                    ticket->price = stoi(word);
                    break;
                }
            }

            // Increase word number
            wordNumber++;
        }

        // Add new ticket to the array
        if (lineNumber > 0) {
            input.tickets.push_back(ticket);
        }

        // Increase line number
        lineNumber++;
    }

    input.numberOfDays++;
    input.timeLimit = stoi(argv[3]);

    return input;
}
