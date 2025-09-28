#include "tiny-00.hpp"

#include <ostream>
#include <bits/stdc++.h>
#include <numeric>
#include <iomanip>
// don't forget to include appropriate headers

#define NUM_OF_DECIMAL_PLACES 2

using namespace std;

void write_stats(std::vector<double> const& data, std::ostream& out) {
    // implement me
    if(data.size() == 0) {
        return;
    }

    double max = *max_element(data.begin(), data.end());
    double min = *min_element(data.begin(), data.end());
    double sum = accumulate(data.begin(), data.end(), 0.0);
    double average = sum / data.size();

    out << "min: " << fixed << setprecision(NUM_OF_DECIMAL_PLACES) << min << "\n";
    out << "max: " << fixed << setprecision(NUM_OF_DECIMAL_PLACES) << max << "\n";
    out << "mean: " << fixed << setprecision(NUM_OF_DECIMAL_PLACES) << average << "\n";
}
