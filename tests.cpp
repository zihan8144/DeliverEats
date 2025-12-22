/*
tests.cpp
Unit tests for Deliver Eats using Catch2
Author: Zihan Wang
*/

#define CATCH_CONFIG_MAIN  
#include "catch.hpp"
#include "riders.h"

TEST_CASE("Rider Initialization", "[rider]") {
    Moped_Rider moped("TestMoped", 30);
    Bicycle_Rider bike("TestBike", 10, 20.0);

    SECTION("Check names and types") {
        REQUIRE(moped.get_name() == "TestMoped");
        REQUIRE(moped.get_vehicle_type() == "Moped");
        REQUIRE(bike.get_name() == "TestBike");
        REQUIRE(bike.get_vehicle_type() == "Bicycle");
    }
}

TEST_CASE("Moped Logic", "[moped]") {
    Moped_Rider moped("Moped1", 30); // 30 mph

    SECTION("Can take priority orders") {
        REQUIRE(moped.can_take_order(1.0, true) == true);
    }

    SECTION("Can take standard orders") {
        REQUIRE(moped.can_take_order(1.0, false) == true);
    }
    
    SECTION("Can take long distance orders") {
        REQUIRE(moped.can_take_order(10.0, false) == true);
    }
    
    SECTION("Time calculation") {
        // 10 miles. Round trip 20 miles. Speed 30 mph.
        // Time = 20 / 30 * 60 = 40 minutes.
        REQUIRE(moped.calculate_time(10.0) == Approx(40.0));
    }
}

TEST_CASE("Bicycle Logic", "[bicycle]") {
    // Name: Bike1, Speed: 10mph, MaxDist: 10 miles
    Bicycle_Rider bike("Bike1", 10, 10.0); 

    SECTION("Cannot take priority orders") {
        REQUIRE(bike.can_take_order(1.0, true) == false);
    }

    SECTION("Cannot take orders > 2 miles (PDF Requirement)") {
        REQUIRE(bike.can_take_order(2.1, false) == false);
        REQUIRE(bike.can_take_order(2.0, false) == true);
    }

    SECTION("Max distance logic") {
        // Currently 0 miles used.
        // Order distance 2.0 -> Trip is 4.0 miles.
        // 0 + 4.0 <= 10.0. OK.
        REQUIRE(bike.can_take_order(2.0, false) == true);
        
        // Simulate taking a trip
        bike.add_distance(2.0); // +4.0 miles total used
        
        // Now used 4.0. Max is 10.0. Remaining capacity: 6.0.
        // Next order: 2.0 miles -> Trip 4.0.
        // 4.0 + 4.0 = 8.0 <= 10.0. OK.
        REQUIRE(bike.can_take_order(2.0, false) == true);
        
        bike.add_distance(2.0); // +4.0 miles. Total used 8.0.
        
        // Remaining capacity: 2.0.
        // Next order: 1.5 miles -> Trip 3.0.
        // 8.0 + 3.0 = 11.0 > 10.0. Fail.
        REQUIRE(bike.can_take_order(1.5, false) == false);
    }
}