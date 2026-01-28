#include <bits/stdc++.h>
using namespace std;

/* QUESTION 7: Book Allocation
Maximize the number of students who get a book, given that each book has
multiple copies available.
*/

/* EXPLANATION:
   This is a Bipartite Matching variation with capacity constraints on the Sink side.
   We create a Source connected to every Student with capacity 1 (each student gets at most 1 book).
   We create a Sink connected from every Book node, but the capacity of this edge is equal to
   the number of copies available for that book. Edges from Students to Books have capacity 1.
   The Max Flow calculates the maximum number of students that can be satisfied.
   */

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
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int n, m;      // n is students
    cin >> n >> m; // m is books
    int s = 0, t = n + m + 1;
    vector<vector<int>> capacity(n + m + 2, vector<int>(n + m + 2, 0));
    vector<vector<int>> adj(n + m + 2);
    vector<pair<int, int>> edges;

    vector<int> copies(m + 1, 0);
    for (int i = 1; i <= m; i++)
    {
        cin >> copies[i];
    }
    int k;
    cin >> k;
    // number of interest pairs is k
    for (int i = 0; i < k; i++)
    {
        int u, v;
        cin >> u >> v;
        if (!capacity[u][v])
            edges.push_back({u, v});
        capacity[u][v + n] = 1;
        adj[u].push_back(v + n);
        adj[v + n].push_back(u);
    }
    for (int i = 1; i <= n; i++)
    {
        capacity[s][i] = 1;
        adj[s].push_back(i);
        adj[i].push_back(s);
    }
    for (int i = n + 1; i <= n + m; i++)
    {
        capacity[i][t] = copies[i - n];
        adj[i].push_back(t);
        adj[t].push_back(i);
    }

    vector<vector<int>> flows(n + m + 2, vector<int>(n + m + 2, 0));

    int flow = maxflow(s, t, n + m + 2, capacity, adj, flows);

    cout << flow << endl;
    for (auto e : edges)
    {
        if (flows[e.first][e.second + n])
        {
            cout << e.first << " " << e.second << endl;
        }
    }
    return 0;
}