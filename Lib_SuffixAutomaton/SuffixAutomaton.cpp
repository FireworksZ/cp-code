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

struct SuffixAutomaton{
	struct state {
		int len, link;
		bool is_clone;
		map<char,int> next;
		//vector<int> inv_link;

	};
	vector<state> st;
	int sz, last;
	vector<int> dp; //store the size of endpos set
	int n;
	void init(int nn) {
		n = nn;
		st.assign(2*n,state());
		sz = last = 0;
		st[0].len = 0;
		st[0].link = -1;
		++sz;

	}
	void extend (char c) {
		int cur = sz++;
		st[cur].len = st[last].len + 1;
		st[cur].is_clone = false;
		int p;
		for (p=last; p!=-1 && !st[p].next.count(c); p=st[p].link)
			st[p].next[c] = cur;
		if (p == -1)
			st[cur].link = 0;
		else {
			int q = st[p].next[c];
			if (st[p].len + 1 == st[q].len)
				st[cur].link = q;
			else {
				int clone = sz++;
				st[clone].len = st[p].len + 1;
				st[clone].next = st[q].next;
				st[clone].link = st[q].link;
				st[clone].is_clone = true;
				for (; p!=-1 && st[p].next[c]==q; p=st[p].link)
					st[p].next[c] = clone;
				st[q].link = st[cur].link = clone;
			}
		}
		last = cur;
	}
//	void fill_inv_link(){
//		for(int i=1;i<sz;i++){
//			st[st[i].link].inv_link.push_back(i);
//		}
//	}
	void topo(){
		vector<int> c(n+1,0);
		for(int i=0;i<sz;i++) c[st[i].len]++;
		for(int i=1;i<=n;i++) c[i]+=c[i-1];
		vector<int> tp(sz,0);
		for(int i=sz-1;i>=0;i--){
			tp[--c[st[i].len]] = i;
		}
		dp.assign(sz,0);
		for(int i=0;i<sz;i++) if(!st[i].is_clone) dp[i]++;
		for(int i=sz-1;i>=0;i--){
			int u =tp[i];
			if(u!=0) dp[st[u].link]+=dp[u];
		}
	}
};


int main(){

	string s;
	cin>>s;
	SuffixAutomaton SA;
	SA.init((int)s.length());
	for(int i=0;i<(int)s.length();i++){
		SA.extend(s[i]);
	}
	//SA.fill_inv_link();
	SA.topo();

	for(int i=0;i<SA.sz;i++){
		cout<<i<<" "<<SA.st[i].len<<": ";
		for(map<char,int>::iterator it = SA.st[i].next.begin();it!=SA.st[i].next.end();it++){
			cout<<it->first<<" "<<it->second<<" ";
		}
		cout<<endl;
	}



	return 0;

}
