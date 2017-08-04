#ifndef _IO_ROUTINES_
#define _IO_ROUTINES_

#include "macros.h"
#include "pcvp-definitions.cpp"
#include "pcvp-pathrepresentation.cpp"

//Ler um valor decimal com (maxprecision) casas decimais e transforma-o em um long long, atribuindo-o a val.
void ler_valor (ll &val, int maxprecision = 2) {
	string s;
	cin>>s;
	FORR (i, sz(s)) if (s[i] == '.') s[i] = ' ';
	stringstream ss;
	ss << s;
	ss >> val;
	//val *= 100;
	FORR (i, maxprecision) val *= 10;
	string t;
	if (ss>>t) {
		if (sz(t)>maxprecision) assert(false);
		while (sz(t)<maxprecision) t += '0';
		stringstream ss2;
		ss2 << t;
		ll w; ss2>>w;
		val += w;
	}
}

//Ler o formato de entrada das instâncias do PCV com caronas.
bool ler_entrada (PCVPInstance &ins) {
	int P;
	if (cin>>ins.vertices>>P>>ins.limiteDePassageiros) {
		ins.cost.assign (ins.vertices, vector<ll> (ins.vertices, 0));
		FORR (i, ins.vertices) {
			FORR (j, ins.vertices) {
				ler_valor (ins.cost[i][j]);
			}
		}
		
		//Lendo os limites de passageiros (não conta com o motorista) de cada carro.
		//FORR (i, ins.vertices) ins.listaDePassageiros[i] = pair<ll, int> (-1, 0);
		ins.listaDePassageiros.assign (ins.vertices, pair<ll, int> (-1, 0));
		FORR (i, P) {
			ll v;
			int x, y;
			ler_valor (v);
			cin>>x>>y;
			ins.listaDePassageiros[x] = pair<ll, int> (v, y);
		}
		return true;
	}
	return false;
}

void print_results (const PCVPInstance &ins, const vector<double> &resultados, const vector<double> &tempos, int teste = 1) {
	map<double, int> mapires;
	map<double, double> mapitempo; 
	double tempomedio = 0.;
	double mediaresultados = 0.;

	cout << fixed;
	int n = sz(resultados);
	FORR (i, n) {
		double aux = resultados[i], tempo_gasto = tempos[i];
		mapires[aux]++;
		mapitempo[aux] += tempo_gasto;
		tempomedio += tempo_gasto;
		mediaresultados += aux;
		cout << aux << " " << tempo_gasto << endl;
	}
	
	mediaresultados /= n;
	tempomedio /= n;
	cout << "Teste #" << teste++ << ": ";
	cout << "(Numero de vertices: " << ins.vertices << ", Limite de passageiros: "  << ins.limiteDePassageiros << ")" << endl << endl;
	cout << "O Melhor resultado foi " << mapires.begin()->first << " e ele foi encontrado " << mapires.begin()->second << " vez(es)." << endl;
	cout << "O Pior resultado foi " << mapires.rbegin()->first << " e ele foi encontrado " << mapires.rbegin()->second << " vez(es)." << endl;
	cout << "A Média dos resultados foi de " << mediaresultados << "." << endl;
	cout << "O tempo médio da melhor resposta foi de " << (mapitempo.begin()->second / mapires.begin()->second) << "." << endl;
	cout << "O tempo médio da pior resposta foi de " << (mapitempo.rbegin()->second / mapires.rbegin()->second) << "." << endl;
	cout << "O tempo médio gasto foi de " << tempomedio << "." << endl << endl;
}

void print_solution (const PathRepresentation &pr) {
	int n = sz(pr.caronas);
	int w = int_length (n-1);
	cout << "Caminho => ";
	cout << setw(w) << 0 << " ";
	FORR (i, n-1) {
		cout << setw(w) << pr.caminho[i] << " "; 
	} cout << endl;
	cout << "Caronas => ";
	FORR (i, n) {
		cout << setw(w) << pr.caronas[i] << " "; 
	} cout << endl; 
}

#endif
