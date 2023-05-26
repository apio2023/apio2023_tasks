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
int t,n,m,a[1000002],ans,A,B,R[1000002],stk[1000002],tp,tot;
char s[1000002];
int main(){
	freopen("5-10.in","w",stdout);
	mt19937 rnd(time(0));
	n=500000,m=250000,tot=125000;
	for(re int i=1;i<=125000;++i)a[i]=i;
	for(re int i=125001;i<=m;++i)a[i]=m+i;
	for(re int i=1;i<=m;++i)a[i+m]=a[i];
	for(re int i=83334;i<=160000;++i)a[i]=++tot,a[i+m]=++tot;
	printf("%d\n",n);
	for(re int i=1;i<=n;++i)printf("%d ",a[i]);
}




