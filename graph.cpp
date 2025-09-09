/*
* Undirektionaler Graph mit DFS und BFS:
* - Jeder Knoten ist mit Namen (Label) und Nummer versehen.
* - Der Graph speichert alle Nachbarn eines Knotens über Adjazenzlisten.
* - Es gibt rekursive und iterative Tiefensuche (DFS) sowie zwei Varianten der Breitensuche (BFS).
*
* Beispiel: VRR-Verbindungen zwischen Städten werden modelliert.
* bfs_iter2() wird aufgerufen, um die Stationen in BFS-Reihenfolge zu besuchen.
*/

#include <queue>
#include <stack>
#include <string>
#include <vector>


class GraphNode {
 public:
  GraphNode(std ::string label = "", int nr = -1)
      : label_{label}, node_nr_{nr} {}

  void add_directed_edge(GraphNode* neighbor) { adj_list_.push_back(neighbor); }

  std::string label_;
  int node_nr_;
  std::vector<GraphNode*> adj_list_;  // meine Nachbarn
};

class Graph {
  bool* visited_;  // helper array for dfs
 public:
  std::vector<GraphNode> nodes_;
  int n_ = 0;  // #Knoten
  int m_ = 0;  // #Kanten

  void add_node(std::string label) { nodes_.push_back(GraphNode(label, n_++)); }

  // füge Kante von Knoten i zu Knoten j hinzu (ungerichtet)
  void add_undirected_edge(int i, int j) {
    nodes_[i].add_directed_edge(&nodes_[j]);
    nodes_[j].add_directed_edge(&nodes_[i]);
    ++m_;
  }

  // Tiefensuche rekursiv:
  void dfs() {
    visited_ = new bool[n_]();  // schnelle Standard-Initialisierung mit 'false'
    //    for (int i = 0; i < n_; ++i) visited_[i] = false;
    dfs(&nodes_[0]);  // starte rekursiven Aufruf
    delete[] visited_;
  }

  void dfs_iter() {
    visited_ = new bool[n_]();  // schnelle Standard-Initialisierung mit 'false'

    std::stack<GraphNode*> s;
    s.push(&nodes_[0]);  // start at node 0
    while (!s.empty()) {
      GraphNode* v = s.top();
      s.pop();
      if (!visited_[v->node_nr_]) {
        visited_[v->node_nr_] = true;
        printf("besuche %s\n", v->label_.c_str());
        for (GraphNode* w : v->adj_list_) {
          s.push(w);
        }
      }
    }

    delete[] visited_;
  }

  void bfs_iter() {
    visited_ = new bool[n_]();  // schnelle Standard-Initialisierung mit 'false'

    std::queue<GraphNode*> q;
    q.push(&nodes_[0]);  // start at node 0
    while (!q.empty()) {
      GraphNode* v = q.front();
      q.pop();
      if (!visited_[v->node_nr_]) {
        visited_[v->node_nr_] = true;
        printf("besuche %s\n", v->label_.c_str());
        for (GraphNode* w : v->adj_list_) {
          q.push(w);
        }
      }
    }

    delete[] visited_;
  }

  // variant: push only nodes not already visited (Fehler aus VL korrigiert)
  void bfs_iter2() {
    visited_ = new bool[n_]();  // schnelle Standard-Initialisierung mit 'false'

    std::queue<GraphNode*> q;
    q.push(&nodes_[0]);  // start at node 0
    visited_[0] = true;  // mark as visited (now visited includes invisited
                         // nodes in queue)
    while (!q.empty()) {
      GraphNode* v = q.front();
      q.pop();
      printf("besuche %s\n", v->label_.c_str());
      for (GraphNode* w : v->adj_list_) {
        if (!visited_[w->node_nr_]) {
          visited_[w->node_nr_] = true;
          q.push(w);
        }
      }
    }

    delete[] visited_;
  }

 private:
  void dfs(GraphNode* v) {
    if (!visited_[v->node_nr_]) {
      visited_[v->node_nr_] = true;
      printf("besuche %s\n", v->label_.c_str());
      for (GraphNode* w : v->adj_list_) {
        dfs(w);
      }
    }
  }
};

int main() {
  Graph vrr;

  vrr.add_node("Wanne-Eickel");      // 0
  vrr.add_node("Herne");             // 1
  vrr.add_node("Duisburg");          // 2
  vrr.add_node("Essen");             // 3
  vrr.add_node("Bochum");            // 4
  vrr.add_node("Dortmund");          // 5
  vrr.add_node("D'dorf-Derendorf");  // 6
  vrr.add_node("Witten");            // 7
  vrr.add_node("Unna");              // 8
  vrr.add_node("Mettmann");          // 9
  vrr.add_node("Düsseldorf");        // 10
  vrr.add_node("Wuppertal");         // 11
  vrr.add_node("Hagen");             // 12

  vrr.add_undirected_edge(0, 1);  // Wanne-E. <-> Herne
  vrr.add_undirected_edge(0, 4);
  vrr.add_undirected_edge(1, 5);
  vrr.add_undirected_edge(2, 3);
  vrr.add_undirected_edge(2, 6);
  vrr.add_undirected_edge(3, 6);
  vrr.add_undirected_edge(3, 4);
  vrr.add_undirected_edge(3, 11);
  vrr.add_undirected_edge(4, 5);
  vrr.add_undirected_edge(4, 7);
  vrr.add_undirected_edge(5, 7);
  vrr.add_undirected_edge(5, 8);
  vrr.add_undirected_edge(5, 12);
  vrr.add_undirected_edge(6, 10);
  vrr.add_undirected_edge(7, 12);
  vrr.add_undirected_edge(8, 12);
  vrr.add_undirected_edge(10, 9);
  vrr.add_undirected_edge(10, 11);
  vrr.add_undirected_edge(11, 12);

  vrr.bfs_iter2();
}
