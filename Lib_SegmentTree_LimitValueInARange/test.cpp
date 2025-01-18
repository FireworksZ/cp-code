#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<cstring>
#include<map>
#include<cmath>
#include<bitset>
#include<set>
#include<cstdlib>
#include<complex>
#include<list>
#include<sstream>
#include<cassert>
#include<iomanip>
#include<limits>

using namespace std;

#define double long double

typedef pair<int,int> ii;
typedef pair<int,long long> il;
typedef pair<long long,long long> ll;
typedef pair<ll,int> lli;
typedef pair<long long,int> li;
typedef pair<double,double> dd;
typedef pair<ii,int> iii;
typedef pair<double,int> di;
typedef pair<int,ii> iii2;
typedef pair<ii,ii> iiii;
typedef vector<int> vi;
long long mod = 1000000007LL;
long long large = 2000000000000000000LL;

//HDU5306
vector<int> cnt,label,maxi,lm;
vector<long long> sum;

vector<int> a;

void maintain(int o,int l,int r){
	sum[o] = 0;
	if(label[o]!=-1) lm[o] = label[o];
	if(l==r){
		cnt[o] = 1;
		lm[o] = -1;
	}else{
		if(label[o*2]!=-1&&label[o*2+1]!=-1){
			label[o] = max(label[o*2],label[o*2+1]);
			if(label[o*2]==label[o]){
				sum[o*2]-=1LL*label[o*2]*cnt[o*2];
				label[o*2] = -1;
			}
			if(label[o*2+1]==label[o]){
				sum[o*2+1]-=1LL*label[o*2+1]*cnt[o*2+1];
				label[o*2+1] = -1;
			}
		}
		sum[o] = sum[o*2]+sum[o*2+1];
		cnt[o] = 0;
		if(label[o*2]==-1) cnt[o]+=cnt[o*2];
		if(label[o*2+1]==-1) cnt[o]+=cnt[o*2+1];
		lm[o] = max(lm[o*2],lm[o*2+1]);
		lm[o] = max(lm[o],max(label[o*2],label[o*2+1]));
	}
	if(label[o]!=-1) sum[o]+=1LL*label[o]*cnt[o];
}

void build(int o,int l,int r){
	if(l==r){
		maxi[o] = a[l];
	}else{
		int m = (l+r)/2;
		build(o*2,l,m);
		build(o*2+1,m+1,r);
		maxi[o] = max(maxi[o*2],maxi[o*2+1]);
	}
	label[o] = maxi[o];
	maintain(o,l,r);
}


int ul,ur;
int uv;

void dfs(int o,int l,int r){
	if(max(lm[o],label[o])<uv) return;
	if(label[o]!=-1&&label[o]>=uv){
		label[o] = -1;
	}
	if(r>l){
		int m = (l+r)/2;
		dfs(o*2,l,m);
		dfs(o*2+1,m+1,r);
	}
	maintain(o,l,r);
}
void update(int o,int l,int r){
	if(label[o]!=-1) if(label[o]<=uv) return;
	if(ul<=l&&ur>=r){
		label[o] = uv;
		if(r>l){
			int m = (l+r)/2;
			dfs(o*2,l,m);
			dfs(o*2+1,m+1,r);
		}
	}else{
		int m = (l+r)/2;
		if(ul<=m) update(o*2,l,m);
		if(ur>m) update(o*2+1,m+1,r);
	}
	maintain(o,l,r);
}


int ql,qr;
long long re;
void query(int o,int l,int r,int val){
	if(ql<=l&&qr>=r){
		re+=sum[o];
		if(label[o]==-1) re+=1LL*val*cnt[o];
	}else{
		int m = (l+r)/2;
		if(label[o]!=-1) val = label[o];
		if(ql<=m) query(o*2,l,m,val);
		if(qr>m) query(o*2+1,m+1,r,val);
	}
}

void querym(int o,int l,int r,int val){
	if(ql<=l&&qr>=r){
		if(label[o] == -1) re = max(re,1LL*val);
		else re = max(re,1LL*label[o]);
	}else{
		int m = (l+r)/2;
		if(label[o]!=-1) val = label[o];
		if(ql<=m) querym(o*2,l,m,val);
		if(qr>m) querym(o*2+1,m+1,r,val);
	}
}


int main(){

	cnt.assign(4000010,0);
	label = maxi = lm = cnt;
	sum.assign(4000010,0);
	int test;
	scanf("%d",&test);
	while(test--){
		int n,q;
		scanf("%d%d",&n,&q);
		a.assign(n,0);
		for(int i=0;i<n;i++) scanf("%d",&a[i]);
		build(1,0,n-1);
		while(q--){
			int t;
			scanf("%d",&t);
			if(t==0){
				scanf("%d%d%d",&ul,&ur,&uv);

				ul--;
				ur--;
				update(1,0,n-1);
			}
			if(t==1){
				scanf("%d%d",&ql,&qr);
				ql--;
				qr--;
				re = 0;
				querym(1,0,n-1,large);
				printf("%lld\n",re);
			}
			if(t==2){
				scanf("%d%d",&ql,&qr);
				ql--;qr--;
				re = 0;
				query(1,0,n-1,large);
				printf("%lld\n",re);
			}
		}
	}


    return 0;
}
