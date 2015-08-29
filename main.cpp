/*
Peter MacLellan
Ben Soper

Lab 5b
12/8/2014

This is the main file for lab 5b.  It reads in mazes from input files and then
solves each of the three using depth-first, breadth-first, and Dijkstra 
algorithms.
*/

#include "maze.h"

using namespace std;

void solveMazeRec(ifstream&, string);
void solveMazeNonRec(ifstream&, string);
void checkIfStream(ifstream&, string);

int main()
{
    ifstream fin;

    // solve maze 1
    solveMazeNonRec(fin, "maze1.txt");

    // solve maze 2
    solveMazeNonRec(fin, "maze2.txt");

    // solve maze 3
    solveMazeNonRec(fin, "maze3.txt");

    return 0;
}

// solves a maze iteratively using three different algorithms
void solveMazeNonRec(ifstream& fin, string fileName)
{
    stack<node> s;
    stack<node> sp;
    queue<node> q;
    vector<dSet> v;

    fin.open(fileName.c_str());
    checkIfStream(fin, fileName);

    try
    {
        maze* m = NULL;
        while (fin && fin.peek() != 'Z')
        {
            m = new maze(fin);
        }

        maze m1 = *m;
        maze m2 = *m;
        maze m3 = *m;

        m1.print(0, 0, 7, 10);
        cout << m1.getGraph() << endl;

        //depth first search
        s.push(m1.getGraph().getNode(0));
        int longestPossiblePathNumber = INT_MAX;
        m1.findPathRecursive(0, 0, s, longestPossiblePathNumber, sp);
        
        //print out shortest path
        m1.printSteps(sp);
        system("Pause");

        //breadth-first search
        m2.findPathBFS(q);
        system("Pause");

        // Dijkstra
        m3.findPathDijkstra(v);
        system("Pause");

        fin.close();
        return;
    }
    catch (indexRangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}

// helper function to check if an input file can be opened
void checkIfStream(ifstream& fin, string fileName)
{
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        system("Pause");
        exit(1);
    }
}
