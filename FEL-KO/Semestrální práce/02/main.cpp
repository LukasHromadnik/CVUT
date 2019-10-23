#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <climits>
#include <queue>
#include <sys/time.h>
#include <random>

#include "Source/utils.cpp"

using namespace std;

int main(int argc, const char * argv[]) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int start = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    Input input = loadInput(argv);

    long int limit = start + input.timeLimit * 1000;

    map<string, map<string, map<int, int> > > tickets;

    int numberOfCities = input.cities.size();

    vector<string> citiesArray = vector<string>(input.cities.begin(), input.cities.end());
    for (int day = 0; day < input.numberOfDays; day++) {
        for (int i = 0; i < numberOfCities; i++) {
            for (int j = 0; j < numberOfCities; j++) {
                string from = citiesArray[i];
                string to = citiesArray[j];
                tickets[from][to][day] = INT_MAX;
            }
        }
    }

    for (Ticket *ticket : input.tickets) {
        tickets[ticket->from][ticket->to][ticket->day] = ticket->price;
    }

    string currentCity = input.startCity;
    
    map<string, bool> seen;
    seen[currentCity] = 1;

    vector<string> path;
    path.push_back(input.startCity);
    int price = 0;

    for (int i = 0; i < input.numberOfDays - 1; i++) {
        int minPrice = INT_MAX;
        string minCity;
        for (int j = 0; j < numberOfCities; j++) {
            string to = citiesArray[j];
            if (seen[to]) continue;
            int currentPrice = tickets[currentCity][to][i];
            if (minPrice > currentPrice) {
                minPrice = currentPrice;
                minCity = to;
            }
        }
        seen[minCity] = 1;
        price += minPrice;
        currentCity = minCity;
        path.push_back(minCity);
    }
    price += tickets[currentCity][input.startCity][input.numberOfDays - 1];

    path.push_back(input.startCity);

    while (true) {
        gettimeofday(&tp, NULL);
        long int current = tp.tv_sec * 1000 + tp.tv_usec / 1000;

        if (current + 200 > limit) break;

        int i1 = rand() % numberOfCities;
        int i2 = rand() % numberOfCities;

        if (i1 == 0 || i2 == 0) continue;

        string city = path[i1];
        path[i1] = path[i2];
        path[i2] = city;

        int newPrice = 0;
        bool infeasibleSolution = false;

        for (int i = 0; i < (int) path.size(); i++) {
            string from = path[i];
            string to = path[i + 1];
            int ticketPrice = tickets[from][to][i];
            if (ticketPrice == INT_MAX) {
                infeasibleSolution = true;
                break;
            }
            newPrice += ticketPrice;
        }
        if (infeasibleSolution || newPrice > price) {
            string swap = path[i1];
            path[i1] = path[i2];
            path[i2] = swap;
        } else {
            price = newPrice;
        }
    }

    path.pop_back();

    ofstream outFile(argv[2]);
    outFile << price << endl;
    cout << price << endl;
    bool firstValue = true;
    for (string city : path) {
        cout << city << " ";

        if (firstValue) {
            outFile << city;
            firstValue = false;
        } else {
            outFile << " " << city;
        }
    }
    cout << endl;

    return 0;
}
