// Archivo: test_IListaDoble.cpp
// Pruebas de unidad de la interfaz IListaDoble<T>.
// Como la interfaz no tiene lógica, se usa una implementación mínima de prueba
// que solo registra qué método se llamó, para comprobar que los 8 métodos
// se pueden invocar a través de un puntero a la interfaz y que el destructor es virtual.
// La lógica real de la lista se prueba en test_ListaDoble.cpp.

#include "doctest.h"
#include "IListaDoble.h"
#include <string>

// Clase de prueba que implementa IListaDoble<int>.
// No guarda los datos: solo cuenta inserciones y recuerda el último método llamado.
class ListaRegistroDePrueba : public IListaDoble<int>
{
public:
   // Recibe la dirección de la bandera de destrucción (puede ser nullptr).
   explicit ListaRegistroDePrueba(bool* bandera) : contador(0), ultimaLlamada(""), destruido(bandera) {}

   // Al destruirse marca la bandera (si se entregó una) como verdadera.
   ~ListaRegistroDePrueba() override
   {
      if (destruido != nullptr)
      {
         *destruido = true;
      }
   }

   // Cuenta la inserción y registra el método; el dato no se guarda.
   void insertarInicio(int*) override
   {
      contador++;
      ultimaLlamada = "insertarInicio";
   }

   // Cuenta la inserción y registra el método; el dato no se guarda.
   void insertarFinal(int*) override
   {
      contador++;
      ultimaLlamada = "insertarFinal";
   }

   // Devuelve true solo si la clave es "existe", para simular ambos resultados.
   bool eliminar(std::string clave) override
   {
      ultimaLlamada = "eliminar";
      return clave == "existe";
   }

   // Nunca encuentra nada: devuelve nullptr.
   int* buscar(std::string) const override
   {
      return nullptr;
   }

   // Nunca tiene elementos reales: devuelve nullptr para cualquier índice.
   int* obtener(int) const override
   {
      return nullptr;
   }

   // Está vacía si no se ha hecho ninguna inserción.
   bool estaVacia(void) const override
   {
      return contador == 0;
   }

   // Devuelve la cantidad de inserciones realizadas.
   int getTamanio(void) const override
   {
      return contador;
   }

   // No imprime nada; existe solo para cumplir el contrato.
   void mostrarTodos(void) const override
   {
   }

   // Devuelve el nombre del último método modificador que se llamó.
   std::string getUltimaLlamada(void) const
   {
      return ultimaLlamada;
   }

private:
   int contador;               // cantidad de inserciones realizadas
   std::string ultimaLlamada;  // nombre del último método modificador invocado
   bool* destruido;            // bandera externa de destrucción (no es dueña)
};

// Verifica que una lista recién creada, vista desde la interfaz, esté vacía y con tamaño 0.
TEST_CASE("IListaDoble_estaVacia_listaNueva")
{
   IListaDoble<int>* lista = new ListaRegistroDePrueba(nullptr);
   CHECK(lista->estaVacia() == true);
   CHECK(lista->getTamanio() == 0);
   delete lista;
}

// Verifica que insertarInicio se despache a la clase concreta a través de la interfaz.
TEST_CASE("IListaDoble_insertarInicio_implementacion")
{
   ListaRegistroDePrueba* concreta = new ListaRegistroDePrueba(nullptr);
   IListaDoble<int>* lista = concreta;
   lista->insertarInicio(nullptr);
   // La clase concreta registró la llamada y contó un elemento
   CHECK(concreta->getUltimaLlamada() == "insertarInicio");
   CHECK(lista->getTamanio() == 1);
   CHECK(lista->estaVacia() == false);
   delete lista;
}

// Verifica que insertarFinal se despache a la clase concreta a través de la interfaz.
TEST_CASE("IListaDoble_insertarFinal_implementacion")
{
   ListaRegistroDePrueba* concreta = new ListaRegistroDePrueba(nullptr);
   IListaDoble<int>* lista = concreta;
   lista->insertarFinal(nullptr);
   lista->insertarFinal(nullptr);
   // Dos inserciones: el tamaño debe ser 2 y la última llamada insertarFinal
   CHECK(concreta->getUltimaLlamada() == "insertarFinal");
   CHECK(lista->getTamanio() == 2);
   delete lista;
}

// Verifica que eliminar devuelva el resultado de la clase concreta (true y false).
TEST_CASE("IListaDoble_eliminar_implementacion")
{
   IListaDoble<int>* lista = new ListaRegistroDePrueba(nullptr);
   // La clase de prueba solo "elimina" la clave "existe"
   CHECK(lista->eliminar("existe") == true);
   CHECK(lista->eliminar("noExiste") == false);
   CHECK(lista->eliminar("") == false);
   delete lista;
}

// Verifica que buscar se pueda llamar a través de la interfaz y devuelva nullptr
// cuando la clase concreta no encuentra nada.
TEST_CASE("IListaDoble_buscar_implementacion")
{
   IListaDoble<int>* lista = new ListaRegistroDePrueba(nullptr);
   CHECK(lista->buscar("cualquiera") == nullptr);
   delete lista;
}

// Verifica que obtener se pueda llamar a través de la interfaz con índices
// válidos, negativos y fuera de rango.
TEST_CASE("IListaDoble_obtener_implementacion")
{
   IListaDoble<int>* lista = new ListaRegistroDePrueba(nullptr);
   CHECK(lista->obtener(0) == nullptr);
   CHECK(lista->obtener(-1) == nullptr);
   CHECK(lista->obtener(100) == nullptr);
   delete lista;
}

// Verifica que mostrarTodos se pueda llamar a través de la interfaz sin fallar.
TEST_CASE("IListaDoble_mostrarTodos_implementacion")
{
   IListaDoble<int>* lista = new ListaRegistroDePrueba(nullptr);
   CHECK_NOTHROW(lista->mostrarTodos());
   delete lista;
}

// Verifica que delete sobre un IListaDoble<int>* ejecute el destructor de la clase concreta.
// Si el destructor de la interfaz no fuera virtual, la bandera quedaría en false.
TEST_CASE("IListaDoble_destructor_virtual")
{
   bool destruido = false;
   IListaDoble<int>* lista = new ListaRegistroDePrueba(&destruido);
   delete lista;
   CHECK(destruido == true);
}
