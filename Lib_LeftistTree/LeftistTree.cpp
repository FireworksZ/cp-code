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



const int maxn = 100000;
struct LeftistHeap{
	int tot;
	vector<int> l,r,d,v;

	LeftistHeap(int sz){
		tot = 0;
		l.assign(sz,-1);
		r = l;
		d.assign(sz,0);
		v.assign(sz,0);
	}

	int merge(int x,int y){
		if(x==-1) return y;
		if(y==-1) return x;
		if(v[x]>v[y]) swap(x,y); //minimal key on the top
		r[x] = merge(r[x],y);
		if(l[x]==-1||(r[x]!=-1&&d[l[x]]<d[r[x]])){
			swap(l[x],r[x]);
		}
		d[x] = d[r[x]]+1;
		return x;
	}

	int init(int x){
		v[tot] = x;
		l[tot] = r[tot] = -1;
		d[tot] = 0;
		return tot++;
	}

	int insert(int x,int y){
		return merge(x,init(y));
	}

	int top(int x){
		assert(x!=-1);
		return v[x];
	}

	int pop(int x){
		assert(x!=-1);
		return merge(l[x],r[x]);
	}
};


int main(){

	LeftistHeap lh(1000000);
	int p = -1;
	int t,v;
	while(cin>>t){
		if(t==0){
			cin>>v;
			p = lh.insert(p,v);
		}else{
			if(t==1) cout<<lh.top(p)<<endl;
			if(t==2) p = lh.pop(p);
		}
	}


    return 0;
}
