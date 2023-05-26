#include "cyberland.h"

#include <bits/stdc++.h>

using namespace std;

typedef long long lo; 

#define fi first
#define se second
#define int long long
#define pb push_back

const lo inf = 1000000000000000000;
const lo li = 100005;

int n,m,k;
int arr[li],flag,t,h,connected[li],vis[li][75];
double pw[75],dis[li][75];
vector<pair<int,int>> v[li];
vector<int> vec;

inline void dfss(int node){
	if(connected[node])return;
	connected[node]=1;
	if(node==h)return ;
	for(auto go:v[node]){
		if(connected[go.fi]==0)dfss(go.fi);
	}
}

inline double sp(){
	priority_queue<pair<double,pair<int,int>>> pq;
	for(auto go:vec){
		for(int i=0;i<=k;i++){
			pq.push({0,{i,go}});
			dis[go][i]=0;
		}
	}
	flag=0;
	while(pq.size()){
		double tim=-pq.top().fi;
		int div=pq.top().se.fi;
		int node=pq.top().se.se;
		pq.pop();
		if(div<0)continue;
		if(vis[node][div])continue;
		vis[node][div]=1;
		if(node==h && div==0) return tim;
		if(node==h)continue;
		for(auto go:v[node]){
			int yes=div;
			double nv=tim+go.se/pw[div];
			if (dis[go.fi][yes]>nv){
				dis[go.fi][yes]=nv;
				pq.push({-nv,{yes,go.fi}});
			}
			if(arr[go.fi]==2){
				yes--;
				if (yes>=0&&dis[go.fi][yes]>nv){
					dis[go.fi][yes]=nv;
					pq.push({-nv,{yes,go.fi}});
				}
			}
		}
	}
    return -1;
}

double solve(int32_t N, int32_t M, int32_t K, int32_t H,
             vector<int32_t> x,vector<int32_t> y,vector<int32_t> c,vector<int32_t> _arr){
	pw[0]=1;
	for(int i=1;i<=70;i++){
		pw[i]=pw[i-1]+pw[i-1];
	}
	n=N;
    m=M;
    k=min(K,70);
    h=H;
    vec.clear();
	vec.pb(0);
	for(int i=0;i<n;i++){
		v[i].clear();
		connected[i]=0;
		for(int j=0;j<=k;j++)
			dis[i][j]=1e18,vis[i][j]=0;
        arr[i]=_arr[i];
	}
	for(int i=0;i<m;i++){
		v[x[i]].pb({y[i],c[i]});
		v[y[i]].pb({x[i],c[i]});
	}
	dfss(0);
    for(int i=0;i<n;i++)
		if(arr[i]==0 && connected[i])vec.push_back(i);
	if(connected[h]==0) return -1;
	return sp();
}
