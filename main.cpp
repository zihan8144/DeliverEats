/*
main.cpp
Author: Zihan Wang
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include "riders.h"

// === Constants (No Magic Numbers) ===
const double COST_STANDARD = 2.0;
const double COST_PRIORITY = 3.0;

// Record of riders who are delivering orders
struct ActiveDelivery {
    int return_time; // In minutes (the minute of the day)
    Delivery_Rider* rider;
};

/**
 * @brief Struct to hold daily statistics, replacing global variables.
 * Tracks deliveries, money, and missed orders for the current day.
 */

struct DailyStats {
    int total_deliveries = 0;
    double total_money = 0.0;
    int bicycle_deliveries = 0;
    double bicycle_money = 0.0;
    int moped_deliveries = 0;
    double moped_money = 0.0;
    int missed_orders = 0;

  /**
     * @brief Resets all statistics to zero.
     */
    void reset() {
        total_deliveries = 0;
        total_money = 0.0;
        bicycle_deliveries = 0;
        bicycle_money = 0.0;
        moped_deliveries = 0;
        moped_money = 0.0;
        missed_orders = 0;
    }
};

/**
 * @brief Splits a string into tokens based on a specified delimiter.
 */
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Converts a time string in "HH.MM" format to minutes.
 */
int to_minutes(const std::string& time_str) {
    size_t dot_pos = time_str.find('.');
    if (dot_pos == std::string::npos) return 0;
    int hours = std::stoi(time_str.substr(0, dot_pos));
    int mins = std::stoi(time_str.substr(dot_pos + 1));
    return hours * 60 + mins;
}

/**
 * @brief Converts a date string to a file name format.
 */
std::string get_output_filename(const std::string& date_str) {
    std::string filename;
    for (char c : date_str) {
        if (c != '/') filename += c;
    }
    return filename + ".dat";
}

/**
 * @brief Writes the daily summary to a file.
 */
void write_daily_summary(const std::string& date_str, const DailyStats& stats) {
    if (date_str.empty()) return;
    std::string fname = get_output_filename(date_str);
    std::ofstream out(fname);
    if (out.is_open()) {
        out << "Total deliveries: " << stats.total_deliveries << std::endl;
        out << "Total money: " << stats.total_money << std::endl;
        out << "Bicycle deliveries: " << stats.bicycle_deliveries << std::endl;
        out << "Bicycle money: " << stats.bicycle_money << std::endl;
        out << "Moped deliveries: " << stats.moped_deliveries << std::endl;
        out << "Moped money: " << stats.moped_money << std::endl;
        out << "Missed orders: " << stats.missed_orders << std::endl;
        out.close();
    }
}

/**
 * @brief Updates the queues by checking if any riders have returned.
 */
void update_returning_riders(int current_time, 
                           std::vector<ActiveDelivery>& active_deliveries, 
                           std::deque<Delivery_Rider*>& waiting_queue) {
    std::vector<int> to_remove_indices;
    for (int i = 0; i < (int)active_deliveries.size(); ++i) {
        if (active_deliveries[i].return_time <= current_time) {
            waiting_queue.push_back(active_deliveries[i].rider);
            to_remove_indices.push_back(i);
        }
    }
    for (int i = to_remove_indices.size() - 1; i >= 0; --i) {
        active_deliveries.erase(active_deliveries.begin() + to_remove_indices[i]);
    }
}

/**
 * @brief Processes a single order line from the input file.
 * Handles parsing, assigning riders, and updating statistics.
 * 
 * @param line The string line containing order details (Time:Dist:Type).
 * @param waiting_queue Reference to the queue of available riders.
 * @param active_deliveries Reference to the list of riders currently delivering.
 * @param stats Reference to the daily statistics object to update.
 */
 
void process_order(const std::string& line, 
                   std::deque<Delivery_Rider*>& waiting_queue,
                   std::vector<ActiveDelivery>& active_deliveries,
                   DailyStats& stats) {
    
    std::vector<std::string> parts = split(line, ':');
    if (parts.size() != 3) return;

    int order_time = to_minutes(parts[0]);
    double dist = std::stod(parts[1]);
    std::string type = parts[2];
    bool is_priority = (type == "Priority");

    // 1. Check for returned riders
    update_returning_riders(order_time, active_deliveries, waiting_queue);

    // 2. Try to assign the order
    Delivery_Rider* assigned_rider = nullptr;
    for (auto it = waiting_queue.begin(); it != waiting_queue.end(); ++it) {
        if ((*it)->can_take_order(dist, is_priority)) {
            assigned_rider = *it;
            waiting_queue.erase(it);
            break;
        }
    }

    // 3. Update stats or record missed order
    if (assigned_rider != nullptr) {
        assigned_rider->add_distance(dist);
        double cost = is_priority ? COST_PRIORITY : COST_STANDARD;
        
        stats.total_deliveries++;
        stats.total_money += cost;
        
        if (assigned_rider->get_vehicle_type() == "Bicycle") {
            stats.bicycle_deliveries++;
            stats.bicycle_money += cost;
        } else {
            stats.moped_deliveries++;
            stats.moped_money += cost;
        }

        double duration = assigned_rider->calculate_time(dist);
        int time_taken = (int)std::round(duration);
        active_deliveries.push_back({order_time + time_taken, assigned_rider});
    } else {
        stats.missed_orders++;
    }
}

/**
 * @brief Main function.
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./main <input_file>" << std::endl;
        return 1;
    }

    // Initialize riders (Heap allocation)
    std::vector<Delivery_Rider*> all_riders;
    all_riders.push_back(new Moped_Rider("Anne", 32));
    all_riders.push_back(new Moped_Rider("Jim", 28));
    all_riders.push_back(new Bicycle_Rider("Sue", 4, 23));
    all_riders.push_back(new Bicycle_Rider("Bill", 5, 17));
    all_riders.push_back(new Moped_Rider("James", 25));
    all_riders.push_back(new Moped_Rider("Amy", 24));
    all_riders.push_back(new Moped_Rider("Bob", 27));
    all_riders.push_back(new Bicycle_Rider("Steve", 3, 21));

    // Queues and Stats
    std::deque<Delivery_Rider*> waiting_queue;
    std::vector<ActiveDelivery> active_deliveries;
    DailyStats stats;

    std::ifstream file(argv[1]);
    if (!file.is_open()) return 1;

    std::string line;
    std::string current_date = "";

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        if (line.find('/') != std::string::npos) {
            // === New Day ===
            if (!current_date.empty()) {
                write_daily_summary(current_date, stats);
            }
            current_date = line;
            stats.reset(); // Reset stats
            waiting_queue.clear();
            active_deliveries.clear();
            
            for (auto* r : all_riders) {
                r->reset_daily_mileage();
                waiting_queue.push_back(r);
            }
            std::cout << "Processing Date: " << current_date << std::endl;
        } 
        else {
            // === Process Order (Delegated to function) ===
            process_order(line, waiting_queue, active_deliveries, stats);
        }
    }

    if (!current_date.empty()) {
        write_daily_summary(current_date, stats);
    }

    for (auto* r : all_riders) delete r;
    return 0;
}