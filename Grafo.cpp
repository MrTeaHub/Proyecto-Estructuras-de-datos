#include "Grafo.h"

template<typename T>
Vertice<T>::Vertice(const T &value) {
    data = value;
    inDegree = 0;
    outDegree = 0;
    connectedTo = {};
    predecesor = 0;
    distance = INT_MAX;
    color = 'w';
}

template<typename T>
Vertice<T>::~Vertice() {}


template<typename T>
void Vertice<T>::addNeighbor(Vertice<T> *to, int weight) {
    Arista<T>* temp = new Arista<T>;
    temp->to = to;
    temp->weight = weight;
    outDegree++;
    to->inDegree++;
    connectedTo.push_back(temp);
}

template<typename T>
int Vertice<T>::getWeight(const T &value) {
    for (int i = 0; i < connectedTo.size(); i++) {
        Arista<T>* temp = connectedTo[i];
        if(temp->to->data == value) return connectedTo[i]->weight;
    }
    return NULL;
}

template<typename T>
Grafo<T>::Grafo() {
    count = 0;
    listaVertices = {};
}

template<typename T>
Grafo<T>::~Grafo() {}

template<typename T>
Vertice<T> *Grafo<T>::addVertice(const T &value) {
    Vertice<T>* newVertice = new Vertice<T>(value);
    listaVertices.push_back(newVertice);
    count++;
    return newVertice;
}

template<typename T>
void Grafo<T>::addAristaNoDireccional(const T &from, const T &to, int weight) {
    Vertice<T>* fromVertice = getVertice(from);
    if (!fromVertice) fromVertice = addVertice(from);
    Vertice<T>* toVertice = getVertice(to);
    if(!toVertice) toVertice = addVertice(to);
    fromVertice->addNeighbor(toVertice,weight);
    toVertice->addNeighbor(fromVertice,weight);
}

template<typename T>
Vertice<T> *Grafo<T>::getVertice(const T &value) {
    for (int i = 0; i < listaVertices.size(); i++) {
        if (listaVertices[i]->data.getNombre() == value.getNombre()) return listaVertices[i];
    }
    return NULL;
}

template <class T>
int Grafo<T>::obtenerPosicionVertice(Vertice<T> *vertice)
{
    for (int i = 0; i < listaVertices.size(); i++) if (listaVertices[i]->data.getNombre() == vertice->data.getNombre()) return i;
    
}
