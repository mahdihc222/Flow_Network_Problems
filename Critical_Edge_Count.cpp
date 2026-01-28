#include <bits/stdc++.h>
using namespace std;

/*
Question:
Count the number of edges that belong to EVERY possible minimum cut in the graph.
These are the most critical bottlenecks.

EXPLANATION:
An edge (u, v) is in ALL Min-Cuts if and only if:
1. The edge is fully saturated (flow == capacity).
2. 'u' is reachable from Source in the residual graph.
3. 'Sink' is reachable from 'v' in the residual graph.*/

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
            if (parent[v] == -1 && capacity[u][v] != 0)
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
// takes an adjacency matrix and determines if v is reachable from u
bool isReachable(int u, int v, vector<vector<int>> &adj)
{
    int n = adj.size();
    queue<int> q;
    q.push(u);
    vector<bool> vis(n, false);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int i = 0; i < n; i++)
        {
            if (!vis[i] && adj[u][i] > 0)
            {
                q.push(i);
                vis[i] = true;
                if (i == v)
                    return true;
            }
        }
    }
    return false;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> capacity(n, vector<int>(n, 0));
    vector<vector<int>> adj(n);
    vector<pair<int, int>> edges;
    int s, t;
    for (int i = 0; i < m; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        capacity[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.push_back({u, v});
    }
    vector<vector<int>> flows(n, vector<int>(n, 0));
    vector<vector<int>> original_cap = capacity;
    cin >> s >> t;

    int flow = maxflow(s, t, n, capacity, adj, flows);

    vector<int> s_cut;
    s_cut.push_back(s);

    queue<int> q;
    q.push(s);
    vector<bool> visS(n, false);
    visS[s] = true;
    // forward BFS for finding s_cut
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int i = 0; i < n; i++)
        {
            if (capacity[u][i] > 0 && !visS[i])
            {
                visS[i] = true;
                s_cut.push_back(i); // Not used here, but can be used when needed.
                q.push(i);
            }
        }
    }

    // backward BFS from t now
    vector<int> t_set;
    t_set.push_back(t);
    vector<bool> visT(n, false);
    visT[t] = true;
    q.push(t);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        for (int prev : adj[curr])
        {
            if (!visT[prev] && capacity[prev][curr] > 0)
            {
                visT[prev] = true;
                q.push(prev);
                t_set.push_back(prev);
            }
        }
    }

    vector<pair<int, int>> critical_edges;
    vector<pair<int, int>> cut_edges;
    /*1. Edges in AT LEAST ONE Min-Cut
     An edge (u, v) belongs to some minimum cut if:
    (i) It is Saturated (Flow == Capacity).
 (ii) u and v belong to different Strongly Connected Components (SCCs) in the residual graph.Simple check: There is no path from u to v in the residual graph.*/
    for (auto it : edges)
    {
        int u = it.first;
        int v = it.second;
        if (capacity[u][v] == 0 && visS[u] && visT[v])
        {
            critical_edges.push_back({u, v});
        }
        if (capacity[u][v] == 0 && isReachable(u, v, capacity) == false)
        {
            cut_edges.push_back({u, v});
        }
    }
    cout << "Critical Edges: " << endl;
    for (auto it : critical_edges)
    {
        cout << it.first << " " << it.second << endl;
    }
    cout << "Possible Cut Edges: " << endl;
    for (auto it : cut_edges)
    {
        cout << it.first << " " << it.second << endl;
    }

    /* QUESTION 4: Expensive Expansion
    Identify which edges, if their capacity were increased, would strictly increase the total Max Flow.
    */

    // this is same as critical edges

    return 0;
}