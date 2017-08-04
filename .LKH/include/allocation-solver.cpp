#ifndef _ALLOCATION_SOLVER_
#define _ALLOCATION_SOLVER_

#include "macros.h"
#include "pcvp-definitions.cpp"
#include "gurobi_c++.h"

/***********************************************************************************************************/

namespace allocation_solver {
	string itos(int i) {stringstream s; s << i; return s.str(); }

	const int MAXL = 600;

	double arestas[MAXL];
	pair<double, int> listaDePassageiros[MAXL];
	GRBVar v[MAXL];
	GRBVar z[MAXL][MAXL], q[MAXL][MAXL];
	vector<int> PnC[MAXL];
	
	int n, k;

	ll get_aresta (int i) {
		return arestas[i-1];
	}

	ll get_tarifa (int l) {
		return listaDePassageiros[l-1].first;
	}

	vector<int> solve_model () {
		GRBEnv env = GRBEnv();	
		GRBModel model = GRBModel(env);
		model.getEnv().set(GRB_IntParam_OutputFlag, 0);
		//model.getEnv().set("OutputFlag", "0");
		//model.getEnv().set("Quad", "1");
		//model.getEnv().set(GRB_IntParam_OutputFlag, 0);
		//Inicializando as variáveis
		FOR (i, 1, n+1) {
			v[i] = model.addVar(0., 1., 0.0, GRB_BINARY, "v_"+itos(i));
		}
		FOR (i, 1, n+1) {
			FOR (j, 2, k+2) {
				z[i][j] = model.addVar (0., 1., 0., GRB_CONTINUOUS, "z_"+itos(i));
			}
		}
		FOR (l, 1, n+1) {
			FOR (i, 1, n+1) {
				q[l][i] = model.addVar (0., GRB_INFINITY, 0., GRB_CONTINUOUS, "q_"+itos(l)+"_"+itos(i));
			}
		}
		
		//Função-objetivo
		GRBLinExpr exp = 0;
		FOR (i, 1, n+1) {
			exp += get_aresta (i);
			FOR (j, 2, k+2) {
				exp -= z[i][j] * get_aresta (i) / (j * (j-1));
			}
		}
	
		model.update();
		model.setObjective (exp, GRB_MINIMIZE);
	
		//Restrições
		FOR (i, 1, n+1) {
			exp = 0;
			FOR (j, 0, sz(PnC[i])) {
				exp += v[PnC[i][j]];
			}
			FOR (j, 2, k+2) {
				exp -= z[i][j];
			}
			model.addConstr (exp==0);
		}
	
		FOR (l, 1, n+1) {
			exp = 0;
			FOR (i, 1, n+1) {
				exp += q[l][i];
			}
			model.addConstr (exp<=get_tarifa(l));
		}
	
		FOR (i, 1, n+1) {
			FORR(j, sz(PnC[i])){
				int l = PnC[i][j];
				exp = get_aresta (i) * v[l];
				FOR (w, 2, k+2) {
					exp -= get_aresta (i) / (w * (w-1)) * z[i][w] ;
				}
				model.addConstr (exp <= q[l][i]);
			}
		}
		
		model.optimize();
	
		//cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		vector<int> ans;
		FOR (i, 1, n+1) {
			//cout << v[i].get(GRB_StringAttr_VarName) << " := " << v[i].get(GRB_DoubleAttr_X) << endl;
			ans.pb (round(v[i].get(GRB_DoubleAttr_X)));
		}
		return ans;
		
	}
	
	vector<int> solve (const PCVPInstance &ins, vector<int> &caminho) {
		n = ins.vertices, k = ins.limiteDePassageiros;
		arestas[0] = ins.cost[0][caminho[0]];
		FOR (i, 1, n-1) arestas[i] = ins.cost[caminho[i-1]][caminho[i]];
		arestas[n-1] = ins.cost[caminho[n-2]][0];
		
		
		vector<int> ordem (n+1, 0);
		FORR (i, n-1) ordem[caminho[i]] = i+1;
		FORR (i, n) {
			int v = i?caminho[i-1]:0;
			int w = ordem[ins.listaDePassageiros[v].second];
			if (w==0) w = n;
			listaDePassageiros[i] = make_pair (ins.listaDePassageiros[v].first, w);
		}
		
		FORR (i, n) PnC[i+1].clear();
		FORR (i, n) {
			FOR (j, i, listaDePassageiros[i].second) {
				PnC[j+1].pb (i+1);
			}
		}
		
		vector<int> ans = solve_model ();
		
		FORR (i, n) if (listaDePassageiros[i].second <= i) ans[i] = 0;
		
		if (!satisfazCapacidadesETarifas(ins, caminho, ans)) {
			cerr << "atribuicoes de caronas invalidas:" << endl;
			__DEBUG_SOLUTION(caminho, ans);
		}
		
		return ans;
	}
	
	vector<int> read_and_solve () {
		cin>>n>>k;
	   	FORR (i, n) cin>>arestas[i];
	   	FORR (i, n) {
	   		cin>>listaDePassageiros[i].first>>listaDePassageiros[i].second;
	   		cout << listaDePassageiros[i].first << endl;
	   		if (listaDePassageiros[i].second==0) listaDePassageiros[i].first = n+1;
	   		if (listaDePassageiros[i].second<i) continue;
	   		FOR (j, i, listaDePassageiros[i].second) {
	   			//cout << i << " " << j << " " << listaDePassageiros[i].second << endl;
	   			PnC[j+1].pb (i+1);	
	   		}
	   	}
	   
	   	FOR (i, 1, n+1) {
	   		cout << i << " -> ";
	   		FORR (j, sz(PnC[i])) {
	   			cout << PnC[i][j] << " ";
	   		} cout << endl;
	   	}
	   	
	   	return solve_model ();
	}

};

#endif
