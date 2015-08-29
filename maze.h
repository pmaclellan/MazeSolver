/*
maze.h

Peter MacLellan
Ben Soper

Lab 5b
12/8/2014

This header file declares the maze class and all of its associated methods.
Importantly, it declares methods for finding the shortest path through a maze
using depth-first search, breadth-first search, and Dijkstra algorithms.
*/
#include <limits.h>
#include "graph.h"
#include <queue>

using namespace std;

// Dijsktra set for a given node
struct dSet {
    node thisNode;
    dSet* parent;
    int dist;
};

class maze
{
public:
    maze(ifstream &fin);
    void  print(int, int, int, int);
    void  printSteps(stack<node>);
    void  printDijkstra(vector<dSet>, string);
    bool  isLegal(int i, int j);

    void  setMap(int i, int j, int n);
    int   getMap(int i, int j) const;
    graph getGraph();
    void  mapMazeToGraph(graph &g);
    void  addEdges(int, int, graph&);

    void  findPathRecursive(int, int, stack<node>&, int&, stack<node>&);

    void  findPathBFS(queue<node>);
    void  addAllNeighbors(int, queue<node>&, vector<dSet>&);

    void  findPathDijkstra(vector<dSet>&);
    void  addAllNeighborsDijkstra(dSet*, vector<dSet>&);

private:
    int rows; // number of rows in the maze
    int cols; // number of columns in the maze

    matrix<bool> value;
    matrix<int> map;      // Mapping from maze (i,j) values to node index values
    graph g;
};

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n. 
{
}

int maze::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
    return 0;
}

graph maze::getGraph()
{
    return g;
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
    fin >> rows;
    fin >> cols;

    char x;

    value.resize(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fin >> x;
            if (x == 'O')
                value[i][j] = true;
            else
                value[i][j] = false;
        }
    }


    map.resize(rows, cols);

    g = graph(rows * cols);
    mapMazeToGraph(g);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
    cout << endl;

    if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
        throw rangeError("Bad value in maze::print");

    if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
        throw rangeError("Bad value in maze::print");

    for (int i = 0; i <= rows - 1; i++)
    {
        for (int j = 0; j <= cols - 1; j++)
        {
            if (i == goalI && j == goalJ)
                cout << "*";
            else
            if (i == currI && j == currJ)
                cout << "+";
            else
            if (value[i][j])
                cout << " ";
            else
                cout << "X";
        }
        cout << endl;
    }
    cout << endl;
}

//helper function to print the final order of steps for the shortest path
void maze::printSteps(stack<node> s)
{
    stack<node> reversed;
    int stackSize = s.size();

    for (int i = 0; i < stackSize; i++)
    {
        reversed.push(s.top());
        s.pop();
    }

    for (int i = 0; i < stackSize; i++)
    {
        print(rows - 1, cols - 1, reversed.top().getId() / cols, 
                                  reversed.top().getId() % cols);
        reversed.pop();
    }

    cout << "Total steps DFS : " << stackSize - 1 << endl;
}

//helper function to print out shortest path
void maze::printDijkstra(vector<dSet> v, string s)
{
    stack<node> dPrint;
    dPrint.push(v.at(v.size() - 1).thisNode);
    dSet* current = v.at(v.size() - 1).parent;

    while (current != NULL)
    {
        dPrint.push(current->thisNode);
        current = current->parent;
    }

    int stackSize = dPrint.size();

    for (int i = 0; i < stackSize; i++)
    {
        print(rows - 1, cols - 1, dPrint.top().getId() / cols,
            dPrint.top().getId() % cols);
        dPrint.pop();
    }

    cout << "Total steps " << s << ": " << stackSize - 1 << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
    if (i < 0 || i > rows || j < 0 || j > cols)
        throw rangeError("Bad value in maze::isLegal");

    return value[i][j];
}

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (value[i][j])
            {
                addEdges(i, j, g);
            }
        }
    }
}

//check all four directions around a given cell and add edges if there
//is an adjacent moveable space
void maze::addEdges(int i, int j, graph& g)
{
    int currentIndex = i*cols + j;

    // check down
    if (i != rows - 1 && value[i + 1][j])
    {
        g.addEdge(currentIndex, currentIndex + cols, 1);
    }

    // check right
    if (j != cols - 1 && value[i][j + 1])
    {
        g.addEdge(currentIndex, currentIndex + 1, 1);
    }

    // check left
    if (j != 0 && value[i][j - 1])
    {
        g.addEdge(currentIndex, currentIndex - 1, 1);
    }

    // check up
    if (i != 0 && value[i - 1][j])
    {
        g.addEdge(currentIndex, currentIndex - cols, 1);
    }
}

// finds a path through a maze recursively using a depth-first search
// prints out the maze and the next move after each step
void maze::findPathRecursive(int i, int j, stack<node>& s, int& shortestPath, stack<node>& sp)
{
    int index = i*cols + j;

    //s.push(g.getNode(index));
    g.visit(index);

    // if you have reached the end of the maze
    if ((index == rows*cols - 1) && (s.size() < shortestPath))
    {
        sp = s;
        shortestPath = sp.size();
    }

    if (s.size() < shortestPath)
    {
        // check down
        if (i != rows - 1 && g.isEdge(index, index + cols)
            && !g.isVisited(index + cols))
        {
            s.push(g.getNode(index + cols));
            findPathRecursive(i + 1, j, s, shortestPath, sp);
            s.pop();
        }

        // check right
        if (j != cols - 1 && g.isEdge(index, index + 1)
            && !g.isVisited(index + 1))
        {
            s.push(g.getNode(index + 1));
            findPathRecursive(i, j + 1, s, shortestPath, sp);
            s.pop();
        }

        // check left
        if (j != 0 && g.isEdge(index, index - 1)
            && !g.isVisited(index - 1))
        {
            s.push(g.getNode(index - 1));
            findPathRecursive(i, j - 1, s, shortestPath, sp);
            s.pop();
        }

        // check up
        if (i != 0 && g.isEdge(index, index - cols)
            && !g.isVisited(index - cols))
        {
            s.push(g.getNode(index - cols));
            findPathRecursive(i - 1, j, s, shortestPath, sp);
            s.pop();
        }
    }
    
    else if (!s.empty())
    {
        g.unVisit(s.top().getId());
    } 
}

