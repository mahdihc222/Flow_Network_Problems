/* QUESTION 3: The Sabotage (Vertex Cut)
Find the minimum number of intersections (nodes) to block to completely stop
traffic from Source to Sink. You cannot block Source or Sink themselves.
*/

#include <bits/stdc++.h>
using namespace std;

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

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
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

    cout << flow << endl;
    for (auto e : edges)
    {
        cout << e.first << " " << e.second << " " << flows[e.first][e.second] << "/" << original_cap[e.first][e.second] << endl;
    }

    return 0;
}