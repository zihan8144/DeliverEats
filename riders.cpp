
/*
riders.cpp
Author: Zihan Wang
*/

#include <limits> 
#include "riders.h"

// === Base Class ===

/**
 * @brief Constructor for Delivery_Rider class.
 * Initializes a delivery rider with a given name, vehicle type, speed, and maximum distance.
 * 
 * @param name Name of the rider.
 * @param vehicle_type Type of the vehicle ("Bicycle" or "Moped").
 * @param speed Speed of the rider in some units.
 * @param max_distance Maximum distance the rider can travel in one day.
 */
Delivery_Rider::Delivery_Rider(std::string name, std::string vehicle_type, int speed, double max_distance)
    : name(name), vehicle_type(vehicle_type), speed(speed), max_distance(max_distance), current_distance(0.0)
{
}

/**
 * @brief Gets the name of the rider.
 * 
 * @return The name of the rider.
 */
std::string Delivery_Rider::get_name() const { return name; }

/**
 * @brief Gets the type of vehicle the rider uses.
 * 
 * @return The vehicle type ("Bicycle" or "Moped").
 */
std::string Delivery_Rider::get_vehicle_type() const { return vehicle_type; }

/**
 * @brief Determines if the rider can take a given order.
 * 
 * @param order_distance The distance of the delivery in miles.
 * @param is_priority Flag indicating whether the order is a priority order.
 * 
 * @return True if the rider can take the order, false otherwise.
 */
bool Delivery_Rider::can_take_order(double order_distance, bool is_priority) const
{
    // 1. Only Moped can take priority orders
    if (is_priority && vehicle_type == "Bicycle") return false;

    // 2. Bicycles cannot handle deliveries over 2 miles one way
    if (vehicle_type == "Bicycle" && order_distance > 2.0) return false;

    // 3. Check the maximum distance (including round-trip)
    double trip = order_distance * 2.0;
    // 999999 represents infinity (for Moped)
    if (max_distance < 90000 && (current_distance + trip) > max_distance) return false;

    return true;
}

/**
 * @brief Adds a given distance to the rider's total traveled distance.
 * The distance is doubled to account for the round-trip.
 * 
 * @param distance The distance traveled for the order.
 */
void Delivery_Rider::add_distance(double distance)
{
    current_distance += (distance * 2.0);
}

/**
 * @brief Resets the rider's daily mileage to zero.
 */
void Delivery_Rider::reset_daily_mileage()
{
    current_distance = 0.0;
}

/**
 * @brief Calculates the time required to complete a delivery based on the distance and rider's speed.
 * The time is calculated for the round-trip distance.
 * 
 * @param distance The one-way distance of the delivery.
 * 
 * @return The time required for the round-trip, in minutes.
 */
double Delivery_Rider::calculate_time(double distance) const
{
    double total_distance = distance * 2.0; // Round-trip distance
    double hours = total_distance / (double)speed;
    return hours * 60.0; // Returns time in minutes
}

// === Derived Classes ===

/**
 * @brief Constructor for Moped_Rider class.
 * Initializes a moped rider with a given name and speed.
 * 
 * @param name Name of the rider.
 * @param speed Speed of the moped rider.
 */
Moped_Rider::Moped_Rider(std::string name, int speed)
    : Delivery_Rider(name, "Moped", speed, std::numeric_limits<double>::max()) {}

/**
 * @brief Constructor for Bicycle_Rider class.
 * Initializes a bicycle rider with a given name, speed, and maximum distance.
 * 
 * @param name Name of the rider.
 * @param speed Speed of the bicycle rider.
 * @param max_dist Maximum distance the bicycle rider can travel in one day.
 */
Bicycle_Rider::Bicycle_Rider(std::string name, int speed, double max_dist)
    : Delivery_Rider(name, "Bicycle", speed, max_dist) {}
