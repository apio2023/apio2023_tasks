#include "sequence.h"
#include<bits/stdc++.h>
#define re register
using namespace std;
int t,n,m,a[1000002],ans,b[1000002];
int sequence(int N, std::vector<int> A){
	n=N;
	for(re int i=1;i<=n;++i)a[i]=L[i-1];
	for(re int i=1;i<=n;++i){
		m=0;
		for(re int j=i;j<=n;++j){
			b[++m]=a[j],sort(b+1,b+m+1);
			if(m%2==1){
				re int s=0;
				for(re int k=1;k<=m;++k)s+=b[k]==b[m+1>>1];
				ans=max(ans,s);
			}
			else{
				re int s=0;
				for(re int k=1;k<=m;++k)s+=b[k]==b[m>>1];
				ans=max(ans,s),s=0;
				for(re int k=1;k<=m;++k)s+=b[k]==b[m+2>>1];
				ans=max(ans,s);
			}
		}
	}
	return ans;
}
