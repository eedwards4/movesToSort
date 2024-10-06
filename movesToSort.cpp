// Created by Ethan Edwards on 10/5/2024
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <sstream>

using namespace std;

// Utility function to print a vector
void printVector(const vector<int>& v) {
    cout << "{ ";
    for (int x : v) {
        if (x == v.back()){
            cout << x << " ";
        } else{
            cout << x << ", ";
        }
    }
    cout << "}\n";
}

// Utility function to convert a vector to a string for tracking visited states
string vectorToString(const vector<int>& v) {
    string result;
    for (int x : v) result += to_string(x) + ",";
    return result;
}

// Generate successors by reversing all possible subarrays
vector<vector<int>> generateSuccessors(const vector<int>& permutation) {
    vector<vector<int>> successors;
    int n = permutation.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 2; j <= n; ++j) {  // Ensure at least 2 elements to reverse
            vector<int> new_perm = permutation;
            reverse(new_perm.begin() + i, new_perm.begin() + j);
            successors.push_back(move(new_perm)); // Use move to optimize copying
        }
    }
    return successors;
}

// Hash function for vector<int>
struct VectorHash {
    size_t operator()(const vector<int>& vec) const {
        size_t hash = 0;
        for (int num : vec) {
            hash = hash * 31 + num;
        }
        return hash;
    }
};

// Equality comparison for vector<int>
struct VectorEqual {
    bool operator()(const vector<int>& lhs, const vector<int>& rhs) const {
        return lhs == rhs;
    }
};

// Breadth-First Search (BFS) for sorting by reversal
void bfs(const vector<int>& start_state, const vector<int>& goal_state) {
    clock_t start_time = clock();
    queue<pair<vector<int>, vector<vector<int>>>> q;  // Each item is (state, path to state)
    unordered_set<vector<int>, VectorHash, VectorEqual> visited; // Using vector<int> as key

    visited.insert(start_state);
    q.push({start_state, {start_state}});

    int num_nodes_expanded = 0;
    int max_queue_size = 1;

    while (!q.empty()) {
        max_queue_size = max(max_queue_size, (int)q.size());
        auto [current_state, path] = q.front();
        q.pop();

        // Check if the goal is reached
        if (current_state == goal_state) {
            clock_t end_time = clock();
            double cpu_time = double(end_time - start_time) / CLOCKS_PER_SEC;

            cout << "Output of BFS:\n\n";
            cout << "Number of moves to sort: " << path.size() - 1 << endl << endl;
            cout << "The path from start state to the goal state:\n";
            for (const auto& state : path) {
                printVector(state);
            }
            cout << endl;
            cout << "The total number of states visited was " << num_nodes_expanded << endl;
            cout << "The max size of the queue was " << max_queue_size << endl;
            cout << "The total CPU time was " << cpu_time << " seconds\n\n";
            return;
        }

        num_nodes_expanded++;

        // Generate successors
        vector<vector<int>> successors = generateSuccessors(current_state);
        for (const auto& successor : successors) {
            // Check if successor is in visited
            if (visited.find(successor) == visited.end()) {  // Not visited
                visited.insert(successor);
                vector<vector<int>> new_path = path;
                new_path.push_back(successor);
                q.push({successor, new_path});
            }
        }
    }

    cout << "No solution found.\n";
}

// Count the number of breakpoints in a permutation
int countBreakpoints(const vector<int>& permutation) {
    int breaks = 0;
    int n = permutation.size();
    for (int i = 0; i < n - 1; ++i) {
        if (abs(permutation[i] - permutation[i + 1]) != 1) {
            breaks++;
        }
    }
    return breaks;
}

