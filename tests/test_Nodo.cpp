// Archivo: test_Nodo.cpp
// Pruebas de unidad de la clase Nodo<T>, usando Nodo<int> para no depender del dominio.
// Como el nodo no es dueño de su dato, cada prueba libera por separado los datos y los nodos.

#include "doctest.h"
#include "Nodo.h"

// Verifica que el constructor guarde el puntero recibido y deje el nodo sin vecinos.
TEST_CASE("Nodo_constructor_valido")
{
   int* valor = new int(5);
   Nodo<int>* nodo = new Nodo<int>(valor);
   CHECK(nodo->getDato() == valor);        // guarda el mismo puntero, no una copia
   CHECK(*nodo->getDato() == 5);
   CHECK(nodo->getSiguiente() == nullptr); // un nodo nuevo no está enlazado
   CHECK(nodo->getAnterior() == nullptr);
   delete nodo;
   delete valor;
}

// Verifica que el constructor acepte nullptr como dato sin fallar (caso nulo).
TEST_CASE("Nodo_constructor_datoNulo")
{
   Nodo<int>* nodo = new Nodo<int>(nullptr);
   CHECK(nodo->getDato() == nullptr);
   CHECK(nodo->getSiguiente() == nullptr);
   CHECK(nodo->getAnterior() == nullptr);
   delete nodo;
}

// Verifica que setDato reemplace el dato y que el anterior siga existiendo:
// el nodo no lo libera, por eso todavía se puede leer y la prueba lo borra al final.
TEST_CASE("Nodo_setDato_valido")
{
   int* primero = new int(1);
   int* segundo = new int(2);
   Nodo<int>* nodo = new Nodo<int>(primero);
   nodo->setDato(segundo);
   CHECK(nodo->getDato() == segundo);
   CHECK(*primero == 1);                   // el dato anterior no fue liberado
   delete nodo;
   delete primero;
   delete segundo;
}

// Verifica que setDato acepte nullptr para dejar el nodo sin dato.
TEST_CASE("Nodo_setDato_nulo")
{
   int* valor = new int(3);
   Nodo<int>* nodo = new Nodo<int>(valor);
   nodo->setDato(nullptr);
   CHECK(nodo->getDato() == nullptr);
   delete nodo;
   delete valor;
}

// Verifica que setSiguiente enlace hacia adelante sin tocar el enlace hacia atrás.
TEST_CASE("Nodo_setSiguiente_valido")
{
   Nodo<int>* a = new Nodo<int>(nullptr);
   Nodo<int>* b = new Nodo<int>(nullptr);
   a->setSiguiente(b);
   CHECK(a->getSiguiente() == b);
   CHECK(a->getAnterior() == nullptr);     // solo cambió el enlace hacia adelante
   CHECK(b->getAnterior() == nullptr);     // el nodo no ajusta el enlace de regreso
   delete a;
   delete b;
}

// Verifica que setSiguiente(nullptr) desenlace el nodo hacia adelante.
TEST_CASE("Nodo_setSiguiente_nulo")
{
   Nodo<int>* a = new Nodo<int>(nullptr);
   Nodo<int>* b = new Nodo<int>(nullptr);
   a->setSiguiente(b);
   a->setSiguiente(nullptr);
   CHECK(a->getSiguiente() == nullptr);
   delete a;
   delete b;
}

// Verifica que setAnterior enlace hacia atrás sin tocar el enlace hacia adelante.
TEST_CASE("Nodo_setAnterior_valido")
{
   Nodo<int>* a = new Nodo<int>(nullptr);
   Nodo<int>* b = new Nodo<int>(nullptr);
   b->setAnterior(a);
   CHECK(b->getAnterior() == a);
   CHECK(b->getSiguiente() == nullptr);    // solo cambió el enlace hacia atrás
   CHECK(a->getSiguiente() == nullptr);    // el nodo no ajusta el enlace de regreso
   delete a;
   delete b;
}

// Verifica que setAnterior(nullptr) desenlace el nodo hacia atrás.
TEST_CASE("Nodo_setAnterior_nulo")
{
   Nodo<int>* a = new Nodo<int>(nullptr);
   Nodo<int>* b = new Nodo<int>(nullptr);
   b->setAnterior(a);
   b->setAnterior(nullptr);
   CHECK(b->getAnterior() == nullptr);
   delete a;
   delete b;
}

// Verifica un enlace doble A <-> B: avanzar de A a B y regresar deja otra vez en A,
// y cada nodo conserva su propio dato.
TEST_CASE("Nodo_enlaceDoble_dosNodos")
{
   int* valorA = new int(10);
   int* valorB = new int(20);
   Nodo<int>* a = new Nodo<int>(valorA);
   Nodo<int>* b = new Nodo<int>(valorB);
   a->setSiguiente(b);
   b->setAnterior(a);
   CHECK(a->getSiguiente()->getAnterior() == a);
   CHECK(*a->getSiguiente()->getDato() == 20);
   CHECK(*b->getAnterior()->getDato() == 10);
   delete a;
   delete b;
   delete valorA;
   delete valorB;
}

// Verifica que el template funcione con otro tipo además de int.
TEST_CASE("Nodo_constructor_otroTipo")
{
   double* valor = new double(2.5);
   Nodo<double>* nodo = new Nodo<double>(valor);
   CHECK(*nodo->getDato() == doctest::Approx(2.5));
   delete nodo;
   delete valor;
}
