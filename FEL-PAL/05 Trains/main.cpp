#include <iostream>
#include <string>
#include <iomanip>
#include <numeric>
#include <limits.h>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int levenshteinDistance(string s1, string s2) {
    int m(s1.size());
    int n(s2.size());

    if (m == 0) return n;
    if (n == 0) return m;

    int * costs = new int[n + 1];

    for (int k = 0; k <= n; k++) {
        costs[k] = k;
    }

    int i = 0;
    for (string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i) {
        costs[0] = i + 1;
        int corner = i;

        int j = 0;
        for (string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j) {
            int upper = costs[j + 1];
            if ( * it1 == * it2) {
                costs[j + 1] = corner;
            } else {
                int t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}

int main(int argc, const char * argv[]) {
    // Helpers for input
    string line, word;
    int lineNumber = 0;

    string input;
    string search;
    int maxEditDistance;

    // Iterate over all input lines
    while (getline(cin, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Parse input line
        vector<string> tokens;

        // Split input line by space and convert the split result to the integer value
        // Then push it to the `tokens` vector
        while (ss >> word) {
            tokens.push_back(word);
        }

        if (lineNumber == 0) {
            input = tokens[0];
        } else if (lineNumber == 1) {
            search = tokens[0];
        } else if (lineNumber == 2) {
            maxEditDistance = stoi(tokens[0]);
        }

        // Increase line number
        lineNumber++;
    }

    int maxLength = (int) search.size() + maxEditDistance;

    vector<int> minData;
    vector<int> maxData;

    for (int i = 0; i < (int) input.size(); i++) {
        int minDistance = INT_MAX;
        int maxDistance = INT_MIN;

        for (int j = 0; j < maxLength; j++) {
            if (i - j < 0) continue;
            
            string substring = input.substr(i - j, j + 1);
            
            int distance = levenshteinDistance(substring, search);
            if (distance > maxEditDistance) continue;

            // Minimal distance
            int shift = (i - j - 1 >= 0) ? minData[i - j - 1] : 0;
            shift += 1;
            shift = shift < 0 ? INT_MAX : shift;
            minDistance = min(minDistance, shift);

            // Maximal distance
            shift = (i - j - 1 >= 0) ? maxData[i - j - 1] : 0;
            shift += 1;
            shift = shift < 0 ? INT_MIN : shift;
            maxDistance = max(maxDistance, shift);
        }

        minData.push_back(minDistance);
        maxData.push_back(maxDistance);
    }

    cout << minData.back() << " " << maxData.back() << endl;

    return 0;
}
