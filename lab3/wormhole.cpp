/*
 * Laboratorio 3 - Wormhole
 * Nome: Eduardo Moreira Freitas de Souza
 * RA: 166779
 */

#include <stdio.h>
#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cstdlib>

#include <iostream>

#define NULL_PI -2
#define SOURCE -1

typedef struct Edge {

    unsigned int u, v;
    int weight;

} edge;

typedef struct Vertex {

    unsigned int id;

    int pi;
    int d;

    std::list<edge *> *adj_list;

} vertex;

class Graph {

    public:

        unsigned int n_v;
        unsigned int n_e;

        std::vector<vertex> vertices;
        std::list<edge *> edges;

        Graph(unsigned int  n_v);
        ~Graph();

        void AddEdge(unsigned int u, unsigned int v, int weight);
};

Graph::Graph(unsigned int n_v) {

    this->n_v = n_v;

    vertex init_vertex;

    init_vertex.adj_list = NULL;

    init_vertex.pi = NULL_PI;
    init_vertex.d = std::numeric_limits<int>::max();

    this->vertices.assign(n_v, init_vertex);

    for(unsigned int i = 0; i < this->vertices.size(); i++){

        this->vertices[i].id = i;
        this->vertices[i].adj_list = new std::list<edge *>();
    }

}

Graph::~Graph() {

    for(std::list<edge *>::iterator it = this->edges.begin(); it != this->edges.end(); ++it)
        free(*it);

    for(unsigned int i = 0; i < this->vertices.size(); ++i)
        delete(this->vertices[i].adj_list);
}

void Graph::AddEdge(unsigned int u, unsigned int v, int weight) {

    edge *e = (edge *) malloc(sizeof(edge));
    e->u = u;
    e->v = v;
    e->weight = weight;

    this->edges.push_back(e);
    this->vertices[u].adj_list->push_back(e);
}

void Relax(Graph *graph, edge *e) {

    if(graph->vertices[e->u].d != std::numeric_limits<int>::max() &&
       graph->vertices[e->v].d > graph->vertices[e->u].d + e->weight){

        graph->vertices[e->v].d = graph->vertices[e->u].d + e->weight;
        graph->vertices[e->v].pi = e->u;
    }
}

bool BellmanFordTimeTravel(Graph *graph){

    std::list<edge *>::iterator it;

    graph->vertices[0].d = 0;
    graph->vertices[0].pi = SOURCE;

    for(unsigned int i = 0; i < graph->n_v; ++i){
        for(it = graph->vertices[i].adj_list->begin();
            it != graph->vertices[i].adj_list->end();
            ++it){
            //printf("Relaxing u: %u and v: %u [%d]->", (*it)->u, (*it)->v, graph->vertices[(*it)->v].d);
            Relax(graph, (*it));
            //printf("[%d]\n", graph->vertices[(*it)->v].d);
        }
    }

    for(it = graph->edges.begin();
        it != graph->edges.end();
        ++it){
        if(graph->vertices[(*it)->u].d != std::numeric_limits<int>::max() && 
           graph->vertices[(*it)->v].d > graph->vertices[(*it)->u].d + (*it)->weight)
            return true;
    }

    return false;
}

int main() {

    unsigned int n, m, u, v;
    int weight;
    Graph *graph;

    bool travel_back_in_time = false;

    scanf("%u %u", &n, &m);

    graph = new Graph(n);

    for(unsigned int i = 0; i < m; ++i){

        scanf("%u %u %d", &u, &v, &weight);
        graph->AddEdge(u, v, weight);
    }

    travel_back_in_time = BellmanFordTimeTravel(graph);

    if(travel_back_in_time == true)
        printf("Possivel\n");
    else
        printf("Impossivel\n");

    delete(graph);

    return 0;
}
