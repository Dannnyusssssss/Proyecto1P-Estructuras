// Archivo: test_ListaDoble.cpp
// Pruebas de unidad de la clase ListaDoble<T>, usando ElementoPrueba para no depender del dominio.
// Además del resultado de cada método, se verifica:
//  - el orden y los enlaces dobles (recorrer hacia adelante y hacia atrás da lo mismo),
//  - que la memoria de los datos se libere (contador de ElementoPrueba vivos).

#include "doctest.h"
#include "ListaDoble.h"
#include "ElementoPrueba.h"
#include <iostream>
#include <sstream>
#include <string>

// Clase auxiliar de prueba con métodos static para recorrer una lista y
// devolver sus claves como texto separado por comas (por ejemplo, "A,B,C").
class RecorridoLista
{
public:
   // Recorre desde la cabeza siguiendo getSiguiente().
   static std::string adelante(const ListaDoble<ElementoPrueba>* lista)
   {
      std::string texto = "";
      Nodo<ElementoPrueba>* actual = lista->getCabeza();
      while (actual != nullptr)
      {
         if (!texto.empty())
         {
            texto += ",";
         }
         texto += actual->getDato()->getClave();
         actual = actual->getSiguiente();
      }
      return texto;
   }

   // Recorre desde la cola siguiendo getAnterior() y devuelve las claves
   // en orden normal (de la cabeza a la cola), para compararlo con adelante().
   static std::string atras(const ListaDoble<ElementoPrueba>* lista)
   {
      std::string texto = "";
      Nodo<ElementoPrueba>* actual = lista->getCola();
      while (actual != nullptr)
      {
         if (!texto.empty())
         {
            texto = "," + texto;
         }
         texto = actual->getDato()->getClave() + texto;
         actual = actual->getAnterior();
      }
      return texto;
   }
};

// Clase auxiliar de prueba que redirige std::cout a un buffer mientras existe.
class CapturaSalidaLista
{
public:
   // Guarda el buffer original de std::cout y lo reemplaza por el interno.
   CapturaSalidaLista() : original(std::cout.rdbuf(buffer.rdbuf())) {}

   // Devuelve std::cout a su buffer original.
   ~CapturaSalidaLista()
   {
      std::cout.rdbuf(original);
   }

   // Devuelve lo que se imprimió mientras la captura estuvo activa.
   std::string texto(void) const
   {
      return buffer.str();
   }

private:
   std::ostringstream buffer;   // destino temporal de la salida
   std::streambuf* original;    // buffer original de std::cout (no es dueño)
};

// ---------------------------------------------------------------------
// Constructor, estaVacia y getTamanio
// ---------------------------------------------------------------------

// Verifica que una lista nueva esté vacía, con tamaño 0 y sin cabeza ni cola.
TEST_CASE("ListaDoble_constructor_listaNueva")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   CHECK(lista->estaVacia() == true);
   CHECK(lista->getTamanio() == 0);
   CHECK(lista->getCabeza() == nullptr);
   CHECK(lista->getCola() == nullptr);
   delete lista;
}

// Verifica que estaVacia y getTamanio cambien al insertar y al eliminar.
TEST_CASE("ListaDoble_estaVacia_getTamanio_cambios")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   CHECK(lista->estaVacia() == false);
   CHECK(lista->getTamanio() == 1);
   lista->insertarFinal(new ElementoPrueba("B"));
   CHECK(lista->getTamanio() == 2);
   lista->eliminar("A");
   lista->eliminar("B");
   // Tras eliminar todo vuelve a estar vacía
   CHECK(lista->estaVacia() == true);
   CHECK(lista->getTamanio() == 0);
   delete lista;
}

// ---------------------------------------------------------------------
// insertarInicio
// ---------------------------------------------------------------------

// Verifica que en una lista vacía el único nodo sea a la vez cabeza y cola, sin vecinos.
TEST_CASE("ListaDoble_insertarInicio_listaVacia")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarInicio(new ElementoPrueba("A"));
   CHECK(lista->getTamanio() == 1);
   CHECK(lista->getCabeza() == lista->getCola());
   CHECK(lista->getCabeza()->getDato()->getClave() == "A");
   CHECK(lista->getCabeza()->getAnterior() == nullptr);
   CHECK(lista->getCabeza()->getSiguiente() == nullptr);
   delete lista;
}

