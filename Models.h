#ifndef MODELS_H
#define MODELS_H

#include <bits/stdc++.h>
using namespace std;

struct Order {
    int id;
    string customerName;
    int locationNode;
    double weight;
    int priority;
    int deadline;

    Order() {}

    Order(
        int id,
        string customerName,
        int locationNode,
        double weight,
        int priority,
        int deadline
    ) {
        this->id = id;
        this->customerName = customerName;
        this->locationNode = locationNode;
        this->weight = weight;
        this->priority = priority;
        this->deadline = deadline;
    }
};

struct Vehicle {
    int vehicleId;
    double maxCapacity;
    double fuelRange;

    Vehicle(int vehicleId, double maxCapacity, double fuelRange) {
        this->vehicleId = vehicleId;
        this->maxCapacity = maxCapacity;
        this->fuelRange = fuelRange;
    }
};

#endif