// A* Search
void a_star(const vector<int>& start_state, const vector<int>& goal_state) {
    clock_t start_time = clock();
    auto cmp = [](const pair<int, pair<vector<int>, vector<vector<int>>>>& a,
                  const pair<int, pair<vector<int>, vector<vector<int>>>>& b) {
        return a.first > b.first;
    };
    priority_queue<pair<int, pair<vector<int>, vector<vector<int>>>>, vector<pair<int, pair<vector<int>, vector<vector<int>>>>>, decltype(cmp)> open_set(cmp);
    unordered_set<vector<int>, VectorHash, VectorEqual> visited;

    visited.insert(start_state);
    open_set.push({0, {start_state, {start_state}}});

    int num_nodes_expanded = 0;
    int max_queue_size = 1;

    while (!open_set.empty()) {
        max_queue_size = max(max_queue_size, (int)open_set.size());
        auto [f_value, state_pair] = open_set.top();
        auto [current_state, path] = state_pair;
        open_set.pop();

        // Check if the goal is reached
        if (current_state == goal_state) {
            clock_t end_time = clock();
            double cpu_time = double(end_time - start_time) / CLOCKS_PER_SEC;

            cout << "Output of A*:\n\n";
            cout << "Number of moves to sort: " << path.size() - 1 << endl << endl;
            cout << "The path from start state to the goal state:\n";
            for (const auto& state : path) {
                printVector(state);
            }
            cout << endl;
            cout << "The total number of states visited was " << num_nodes_expanded << endl;
            cout << "The max size of the queue was " << max_queue_size << endl;
            cout << "The total CPU time was " << cpu_time << " seconds\n\n";
            return;
        }

        num_nodes_expanded++;

        // Generate successors and optimize heuristic calculation
        for (const auto& successor : generateSuccessors(current_state)) {
            if (visited.find(successor) == visited.end()) {  // Not visited
                visited.insert(successor);

                // Update the heuristic using the incremental count
                int h_value = countBreakpoints(successor) / 2;
                int g_value = path.size();  // Cost is path length (number of moves)
                int f_value_new = g_value + h_value;

                vector<vector<int>> new_path = path;  // Keep track of the path
                new_path.push_back(successor);
                open_set.push({f_value_new, {successor, new_path}});
            }
        }
    }

    cout << "No solution found.\n";
}

bool depthLimitedSearch(const vector<int>& current_state, const vector<int>& goal_state, int depth,
                        vector<vector<int>>& path, int& num_nodes_expanded, int& max_queue_size,
                        unordered_set<string>& visited) {

    if (current_state == goal_state) {
        return true;  // Solution found
    }

    if (depth == 0) {
        return false;  // No solution found at this depth
    }

    num_nodes_expanded++;

    // Generate successors
    for (const auto& successor : generateSuccessors(current_state)) {
        string successor_str = vectorToString(successor);

        // Only explore if successor has not been visited
        if (visited.find(successor_str) == visited.end()) {
            visited.insert(successor_str);  // Mark as visited
            path.push_back(successor);      // Add to path

            // Recursive call with decreased depth
            if (depthLimitedSearch(successor, goal_state, depth - 1, path, num_nodes_expanded, max_queue_size, visited)) {
                return true;  // Solution found in this branch
            }

            path.pop_back();  // Backtrack if not a solution
        }
    }

    return false;  // No solution found at this depth
}

// Iterative Deepening Search (IDS)
void ids(const vector<int>& start_state, const vector<int>& goal_state) {
    clock_t start_time = clock();
    int depth = 0;
    int num_nodes_expanded = 0;
    int max_queue_size = 1;

    // Loop over increasing depth limits
    while (true) {
        vector<vector<int>> path = {start_state};  // Reset the path for each depth
        unordered_set<string> visited;             // Track visited states for each depth level

        // Call depth-limited search
        if (depthLimitedSearch(start_state, goal_state, depth, path, num_nodes_expanded, max_queue_size, visited)) {
            clock_t end_time = clock();
            double cpu_time = double(end_time - start_time) / CLOCKS_PER_SEC;

            cout << "Output of IDS:\n\n";
            cout << "Number of moves to sort: " << path.size() - 1 << endl << endl;
            cout << "The path from start state to the goal state:\n";
            for (const auto& state : path) {
                printVector(state);
            }
            cout << endl;
            cout << "The total CPU time was " << cpu_time << " seconds\n";
            cout << "The total number of states visited was " << num_nodes_expanded << endl << endl;
            return;  // Return after finding the solution
        }

        // Increment depth limit and try again
        depth++;
    }
}

// Main function
int main() {
    cout << "Enter a permutation (space-separated numbers): ";
    string input;
    getline(cin, input);

    vector<int> start_state;
    int num;
    stringstream ss(input);

    while (ss >> num) {
        start_state.push_back(num);
    }

    // Confirm that the input is no greater than 10
    if (start_state.size() > 10) {
        cout << "Please enter a permutation of at most 10 numbers.\n";
        return 0;
    } else{
        cout << "Input: ";
        printVector(start_state);
    }

    vector<int> goal_state = start_state;
    sort(goal_state.begin(), goal_state.end());

    bfs(start_state, goal_state);

    a_star(start_state, goal_state);

    ids(start_state, goal_state);

    return 0;
}

