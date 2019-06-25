#include <iostream>
#include <algorithm>
#include <math.h>
#include <thread>

#include <mutex>

#include "config.h"
#include "my_queue.h"
#include "visualization.h"


std::mutex new_mtx;
std::mutex old_mtx;


bool system_is_stable(const MyConfig &mc){
    return mc.delta_t <= (pow(std::max(mc.delta_x, mc.delta_y), 2) / (4 * mc.alpha));
}


double new_temperature(const MyConfig &mc, const unsigned long i, const unsigned long j, const std::vector<std::vector<double>> &old_field){
    std::lock_guard<std::mutex> lock(old_mtx);
    return old_field.at(i).at(j) +  mc.delta_t * mc.alpha *
    (((old_field.at(i - 1).at(j) - 2 * old_field.at(i).at(j) + old_field.at(i + 1).at(j)) / pow(mc.delta_x, 2)) +
    ((old_field.at(i).at(j - 1) - 2 * old_field.at(i).at(j) + old_field.at(i).at(j + 1)) / pow(mc.delta_y, 2)));
}

void next_iteration_thread(std::vector<std::vector<double>> &new_field, const MyConfig &mc, const std::vector<std::vector<double>> &old_field,
        const unsigned long i_down, const unsigned long i_up, const unsigned long j_down, const unsigned long j_up){
    for (unsigned long i = i_down; i <= i_up; i++){
        std::vector<double> row;
        for (unsigned long j = j_down; j <= j_up; j++){
            row.push_back(new_temperature(mc, i , j, old_field));
        }
        new_mtx.lock();
        new_field[i] = row;
        new_mtx.unlock();
    }
}


std::vector<std::vector<double>> next_iteration(const MyConfig &mc, const std::vector<std::vector<double>> &old_field){
    std::vector<std::vector<double>> new_field = old_field;
    std::vector<double> row;
    int length = (int) old_field.size(), width = (int) old_field[0].size();
    std::vector<std::thread> threads;
    int delta_rows = (length - 2) / mc.num_of_threads;

    for (int i = 0; i < mc.num_of_threads - 1; i++){
        threads.emplace_back(next_iteration_thread, std::ref(new_field), std::ref(mc), std::ref(old_field),
                i * delta_rows + 1, (i + 1) * delta_rows, 1, width - 2);
    }
    threads.emplace_back(next_iteration_thread, std::ref(new_field),
            std::ref(mc), std::ref(old_field), (mc.num_of_threads - 1) * delta_rows + 1, length - 2, 1, width - 2);

    for (auto &thread: threads){
        thread.join();
    }

    for (int i = 1; i < length - 1; i++){
        new_field[i].insert(new_field[i].begin(), old_field[i][0]);
        new_field[i].push_back(old_field[i][width - 1]);
    }
    return new_field;
}


void visualization_thread(MyQueue<std::vector<std::vector<double>>> &v_q, const std::string &out_filename,
        const double &field_min, const double &field_max){
    int i = 0;
    std::vector<std::vector<std::vector<double>>> result;
    while (true){
        result = v_q.pop();
        if (result.empty()){
            break;
        }
        std::string filename(std::to_string(i) + "_" + out_filename);
        visualize(result.front(), filename, field_min, field_max);
        i++;
    }
}


int main(int argc, char* argv[]){

    // Configurations
    std::string conf_file_name = "config.dat";
    if (argc >= 2){
        conf_file_name = argv[1];
    }

    MyConfig mc;
    mc.load_configs_from_file(conf_file_name);
    if (mc.is_configured()) {
        std::cout << "Configurations loaded successfully.\n" << std::endl;
    } else {
        std::cerr << "Error. Not all configurations were loaded properly.";
        return -1;
    }

    // Check system stability
    if (!system_is_stable(mc)){
        std::cerr << "System is not stable." << std::endl;
        return -2;
    }

    // First iteration
    std::cout << "Reading first iteration." << std::endl;
    std::vector<std::vector<double>> field;
    double field_min = INFINITY, field_max = -INFINITY;

    std::ifstream file(mc.map_file);
    try{
        if (file.is_open()){
            std::istream& cf = file;

            std::ios::fmtflags flags( cf.flags() ); // Save stream state
            cf.exceptions(std::ifstream::failbit); // Enable exception on fail
            std::string line;

            for(int i = 0; i < (int) (mc.length / mc.delta_y); i++){
                std::vector<double> row;
                for(int j = 0; j < (int) (mc.width / mc.delta_x); j++){
                    double value;
                    cf >> value;

                    // Looking for min and max on the field
                    if (value < field_min){
                        field_min = value;
                    } else if (value > field_max){
                        field_max = value;
                    }

                    row.push_back(value);
                }
                getline(cf, line);
                field.push_back(row);
            }
        } else{
            std::cerr << "File " << mc.map_file << " couldn't be opened." << std::endl;
            return -4;
        }
    } catch(std::string &err){
        std::cerr << "Something wrong with loading field." << std::endl;
        std::cerr << err << std::endl;
        return -3;
    }

    // Visualization queue
    MyQueue<std::vector<std::vector<double> > > vis_q;
    std::thread visualization(visualization_thread, std::ref(vis_q), std::ref(mc.visualization_filename),
            std::ref(field_min), std::ref(field_max));

    std::cout << "Next iterations." << std::endl;

    // Iterations
    for (int i = 0; i < mc.num_of_steps; i++){
        for (int j = 0; j < mc.visualization_interval; j++){
            field = next_iteration(mc, field);
        }
        vis_q.push(field);
    }


    vis_q.finish();
    visualization.join();

    std::cout << "Finish." << std::endl;
    return 0;
}

