#include <vector>
#include <thread>
#include "lib/littletest.hpp"

#include "../src/ListaAtomica.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/CargarArchivos.hpp"
#include "../src/TestsConcurrencia.cpp"

// Tests Ejercicio 1

LT_BEGIN_SUITE(TestsEjercicio1)

ListaAtomica<int> l;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio1)

LT_BEGIN_TEST(TestsEjercicio1, ListaComienzaVacia)
    LT_CHECK_EQ(l.longitud(), 0);
LT_END_TEST(ListaComienzaVacia)

LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaElemento)
    l.insertar(42);
    LT_CHECK_EQ(l.longitud(), 1);
LT_END_TEST(InsertarAgregaElemento)

LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaElementoCorrecto)
    l.insertar(42);
    LT_CHECK_EQ(l[0], 42);
LT_END_TEST(InsertarAgregaElementoCorrecto)

LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaEnOrden)
    l.insertar(4);
    l.insertar(3);
    l.insertar(2);
    l.insertar(1);
    LT_CHECK_EQ(l.longitud(), 4);
    LT_CHECK_EQ(l[0], 1);
    LT_CHECK_EQ(l[1], 2);
    LT_CHECK_EQ(l[2], 3);
    LT_CHECK_EQ(l[3], 4);
LT_END_TEST(InsertarAgregaEnOrden)

// Tests Ejercicio 2

LT_BEGIN_SUITE(TestsEjercicio2)

HashMapConcurrente hM;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio2)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoEnHashMapVacio)
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 0);
LT_END_TEST(ValorEsCorrectoEnHashMapVacio)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoEnHashMapVacio)
    std::vector<std::string> actual = hM.claves();
    std::vector<std::string> expected = {};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoEnHashMapVacio)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasUnaInsercion)
    hM.incrementar("tiranosaurio");
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 1);
LT_END_TEST(ValorEsCorrectoTrasUnaInsercion)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasUnaInsercion)
    hM.incrementar("tiranosaurio");
    std::vector<std::string> actual = hM.claves();
    std::vector<std::string> expected = {"tiranosaurio"};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoTrasUnaInsercion)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasDosInsercionesMismaPalabra)
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 2);
LT_END_TEST(ValorEsCorrectoTrasDosInsercionesMismaPalabra)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasDosInsercionesMismaPalabra)
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    std::vector<std::string> actual = hM.claves();
    std::vector<std::string> expected = {"tiranosaurio"};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoTrasDosInsercionesMismaPalabra)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasVariasInsercionesMismoBucket)
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("triceratops");
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 2);
    LT_CHECK_EQ(hM.valor("triceratops"), 1);
LT_END_TEST(ValorEsCorrectoTrasVariasInsercionesMismoBucket)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasVariasInsercionesMismoBucket)
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("triceratops");
    std::vector<std::string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), 2);
    LT_CHECK(std::find(actual.begin(), actual.end(), "tiranosaurio") != actual.end());
    LT_CHECK(std::find(actual.begin(), actual.end(), "triceratops") != actual.end());
LT_END_TEST(ClavesEsCorrectoTrasVariasInsercionesMismoBucket)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasVariasInsercionesDistintoBucket)
    hM.incrementar("tiranosaurio");
    hM.incrementar("estegosaurio");
    hM.incrementar("tiranosaurio");
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 2);
    LT_CHECK_EQ(hM.valor("estegosaurio"), 1);
LT_END_TEST(ValorEsCorrectoTrasVariasInsercionesDistintoBucket)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasVariasInsercionesDistintoBucket)
    hM.incrementar("tiranosaurio");
    hM.incrementar("estegosaurio");
    hM.incrementar("tiranosaurio");
    std::vector<std::string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), 2);
    LT_CHECK(std::find(actual.begin(), actual.end(), "tiranosaurio") != actual.end());
    LT_CHECK(std::find(actual.begin(), actual.end(), "estegosaurio") != actual.end());
LT_END_TEST(ClavesEsCorrectoTrasVariasInsercionesDistintoBucket)

// Tests Ejercicio 3

LT_BEGIN_SUITE(TestsEjercicio3)

HashMapConcurrente hM;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio3)

LT_BEGIN_TEST(TestsEjercicio3, MaximoEsCorrecto)
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("estegosaurio");
    hM.incrementar("estegosaurio");

    hashMapPair actual = hM.maximo();
    LT_CHECK_EQ(actual.first, "tiranosaurio");
    LT_CHECK_EQ(actual.second, 4);
LT_END_TEST(MaximoEsCorrecto)

