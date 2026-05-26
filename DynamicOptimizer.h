#ifndef DYNAMIC_OPTIMIZER_H
#define DYNAMIC_OPTIMIZER_H

#include <bits/stdc++.h>
#include "Models.h"
using namespace std;

class DynamicOptimizer {
private:
    unordered_map<string, double> memo;
    unordered_map<int, unordered_map<int, double>> distanceMatrix;
    int depot;

public:
    DynamicOptimizer(
        unordered_map<int, unordered_map<int, double>> distanceMatrix,
        int depot
    ) {
        this->distanceMatrix = distanceMatrix;
        this->depot = depot;
    }

    double getDistance(int a, int b) {
        string key = to_string(a) + "-" + to_string(b);

        if (memo.find(key) != memo.end()) {
            return memo[key];
        }

        memo[key] = distanceMatrix[a][b];

        return memo[key];
    }

    vector<Order> bestInsertion(vector<Order> currentRoute, Order newOrder) {
        if (currentRoute.empty()) {
            currentRoute.push_back(newOrder);
            return currentRoute;
        }

        double minExtraCost = DBL_MAX;
        int bestPosition = 0;

        for (int i = 0; i <= currentRoute.size(); i++) {
            int previousNode;
            int nextNode;

            if (i == 0) {
                previousNode = depot;
            }
            else {
                previousNode = currentRoute[i - 1].locationNode;
            }

            if (i == currentRoute.size()) {
                nextNode = -1;
            }
            else {
                nextNode = currentRoute[i].locationNode;
            }

            double extraCost;

            if (nextNode == -1) {
                extraCost = getDistance(previousNode, newOrder.locationNode);
            }
            else {
                extraCost =
                    getDistance(previousNode, newOrder.locationNode) +
                    getDistance(newOrder.locationNode, nextNode) -
                    getDistance(previousNode, nextNode);
            }

            if (extraCost < minExtraCost) {
                minExtraCost = extraCost;
                bestPosition = i;
            }
        }

        currentRoute.insert(currentRoute.begin() + bestPosition, newOrder);

        return currentRoute;
    }

    void displayMemoTable() {
        cout << "\nMemoization Table:\n";

        for (auto &entry : memo) {
            cout << entry.first << " = " << entry.second << " km\n";
        }
    }
};

#endif
