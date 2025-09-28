#include "telescope.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

std::pair<size_t, size_t> parse_matrix(std::istream& in) {
    string line;

    size_t rows = 0;
    size_t cols = 0;

    while(getline(in, line)) {
        istringstream row_stream(line);
        vector<int> row;
        int value;

        while(row_stream >> value) {
            row.push_back(value);
        }

        if(cols != 0 && cols != row.size()) {
            throw invalid_argument("Inconsistent number of columns.");
        }

        cols = row.size();
        rows++;
    };

    return {rows, cols};
}

std::vector<int> parse_matrix(std::istream& in, const std::pair<size_t, size_t>& m_size) {
    string line;
    vector<int> matrix;
    int row_size = 0;

    while(getline(in, line)) {
        istringstream row_stream(line);
        int value;
        vector<int> row;

        while(row_stream >> value) {
            matrix.push_back(value);
            row.push_back(value);
        }

        if(row_size != 0 && row_size != (int)row.size()) {
            throw invalid_argument("Inconsistent number of columns.");
        }

        row_size = (int)row.size();
    }

    return matrix;
}

int get_length(int num) {
    return to_string(num).size();
}

string get_spaces(int value, int max_num) {
    int len = get_length(value);
    return string(max_num - len, ' ');
}

void print_matrix(std::ostream& out, const std::pair<size_t, size_t>& m_size, const std::vector<int>& vec) {
    size_t rows = m_size.first;
    size_t cols = m_size.second;

    // Find max number
    int max_len = 0;
    for(int i : vec) {
        max_len = max(max_len, get_length(i));
    }

    for(size_t i = 0; i < rows; i++) {
        // Print first border of matrix
        if(i == 0) {
            int matrix_width = (max_len + 2) * cols + cols + 1;
            // cout << string(matrix_width, '-') << endl;
            out << string(matrix_width, '-') << endl;
        }
        for (size_t j = 0; j < cols; j++) {
            int idx = i * cols + j;
            int value = vec[idx];
            if(idx % m_size.second == 0) {
                // cout << "| " << get_spaces(value, max_len) << value << " |";
                out << "| " << get_spaces(value, max_len) << value << " |";
            } else {
                // cout << " " << get_spaces(value, max_len) << value << " |";
                out << " " << get_spaces(value, max_len) << value << " |";
            }
        }
        out << endl;
        // Print last border of matrix
        if(i == (rows - 1)) {
            int matrix_width = (max_len + 2) * cols + cols + 1;
            out << string(matrix_width, '-') << endl;
        }
    }
}

std::vector<unsigned char> parse_stream(std::istream& in, const std::pair<size_t, size_t>& m_size) {
    size_t rows = m_size.first;
    size_t cols = m_size.second;
    unsigned char value;
    vector<unsigned char> mat;

    while(in.get(reinterpret_cast<char&>(value))) {
        mat.push_back(value);
    }

    if(mat.size() != (rows * cols)) {
        throw invalid_argument("Inconsistent number of columns.");
    }

    return mat;
}

void rotate_down(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    size_t cols = m_size.second;

    // Reverse vector
    reverse(vec.begin(), vec.end());
    // Get first cols elements
    vector<int> first_el(vec.begin(), vec.begin() + cols);
    // Reverse first elements
    reverse(first_el.begin(), first_el.end());
    // Remove first cols elements
    vec.erase(vec.begin(), vec.begin() + cols);
    // Reverse the vector back
    reverse(vec.begin(), vec.end());
    // Append first elements to the beginning of the updated vector
    vec.insert(vec.begin(), first_el.begin(), first_el.end());
}

void rotate_up(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    size_t cols = m_size.second;

    // Get first cols elements
    vector<int> first_el(vec.begin(), vec.begin() + cols);
    // Remove first cols elements
    vec.erase(vec.begin(), vec.begin() + cols);
    // Append first elements to the end of the updated vector
    vec.insert(vec.end(), first_el.begin(), first_el.end());
}

void rotate_down(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, int step) {
    size_t rows = m_size.first;
    int updated_step = 0;

    if(step == 0) {
        return;
    }else if(step > 0) {
        updated_step = step % rows;
        for(int i = 0; i < updated_step; i++) {
            rotate_down(m_size, vec);
        }
    } else {
        updated_step = (-step) % rows;
        for(int i = 0; i < updated_step; i++) {
            rotate_up(m_size, vec);
        }
    }
}

void rotate_right(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    size_t rows = m_size.first;
    size_t cols = m_size.second;

    for (size_t r = 0; r < rows; r++) {
        unsigned char last = vec[r * cols + (cols - 1)];

        for (size_t c = cols - 1; c > 0; c--) {
            vec[r * cols + c] = vec[r * cols + (c - 1)];
        }
        vec[r * cols + 0] = last;
    }
}

void rotate_left(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    size_t rows = m_size.first;
    size_t cols = m_size.second;

    for(size_t r = 0; r < rows; r++) {
        unsigned char first = vec[r * cols + 0];
        for(size_t c = 0; c < (cols - 1); c++) {
            vec[r * cols + c] = vec[r * cols + (c + 1)];
        }
        vec[r * cols + (cols - 1)] = first;
    }
}

