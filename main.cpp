#include "Grafo.cpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>


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
            for (int i = 0; i < vertices.size(); i++) {
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

void dijkstra(Vertice<Almacen> *begin, Grafo<Almacen> *grafo)
{
    for (int i = 0; i < grafo->count; i++)
    {
        grafo->listaVertices[i]->color = 'w';
        grafo->listaVertices[i]->distance = INT_MAX;
    }
    begin->distance = 0;
    begin->predecesor = 0;

    for (int i = 0; i < grafo->count; i++)
    {
        Vertice<Almacen> *curVertice = 0;
        int minDistance = INT_MAX;

        for (int j = 0; j < grafo->count; j++)
        {
            if (grafo->listaVertices[j]->color == 'w' && grafo->listaVertices[j]->distance < minDistance)
            {
                curVertice = grafo->listaVertices[j];
                minDistance = curVertice->distance;
            }
        }

        if (curVertice == 0)
            break;
        curVertice->color = 'b';

        for (Arista<Almacen> *vecino : curVertice->connectedTo)
        {
            if (vecino->to->color == 'w')
            {
                int nuevaDistancia = curVertice->distance + vecino->weight;
                if (nuevaDistancia < vecino->to->distance)
                {
                    vecino->to->distance = nuevaDistancia;
                    vecino->to->predecesor = curVertice;
                }
            }
        }
    }
}

void traversal(Vertice<Almacen>* vertice){
    stack<string> stack;
    string ruta = "INICIO->";
    while(vertice->predecesor){
        stack.push(vertice->data.getNombre());
        vertice = vertice->predecesor;
    }
    stack.push(vertice->data.getNombre());
    while(stack.size() > 0){
        ruta += stack.top() + "->";
        stack.pop();
    }
    cout << ruta << "FIN" << endl << endl;
}

void dijkstra(vector<vector<string>> posicionVertices, Grafo<Almacen>* grafo){
    Vertice<Almacen>* v,*v1;
    for (int i = 0; i < posicionVertices.size()-1; i++) {
        v = grafo->getVertice(grafo->listaVertices[stoi(posicionVertices[i][0])-1]->data);
        v1 = grafo->getVertice(grafo->listaVertices[stoi(posicionVertices[i+1][0])-1]->data);
        dijkstra(v,grafo);
        traversal(v1);
    }
}

void menu(Grafo<Almacen> *grafo){
    //contiene la posicion del almacen en el grafo y los productos que se van a comprar
    vector<vector<string>> posicionInicioFin;
    while(true){
        bool flag = true;
        int opc;
        cout << "Almacenes: " << endl;
        cout << "Digite el numero del almacen para ver sus productos" << endl;
        for (int i = 0; i < grafo->listaVertices.size(); i++) {
            Vertice<Almacen> *vertice = grafo->listaVertices[i];
            cout << i + 1 << ") " << vertice->data.getNombre() << endl;
        }
        cout << "21) Productos a comprar" << endl;
        cout << "22) Calcular distancia mas corta" << endl;
        cout << "23) Borrar carro de compras" << endl;
        cout << "24) Salir" << endl;
        cin>> opc;
        if(opc == 24) {
            cout << "Saliendo del programa...";
            break;
        }
        else if(opc == 21) {
            if(posicionInicioFin.empty()) cout << endl << "No hay productos en el carro de compras" << endl << endl;
            else {
                cout << endl;
                for (int i = 0; i < posicionInicioFin.size(); i++) {
                    for (int j = 1; j < posicionInicioFin[i].size(); j++) {
                        cout << posicionInicioFin[i][j] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
        }
        else if(opc == 22){
            if(posicionInicioFin.empty()) cout << endl << "No hay productos en el carro de compras" << endl << endl;
            else {
                cout << endl << "Distancia mas corta" << endl;
                dijkstra(posicionInicioFin,grafo);
            }

        }
        else if(opc == 23) {
            posicionInicioFin.clear();
            cout << endl << "Se ha borrado el carro de compras" << endl << endl;
        }
        else {
            Vertice<Almacen> *vertice = grafo->listaVertices[opc-1];
            if (vertice->connectedTo.size() > 0) {
                int posicion = opc;
                while(true) {
                    vector<string> posicionNombre;
                    posicionNombre.push_back(to_string(posicion));
                    cout << endl << vertice->data.getNombre() << endl;
                    cout << "Digite el numero del producto que quiera comprar" << endl;
                    for (int j = 0; j < vertice->data.getProductos().size(); j++) {
                        cout << j+1 << ") " << vertice->data.getProductos()[j] << endl;
                    }
                    cout << "11) Atras" << endl;
                    cin >> opc;
                    if(opc == 11) break;
                    for (int i = 0; i < posicionInicioFin.size(); i++) {
                        if(posicionInicioFin[i][0] == to_string(posicion)) {
                            posicionInicioFin[i].push_back(vertice->data.getProductos()[opc-1]);
                            flag = false;
                        }
                    }
                    if(flag){
                        posicionNombre.push_back(vertice->data.getProductos()[opc-1]);
                        posicionInicioFin.push_back(posicionNombre);
                    }
                }
                cout << endl;
            } else {
                cout << "No tiene productos.";
            }
            cout << endl;
        }

    }
}


int main () {
    Grafo<Almacen> bot;
    almacenarArchivo("listaInicial.txt",&bot);
    menu(&bot);
    return 0;
}