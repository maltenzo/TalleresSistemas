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
    //invoca 100 threads que c/u hace 1000 inserciones
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
    //invoca 100 threads que c/u incremente 1000 veces una clave, y pide su valor
    vector<thread> threads(100);
    for(int i = 100; i>0; i--){
        threads[i-1] = thread(insertarEnHash, elementos[i%10], ref(hM));
    }

    for(int i = 0; i<100; i++){
        threads[i].join();
    }
}

void insertarYMaximoEnHash(string s, HashMapConcurrente& m){
    for(int i = 0; i<1000; i++){
        m.incrementar(s);
        m.maximoParalelo(4);

    }
}

void testHashMaximo(vector<string> elementos, HashMapConcurrente& hM){
    //invoca 100 threads que incrementan 1000 veces una clave y piden el maximo
    vector<thread> threads(100);
    for(int i = 100; i>0; i--){
        threads[i-1] = thread(insertarYMaximoEnHash, elementos[i%10], ref(hM));
    }

    for(int i = 0; i<100; i++){
        threads[i].join();
    }
}


bool testConcurrenciaCargaArchivos(HashMapConcurrente& hM, HashMapConcurrente& hm2){

    cargarMultiplesArchivos(hm2, 1, {"data/corpus"});
    cargarMultiplesArchivos(hM, 4, {"data/corpus", "data/corpus","data/corpus", "data/corpus" });
    bool compartenMaximo = (hm2.maximo().first == hM.maximo().first);
    bool clavesSonProporcionales = true;
    bool tienenLasMismasClaves = (hM.claves().size() == hm2.claves().size());
    for(auto clave : hm2.claves()){
        if(hM.valor(clave) != 4 *hm2.valor(clave)){
            clavesSonProporcionales = false;
            tienenLasMismasClaves = false;
        }
    }
   
  
    return  clavesSonProporcionales && compartenMaximo && tienenLasMismasClaves ;
    
}
