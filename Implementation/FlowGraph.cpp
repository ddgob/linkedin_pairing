#include "FlowGraph.hpp"

FlowGraph::FlowGraph(OriginalGraph origGraph) {

    //numero de vertices do grafo eh 
    //igual ao numero de vertices do 
    //grafo original mais o source e sink
    numVerts = origGraph.numVerts + 2;

    //alocacao de memoria  
    //para armazenar arestas
    flowAdjList = new vector<Edge>[numVerts];

    //itera pelos usuarios
    for (int currUserIndex = 0; currUserIndex < origGraph.numUsers; currUserIndex++) {
        
        //adiciona arestas de usuario a vaga na 
        //lista de arestas de saida do usuario atual
        flowAdjList[currUserIndex] = origGraph.origAdjList[currUserIndex];
        
        //cria aresta do source 
        //ao usuario atual
        Edge currSourceToUserEdge;
        currSourceToUserEdge.destVert = currUserIndex;
        currSourceToUserEdge.capacity = 1;
        currSourceToUserEdge.flow = 0;

        //adiciona aresta, criada anteriormente,   
        //na lista de arestas de saida do source
        flowAdjList[numVerts - 2].push_back(currSourceToUserEdge);

        //cria aresta residual do source ao 
        //usuario atual (ou seja, cria  
        //aresta do usuario atual ao source)
        Edge currResidualSourceToUserEdge;
        currResidualSourceToUserEdge.destVert = numVerts - 2;
        currResidualSourceToUserEdge.capacity = 0;
        currResidualSourceToUserEdge.flow = 0;

        //adiciona aresta, criada anteriormente, na
        //lista de arestas de saida do usuario atual
        flowAdjList[currUserIndex].push_back(currResidualSourceToUserEdge);

    }

    //itera pelas vagas
    for (int currJobIndex = origGraph.numUsers; currJobIndex < origGraph.numVerts; currJobIndex++) {

        //adiciona arestas de residuais usuario a vaga 
        //(ou seja, adiciona aresta de vaga a usuario) 
        //na lista de arestas de saida da vaga atual
        flowAdjList[currJobIndex] = origGraph.origAdjList[currJobIndex];
        
        //cria aresta da 
        //vaga atual ao sink
        Edge currJobToSinkEdge;
        currJobToSinkEdge.destVert = numVerts - 1;
        currJobToSinkEdge.capacity = 1;
        currJobToSinkEdge.flow = 0;

        //adiciona aresta, criada anteriormente,   
        //na lista de arestas de saida da vaga atual
        flowAdjList[currJobIndex].push_back(currJobToSinkEdge);

        //cria aresta residual da vaga
        //atual ao sink (ou seja, cria 
        //aresta do sink a vaga atual)
        Edge currResidualJobToSinkEdge;
        currResidualJobToSinkEdge.destVert = currJobIndex;
        currResidualJobToSinkEdge.capacity = 0;
        currResidualJobToSinkEdge.flow = 0;

        //adiciona aresta, criada anteriormente, 
        //na lista de arestas de saida do sink
        flowAdjList[numVerts - 1].push_back(currResidualJobToSinkEdge);

    }

}

void FlowGraph::printFlowGraph() {

    cout << "---------------------- Start Flow Graph ----------------------" << endl;
    
    //itera pelos vertices
    for (int currVertIndex = 0; currVertIndex < numVerts; currVertIndex++) {

        cout << currVertIndex << " -->   ";
        
        vector<Edge>& currVertNeighbors = flowAdjList[currVertIndex];

        //itera pelas arestas de saida do vertice atual
        for (Edge& currVertNeighbor : currVertNeighbors) {

            //imprime tupla: (vertice destino, capacidade, flow)
            cout << "(D: " << currVertNeighbor.destVert << ", ";
            cout << "C: " << currVertNeighbor.capacity << ", ";
            cout << "F: " << currVertNeighbor.flow << ")   ";

        }

        cout << endl;

    }

    cout << "---------------------- End Flow Graph ----------------------" << endl;

}

