#include "Configuration.hpp"

#include <vector>

Configuration::Configuration(int argc, char ** argv) {
    std::vector<std::string> args;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            show_help = true;
            return;
        } else if (strcmp(argv[i], "-pp") == 0) {
            use_k_means_pp = true;
        } else if (strcmp(argv[i], "-i") == 0) {
            use_interactive = true;
        } else if (strcmp(argv[i], "-s") == 0) {
            save_result = true;
            if (i + 1 >= argc) {
                set_error("Undefined path for result to save");
                return;
            }
            save_filename = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0) {
            use_plot = true;
        } else if (strcmp(argv[i], "-m") == 0) {
            use_multithreading = true;
        } else if (argv[i][0] == '-') {
            std::string error = "Unknown argument ";
            error += argv[i];
            set_error(error);
            return;
        } else {
            args.push_back(argv[i]);
        }
    }

    if (args.size() < 2) {
        set_error("Too few arguments");
        return;
    }

    input_filename = args[0];
    number_of_clusters = 0;
    try {
        number_of_clusters = std::stoi(args[1]);
    } catch (std::invalid_argument) {
        set_error("Invalid argument for NUMBER OF CLUSTERS");
        return;
    }
}

void Configuration::set_error(std::string message) {
    has_error = true;
    error_message = message;
}
