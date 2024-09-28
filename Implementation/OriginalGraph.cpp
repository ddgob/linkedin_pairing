#include "OriginalGraph.hpp"

OriginalGraph::OriginalGraph(int numUsers_, int numJobs_) {

    numUsers = numUsers_;
    numJobs = numJobs_;
    
    numVerts = numUsers + numJobs;

    //aloca espaco para armazenar arestas do grafo
    origAdjList = new vector<Edge>[numVerts];

}


void OriginalGraph::addNewEdge(string user, string job, int& userCount, int& jobCount) {

    //verifica se usuario ja existe
    unordered_map<string, int>::iterator foundUser = mapUsersToIndex.find(user);
    bool userNotExist = foundUser == mapUsersToIndex.end();
    
    //se usuario nao existe
    if (userNotExist) {

        //adicione ele na lista de usuarios e mapeia 
        //seu respectivo indice na lista de adj
        mapUsersToIndex[user] = userCount;

        userCount++;

    }

    //verifica se vaga ja existe
    unordered_map<string, int>::iterator foundJob = mapJobsToIndex.find(job);
    bool jobNotExist = foundJob == mapJobsToIndex.end();

    //se vaga nao existe
    if (jobNotExist) {

        //adicione ela na lista de vagas e mapeia 
        //seu respectivo indice na lista de adj
        mapJobsToIndex[job] = jobCount + numUsers;
        
        jobCount++;

    }

    //cria aresta usuario->vaga
    Edge userQualifiesForJob;
    userQualifiesForJob.destVert = mapJobsToIndex[job];
    userQualifiesForJob.capacity = 1;
    userQualifiesForJob.flow = 0;

    //adiciona aresta usuario->vaga no grafo
    origAdjList[mapUsersToIndex[user]].push_back(userQualifiesForJob);

    //cria aresta vaga->usuario
    Edge jobIsQualifiedByUser;
    jobIsQualifiedByUser.destVert = mapUsersToIndex[user];
    jobIsQualifiedByUser.capacity = 0;
    jobIsQualifiedByUser.flow = 0;

    //adiciona aresta vaga->usuario no grafo
    origAdjList[mapJobsToIndex[job]].push_back(jobIsQualifiedByUser);

}

void OriginalGraph::printOrigGraph() {

    cout << "---------------------- Start Orig Graph ----------------------" << endl;
    
    //itera pelos vertices
    for (int currVertIndex = 0; currVertIndex < numVerts; currVertIndex++) {

        cout << currVertIndex << " -->   ";
        
        vector<Edge>& currVertNeighbors = origAdjList[currVertIndex];

        //itera pelas arestas de saida dos vertices
        for (Edge& currVertNeighbor : currVertNeighbors) {

            //printa tupla: (vertice destino, capacidade, flow)
            cout << "(D: " << currVertNeighbor.destVert << ", ";
            cout << "C: " << currVertNeighbor.capacity << ", ";
            cout << "F: " << currVertNeighbor.flow << ")   ";

        }

        cout << endl;

    }

    cout << "---------------------- End Orig Graph ----------------------" << endl;

}

int OriginalGraph::greedy() {
    
    //armazena todos os vertices, 
    //seja usuario seja vaga
    vector<Vertex> vertices;
    
    //itera pelos vertices
    for (int currVert = 0; currVert < numVerts; currVert++) {

        //cria vertice e armazena o 
        //indice do vertice e seu grau
        Vertex currVertex;
        currVertex.indexVert = currVert;
        currVertex.numNeighbors = origAdjList[currVert].size();

        //armazena vertice na "lista" de vertices
        vertices.push_back(currVertex);

    }

    //ordena os vertices e ordem ascendente de grau
    sort(vertices.begin(), vertices.end());

    //vetor que armazena vertices 
    //que ja foram pareados
    bool* isPaired = new bool[numVerts];
    //inicializa esse vetor com 
    //todos os vertices nao pareados
    for (int currVert = 0; currVert < numVerts; currVert++) {

        isPaired[currVert] = false;

    }

    int matches = 0;

    //itera por todos os vertices 
    //em ordem ascendente de grau
    for (Vertex& currVertex : vertices) {

        int currVertexIndex = currVertex.indexVert;
        
        //se o vertice nao tiver 
        //sido pareado ainda
        if (!isPaired[currVertexIndex]) {

            int numNeighbors = origAdjList[currVertexIndex].size();

            //itera pelos vizinhos desse vertice
            for (int currNeighbor = 0; currNeighbor < numNeighbors; currNeighbor++) {

                int currNeighborIndex = origAdjList[currVertexIndex][currNeighbor].destVert;
                
                //se o vizinho atual nao tiver sido pareado
                if (!isPaired[currNeighborIndex]) {

                    //pareia o vertice atual com seu vizinho
                    isPaired[currVertexIndex] = true;
                    isPaired[currNeighborIndex] = true;

                    //incrementa o numero de pareamentos
                    matches++;

                    break;

                }

            }

        }

    }

    delete[] isPaired;

    return matches;

}