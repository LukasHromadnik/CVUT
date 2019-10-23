#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <fstream>
#include <numeric>
#include <thread>
#include <queue>
#include <chrono>

#include "gnuplot-iostream.h"
#include "Configuration.hpp"

using Coord = std::pair<double, double>;
using Cluster = std::vector<Coord>;
using ClusterPoint = std::tuple<double, double, int>;

bool operator == (const Coord & lhs, const Coord & rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

struct Point {
    Point(Coord coord) : coord(coord), cluster(-1) { };
    Coord coord;
    int cluster;
};

std::ostream & operator << (std::ostream & os, const Point & point) {
    os << "[" << point.coord.first << " " << point.coord.second << "] " << point.cluster;

    return os;
}

double distance_between(Point * point, const Coord & rhs) {
    Coord lhs = point->coord;
    double distance = 0;
    
    distance += pow(lhs.first - rhs.first, 2);
    distance += pow(lhs.second - rhs.second, 2);
    
    return sqrt(distance);
}

void show_result(Gnuplot & gp, std::vector<Point *> points, std::vector<Coord> means, int number_of_clusters, int iteration) {
    int min_x = INT_MAX;
    int max_x = -INT_MAX;
    int min_y = INT_MAX;
    int max_y = -INT_MAX;

    std::vector<ClusterPoint> cluster_points;

    for (Point * point : points) {
        Coord coord = point->coord;

        int x = (int) coord.first;
        int y = (int) coord.second;

        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);

        ClusterPoint p = std::make_tuple(point->coord.first, point->coord.second, point->cluster);
        cluster_points.push_back(p);
    }

    min_x--;
    max_x++;
    min_y--;
    max_y++;

    std::vector<ClusterPoint> cluster_means;
    for (int i = 0; i < number_of_clusters; i++) {
        ClusterPoint p = std::make_tuple(means[i].first, means[i].second, i);
        cluster_means.push_back(p);
    }

    // Plot
    gp << "set key off\n";
    gp << "set xrange [" << min_x << ":" << max_x << "]\n";
    gp << "set yrange [" << min_y << ":" << max_y << "]\n";
    gp << "unset colorbox\n";
    gp << "set title 'K-Means algorithm, number of iterations: " << iteration << "'\n";

    gp << "plot '-' u 1:2:3 with points pt 7 ps 1 palette, '-' u 1:2:3 with points pt 7 ps 1 lc 8\n";

    gp.send1d(cluster_points);
    gp.send1d(cluster_means);
}

void help(std::ostream & os = std::cout) {
    os << std::endl;
    os << "Arguments:" << std::endl;
    os << "  --help     Displays usage" << std::endl;
    os << "  -pp        Use K-Means++ initialization of the means" << std::endl;
    os << "  -i         Plot results after each iteration" << std::endl;
    os << "  -s <name>  Save the final result as SVG file named <name>" << std::endl;
    os << "  -m         Enable multithreading" << std::endl;
    os << "  -p         Plot final result" << std::endl;
    os << std::endl;
    os << "Usage:" << std::endl;
    os << "kmeans [arguments] input-file number-of-clusters" << std::endl;
    os << std::endl;
}

void raise_error(const std::string & message, std::ostream & os = std::cout) {
    os << "[ERROR] " << message << std::endl;
    help(os);
    exit(1);
}

std::vector<Coord> load_input_from(std::string filename) {
    std::vector<Coord> coords;
    
    // Open filestream
    std::ifstream file(filename);

    // Check if the file is readable
    if (file.fail()) {
        raise_error("Invalid path to the INPUT FILE");
    }

    // Read all coords
    double x, y;
    while (file >> x >> y) {
        Coord coord = std::make_pair(x, y);
        coords.push_back(coord);
    }
    
    // Close the file
    file.close();

    return coords;
}

void compute_new_mean(Point * point, const int & number_of_clusters, const std::vector<Coord> & means) {
    int min_distance = INT_MAX;
    int min_mean = -1;

    for (int j = 0; j < number_of_clusters; j++) {
        Coord mean = means[j];

        double distance = distance_between(point, mean);

        if (distance < min_distance) {
            min_distance = distance;
            min_mean = j;
        }
    }

    point->cluster = min_mean;
}

template <typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}

