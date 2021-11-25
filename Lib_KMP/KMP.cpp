/*
 * KMP.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Zhao Huangjie
 */


#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<cstring>
#include<string>
using namespace std;
//KMP (return all indexes of t that the following substring matches p;
string t,p;
vector<int> b;

//Call this before calling kmpSearch()
void kmpPreprocess(){
	b.assign((int)p.length()+1,0);
	int i=0,j=-1;
	b[0]=-1;
	while(i<(int)p.length()){
		while(j>=0&&p[i]!=p[j]) j=b[j];
		i++;
		j++;
		b[i]=j;
	}
}

void kmpSearch(){
	int i=0,j=0;
	while(i<(int)t.length()){
		while(j>=0&&t[i]!=p[j]) j=b[j];
		i++;
		j++;
		if(j==(int)p.length()){
			printf("p is found at index %d in T\n",i-j); //this line produce the index
			j=b[j];
		}
	}
}

int main(){

	return 0;
}

