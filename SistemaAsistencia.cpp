/***********************************************************************
 * Module:  SistemaAsistencia.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:28:38
 * Purpose: Implementation of the class SistemaAsistencia
 ***********************************************************************/

// Archivo: SistemaAsistencia.cpp
// Contiene la implementación de la clase SistemaAsistencia.

#include "Profesor.h"
#include "Asistencia.h"
#include "SistemaAsistencia.h"
#include "Validador.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::SistemaAsistencia()
// Purpose:    Implementation of SistemaAsistencia::SistemaAsistencia()
// Return:
////////////////////////////////////////////////////////////////////////

// Crea las dos listas vacías con new; el sistema es su dueño.
SistemaAsistencia::SistemaAsistencia()
   : profesores(new ListaDoble<Profesor>()), asistencias(new ListaDoble<Asistencia>())
{
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::~SistemaAsistencia()
// Purpose:    Implementation of SistemaAsistencia::~SistemaAsistencia()
// Return:
////////////////////////////////////////////////////////////////////////

// Al borrar cada lista, su destructor libera los objetos que contiene:
// los profesores liberan sus franjas, y las franjas y asistencias liberan sus horas.
SistemaAsistencia::~SistemaAsistencia()
{
   delete asistencias;
   delete profesores;
   asistencias = nullptr;
   profesores = nullptr;
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::agregarProfesor(Profesor* pro)
// Purpose:    Implementation of SistemaAsistencia::agregarProfesor()
// Parameters:
// - pro
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Valida los datos del profesor y lo guarda si todo es correcto.
// Si devuelve false NO se libera pro: quien llamó sigue siendo su dueño (decisión 4).
bool SistemaAsistencia::agregarProfesor(Profesor* pro)
{
   // Sin profesor no hay nada que agregar
   if (pro == nullptr)
   {
      return false;
   }

   // La cédula debe ser ecuatoriana y real (formato, provincia, tercer dígito y verificador)
   if (!Validador::validarCedula(pro->getCedula()))
   {
      return false;
   }

   // Nombre y apellido: solo letras (con tildes y ñ) separadas por un espacio
   if (!Validador::validarNombre(pro->getNombre()) || !Validador::validarNombre(pro->getApellido()))
   {
      return false;
   }

   // No se permiten cédulas duplicadas
   if (profesores->buscar(pro->getCedula()) != nullptr)
   {
      return false;
   }

   // Desde aquí la lista de profesores es dueña del objeto
   profesores->insertarFinal(pro);
   return true;
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::agregarFranja(std::string cedula, FranjaHoraria* fra)
// Purpose:    Implementation of SistemaAsistencia::agregarFranja()
// Parameters:
// - cedula
// - fra
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Busca al profesor, valida el aula y delega en el profesor las reglas de la franja.
// Si devuelve false NO se libera fra: quien llamó sigue siendo su dueño (decisión 4).
bool SistemaAsistencia::agregarFranja(std::string cedula, FranjaHoraria* fra)
{
   // Sin franja no hay nada que agregar
   if (fra == nullptr)
   {
      return false;
   }

   // La franja solo se puede asignar a un profesor registrado
   Profesor* profesor = profesores->buscar(cedula);
   if (profesor == nullptr)
   {
      return false;
   }

   // El aula debe tener un formato válido (letras o dígitos, con guiones entre ellos)
   if (!Validador::validarAula(fra->getAula()))
   {
      return false;
   }

   // El profesor comprueba que sea predefinida, de su cédula, no duplicada y sin cruces.
   // Si la acepta, el profesor pasa a ser dueño de la franja
   return profesor->agregarFranja(fra);
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::registrarEntrada(std::string cedula, std::string fecha, Hora* hora)
// Purpose:    Implementation of SistemaAsistencia::registrarEntrada()
// Parameters:
// - cedula
// - fecha
// - hora
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Registra la entrada si la hora cae en la ventana de entrada de alguna franja del profesor
// y todavía no existe la asistencia de esa franja y esa fecha.
bool SistemaAsistencia::registrarEntrada(std::string cedula, std::string fecha, Hora* hora)
{
   // La fecha debe ser AAAA-MM-DD y existir en el calendario; sin hora no hay nada que comparar
   if (!Validador::validarFecha(fecha) || hora == nullptr)
   {
      return false;
   }

   // El profesor debe estar registrado
   Profesor* profesor = profesores->buscar(cedula);
   if (profesor == nullptr)
   {
      return false;
   }

   // Debe existir una franja cuya ventana de entrada [inicio, inicio + 10] contenga la hora.
   // Si no hay ninguna, el profesor llegó tarde, temprano o no tiene clase a esa hora:
   // el registro se rechaza (no se marca atrasado)
   FranjaHoraria* franja = profesor->buscarFranjaParaEntrada(hora);
   if (franja == nullptr)
   {
      return false;
   }

   // Se crea la asistencia de esa franja y fecha (guarda su propia copia de la hora de inicio)
   Asistencia* nueva = new Asistencia(cedula, fecha, franja->getHoraInicio());

   // Solo puede haber una asistencia por cédula + fecha + franja: si la clave ya existe,
   // la entrada ya se registró antes. Como la nueva no se guardará, se libera aquí
   if (asistencias->buscar(nueva->getClave()) != nullptr)
   {
      delete nueva;
      return false;
   }

   // Se registra la entrada (estado 0 -> 1) y la lista pasa a ser dueña de la asistencia.
   // hora no se libera: la asistencia guardó una copia y la original es de quien llamó
   nueva->registrarEntrada(hora);
   asistencias->insertarFinal(nueva);
   return true;
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::registrarSalida(std::string cedula, std::string fecha, Hora* hora)
// Purpose:    Implementation of SistemaAsistencia::registrarSalida()
// Parameters:
// - cedula
// - fecha
// - hora
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Registra la salida si la hora cae en la ventana de salida de alguna franja del profesor
// y en esa franja y fecha ya se registró la entrada.
bool SistemaAsistencia::registrarSalida(std::string cedula, std::string fecha, Hora* hora)
{
   // La fecha debe ser AAAA-MM-DD y existir en el calendario; sin hora no hay nada que comparar
   if (!Validador::validarFecha(fecha) || hora == nullptr)
   {
      return false;
   }

   // El profesor debe estar registrado
   Profesor* profesor = profesores->buscar(cedula);
   if (profesor == nullptr)
   {
      return false;
   }

   // Debe existir una franja cuya ventana de salida [final, final + 10] contenga la hora.
   // Con franjas consecutivas (7-9 y 9-11), a las 09:05 esta es la franja 7-9
   FranjaHoraria* franja = profesor->buscarFranjaParaSalida(hora);
   if (franja == nullptr)
   {
      return false;
   }

   // Para obtener la clave se usa una asistencia temporal, así el formato de la clave
   // se define en un solo lugar (Asistencia::getClave). Se libera enseguida
   Asistencia* consulta = new Asistencia(cedula, fecha, franja->getHoraInicio());
   std::string clave = consulta->getClave();
   delete consulta;

   // No se puede registrar salida sin una entrada previa en esa franja y fecha
   Asistencia* asistencia = asistencias->buscar(clave);
   if (asistencia == nullptr)
   {
      return false;
   }

   // Solo se registra la salida si está en estado 1; si ya está completa se rechaza
   if (!asistencia->tieneEntrada())
   {
      return false;
   }

   // Estado 1 -> 2. hora no se libera: la asistencia guarda una copia
   asistencia->registrarSalida(hora);
   return true;
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::mostrarAsistencias()
// Purpose:    Implementation of SistemaAsistencia::mostrarAsistencias()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Muestra todas las asistencias en el orden en que se registraron.
void SistemaAsistencia::mostrarAsistencias(void) const
{
   if (asistencias->estaVacia())
   {
      std::cout << "No hay asistencias registradas." << std::endl;
      return;
   }
   asistencias->mostrarTodos();
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::mostrarAsistenciaProfesor(std::string cedula)
// Purpose:    Implementation of SistemaAsistencia::mostrarAsistenciaProfesor()
// Parameters:
// - cedula
// Return:     void
////////////////////////////////////////////////////////////////////////

// Recorre las asistencias y muestra solo las de esa cédula.
void SistemaAsistencia::mostrarAsistenciaProfesor(std::string cedula) const
{
   bool encontro = false;   // indica si se mostró al menos una asistencia
   Nodo<Asistencia>* actual = asistencias->getCabeza();
   while (actual != nullptr)
   {
      if (actual->getDato()->getCedulaProfesor() == cedula)
      {
         actual->getDato()->mostrar();
         encontro = true;
      }
      actual = actual->getSiguiente();
   }

   if (!encontro)
   {
      std::cout << "No hay asistencias registradas para la cédula " << cedula << "." << std::endl;
   }
}
