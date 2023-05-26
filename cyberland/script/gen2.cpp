#include<bits/stdc++.h>

using namespace std;

int rnd(){
	int x=0;
	for (int i=1;i<=9;i++)
		x=x*10+rand()%10;
	return x;
}

int main(int argc,char **argv){
	printf("%d\n",1);
	printf("100000 99999 1000000\n");
	printf("99999\n");
	for (int i=0;i<=99999;i++)
		printf("%d%c",i==99998||i==99997?2:1,i==99999?'\n':' ');
	for (int i=1;i<=99999;i++)
		printf("%d %d %d\n",i-1,i,i==99998||i==99999?1:1000000000);
} 
