#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<map>
#include<cmath>
#include<bitset>
#include<set>
#include<iomanip>

using namespace std;
typedef pair<int,int> ii;
typedef pair<int,ii> iii;

int ux,uy;
vector<vector<int> > sum;
int v;
int n;

void updatey(int o1,int o2,int yl,int yr){
	if(yl==yr){
		sum[o1][o2]+=v;
	}else{
		int m = (yl+yr)/2;
		if(uy<=m)updatey(o1,o2*2,yl,m);
		if(uy>m) updatey(o1,o2*2+1,m+1,yr);
		sum[o1][o2]+=v;
	}
}

void updatex(int o1,int xl,int xr){
	if(xl==xr){
		updatey(o1,1,0,n-1);
	}else{
		int m = (xl+xr)/2;
		if(ux<=m) updatex(o1*2,xl,m);
		if(ux>m) updatex(o1*2+1,m+1,xr);
		updatey(o1,1,0,n-1);
	}
}

int qux,quy,qvx,qvy,re1,re2;
void queryy(int o1,int o2,int yl,int yr){
	if(quy<=yl&&qvy>=yr){
		re2+=sum[o1][o2];
	}else{
		int m = (yl+yr)/2;
		if(quy<=m) queryy(o1,o2*2,yl,m);
		if(qvy>m) queryy(o1,o2*2+1,m+1,yr);

	}
}

void queryx(int o1,int xl,int xr){
	if(qux<=xl&&qvx>=xr){
		re2=0;
		queryy(o1,1,0,n-1);
		re1+=re2;
	}else{
		int m = (xl+xr)/2;
		if(qux<=m) queryx(o1*2,xl,m);
		if(qvx>m) queryx(o1*2+1,m+1,xr);
	}
}

int main(){
	return 0;
}
