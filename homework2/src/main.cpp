#include <iostream>
#include "AdjListArrayGraph.hpp"
#include "AdjMatrixGraph.hpp"
#include "Kruskal.hpp"
#include "Prim.hpp"
#include "ActivityNetwork.hpp"

using namespace std;

void testBasicGraph() {
    cout << "===DFS, BFS, Components, DFNLow, Biconnected==" << endl;
    AdjListArrayGraph g(8, false, false);
    g.InsertEdge(0, 1);
    g.InsertEdge(1, 2);
    g.InsertEdge(2, 0);
    g.InsertEdge(3, 4);
    g.InsertEdge(4, 5);
    g.InsertEdge(5, 3);
    g.InsertEdge(6, 7);
    
    cout << "Graph structure (AdjListArrayGraph):" << endl;
    g.Print();
    
    cout << "\nComponents:" << endl;
    g.Components();
    
    cout << "\nDFS:" << endl;
    g.DFS();
    
    cout << "\nBFS from 0:" << endl;
    g.BFS(0);
    
    cout << "\nDFNLow from 0:" << endl;
    g.DFNLow(0);
    
    cout << "\nBiconnected components:" << endl;
    g.Biconnected();
    cout << "=========================================================\n" << endl;
}

void testMST() {
    cout << "===Kruskal and Prim===" << endl;
    AdjListArrayGraph g(6, false, true); // Undirected, weighted
    g.InsertEdge(0, 1, 5);
    g.InsertEdge(0, 2, 1);
    g.InsertEdge(1, 2, 3);
    g.InsertEdge(1, 3, 4);
    g.InsertEdge(2, 4, 6);
    g.InsertEdge(3, 4, 2);
    g.InsertEdge(3, 5, 8);
    g.InsertEdge(4, 5, 7);
    
    auto [kruskalEdges, kruskalWeight] = Kruskal::findMST(g);
    cout << "Kruskal's MST Weight: " << kruskalWeight << "\nEdges: ";
    for (const auto& [u, v, w] : kruskalEdges) {
        cout << "(" << u << "-" << v << ":" << w << ") ";
    }
    cout << endl;
    
    auto [primEdges, primWeight] = Prim::findMST(g, 0);
    cout << "Prim's MST Weight: " << primWeight << "\nEdges: ";
    for (const auto& [u, v, w] : primEdges) {
        cout << "(" << u << "-" << v << ":" << w << ") ";
    }
    cout << endl;
    cout << "=========================================================\n" << endl;
}

void testActivityNetwork() {
    cout << "===Activity Network (Topological Sort and AOE) ===" << endl;
    AdjListArrayGraph g(6, true, true); // Directed, weighted
    g.InsertEdge(0, 1, 2);
    g.InsertEdge(0, 2, 3);
    g.InsertEdge(1, 3, 4);
    g.InsertEdge(2, 3, 1);
    g.InsertEdge(2, 4, 2);
    g.InsertEdge(3, 5, 3);
    g.InsertEdge(4, 5, 1);
    
    auto topo = ActivityNetwork::TopologicalOrder(g);
    cout << "Topological Sort: ";
    for (int v : topo) {
        cout << v << " ";
    }
    cout << endl;
    
    auto [activities, success] = ActivityNetwork::AOENetwork(g);
    if (success) {
        cout << "AOE Network Activities:" << endl;
        for (const auto& a : activities) {
            cout << "Edge " << a.u << "->" << a.v 
                 << " Weight=" << a.weight 
                 << " Early=" << a.earlyTime 
                 << " Late=" << a.lateTime 
                 << " Slack=" << a.slack 
                 << " Critical=" << (a.isCritical ? "Yes" : "No") << endl;
        }
    } else {
        cout << "contains" << endl;
    }
    cout << "=========================================================\n" << endl;
}

int main() {
    testBasicGraph();
    testMST();
    testActivityNetwork();
    return 0;
}
