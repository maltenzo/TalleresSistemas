

#include <iostream>
#include <chrono>

#include <string.h>

#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"


double tiempoDeEjecucion;

std::vector<std::string> filePaths = {};

int cantThreadsLectura;
int cantThreadsMaximo;


void experimentoMaximo(bool secuencial){

    HashMapConcurrente hashMap{};

    cargarMultiplesArchivos(hashMap, cantThreadsLectura, filePaths);

    // inicio el reloj para medir la duración del algoritmo
    auto start = chrono::steady_clock::now();

    if (secuencial){
        hashMap.maximo();

    }else{
        hashMap.maximoParalelo(cantThreadsMaximo);
    }

    // calculo cuanto tardó la ejecución
    auto end = chrono::steady_clock::now();
    tiempoDeEjecucion = chrono::duration<double, milli>(end - start).count();

    

}

void experimentoCargarArchivos(bool secuencial){
    HashMapConcurrente hashMap{};

    // inicio el reloj para medir la duración del algoritmo
    auto start = chrono::steady_clock::now();

    if (secuencial){

        for(auto path : filePaths){
            cargarArchivo(hashMap, path);
        }


    }else{

        cargarMultiplesArchivos(hashMap, cantThreadsLectura, filePaths);
        
    }

    // calculo cuanto tardó la ejecución
    auto end = chrono::steady_clock::now();
    tiempoDeEjecucion = chrono::duration<double, milli>(end - start).count();


}


void experimentoAmbos(){


    // inicio el reloj para medir la duración del algoritmo
    auto start = chrono::steady_clock::now();



    // calculo cuanto tardó la ejecución
    auto end = chrono::steady_clock::now();
    tiempoDeEjecucion = chrono::duration<double, milli>(end - start).count();

}

int main(int argc, char **argv) {
    if (argc < 6) {
        std::cout << "Error: faltan argumentos." << std::endl;
        std::cout << std::endl;
        std::cout << "Modo de uso: " << argv[0] << "<experimento> <secuencial> <threads_lectura> <threads_maximo>" << std::endl;
        std::cout << "    " << "<archivo1> [<archivo2>...]" << std::endl;
        std::cout << std::endl;
        std::cout << "    experimento: "
            << "maximo, cargarArchivos o ambas" << std::endl;
        std::cout << "    secuencial: "
            << "'Si' si es secuencial, 'No' si es paralelo" << std::endl;
        std::cout << "    threads_lectura: "
            << "Cantidad de threads a usar para leer archivos." << std::endl;
        std::cout << "    threads_maximo: "
            << "Cantidad de threads a usar para computar máximo." << std::endl;
        std::cout << "    archivo1, archivo2...: "
            << "Archivos a procesar." << std::endl;
        return 1;
    }

    bool secuencial = false;
    if (strcmp(argv[2], "si") == 0 || strcmp(argv[2], "Si") == 0){
        secuencial = true;
    }

    cantThreadsLectura = std::stoi(argv[3]);
    cantThreadsMaximo = std::stoi(argv[4]);

    
    for (int i = 3; i < argc; i++) {
        filePaths.push_back(argv[i]);
    }

    if (strcmp(argv[1] ,"maximo") == 0){

        experimentoMaximo(secuencial);

    } else if (strcmp(argv[1] ,"cargarArchivos") == 0){

        experimentoCargarArchivos(secuencial);

    } else if (strcmp(argv[1] ,"ambas") == 0){

        experimentoAmbos();

    } else{
        std::cout << "El primer argumento debe ser 'maximo', 'cargarArchivos' o 'ambas'" << endl;
        return 1;
    }


    std::clog << tiempoDeEjecucion << endl;

    return 0;
}
