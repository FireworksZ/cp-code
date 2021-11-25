/*
 * ET.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: SCE15-0683
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<list>

using namespace std;
typedef pair<int,int> ii;
vector<vector<ii> > adj;

//note that cyc stores the reverse order of Euler path
list<ii> cyc;
void EulerPath(list<ii>::iterator i,int u){
	for(int j=0;j<(int)adj[u].size();j++){
		ii& v = adj[u][j];
		if(v.second){
			v.second=0;
			for(int k=0;k<(int)adj[v.first].size();k++){
				ii& uu = adj[v.first][k];
				if(uu.first==u&&uu.second){
					uu.second=0;
					break;
				}
			}
			EulerPath(cyc.insert(i,ii(u,v.first)),v.first);
		}
	}
}

int main(){
	cyc.clear();
	//build adj list
	int start; //pick the start point;
	EulerPath(cyc.begin(),start);


	return 0;
}


