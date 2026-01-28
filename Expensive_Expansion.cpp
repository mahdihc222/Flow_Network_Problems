/* QUESTION 4: Expensive Expansion
Identify which edges, if their capacity were increased, would strictly increase the total Max Flow.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Edge
{
    int v, cap, flow, rev;
};

vector<vector<Edge>> adj;
vector<pair<int, int>> parent;

void add_edge(int u, int v, int cap)
{
    Edge a = {v, cap, 0, (int)adj[v].size()};
    Edge b = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(a);
    adj[v].push_back(b);
}

int bfs(int s, int t)
{
    fill(parent.begin(), parent.end(), make_pair(-1, -1));
    parent[s] = {-2, -2};
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty())
    {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int i = 0; i < adj[u].size(); i++)
        {
            Edge &e = adj[u][i];
            if (parent[e.v].first == -1 && e.cap - e.flow > 0)
            {
                parent[e.v] = {u, i};
                int new_flow = min(flow, e.cap - e.flow);
                if (e.v == t)
                    return new_flow;
                q.push({e.v, new_flow});
            }
        }
    }
    return 0;
}

void solve_max_flow(int s, int t)
{
    int new_flow;
    while (new_flow = bfs(s, t))
    {
        int cur = t;
        while (cur != s)
        {
            int prev = parent[cur].first;
            int idx = parent[cur].second;
            adj[prev][idx].flow += new_flow;
            int rev = adj[prev][idx].rev;
            adj[cur][rev].flow -= new_flow;
            cur = prev;
        }
    }
}

int main()
{
    int N, M;
    if (!(cin >> N >> M))
        return 0;

    adj.assign(N + 1, vector<Edge>());
    parent.resize(N + 1);

    for (int i = 0; i < M; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    /* EXPLANATION:
    An edge (u, v) is a candidate for expansion only if it is fully saturated (bottleneck).
    However, simply being saturated isn't enough; increasing its capacity must open a new path
    from Source to Sink. This happens if and only if 'u' is reachable from the Source in the
    residual graph, AND the Sink is reachable from 'v' in the residual graph. We identify
    these sets by running BFS from Source (forward) and BFS from Sink (backward/reversed)
    on the residual graph after the initial Max Flow calculation.
    */

    solve_max_flow(1, N);

    // 1. Find S-Reachable Nodes (Forward BFS in Residual)
    vector<bool> S_reach(N + 1, false);
    queue<int> q;
    q.push(1);
    S_reach[1] = true;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto &e : adj[u])
        {
            if (!S_reach[e.v] && e.cap - e.flow > 0)
            {
                S_reach[e.v] = true;
                q.push(e.v);
            }
        }
    }

    // 2. Find T-Reachable Nodes (Backward BFS in Residual)
    // We build the explicit reverse residual graph to make this BFS simple.
    vector<vector<int>> rev_res(N + 1);
    for (int u = 1; u <= N; u++)
    {
        for (auto &e : adj[u])
        {
            if (e.cap - e.flow > 0)
            {                              // If u->v exists in residual
                rev_res[e.v].push_back(u); // Add v->u to reverse graph
            }
        }
    }

    vector<bool> T_reach(N + 1, false);
    q.push(N);
    T_reach[N] = true;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : rev_res[u])
        {
            if (!T_reach[v])
            {
                T_reach[v] = true;
                q.push(v);
            }
        }
    }

    cout << "Edges to expand:" << endl;
    for (int u = 1; u <= N; u++)
    {
        for (auto &e : adj[u])
        {
            if (e.cap > 0 && e.flow == e.cap)
            { // Saturated
                if (S_reach[u] && T_reach[e.v])
                {
                    cout << u << " " << e.v << endl;
                }
            }
        }
    }

    return 0;
}