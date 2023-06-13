#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <ctime>

#include <emscripten/bind.h>

class Maze
{
private:
    // Structure to represent a maze cell with its parent and rank.
    struct Cell { int parent, rank; };

    // Structure to represent a wall between two cells.
    struct Wall { int cell1, cell2; };

    // Find the root of a cell using path compression.
    const int Find(int x)
    {
        int root = x;
        while (root != cells[root].parent) root = cells[root].parent;

        // Path compression
        while (x != root)
        {
            int parent = cells[x].parent;
            cells[x].parent = root;
            x = parent;
        }

        return root;
    }

    // Union two sets of cells using the rank-based union optimisation.
    void UnionSets(int x, int y)
    {
        if (cells[x].rank < cells[y].rank) std::swap(x, y);
        cells[y].parent = x;
        if (cells[x].rank == cells[y].rank) cells[x].rank++;
    }

    // Maze dimensions and data structures to store cells and the grid.
    int width, height;
    std::vector<Cell> cells;
    std::vector<char> grid;

public:
    // Empty constructor
    Maze() = default;

    // Generate a random maze using Kruskal's Algorithm.
    void Generate(int w, int h, int seed = -1)
    {
        // If the seed is -1, use the current time as the seed.
        if (seed == -1) seed = std::time(0);

        // Set width and height
        width = w;
        height = h;

        // Resize and initialize cells
        cells.resize(w * h);
        for (int i = 0; i < width * height; i++) cells[i] = { i, 0 };

        // Resize and initialize the maze grid.
        grid.resize((h * 2 + 1) * (w * 2 + 1), ' ');
        for (int y = 0; y < height * 2 + 1; y++)
            for (int x = 0; x < width * 2 + 1; ++x) grid[y * (width * 2 + 1) + x] = (x % 2 == 0 || y % 2 == 0) ? '#' : ' ';

        // Vector to store the walls between the cells.
        std::vector<Wall> walls;
        walls.reserve((width - 1) * height + (height - 1) * width);

        // Add walls between adjacent cells.
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
            {
                int index = y * width + x;
                if (x < width - 1) walls.push_back({ index, index + 1 });
                if (y < height - 1) walls.push_back({ index, index + width });
            }

        // Shuffle the walls randomly.
        std::mt19937 rng(seed);
        std::shuffle(walls.begin(), walls.end(), rng);

        // Iterate through the shuffled walls.
        for (const auto& wall : walls)
        {
            // Find the roots of the cells on either side of the wall.
            int root1 = this->Find(wall.cell1), root2 = this->Find(wall.cell2);

            // If the roots are different, remove the wall and union the sets.
            if (root1 != root2)
            {
                this->UnionSets(root1, root2);
                int x1 = wall.cell1 % width, y1 = wall.cell1 / width, x2 = wall.cell2 % width, y2 = wall.cell2 / width;
                grid[y1 * 2 + 1 + (width * 2 + 1) * (y1 * 2 + 1)] = grid[y2 * 2 + 1 + (width * 2 + 1) * (y2 * 2 + 1)] = grid[(y1 + y2) + 1 + (width * 2 + 1) * ((x1 + x2) + 1)] = '-';
            }
        }

        // Mark the start and end points in the maze. This is always the top left and bottom right corners.
        grid[(width * 2 + 1) + 1] = 'S';
        grid[(height * 2 - 1) * (width * 2 + 1) + width * 2] = 'E';
    }

    // Print the generated maze to the console.
    void Print()
    {
        for (int y = 0; y < height * 2 + 1; y++)
        {
            for (int x = 0; x < width * 2 + 1; x++) std::cout << grid[y * (width * 2 + 1) + x];
            std::cout << '\n';
        }
    }

    // Convert the generated maze to a string.
    std::string MazeToString()
    {
        std::string mazeStr;
        for (int y = 0; y < height * 2 + 1; y++)
        {
            for (int x = 0; x < width * 2 + 1; x++) 
            {
                mazeStr += grid[y * (width * 2 + 1) + x];
            }
            mazeStr += '\n';
        }
        return mazeStr;
    }

    // Save the generated maze to a file.
    void SaveToFile(const std::string& filename, const std::string& content)
    {
        std::ofstream file(filename, std::ios::binary);

        // Check if the file is open.
        if (file.is_open())
        {
            // Write the maze to the file.
            file << content;

            // Close the file.
            file.close();
        }
        else std::cerr << "Unable to open file " << filename << std::endl;
    }
};

// Binding to WASM
EMSCRIPTEN_BINDINGS(maze_class) {
    emscripten::class_<Maze>("Maze")
    .constructor()
    .function("Generate", &Maze::Generate)
    .function("Print", &Maze::Print)
    .function("MazeToString", &Maze::MazeToString)
    ;
}

void LocalDemo()
{
    // Create a maze with the specified dimensions
    Maze maze;

    // Generate the maze
    maze.Generate(15, 15);

    // Print the maze to the console
    maze.Print();

    // Save the maze to a file
    maze.SaveToFile("maze.txt", maze.MazeToString());
}

int main()
{
    // LocalDemo();
    return 0;
}