#ifndef PRYECTOFINAL_GRAFO_H
#define PRYECTOFINAL_GRAFO_H

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Vertice;

template <typename T>
class Arista{
public:
    Vertice<T>* to;
    int weight;

    friend ostream &operator<<(ostream &out, Arista<T>* arista){
        out << "To: " << arista->to->data << ", Weight: " << arista->weight << endl;
        return out;
    }
};

template <typename T>
class  Vertice{
public:
    T data;
    int inDegree, outDegree;
    vector<Arista<T>*> connectedTo;

    Vertice<T>* predecesor;
    int distance;
    char color;

    Vertice(const T& value);
    ~Vertice();

    void addNeighbor(Vertice<T>* to, int weight = 0);
    int getWeight(T& value);

    friend ostream &operator<<(ostream &out, Vertice<T>* vertice){
        out << vertice->data.getNombre() << endl;
        out << "In degree: " << vertice->inDegree << endl;
        out << "Out degree: " << vertice->outDegree << endl;
        out << "Aristas: " << endl;
        out << vertice->connectedTo;
        return out;
    }
};



template <typename T>
class Grafo {
public:
    int count;
    vector<Vertice<T>*> listaVertices;
    Grafo();
    ~Grafo();
    Vertice<T>* addVertice(const T& value);
    Vertice<T>* getVertice(const T& value);
    void addAristaNoDireccional(const T& from, const T& to, int weight = 0);
    int obtenerPosicionVertice(Vertice<T> *vertice);
};



#endif //PRYECTOFINAL_GRAFO_H
