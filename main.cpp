
#include "Grafo.cpp"
#include <vector>
#include <fstream>
#include <sstream>

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
            getline(stream, nombreAlmacen,':');
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
int main () {
    vector<string> productos;
    Grafo<Almacen> bot;
    almacenarArchivo("listaInicial.txt",&bot);
    bool opc = true;
    while (opc){
        cout << "Almacenes: " << endl;
        relacionproducto(&bot);
        cout << "Si quiere seguir con el programa escriba 1" << endl;
        cin >> opc;
    }

    return 0;
}