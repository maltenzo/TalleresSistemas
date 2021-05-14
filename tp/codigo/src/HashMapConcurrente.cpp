#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>
#include "semaphore.h"

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {

    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
        //inicializamos los semaforos
        sem_t* semaforo;
        sem_init(semaforo, 0, 1);
        semaforos.push_back(semaforo);
    }
    
}


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
        //lion: usamos un semaforo que empieza en 1, asi bloqueamos lectura, creación y escritura del bucket accedido.
        int indice_tabla = hashIndex(clave); 
        ListaAtomica<hashMapPair>* lista = tabla[indice_tabla];
        
        sem_wait(semaforos[indice_tabla]);
            if (!lista->find(clave))
            {
                lista->insertar( hashMapPair(clave,1) );
            }else
            {       
                lista->incrementar(clave);
            }
         sem_post(semaforos[indice_tabla]);
         
}
 
std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
        //lion: solo dejamos que lea todo, no? sin semaforos ni nada.
    vector<string> las_claves; 
    for(unsigned int i = 0; i<cantLetras; i++){
        vector<string> claves_i = tabla[i]->claves();
        las_claves.insert(las_claves.end(), std::begin(claves_i), std::end(claves_i));
    }
    return las_claves;    
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
        //lion: mismo que claves, solo lo hace y ya
    int indice_tabla = hashIndex(clave); 
    ListaAtomica<hashMapPair>* lista = tabla[indice_tabla];
    int valor = lista->apariciones(clave);
    return valor;
}

hashMapPair HashMapConcurrente::maximo() { 
        //lion: habría que usar el mismo semaforo que usa insertar. Para que que sea el maximo de un momento de ejecución
            
    hashMapPair *max = new hashMapPair();
    max->second = 0;
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {sem_wait(semaforos[i])}
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {sem_post(semaforos[i])}
    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
        //lion: hacer cant_treads que ejecuten sobre su (1/cant_threads) de la tabla?
            //luego ponen su resultado en un array o algo (semaforo para leerlo y escribirlo)
            //cuando terminan, el proceso original revisa ese array y busca el máx
            //para hacer consistente con insertar, habría que hacer que primero de todo agarre el semaforo de este?
            //esto esta incompleto, hay que terminarlo
            vector<hashMapPair*> maximos [cant_threads];
            std::thread t = std::thread([maximos] (int miNumeroDeThread, int tablaInicio, int tablaFin) {
                hashMapPair* maxi = & hashMapPair("", 0);
                (*maximos)[miNumeroDeThread] = maxi;
            });
            
            
}
    // podemos hacer varias implementaciones de esto para experimentar
#endif
