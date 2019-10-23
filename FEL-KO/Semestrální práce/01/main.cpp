#include <iostream>
#include <sstream>
#include <vector>
#include <gurobi_c++.h>
#include <set>
#include <map>
#include <climits>

using namespace std;

struct Ticket {
    string from;
    string to;
    int day;
    int price;
};

int main(int argc, const char * argv[])
{
    string startCity;
    int numberOfDays = 0;
    map<string, map<string, map<int, int> > > tickets;
    vector<Ticket *> data;
    set<string> cities;

    bool showOutput = (argv[2] == NULL);

    /// LOAD INPUT
    string line, word;
    int lineNumber = 0;

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

        // Iterate over all "token"
        while (ss >> word) {

            if (lineNumber == 0) {
                startCity = word;
            } else {
                switch (wordNumber) {
                case 0:
                    ticket->from = word;
                    cities.insert(word);
                    break;
                case 1:
                    ticket->to = word;
                    cities.insert(word);
                    break;
                case 2:
                    ticket->day = stoi(word);
                    numberOfDays = max(numberOfDays, stoi(word));
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
            data.push_back(ticket);
        }

        // Increase line number
        lineNumber++;
    }

    numberOfDays++;

    int numberOfCities = cities.size();
    vector<string> citiesArray = vector<string>(cities.begin(), cities.end());
    for (int day = 0; day < numberOfDays; day++) {
        for (int i = 0; i < numberOfCities; i++) {
            for (int j = 0; j < numberOfCities; j++) {
                string from = citiesArray[i];
                string to = citiesArray[j];
                tickets[from][to][day] = INT_MAX;
            }
        }
    }
    
    /// GUROBI
    try {
        GRBEnv environment;
        environment.set(GRB_IntParam_OutputFlag, 0);
        GRBModel model(environment);
        
        map<string, map<string, map<int, GRBVar> > > vars;

        // Set price from the input
        for (Ticket *ticket : data) {
            string from = ticket->from;
            string to = ticket->to;
            int day = ticket->day;
            int price = ticket->price;
            tickets[from][to][day] = price;
            vars[from][to][day] = model.addVar(0, GRB_INFINITY, price, GRB_BINARY, to_string(day) + ":" + from + ">" + to);
        }
        
        // There will be one flight each day
        for (int day = 0; day < numberOfDays; day++) {
            GRBLinExpr lin;
            for (int i = 0; i < numberOfCities; i++) {
                for (int j = 0; j < numberOfCities; j++) {
                    string from = citiesArray[i];
                    string to = citiesArray[j];
                    int price = tickets[from][to][day];
                    if (price == INT_MAX) continue;
                    lin += vars[from][to][day];
                }
            }
            model.addConstr(lin <= 1);
            model.addConstr(lin >= 1);
        }

        // Leave once
        for (int i = 0; i < numberOfCities; i++) {
            GRBLinExpr lin;
            for (int day = 0; day < numberOfDays; day++) {
                for (int j = 0; j < numberOfCities; j++) {
                    string from = citiesArray[i];
                    string to = citiesArray[j];
                    int price = tickets[from][to][day];
                    if (price == INT_MAX) continue;
                    lin += vars[from][to][day];
                }
            }
            model.addConstr(lin <= 1);
            model.addConstr(lin >= 1);
        }

        // Enter once
        for (int j = 0; j < numberOfCities; j++) {
            GRBLinExpr lin;
            for (int i = 0; i < numberOfCities; i++) {
                for (int day = 0; day < numberOfDays; day++) {
                    string from = citiesArray[i];
                    string to = citiesArray[j];
                    int price = tickets[from][to][day];
                    if (price == INT_MAX) continue;
                    lin += vars[from][to][day];
                }
            }
            model.addConstr(lin <= 1);
            model.addConstr(lin >= 1);
        }

        // Leave from the start city
        GRBLinExpr lin;
        for (int i = 0; i < numberOfCities; i++) {
            string from = startCity;
            string to = citiesArray[i];
            int price = tickets[from][to][0];
            if (price == INT_MAX) continue;
            lin += vars[from][to][0];
        }
        model.addConstr(lin <= 1);
        model.addConstr(lin >= 1);

        // Number flights to the city have to equal to the number of flights from the city
        for (int day = 1; day < numberOfDays - 1; day++) {
            for (int i = 0; i < numberOfCities; i++) {
                GRBLinExpr linIn;
                GRBLinExpr linOut;
                for (int j = 0; j < numberOfCities; j++) {
                    string from = citiesArray[j];
                    string to = citiesArray[i];
                    if (tickets[from][to][day] != INT_MAX) {
                        linIn += vars[from][to][day];
                    }
                    if (tickets[to][from][day + 1] != INT_MAX) {
                        linOut += vars[to][from][day + 1];
                    }
                }
                model.addConstr(linIn >= linOut);
                model.addConstr(linIn <= linOut);
            }
        }

        model.write("model.lp");
        model.optimize();

        /// SOLUTION
        if (showOutput) {
            cout << model.get(GRB_DoubleAttr_ObjVal) << endl;
        }
        vector<string> solution;
        for (int day = 0; day < numberOfDays; day++) {
            for (int i = 0; i < numberOfCities; i++) {
                for (int j = 0; j < numberOfCities; j++) {
                    string from = citiesArray[i];
                    string to = citiesArray[j];
                    int price = tickets[from][to][day];
                    if (price == INT_MAX) continue;
                    if (vars[from][to][day].get(GRB_DoubleAttr_X) > 0) {
                        solution.push_back(from);
                        if (showOutput) {
                            cout << from << " " << to << " " << day << " " << price << endl;
                        }
                    }
                }
            }
        }

        if ( ! showOutput) {
            ofstream outputFile(argv[2]);

            outputFile << model.get(GRB_DoubleAttr_ObjVal) << endl;
            for (int i = 0; i < numberOfDays; i++) {
                if (i > 0) {
                    outputFile << " ";
                }
                outputFile << solution[i];
            }
            outputFile.close();
        }
    } catch (GRBException e) {
        cout << e.getMessage() << endl;
    }

    return 0;
}
