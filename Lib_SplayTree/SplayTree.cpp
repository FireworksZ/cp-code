/*
 * SplayTree.cpp
 *
 *  Created on: 2018Äê1ÔÂ5ÈÕ
 *      Author: Administrator
 */

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
long long mod = 1000000007LL;
long long large = 2000000000000000000LL;


struct SplayTree{
	int nodeCnt;
	int root;
	vector<int> type,parent,size,stack,reversed;
	vector<vector<int> > childs;

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
	void dfs(int u){
		if(u==-1) return;
		pass(u);
		dfs(childs[u][0]);
		//cout<<u<<" ";
		dfs(childs[u][1]);
	}
};

//HDU1890 solution with splay tree

int main(){
	int n;
	SplayTree t;
	t.init(100010);

	while(cin>>n&&n>0){
		//t.init(n);
		t.clear();
		vector<ii> e(n),p;
		for(int i=0;i<n;i++){
			int x;
			scanf("%d",&x);
			x--;
			e[i] = ii(x,i);
		}
		p = e;
		sort(e.begin(),e.end());
		map<ii,int> comp;
		for(int i=0;i<n;i++) comp[e[i]] = i;
		vector<int> b(n,0);
		for(int i=0;i<n;i++){
			b[comp[p[i]]] = t.malloc();
			t.join(t.root,b[comp[p[i]]]);
		}

		for(int i=0;i<n;i++){
			int ans = t.getRank(t.root,b[i]);
			if(i) printf(" ");
			printf("%d",ans+1);
			if(ans!=i) t.reverse(i,ans);
		}
		cout<<endl;
	}

    return 0;
}


