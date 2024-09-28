#include "OriginalGraph.hpp"
#include "FlowGraph.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    
    int numUsers, numJobs, numUqualifyForJ;

    //le numero de ususarios, vagas e 
    //pares de (usuario, vaga) compativeis
    cin >> numUsers >> numJobs >> numUqualifyForJ;

    //inicia grafo bipartido para modelagem em grafo
    OriginalGraph bipartGraph = OriginalGraph(numUsers, numJobs);

    string user, job;

    //contador para numero de 
    //usuarios ja adicionados
    int userCount = 0;
    //contador para numero de 
    //vagas ja adicionados
    int jobCount = 0;

    //itera por todos os pares de 
    //(usuario, vaga) compativeis
    for (int i = 0; i < numUqualifyForJ; i++) {

        //le usuario e vaga
        cin >> user >> job;

        //adiciona aresta usuario->vaga e aresta de 
        //arrependimento vaga->usuario no grafo bipartido
        bipartGraph.addNewEdge(user, job, userCount, jobCount);

    }

    //gera o grafo bipartido original + source 
    //e sink e as arestas correspondentes
    FlowGraph FGraph = FlowGraph(bipartGraph);

    cout << "Guloso: " << bipartGraph.greedy() << endl;

    cout << "Exato: " << FGraph.fordFulkerson() << endl;

    return 0;
}