// Verifica que cada inserción al inicio quede delante: insertar A, B, C da C,B,A.
TEST_CASE("ListaDoble_insertarInicio_conElementos")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarInicio(new ElementoPrueba("A"));
   lista->insertarInicio(new ElementoPrueba("B"));
   lista->insertarInicio(new ElementoPrueba("C"));
   CHECK(lista->getTamanio() == 3);
   CHECK(lista->getCabeza()->getDato()->getClave() == "C");
   CHECK(lista->getCola()->getDato()->getClave() == "A");
   CHECK(RecorridoLista::adelante(lista) == "C,B,A");
   delete lista;
}

// Verifica que insertar nullptr se ignore: el tamaño no cambia.
TEST_CASE("ListaDoble_insertarInicio_nulo")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarInicio(nullptr);
   CHECK(lista->estaVacia() == true);
   CHECK(lista->getCabeza() == nullptr);
   delete lista;
}

// ---------------------------------------------------------------------
// insertarFinal
// ---------------------------------------------------------------------

// Verifica que en una lista vacía el único nodo sea a la vez cabeza y cola, sin vecinos.
TEST_CASE("ListaDoble_insertarFinal_listaVacia")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   CHECK(lista->getTamanio() == 1);
   CHECK(lista->getCabeza() == lista->getCola());
   CHECK(lista->getCola()->getAnterior() == nullptr);
   CHECK(lista->getCola()->getSiguiente() == nullptr);
   delete lista;
}

// Verifica que cada inserción al final quede detrás: insertar A, B, C da A,B,C.
TEST_CASE("ListaDoble_insertarFinal_conElementos")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(lista->getTamanio() == 3);
   CHECK(lista->getCabeza()->getDato()->getClave() == "A");
   CHECK(lista->getCola()->getDato()->getClave() == "C");
   CHECK(RecorridoLista::adelante(lista) == "A,B,C");
   delete lista;
}

// Verifica que insertar nullptr al final se ignore.
TEST_CASE("ListaDoble_insertarFinal_nulo")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(nullptr);
   CHECK(lista->getTamanio() == 1);
   CHECK(lista->getCola()->getDato()->getClave() == "A");
   delete lista;
}

// Verifica que mezclar inserciones al inicio y al final dé el orden esperado:
// final A, inicio B, final C, inicio D -> D,B,A,C.
TEST_CASE("ListaDoble_insertar_mezclado")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarInicio(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   lista->insertarInicio(new ElementoPrueba("D"));
   CHECK(RecorridoLista::adelante(lista) == "D,B,A,C");
   delete lista;
}

// ---------------------------------------------------------------------
// Enlaces dobles
// ---------------------------------------------------------------------

// Verifica que recorrer hacia adelante y hacia atrás dé el mismo contenido:
// si algún enlace anterior estuviera mal, el recorrido hacia atrás no coincidiría.
TEST_CASE("ListaDoble_enlacesDobles_recorridos")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarInicio(new ElementoPrueba("C"));
   lista->insertarFinal(new ElementoPrueba("D"));
   CHECK(RecorridoLista::adelante(lista) == "C,A,B,D");
   CHECK(RecorridoLista::atras(lista) == "C,A,B,D");
   // Los extremos no tienen vecinos hacia afuera
   CHECK(lista->getCabeza()->getAnterior() == nullptr);
   CHECK(lista->getCola()->getSiguiente() == nullptr);
   delete lista;
}

// ---------------------------------------------------------------------
// buscar
// ---------------------------------------------------------------------

// Verifica que buscar encuentre elementos al inicio, en el medio y al final.
TEST_CASE("ListaDoble_buscar_existente")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   REQUIRE(lista->buscar("A") != nullptr);
   REQUIRE(lista->buscar("B") != nullptr);
   REQUIRE(lista->buscar("C") != nullptr);
   CHECK(lista->buscar("B")->getClave() == "B");
   // Buscar no debe modificar la lista
   CHECK(lista->getTamanio() == 3);
   delete lista;
}

