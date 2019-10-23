#include <iostream>
#include <limits>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

void write_stats(std::vector<double> const& data, std::ostream& out) {
    double minimum;
    double sum = 0;
    double maximum;

    for (int i = 0; i < (int) data.size(); i++) {
        double number = data[i];
        if (i == 0) {
            minimum = number;
            maximum = number;
        } else {
            minimum = min(minimum, number);
            maximum = max(maximum, number);
        }
        sum += number;    
    }

    out << "min: " << fixed << setprecision(2) << minimum << endl;
    out << "max: " << fixed << setprecision(2) << maximum << endl;
    out << "mean: " << fixed << setprecision(2) << sum / data.size() << endl;
}

int main() {
    stringstream sstr;
    write_stats({ -numeric_limits<double>::max(), -numeric_limits<double>::max() }, sstr);

    cout << sstr.str() << endl;

    return 0;
}