#ifndef _PCVP_DEFINITIONS_
#define _PCVP_DEFINITIONS_

#include "macros.h"

const int __EXHAUSTED_CALLS = 1;

/****************************************** Some Definitions ********************************************************/

struct PCVPInstance {
	int vertices, limiteDePassageiros;
	vector< vector<ll> > cost;
	vector< pair<ll, int> > listaDePassageiros;
};


/****************************************** Basic Stuff *************************************************************/

void __DEBUG_SOLUTION (vector<int> caminho, vector<int> caronas) {
	cerr << "Caminho: 0 "; FORR (i, sz(caminho)) cerr << caminho[i] << " "; cerr << endl;
	cerr << "Caronas: "; FORR (i, sz(caronas)) cerr << caronas[i] << " "; cerr << endl;
}

int __DEBUG_MODE = 0;

bool satisfazCapacidadesETarifas (const PCVPInstance &ins, const vector<int> & caminho, const vector<int> &caronas) {
	using namespace boost::multiprecision;
	
	//Preenche um vetor com o caminho
	vector<int> rota (ins.vertices+1, 0);		
	FORR (i, ins.vertices-1) rota[i+1] = caminho[i];
	//Pega a ordem de cada vertice na rota
	vector<int> posicao (ins.vertices, 0);
	FORR (i, ins.vertices-1) posicao[caminho[i]] = i+1;
	//Verifica se cada vértice é visitido uma única vez.
	FOR (i, 1, ins.vertices) {
		if (posicao[i]==0) {
			cerr << "O trajeto não é um ciclo válido, pois o vértice " << i << " não foi visitado." << endl;
			assert (false);
			return false;
		}
	}
	//armazena a quantidade de passageiros desembarca em cada vértice, uso: saiDoCarro[idDoVertice].
	vector< int > saiDoCarro (ins.vertices, 0);
	
	int capacidadeAtual = 0;
	vector< cpp_rational > acumulado (ins.vertices+1, 0);
	FOR (i, 1, ins.vertices+1) {
		int u = rota[i-1], v = ins.listaDePassageiros[rota[i-1]].second;
		capacidadeAtual -= saiDoCarro[u];
		if (caronas[i-1]) capacidadeAtual++, saiDoCarro[v]++;
		if (capacidadeAtual>ins.limiteDePassageiros) {
			if (__DEBUG_MODE) cerr << "capacidade extrapolada." << endl;
			return false;
		}
		acumulado[i] = acumulado[i-1] + ins.cost[rota[i-1]][rota[i]] / (1 + capacidadeAtual);
	}
	
	FORR (i, ins.vertices) {
		if (!caronas[i]) continue;
		int partida = rota[i], destino = ins.listaDePassageiros[rota[i]].second;
		int left = partida?posicao[partida]:0, right = destino?posicao[destino]:ins.vertices;
		ll podePagar = ins.listaDePassageiros[partida].first;
		if (right<=left) {
			if (__DEBUG_MODE) cerr << "passageiro na pos(" << i << ") tentou voltar para vértice anterior." << endl;
			return false;
		}
		if (acumulado[right] - acumulado[left] > podePagar) { 
			if (__DEBUG_MODE) {
				cerr << "estourou a tarifa." << endl;
				cerr << i << " -> " << acumulado[right] - acumulado[left] << " " << podePagar << endl;
			}
			return false; 
		}
	}
	return true;
}

int __chamadas_funcao_objetivo = -1;
clock_t __begin_time;

vector<int> __melhor_caminho;
vector<int> __melhor_caronas;
double __melhor_custo;

//Supondo que o cromossomo é consistente
double calculaCusto (const PCVPInstance &ins, const vector<int> & caminho, const vector<int> &caronas) {
	//double tempogasto = 0.;
	if (__chamadas_funcao_objetivo > 0) {
		__chamadas_funcao_objetivo--;
		//tempogasto = (double) ( clock () - __begin_time ) /  CLOCKS_PER_SEC;
	}
	
	//Lança exceção, caso ultrapasse o limite de chamadas a função objetivo.
	//if (tempogasto > 3000. || __chamadas_funcao_objetivo == 0) throw 1;
	if (__chamadas_funcao_objetivo == 0) throw __EXHAUSTED_CALLS;
	
	
	//Preenche um vetor com o caminho
	vector<int> rota (ins.vertices+1, 0);		
	FORR (i, ins.vertices-1) rota[i+1] = caminho[i];
	//Pega a ordem de cada vertice na rota
	vector<int> posicao (ins.vertices, 0);
	FORR (i, ins.vertices-1) posicao[caminho[i]] = i+1;
	//saiDoCarro[idDoVertice]
	vector< int > saiDoCarro (ins.vertices, 0);
	vector<double> acumulado (ins.vertices+1, 0.);

	int capacidadeAtual = 0;
	FOR (i, 1, ins.vertices+1) {
		int u = rota[i-1], v = ins.listaDePassageiros[rota[i-1]].second;
		capacidadeAtual -= saiDoCarro[u];
		if (caronas[i-1]) capacidadeAtual++, saiDoCarro[v]++;
		acumulado[i] = acumulado[i-1] + (ins.cost[rota[i-1]][rota[i]] / (double) (1 + capacidadeAtual));				
	}

	//Mantêm sempre a melhor solução.
	if (__melhor_caminho.empty() || __melhor_custo > acumulado[ins.vertices]){
		__melhor_caminho = caminho;
		__melhor_caronas = caronas;
		__melhor_custo = acumulado[ins.vertices];
	}
	return acumulado[ins.vertices];
}

void inicializa_funcao_calcula_custo (int limite) {
	__begin_time = clock();
	__chamadas_funcao_objetivo = limite;
	__melhor_caminho = __melhor_caronas = vector<int> ();
}


