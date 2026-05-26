#ifndef GRAPH_ENGINE_H
#define GRAPH_ENGINE_H

#include <bits/stdc++.h>
using namespace std;

class GraphEngine {
private:
    unordered_map<int, vector<pair<int, double>>> adjList;

public:
    void addVertex(int node) {
        if (adjList.find(node) == adjList.end()) {
            adjList[node] = {};
        }
    }

    void addEdge(int source, int destination, double weight) {
        addVertex(source);
        addVertex(destination);
        adjList[source].push_back({destination, weight});
    }

    void updateTraffic(int source, int destination, double newWeight) {
        for (auto &edge : adjList[source]) {
            if (edge.first == destination) {
                edge.second = newWeight;
                return;
            }
        }
    }

    unordered_map<int, double> dijkstra(int start) {
        unordered_map<int, double> distance;

        for (auto &node : adjList) {
            distance[node.first] = DBL_MAX;
        }

        priority_queue<
            pair<double, int>,
            vector<pair<double, int>>,
            greater<pair<double, int>>
        > minHeap;

        distance[start] = 0;
        minHeap.push({0, start});

        while (!minHeap.empty()) {
            auto current = minHeap.top();
            minHeap.pop();

            double currentDistance = current.first;
            int currentNode = current.second;

            if (currentDistance > distance[currentNode]) {
                continue;
            }

            for (auto &neighbor : adjList[currentNode]) {
                int nextNode = neighbor.first;
                double weight = neighbor.second;

                if (distance[currentNode] + weight < distance[nextNode]) {
                    distance[nextNode] = distance[currentNode] + weight;
                    minHeap.push({distance[nextNode], nextNode});
                }
            }
        }

        return distance;
    }

    void displayGraph() {
        cout << "\nCity Graph using Adjacency List:\n";

        for (auto &node : adjList) {
            cout << "Node " << node.first << " -> ";

            for (auto &edge : node.second) {
                cout << "(" << edge.first << ", " << edge.second << " km) ";
            }

            cout << endl;
        }
    }
};

#endif