LT_BEGIN_TEST(TestsEjercicio3, MaximoParaleloEsCorrectoUnThread)
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("estegosaurio");
    hM.incrementar("estegosaurio");

    hashMapPair actual = hM.maximoParalelo(1);
    LT_CHECK_EQ(actual.first, "tiranosaurio");
    LT_CHECK_EQ(actual.second, 4);
LT_END_TEST(MaximoParaleloEsCorrectoUnThread)

LT_BEGIN_TEST(TestsEjercicio3, MaximoParaleloEsCorrectoDosThreads)
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("tiranosaurio");
    hM.incrementar("estegosaurio");
    hM.incrementar("estegosaurio");

    hashMapPair actual = hM.maximoParalelo(2);
    LT_CHECK_EQ(actual.first, "tiranosaurio");
    LT_CHECK_EQ(actual.second, 4);
LT_END_TEST(MaximoParaleloEsCorrectoDosThreads)

// Tests Ejercicio 4

LT_BEGIN_SUITE(TestsEjercicio4)

HashMapConcurrente hM;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio4)

LT_BEGIN_TEST(TestsEjercicio4, CargarArchivoFunciona)
    cargarArchivo(hM, "data/test-1");
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 1);
    LT_CHECK_EQ(hM.claves().size(), 5);
LT_END_TEST(CargarArchivoFunciona)

LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaUnThread)
    cargarMultiplesArchivos(hM, 1, {"data/test-1", "data/test-2", "data/test-3"});
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 2);
    LT_CHECK_EQ(hM.valor("linux"), 3);
    LT_CHECK_EQ(hM.valor("estegosaurio"), 4);
    LT_CHECK_EQ(hM.claves().size(), 12);
LT_END_TEST(CargarMultiplesArchivosFuncionaUnThread)

LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaDosThreads)
    cargarMultiplesArchivos(hM, 1, {"data/test-1", "data/test-2", "data/test-3"});
    LT_CHECK_EQ(hM.valor("tiranosaurio"), 2);
    LT_CHECK_EQ(hM.valor("linux"), 3);
    LT_CHECK_EQ(hM.valor("estegosaurio"), 4);
    LT_CHECK_EQ(hM.claves().size(), 12);
LT_END_TEST(CargarMultiplesArchivosFuncionaDosThreads)


using namespace std;

LT_BEGIN_SUITE(TestsConcurrencia)

    ListaAtomica<int> l;
    HashMapConcurrente hM;
    HashMapConcurrente hm2;

    void set_up()
    {
    }

    void tear_down()
    {
    }

LT_END_SUITE()



LT_BEGIN_TEST(TestsConcurrencia, ListaAtomicaNoTieneRaceConditions)

    testLista(l);

    LT_CHECK_EQ(l.longitud(), 100000);

    vector<int> elementos(100,0);

    for(int i = 0; i<10000; i++){
        elementos[l[i]]++;
        LT_CHECK_LTE(elementos[l[i]], 1000);
    }

LT_END_TEST(ListaAtomicaNoTieneRaceConditions)


LT_BEGIN_TEST(TestsConcurrencia, HashMapNoTieneRaceConditionsConIncrementar)

    vector<string> elementos = {"aaa","aab","aba", "baa", "bab", "bba", "caa", "cab", "cba", "d"};
    testHashIncrementar(elementos, hM);

    LT_CHECK_EQ(hM.claves().size(), 10);
    for(int i = 0; i<10; i++){
        LT_CHECK_EQ(hM.valor(elementos[i]), 10000);
    }

LT_END_TEST(HashMapNoTieneRaceConditionsConIncrementar)

LT_BEGIN_TEST(TestsConcurrencia, HashMapNoTieneRaceConditionsConMaximo)

    vector<string> elementos = {"aaa","aab","aba", "baa", "bab", "bba", "caa", "cab", "cba", "d"};
    testHashMaximo(elementos, hM);

    LT_CHECK_EQ(hM.claves().size(), 10);
    for(int i = 0; i<10; i++){
        LT_CHECK_EQ(hM.valor(elementos[i]), 10000);
    }    

LT_END_TEST(HashMapNoTieneRaceConditionsConMaximo)

LT_BEGIN_TEST(TestsConcurrencia, HashMapNoTieneRaceConditionsConCargarMultiplesArchivos)
    bool res = testConcurrenciaCargaArchivos(hM, hm2);
    LT_CHECK(res);

LT_END_TEST(HashMapNoTieneRaceConditionsConCargarMultiplesArchivos)


// Ejecutar tests
LT_BEGIN_AUTO_TEST_ENV()
    AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
