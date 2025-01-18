/*
 * AC.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: Zhao Huangjie
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<string>
#include<cstring>
#include<queue>
using namespace std;

class AC {
public:
	int sz;
	vector<int> val,f,last;
	vector<vector<int> > ch;
	int sigma_size;
	AC(int maxnode,int _sigma_size) {
		sz=1;
		ch.assign(maxnode,vector<int>(_sigma_size,-1));
		val.assign(maxnode,0);
		f.assign(maxnode,0);
		last.assign(maxnode,0);
		sigma_size=_sigma_size;
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

	void getFail(){
		queue<int> q;
		f[0]=0;
		for(int c=0;c<sigma_size;c++){
			int u=ch[0][c];
			if(u!=-1){
				f[u]=0;
				q.push(u);
				last[u]=0;
			}
		}
		while(!q.empty()){
			int r=q.front();
			q.pop();
			for(int c=0;c<sigma_size;c++){
				int u=ch[r][c];
				if(u==-1) continue;
				q.push(u);
				int v=f[r];
				while(v&&ch[v][c]==-1) v=f[v];
				f[u]=ch[v][c];
				if(f[u]==-1) f[u]=0;
				last[u]= val[f[u]] ? f[u]:last[f[u]];
			}
		}
	}
	void find(string t){ //better set as global variable;
		int n=(int)t.length();
		int j=0;
		for(int i=0;i<n;i++){
			int c=idx(t[i]);
            while(j && ch[j][c]==-1) j = f[j];
			j=ch[j][c];
			if(j==-1) j=0;
			if(val[j]){
				print(i,j);
			}else{
				if(last[j]){
					print(i,last[j]);
				}
			}
		}
	}
	void print(int i,int j){
		if(j){
			printf("%d: %d\n",i,val[j]); //the string val[j] end at i
			print(i,last[j]);
		}
	}
	void clear(){
		sz=1;
		ch.clear();
		val.clear();
		f.clear();
		last.clear();
	}
	void assign(int x,int y){
		sz=1;
		ch.assign(x+10,vector<int>(y+10,-1));
		val.assign(x+10,0);
		f.assign(x+10,0);
		last.assign(x+10,0);
	}
};


int main(){

	return 0;
}


