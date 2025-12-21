/*
riders.h
Author: Zihan Wang
*/

#ifndef _RIDERS_H_
#define _RIDERS_H_

#include <string>

// Base class for delivery riders
class Delivery_Rider
{
protected:
    std::string name;             // Rider's name
    std::string vehicle_type;     // Vehicle type ("Moped" or "Bicycle")
    int speed;                    // Rider's speed
    double max_distance;         // Maximum distance the rider can travel in a day
    double current_distance;     // Current distance traveled by the rider

public:
    /**
     * @brief Constructor for the Delivery_Rider class
     * Initializes a delivery rider with a given name, vehicle type, speed, and maximum distance.
     * 
     * @param name The name of the rider.
     * @param vehicle_type The type of vehicle ("Moped" or "Bicycle").
     * @param speed The speed of the rider.
     * @param max_distance The maximum distance the rider can travel in one day.
     */
    Delivery_Rider(std::string name, std::string vehicle_type, int speed, double max_distance);

    virtual ~Delivery_Rider() {}

    /**
     * @brief Get the name of the rider
     * 
     * @return The name of the rider as a string.
     */
    std::string get_name() const;

    /**
     * @brief Get the type of vehicle the rider uses
     * 
     * @return The vehicle type as a string ("Moped" or "Bicycle").
     */
    std::string get_vehicle_type() const;

    /**
     * @brief Check if the rider can take a specific order
     * 
     * @param order_distance The one-way distance of the order.
     * @param is_priority Indicates whether the order is a priority order.
     * 
     * @return Returns true if the rider can take the order, false otherwise.
     */
    bool can_take_order(double order_distance, bool is_priority) const;

    /**
     * @brief Add the distance (round trip) of an order to the rider's total daily mileage
     * 
     * @param distance The one-way distance of the order.
     */
    void add_distance(double distance);

    /**
     * @brief Reset the rider's daily mileage (to be called at the start of each day)
     */
    void reset_daily_mileage();
    
    /**
     * @brief Calculate the time required to complete the delivery (round trip)
     * 
     * @param distance The one-way distance of the order.
     * 
     * @return The time required for the round trip, in minutes.
     */
    double calculate_time(double distance) const;
};

// Derived class for Moped riders
class Moped_Rider : public Delivery_Rider
{
public:
    /**
     * @brief Constructor for the Moped_Rider class
     * 
     * @param name The name of the moped rider.
     * @param speed The speed of the moped rider.
     */
    Moped_Rider(std::string name, int speed);
};

// Derived class for Bicycle riders
class Bicycle_Rider : public Delivery_Rider
{
public:
    /**
     * @brief Constructor for the Bicycle_Rider class
     * 
     * @param name The name of the bicycle rider.
     * @param speed The speed of the bicycle rider.
     * @param max_dist The maximum distance the bicycle rider can travel in one day.
     */
    Bicycle_Rider(std::string name, int speed, double max_dist);
};

#endif
