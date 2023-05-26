#include "sequence.h"
#include<bits/stdc++.h>
#define re register
using namespace std;
int n,m,a[500002],ans,mx[2000002],mn[2000002],sum,tg[2000002],Mx,Mn,pmx[2000002],pmn[2000002],smx[2000002],smn[2000002],Q[500002],hd,tl;
vector<int>AA[500002];
inline void Add(re int p,re int x){
	mx[p]+=x,mn[p]+=x,tg[p]+=x;
}
inline void pd(re int p){
	if(tg[p])Add(p<<1,tg[p]),Add(p<<1|1,tg[p]),tg[p]=0;
}
inline void pu(re int p){
	mx[p]=max(mx[p<<1],mx[p<<1|1]);
	mn[p]=min(mn[p<<1],mn[p<<1|1]);
}
inline void add(re int p,re int l,re int r,re int x,re int y,re int z){
	if(l>=x&&r<=y)return Add(p,z);
	pd(p);
	re int mid=l+r>>1;
	if(x<=mid)add(p<<1,l,mid,x,y,z);
	if(y>mid)add(p<<1|1,mid+1,r,x,y,z);
	pu(p);
}
inline void ask(re int p,re int l,re int r,re int x,re int y){
	if(l>=x&&r<=y){
		Mx=max(Mx,mx[p]),Mn=min(Mn,mn[p]);
		return;
	}
	pd(p);
	re int mid=l+r>>1;
	if(x<=mid)ask(p<<1,l,mid,x,y);
	if(y>mid)ask(p<<1|1,mid+1,r,x,y);
}
int sequence(int N, std::vector<int> A){
	n=N;
	for(re int i=1;i<=n;++i)a[i]=A[i-1],AA[a[i]].push_back(i),add(1,0,n,i,n,1);
	for(re int i=1;i<=n;++i){
		for(auto z:AA[i])Mn=1e9,ask(1,0,n,0,z-1),pmn[z]=Mn,Mx=-1e9,ask(1,0,n,z,n),smx[z]=Mx;
		for(auto z:AA[i])add(1,0,n,z,n,-2);
		for(auto z:AA[i])Mx=-1e9,ask(1,0,n,0,z-1),pmx[z]=Mx,Mn=1e9,ask(1,0,n,z,n),smn[z]=Mn;
		hd=1,tl=0;
		for(auto z:AA[i]){
			Q[++tl]=z;
			while(hd<=tl&&1ll*(smx[z]-pmn[Q[hd]])*(smn[z]-pmx[Q[hd]])>0)++hd;
			ans=max(ans,tl-hd+1);
		}
	}
	return ans;
}
