#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
<<<<<<< HEAD
//#include <pthread.h>
=======
#include <pthread.h>
>>>>>>> f3c37b7db061516462d954608da509ca71cef75e
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
    
<<<<<<< HEAD
    std::vector<thread*> threads(cantThreads);
=======
    vector<thread> threads(cantThreads);
>>>>>>> f3c37b7db061516462d954608da509ca71cef75e

    for (int i = 0; i < cantThreads; i++) { 
        threads[i] = thread(cargarArchivo, hashMap, filePaths[i]);
    }

    for (auto &t : threads) { 
        t.join();
    }

}

#endif
