#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>

//#include <pthread.h>

#include <thread>

#include "CargarArchivos.hpp"
using namespace std;

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual) {
        // Completar (Ejercicio 4)
            //lion: hacemos insertar de cada palabra
            hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}


void cargarMultiplesArchivos(HashMapConcurrente &hashMap,
                             unsigned int cantThreads,
                             std::vector<std::string> filePaths) 
{
    // Completar (Ejercicio 4)
        //lion: hacemos varios threads, cada uno hace cargarArchivo
        //no se si hay que tener algo más en cuenta??
        //agus: Me parece que no, se supone que de la consistencia de incrementar 
        //ya se encarga la función misma
    
    vector<thread> threads(cantThreads);
    unsigned int cantidadArchivos = filePaths.size();
    for (unsigned int i = 0; i < cantidadArchivos; i++) { 
        threads[i] = thread(cargarArchivo, ref(hashMap), filePaths[i]);

        if(i==cantThreads or i==cantidadArchivos){//si ya use mi tope de threads o termine de cargar archivos le hago un wait a todos antes de continuar
            for (auto &t : threads) { 
            t.join();
            }
        }
    }

}

#endif
