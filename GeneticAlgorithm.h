#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <bits/stdc++.h>
#include "Models.h"
using namespace std;

class GeneticAlgorithm {
private:
    vector<Order> orders;
    unordered_map<int, unordered_map<int, double>> distanceMatrix;
    int depot;
    int populationSize;
    int generations;

public:
    GeneticAlgorithm(
        vector<Order> orders,
        unordered_map<int, unordered_map<int, double>> distanceMatrix,
        int depot,
        int populationSize = 40,
        int generations = 80
    ) {
        this->orders = orders;
        this->distanceMatrix = distanceMatrix;
        this->depot = depot;
        this->populationSize = populationSize;
        this->generations = generations;
    }

    vector<Order> makePrioritySafe(vector<Order> route) {
        vector<Order> vip, express, normal;

        for (auto &order : route) {
            if (order.priority == 3) {
                vip.push_back(order);
            }
            else if (order.priority == 2) {
                express.push_back(order);
            }
            else {
                normal.push_back(order);
            }
        }

        vector<Order> fixedRoute;

        fixedRoute.insert(fixedRoute.end(), vip.begin(), vip.end());
        fixedRoute.insert(fixedRoute.end(), express.begin(), express.end());
        fixedRoute.insert(fixedRoute.end(), normal.begin(), normal.end());

        return fixedRoute;
    }

    vector<vector<Order>> generatePopulation() {
        vector<vector<Order>> population;

        random_device rd;
        mt19937 g(rd());

        for (int i = 0; i < populationSize; i++) {
            vector<Order> chromosome = orders;

            shuffle(chromosome.begin(), chromosome.end(), g);

            chromosome = makePrioritySafe(chromosome);

            population.push_back(chromosome);
        }

        return population;
    }

    double routeDistance(vector<Order> &route) {
        if (route.empty()) return 0;

        double total = 0;
        int current = depot;

        for (auto &order : route) {
            total += distanceMatrix[current][order.locationNode];
            current = order.locationNode;
        }

        return total;
    }

    double fitness(vector<Order> route) {
        route = makePrioritySafe(route);

        double distanceCost = routeDistance(route);
        double priorityPenalty = 0;
        double timePenalty = 0;

        int currentTime = 9 * 60;

        for (int i = 0; i < route.size(); i++) {
            currentTime += 20;

            if (currentTime > route[i].deadline) {
                timePenalty += (currentTime - route[i].deadline) * 2;
            }

            if (route[i].priority == 3) {
                priorityPenalty += i * 20;
            }
            else if (route[i].priority == 2) {
                priorityPenalty += i * 8;
            }
        }

        return 1.0 / (distanceCost + priorityPenalty + timePenalty + 1);
    }

    vector<Order> tournamentSelection(vector<vector<Order>> &population) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, population.size() - 1);

        vector<Order> best = population[dis(gen)];

        for (int i = 0; i < 5; i++) {
            vector<Order> candidate = population[dis(gen)];

            if (fitness(candidate) > fitness(best)) {
                best = candidate;
            }
        }

        return best;
    }

    vector<Order> orderedCrossover(vector<Order> parent1, vector<Order> parent2) {
        int n = parent1.size();

        if (n <= 1) return parent1;

        vector<Order> child(n);
        vector<bool> filled(n, false);

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, n - 1);

        int start = dis(gen);
        int end = dis(gen);

        if (start > end) swap(start, end);

        unordered_set<int> usedIds;

        for (int i = start; i <= end; i++) {
            child[i] = parent1[i];
            filled[i] = true;
            usedIds.insert(parent1[i].id);
        }

        int p2Index = 0;

        for (int i = 0; i < n; i++) {
            if (!filled[i]) {
                while (p2Index < n && usedIds.count(parent2[p2Index].id)) {
                    p2Index++;
                }

                if (p2Index < n) {
                    child[i] = parent2[p2Index];
                    usedIds.insert(parent2[p2Index].id);
                }
            }
        }

        return makePrioritySafe(child);
    }

    void mutate(vector<Order> &route) {
        if (route.size() < 2) return;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, route.size() - 1);

        int a = dis(gen);
        int b = dis(gen);

        swap(route[a], route[b]);

        route = makePrioritySafe(route);
    }

    vector<Order> optimize() {
        vector<vector<Order>> population = generatePopulation();

        for (int gen = 0; gen < generations; gen++) {
            vector<vector<Order>> newPopulation;

            while (newPopulation.size() < populationSize) {
                vector<Order> parent1 = tournamentSelection(population);
                vector<Order> parent2 = tournamentSelection(population);

                vector<Order> child = orderedCrossover(parent1, parent2);

                if (rand() % 100 < 20) {
                    mutate(child);
                }

                newPopulation.push_back(child);
            }

            population = newPopulation;
        }

        vector<Order> best = population[0];

        for (auto &route : population) {
            if (fitness(route) > fitness(best)) {
                best = route;
            }
        }

        return makePrioritySafe(best);
    }
};

#endif