// Verifica que una clave inexistente (o vacía) devuelva nullptr.
TEST_CASE("ListaDoble_buscar_inexistente")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   CHECK(lista->buscar("Z") == nullptr);
   CHECK(lista->buscar("") == nullptr);
   delete lista;
}

// Verifica que buscar en una lista vacía devuelva nullptr sin fallar.
TEST_CASE("ListaDoble_buscar_listaVacia")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   CHECK(lista->buscar("A") == nullptr);
   delete lista;
}

// ---------------------------------------------------------------------
// obtener
// ---------------------------------------------------------------------

// Verifica obtener en el índice 0, uno del medio y el último (tamanio - 1).
TEST_CASE("ListaDoble_obtener_indicesValidos")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(lista->obtener(0)->getClave() == "A");
   CHECK(lista->obtener(1)->getClave() == "B");
   CHECK(lista->obtener(2)->getClave() == "C");
   delete lista;
}

// Verifica que un índice negativo o fuera de rango devuelva nullptr.
// El índice 3 es justo el primero fuera de rango en una lista de 3 elementos.
TEST_CASE("ListaDoble_obtener_fueraDeRango")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(lista->obtener(-1) == nullptr);
   CHECK(lista->obtener(3) == nullptr);
   CHECK(lista->obtener(100) == nullptr);
   delete lista;
}

// Verifica que obtener en una lista vacía devuelva nullptr, incluso con índice 0.
TEST_CASE("ListaDoble_obtener_listaVacia")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   CHECK(lista->obtener(0) == nullptr);
   delete lista;
}

// ---------------------------------------------------------------------
// eliminar
// ---------------------------------------------------------------------

// Verifica eliminar la cabeza: el segundo pasa a ser cabeza y no tiene anterior.
// También comprueba que el dato eliminado se haya liberado.
TEST_CASE("ListaDoble_eliminar_cabeza")
{
   int vivosAntes = ElementoPrueba::getVivos();
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(lista->eliminar("A") == true);
   CHECK(lista->getTamanio() == 2);
   CHECK(lista->getCabeza()->getDato()->getClave() == "B");
   CHECK(lista->getCabeza()->getAnterior() == nullptr);
   CHECK(RecorridoLista::adelante(lista) == "B,C");
   CHECK(RecorridoLista::atras(lista) == "B,C");
   CHECK(ElementoPrueba::getVivos() == vivosAntes + 2);   // A fue liberado
   delete lista;
}

// Verifica eliminar la cola: el penúltimo pasa a ser cola y no tiene siguiente.
TEST_CASE("ListaDoble_eliminar_cola")
{
   int vivosAntes = ElementoPrueba::getVivos();
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(lista->eliminar("C") == true);
   CHECK(lista->getTamanio() == 2);
   CHECK(lista->getCola()->getDato()->getClave() == "B");
   CHECK(lista->getCola()->getSiguiente() == nullptr);
   CHECK(RecorridoLista::adelante(lista) == "A,B");
   CHECK(RecorridoLista::atras(lista) == "A,B");
   CHECK(ElementoPrueba::getVivos() == vivosAntes + 2);   // C fue liberado
   delete lista;
}

// Verifica eliminar uno del medio: sus vecinos quedan enlazados entre sí en ambos sentidos.
TEST_CASE("ListaDoble_eliminar_medio")
{
   int vivosAntes = ElementoPrueba::getVivos();
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(lista->eliminar("B") == true);
   CHECK(lista->getTamanio() == 2);
   CHECK(lista->getCabeza()->getSiguiente() == lista->getCola());
   CHECK(lista->getCola()->getAnterior() == lista->getCabeza());
   CHECK(RecorridoLista::adelante(lista) == "A,C");
   CHECK(RecorridoLista::atras(lista) == "A,C");
   CHECK(ElementoPrueba::getVivos() == vivosAntes + 2);   // B fue liberado
   delete lista;
}

