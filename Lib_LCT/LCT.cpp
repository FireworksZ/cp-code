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
long long mod = 1000000009LL;
long long large = 2000000000000000000LL;


struct LCT{
	int nodeCnt;
	int root;
	vector<int> type,parent,size,stack,reversed;
	vector<vector<int> > childs;
	//type: 0 left child, 1 right child, 2 root node in the current splay
	//when type[i] == 0 or 1, parent[i] store parent node in splay.
	//when type[i] == 2, parent[i] store the parent node of current splay in LCT.

	void init(int n){
		root = -1;
		nodeCnt = 0;
		type.assign(n,-1);
		parent = size = stack = type;
		reversed.assign(n,0);
		childs.assign(n,vector<int>(2,-1));
	}

	/*SplayTree(int n){
		init(n);
	}*/

	void clear(){
		root = -1;
		nodeCnt = 0;
	}
	int malloc(){
		type[nodeCnt] = 2;
		childs[nodeCnt][0] = childs[nodeCnt][1] = -1;
		parent[nodeCnt] = -1;
		size[nodeCnt] = 1;
		reversed[nodeCnt] = 0;
		return nodeCnt++;
	}

	void update(int x){
		if(x==-1) return;
		size[x] = 1;
		if(childs[x][0]!=-1) size[x]+=size[childs[x][0]];
		if(childs[x][1]!=-1) size[x]+=size[childs[x][1]];
	}

	void pass(int x){
		if(x==-1) return;
		if(reversed[x]){
			swap(childs[x][0],childs[x][1]);
			if(childs[x][0]!=-1){
				type[childs[x][0]] = 0;
				reversed[childs[x][0]] ^= 1;
			}
			if(childs[x][1]!=-1){
				type[childs[x][1]] = 1;
				reversed[childs[x][1]] ^= 1;
			}
			reversed[x] = 0;
		}
	}

	void rotate(int x){
		if(x==-1) return;
		//pass(x);
		int t = type[x],y = parent[x],z = childs[x][1-t];
		type[x] = type[y];
		parent[x] = parent[y];
		if(type[x]!=2){
			childs[parent[x]][type[x]] = x;
		}
		type[y] = 1-t;
		parent[y] = x;
		childs[x][1-t] = y;
		if(z!=-1){
			type[z] = t;
			parent[z] = y;
		}
		childs[y][t] = z;
		update(y);
	}
	void splay(int x){
		if(x==-1) return;
		int stackCnt = 0;
		stack[stackCnt++] = x;
		for(int i=x;type[i]!=2;i = parent[i]){
			stack[stackCnt++] = parent[i];
		}
		for(int i = stackCnt-1;i>=0;i--){
			pass(stack[i]);
		}
		while(type[x]!=2){
			int y = parent[x];
			if(type[x]==type[y]){
				rotate(y);
			}else{
				rotate(x);
			}
			if(type[x] == 2) break;
			rotate(x);
		}
		update(x);
	}
	// rank in [0, size[x] )
	int find(int x,int rank){
		if(x==-1) return -1;
		if(size[x]<=rank) return -1;
		while(true){
			pass(x);
			int temp = 0;
			if(childs[x][0]!=-1) temp = size[childs[x][0]];
			if(temp == rank){
				break;
			}
			if(childs[x][0]!=-1&&rank<size[childs[x][0]]){
				x = childs[x][0];
			}else{
				rank-=1;
				if(childs[x][0]!=-1) rank-=size[childs[x][0]];
				x = childs[x][1];
			}
		}
		return x;
	}
	void split(int &x,int &y,int a){
		if(x==-1){
			y=-1;
			return ;
		}
		y = find(x,a+1);
		if(y==-1) return;
		splay(y);
		x = childs[y][0];
		if(x==-1) return;
		type[x] = 2;
		parent[x] = -1;
		childs[y][0] = -1;
		update(y);

		//cout<<x<<" "<<y<<" "<<a<<endl;

	}

	//includsive [a,b], index from 0
	void split3(int &x,int &y,int &z,int a,int b){
		split(x,z,b);
		split(x,y,a-1);
	}

	void join(int &x,int y){
		if(y==-1) return;
		if(x==-1) {
			x = y;
			return ;
		}
		x = find(x,size[x]-1);
		splay(x);
		childs[x][1] = y;
		type[y] = 1;
		parent[y] = x;
		update(x);
	}
	void join3(int &x,int y,int z){
		join(y,z);
		join(x,y);
	}
	int getRoot(int x){
		if(x==-1) return -1;
		while(parent[x]!=-1) x = parent[x];
		return x;
	}
	int getRank(int &x,int y){
		if(y==-1) return -1;
		assert(x==getRoot(y));
		splay(y);
		x = y;
		if(childs[x][0]==-1) return 0;
		return size[childs[x][0]];
	}
	void reverse(int a,int b){
		int x,y;
		split3(root,x,y,a,b);
		reversed[x]^=1;
		join3(root,x,y);
	}
	void access(int x){
		int y = -1;
		do{
			splay(x);
			int z = childs[x][1];
			if(z!=-1){
				parent[z] = x;
				type[z] = 2;
				childs[x][1] = -1;
			}
			if(y!=-1){
				childs[x][1] = y;
				type[y] = 1;
				parent[y] = x;
			}
			update(x);
			y = x;
			x = parent[x];
		}while(x!=-1);

	}
	int findLCTRoot(int x){
		access(x);
		splay(x);
		return find(x,0);
	}
	void cut(int x){
		access(x);
		splay(x);
		int y = childs[x][0];
		if(y!=-1){
			type[y] = 2;
			parent[y] = -1;
			childs[x][0] = -1;
			update(x);
			parent[y] = parent[x];
			parent[x] = -1;
		}
	}
	void mroot(int x){
		access(x);
		splay(x);
		if(childs[x][0]!=-1) reversed[x] ^= 1;
	}
	void cut(int x,int y){
		mroot(x);
		cut(y);
	}
	void link(int x,int y){
		mroot(x);
		if(x==findLCTRoot(y)) return;
		parent[x] = y;
	}
	bool query(int x,int y){
		mroot(x);
		access(y);
		return x==findLCTRoot(y);
	}
	void dfs(int u){
		if(u==-1) return;
		pass(u);
		dfs(childs[u][0]);
		//cout<<u<<" ";
		dfs(childs[u][1]);
	}
};

//link cut tree
//bzoj2049 solution
int main(){

	int n,m;
	cin>>n>>m;
	LCT lct;
	lct.init(n);
	for(int i=0;i<n;i++) lct.malloc();
	char opt[50];
	int u,v;
	while(m--){
		scanf("%s%d%d",opt,&u,&v);
		u--;v--;
		if(opt[0]=='Q'){
			if(lct.query(u,v)) printf("Yes\n");
			else printf("No\n");
		}
		//fflush(stdout);
		if(opt[0]=='C') lct.link(u,v);
		if(opt[0]=='D') lct.cut(u,v);

	}


	return 0;
}
