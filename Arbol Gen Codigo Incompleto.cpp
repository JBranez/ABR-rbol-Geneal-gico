#include <iostream>
#include <string>

using namespace std;

struct Persona {
    int id;
    string nombre;
    string fecha_nac;
    Persona* padre;
    Persona* madre;
    Persona* izq;
    Persona* der;
};

Persona* crearPersona(int id, string nombre, string fecha) {
    Persona* nueva = new Persona;
    nueva->id = id;
    nueva->nombre = nombre;
    nueva->fecha_nac = fecha;
    nueva->padre = NULL;
    nueva->madre = NULL;
    nueva->izq = NULL;
    nueva->der = NULL;
    return nueva;
}

void insertar(Persona* &raiz, int id, string nombre, string fecha, Persona* padre, Persona* madre) {
    if (raiz == NULL) {
        raiz = crearPersona(id, nombre, fecha);
        raiz->padre = padre;
        raiz->madre = madre;
        return;
    }
    
    if (id < raiz->id) {
        insertar(raiz->izq, id, nombre, fecha, padre, madre);
    } else if (id > raiz->id) {
        insertar(raiz->der, id, nombre, fecha, padre, madre);
    }
}

Persona* buscar(Persona* raiz, int id) {
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    
    if (id < raiz->id) {
        return buscar(raiz->izq, id);
    } else {
        return buscar(raiz->der, id);
    }
}

Persona* encontrarMinimo(Persona* raiz) {
    while (raiz->izq != NULL) {
        raiz = raiz->izq;
    }
    return raiz;
}

Persona* eliminar(Persona* raiz, int id) {
    if (raiz == NULL) return raiz;

    if (id < raiz->id) {
        raiz->izq = eliminar(raiz->izq, id);
    } else if (id > raiz->id) {
        raiz->der = eliminar(raiz->der, id);
    } else {
        if (raiz->izq == NULL) {
            Persona* temp = raiz->der;
            delete raiz;
            return temp;
        } else if (raiz->der == NULL) {
            Persona* temp = raiz->izq;
            delete raiz;
            return temp;
        }
        
        Persona* temp = encontrarMinimo(raiz->der);
        raiz->id = temp->id;
        raiz->nombre = temp->nombre;
        raiz->fecha_nac = temp->fecha_nac;
        raiz->der = eliminar(raiz->der, temp->id);
    }
    return raiz;
}

void inorden(Persona* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izq);
        cout << "ID: " << raiz->id << ", Nombre: " << raiz->nombre 
             << ", Nacimiento: " << raiz->fecha_nac << endl;
        inorden(raiz->der);
    }
}

void mostrarAncestros(Persona* persona, int nivel = 0) {
    if (persona == NULL) return;
    
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << persona->nombre << endl;
    
    if (persona->padre != NULL) {
        for (int i = 0; i < nivel; i++) cout << "  ";
        cout << "Padre: ";
        mostrarAncestros(persona->padre, nivel + 1);
    }
    
    if (persona->madre != NULL) {
        for (int i = 0; i < nivel; i++) cout << "  ";
        cout << "Madre: ";
        mostrarAncestros(persona->madre, nivel + 1);
    }
}

const int MAX_NODOS = 100;

struct ListaPersonas {
    Persona* elementos[MAX_NODOS];
    int cantidad;
};

void inicializarLista(ListaPersonas &lista) {
    lista.cantidad = 0;
}

void agregarALista(ListaPersonas &lista, Persona* p) {
    if (lista.cantidad < MAX_NODOS) {
        lista.elementos[lista.cantidad++] = p;
    }
}

void almacenarNodos(Persona* raiz, ListaPersonas &lista) {
    if (raiz == NULL) return;
    almacenarNodos(raiz->izq, lista);
    agregarALista(lista, raiz);
    almacenarNodos(raiz->der, lista);
}

Persona* construirArbolBalanceado(ListaPersonas &lista, int inicio, int fin) {
    if (inicio > fin) return NULL;
    
    int medio = (inicio + fin) / 2;
    Persona* raiz = lista.elementos[medio];
    
    raiz->izq = construirArbolBalanceado(lista, inicio, medio - 1);
    raiz->der = construirArbolBalanceado(lista, medio + 1, fin);
    
    return raiz;
}

void balancearArbol(Persona* &raiz) {
    ListaPersonas lista;
    inicializarLista(lista);
    almacenarNodos(raiz, lista);
    raiz = construirArbolBalanceado(lista, 0, lista.cantidad - 1);
}

