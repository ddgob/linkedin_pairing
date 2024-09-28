#ifndef FLOWGRAPH_HPP
#define FLOWGRAPH_HPP

#include "Edge.hpp"
#include "OriginalGraph.hpp"

#include <vector>

using namespace std;

class FlowGraph {

    private:

        int numVerts;
        
        //lista de adj que armazena arestas:
        //source->ususario
        //ususario->source
        //ususario->vaga
        //vaga->ususario
        //vaga->sink
        //sink->vaga
        vector<Edge>* flowAdjList;

    public:

        //cria grafocom vertices de 
        //usuario, vaga, source e sink
        FlowGraph(OriginalGraph origGraph);

        //imprime o grafo
        void printFlowGraph();

        //determina o fluxo maximo que 
        //pode ser passado pelo grafo
        int fordFulkerson();

        //DFS para encontrar caminho aumentante
        //no grafo e retorna esse caminho
        vector<Edge*> DFS();

        //componente recursiva da DFS
        bool RecursiveDFS(int currVert, bool* visited, vector<Edge*>& augPath);

        //dado uma aresta e o vertice de origem dessa 
        //aresta, retorna a aresta inversa dessa aresta
        Edge* inverseEdge(Edge* edge, int origVert);

        //imprime um caminho aumentante
        void printAugPath(vector<Edge*> augPath);

};

#endif