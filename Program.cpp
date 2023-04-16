#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <ctime>

class Maze
{
public:
    // Constructor with width and height as arguments
    Maze(int w, int h) : width(w), height(h), cells(w* h), grid((h * 2 + 1)* (w * 2 + 1), ' ')
    {
        // Initialise cells with their parent and rank.
        for (int i = 0; i < width * height; i++) cells[i] = { i, 0 };

        // Initialise the maze grid.
        for (int y = 0; y < height * 2 + 1; y++)
            for (int x = 0; x < width * 2 + 1; ++x) grid[y * (width * 2 + 1) + x] = (x % 2 == 0 || y % 2 == 0) ? '#' : '-';
    }

    // Generate a random maze using Kruskal's Algorithm.
    void Generate()
    {
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
        std::mt19937 rng(std::time(0));
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

    // Save the generated maze to a file.
    void SaveToFile(const std::string& filename)
    {
        std::ofstream file(filename, std::ios::binary);

        // Check if the file is open.
        if (file.is_open())
        {
            // Write the maze to the file.
            for (int y = 0; y < height * 2 + 1; y++)
            {
                for (int x = 0; x < width * 2 + 1; x++) file << grid[y * (width * 2 + 1) + x];
                file << '\n';
            }

            // Close the file.
            file.close();
        }
        else std::cerr << "Unable to open file " << filename << std::endl;
    }

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
};

int main()
{
    // Create a maze with the specified dimensions
    Maze maze(2000, 2000);

    // Generate the maze
    maze.Generate();

    // Print the maze to the console
    maze.Print();

    // Save the maze to a file
    maze.SaveToFile("maze.txt");

    return 0;
}