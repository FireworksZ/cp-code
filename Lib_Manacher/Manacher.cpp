/*
 * Manacher.cpp
 *
 *  Created on: Oct 10, 2016
 *      Author: Zhao Huangjie
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;

//remember to change the string abaca to #a#b#a#c#a#
vector<int> manacher(string s){
	vector<int> p;
	int n = (int)s.length();
	p.assign(n,0);
	int mx=0;
	int id=0;
	for(int i=0;i<n;i++){
		p[i]=mx>i?min(p[2*id-i],mx-i):1;
		while(i+p[i]<n&&i>=p[i]&&s[i+p[i]]==s[i-p[i]]) p[i]++;
		if(i+p[i]>mx){
			mx=i+p[i];
			id=i;
		}
	}
	return p;
}
int main(){

	return 0;
}


