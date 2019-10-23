#pragma once

#include <memory>
#include <limits>
#include <algorithm>

enum class generator_kind {
    random,
    mersenne,
    minstd
};


class numbers_generator {
    int int_min = std::numeric_limits<int>::max();
    int int_max = std::numeric_limits<int>::min();
    size_t int_count = 0;
    
    double double_min = std::numeric_limits<double>::max();
    double double_max = std::numeric_limits<double>::min();
    size_t double_count = 0;

    size_t bool_false_count = 0;
    size_t bool_true_count = 0;
    size_t bool_count = 0;
public:
    /**
     * Vygeneruje náhodné číslo v intervalu [0, 1)
     */
    double random_double() {
        double r = random_double_impl();
        double_min = std::min(double_min, r);
        double_max = std::max(double_max, r);
        double_count++;
        return r;
    }
    
    /**
     * Vygeneruje náhodné celé číslo v intervalu [lower, upper]
     */
    int random_int(int lower, int upper) {
        int r = random_int_impl(lower, upper);
        int_min = std::min(int_min, r);
        int_max = std::max(int_max, r);
        int_count++;
        return r;
    }
    
    /**
     * Vygeneruje náhodnou pravdivostní hodnotu (true/false)
     */
    bool random_bool() {
        bool r = random_bool_impl();
        
        (r) ? bool_true_count++ : bool_false_count++;
        bool_count++;
        return r;
    }

    virtual ~numbers_generator() = default;

    static std::unique_ptr<numbers_generator> create_by_kind(generator_kind kind, size_t seed);

    double min_generated_double() const {return double_min;}
    double max_generated_double() const {return double_max;}
    size_t doubles_generated() const {return double_count;}
    int min_generated_int() const {return int_min;}
    int max_generated_int() const {return int_max;}
    size_t ints_generated() const {return int_count;}
    size_t trues_generated() const {return bool_true_count;}
    size_t falses_generated() const {return bool_false_count;}
    size_t bools_generated() const {return bool_count;}
private:
    virtual double random_double_impl() = 0;
    virtual int random_int_impl(int lower, int upper) = 0;
    virtual bool random_bool_impl() = 0;
};
