#pragma once

#include <string>

struct Configuration {
    Configuration(int argc, char ** argv);

    bool use_k_means_pp = false;
    bool use_interactive = false;
    bool use_multithreading = false;
    bool use_plot = false;
    bool show_help = false;
    bool save_result = false;
    std::string save_filename;
    std::string input_filename;
    int number_of_clusters;
    bool has_error = false;
    std::string error_message;
private:
    void set_error(std::string message);
};
