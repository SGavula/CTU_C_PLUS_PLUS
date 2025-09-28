#pragma once

#include <memory>
#include <limits>

using namespace std;

enum class generator_kind {
    random,
    mersenne,
    minstd
};


class numbers_generator {
public:
    numbers_generator() {
        min_int = numeric_limits<int>::max();
        max_int = numeric_limits<int>::min();
        min_double = numeric_limits<double>::max();
        max_double = numeric_limits<double>::min();
        int_count = 0;
        double_count = 0;
        bool_count = 0;
        true_count = 0;
        false_count = 0;
    }

    /**
     * Vygeneruje náhodné číslo v intervalu [0, 1)
     */
    virtual double random_double() = 0;
    /**
     * Vygeneruje náhodné celé číslo v intervalu [lower, upper]
     */
    virtual int    random_int(int lower, int upper) = 0;
    /**
     * Vygeneruje náhodnou pravdivostní hodnotu (true/false)
     */
    virtual bool   random_bool() = 0;

    virtual ~numbers_generator() = default;

    static std::unique_ptr<numbers_generator> create_by_kind(generator_kind kind, size_t seed);

    double min_generated_double() const {return min_double;}
    double max_generated_double() const {return max_double;}
    size_t doubles_generated() const {return double_count;}
    int min_generated_int() const {return min_int;}
    int max_generated_int() const {return max_int;}
    size_t ints_generated() const {return int_count;}
    size_t trues_generated() const {return true_count;}
    size_t falses_generated() const {return false_count;}
    size_t bools_generated() const {return bool_count;}

protected:
    void process_double(double generated_num) {
        // Compare to max number
        if(generated_num > max_double) {
            max_double = generated_num;
        }

        // Compare to min number
        if(generated_num < min_double) {
            min_double = generated_num;
        }

        double_count++;
    }

    void process_int(int generated_num) {
        // Compare to max number
        if(generated_num > max_int) {
            max_int = generated_num;
        }

        // Compare to min number
        if(generated_num < min_int) {
            min_int = generated_num;
        }

        int_count++;
    }

    void process_bool(bool generated_bool) {
        if(generated_bool) {
            true_count++;
        } else {
            false_count++;
        }

        bool_count++;
    }

private:
    int min_int, max_int;
    double min_double, max_double;
    size_t int_count, double_count, bool_count, true_count, false_count;
};
