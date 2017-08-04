#ifndef _ROUTE_HEURISTICS_
#define _ROUTE_HEURISTICS_

#include "macros.h"
#include "pcvp-definitions.cpp"

vector<int> least_edge_heuristic (PCVPInstance &ins) {
	vector<int> solution;
	vector<int> used_vertices (ins.vertices, 0);
	solution.pb (0);
	FOR (i, 1, ins.vertices) {
		int k = -1;
		FOR (j, 1, ins.vertices) {
			if (used_vertices[j]) continue;
			if (k==-1 || ins.cost[solution.back()][j] < ins.cost[solution.back()][k]) k = j;
		}
		used_vertices[k] = 1;
		solution.push_back (k);
	}
	solution.erase (solution.begin());
	return solution;
}

#endif
