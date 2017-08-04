#ifndef _ROULETTE_
#define _ROULETTE_

#include "macros.h"

template <class T>
vector<T> roleta (vector<T> &v, const vector<double> &pesos, int K) {
	int len = sz(v);
	double soma = 0.;
	vector<double> sum (len, 0.);
	FORR (i, len) {
		soma += pesos[i];
		sum[i] = soma;
	}
	vector<T> ans;
	FORR (i, K) {
		double chosen = soma * (1. * rand() / RAND_MAX);
		int lo = 0, hi = len-1;
		while (lo < hi) {
			int mid = (lo+hi)>>1;
			if (chosen > sum[mid]) {
				lo = mid+1;
			} else {
				hi = mid;
			}
		}
		ans.pb (v[lo]);
	}
	return ans;
}

#endif
