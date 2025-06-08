#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>
#include <ctime>
#include "Node.h"

using namespace std;

vector<vector<Node>> GenerateGrid(int width, int height, bool withObstacles, float obstacleDensity) {
    vector<vector<Node>> grid(width, vector<Node>(height, Node(0, 0, true)));
    srand(static_cast<unsigned>(time(nullptr)));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            bool walkable = true;
            if (withObstacles) {
                float r = static_cast<float>(rand()) / RAND_MAX;  // between 0.0 and 1.0
                walkable = r > obstacleDensity;
            }
            grid[x][y] = Node(x, y, walkable);
        }
    }

    return grid;
}


vector<Node*> GetNeighbors(Node& node, vector<vector<Node>>& grid)
{
    vector<Node*> neighbors;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }
            int checkX = node.X + dx;
            int checkY = node.Y + dy;
            if (checkX >= 0 && checkX < grid.size() && checkY >= 0 && checkY < grid[0].size())
            {
                neighbors.push_back(&grid[checkX][checkY]);
            }
        }
    }
    return neighbors;
}

int GetDistance(Node* a, Node* b)
{
    int dx = abs(a->X - b->X);
    int dy = abs(a->Y - b->Y);
    return (dx > dy) ? 14 * dy + 10 * (dx - dy) : 14 * dx + 10 * (dy - dx);
}

vector<Node*> RetracePath(Node* start, Node* end)
{
    vector<Node*> path;
    Node* current = end;

    while (current != start)
    {
        path.push_back(current);
        current = current->Parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<pair<int,int>> AStar(vector<vector<Node>>& grid, Node* start, Node* goal)
{
    vector<Node*> openSet;
    vector<Node*> closedSet;
    openSet.push_back(start);

    while (!openSet.empty())
    {
        Node* current = openSet[0];
        for (int i = 1; i < openSet.size(); i++)
        {
            if (openSet[i]->FCost() < current->FCost() ||
            (openSet[i]->FCost() == current->FCost() && openSet[i]->HCost < current->HCost))
            {
                current = openSet[i];
            }
        }

        openSet.erase(remove(openSet.begin(), openSet.end(), current), openSet.end());
        closedSet.push_back(current);

        if (current == goal)
        {
            vector<Node*> path = RetracePath(start, goal);
            vector<pair<int,int>> result = {{start->X, start->Y}};
            for (Node* n : path)
            {
                result.emplace_back(n->X, n->Y);
            }
            return result;
        }

        for (Node* neighbour : GetNeighbors(*current, grid))
        {
            if (!neighbour->Walkable || find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end()) {
                continue;
            }
            int newCost = current->GCost + GetDistance(current, neighbour);
            if (newCost < neighbour->GCost || find(openSet.begin(), openSet.end(), neighbour) == openSet.end()) {
                neighbour->GCost = newCost;
                neighbour->HCost = GetDistance(neighbour, goal);
                neighbour->Parent = current;

                if (find(openSet.begin(), openSet.end(), neighbour) == openSet.end()) {
                    openSet.push_back(neighbour);
                }
            }
        }
    }
    return {}; // no path
}


void AnimatePath(const vector<vector<Node>>& grid, const vector<pair<int, int>>& path) {
    int height = grid[0].size();

    for (size_t i = 0; i < path.size(); ++i) {
        // Move cursor up to overwrite the grid output
        if (i > 0) {
            cout << "\033[" << height << "A";
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < grid.size(); ++x) {
                if (make_pair(x, y) == path[i])
                    cout << "🚶";
                else if (find(path.begin(), path.end(), make_pair(x, y)) != path.end())
                    cout << "🌟";
                else if (!grid[x][y].Walkable)
                    cout << "🟥";
                else
                    cout << "🟩";
            }
            cout << "\n";
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}


int main()
{
    int width = 40, height = 20;

    char obstacleChoice;
    float density = 0.2f;

    cout << "Do you want obstacles in the grid? (y/n): ";
    cin >> obstacleChoice;
    bool withObstacles = (obstacleChoice == 'y' || obstacleChoice == 'Y');

    if (withObstacles) {
        cout << "Enter obstacle density (0.0 = none, 1.0 = all blocked), e.g. 0.2 for 20%: ";
        cin >> density;
    }

    auto grid = GenerateGrid(width, height, withObstacles, density);

    cout << "🟥 Non-walkable Nodes:\n";
    for (int y = 0; y < grid[0].size(); ++y)
    {
        for (int x = 0; x < grid.size(); ++x)
        {
            if (grid[x][y].Walkable)
            {
                std::cout << "🟩";
            }
            else
            {
                cout << "🟥";
            }
        }
        cout << "\n";
    }
    cout << "\n\n";

    int sx, sy, gx, gy;

    cout << "Rows value between 1 and 20\n";
    cout << "Columns value between 1 and 40\n";
    cout << "\n";


    cout << "Enter start coordinates (rows, columns): ";
    cin >> sx >> sy;
    cout << "Enter goal coordinates (rows, columns): ";
    cin >> gx >> gy;

    // to make the cordinate bewtween 1 and 20 or 1 and 40 for the users at 0 - 19 and 0 - 39
    sx--;
    sy--;
    gx--;
    gy--;


    if (sx < 0 || sx >= height || sy < 0 || sy >= width ||
        gx < 0 || gx >= height || gy < 0 || gy >= width) {
        cerr << "Coordinates out of bounds!\n";
        return 1;
        }

    Node* start = &grid[sy][sx];
    Node* goal = &grid[gy][gx];

    if (!start->Walkable || !goal->Walkable) {
        cerr << "Start or goal is not walkable.\n";
        return 1;
    }

    auto path = AStar(grid, start, goal);

    if (path.empty()) {
        std::cout << "❌ No path found.\n";
    } else {
        std::cout << "\n🌟 Animating path:\n";
        AnimatePath(grid, path);
    }

    return 0;
}
