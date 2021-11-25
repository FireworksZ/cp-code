/*
 * SA.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: Zhao Huangjie
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<string>
#include<cstring>

using namespace std;
typedef pair<int,int> ii;
string t;
int n;
vector<int> ra,tempra,sa,tempsa,c;

void countingSort(int k){
	int i,sum,maxi=max(400,n);
	c.assign(max(400,n+10),0);
	for(i=0;i<n;i++){
		c[i+k<n?ra[i+k]:0]++;
	}
	for(i=sum=0;i<maxi;i++){
		int temp=c[i];
		c[i]=sum;
		sum+=temp;
	}
	for(i=0;i<n;i++){
		tempsa[c[sa[i]+k < n ? ra[sa[i]+k] : 0]++]=sa[i];
	}
	for(i=0;i<n;i++){
		sa[i]=tempsa[i];
	}
}

void constructSA(){
	ra.assign(max(400,n+10),0);
	tempra=sa=tempsa=c=ra;//vector initialize
	int i,k,r;
	for(i=0;i<n;i++) ra[i]=(int)t[i];
	for(i=0;i<n;i++) sa[i]=i;
	for(k=1;k<n;k=k<<1){
		countingSort(k);
		countingSort(0);
		tempra[sa[0]]=r=0;
		for(i=1;i<n;i++){
			tempra[sa[i]]=(ra[sa[i]]==ra[sa[i-1]]&&ra[sa[i]+k]==ra[sa[i-1]+k]) ? r : ++r;
		}
		for(i=0;i<n;i++){
			ra[i]=tempra[i];
		}
		if(ra[sa[n-1]]==n-1) break;
	}
}

string p;
int m;

ii stringMatching() {
	int lo=0,hi=n-1,mid=lo;
	while(lo<hi){         //calculate inclusive lowerbound
		mid=(lo+hi)/2;
		int res = t.compare(sa[mid],min(n-sa[mid],(int)p.length()),p);
		if(res>=0) hi=mid;
		else lo=mid+1;
	}
	if(t.compare(sa[lo],min(n-sa[lo],(int)p.length()),p)!=0) return ii(-1,-1);
	ii ans;
	ans.first=lo;
	lo=0;
	hi=n-1;
	mid=lo;
	while(lo<hi){      //calculate exclusive upperbound
		mid=(lo+hi)/2;
		int res= t.compare(sa[mid],min(n-sa[mid],(int)p.length()),p);
		if(res>0) hi=mid;
		else lo=mid+1;
	}
	if(t.compare(sa[hi],min(n-sa[hi],(int)p.length()),p)==0) hi++;
	ans.second=hi;
	return ans;  //    [ pos.first, pos.second )
}

//Finding the Longest Common Prefix in O(n)
vector<int> phi,plcp,lcp;
void computeLCP(){
	int i,L;
	phi.assign(n+10,0);
	lcp=plcp=phi;
	phi[sa[0]]=-1;
	for(i=1;i<n;i++){
		phi[sa[i]]=sa[i-1];
	}
	for(i=L=0;i<n;i++){
		if(phi[i]==-1){
			plcp[i]=0;
			continue;
		}
		while(t[i+L]==t[phi[i]+L]) L++;
		plcp[i]=L;
		L=max(L-1,0);
	}
	for(i=0;i<n;i++){
		lcp[i]=plcp[sa[i]];
	}
}
//lcp[i] saves the length of the longest common prefix between sa[i] and sa[i-1]

//Find the Longest Common Substring in O(n)
//1. concatenate two string with different terminator ("#","$")
//2. build suffix array
//3. compute LCP
//4. go through consecutive suffixes in suffix array  (i,i-1)
//5. if two consecutive suffixes belong to different owner (check sa[i]),
//   check LCP array to see the if the maximum LCP found so far can be increased
//   After go through the suffix array we obtain the answer we want

int main(){
	cin>>t;//get the string which need suffix array
	t=t+"$";//add termination indicator
	n=(int)t.length();
	constructSA();
	for(int i=0;i<n;i++){
		cout<<sa[i]<<endl;
	}

	while(cin>>p&&p!="0"){
		ii pos=stringMatching();
		if(pos!=ii(-1,-1)){
			cout<<pos.first<<" "<<pos.second<<endl;
			for(int i=pos.first;i<pos.second;i++){
				cout<<sa[i]<<" "<<t.substr(sa[i],n-sa[i]-1)<<endl;
			}
		}else{
			cout<<"none"<<endl;
		}
	}
	computeLCP();
	for(int i=0;i<n;i++){
		cout<<lcp[i]<<endl;
	}
	return 0;
}


