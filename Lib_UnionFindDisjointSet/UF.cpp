/*
 * UF.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Zhao Huangjie
 */



#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;

class UnionFind {
	private: vector<int> p;
	public:
	UnionFind(int n){
		p.assign(n,0);
		for(int i=0;i<n;i++){
			p[i]=i;
		}
	}

	int findSet(int i){
		return (p[i]==i)?i:(p[i]=findSet(p[i]));
	}

	bool isSameSet(int i,int j){
		return findSet(i)==findSet(j);

	}

	void unionSet(int i,int j){
		if(!isSameSet(i,j)){
			int x=findSet(i),y=findSet(j);
			p[y] = x;
		}
	}
};


int main(){
	//try to modify the class to fit in different problem
	return 0;
}
