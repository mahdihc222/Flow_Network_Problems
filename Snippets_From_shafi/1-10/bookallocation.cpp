//question 6 ni nai.

/* QUESTION 7: Book Allocation
Maximize the number of students who get a book, given that each book has 
multiple copies available.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Edge {
    int v, cap, flow, rev;
};

vector<vector<Edge>> adj;
vector<pair<int, int>> parent;

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
    int Students, Books;
    if (!(cin >> Students >> Books)) return 0;

    int s = 0, t = Students + Books + 1;
    adj.assign(t + 1, vector<Edge>());
    parent.resize(t + 1);

    // Read copies for each book
    vector<int> copies(Books + 1);
    for(int i=1; i<=Books; i++) cin >> copies[i];

    // Read student interests
    int K; cin >> K; // Number of interest pairs
    for(int i=0; i<K; i++) {
        int stu, book; cin >> stu >> book;
        add_edge(stu, Students + book, 1);
    }

    /* EXPLANATION:
    This is a Bipartite Matching variation with capacity constraints on the Sink side.
    We create a Source connected to every Student with capacity 1 (each student gets at most 1 book).
    We create a Sink connected from every Book node, but the capacity of this edge is equal to 
    the number of copies available for that book. Edges from Students to Books have capacity 1. 
    The Max Flow calculates the maximum number of students that can be satisfied.
    */

    // 1. Source -> Student (Cap 1)
    for(int i=1; i<=Students; i++) add_edge(s, i, 1);

    // 2. Book -> Sink (Cap = Copies)
    for(int j=1; j<=Books; j++) add_edge(Students + j, t, copies[j]);

    cout << "Max Students Satisfied: " << max_flow(s, t) << endl;

    return 0;
}