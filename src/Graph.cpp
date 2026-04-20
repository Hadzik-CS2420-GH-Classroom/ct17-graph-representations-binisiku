#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_set>

// =============================================================================
// 1. Constructor
// =============================================================================

Graph::Graph() {
    // The unordered_map adj_list_ is automatically initialized as empty.
}

// =============================================================================
// 2. add_vertex / add_edge
// =============================================================================

void Graph::add_vertex(const std::string& vertex) {
    // Idempotent insertion: only add if the key doesn't exist
    if (adj_list_.find(vertex) == adj_list_.end()) {
        adj_list_[vertex] = {};
    }
}

void Graph::add_edge(const std::string& from, const std::string& to) {
    // Ensure both vertices exist first
    add_vertex(from);
    add_vertex(to);

    // Undirected graph: add to both adjacency lists
    adj_list_[from].push_back(to);
    adj_list_[to].push_back(from);
}

// =============================================================================
// 3. Queries
// =============================================================================

bool Graph::has_vertex(const std::string& vertex) const {
    return adj_list_.find(vertex) != adj_list_.end();
}

bool Graph::has_edge(const std::string& from, const std::string& to) const {
    auto it = adj_list_.find(from);
    if (it == adj_list_.end()) return false;

    // Linear search through the neighbor vector
    const auto& neighbors_list = it->second;
    return std::find(neighbors_list.begin(), neighbors_list.end(), to) != neighbors_list.end();
}

int Graph::vertex_count() const {
    return static_cast<int>(adj_list_.size());
}

int Graph::edge_count() const {
    int total = 0;
    // Sum up the size of every neighbor list
    for (const auto& [vertex, neighbors_list] : adj_list_) {
        total += static_cast<int>(neighbors_list.size());
    }
    // Divide by 2 because each undirected edge is stored twice
    return total / 2;
}

std::vector<std::string> Graph::neighbors(const std::string& vertex) const {
    auto it = adj_list_.find(vertex);
    if (it != adj_list_.end()) {
        return it->second;
    }
    return {}; // Return empty vector if vertex not found
}

// =============================================================================
// 4. BFS — Breadth-First Search (Queue: FIFO)
// =============================================================================

std::vector<std::string> Graph::bfs(const std::string& start) const {
    if (!has_vertex(start)) return {};

    std::vector<std::string> result;
    std::queue<std::string> q;
    std::unordered_set<std::string> visited;

    // Start: enqueue and mark visited immediately
    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        result.push_back(current);

        // Process all neighbors
        for (const std::string& neighbor : adj_list_.at(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return result;
}

// =============================================================================
// 5. DFS — Depth-First Search (Stack: LIFO)
// =============================================================================

std::vector<std::string> Graph::dfs(const std::string& start) const {
    if (!has_vertex(start)) return {};

    std::vector<std::string> result;
    std::stack<std::string> s;
    std::unordered_set<std::string> visited;

    s.push(start);

    while (!s.empty()) {
        std::string current = s.top();
        s.pop();

        // Check visited at pop time for DFS
        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            result.push_back(current);

            // Push neighbors in reverse order so they are processed in the 
            // order they appear in the adjacency list (LIFO behavior)
            const auto& current_neighbors = adj_list_.at(current);
            for (auto it = current_neighbors.rbegin(); it != current_neighbors.rend(); ++it) {
                if (visited.find(*it) == visited.end()) {
                    s.push(*it);
                }
            }
        }
    }
    return result;
}

// =============================================================================
// 6. print — provided by instructor
// =============================================================================

void Graph::print() const {
    for (const auto& [vertex, neighbors] : adj_list_) {
        std::cout << vertex << ": ";
        for (size_t i = 0; i < neighbors.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << neighbors[i];
        }
        std::cout << std::endl;
    }
}