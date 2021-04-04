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
        int estado;
        Vertex(){}
};

class H
{
    public:
        Vertex *h;
};

class Graph
{
    public:
        int V;
        int E;
        H **adj;
        
};

class Item
{
    public:
        int value;
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

void Queue(Fila *f, int value)
{
    Item *d = new Item();
    d->value = value;
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
    v->estado = 0;
    return v;
}

Graph* GraphInitialize(int V)
{
    Graph *G = new Graph();
    G->V = V;
    G->E = 0;
    G->adj = new H*[V];
    for (int i = 0; i < V; i++) 
    {
        G->adj[i] = new H[V];
        for (int j = 0; j < V; j++)
        {
            G->adj[i][j].h = VertexInitialize(0);
            G->adj[i][j].h->value = 0;
            G->adj[i][j].h->estado = 0;
        }
    }
    
    return G;
}

void GraphInsertEdge(Graph *G,int v1, int v2)
{
    
    G->adj[v1][v2].h = VertexInitialize(-1);
    
    G->adj[v2][v1].h = VertexInitialize(1);

    G->E++;
}

void ImprimeGrpah(Graph *G)
{
    for(int i =0;i< G->V;i++)
    {
        for(int j =0;j<G->V;j++)
        {
            cout<<"["<<G->adj[i][j].h->value<<"]";
        }
        cout<<endl;
    }
}


void BFS(Graph *G, int s)
{
    int cor[G->V];
    int d[G->V];
    int pi[G->V];

    for(int i=0;i < G->V;i++)
    {
        if(i != s)
        {
            cor[i] = 0;
            d[i] = -1;
            pi[i] = -1;
        }
    }
    cor[s] = 1;
    d[s] = 0;
    pi[s] = -1;
    Fila *f = FFVazia();
    Queue(f, s);

    while(f->size > 0)
    {
        Item *u = Dequeue(f);
        for(int i=0;i<G->V;i++)
        {
            if(G->adj[u->value][i].h->value == -1 && cor[i] == 0)
            {
                cor[i] = 1;
                d[i] = 0;
                pi[i] = -1;
                Queue(f,i);
            }
        }
        cor[u->value] = 2;
        cout<<"Vertex: "<<u->value<<endl;
    }
}

void DFS_VISIT(Graph *G, int v, int *cor,int *d,int *f,int *tempo)
{
    cor[v] = 1;
    *tempo +=1;
    d[v] = *tempo;

    for(int i=0;i<G->V;i++)
    {
        if(cor[i] == 0 && G->adj[v][i].h->value == -1)
            DFS_VISIT(G,i,cor,d,f,tempo);
    }
    cor[v] = 2;
    *tempo += 1;
    f[v] = *tempo;
    cout<<" Vertex: "<<v<<" D: "<<d[v]<<" F: "<<f[v]<<endl;
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
        for(int j =0;j<G->V;j++)
        {
            if(cor[i] == 0 && G->adj[i][j].h->value == -1)
                DFS_VISIT(G,i,cor,d,f,&tempo);
        }
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
        GraphInsertEdge(G,x,y);
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
    int a,b;
    clock_t t;
    t = clock();
    Graph *G = GraphInitialize(100);
    carregaArquivo(G);

    ImprimeGrpah(G);

    cout<<"BFS"<<endl;
    BFS(G, 0);
    cout<<endl<<"DFS"<<endl;
    DFS(G);
    cout<<endl;
    t=clock() - t;
    cout<<"Tempo de execução:"<<((double)t)/(CLOCKS_PER_SEC)<<endl;
    return 0;
}