#include <vector>
#include <thread>
#include "lib/littletest.hpp"

#include "../src/ListaAtomica.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/CargarArchivos.hpp"

using namespace std;

void insertarEnLista(int x, ListaAtomica<int>& lista){
    for(int i = 0; i<1000; i++){
        lista.insertar(x);
    }
}

void testLista(ListaAtomica<int>& l){    

    vector<thread> threads(100);
    for(int i = 100; i>0; i--){
        threads[i-1] = thread(insertarEnLista, i, ref(l));
    }

    for(int i = 0; i<100; i++){
        threads[i].join();
    }
}



void insertarEnHash(string s, HashMapConcurrente &m){
    for(int i = 0; i<1000; i++){
        m.incrementar(s);
        m.valor(s);
    }
}

void testHashIncrementar(vector<string> elementos, HashMapConcurrente& hM){
    vector<thread> threads(100);
    for(int i = 100; i>0; i--){
        threads[i-1] = thread(insertarEnHash, elementos[i%10], ref(hM));
    }

    for(int i = 0; i<100; i++){
        threads[i].join();
    }
}

void testConcurrenciaCargaArchivos(HashMapConcurrente hM){
    HashMapConcurrente hm2 ;
    cargarMultiplesArchivos(hm2, 1, {"data/corpus"});
    cargarMultiplesArchivos(hM, 4, {"data/corpus", "data/corpus","data/corpus", "data/corpus" });

    //return (hm2.maximo().first() == hM.maximo()->first());
    //return true;
}
