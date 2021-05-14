#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    // Completar (Ejercicio 2)
        //lion: usamos un semaforo que empieza en 1, asi bloqueamos lectura, creación y escritura del bucket accedido.
}

std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
        //lion: solo dejamos que lea todo, no? sin semaforos ni nada.
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
        //lion: mismo que claves, solo lo hace y ya
}

hashMapPair HashMapConcurrente::maximo() { 
        //lion: habría que usar el mismo semaforo que usa insertar. Para que que sea el maximo de un momento de ejecución
            
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
        //lion: hacer cant_treads que ejecuten sobre su (1/cant_threads) de la tabla?
            //luego ponen su resultado en un array o algo (semaforo para leerlo y escribirlo)
            //cuando terminan, el proceso original revisa ese array y busca el máx
            //para hacer consistente con insertar, habría que hacer que primero de todo agarre el semaforo de este?
}

#endif
