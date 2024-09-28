#include "Edge.hpp"

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>

using namespace std;

class OriginalGraph {

    private:
    
        int numVerts, numUsers, numJobs;
        
        //armazena os usuarios unicos e mapeia para 
        //seus respectivos indices na lista de adj.
        unordered_map<string, int> mapUsersToIndex;
        //armazena as vagas unicas e mapeia para 
        //seus respectivos indices na lista de adj.
        unordered_map<string, int> mapJobsToIndex;

        //lista de adj que armazena as arestas 
        //usuario->vaga e vaga->usuario do grafo
        vector<Edge> *origAdjList;

        friend class FlowGraph;

    public:

        //constroi grafo
        OriginalGraph(int numUsers_, int numJobs_);

        //adiciona 2 arestas: usuario->vaga 
        //e vaga->usuario no grafo
        void addNewEdge(string user, string job, int& userCount, int& jobCount);

        //imprime grafo
        void printOrigGraph();

        //encontra o numero maximo de pares compativeis 
        //unicos (usuario, vaga) que podem existir
        // seguindo um metodo guloso (podendo nao ser exato)
        int greedy();

};

#endif