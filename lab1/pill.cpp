/*
 * Laboratorio 1 - A pilula fantastica
 * Nome: Eduardo Moreira Freitas de Souza
 * RA: 166779
 */

#include <stdio.h>
#include <vector>
#include <queue>
#include <list>
#include <limits>

#include <iostream>
#define NULL_PI -1

enum Color { WHITE, GREY, BLACK };
enum Color_ID { NONE, A, B};

typedef struct Vertex {

    Color color;
    unsigned int d;
    unsigned int pi;

    Color_ID color_id;

    std::list<unsigned int> *adj_list;


} vertex;

class Graph {

    public:

        unsigned int n_v;
        unsigned int n_e;

        std::vector<vertex> vertices;

        Graph(unsigned int  n_v, unsigned int n_e);
        void AddEdge(unsigned int u, unsigned int v);
};

Graph::Graph(unsigned int n_v, unsigned int n_e){

    this->n_v = n_v;
    this->n_e = n_e;

    vertex init_vertex;

    init_vertex.color = WHITE;
    init_vertex.d = std::numeric_limits<unsigned int>::max();
    init_vertex.pi = NULL_PI;

    init_vertex.color_id = NONE;

    init_vertex.adj_list = NULL;

    this->vertices.assign(n_v, init_vertex);

    for(unsigned int i = 0; i < this->vertices.size(); i++)
        this->vertices[i].adj_list = new std::list<unsigned int>();

}

void Graph::AddEdge(unsigned int u, unsigned int v) {

    this->vertices[u].adj_list->push_back(v);
}

Color_ID CheckFreeColorInAdj(Graph *graph, unsigned v) {

    std::list<unsigned int>::iterator it;

    bool is_A_used = false;
    bool is_B_used = false;

    Color_ID free_color;

    //printf("v: %d\n", v);

    for(it = graph->vertices[v].adj_list->begin(); 
        it != graph->vertices[v].adj_list->end();
        it++)
    {

        //printf("Color of %d: %d\n", (*it), graph->vertices[(*it)].color_id);
        if(!is_A_used)
            is_A_used = ((graph->vertices[(*it)].color_id == A) ? true : false);
        if(!is_B_used)
            is_B_used = ((graph->vertices[(*it)].color_id == B) ? true : false);

    }

    
    if(is_A_used && !is_B_used)
        free_color = B;
    else if(!is_A_used && is_B_used)
        free_color = A;
    else if(!is_A_used)
        free_color = A;
    else
        free_color = NONE;

    return free_color;

}

bool IdentifyDotutamaWithBFS(Graph *graph) {

    std::queue<unsigned int> priority_queue;
    std::list<unsigned int>::iterator it;

    for(unsigned int src = 0; src < graph->vertices.size(); src++){

        //printf("Src is: %u\n", src);
        graph->vertices[src].color = GREY;
        graph->vertices[src].d = 0;
        graph->vertices[src].pi = NULL_PI;
        graph->vertices[src].color_id = A;

        priority_queue.push(src);

        while(priority_queue.empty() == false){

            unsigned int u = priority_queue.front();
            priority_queue.pop();

            //printf("u is: %u\n", u);

            for(it = graph->vertices[u].adj_list->begin(); 
                it != graph->vertices[u].adj_list->end();
                it++)
            {
                //printf("v is: %u\n", (*it));

                if(u == (*it))
                    return true;

                if(graph->vertices[(*it)].color == WHITE){

                    graph->vertices[(*it)].color = GREY;
                    graph->vertices[(*it)].d = graph->vertices[u].d + 1;
                    graph->vertices[(*it)].pi = u;
                    graph->vertices[(*it)].color_id = CheckFreeColorInAdj(graph, (*it));
                    //printf("Color_id of %d: %d\n", (*it), graph->vertices[(*it)].color_id);

                    if(graph->vertices[(*it)].color_id == NONE)
                        return true;

                    priority_queue.push(*it);
                }
            }

           graph->vertices[u].color = BLACK;

        }

    }

    return false;
}

int main() {

    unsigned int n, m, u, v;
    Graph *graph;

    scanf("%u %u\n", &n, &m);

    graph = new Graph(n, m);

    for(unsigned int i = 0; i < m; i++){

        scanf("%u %u\n", &u, &v);
        graph->AddEdge(u - 1, v - 1);
        graph->AddEdge(v - 1, u - 1);
    }

    bool result = IdentifyDotutamaWithBFS(graph);

    if(result == true)
        printf("dotutama\n");
    else
        printf("doturacu ou dotutama\n");

    return 0;
}
