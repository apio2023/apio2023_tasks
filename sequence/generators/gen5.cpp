#include<bits/stdc++.h>
#define re register
using namespace std;
inline int read(){
	re int t=0;re char v=getchar();
	while(v<'0')v=getchar();
	while(v>='0')t=(t<<3)+(t<<1)+v-48,v=getchar();
	return t;
}
const int M=998244353;
inline void add(re int &x,re int y){(x+=y)>=M?x-=M:x;}
inline int Mod(re int x){return x>=M?x-M:x;}
inline int ksm(re int x,re int y){
	re int s=1;
	while(y){
		if(y&1)s=1ll*s*x%M;
		x=1ll*x*x%M,y>>=1;
	}
	return s;
}
int t,n,m,a[1000002],ans,A,B,R[1000002],stk[1000002],tp;
char s[1000002];
int main(){
	freopen("7-17.in","w",stdout);
	mt19937 rnd(time(0));
	n=500000-rnd()%5,m=n/10;
	int L=rnd()%(m+m)+1,R=n-rnd()%(m+m);
	for(re int i=L;i<=R;++i)a[i]=rnd()%n+1;
	sort(a+L,a+R+1);
	int px=L+R>>1,py=px+rnd()%m+m;
	for(re int i=L;i<px;++i)if(a[i]==a[px])--a[i];
	for(re int i=px;i<=py;++i)a[i]=a[px];
	for(re int i=1;i<L;++i)a[i]=rnd()%(a[px]-1)+1;
	for(re int i=R+1;i<=n;++i)a[i]=rnd()%(a[px]-1)+1;
	shuffle(a+L,a+R+1,rnd);
	if(rnd()&1)for(re int i=1;i<=n;++i)a[i]=n-a[i]+1;
	printf("%d\n",n);
	for(re int i=1;i<=n;++i)printf("%d ",a[i]);
}