int FlowGraph::fordFulkerson() {

    //fluxo maximo = numero maximo de pareamentos
    int maxFlowMatch = 0;

    //armazena o caminho aumentante 
    //atual da DFS de tras pra frente
    vector<Edge*> augPath;
    int numEdgesAugPath;
    
    //enquanto houver caminho aumentante
    while ((numEdgesAugPath = (augPath = DFS()).size()) != 0) {
        
        //armazena o vertice de origem da 
        //aresta atual do caminho aumentante
        int origVert = numVerts - 2;

        //itera pelo caminho aumentante de tras pra frente
        for (Edge* currAugPathEdge : augPath) {

            //passa fluxo 1 pela aresta 
            //do caminho aumentante
            currAugPathEdge->flow = 1;

            //retira fluxo da aresta inversa 
            //da aresta do caminho aumentante
            inverseEdge(currAugPathEdge, origVert)->flow = -1;

            //o vertice destino da aresta atual 
            //passa a ser o vertice de origem da 
            //proxima aresta do caminho aumentante
            origVert = currAugPathEdge->destVert;

        }
        
        //incrementa o fluxo passado 
        //no grafo a cada novo  
        //caminho aumentante encontrado
        maxFlowMatch++;

    }

    return maxFlowMatch;

}

vector<Edge*> FlowGraph::DFS() {

    //indica os vertices 
    //que ja foram visitados
    bool* visited = new bool[numVerts];
    //marca todos os vertices 
    //como ainda nao visitados
    for (int vert = 0; vert < numVerts; vert++) {

        visited[vert] = false;

    }

    //armazena o caminho aumentante
    vector<Edge*> augPath;

    //chamada recursiva para encontrar caminho aumentante
    RecursiveDFS(numVerts - 2, visited, augPath);

    delete[] visited;

    return augPath;

}

bool FlowGraph::RecursiveDFS(int currVert, bool* visited, vector<Edge*>& augPath) {

    //retorna verdadeiro se o sink ja tiver 
    //sido "atingido" pelo caminho aumentante
    if (currVert == numVerts - 1) {

        return true;

    }

    //marca vertice atual como visitado
    visited[currVert] = true;

    //armazena as arestas de saida do vertice atual
    vector<Edge>& currVertOutEdges = flowAdjList[currVert];
    
    //itera pelas arestas de saida do vertice atual
    for (Edge& currVertOutEdge : currVertOutEdges) {

        //armazena se a capacidade atual for 
        //maior que zero (pode passar fluxo)
        bool canPassFlow = (currVertOutEdge.capacity - currVertOutEdge.flow) > 0;
        //armazena se o vertice destino da aresta de 
        //saida atual nao tiver sido visitado ainda
        bool neighborNotVisited = !visited[currVertOutEdge.destVert];
        
        //se puder passar fluxo pela aresta 
        //atual e o vertice vizinho (vertice 
        //destino da aresta) n tiver sido visitado
        if (canPassFlow && neighborNotVisited) {

            //adiciona essa aresta ao caminho aumentante
            augPath.push_back(&currVertOutEdge);

            //retorna verdadeiro se o sink ja tiver 
            //sido "atingido" pelo caminho aumentante
            if (RecursiveDFS(currVertOutEdge.destVert, visited, augPath)) {

                return true;

            }

        }

    }

    //se o caminho aumentante 
    //tiver pelo o menos uma aresta
    if (augPath.size() != 0) {

        //elimina a ultima aresta
        //do caminho aumentante
        augPath.pop_back();

    }

    return false;

}

Edge* FlowGraph::inverseEdge(Edge* edge, int origVert) {

    //armazena as arestas de saida do vertice de 
    //destino da aresta passada como parametro
    vector<Edge>& destVertOutEdges = flowAdjList[edge->destVert];

    //itera pelas arestas de saida do vertice de 
    //destino da aresta passada como parametro
    for (Edge& destVertOutEdge : destVertOutEdges) {

        //se o vertice de destino da aresta de 
        //saida for igual ao vertice de origem 
        //da aresta passada como parametro
        if (destVertOutEdge.destVert == origVert) {

            //retorna a aresta inversa da aresta passada como parametro
            return &destVertOutEdge;

        }

    }

    //apenas para evitar warning
    return edge;

}

void FlowGraph::printAugPath(vector<Edge*> augPath) {

    //itera pelas arestas do caminho aumentante
    for (Edge* currAugPathEdge : augPath) {

        cout << currAugPathEdge->destVert << " --> ";

    }

    cout << endl;

}