#ifndef CONSTRAINT_VALIDATOR_H
#define CONSTRAINT_VALIDATOR_H

#include <bits/stdc++.h>
#include "Models.h"
using namespace std;

class ConstraintValidator {
public:
    static bool checkCapacity(vector<Order> &route, Vehicle &vehicle) {
        double totalWeight = 0;

        for (auto &order : route) {
            totalWeight += order.weight;
        }

        return totalWeight <= vehicle.maxCapacity;
    }

    static bool checkFuel(double totalDistance, Vehicle &vehicle) {
        return totalDistance <= vehicle.fuelRange;
    }

    static bool checkTimeWindow(int arrivalTime, int deadline) {
        return arrivalTime <= deadline;
    }

    static void prioritizeOrders(vector<Order> &orders) {
        sort(orders.begin(), orders.end(), [](Order &a, Order &b) {
            if (a.priority != b.priority) {
                return a.priority > b.priority;
            }

            return a.deadline < b.deadline;
        });
    }

    static string priorityName(int priority) {
        if (priority == 3) return "VIP";
        if (priority == 2) return "Express";
        return "Normal";
    }
};

#endif
