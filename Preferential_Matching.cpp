/* QUESTION 5: Preferential Matching
Maximize matches for Priority 1 choices first, then maximize Priority 2
without reducing the number of Priority 1 matches.
*/

#include <bits/stdc++.h>
using namespace std;

// Standard BFS for augmenting paths
int bfs(int s, int t, int n, vector<int> &parent, vector<vector<int>> &capacity, vector<vector<int>> &adj)
{
    parent.assign(n, -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty())
    {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int v : adj[u])
        {
            // Check for residual capacity
            if (parent[v] == -1 && capacity[u][v] > 0)
            {
                parent[v] = u;
                int new_flow = min(flow, capacity[u][v]);
                if (v == t)
                    return new_flow;
                q.push({v, new_flow});
            }
        }
    }
    return 0;
}

// Standard MaxFlow function
// NOTE: This modifies 'capacity' by reference, preserving the state for Phase 2.
int maxflow(int s, int t, int n, vector<vector<int>> &capacity, vector<vector<int>> &adj, vector<vector<int>> &flows)
{
    int flow = 0;
    vector<int> parent(n);
    int new_flow;
    while (new_flow = bfs(s, t, n, parent, capacity, adj))
    {
        flow += new_flow;
        int cur = t;
        while (cur != s)
        {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;

            flows[prev][cur] += new_flow;
            flows[cur][prev] -= new_flow;
            cur = prev;
        }
    }
    return flow;
}

int main()
{
    int students, topics;
    if (!(cin >> students >> topics))
        return 0;

    // Node Indexing:
    // 0: Source (S)
    // 1 to students: Student Nodes
    // students+1 to students+topics: Topic Nodes
    // students+topics+1: Sink (T)

    int s = 0;
    int t = students + topics + 1;
    int total_nodes = t + 1;

    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> flows(total_nodes, vector<int>(total_nodes, 0));

    // Store choices to add later
    vector<pair<int, int>> choices(students + 1);
    for (int i = 1; i <= students; i++)
    {
        cin >> choices[i].first >> choices[i].second;
    }

    // --- SETUP CONSTANT EDGES ---
    // Source -> Students (Capacity 1)
    for (int i = 1; i <= students; i++)
    {
        capacity[s][i] = 1;
        adj[s].push_back(i);
        adj[i].push_back(s);
    }
    // Topics -> Sink (Capacity 1)
    for (int j = 1; j <= topics; j++)
    {
        int u = students + j;
        capacity[u][t] = 1;
        adj[u].push_back(t);
        adj[t].push_back(u);
    }

    // --- PHASE 1: Priority 1 Only ---
    for (int i = 1; i <= students; i++)
    {
        int u = i;
        int v = students + choices[i].first; // Priority 1 Topic Node

        capacity[u][v] = 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Run Max Flow for Phase 1
    // The 'capacity' matrix will update to reflect used edges.
    int p1_matches = maxflow(s, t, total_nodes, capacity, adj, flows);

    // --- PHASE 2: Add Priority 2 Edges ---
    // We add these edges to the EXISTING graph state.
    for (int i = 1; i <= students; i++)
    {
        int u = i;
        int v = students + choices[i].second; // Priority 2 Topic Node

        // Only add if it's a new edge (or increase capacity if edge existed, though here cap is 1)
        // Check if edge already exists in adjacency to avoid duplicates in list
        bool exists = false;
        for (int neighbor : adj[u])
            if (neighbor == v)
                exists = true;

        if (!exists)
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        capacity[u][v] = 1;
    }

    // Run Max Flow again. It picks up where Phase 1 left off.
    int p2_matches = maxflow(s, t, total_nodes, capacity, adj, flows);

    cout << "Priority 1 Matches: " << p1_matches << endl;
    cout << "Priority 2 Matches (Added): " << p2_matches << endl;
    cout << "Total Matches: " << p1_matches + p2_matches << endl;

    return 0;
}