// Verifica eliminar el único elemento: la lista queda vacía, sin cabeza ni cola.
TEST_CASE("ListaDoble_eliminar_unicoElemento")
{
   int vivosAntes = ElementoPrueba::getVivos();
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   CHECK(lista->eliminar("A") == true);
   CHECK(lista->estaVacia() == true);
   CHECK(lista->getCabeza() == nullptr);
   CHECK(lista->getCola() == nullptr);
   CHECK(ElementoPrueba::getVivos() == vivosAntes);       // A fue liberado
   // La lista sigue usable después de quedar vacía
   lista->insertarFinal(new ElementoPrueba("B"));
   CHECK(RecorridoLista::adelante(lista) == "B");
   delete lista;
}

// Verifica que una clave inexistente devuelva false sin modificar la lista ni liberar nada.
TEST_CASE("ListaDoble_eliminar_inexistente")
{
   int vivosAntes = ElementoPrueba::getVivos();
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   CHECK(lista->eliminar("Z") == false);
   CHECK(lista->getTamanio() == 2);
   CHECK(RecorridoLista::adelante(lista) == "A,B");
   CHECK(ElementoPrueba::getVivos() == vivosAntes + 2);
   delete lista;
}

// Verifica que eliminar en una lista vacía devuelva false sin fallar.
TEST_CASE("ListaDoble_eliminar_listaVacia")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   CHECK(lista->eliminar("A") == false);
   CHECK(lista->estaVacia() == true);
   delete lista;
}

// ---------------------------------------------------------------------
// mostrarTodos
// ---------------------------------------------------------------------

// Verifica que mostrarTodos llame a mostrar() de cada elemento en orden de cabeza a cola.
TEST_CASE("ListaDoble_mostrarTodos_conElementos")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   std::string impreso;
   {
      CapturaSalidaLista captura;
      lista->mostrarTodos();
      impreso = captura.texto();
   }
   // ElementoPrueba imprime su clave seguida de un espacio
   CHECK(impreso == "A B ");
   delete lista;
}

// Verifica que con la lista vacía no se imprima nada.
TEST_CASE("ListaDoble_mostrarTodos_listaVacia")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   std::string impreso;
   {
      CapturaSalidaLista captura;
      lista->mostrarTodos();
      impreso = captura.texto();
   }
   CHECK(impreso.empty());
   delete lista;
}

// ---------------------------------------------------------------------
// getCabeza y getCola
// ---------------------------------------------------------------------

// Verifica que getCabeza y getCola apunten al primer y último nodo,
// y que se actualicen al insertar por cada extremo.
TEST_CASE("ListaDoble_getCabezaGetCola_extremos")
{
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarInicio(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(lista->getCabeza()->getDato()->getClave() == "A");
   CHECK(lista->getCola()->getDato()->getClave() == "C");
   delete lista;
}

// ---------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------

// Verifica que destruir una lista vacía no falle ni libere nada de más.
TEST_CASE("ListaDoble_destructor_listaVacia")
{
   int vivosAntes = ElementoPrueba::getVivos();
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   delete lista;
   CHECK(ElementoPrueba::getVivos() == vivosAntes);
}

// Verifica que destruir una lista llena libere todos sus datos:
// el contador de objetos vivos vuelve al valor que tenía antes de crearlos.
TEST_CASE("ListaDoble_destructor_listaLlena")
{
   int vivosAntes = ElementoPrueba::getVivos();
   ListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   lista->insertarFinal(new ElementoPrueba("C"));
   CHECK(ElementoPrueba::getVivos() == vivosAntes + 3);
   delete lista;
   CHECK(ElementoPrueba::getVivos() == vivosAntes);
}

// Verifica que la lista también se pueda usar y destruir a través de la interfaz IListaDoble<T>.
TEST_CASE("ListaDoble_destructor_desdeInterfaz")
{
   int vivosAntes = ElementoPrueba::getVivos();
   IListaDoble<ElementoPrueba>* lista = new ListaDoble<ElementoPrueba>();
   lista->insertarFinal(new ElementoPrueba("A"));
   lista->insertarFinal(new ElementoPrueba("B"));
   CHECK(lista->getTamanio() == 2);
   delete lista;   // el destructor virtual libera los datos de la lista concreta
   CHECK(ElementoPrueba::getVivos() == vivosAntes);
}
