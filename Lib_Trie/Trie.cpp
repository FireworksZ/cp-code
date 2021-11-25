/*
 * Trie.cpp
 *
 *  Created on: Oct 10, 2015
 *      Author: Zhao Huangjie
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<string>
#include<cstring>
using namespace std;

class Trie {
public:
	vector<vector<int> >ch;
	vector<int> val;
	int sz;
	Trie(int maxnode,int sigma_size) {
		sz=1;
		ch.assign(maxnode,vector<int>(sigma_size,-1));
		val.assign(maxnode,0);
	}
	int idx(char c) {return c-'a';}//change 'a' to other for different problem
	void insert(string s,int v){
		int u=0,n=(int)s.length();
		for(int i=0;i<n;i++){
			int c=idx(s[i]);
			if(ch[u][c]==-1){
				val[sz]=0;
				ch[u][c]=sz++;
			}
			u=ch[u][c];
		}
		val[u]=v;
	}
	bool search(string s,int v){ //better set s as global to avoid pass string in function
		int u=0,n=(int)s.length();
		for(int i=0;i<n;i++){
			int c=idx(s[i]);
			if(ch[u][c]==-1){
				return false;
			}
			u=ch[u][c];
		}
		if(val[u]!=v) return false;
		return true;
	}
	void clear(){
		sz=1;
		ch.clear();
		val.clear();
	}
	void assign(int x,int y){
		sz=1;
		ch.assign(x+10,vector<int>(y+10,-1));
		val.assign(x+10,0);
	}
};

int main(){

	return 0;
}


