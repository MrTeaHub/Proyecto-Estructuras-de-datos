#include "Grafo.cpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <stack>
#include <queue>


class Almacen {
private:
    string nombre;
    vector<string> productos;
public:
    Almacen():nombre(""),productos({}){}
    Almacen(const string &nombre, const vector<string> &productos) : nombre(nombre), productos(productos) {}

    const string getNombre() const {
        return nombre;
    }

    void setNombre(const string &nombre) {
        Almacen::nombre = nombre;
    }

    const vector<string> &getProductos() const {
        return productos;
    }

    void setProductos(const vector<string> &productos) {
        Almacen::productos = productos;
    }
    friend ostream &operator<<(ostream &output, const Almacen &almacen){
        output << almacen.getNombre() << endl;
        return output;
    }
};

void almacenarArchivo(string Archivo, Grafo<Almacen> *grafo) {
    ifstream archivo(Archivo);
    string linea, nombreAlmacen, producto, from, to,weight;
    char delim = ',';
    int count = 0;

    vector<Almacen> vertices;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        Almacen almacen;

        if(count > 20){
            Almacen almacen2;
            stringstream stream(linea);
            getline(stream, from, delim);
            getline(stream, to, delim);
            getline(stream, weight);
            almacen.setNombre(from);
            almacen2.setNombre(to);
            grafo->addAristaNoDireccional(almacen,almacen2,stoi(weight));
        }
        else if(count == 20){
            for (int i = 0; i < vertices.size(); ++i) {
                grafo->addVertice(vertices[i]);
            }
        }
        else {
            vector<string> productos;
            stringstream stream(linea);
            getline(stream, nombreAlmacen,delim);

            for (int i = 0; i < 10; i++) {
                getline(stream,producto,delim);
                productos.push_back(producto);
            }

            almacen.setNombre(nombreAlmacen);
            almacen.setProductos(productos);
            vertices.push_back(almacen);
        }
        count++;
    }
    archivo.close();
}

void relacionproducto(Grafo<Almacen> *grafo){
    int productos;

    cout << "Digite el numero del producto que quiere ver sus relaciones" << endl;

    for (int i = 0; i < grafo->listaVertices.size(); i++) {
        Vertice<Almacen> *vertex = grafo->listaVertices[i];
        cout << i + 1 << ") " << vertex->data.getNombre() << endl;
    }

    cin>> productos;
    Vertice<Almacen> *vertex = grafo->listaVertices[productos-1];

    if (vertex->connectedTo.size() > 0) {
        for (int j = 0; j < vertex->connectedTo.size(); j++) {
            Arista<Almacen>* edge = vertex->connectedTo[j];
            cout << edge->to->data.getNombre() << " (Peso: " << edge->weight << ") ";
        }
    } else {
        cout << "No tiene aristas.";
    }
    cout << endl;
}
template <typename T>
void bea(Vertice<T>* begin){
    begin->distance = 0;
    begin->predecesor = 0;
    queue<Vertice<T>*> queueVertex;
    queueVertex.push(begin);
    while(queueVertex.size() > 0){
        Vertice<T>* curVertex = queueVertex.front();
        queueVertex.pop();
        for(Arista<T>* neighbor: curVertex->connectedTo){
            if(neighbor->to->color == 'w') { // white
                neighbor->to->color = 'g'; // grey
                neighbor->to->distance = curVertex->distance + 1;
                neighbor->to->predecesor = curVertex;
                queueVertex.push(neighbor->to);
            }
        }
        curVertex->color = 'b'; //black
    }
}
template <typename T>
void traversal(Vertice<T>* vertex){
    stack<T> stack;
    while(vertex->predecesor){
        stack.push(vertex->data);
        vertex = vertex->predecesor;
    }
    stack.push(vertex->data);
    //necesitamos una pila para que se imprima en orden
    while(stack.size() > 0){
        cout << "Elemento top: " << stack.top() << endl;
        stack.pop();
    }

}
int main (){
    vector<string> productos;
    Grafo<Almacen> bot;
    almacenarArchivo("listaInicial.txt",&bot);
    bool opc = true;
    Almacen inicial("Smartphones", vector<string>());
    Vertice<Almacen>* vertexInicial = bot.getVertice(inicial);
    Almacen final("Videojuegos", vector<string>());
    Vertice<Almacen>* vertexfinal = bot.getVertice(final);
    while (opc){
        cout << "Almacenes: " << endl;
        relacionproducto(&bot);
        cout << "Realizando BEA desde vertice..." << endl;
        bea(vertexInicial); // palabra inicial
        cout << "Imprimiendo recorrido..." << endl;
        traversal(vertexfinal); // palabra final
        cout << "Si quiere seguir con el programa escriba 1" << endl;
        cin >> opc;
    }
    return 0;
}