/*
 * test.cpp
 *
 *
 *      Author: Fireworks
 */

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
#include<fstream>
#include<bitset>
#include<cstring>
#include<cstdlib>
#include<complex>
#include<list>
#include<sstream>
#include<cassert>   // assert

using namespace std;

typedef pair<int,int> ii;
typedef pair<int,long long> il;
typedef pair<long long,long long> ll;
typedef pair<ll,int> lli;
typedef pair<long long,int> li;
typedef pair<double,double> dd;
typedef pair<ii,int> iii;
typedef pair<double,int> di;
long long mod = 1000000007LL;
long long base = 10000007;
long long large = 1000000000000000000LL;



struct SuffixTree{
	struct node {
		int l, r, par, link;
		map<char,int> next;

		node (int l=0, int r=0, int par=-1)
			: l(l), r(r), par(par), link(-1) {}
		int len()  {  return r - l;  }
		int &get (char c) {
			if (!next.count(c))  next[c] = -1;
			return next[c];
		}
	};

	struct state {
		int v, pos;
		state (int v = 0, int pos = 0) : v(v), pos(pos)  {}
	};

	string s;
	int n;
	vector<node> t;
	int sz;
	state ptr;

	state go (state st, int l, int r) {
		while (l < r)
			if (st.pos == t[st.v].len()) {
				st = state (t[st.v].get( s[l] ), 0);
				if (st.v == -1)  return st;
			}
			else {
				if (s[ t[st.v].l + st.pos ] != s[l])
					return state (-1, -1);
				if (r-l < t[st.v].len() - st.pos)
					return state (st.v, st.pos + r-l);
				l += t[st.v].len() - st.pos;
				st.pos = t[st.v].len();
			}
		return st;
	}

	int split (state st) {
		if (st.pos == t[st.v].len())
			return st.v;
		if (st.pos == 0)
			return t[st.v].par;
		node v = t[st.v];
		int id = sz++;
		t[id] = node (v.l, v.l+st.pos, v.par);
		t[v.par].get( s[v.l] ) = id;
		t[id].get( s[v.l+st.pos] ) = st.v;
		t[st.v].par = id;
		t[st.v].l += st.pos;
		return id;
	}

	int get_link (int v) {
		if (t[v].link != -1)  return t[v].link;
		if (t[v].par == -1)  return 0;
		int to = get_link (t[v].par);
		return t[v].link = split (go (state(to,t[to].len()), t[v].l + (t[v].par==0), t[v].r));
	}

	void tree_extend (int pos) {
		for(;;) {
			state nptr = go (ptr, pos, pos+1);
			if (nptr.v != -1) {
				ptr = nptr;
				return;
			}

			int mid = split (ptr);
			int leaf = sz++;
			t[leaf] = node (pos, n, mid);
			t[mid].get( s[pos] ) = leaf;

			ptr.v = get_link (mid);
			ptr.pos = t[ptr.v].len();
			if (!mid)  break;
		}
	}

	void build_tree() {
		sz = 1;
		for (int i=0; i<n; ++i)
			tree_extend (i);
	}

	//traverse all node in suffix tree
	void dfs(int u,int d){
		if(t[u].next.size()==0) {
			cout<<s.substr(n-d,d)<<endl;
		}
		for(map<char,int>::iterator it = t[u].next.begin();it!=t[u].next.end();it++){
			int v = it->second;
			dfs(v,d+t[v].len());
		}
	}

	void init(string ss){
		s = ss+"$";
		n = (int)s.length();
		ptr = state(0,0);
		t.assign(2*n,node());
		build_tree();
	}

};

//remember to add '$' at the end of the string
int main(){
	string s;
	cin>>s;
	SuffixTree ST;
	ST.init(s);
	ST.dfs(0,0);

	return 0;
}
