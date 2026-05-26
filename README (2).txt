SMART LOGISTICS ROUTE OPTIMIZATION SYSTEM
DAA PBL - C++ Integrated Version

MODULES INCLUDED:
1. GraphEngine.h
   - Weighted Directed Graph
   - Adjacency List
   - Dijkstra Algorithm with Priority Queue
   - Dynamic Traffic Update

2. GeneticAlgorithm.h
   - Population Generation
   - Fitness Function
   - Tournament Selection
   - Ordered Crossover
   - Mutation
   - Priority-safe Route Generation

3. DynamicOptimizer.h
   - Local Optimal Insertion
   - Memoization Table
   - Mid-Day New Order Handling

4. ConstraintValidator.h
   - Fuel Check
   - Vehicle Capacity Check
   - Time Window Check
   - VIP / Express / Normal Priority Ranking

5. main.cpp
   - Fully integrated working demo

HOW TO RUN IN VS CODE:

METHOD 1:
Open terminal in this folder and run:

g++ main.cpp -o logistics
./logistics

METHOD 2 FOR WINDOWS:
g++ main.cpp -o logistics.exe
logistics.exe

IMPORTANT:
Install MinGW C++ compiler if g++ is not recognized.

VIVA POINT:
This project integrates Dijkstra, Genetic Algorithm, Dynamic Programming and Heuristic Constraint Validation in one delivery route optimization system.
