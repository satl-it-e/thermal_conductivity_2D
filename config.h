#ifndef THERMAL_CONDUCTIVITY_2D_CONFIG_H
#define THERMAL_CONDUCTIVITY_2D_CONFIG_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <sstream>
#include <map>
#include <fstream>
#include <unordered_set>


class MyConfig{

public:
    std::string map_file;
    double heat_capacity;
    double thermal_conductivity;
    double density;
    double width;
    double length;
    double delta_x;
    double delta_y;
    double delta_t;
    double alpha;
    unsigned int num_of_threads;
    unsigned int visualization_interval;
    unsigned int num_of_steps;
    std::string visualization_filename;

private:
    std::unordered_set<std::string> check_set = {"map_file",
                                                 "heat_capacity",
                                                 "thermal_conductivity",
                                                 "density",
                                                 "width",
                                                 "length",
                                                 "delta_x",
                                                 "delta_y",
                                                 "delta_t",
                                                 "num_of_threads",
                                                 "visualization_interval",
                                                 "num_of_steps",
                                                 "visualization_filename"};

public:
    bool set_file(std::string &filename, const std::list<std::string> &s_values);
    bool set_unsigned_double(double &value, const std::list<std::string> &s_values);
    bool set_unsigned_int(unsigned int &value, const std::list<std::string> &s_values);


    bool is_configured();
    int load_configs_from_file(const std::string &f_name);
};


#endif //THERMAL_CONDUCTIVITY_2D_CONFIG_H
