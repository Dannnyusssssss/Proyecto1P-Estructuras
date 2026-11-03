/***********************************************************************
 * Module:  Nodo.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:34:05
 * Purpose: Declaration of the class Nodo
 ***********************************************************************/

// Archivo: Nodo.h
// Contiene la clase genérica Nodo<T>, el eslabón de la lista doblemente enlazada.
// Al ser un template, su implementación está en este mismo archivo y no en un .cpp:
// el compilador necesita ver el código completo del template en el momento de usarlo
// con un tipo concreto (por ejemplo, Nodo<Profesor>), y desde un .cpp aparte no lo vería.

#if !defined(__Proyecto_Nodo_h)
#define __Proyecto_Nodo_h

// Clase Nodo<T>
// Representa un eslabón de la lista doble: guarda un puntero al dato y punteros
// al nodo siguiente y al anterior.
// El nodo NO es dueño de nada: no libera su dato ni a sus vecinos.
// Quien crea y libera los nodos y los datos es ListaDoble<T>.
template<typename T>
class Nodo
{
public:
   // Crea un nodo que guarda el puntero _dato (puede ser nullptr), sin vecinos.
   Nodo(T* _dato);

   // Devuelve el puntero al dato guardado (puede ser nullptr).
   T* getDato(void) const;

   // Reemplaza el dato guardado. No libera el dato anterior: eso le corresponde a la lista.
   void setDato(T* newDato);

   // Devuelve el nodo siguiente, o nullptr si este es el último.
   Nodo<T>* getSiguiente(void) const;

   // Enlaza este nodo con newSiguiente (nullptr para desenlazarlo hacia adelante).
   // Solo cambia el enlace de este nodo; el enlace de regreso lo ajusta la lista.
   void setSiguiente(Nodo<T>* newSiguiente);

   // Devuelve el nodo anterior, o nullptr si este es el primero.
   Nodo<T>* getAnterior(void) const;

   // Enlaza este nodo con newAnterior (nullptr para desenlazarlo hacia atrás).
   // Solo cambia el enlace de este nodo; el enlace de regreso lo ajusta la lista.
   void setAnterior(Nodo<T>* newAnterior);

protected:
private:
   T* dato;             // puntero al elemento guardado (la lista es su dueña)
   Nodo<T>* siguiente;  // nodo que viene después, o nullptr si es el último
   Nodo<T>* anterior;   // nodo que viene antes, o nullptr si es el primero


};

// ---------------------------------------------------------------------
// Implementación de los métodos (en el .h por ser un template)
// ---------------------------------------------------------------------

// Guarda el dato y deja el nodo suelto: todavía no pertenece a ninguna cadena,
// así que no tiene ni siguiente ni anterior.
template<typename T>
Nodo<T>::Nodo(T* _dato) : dato(_dato), siguiente(nullptr), anterior(nullptr)
{
}

// Devuelve el dato guardado sin transferir su propiedad.
template<typename T>
T* Nodo<T>::getDato(void) const
{
   return dato;
}

// Cambia el dato guardado. El anterior no se libera aquí porque el nodo no es su dueño.
template<typename T>
void Nodo<T>::setDato(T* newDato)
{
   dato = newDato;
}

// Devuelve el enlace hacia adelante.
template<typename T>
Nodo<T>* Nodo<T>::getSiguiente(void) const
{
   return siguiente;
}

// Cambia el enlace hacia adelante.
template<typename T>
void Nodo<T>::setSiguiente(Nodo<T>* newSiguiente)
{
   siguiente = newSiguiente;
}

// Devuelve el enlace hacia atrás.
template<typename T>
Nodo<T>* Nodo<T>::getAnterior(void) const
{
   return anterior;
}

// Cambia el enlace hacia atrás.
template<typename T>
void Nodo<T>::setAnterior(Nodo<T>* newAnterior)
{
   anterior = newAnterior;
}

#endif
