#include"testlib.h"
#include<bits/stdc++.h>
using namespace std;

const int N=100005;
map<pair<int,int>, int> mp;
vector<int> e[100005];
int fa[100005];
int get(int x){
    return x==fa[x]?x:fa[x]=get(fa[x]);
}
bool connectivity(int n){
    for (int i=0;i<n;i++)
        fa[i]=i;
    for (int i=0;i<n;i++)
        for (int j=0;j<e[i].size();j++)
            fa[get(i)]=get(e[i][j]);
    for (int i=0;i<n;i++)
        if (get(i)!=get(0))
            return 0;
    return 1;
}
int main(int argc, char* argv[]){
	registerValidation(); 
    int tid = atoi(argv[1]);
    int T = inf.readInt(1,100000,"T");
    inf.readEoln();
    int remn = 100000;
    int remm = 100000;
    while (T--){
        int n=inf.readInt(2, remn ,"n");
        inf.readSpace();
        int limm = (n>=1000?remm: min(remm,n*(n-1)/2));
        int m=inf.readInt(0, limm, "m");
        inf.readSpace();
        int k=inf.readInt(1, tid==8?1e6:30, "k");
        inf.readEoln();
        ensuref(tid<2||tid>4||m==n-1, "m == n-1 not holds");
        int H=inf.readInt(1, n-1, "H");
        inf.readEoln();
        remn-=n;
        remm-=m;
        for (int i=0;i<n;i++){
            int larr=(tid==2||tid==5?1:0);
            int rarr=(tid==2||tid==3||tid==5||tid==6?1:2);
            inf.readInt(larr,rarr,"arr[i]");
            if (i+1==n)
                inf.readEoln();
            else
                inf.readSpace();
        }
        mp.clear();
        for (int i=0;i<n;i++)
            e[i].resize(0);
        for (int i=0;i<m;i++){
            int x=inf.readInt(0,n-1,"x[i]");
            inf.readSpace();
            int y=inf.readInt(0,n-1,"y[i]");
            inf.readSpace();
            int c=inf.readInt(0,1e9,"c[i]");
            inf.readEoln();
            ensuref(!mp[make_pair(x,y)], "duplicate edge");
            ensuref(x!=y, "self loop");
            ensuref(x==i||tid!=4, "x[i] != i");
            ensuref(y==i+1||tid!=4, "y[i] != i+1");
            mp[make_pair(x,y)]=1;
            mp[make_pair(y,x)]=1;
            e[x].push_back(y);
        }
        if (tid>=2&&tid<=4)
            ensuref(connectivity(n), "graph not connected");
    }
	inf.readEof();
}
