#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>
#include "semaphore.h"
#include <vector>
#include <atomic>

#include "HashMapConcurrente.hpp"
using namespace std ;

struct Info_Tabla{
    Info_Tabla(vector<hashMapPair*> *maxs, vector<sem_t*> semaforos_hash, void *tabla_hash )
    : _maximos(maxs), _sems(semaforos_hash), _la_tabla(tabla_hash) {}

    vector<hashMapPair*> *_maximos;
    vector<sem_t*> _sems;
    void *_la_tabla;
};


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
        //agus: tal vez se la podría optimizar con threads?
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
        // preguntar inconsistencias  
        //agus: cabe aclarar que se bloquean los semaforos uno a uno permitiendo que se realice cualquier cambio posterior a la lista 
        // que está siendo analizada y luego se devuelven todos juntos.
    hashMapPair* max = new hashMapPair();
    max->second = 0;
    //for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {sem_wait(semaforos[index]);}
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        sem_wait(semaforos[index]);
        for(auto &p : *tabla[index]){
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {sem_post(semaforos[index]);}
    return *max;
}


/*void maximo_en_segmento(int threadID, int tablaInicio, int tablaFin, Info_Tabla info) {
                
    //hashMapPair* maximo_local = &hashMapPair("", 0); no compila
    hashMapPair* maximo_local = nullptr;
    int index_tabla = tablaInicio;
    vector<sem_t*> semaforos_hash = info._sems; //agus: por que todas estas copias?
    ListaAtomica<hashMapPair>* tabla_hash = (ListaAtomica<hashMapPair>*) info._la_tabla;
    vector<hashMapPair*> *vector_maximos = info._maximos;

    while(index_tabla < tablaFin){
        sem_wait(semaforos_hash[index_tabla]);
            for(unsigned int i = 0; i< tabla_hash[index_tabla].longitud(); i++){
                hashMapPair* entrada = &(tabla_hash[index_tabla][i]);
                if(maximo_local == nullptr or  entrada->second >= maximo_local->second){
                    maximo_local = entrada;
                }
            }
    }

    (*vector_maximos)[threadID] = maximo_local;
    for(int i = tablaInicio; i< tablaFin; i++){
        sem_post(semaforos_hash[i]);
    }
};*/
    // podemos hacer varias implementaciones de esto para experimentar

void maximo_desde_thread(int threadID, atomic<int>* progreso, Info_Tabla* info){

    int bucket_index = progreso->fetch_add(1);
    //vector<int> buckets_revisados;
    hashMapPair* maximo_local = nullptr;
    //agus: por que todas estas copias?
    //lion: pq sino no funciona/se ve re feo desde info
    vector<sem_t*> semaforos_hash = info->_sems; 
    ListaAtomica<hashMapPair>* tabla_hash = (ListaAtomica<hashMapPair>*) info->_la_tabla;
    vector<hashMapPair*> *vector_maximos = info->_maximos;

    while(bucket_index < HashMapConcurrente::cantLetras){
    //    buckets_revisados.push_back(bucket_index);
        sem_wait(semaforos_hash[bucket_index]);
            for(unsigned int i = 0; i< tabla_hash[bucket_index].longitud(); i++){
                    hashMapPair* entrada = &(tabla_hash[bucket_index][i]);
                    if(maximo_local == nullptr or  entrada->second >= maximo_local->second){
                        maximo_local = entrada;
                    }
                }
            bucket_index = progreso->fetch_add(1);
    }

    (*vector_maximos)[threadID] = maximo_local;
    /*sem_post(termine_de_buscar);
    sem_wait(terminar_busqueda);
    for(int i : buckets_revisados){
        sem_post(semaforos_hash[i]);
    }*/
    
}


hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
    //lion: hacer cant_treads que ejecuten sobre su (1/cant_threads) de la tabla?
    //luego ponen su resultado en un array o algo (semaforo para leerlo y escribirlo)
    //cuando terminan, el proceso original revisa ese array y busca el máx
    //para hacer consistente con insertar, habría que hacer que primero de todo agarre el semaforo de este?
    //esto esta incompleto, hay que terminarlo
    vector<hashMapPair*> maximos(cant_threads);
    atomic<int> progreso;
    progreso.store(0);
    vector<thread> threads(cant_threads);
    Info_Tabla info = Info_Tabla(&maximos, semaforos, tabla); //agus: no es HashMapConcurrente::semaforos? lo mismo para tabla
                                                              //lion: no? tipo, si lo hablamos desde el HashMapConcurretne, es redundante.

    for(unsigned int id = 0; id<cant_threads; id++){
        threads[id] = thread(maximo_desde_thread, id, &progreso, &info);
        
    }
    
    hashMapPair max = hashMapPair("",0);

    
    for(unsigned int id = 0; id<cant_threads; id++){
        (threads[id]).join(); //agus: join de un hashMapPair? no sera threads?
        if(max.second <= maximos[id]->second){
            max = *(maximos[id]);
        }
    }

    for(int i = 0; i<cantLetras; i++){
        sem_post(semaforos[i]);
    }

    return max;
            
}


#endif
