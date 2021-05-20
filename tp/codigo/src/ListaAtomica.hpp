#ifndef LISTA_ATOMICA_HPP
#define LISTA_ATOMICA_HPP

#include <atomic>
#include <cstddef>
#include <mutex>
#include <vector>
using namespace std;



template<typename T>
class ListaAtomica {
 private:
    struct Nodo {
        Nodo(const T &val) : _valor(val), _siguiente(nullptr) {}

        T _valor;
        Nodo *_siguiente;
    };

    std::atomic<Nodo *> _cabeza;
    mutex mutex_insertar;

 public:
    ListaAtomica() : _cabeza(nullptr) {}

    ~ListaAtomica() {
        Nodo *n, *t;
        n = _cabeza.load();
        while (n) {
            t = n;
            n = n->_siguiente;
            delete t;
        }
    }

    void insertar(const T &valor) {
        // Completar (Ejercicio 1)
        //preparar nodo
        Nodo nuevoNodo = Nodo(valor);
        mutex_insertar.lock();
            //editar lista
            nuevoNodo._siguiente = _cabeza;
            _cabeza = &nuevoNodo;

        mutex_insertar.unlock();

        
        //lion : usar un mutex, para que solo edite un thread a la vez
                //podemos preparar el nodo a ser insertado afuera del mutex, y solo lockeamos el cambio de cabeza y asignar cabeza previa como sig.
    }

    T& operator[](size_t i) const {
        Nodo *n = _cabeza.load();
        for (size_t j = 0; j < i; j++) {
            n = n->_siguiente;
        }
        return n->_valor;
    }

    unsigned int longitud() const {
        Nodo *n = _cabeza.load();
        unsigned int cant = 0;
        while (n != nullptr) {
            cant++;
            n = n->_siguiente;
        }
        return cant;
    }

    bool find(string clave) const{
        Nodo *n = _cabeza.load();
        bool finded = false;
        while (n != nullptr && !finded) {
            if (clave == n->_valor.first){finded = true;}
            n = n->_siguiente;
        }
        return finded;
    }

     void incrementar(string clave) const{
        Nodo *n = _cabeza.load();
        bool finded = false;
        while (n != nullptr && !finded) {
            if (clave == n->_valor.first){
                finded = true;
                n->_valor.second++;
                }
            n = n->_siguiente;
        }
    }

    vector <string> claves()const{
        vector<string> claves;
        Nodo *n = _cabeza.load();
        while (n != nullptr) {
            claves.push_back(n->_valor.first);
            n = n->_siguiente;
        }
        return claves;

    }

    unsigned int apariciones(string clave){
        Nodo *n = _cabeza.load();
        bool finded = false;
        unsigned int apariciones = 0;
        while (n != nullptr && !finded) {
            if (clave == n->_valor.first){
                finded = true;
                apariciones = n->_valor.second;
                }
            n = n->_siguiente;
        }
        return apariciones;
    }

    struct iterator {
    private:
        ListaAtomica *_lista;

        typename ListaAtomica::Nodo *_nodo_sig;

        iterator(ListaAtomica<T> *lista, typename ListaAtomica<T>::Nodo *sig)
            : _lista(lista), _nodo_sig(sig) {}

    public:
        iterator &operator=(const typename ListaAtomica::iterator &otro) {
            _lista = otro._lista;
            _nodo_sig = otro._nodo_sig;
            return *this;
        }

        T& operator*() {
            return _nodo_sig->_valor;
        }

        iterator& operator++() { 
            _nodo_sig = _nodo_sig->_siguiente;
            return *this;
        }

        iterator operator++(int) { 
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const typename ListaAtomica::iterator &otro) const {
            return _lista->_cabeza.load() == otro._lista->_cabeza.load()
                && _nodo_sig == otro._nodo_sig;
        }

        bool operator!=(const typename ListaAtomica::iterator &otro) const {
            return !(*this == otro);
        }

        friend iterator ListaAtomica<T>::begin();
        friend iterator ListaAtomica<T>::end();
    };

    iterator begin() { 
        return iterator(this, _cabeza);
    }

    iterator end() { 
        return iterator(this, nullptr);
    }
};

#endif /* LISTA_ATOMICA_HPP */
