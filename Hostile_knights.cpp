#include <bits/stdc++.h>
using namespace std;

// --- Your Template Functions ---

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

int maxflow(int s, int t, int n, vector<vector<int>> &capacity, vector<vector<int>> &adj)
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
            cur = prev;
        }
    }
    return flow;
}

// --- Problem Specifics ---

// Knight Moves
int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

int main()
{
    int R, C, K;
    if (!(cin >> R >> C >> K))
        return 0;

    // 1. Mark blocked squares
    vector<vector<bool>> blocked(R, vector<bool>(C, false));
    for (int i = 0; i < K; i++)
    {
        int r, c;
        cin >> r >> c;
        blocked[r][c] = true;
    }

    // 2. Setup Graph Nodes
    // Nodes 0 to (R*C - 1) represent board squares.
    // Node s = R*C (Source)
    // Node t = R*C + 1 (Sink)
    int s = R * C;
    int t = R * C + 1;
    int n = t + 1; // Total nodes size

    vector<vector<int>> capacity(n, vector<int>(n, 0));
    vector<vector<int>> adj(n);

    int valid_squares = 0;
    int INF = 1e9;

    // 3. Build Graph
    for (int r = 0; r < R; r++)
    {
        for (int c = 0; c < C; c++)
        {
            if (blocked[r][c])
                continue; // Skip blocked squares

            valid_squares++;
            int u = r * C + c; // Flatten 2D coordinate to 1D ID

            // Bipartite Split: (r + c) % 2 == 0 is "Black", else "White"
            if ((r + c) % 2 == 0)
            {
                // CASE 1: Black Square (Source -> u)
                capacity[s][u] = 1;
                adj[s].push_back(u);
                adj[u].push_back(s);

                // Add edges to valid knight moves (u -> v)
                for (int i = 0; i < 8; i++)
                {
                    int nr = r + dr[i];
                    int nc = c + dc[i];

                    // Check bounds and if neighbor is not blocked
                    if (nr >= 0 && nr < R && nc >= 0 && nc < C && !blocked[nr][nc])
                    {
                        int v = nr * C + nc;
                        // Directed edge: Black Square -> White Square
                        capacity[u][v] = INF; // Infinite capacity (or just 1 works too for matching)
                        adj[u].push_back(v);
                        adj[v].push_back(u);
                    }
                }
            }
            else
            {
                // CASE 2: White Square (v -> Sink)
                capacity[u][t] = 1;
                adj[u].push_back(t);
                adj[t].push_back(u);
            }
        }
    }

    // 4. Calculate Max Matching
    int matching = maxflow(s, t, n, capacity, adj);

    // 5. Result: Total Valid Squares - Max Matching
    cout << "Max Knights: " << valid_squares - matching << endl;

    return 0;
}