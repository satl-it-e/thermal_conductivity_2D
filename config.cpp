#include "config.h"


bool MyConfig::set_file(std::string &filename, const std::list<std::string> &s_values) {
    filename = s_values.front();
    return true;
}


bool MyConfig::set_unsigned_double(double &value, const std::list<std::string> &s_values){
    double ch = std::stod(s_values.front());
    if (0 < ch){
        std::istringstream ss (s_values.front());
        return (ss >> value) && (value > 0);
    }
    return false;
}

bool MyConfig::set_unsigned_int(unsigned int &value, const std::list<std::string> &s_values){
    int ch = std::stoi(s_values.front());
    if (0 < ch){
        std::istringstream ss (s_values.front());
        return (ss >> value) && (value > 0);
    }
    return false;
}


bool MyConfig::is_configured(){
    return check_set.empty();
}

int MyConfig::load_configs_from_file(const std::string &f_name){

    std::map< std::string, std::function<bool(const std::list<std::string>&)> > cnf;

    cnf.emplace(std::make_pair("map_file", [this](const std::list<std::string> &s_values)-> bool {return set_file(map_file, s_values);}));
    cnf.emplace(std::make_pair("heat_capacity", [this](const std::list<std::string> &s_values)-> bool {return set_unsigned_double(heat_capacity, s_values);}));
    cnf.emplace(std::make_pair("thermal_conductivity", [this](const std::list<std::string> &s_values)-> bool {return set_unsigned_double(thermal_conductivity, s_values);}));
    cnf.emplace(std::make_pair("density", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_double(density, s_values);}));
    cnf.emplace(std::make_pair("width", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_double(width, s_values);}));
    cnf.emplace(std::make_pair("length", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_double(length, s_values);}));
    cnf.emplace(std::make_pair("delta_x", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_double(delta_x, s_values);}));
    cnf.emplace(std::make_pair("delta_y", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_double(delta_y, s_values);}));
    cnf.emplace(std::make_pair("delta_t", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_double(delta_t, s_values);}));
    cnf.emplace(std::make_pair("num_of_threads", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_int(num_of_threads, s_values);}));
    cnf.emplace(std::make_pair("visualization_interval", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_int(visualization_interval, s_values);}));
    cnf.emplace(std::make_pair("num_of_steps", [this](const std::list<std::string> s_values)-> bool { return set_unsigned_int(num_of_steps, s_values);}));
    cnf.emplace(std::make_pair("visualization_filename", [this](const std::list<std::string> s_values)-> bool { return set_file(visualization_filename, s_values);}));
    cnf.emplace(std::make_pair("last_state_filename", [this](const std::list<std::string> s_values)-> bool { return set_file(last_state_filename, s_values);}));

    try{

        // read config file line by line
        std::ifstream f(f_name);
        if (f){
            std::string line;
            while (getline(f, line)){
                std::list<std::string> content;
                int c = 0;
                for(int i = 0; i < line.length(); i++) {
                    if (isspace(line[i])){
                        if (c!= i){
                            content.emplace_back(line.substr(c, i-c));
                        }
                        c = i+1;
                    }
                }
                if (c != line.length()){ content.emplace_back(line.substr(c, line.length()-c)); }

                // load values into attributes
                std::string cnf_name = content.front();
                content.pop_front();

                if ( cnf.find(cnf_name) != cnf.end()){
                    if ( cnf[cnf_name](content) ){
                        check_set.erase (cnf_name);
                    } else { std::cerr << "Error. Couldn't load " + cnf_name + "\n" << std::endl; return -3; }
                }
            }
            alpha = thermal_conductivity / density / heat_capacity;
            f.close();
            return 0;

        } else { std::cerr << "File couldn't be opened."; return -1; }
    } catch(std::string &err){ std::cout << err << std::endl; return -2; }
}