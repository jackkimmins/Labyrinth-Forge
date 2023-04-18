# Random Maze Generation Employing an Adapted Kruskal's Algorithm in C++
This project presents a C++ implementation of a random maze generator utilising a modified version of Kruskal's Algorithm. The program can create a maze with user-defined dimensions and provides the option to either display the generated maze in the console or save it to a file.

[Kruskal's Algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm) is prevalently employed to ascertain the [minimum spanning tree (MST)](https://en.wikipedia.org/wiki/Minimum_spanning_tree) in a connected, undirected graph. It operates by continuously selecting the smallest edge connecting two disjoint vertex sets and incorporating it into the MST. The algorithm continues until all vertices are connected or all edges have been processed.

In the context of maze generation, each cell constitutes a vertex, while the walls separating the cells represent edges. Initially, every cell is an independent set. The adapted Kruskal's Algorithm for maze construction encompasses the subsequent stages:

     - create a forest F (a set of trees), where each vertex in the graph is a separate tree
     - create a sorted set S containing all the edges in the graph
     - while S is nonempty and F is not yet spanning
    	 - remove an edge with minimum weight from S
    	 - if the removed edge connects two different trees then add it to the forest F, combining two trees into a single tree

My modified Kruskal's Algorithm produces a stochastic maze with a singular solution, as it cultivates a spanning tree of the cells, ensuring the absence of loops within the maze. The randomised sequence in which walls undergo processing contributes to the maze's arbitrary appearance.

The code has been optimised for performance, permitting the expeditious generation of sizable mazes (2000+) within a matter of seconds.

## Compilation
To compile the project, use the following command:

    g++ -O3 -flto Program.cpp -o Program && ./Program
