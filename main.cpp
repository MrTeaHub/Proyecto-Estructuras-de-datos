#include "Grafo.cpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>

class Almacen
{
private:
    string nombre;
    vector<string> productos;

public:
    Almacen() : nombre(""), productos({}) {}
    Almacen(const string &nombre, const vector<string> &productos) : nombre(nombre), productos(productos) {}

    const string getNombre() const
    {
        return nombre;
    }

    void setNombre(const string &nombre)
    {
        Almacen::nombre = nombre;
    }

    const vector<string> &getProductos() const
    {
        return productos;
    }

    void setProductos(const vector<string> &productos)
    {
        Almacen::productos = productos;
    }
};

// Funcion que construye el grafo con sus vertices, aristas y pesos
void almacenarArchivo(string Archivo, Grafo<Almacen> *grafo)
{
    ifstream archivo(Archivo);
    string linea, nombreAlmacen, producto, from, to, weight;
    char delim = ',';
    // count ayuda a ver cuando se llega a "DISTANCIA" en el .txt
    int count = 0;
    vector<Almacen> vertices;
    getline(archivo, linea);
    while (getline(archivo, linea))
    {
        Almacen almacen;
        // Crea las aristas que contienen los pesos y relaciones entre los vertices
        if (count > 20)
        {
            Almacen almacen2;
            stringstream stream(linea);
            getline(stream, from, delim);
            getline(stream, to, delim);
            getline(stream, weight);
            almacen.setNombre(from);
            almacen2.setNombre(to);
            grafo->addAristaNoDireccional(almacen, almacen2, stoi(weight));
        }
        else if (count == 20) // Crea los vertices en el grafo con los almacenes
            for (int i = 0; i < vertices.size(); i++)
                grafo->addVertice(vertices[i]);
        else
        {
            // Crea los almacenes y los guarda en el vector "vertices"
            vector<string> productos;
            stringstream stream(linea);
            getline(stream, nombreAlmacen, ':');
            for (int i = 0; i < 10; i++)
            {
                getline(stream, producto, delim);
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

// Algoritmo de distancia mas corta
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

// Funcion que genera el string de la ruta mas corta, el vector con las posiciones
// de los vertices de la ruta mas corta y la distancia de esta
void traversal(Vertice<Almacen> *vertice, vector<string> &rutaMasCorta, vector<int> &posicionVertices, Grafo<Almacen> *grafo, int &distancia)
{
    stack<string> pila;
    stack<int> pila2;
    // Retrocede desde el ultimo almacen que toque visitar hasta el primero
    // a visitar
    while (vertice->predecesor)
    {
        // En pila se guardan los nombres de los almacenes.
        // En pila2 se guardan las posiciones de los almacenes
        // en la "listaVertices" del grafo.
        // distancia guarda la distancia total que tiene la ruta.
        pila.push(vertice->data.getNombre());
        pila2.push(grafo->obtenerPosicionVertice(vertice));
        distancia += vertice->getWeight(vertice->predecesor->data);
        vertice = vertice->predecesor;
    }
    // pila guarda el nombre del primer almacen por donde se empieza
    // pila2 guarda la posicion del primer almacen
    // Esto se hace porque el while para cuando el vertice no apunte a nada,
    // es decir, cuando llegue al primer almacen
    pila.push(vertice->data.getNombre());
    pila2.push(grafo->obtenerPosicionVertice(vertice));
    while (pila.size() > 0)
    {
        rutaMasCorta.push_back(pila.top());
        posicionVertices.push_back(pila2.top());
        pila.pop();
        pila2.pop();
    }
}

// Funcion que llama de manera iterativa al algoritmo dijkstra y que imprime
// la ruta mas corta con su distancia
void dijkstra(vector<vector<string>> posicionVertices, Grafo<Almacen> *grafo)
{
    // Esta funcion recibe el vector "posicionVertices", vector que como su nombre
    // lo indica, las posiciones de los vertices. Que son los vertices por donde se
    // debe pasar secuencialmente

    Vertice<Almacen> *v, *v1;
    vector<string> rutaMasCorta;
    // Vector que contiene las posiciones de la ruta más corta
    vector<int> posicionVerticesRutaCorta;
    // String que contiene la ruta más corta
    string ruta = "INICIO->";
    int distanciaRutaMasCorta = 0;

    // Si son 4 vertices por donde se tiene que pasar, este for calculará la
    // distancia más corta del vertice1 al vertice2, luego del vertice2 al vertice3
    // y así sucesivamente. Hallando secuencialmente, la distancia más corta.
    for (int i = 0; i < posicionVertices.size() - 1; i++)
    {
        v = grafo->getVertice(grafo->listaVertices[stoi(posicionVertices[i][0]) - 1]->data);
        v1 = grafo->getVertice(grafo->listaVertices[stoi(posicionVertices[i + 1][0]) - 1]->data);
        dijkstra(v, grafo);
        traversal(v1, rutaMasCorta, posicionVerticesRutaCorta, grafo, distanciaRutaMasCorta);
    }

    // Como en el anterior ejemplo, al calcular secuencialmente la ruta mas corta,
    // al ir del vertice1 al vertice4, el algoritmo me dirá que la ruta es:
    // vertice1->vertice2->vertice2->vertice3->vertice3->vertice4.
    // Como ven se repite el vertice2, debido a que es secuencialmente, entonces
    // este for elimina un vertice2 y quedaría:
    // vertice1->vertice2->vertice3->, arreglando este problema
    for (int i = 0; i < rutaMasCorta.size(); i++)
        if (rutaMasCorta[i] == rutaMasCorta[i + 1])
            rutaMasCorta.erase(rutaMasCorta.begin() + i, rutaMasCorta.begin() + i + 1);

    // Este for guarda la ruta más corta en el string "ruta"
    for (int i = 0; i < rutaMasCorta.size(); i++)
        ruta += rutaMasCorta[i] + "->";
    ruta += "FIN";
    cout << ruta << endl;
    cout << "Distancia: " << distanciaRutaMasCorta << " km" << endl
         << endl;
}

// Funcion que despliega el menu
void menu(Grafo<Almacen> *grafo)
{
    // Este vector contiene la posicion del almacen en el grafo y los productos
    // que se van a comprar
    vector<vector<string>> posicionInicioFin;
    while (true)
    {
        bool flag = true;
        int opc;
        cout << "Almacenes: " << endl;
        cout << "Digite el numero del almacen para ver sus productos" << endl;
        // Este for imprime el nombre de los almacenes del grafo
        for (int i = 0; i < grafo->listaVertices.size(); i++)
        {
            Vertice<Almacen> *vertice = grafo->listaVertices[i];
            cout << i + 1 << ") " << vertice->data.getNombre() << endl;
        }
        cout << "21) Productos a comprar" << endl;
        cout << "22) Calcular distancia mas corta" << endl;
        cout << "23) Borrar carro de compras" << endl;
        cout << "24) Salir" << endl;
        cin >> opc;
        if (opc == 24)
        {
            cout << "Saliendo del programa...";
            break;
        }
        else if (opc == 21)
        { // Me muestra que productos se van a comprar
            if (posicionInicioFin.empty())
                cout << endl
                     << "No hay productos en el carro de compras" << endl
                     << endl;
            else
            {
                cout << endl;
                for (int i = 0; i < posicionInicioFin.size(); i++)
                {
                    for (int j = 1; j < posicionInicioFin[i].size(); j++)
                    {
                        cout << posicionInicioFin[i][j] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
        }
        else if (opc == 22)
        { // Calcula la distancia mas corta
            if (posicionInicioFin.empty())
                cout << endl
                     << "No hay productos en el carro de compras" << endl
                     << endl;
            else
            {
                cout << endl
                     << "Distancia mas corta" << endl;
                dijkstra(posicionInicioFin, grafo);
            }
        }
        else if (opc == 23)
        { // Me elimina lo haya en el carro de compras
            posicionInicioFin.clear();
            cout << endl
                 << "Se ha borrado el carro de compras" << endl
                 << endl;
        }
        else
        {
            // vertice es el almacen que el usuario eligió para
            // visualizar los productos que este contenga
            // Se resta 1 a la opcion elegida, debido a que los vectores empiezan
            // por 0, entonces si se elige 1, el almacen estara en la posicion 0
            // en la lista de vertices del grafo
            Vertice<Almacen> *vertice = grafo->listaVertices[opc - 1];
            if (vertice->connectedTo.size() > 0)
            {
                int posicion = opc;
                while (true)
                {
                    // El vector "posicionNombre, almacenará la posicion del
                    vector<string> posicionNombre;
                    posicionNombre.push_back(to_string(posicion));
                    cout << endl
                         << vertice->data.getNombre() << endl;
                    cout << "Digite el numero del producto que quiera comprar" << endl;

                    // Este for muestra los productos que hay en el almacen elegido
                    for (int j = 0; j < vertice->data.getProductos().size(); j++)
                    {
                        cout << j + 1 << ") " << vertice->data.getProductos()[j] << endl;
                    }
                    cout << "11) Atras" << endl;
                    cin >> opc;
                    if (opc == 11)
                        break;

                    // Si el usuario no escoge ir atrás, significa que va a comprar un
                    // producto. Este for tiene dentro un if, if en el cual se entrará
                    // si anteriormente ya se ha comprado un producto de un
                    // mismo almacen. Poniendo a flag como false para que no entre en el
                    // if que sigue después del for
                    for (int i = 0; i < posicionInicioFin.size(); i++)
                    {
                        if (posicionInicioFin[i][0] == to_string(posicion))
                        {
                            posicionInicioFin[i].push_back(vertice->data.getProductos()[opc - 1]);
                            flag = false;
                        }
                    }

                    // Se entrará a este if, solamente cuando el vector
                    // "posicionInicioFin" no contenga un almacen
                    // Ejemplo: Si elijo comprar un producto del almacen Smartphones
                    // y anteriormente no se ha agregado nunca un producto del almacen
                    // Smartphones, se entrará en este if agregandolo en el vector
                    // "posicionInicioFin". Nunca se volverá a entrar hasta que se
                    // vacie el carro de compras
                    if (flag)
                    {
                        posicionNombre.push_back(vertice->data.getProductos()[opc - 1]);
                        posicionInicioFin.push_back(posicionNombre);
                    }
                }
                cout << endl;
            }
            else
            {
                cout << "No tiene productos.";
            }
            cout << endl;
        }
    }
}

int main()
{
    Grafo<Almacen> bot;
    almacenarArchivo("listaInicial.txt", &bot);
    menu(&bot);
    return 0;
}