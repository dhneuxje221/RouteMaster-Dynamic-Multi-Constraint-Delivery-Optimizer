#include <bits/stdc++.h>
#include "Models.h"
#include "GraphEngine.h"
#include "ConstraintValidator.h"
#include "GeneticAlgorithm.h"
#include "DynamicOptimizer.h"

using namespace std;

double calculateRouteDistance(
    vector<Order> &route,
    unordered_map<int, unordered_map<int, double>> &distanceMatrix,
    int depot
) {
    double total = 0;
    int current = depot;

    for (auto &order : route) {
        total += distanceMatrix[current][order.locationNode];
        current = order.locationNode;
    }

    return total;
}

void displayRoute(vector<Order> &route) {
    cout << "\nOptimized Delivery Route:\n";

    for (int i = 0; i < route.size(); i++) {
        cout << i + 1 << ". "
             << route[i].customerName
             << " | Node: " << route[i].locationNode
             << " | Priority: "
             << ConstraintValidator::priorityName(route[i].priority)
             << " | Weight: " << route[i].weight << " kg"
             << endl;
    }
}

int main() {
    srand(time(0));

    cout << "=============================================\n";
    cout << " SMART LOGISTICS ROUTE OPTIMIZATION SYSTEM\n";
    cout << " DAA PBL - C++ Integrated Version\n";
    cout << "=============================================\n";

    int depot = 0;

    GraphEngine cityGraph;

    cityGraph.addEdge(0, 1, 10);
    cityGraph.addEdge(0, 2, 15);
    cityGraph.addEdge(1, 3, 12);
    cityGraph.addEdge(1, 4, 15);
    cityGraph.addEdge(2, 4, 10);
    cityGraph.addEdge(3, 5, 8);
    cityGraph.addEdge(4, 5, 6);
    cityGraph.addEdge(5, 6, 7);
    cityGraph.addEdge(2, 6, 20);

    cityGraph.displayGraph();

    cout << "\nLive Traffic Update: Road 2 -> 6 changed from 20 km to 12 km.\n";
    cityGraph.updateTraffic(2, 6, 12);

    vector<int> allNodes = {0, 1, 2, 3, 4, 5, 6};

    unordered_map<int, unordered_map<int, double>> distanceMatrix;

    for (int node : allNodes) {
        auto distance = cityGraph.dijkstra(node);

        for (int otherNode : allNodes) {
            distanceMatrix[node][otherNode] = distance[otherNode];
        }
    }

    vector<Order> orders = {
        Order(1, "VIP Hospital Package", 6, 12, 3, 11 * 60 + 30),
        Order(2, "Express College Parcel", 3, 8, 2, 13 * 60),
        Order(3, "Mall Delivery", 4, 15, 2, 15 * 60),
        Order(4, "Normal Grocery", 1, 10, 1, 18 * 60),
        Order(5, "Office Documents", 5, 5, 3, 12 * 60)
    };

    Vehicle vehicle(101, 60, 100);

    cout << "\nPending Orders:\n";

    for (auto &order : orders) {
        cout << order.customerName
             << " | Node: " << order.locationNode
             << " | Priority: "
             << ConstraintValidator::priorityName(order.priority)
             << endl;
    }

    ConstraintValidator::prioritizeOrders(orders);

    cout << "\nAfter Constraint Validator Priority Ranking:\n";

    for (auto &order : orders) {
        cout << order.customerName
             << " -> "
             << ConstraintValidator::priorityName(order.priority)
             << endl;
    }

    GeneticAlgorithm ga(orders, distanceMatrix, depot);

    vector<Order> optimizedRoute = ga.optimize();

    displayRoute(optimizedRoute);

    double totalDistance = calculateRouteDistance(
        optimizedRoute,
        distanceMatrix,
        depot
    );

    cout << "\nTotal Route Distance: " << totalDistance << " km\n";

    bool capacityStatus = ConstraintValidator::checkCapacity(
        optimizedRoute,
        vehicle
    );

    bool fuelStatus = ConstraintValidator::checkFuel(
        totalDistance,
        vehicle
    );

    cout << "\nConstraint Validation:\n";
    cout << "Capacity Status: "
         << (capacityStatus ? "VALID" : "REJECTED")
         << endl;

    cout << "Fuel Status: "
         << (fuelStatus ? "VALID" : "REJECTED")
         << endl;

    cout << "\nMid-Day Change: New urgent customer order arrived.\n";

    Order newOrder(
        6,
        "New Urgent Medicine Order",
        2,
        6,
        3,
        10 * 60 + 45
    );

    DynamicOptimizer dpOptimizer(distanceMatrix, depot);

    vector<Order> updatedRoute =
        dpOptimizer.bestInsertion(optimizedRoute, newOrder);

    ConstraintValidator::prioritizeOrders(updatedRoute);

    cout << "\nUpdated Route after DP Local Optimal Insertion:\n";

    displayRoute(updatedRoute);

    dpOptimizer.displayMemoTable();

    cout << "\n=============================================\n";
    cout << " Final Flow:\n";
    cout << " 1. Dijkstra calculated shortest paths.\n";
    cout << " 2. Genetic Algorithm selected best sequence.\n";
    cout << " 3. Constraint Validator checked rules.\n";
    cout << " 4. DP inserted new order efficiently.\n";
    cout << "=============================================\n";

    return 0;
}
