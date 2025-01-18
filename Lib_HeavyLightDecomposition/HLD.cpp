/*
 * HLD.cpp
 *
 *  Created on: Jul 2, 2016
 *      Author: Zhao Huangjie
 */
#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;
typedef pair<int,int> ii;

vector<vector<int> > adj; //for undirected graph
vector<vector<int> > chains; //record the chain
vector<int> pa;// record the parent node
vector<ii> pos;//pos[i].first is the chain that node i belong to, pos[i].second is the position of node i in the chain
vector<int> dp;//to calculate the size of subtree

int f(int u,int parent){
	int ans = 0;
	for(int j=0;j<(int)adj[u].size();j++){
		int v= adj[u][j];
		if(v!=parent){
			ans+=f(v,u);
		}
	}
	return dp[u]=ans+1;
}

void hld(int u,int c,int parent){
    vector<ii> temp;
    for(int j=0;j<(int)adj[u].size();j++){
        int v=adj[u][j];
        if(v!=parent){
        	temp.push_back(ii(dp[v],v)); //dp[u] record the size of subtree (by dfs)
        	pa[v]=u;
        }
    }
    chains[c].push_back(u);
    pos[u]=ii(c,(int)chains[c].size()-1);

    if(temp.size()>0){
        sort(temp.begin(),temp.end());
        ii v = temp.back();
        hld(v.second,c,u);
        for(int i=0;i<(int)temp.size()-1;i++){
            chains.push_back(vector<int>(0,0));
            int nc = (int)chains.size()-1;
            hld(temp[i].second,nc,u);
        }
    }
}

int main(){


	return 0;
}
