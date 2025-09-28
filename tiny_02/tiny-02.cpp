#include "tiny-02.hpp"

#include <algorithm>
#include <tuple>
#include <sstream>

std::pair<size_t, std::vector<int>> parse_line(std::string const& line) {
    std::stringstream sstr(line);
    size_t line_size;
    sstr >> line_size;
    // int* data = new int[line_size];
    std::vector<int> data(line_size);
    for (size_t i = 0; i < line_size; ++i) {
        sstr >> data[i];
    }
    return {line_size, data};
}

bool validate_data(std::vector<int> data, size_t sz) {
    for (size_t i = 0; i < sz; ++i) {
        if (data[i] < 0 || data[i] > 255) {
            return false;
        }
    }
    return true;
}

std::vector<statistics> summarize_data(std::istream& in) {
    std::vector<statistics> summary;
    
    std::string line;
    while (std::getline(in, line)) {
        size_t num_data;
        std::vector<int> data;
        std::tie(num_data, data) = parse_line(line);
        if (validate_data(data, num_data)) {
            std::sort(data.begin(), data.end());
            auto median = (num_data % 2 == 1)? data[num_data/2] : (data[(num_data - 1) /2] + data[num_data/2]) / 2;
            summary.push_back({num_data, data[0], data[num_data-1], median});
        }    }
    
    return summary;
}
