# DTU AI-MAS Final Project

This is the final project for the DTU course Artificial Intelligence and Multi-Agent Systems (02285).

This project implements different search-based problem solving algorithms, such as BFS, DFS, Greedy BFS and A-Star, with different heuristic techniches. The project attachs itself to the simulation system server provided for the course, to solve the requested level.

#### Usage
Compile the project:
```
make -j8
```

Run the provided simulation server and attach the Project:
```
make -j8 run
```

Run a given level:
```
make run -j8 LEVEL=complevels/YungCode.lvl:
```
