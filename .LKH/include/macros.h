#ifndef _MACROS_H
#define _MACROS_H

#include <bits/stdc++.h> 
#include <boost/multiprecision/cpp_int.hpp>

#define FOR(i, a, b) for(int i = a; i < b; ++i) 
#define FORR(i, n) FOR(i, 0, n) 
#define FOREACH(i, x) for(typeof(x.begin()) i = x.begin(); i != x.end(); i++)
#define sz(v) int((v).size()) 
#define pb push_back 
#define all(e) (e).begin(), (e).end()
#define LSOne(S) (S & (-S)) // bit menos significativo

using namespace std;

#define dd pair<double, double>
#define ii pair<int,int>
#define ll long long

const int MAXN = 1005, MAXC = 100;
const ll MOD = 1000000007LL;

int int_length (int n) {
	int len = (n ? 0: 1);
	for (;n; n/=10, len++);
	return len;
}

#endif
