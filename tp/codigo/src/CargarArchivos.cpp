#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>

//#include <pthread.h>

#include <thread>
#include <atomic>

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



void cargarArchivosThread(HashMapConcurrente &hashMap, std::vector<std::string> filePaths, atomic<int>* progreso){
    //usando el int atómico como indice, buscamos el archivo que hace falta cargar y llamamos a cargarArchivo.
    unsigned int file_index = progreso->fetch_add(1);

    while(file_index < filePaths.size()){

        cargarArchivo(hashMap, filePaths[file_index]);

        file_index = progreso->fetch_add(1);

    }

}


void cargarMultiplesArchivos(HashMapConcurrente &hashMap,
                             unsigned int cantThreads,
                             std::vector<std::string> filePaths) 
{
    // Completar (Ejercicio 4)
    //llamamos threads para que carguen archivos
    atomic<int> progreso;
    progreso.store(0);
    vector<thread> threads(cantThreads);
    for (unsigned int i = 0; i < cantThreads; i++) { 
        threads[i] = thread(cargarArchivosThread, ref(hashMap), filePaths, &progreso);

    }

    for (auto &t : threads) { 
    t.join();
    }

}

#endif