//finds the shortest path through a maze using a breadth-first search
void maze::findPathBFS(queue<node> q)
{
    vector<dSet> dSets;
    for (int i = 0; i < g.numNodes(); i++)
    {
        dSet d;
        d.dist = INT_MAX;
        d.thisNode = g.getNode(i);
        d.parent = NULL;
        dSets.push_back(d);
    }
    //in an unweighted graph, the first path to reach the end is the shortest
    int index = 0;
    q.push(g.getNode(index));

    while (!q.empty())
    {
        index = q.front().getId();
        if (index == rows * cols - 1) {
            printDijkstra(dSets, "BFS");
            return;
        }

        addAllNeighbors(index, q, dSets);

        g.visit(index);
        q.pop();
        //print(rows - 1, cols - 1, index / cols, index % cols);
    }
}

//helper function to add all unvisited neighbors of a given node
void maze::addAllNeighbors(int currentIndex, queue<node>& q, vector<dSet>& dSets)
{
    int i = currentIndex / cols;
    int j = currentIndex % cols;

    // check down
    if (i != rows - 1 && g.isEdge(currentIndex, currentIndex + cols)
        && !g.isVisited(currentIndex + cols))
    {
        q.push(g.getNode(currentIndex + cols));
        dSets.at(currentIndex + cols).parent = &dSets.at(currentIndex);
    }

    // check right
    if (j != cols - 1 && g.isEdge(currentIndex, currentIndex + 1)
        && !g.isVisited(currentIndex + 1))
    {
        q.push(g.getNode(currentIndex + 1));
        dSets.at(currentIndex + 1).parent = &dSets.at(currentIndex);
    }

    // check left
    if (j != 0 && g.isEdge(currentIndex, currentIndex - 1)
        && !g.isVisited(currentIndex - 1))
    {
        q.push(g.getNode(currentIndex - 1));
        dSets.at(currentIndex - 1).parent = &dSets.at(currentIndex);
    }

    // check up
    if (i != 0 && g.isEdge(currentIndex, currentIndex - cols)
        && !g.isVisited(currentIndex - cols))
    {
        q.push(g.getNode(currentIndex - cols));
        dSets.at(currentIndex - cols).parent = &dSets.at(currentIndex);
    }
}

//finds the shortest path through the maze using Dijkstra's algorithm
void maze::findPathDijkstra(vector<dSet> &q)
{
    for (int i = 0; i < g.numNodes(); i++)
    {
        dSet d;
        d.dist = INT_MAX;
        d.thisNode = g.getNode(i);
        d.parent = NULL;
        q.push_back(d);
    }

    q.front().dist = 0;

    int prevShortDist, shortDist = 0, i;
    int index = 0;

    while (!q.empty())
    {
        prevShortDist = shortDist;
        shortDist = INT_MAX;

        for (i = 0; i < q.size(); i++)
        {
            if (!(g.getNode(q.at(i).thisNode.getId()).isVisited()) &&
                q.at(i).dist < shortDist)
            {
                shortDist = q.at(i).dist;
                index = i;
                if (shortDist == prevShortDist) break;
            }
        }

        dSet* temp = &(q.at(index));

        if (temp->thisNode.getId() == rows*cols - 1)
        {
            printDijkstra(q, "Dijkstra");
            return;
        }
        else
        {
            addAllNeighborsDijkstra(temp, q);
        }
    }// end while
}

//helper function to add eligible neighbors for a given maze node
void maze::addAllNeighborsDijkstra(dSet* current, vector<dSet>& q)
{
    int currentIndex = current->thisNode.getId();

    g.visit(currentIndex);

    int i = currentIndex / cols;
    int j = currentIndex % cols;

    dSet* neighbor;

    // check down
    if (i != rows - 1 && g.isEdge(currentIndex, currentIndex + cols)
        && !g.isVisited(currentIndex + cols))
    {
        neighbor = &(q.at(currentIndex + cols));
        if (current->dist + 1 < neighbor->dist)
        {
            neighbor->dist = current->dist + 1;
            neighbor->parent = current;
        }
    }

    // check right
    if (j != cols - 1 && g.isEdge(currentIndex, currentIndex + 1)
        && !g.isVisited(currentIndex + 1))
    {
        neighbor = &(q.at(currentIndex + 1));
        if (current->dist + 1 < neighbor->dist)
        {
            neighbor->dist = current->dist + 1;
            neighbor->parent = current;
        }
    }

    // check left
    if (j != 0 && g.isEdge(currentIndex, currentIndex - 1)
        && !g.isVisited(currentIndex - 1))
    {
        neighbor = &(q.at(currentIndex - 1));
        if (current->dist + 1 < neighbor->dist)
        {
            neighbor->dist = current->dist + 1;
            neighbor->parent = current;
        }
    }

    // check up
    if (i != 0 && g.isEdge(currentIndex, currentIndex - cols)
        && !g.isVisited(currentIndex - cols))
    {
        neighbor = &(q.at(currentIndex - cols));
        if (current->dist + 1 < neighbor->dist)
        {
            neighbor->dist = current->dist + 1;
            neighbor->parent = current;
        }
    }

}