struct Cola {
    Persona* elementos[MAX_NODOS];
    int frente;
    int final;
};

void inicializarCola(Cola &c) {
    c.frente = 0;
    c.final = -1;
}

void encolar(Cola &c, Persona* p) {
    if (c.final < MAX_NODOS - 1) {
        c.elementos[++c.final] = p;
    }
}

Persona* frente(Cola &c) {
    if (c.frente > c.final) return NULL;
    return c.elementos[c.frente];
}

void desencolar(Cola &c) {
    if (c.frente <= c.final) {
        c.frente++;
    }
}

bool colaVacia(Cola &c) {
    return c.frente > c.final;
}

void porNiveles(Persona* raiz) {
    if (raiz == NULL) return;
    
    Cola c;
    inicializarCola(c);
    encolar(c, raiz);
    
    while (!colaVacia(c)) {
        Persona* actual = frente(c);
        desencolar(c);
        
        cout << "ID: " << actual->id << ", Nombre: " << actual->nombre << endl;
        
        if (actual->izq != NULL) encolar(c, actual->izq);
        if (actual->der != NULL) encolar(c, actual->der);
    }
}

void mostrarDescendientes(Persona* persona) {
    if (persona == NULL) return;
    
    cout << "Descendientes de " << persona->nombre << ":" << endl;
    if (persona->izq != NULL) {
        cout << "- " << persona->izq->nombre << endl;
        mostrarDescendientes(persona->izq);
    }
    if (persona->der != NULL) {
        cout << "- " << persona->der->nombre << endl;
        mostrarDescendientes(persona->der);
    }
}

void menu() {
    Persona* arbol = NULL;
    int opcion, id, id_padre, id_madre;
    string nombre, fecha;
    
    do {
        cout << "\n=== MENU ARBOL GENEALOGICO ===" << endl;
        cout << "1. Agregar persona" << endl;
        cout << "2. Eliminar persona" << endl;
        cout << "3. Buscar persona" << endl;
        cout << "4. Mostrar arbol (inorden)" << endl;
        cout << "5. Mostrar por niveles" << endl;
        cout << "6. Mostrar ancestros" << endl;
        cout << "7. Mostrar descendientes" << endl;
        cout << "8. Balancear arbol" << endl;
        cout << "9. Salir" << endl;
        cout << "Ingrese opcion: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1: {
                cout << "ID: "; cin >> id;
                cout << "Nombre: "; cin.ignore(); getline(cin, nombre);
                cout << "Fecha nacimiento (dd/mm/aaaa): "; cin >> fecha;
                cout << "ID del padre (0 si no existe): "; cin >> id_padre;
                cout << "ID de la madre (0 si no existe): "; cin >> id_madre;
                
                Persona* padre = (id_padre != 0) ? buscar(arbol, id_padre) : NULL;
                Persona* madre = (id_madre != 0) ? buscar(arbol, id_madre) : NULL;
                
                insertar(arbol, id, nombre, fecha, padre, madre);
                break;
            }
                
            case 2: {
                cout << "ID a eliminar: "; cin >> id;
                arbol = eliminar(arbol, id);
                break;
            }
                
            case 3: {
                cout << "ID a buscar: "; cin >> id;
                Persona* encontrado = buscar(arbol, id);
                if (encontrado != NULL) {
                    cout << "Encontrado: " << encontrado->nombre << endl;
                } else {
                    cout << "No encontrado" << endl;
                }
                break;
            }
                
            case 4: {
                cout << "\nArbol genealogico (inorden):" << endl;
                inorden(arbol);
                break;
            }
                
            case 5: {
                cout << "\nArbol por niveles:" << endl;
                porNiveles(arbol);
                break;
            }
                
            case 6: {
                cout << "ID para ver ancestros: "; cin >> id;
                mostrarAncestros(buscar(arbol, id));
                break;
            }
                
            case 7: {
                cout << "ID para ver descendientes: "; cin >> id;
                mostrarDescendientes(buscar(arbol, id));
                break;
            }
                
            case 8: {
                balancearArbol(arbol);
                cout << "Arbol balanceado" << endl;
                break;
            }
                
            case 9: {
                cout << "Saliendo..." << endl;
                break;
            }
                
            default:
                cout << "Opcion no valida" << endl;
        }
    } while (opcion != 9);
}

int main() {
    menu();
    return 0;
}
