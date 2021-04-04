#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

using namespace std;

class Vertex
{
    public:
        int value;
        Vertex *prox;
        Vertex(){}
};

class adjList
{
    public:
        Vertex *h;
};

class Graph
{
    public:
        int V;
        int E;
        adjList *list;
        Graph(){}     
};


class Item
{
    public:
        int data;
        Item *prox;
        Item(){}
};

class Fila
{
    public:
        Item *head;
        Item *tail;
        int size;
        Fila(){}
};


Fila* FFVazia()
{
    Fila *f = new Fila();
    f->head = NULL;
    f->tail = NULL;
    return f;
}

void Queue(Fila *f, int vertex)
{
    Item *d = new Item();
    d->data = vertex;
    d->prox = NULL;

    if(f->head == NULL)
    {
        f->head = d;
        f->tail = d;
    }
    else 
    {
        f->tail->prox = d;
        f->tail = d;
    }
    f->size ++;
}

Item* Dequeue(Fila *f)
{
    Item *aux;
    if(f->head == NULL)
        return NULL;

    aux = f->head;
    f->head = f->head->prox;
    f->size --;
    return aux;
}

Vertex* VertexInitialize(int value)
{
    Vertex *v = new Vertex();
    v->value = value;
    v->prox = NULL;
    return v;
}

Graph* GraphInitialize(int V)
{
    Graph *G = new Graph();
    G->V = V;
    G->E = 0;
    G->list = new adjList[V];
    for(int i=0;i<V;i++)
    {
        G->list[i].h = VertexInitialize(i);
    }
    return G;
}

void GraphInsertEdge(Graph *G,Vertex *v1, Vertex *v2)
{
    Vertex *v = G->list[v1->value].h;

    while(v->prox != NULL)
    {
        if(v->value == v2->value)
            return;
        v = v->prox;
    }
    v->prox = VertexInitialize(v2->value);
    G->E++;
}

void ImprimeGrpah(Graph *G)
{
    Vertex *aux;
    for(int i =0;i < G->V; i++)
    {
        aux =G->list[i].h;
        while(aux != NULL)
        {
            cout<<aux->value<<" -> ";
            aux = aux->prox;
        }
        cout<<endl;
    }
}

void BFS(Graph *G, Vertex *s)
{
    int cor[G->V];
    int d[G->V];
    int pi[G->V];
    Fila *f = FFVazia();

    for(int i=0;i < G->V;i++)
    {
        if(i != s->value)
        {
            cor[i] = 0;
            d[i] = -1;
            pi[i] = -1;
        }
    }
    cor[s->value] = 1;
    d[s->value] = 0;
    pi[s->value] = -1;

    Queue(f, s->value);

    while(f->size > 0)
    {
        Item *u = Dequeue(f);
        for(Vertex *v = G->list[u->data].h; v != NULL; v=v->prox)
        {
            if(cor[v->value] == 0)
            {
                cor[v->value] = 1;
                d[v->value] = d[u->data] + 1;
                pi[v->value] = u->data;
                Queue(f,v->value);
            }
        }
        cor[u->data] = 2;
        cout<<"Vertex: "<<u->data<<endl;
    }
}

void DFS_VISIT(Graph *G, Vertex *v, int *cor,int *d,int *f,int *tempo)
{
    cor[v->value] = 1;
    *tempo +=1;
    d[v->value] = *tempo;

    for(Vertex *u = G->list[v->value].h;u != NULL; u=u->prox)
    {
        if(cor[u->value] == 0)
            DFS_VISIT(G,u,cor,d,f,tempo);
    }
    cor[v->value] = 2;
    *tempo += 1;
    f[v->value] = *tempo;
    cout<<" Vertex: "<<v->value<<" D: "<<d[v->value]<<" F: "<<f[v->value]<<endl;
}

void DFS(Graph *G)
{
    int cor[G->V];
    int d[G->V];
    int f[G->V];
    int tempo = 0;

    for(int i =0;i < G->V;i++)
        cor[i] = 0;

    for(int i =0;i<G->V;i++)
        if(cor[i] == 0)
            DFS_VISIT(G,G->list[i].h,cor,d,f,&tempo);
}


void carregaArquivo(Graph *G)
{    
    int x,y;
    string linha;
    string a,b;
    ifstream arquivo;
    arquivo.open("Vertex.txt");
    if(arquivo.is_open())
    {
        while(getline(arquivo,linha))
        {
        stringstream ss(linha);
        getline(ss,a,';');  
        getline(ss,b,';');
        x= stoi(a);
        y= stoi(b);
        GraphInsertEdge(G, G->list[x].h, G->list[y].h);
        }
    }
    else
    {
        cout<<"Não foi possível abrir o arquivo"<<endl;
        
    }
    arquivo.close();
}
int main()
{
    Graph *G = NULL;
    int a,b;
    clock_t t;

    t = clock();
    G = GraphInitialize(100);
    carregaArquivo(G);
   
    ImprimeGrpah(G);

    cout<<endl<<"BFS"<<endl;
    BFS(G, G->list[0].h);

    cout<<endl<<endl;
    cout<<"DFS"<<endl;
    DFS(G);
    cout<<endl;
    t=clock() - t;
    cout<<"Tempo de execução:"<<((double)t)/(CLOCKS_PER_SEC)<<endl;
    return 0;
}