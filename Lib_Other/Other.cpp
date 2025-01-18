#include<iostream>

using namespace std;

/*
 * Other.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Zhao Huangjie
 */

// Combination Number:
// C(n,m) = C(n-1,m-1)+C(n-1,m)
// where C(n,0) = C(n,n) = 1


// Derangement: A permutation of elements of a set such that
//             none of the elements appear in their original
//             position
// der(n) = (n-1)*(der(n-1)+der(n-2))  der(0)=1  der(1)=0

// Euler's Formula: V - E + F = 2

// Catalan Number : f(n) = f(n-1) * (4*n-2)/(n+1); f(0)=1;

// Number of spanning tree in an undirected graph with n nodes and adj matix g[][];
// g[i][j] = number of edges between i and j.
// if there is no edges between i and j, than g[i][j] = 0.
// note that g need to be symmetric.
// This allows multiple edges between two vertices i and j.
// 1. Compute Kirchhoff's matrix l:
//    	l[i][j] = degree of i if i==j
//		l[i][j] = -g[i][j] otherwise
// 2. erase row r and column r (pick any r from 0 to n-1)
// 3. compute the determinant of l which is the number of spanning trees for g.

//First and second kind of Stirling number
//1. s[n][k] = (n-1)*s[n-1][k] + s[n-1][k-1];
//2. s[n][k] = k*s[n-1][k]+s[n-1][k-1];
//3. use FFT to calculate Stirling number

//#include <ext/pb_ds/assoc_container.hpp>
//
//#include <ext/pb_ds/tree_policy.hpp>
//
//using namespace std;
//
//using namespace __gnu_pbds;
//
//#define ordered_set tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>
//Then can use st.order_of_key(val) to find order of val in ordered set st.

istream &operator>>(istream &is, __int128 &x) {
	string s;
	is >> s;
	bool neg = false;
	if(s.size() > 0 && s[0] == '-') {
		neg = true; s = s.substr(1);
	}
	x = 0;
	for (char t: s) x = x * 10 + t - '0';
	if (neg) x = -x;
	return is;
}

ostream &operator<<(ostream &os, __int128 x) {
	if (x < 0) return os << "-" << (-x);
	else if (x == 0) return os << "0";
	else {
		string s = "";
		while (x > 0) {
			s = static_cast<char>('0' + x % 10) + s;
			x /= 10;
		}
		return os << s;
	}
}

int main(){

}
