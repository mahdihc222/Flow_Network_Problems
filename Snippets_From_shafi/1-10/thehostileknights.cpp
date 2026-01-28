/* QUESTION 6: The "Hostile" Knights
Place the maximum number of Knights on a chessboard (with some blocked squares) 
such that no two Knights attack each other.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;
int R, C, K; // Rows, Cols, Blocked squares

struct Edge {
    int v, cap, flow, rev;
};

vector<vector<Edge>> adj;
vector<pair<int, int>> parent;

// Knight moves
int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

void add_edge(int u, int v, int cap) {
    Edge a = {v, cap, 0, (int)adj[v].size()};
    Edge b = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(a);
    adj[v].push_back(b);
}

int bfs(int s, int t) {
    fill(parent.begin(), parent.end(), make_pair(-1, -1));
    parent[s] = {-2, -2};
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int i = 0; i < adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (parent[e.v].first == -1 && e.cap - e.flow > 0) {
                parent[e.v] = {u, i};
                int new_flow = min(flow, e.cap - e.flow);
                if (e.v == t) return new_flow;
                q.push({e.v, new_flow});
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0, new_flow;
    while (new_flow = bfs(s, t)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur].first;
            int idx = parent[cur].second;
            adj[prev][idx].flow += new_flow;
            int rev = adj[prev][idx].rev;
            adj[cur][rev].flow -= new_flow;
            cur = prev;
        }
    }
    return flow;
}

int main() {
    if (!(cin >> R >> C >> K)) return 0;

    // Use a 2D grid to track blocked squares
    vector<vector<bool>> blocked(R, vector<bool>(C, false));
    for(int i=0; i<K; ++i) {
        int r, c; cin >> r >> c;
        blocked[r][c] = true;
    }

    /* EXPLANATION:
    This is a "Maximum Independent Set" problem on a Bipartite Graph. A chessboard can be colored 
    black and white such that Knights always move from black to white. We construct a flow network 
    where Source connects to all "Black" squares (capacity 1) and all "White" squares connect to 
    Sink (capacity 1). Edges represent Knight moves from Black to White (capacity INF). 
    Max Flow finds the Maximum Matching (max pairs of attacking knights). By Konig's theorem, 
    Max Independent Set = Total Valid Squares - Max Matching.
    */

    int s = R * C, t = R * C + 1;
    adj.assign(t + 1, vector<Edge>());
    parent.resize(t + 1);

    int valid_squares = 0;

    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (blocked[r][c]) continue;
            valid_squares++;
            
            int u = r * C + c; // Flatten grid index

            // Check if "Black" square (arbitrarily defined by parity sum)
            if ((r + c) % 2 == 0) {
                add_edge(s, u, 1); // Source -> Black
                
                // Add edges to valid "White" moves
                for(int i=0; i<8; i++) {
                    int nr = r + dr[i], nc = c + dc[i];
                    if(nr >= 0 && nr < R && nc >= 0 && nc < C && !blocked[nr][nc]) {
                        int v = nr * C + nc;
                        add_edge(u, v, INF);
                    }
                }
            } else {
                add_edge(u, t, 1); // White -> Sink
            }
        }
    }

    int matching = max_flow(s, t);
    cout << "Max Knights: " << valid_squares - matching << endl;

    return 0;
}