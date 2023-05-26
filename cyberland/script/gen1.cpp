#include<bits/stdc++.h>

using namespace std;

int rnd(){
	int x=0;
	for (int i=1;i<=9;i++)
		x=x*10+rand()%10;
	return x;
}

map<pair<int,int>,int> mp;
void gen(int n,int m,int tp){
	if (n<=15||m>=3*n){
		//rand
		int k;
		if (tp==1) k=rnd()%25+5;
		else{
			k=rnd()%4990+10;/*[10~5000] 5000*70=30~350000*/
			k=(int)(2e6/pow(k,1.5));
			k=(int)(1.5*k-rnd()%k);
		}
		int h=rnd()%(n-2)+1;
		printf("%d %d %d\n%d\n",n,m,k,h);
		mp.clear();
		for (int i=0;i<n;i++)
			printf("%d%c",i==0||i==h?1:rnd()%3,i==n-1?'\n':' ');
		for (int i=1;i<=m;i++){
			int x=rnd()%n;
			int y=rnd()%n;
			while (x==y||mp[make_pair(x,y)]){
				x=rnd()%n;
				y=rnd()%n;
			}
			printf("%d %d %d\n",x,y,(rnd()&1?rnd()%3000+1:rnd()+1));
			mp[make_pair(x,y)]=1;
			mp[make_pair(y,x)]=1;
		}
	}
	else{
		int k;
		if (tp==1) k=rnd()%25+5;
		else{
			k=rnd()%4990+10;/*[10~5000] 5000*70=30~350000*/
			k=(int)(2e6/pow(k,1.5));
			k=(int)(1.5*k-rnd()%k);
		}
		int h=n-1;
		printf("%d %d %d\n%d\n",n,m,k,h);
		int lowb=min(4,m-(n-1)+1);
		int upn=n-2-lowb;
		mp.clear();
		for (int i=0;i<n;i++){
			int c=1;
			if (i>upn&&i<=upn+lowb)
				c=2;
			else if (i!=0&&i!=n-1){
				if (rnd()%(int)(sqrt(n))==0) c=0;
				if (rnd()%(int)(sqrt(n))==0) c=2;
			}
			printf("%d%c",c,i==n-1?'\n':' ');
		}
		for (int i=1;i<=upn;i++){
			int x=i,y=i-1-rnd()%min(int(sqrt(n)),i);
			assert(!mp[make_pair(x,y)]);
			printf("%d %d %d\n",x,y,rnd()+1);
			mp[make_pair(x,y)]=1;
			mp[make_pair(y,x)]=1;
		}
		for (int i=upn+1;i<=m-2*lowb;i++){
			int x=rnd()%upn+1,y=x-1-rnd()%min(int(sqrt(n)),x);
			while (mp[make_pair(x,y)])
				x=rnd()%upn+1,y=x-1-rnd()%min(int(sqrt(n)),x);
			printf("%d %d %d\n",x,y,rnd()+1);
			mp[make_pair(x,y)]=1;
			mp[make_pair(y,x)]=1;
		}
		for (int i=upn+1;i<n-1;i++){
			int p=rnd()%(upn+1);
			int x=i,y=p;
			assert(!mp[make_pair(x,y)]);
			printf("%d %d %d\n",x,y,rnd()%10+1);
			mp[make_pair(x,y)]=1;
			mp[make_pair(y,x)]=1;
			x=i,y=n-1;
			assert(!mp[make_pair(x,y)]);
			printf("%d %d %d\n",x,y,rnd()%100+1);
			mp[make_pair(x,y)]=1;
			mp[make_pair(y,x)]=1;
		}
	}
}
int a[100005],b[100005];
int main(int argc,char **argv){
	srand(time(NULL));
	for (;clock()<=100;rand());
	int T=atoi(argv[1]);
	int tp=atoi(argv[2]);
	cerr<<T<<' '<<tp<<endl;
	for (int i=1;i<=T;i++)
		a[i]=rnd()%100000+1;
	sort(a+1,a+T+1);
	for (int i=T;i>=1;i--)
		a[i]=a[i]-a[i-1];
	//cerr<<T<<' '<<tp<<endl;
	int top=0;
	for (int i=1;i<=T;i++)
		if (a[i]>=5) a[++top]=a[i];
	assert(top>=1);
	T=top;
	for (int i=1;i<=T;i++)
		b[i]=a[i];
	int remm=100000;
	for (int i=1;i<=T;i++)
		remm-=b[i];
	//cerr<<T<<' '<<tp<<endl;
	for (int i=1;i<=remm;i++){
		bool flg=0;
		for (int j=1;j<=1000&&!flg;j++){
			int p=rnd()%T+1;
			if (b[p]!=1ll*a[p]*(a[p]-1)/2)
				++b[p],flg=1;
		}
		if (!flg) break;
	}
	int sumn=0,summ=0;
	for (int i=1;i<=T;i++)
		sumn+=a[i],summ+=b[i];
	cerr<<sumn<<' '<<summ<<' '<<T<<endl;
	printf("%d\n",T);
	while (T--)
		gen(a[T+1],b[T+1],tp);
} 
