#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>

#include "HashMapConcurrente.hpp"

struct Info_Tabla{
    Info_Tabla( mutex* m, ListaAtomica<hashMapPair> *tabla_hash )
    : _m(m), _la_tabla(tabla_hash) {}

    mutex* _m;
    ListaAtomica<hashMapPair> *_la_tabla;
};

HashMapConcurrente::HashMapConcurrente() {

    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        //inicializamos los semaforos
        sem_t* semaforo = new sem_t();
        sem_init(semaforo, 0, 1);
        semaforos.push_back(semaforo);
    }
    
}

//destructor para destruir los semaforos usados
HashMapConcurrente::~HashMapConcurrente(){
     for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
         sem_destroy(semaforos[i]);
     }
}


unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    // Completar (Ejercicio 2)
        //buscamos si la clave ya fué registrada. caso de que no, insertamos un nuevo nodo. Caso que si, incrementamos el existente
        //usamos un semaforo que empieza en 1, asi bloqueamos lectura, creación y escritura del bucket accedido.
        int indice_tabla = hashIndex(clave); 
        ListaAtomica<hashMapPair>* lista = &tabla[indice_tabla];
        
        sem_wait(semaforos[indice_tabla]);
            if (!lista->find(clave))
            {
                hashMapPair nuevoPar = hashMapPair(clave,1);
                lista->insertar(nuevoPar);
            }else
            {       
                lista->incrementar(clave);
            }
         sem_post(semaforos[indice_tabla]);
         
}
 
std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
        //buscamos todas las claves registradas en cada bucket del HashMap
    vector<string> las_claves; 
    for(unsigned int i = 0; i<cantLetras; i++){
        vector<string> claves_i = tabla[i].claves();
        las_claves.insert(las_claves.end(), std::begin(claves_i), std::end(claves_i));
    }
    return las_claves;    
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
        //buscamos el par de la clave y devolvemos su valor
    int indice_tabla = hashIndex(clave); 
    ListaAtomica<hashMapPair>* lista = &tabla[indice_tabla];
    int valor = lista->apariciones(clave);
    return valor;
}


hashMapPair HashMapConcurrente::maximo() {
    hashMapPair* max = new hashMapPair();
    max->second = 0;
    //tomamos el semaforo antes de entrar a cada bucket para buscar el maximo
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        sem_wait(semaforos[index]);
        for(auto &p : tabla[index]){
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {sem_post(semaforos[index]);}
    return *max;
}



void maximo_desde_thread(int threadID, atomic<int>* progreso, hashMapPair*& maximo, Info_Tabla* info){

    int bucket_index = progreso->fetch_add(1);

    ListaAtomica<hashMapPair>* tabla_hash = info->_la_tabla;
    mutex* puedoUsarElMaximo = info->_m;
    hashMapPair* maximo_local = new hashMapPair("", 0);

    //buscamos el maximo local de los buckets que revisamos
    //usamos el int atómico como indice para saber que buckets revisar
    while(bucket_index < HashMapConcurrente::cantLetras){
        for (hashMapPair &entrada : tabla_hash[bucket_index])
        {
             if( entrada.second > maximo_local->second){
                    maximo_local = &entrada;
                }
        }
        
        bucket_index = progreso->fetch_add(1);

    }

    //cuando no hay más buckets para revisar, subimos nuestro máximo de ser mayor al global
    puedoUsarElMaximo->lock();
        if(maximo->second < maximo_local->second){
            maximo = maximo_local;
        }
    puedoUsarElMaximo->unlock();
    
}


hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
    
    //llamamos a threads para que busquen maximos en cada bucket y actualizen "max" como valor maximo global
    hashMapPair* max = new hashMapPair("",0);
    atomic<int> progreso;
    progreso.store(0);
    vector<thread> threads(cant_threads);
    Info_Tabla info = Info_Tabla(&puedoUsarElMax, tabla); 

    for(int i = 0; i<cantLetras; i++){
        sem_wait(semaforos[i]);
    }

    for(unsigned int id = 0; id<cant_threads; id++){
        threads[id] = thread(maximo_desde_thread, id, &progreso, ref(max), &info);
        
    }
    
    for(unsigned int id = 0; id<cant_threads; id++){
        (threads[id]).join();
    }

    for(int i = 0; i<cantLetras; i++){
        sem_post(semaforos[i]);
    }

    return *max;
            
}


#endif
