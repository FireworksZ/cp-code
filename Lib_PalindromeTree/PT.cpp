/*
 * PT.cpp
 *
 *  Created on: Aug 31, 2016
 *      Author: SCE15-0683
 */




#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <cstring>
#include <cassert>
#include <cmath>
#include <stack>
#include <queue>

using namespace std;

struct PalindromeTree{

	struct node{
		//each node represent a unique substring
		//cnt store number of different substring given this unique substring, after call function calc()
		int len,fail;
		long long cnt;
		int par;
		char ch;
		map<char,int> next;
	};

	string s;
	vector<node> t;

	int suf;

	void init(){
		//odd root 0, even root 1
		node a;
		a.len = -1;
		a.fail = 0;
		a.cnt = 0;
		a.par = -1;
		a.ch = '!';
		t.push_back(a);
		a = node();
		a.len = 0;
		a.fail = 0;
		a.cnt = 0;
		a.par = -1;
		a.ch ='!';
		t.push_back(a);
		suf = 1;
	}

	int get_fail(int x,int l){
		while(l-1-t[x].len<0||s[l-1-t[x].len]!=s[l]){
			x = t[x].fail;
		}
		return x;
	}

	int extend(int x){
		char c = s[x];
		int p = get_fail(suf,x);
		if(t[p].next.find(c)==t[p].next.end()){
			int q = t.size();
			t.push_back(node());
			t[q].cnt = 0;
			t[q].len = t[p].len+2;
			int nq = get_fail(t[p].fail,x);
			if(t[nq].next.find(c)==t[nq].next.end()) nq = 1;
			else nq = t[nq].next[c];
			t[q].fail = nq;
			t[q].par = p;
			t[q].ch = c;
			t[p].next[c] = q;
		}
		p = t[p].next[c];
		t[p].cnt++;
		suf = p;
		return suf;
	}
	void cal(){

		for(int i=(int)t.size()-1;i>=0;i--){
			if(t[i].fail!=i) t[t[i].fail].cnt+=t[i].cnt;
		}
	}
	void build(){
		//call init() first
		for(int i=0;i<(int)s.length();i++){
			extend(i);
		}
		cal();
	}
};


int main() {


	return 0;
}
