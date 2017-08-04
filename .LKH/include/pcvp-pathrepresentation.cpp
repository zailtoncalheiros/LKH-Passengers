#ifndef _PCVP_PATHREPRESENTATION_
#define _PCVP_PATHREPRESENTATION_

#include "macros.h"

struct PathRepresentation {
	vector<int> caminho;
	vector<int> caronas;
	
	PathRepresentation (const vector<int> &x = vector<int> (), const vector<int> &y = vector<int> ()) : caminho (x), caronas (y) { }
};

#endif