void rotate_right(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, int step) {
    size_t cols = m_size.second;
    int updated_step = 0;

    if(step == 0) {
        return;
    }else if(step > 0) {
        updated_step = step % cols;
        for(int i = 0; i < updated_step; i++) {
            rotate_right(m_size, vec);
        }
    } else {
        updated_step = (-step) % cols;
        for(int i = 0; i < updated_step; i++) {
            rotate_left(m_size, vec);
        }
    }
}

void swap_points(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, const Point& p1, const Point& p2) {
    size_t rows = m_size.first;
    size_t cols = m_size.second;

    if(p1.x >= cols) {
        throw invalid_argument("Invalid point p1.");
    } else if(p1.y >= rows) {
        throw invalid_argument("Invalid point p1.");
    }else if(p2.x >= cols) {
        throw invalid_argument("Invalid point p2.");
    } else if(p2.y >= rows) {
        throw invalid_argument("Invalid point p2.");
    }

    unsigned char first_el = vec[p1.y * cols + p1.x];
    unsigned char second_el = vec[p2.y * cols + p2.x];

    vec[p2.y * cols + p2.x] = first_el;
    vec[p1.y * cols + p1.x] = second_el;
}

void swap_points(const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec, const Point& p1, const Point& p2, const Point& delta) {
    size_t rows = m_size.first;
    size_t cols = m_size.second;

    vector<int> first_shape_coords;
    vector<int> second_shape_coords;

    if(p1.x >= cols || p1.x + delta.x > cols) {
        throw invalid_argument("Invalid point p1.");
    } else if(p1.y >= rows || p1.y + delta.y > rows) {
        throw invalid_argument("Invalid point p1.");
    }else if(p2.x >= cols || p2.x + delta.x > cols) {
        throw invalid_argument("Invalid point p2.");
    } else if(p2.y >= rows || p2.y + delta.y > rows) {
        throw invalid_argument("Invalid point p2.");
    }

    // Check if the two point overlap
    size_t smaller_x = min(p1.x, p2.x);
    size_t smaller_y = min(p1.y, p2.y);
    size_t bigger_x = max(p1.x, p2.x);
    size_t bigger_y = max(p1.y, p2.y);

    if(smaller_x + delta.x > bigger_x && smaller_y + delta.y > bigger_y) {
        throw invalid_argument("Invalid points");
    }

    for(size_t i = 0; i < delta.y; i++) {
        for(size_t j = 0; j < delta.x; j++) {
            int new_p1_x = p1.x + j;
            int new_p1_y = p1.y + i;
            first_shape_coords.push_back(new_p1_y * cols + new_p1_x);
            int new_p2_x = p2.x + j;
            int new_p2_y = p2.y + i;
            second_shape_coords.push_back(new_p2_y * cols + new_p2_x);
        }
    }

    for(size_t i = 0; i < first_shape_coords.size(); i++) {
        int first_coord = first_shape_coords[i];
        int second_coord = second_shape_coords[i];
        unsigned char first_coord_item = vec[first_coord];
        vec[first_coord] = vec[second_coord];
        vec[second_coord] = first_coord_item;
    }
}

void decode_picture(const std::string& file, const std::pair<size_t, size_t>& m_size, std::vector<unsigned char>& vec) {
    ifstream f(file);
    if(!f.is_open()) {
        cerr << "Failed to open file: " << file << endl;
        return;
    }

    string line;

    while(getline(f, line)) {
        istringstream line_stream(line);
        char command;

        line_stream >> command;

        switch (command) {
            case 'r': {
                int value;
                if(line_stream >> value) {
                    rotate_right(m_size, vec, value);
                } else {
                    rotate_right(m_size, vec);
                }
                break;
            }
            case 'l': {
                int value;
                if(line_stream >> value) {
                    rotate_right(m_size, vec, value * (-1));
                } else {
                    rotate_left(m_size, vec);
                }
                break;
            }
            case 'd': {
                int value;
                if(line_stream >> value) {
                    rotate_down(m_size, vec, value);
                } else {
                    rotate_down(m_size, vec);
                }
                break;
            }
            case 'u': {
                int value;
                if(line_stream >> value) {
                    rotate_down(m_size, vec, value * (-1));
                } else {
                    rotate_up(m_size, vec);
                }
                break;
            }
            case 's': {
                int p1_x, p1_y, p2_x, p2_y, delta_x = 0, delta_y = 0;
                line_stream >> p1_x >> p1_y >> p2_x >> p2_y;
                if(line_stream >> delta_x) {
                    line_stream >> delta_y;
                    swap_points(m_size, vec, Point(p1_x, p1_y), Point(p2_x, p2_y), Point(delta_x, delta_y));
                } else {
                    swap_points(m_size, vec, Point(p1_x, p1_y), Point(p2_x, p2_y));
                }
                break;
            }
            default: {
                cerr << "Unknown command: " << command << endl;
            }
        }
    }

    f.close();
}