int main(int argc, char ** argv) {
    // Parse command line arguments
    Configuration config = Configuration(argc, argv);

    // Show errors
    if (config.has_error) {
        raise_error(config.error_message);
    }

    // Show help
    if (config.show_help) {
        help();
        return 0;
    }

    // Load input
    std::vector<Coord> coords = load_input_from(config.input_filename);

    // Initialize all helper properties
    int number_of_points = coords.size();
    int iteration = 0;
    Gnuplot gp;

    // Random setup
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> means_distribution(0, number_of_points - 1);
    std::uniform_real_distribution<double> unif(0, 1);

    // Convert coords to the Points
    std::vector<Point *> points;
    for (int i = 0; i < number_of_points; i++) {
        points.push_back(new Point(coords[i]));
    }

    // auto start = std::chrono::high_resolution_clock::now();

    // Initialize means
    std::vector<Coord> means;
    if (config.use_k_means_pp) {
        // Select first mean at random
        int random_index = means_distribution(eng);
        means.push_back(coords[random_index]);

        for (int k = 1; k < config.number_of_clusters; k++) {
            // Compute distance to the closest mean
            std::vector<double> distances = std::vector<double>(number_of_points, 0);
            for (int i = 0; i < number_of_points; i++) {
                double min_distance = INT_MAX;
                Point * point = points[i];
                for (size_t j = 0; j < means.size(); j++) {
                    double distance = distance_between(point, means[j]);
                    if (distance < min_distance) {
                        min_distance = distance;
                    }
                }
                distances[i] = min_distance;
            }

            // Find new mean proportionally
            double distance_sum = std::accumulate(distances.begin(), distances.end(), 0);
            double random = unif(eng);
            double sum = 0;
            for (size_t i = 0; i < distances.size(); i++) {
                double current_distance = distances[i] / distance_sum;
                sum += current_distance;
                if (sum > random) {
                    means.push_back(coords[i]);
                    break;
                }
            }
        }
    } else {
        for (int i = 0; i < config.number_of_clusters; i++) {
            int random_index = means_distribution(eng);
            means.push_back(coords[random_index]);
        }
    }

    // K-means algorithm
    while (true) {

        if (config.use_interactive) {
            show_result(gp, points, means, config.number_of_clusters, iteration);
            usleep((iteration == 1) ? 3000 : 100);
        }

        // Assign each point to the nearest cluster
        int number_of_threads = std::thread::hardware_concurrency();
        std::vector<std::queue<int> > tasks = std::vector<std::queue<int> >(number_of_threads, std::queue<int>());
        for (int i = 0; i < number_of_points; i++) {
            tasks[i % number_of_threads].push(i);
        }

        auto thread_func = [&points, &config, &means](std::queue<int> tasks) {
            while (tasks.empty() == false) {
                int i = tasks.front();
                tasks.pop();
                compute_new_mean(points[i], config.number_of_clusters, means);
            }
        };
        
        if (config.use_multithreading) {
            std::vector<std::thread> threads;
            for (int i = 0; i < number_of_threads; i++) {
                threads.push_back(std::thread(thread_func, tasks[i]));
            }
            for (size_t i = 0; i < threads.size(); i++) {
                threads.at(i).join();
            }
        } else {
            for (int i = 0; i < number_of_threads; i++) {
                thread_func(tasks[i]);
            }
        }

        std::vector<Cluster> clusters = std::vector<Cluster>(config.number_of_clusters, Cluster());

        // Update means
        for (int i = 0; i < number_of_points; i++) {
            Point * point = points[i];
            clusters[point->cluster].push_back(point->coord);
        }

        // Flag if the solution changed
        bool found = false;

        // Compute new mean for each cluster
        for (int i = 0; i < config.number_of_clusters; i++) {
            Coord new_mean = std::make_pair(0, 0);
            
            for (Coord coord : clusters[i]) {
                new_mean.first += coord.first;
                new_mean.second += coord.second;
            }

            // Compute average
            double size = clusters[i].size();
            new_mean.first = (size == 0) ? 0 : new_mean.first / size;
            new_mean.second = (size == 0) ? 0 : new_mean.second / size;
            
            if (new_mean != means[i]) {
                means[i] = new_mean;
                found = true;
            }
        }

        iteration++;
        
        // If solution doesn't change, break the loop
        if (found == false) break;
    }

    // auto end = std::chrono::high_resolution_clock::now();
    // std::cout << "Needed " << to_ms(end - start).count() << " ms to finish.\n";

    if (config.use_plot) {
        show_result(gp, points, means, config.number_of_clusters, iteration);
    }

    if (config.save_result) {
        gp << "set term svg\n";
        gp << "set output '" << config.save_filename << "'\n";
        if (config.use_plot) {
            gp << "replot\n";
        } else {
            show_result(gp, points, means, config.number_of_clusters, iteration);
        }
        gp << "unset output\n";
    }

    for (Point * point : points) {
        std::cout << (*point) << std::endl;
    }

    // Memory clean
    while (points.empty() == false) {
        Point * point = points.back();
        points.pop_back();
        delete point;
    }

    return 0;
}
