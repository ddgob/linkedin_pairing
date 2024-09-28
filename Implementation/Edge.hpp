#ifndef EDGE_HPP
#define EDGE_HPP

struct Edge {
    
    int destVert;
    int capacity;
    int flow;

    bool operator==(const Edge& other) const {
        return destVert == other.destVert;
    }

};

struct Vertex {
    
    int indexVert;
    int numNeighbors;

    bool operator < (const Vertex& vert) const{
        return numNeighbors < vert.numNeighbors;
    }

};

#endif