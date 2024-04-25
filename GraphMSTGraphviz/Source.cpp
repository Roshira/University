#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Subset {
    int parent;
    int rank;
};

struct Edge {
    int src, dest;
    double weight;
};

class Graph {
public:
    vector<Edge> edges;
    int V; 

    Graph(int vertices) : V(vertices) {}

    void addEdge(int src, int dest, double weight) {
        Edge edge;
        edge.src = src;
        edge.dest = dest;
        edge.weight = weight;
        edges.push_back(edge);
    }

    int find(vector<Subset>& subsets, int i) {
        if (subsets[i].parent != i) {
            subsets[i].parent = find(subsets, subsets[i].parent);
        }
        return subsets[i].parent;
    }

    void Union(vector<Subset>& subsets, int x, int y) {
        int xroot = find(subsets, x);
        int yroot = find(subsets, y);

        if (subsets[xroot].rank < subsets[yroot].rank) {
            subsets[xroot].parent = yroot;
        }
        else if (subsets[xroot].rank > subsets[yroot].rank) {
            subsets[yroot].parent = xroot;
        }
        else {
            subsets[yroot].parent = xroot;
            subsets[xroot].rank++;
        }
    }

    void KruskalMST() {
        vector<Edge> result(V - 1);
        int e = 0; 

        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
            });

        vector<Subset> subsets(V);
        for (int v = 0; v < V; ++v) {
            subsets[v].parent = v;
            subsets[v].rank = 0;
        }

        int i = 0;

        while (e < V - 1 && i < edges.size()) {
            Edge next_edge = edges[i++];
            int x = find(subsets, next_edge.src);
            int y = find(subsets, next_edge.dest);

            if (x != y) {
                result[e++] = next_edge;
                Union(subsets, x, y);
            }
        }

        cout << "Edges in the Minimum Spanning Tree:" << endl;
        for (i = 0; i < e; ++i) {
            cout << result[i].src << " -- " << result[i].dest << " : " << result[i].weight << endl;
        }
        cout << "Enter type of sorting(1 - Bubble, 2 - Shell):" << endl;
        int type;
        cin >> type;
        switch (type)
        {
        case 1:
            cout << "BubbleSorted:" << endl;
            bubbleSortEdges(result);
            for (i = 0; i < e; ++i) {
                cout << result[i].src << " -- " << result[i].dest << " : " << result[i].weight << endl;}
            break;
        case 2:
            cout << "ShellSorted:" << endl;
            shellSortEdges(result);
            for (i = 0; i < e; ++i) {
                cout << result[i].src << " -- " << result[i].dest << " : " << result[i].weight << endl;}
            break;
        default:
            cerr << "Error not true task";
            break;
        }
    }
    int getMidpoint(int a, int b) {
        return (a + b) / 2;
    }
    void bubbleSortEdges(vector<Edge>& result) {
        vector<int> BubbleSort;

        for (int i = 0; i < result.size(); ++i) {
            int midpoint = getMidpoint(result[i].dest, result[i].src);
            BubbleSort.push_back(midpoint);
        }

        int n = result.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (BubbleSort[j] > BubbleSort[j + 1]) {
                    swap(BubbleSort[j], BubbleSort[j + 1]);
                    swap(result[j], result[j + 1]);
                }
            }
        }
    }

    void shellSortEdges(vector<Edge>& result) {
        vector<int> BubbleSort;
        for (int i = 0; i < result.size(); ++i) {
            int midpoint = getMidpoint(result[i].dest, result[i].src);
            BubbleSort.push_back(midpoint);
        }

        int n = BubbleSort.size();
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; ++i) {
                int temp = BubbleSort[i];
                Edge tempEdge = result[i];
                int j;
                for (j = i; j >= gap && BubbleSort[j - gap] > temp; j -= gap) {
                    BubbleSort[j] = BubbleSort[j - gap];
                    result[j] = result[j - gap];
                }
                BubbleSort[j] = temp;
                result[j] = tempEdge;
            }
        }
    }
    void generateDotFile(const string& filename) {
        ofstream dotFile(filename);
        dotFile << "graph G {\n";
        for (Edge edge : edges) {
            dotFile << edge.src << " -- " << edge.dest << " [label=\"" << edge.weight << "\"];\n";
        }
        dotFile << "}\n";
        dotFile.close();
    }
};

void generateGraphFromDotFile(const std::string& dotFileName, const std::string& outputFormat = "png") {
    std::string command = "dot -T" + outputFormat + " " + dotFileName + " -o output." + outputFormat;

    int returnValue = std::system(command.c_str());
    if (returnValue != 0) {
        std::cerr << "Error." << std::endl;
    }
    else {
        std::cout << "The graph was successfully generated in the format " << outputFormat << "!" << std::endl;
    }
}

double calculateMidpoint(Edge edge) {
    return (edge.src + edge.dest) / 2.0;
}

int main() {
    ifstream inputFile("test_graph.txt");
    if (!inputFile) {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    int numVertices, numEdges;
    inputFile >> numVertices >> numEdges;

    Graph graph(numVertices);

    for (int i = 0; i < numEdges; ++i) {
        int src, dest;
        double weight;
        inputFile >> src >> dest >> weight;
        graph.addEdge(src, dest, weight);
    }
    
    inputFile.close();
    graph.KruskalMST();

    graph.generateDotFile("graph.dot");
    std::string dotFileName = "graph.dot";
    generateGraphFromDotFile(dotFileName);

    return 0;
}