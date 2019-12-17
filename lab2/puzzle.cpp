/*
 * Laboratorio 2 - Puzzle
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
#define NULL_PI -1

enum Color { WHITE, GREY, BLACK };

typedef struct Edge {

    unsigned int u, v;
    int weight;

} edge;

typedef struct Vertex {

    unsigned int id;

    Color color;
    int max_dif;
    unsigned int pi;

    std::list<edge *> *adj_list;

} vertex;

class Graph {

    public:

        unsigned int n_v;
        unsigned int n_e;

        std::vector<vertex> vertices;
        std::list<edge *> edges;

        Graph(unsigned int  n_v);

        void AddEdge(unsigned int u, unsigned int v, int weight);
};

Graph::Graph(unsigned int n_v){

    this->n_v = n_v;

    vertex init_vertex;

    init_vertex.color = WHITE;
    init_vertex.max_dif = std::numeric_limits<unsigned int>::min();
    init_vertex.pi = NULL_PI;

    init_vertex.adj_list = NULL;

    this->vertices.assign(n_v, init_vertex);

    for(unsigned int i = 0; i < this->vertices.size(); i++){

        this->vertices[i].id = i;
        this->vertices[i].adj_list = new std::list<edge *>();
    }

}

void Graph::AddEdge(unsigned int u, unsigned int v, int weight) {

    edge *e = (edge *) malloc(sizeof(edge));
    e->u = u;
    e->v = v;
    e->weight = weight;

    this->edges.push_back(e);
    this->vertices[u].adj_list->push_back(e);
}

int RebukeGLaDOSWithBFS(Graph graph, unsigned int src, unsigned int dest) {

    std::queue<int> priority_queue;
    std::list<edge *>::iterator it;

    graph.vertices[src].color = GREY;

    priority_queue.push(src);

    while(priority_queue.empty() == false){

        unsigned int u = priority_queue.front();
        priority_queue.pop();

        if(u == dest)
            return graph.vertices[u].max_dif;

        for(it = graph.vertices[u].adj_list->begin(); 
            it != graph.vertices[u].adj_list->end();
            ++it)
        {
            if(graph.vertices[(*it)->v].color == WHITE){

                graph.vertices[(*it)->v].color = GREY;
                graph.vertices[(*it)->v].max_dif = ((graph.vertices[u].max_dif > (*it)->weight) ? graph.vertices[u].max_dif : (*it)->weight);
                graph.vertices[(*it)->v].pi = u;

                priority_queue.push((*it)->v);
            }
        }

       graph.vertices[u].color = BLACK;
    }

    return graph.vertices[dest].max_dif;
}

bool CompareEdges(const edge *first, const edge *second) {
    return (first->weight < second->weight);
}

Graph *MSTKruskal(Graph *G){

    Graph *mst = new Graph(G->n_v);

    std::vector<unsigned int> which_set_belongs_to;
    std::list<edge *>::iterator it;
    
    for(unsigned int i = 0; i < G->n_v; ++i)
        which_set_belongs_to.push_back(i);

    G->edges.sort(CompareEdges);

    for(it = G->edges.begin(); it != G->edges.end(); ++it){

        unsigned int find_set_u = (*it)->u;
        unsigned int find_set_v = (*it)->v;

        while(which_set_belongs_to[find_set_u] != find_set_u)
            find_set_u = which_set_belongs_to[find_set_u];

        while(which_set_belongs_to[find_set_v] != find_set_v)
            find_set_v = which_set_belongs_to[find_set_v];
        
        if(find_set_u != find_set_v){

            mst->AddEdge((*it)->u, (*it)->v, (*it)->weight);
            mst->AddEdge((*it)->v, (*it)->u, (*it)->weight);

            which_set_belongs_to[find_set_v] = find_set_u;
        }
    }

    return mst;
}

int main() {

    unsigned int n, m, u, v, k;
    int weight, result;
    Graph *graph, *mst;

    scanf("%u %u", &n, &m);

    graph = new Graph(n);

    for(unsigned int i = 0; i < m; ++i){

        scanf("%u %u %d", &u, &v, &weight);
        graph->AddEdge(u, v, weight);
        graph->AddEdge(v, u, weight);
    }

    mst = MSTKruskal(graph);

    scanf("%u", &k);

    for(unsigned int i = 0; i < k; ++i){
    
        scanf("%u %u", &u, &v);

        result = RebukeGLaDOSWithBFS(*mst, u, v);
        printf("%d\n", result);

    }

    return 0;
}