/************************************************* Repair *****************************************************/

//Retira os passageiros em que a tarifa é violada
//A remoção é feita da esquerda para a direita, equanto houver mudança
void removePorTarifa (const PCVPInstance &ins, vector<int> & caminho, vector<int> &caronas) {
	using namespace boost::multiprecision;

	bool problema = true;
	//O (N²)
	while (problema) {
		// O (N)
		problema = false;
		//Preenche um vetor com o caminho
		vector<int> rota (ins.vertices+1, 0);		
		FORR (i, ins.vertices-1) rota[i+1] = caminho[i];
		//Pega a ordem de cada vertice na rota
		vector<int> posicao (ins.vertices, 0);
		FORR (i, ins.vertices-1) posicao[caminho[i]] = i+1;
		//saiDoCarro[idDoVertice]
		vector<int> saiDoCarro (ins.vertices, 0);
		
		int capacidadeAtual = 0;

		vector< cpp_rational > acumulado (ins.vertices+1, 0);
		FOR (i, 1, ins.vertices+1) {
			int u = rota[i-1], v = ins.listaDePassageiros[rota[i-1]].second;
			capacidadeAtual -= saiDoCarro[u];
			if (caronas[i-1]) capacidadeAtual++, saiDoCarro[v]++;
			//acumulado[i] = acumulado[i-1] + (ins.cost[rota[i-1]][rota[i]] / (double) (1 + capacidadeAtual));				
			acumulado[i] = acumulado[i-1] + ins.cost[rota[i-1]][rota[i]] / (1 + capacidadeAtual);
		}

		FORR (i, ins.vertices) {
			if (!caronas[i]) continue;
			int partida = rota[i], destino = ins.listaDePassageiros[rota[i]].second;
			int left = partida?posicao[partida]:0, right = destino?posicao[destino]:ins.vertices;
			ll podePagar = ins.listaDePassageiros[partida].first;
			if (acumulado[right] - acumulado[left] > podePagar) caronas[i] = 0, problema = true;
		}
	}
}

//Remove aqueles passageiros em que no melhor caso (carro sempre cheio) não teria dinheiro para pagar.
//Remove os vértices em que o ponto de chegada é anterior ao ponto de partida.
void removePassageirosImpossiveis (const PCVPInstance &ins, vector<int> & caminho, vector<int> &caronas) {
	vector<int> rota (ins.vertices+1, 0);		
	FORR (i, ins.vertices-1) rota[i+1] = caminho[i];
	
	vector<ll> acumulado (ins.vertices+1, 0.);
	FOR (i, 1, ins.vertices+1) acumulado[i] = acumulado[i-1] + ins.cost[rota[i-1]][rota[i]];
	vector<int> posicao (ins.vertices, 0);
	FORR (i, ins.vertices-1) posicao[caminho[i]] = i+1;
	
	//Para cada passageiros
	FORR (k, ins.vertices) {
		if (!caronas[k]) continue;
		int partida = rota[k], destino = ins.listaDePassageiros[rota[k]].second;
		int left = partida?posicao[partida]:0, right = destino?posicao[destino]:ins.vertices;
		ll podePagar = ins.listaDePassageiros[partida].first;
		ll melhorCorrida = acumulado[right]-acumulado[left];
		if (melhorCorrida > podePagar * (1 + ins.limiteDePassageiros) || right<=left) caronas[k] = 0; 
	}
}

//Retira os passageiros em que sua adição no trajeto acarreta em violção da capacidade veicular.
//Os passageiros são analizados pela ordem de embarque
void preenchePorCapacidade (const PCVPInstance &ins, vector<int> & caminho, vector<int> &caronas) {
	vector<int> rota (ins.vertices+1, 0);		
	FORR (i, ins.vertices-1) rota[i+1] = caminho[i];

	//saiDoCarro[idDoVertice]
	vector< int > saiDoCarro (ins.vertices, 0);
	int capacidadeAtual = 0;
	FORR (i, ins.vertices) {
		int u = rota[i], v = ins.listaDePassageiros[rota[i]].second;
		capacidadeAtual -= saiDoCarro[u];
		if (!caronas[i]) continue;
		if (capacidadeAtual>=ins.limiteDePassageiros) {
			caronas[i] = 0;
		} else {
			capacidadeAtual++;
			saiDoCarro[v]++;
		}
	}
}

void tentaMelhorar (const PCVPInstance &ins, vector<int> & caminho, vector<int> &caronas) {
	//Preenche um vetor com o caminho
	vector<int> rota (ins.vertices+1, 0);		
	FORR (i, ins.vertices-1) rota[i+1] = caminho[i];
	//Pega a ordem de cada vertice na rota
	vector<int> posicao (ins.vertices, 0);
	FORR (i, ins.vertices-1) posicao[caminho[i]] = i+1;
	
	FORR (i, ins.vertices) {
		if (caronas[i]) continue;
		int par = rota[i], des = ins.listaDePassageiros[rota[i]].second;
		int left = par?posicao[par]:0, right = des?posicao[des]:ins.vertices;
		if (right<=left) continue;
		caronas[i] = 1;
		if (!satisfazCapacidadesETarifas (ins, caminho, caronas)) caronas[i] = 0;
	}
}

void reparar(const PCVPInstance &ins, vector<int> & caminho, vector<int> &caronas) {
	removePassageirosImpossiveis (ins, caminho, caronas);
	//removePorTarifa (ins.vertices, ins.cost, ins.listaDePassageiros, ins.limiteDePassageiros, caminho, caronas);
	preenchePorCapacidade (ins, caminho, caronas);
	removePorTarifa (ins, caminho, caronas);
	tentaMelhorar (ins, caminho, caronas);
}

#endif
