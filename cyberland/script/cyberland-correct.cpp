#include "cyberland.h"
#include <bits/stdc++.h>
#define debug(x) cerr << #x << " " << (x) << endl
#define fi first
#define se second
#define pb push_back
#define mp make_pair
using namespace std;

const int MAXN = 100005, MAXM = 100005;
const double inf = 1e16;

int n, m, k, h, arr_[MAXN];
vector<pair<int, int> > g[MAXN];
double dist[MAXN], dist_[MAXN];
bool vis[MAXN];
void dfs(int u) {
	vis[u] = true;
	for (auto i : g[u]) {
		if (!vis[i.fi] && i.fi != h) dfs(i.fi);
	}
}

priority_queue<pair<double, int> > que;
double dijk(int x) {
	while (!que.empty()) que.pop();
	for (int i = 0; i < n; i++) vis[i] = false, que.push(mp(-dist[i], i));
	for (int i = 0; i < n; i++) {
		while (!que.empty() && vis[que.top().se]) que.pop();
		pair<double, int> pi = que.top();
		int u = pi.se;
		vis[u] = true, que.pop();
		for (auto i : g[u]) {
			int v = i.fi;
			if (v == h) continue;
			if (dist[v] > dist[u] + 1.0 * i.se) {
				dist[v] = dist[u] + 1.0 * i.se;
				que.push(mp(-dist[v], v));
			}
		}
	}
	for (int i = 0; i < n; i++) {
		dist_[i] = inf;
		for (auto j : g[i]) {
			if (arr_[j.fi] == 2 && dist[j.fi] < inf / 2)
                dist_[i] = min(dist_[i], dist[j.fi] / 2.0 + j.se);
		}
	}
	double res = x ? dist_[h] : inf;
	for (auto i : g[h]) res = min(res, dist[i.fi] + i.se);
	for (int i = 0; i < n; i++) dist[i] = i == h ? inf : dist_[i];
	return res;
}

double solve(int N, int M, int K, int H, vector<int> x, vector<int> y, vector<int> c, vector<int> arr) {
	n = N, m = M, k = K, h = H;
	for (int i = 0; i < n; i++) {
		arr_[i] = arr[i];
		dist[i] = i ? inf : 0.0;
		g[i].clear();
		vis[i] = false;
	}
	for (int i = 0; i < n; i++) g[i].clear();
	for (int i = 0; i < m; i++) {
		g[x[i]].pb(mp(y[i], c[i]));
		g[y[i]].pb(mp(x[i], c[i]));
	}
	dfs(0);
	for (int i = 0; i < n; i++) {
		if (vis[i] && !arr[i]) dist[i] = 0.0;
	}
	double ans = inf;
	k = min(k, 70);
	for (int i = k; i >= 0; i--) ans = min(ans, dijk(i));
	return ans >= inf / 2 ? -1 : ans;
}
