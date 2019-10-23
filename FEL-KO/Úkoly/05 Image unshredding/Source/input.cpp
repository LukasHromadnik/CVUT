#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Stripe {
    vector<int> values;
    int width;
    int height;

    int value(int row, int col, int color);
};

int Stripe::value(int row, int col, int color) {
    int index = (row * width * 3) + col * 3 + color;
    return values[index];
}

struct Input {
    vector<Stripe *> stripes;
    int numberOfStripes;
    int stripeWidth;
    int stripeHeight;

    Input();
};

Input::Input() {
    this->numberOfStripes = 0;
    this->stripeWidth = 0;
    this->stripeHeight = 0;
    this->stripes = vector<Stripe *>();
}

Input * loadInput(const char * argv[]) {
    ifstream inFile(argv[1]);

    string line;
    string word;
    int lineNumber = 0;

    Input * input = new Input();

    while (getline(inFile, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Count word count
        int wordNumber = 0;

        Stripe * stripe = new Stripe();
        stripe->width = input->stripeWidth;
        stripe->height = input->stripeHeight;

        // Iterate over all "token"
        while (ss >> word) {

            // Convert input to the integer
            int value = stoi(word);

            if (lineNumber == 0) {
                if (wordNumber == 0) {
                    input->numberOfStripes = value;
                } else if (wordNumber == 1) {
                    input->stripeWidth = value;
                } else if (wordNumber == 2) {
                    input->stripeHeight = value;
                }
            } else {
                stripe->values.push_back(value);
            }

            // Increase word number
            wordNumber++;
        }

        if (lineNumber > 0) {
            input->stripes.push_back(stripe);
        }

        // Increase line number
        lineNumber++;
    }

    return input;
}
