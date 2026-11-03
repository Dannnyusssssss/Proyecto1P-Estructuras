/***********************************************************************
 * Module:  Menu.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 11:22:21
 * Purpose: Implementation of the class Menu
 ***********************************************************************/

// Archivo: Menu.cpp
// Contiene la implementación de la clase Menu.
// Toda la entrada se lee por líneas completas con std::getline, para que el Enter vacío
// se pueda detectar (usar la fecha u hora actual) y no queden restos en el buffer.

#include "SistemaAsistencia.h"
#include "Menu.h"
#include "Validador.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

////////////////////////////////////////////////////////////////////////
// Name:       Menu::Menu()
// Purpose:    Implementation of Menu::Menu()
// Return:
////////////////////////////////////////////////////////////////////////

// Crea el sistema con new; el menú es su dueño.
Menu::Menu() : sistema(new SistemaAsistencia())
{
}

////////////////////////////////////////////////////////////////////////
// Name:       Menu::~Menu()
// Purpose:    Implementation of Menu::~Menu()
// Return:
////////////////////////////////////////////////////////////////////////

// Al liberar el sistema se liberan en cascada sus listas y todo lo que contienen.
Menu::~Menu()
{
   delete sistema;
   sistema = nullptr;
}

////////////////////////////////////////////////////////////////////////
// Name:       Menu::iniciar()
// Purpose:    Implementation of Menu::iniciar()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Bucle del menú principal: se repite hasta elegir 0 o hasta que se termine la entrada.
void Menu::iniciar(void)
{
   std::string linea;
   while (true)
   {
      std::cout << std::endl;
      std::cout << "=== SISTEMA DE ASISTENCIA DOCENTE ===" << std::endl;
      std::cout << "1. Administrador" << std::endl;
      std::cout << "2. Profesor" << std::endl;
      std::cout << "0. Salir" << std::endl;
      std::cout << "Opción: ";

      // Si la entrada se terminó (fin de archivo o error), no hay nada más que leer
      if (!std::getline(std::cin, linea))
      {
         return;
      }

      // El Validador comprueba que sea un número de 1 o 2 dígitos; el rango se revisa aquí
      if (!Validador::validarOpcionMenu(linea))
      {
         std::cout << "Opción inválida: escriba solo el número de la opción." << std::endl;
         continue;
      }

      // stoi es seguro: la regex garantizó que son 1 o 2 dígitos
      int opcion = std::stoi(linea);
      if (opcion == 1)
      {
         menuAdministrador();
      }
      else if (opcion == 2)
      {
         menuProfesor();
      }
      else if (opcion == 0)
      {
         std::cout << "Hasta pronto." << std::endl;
         return;
      }
      else
      {
         std::cout << "Opción fuera de rango: elija 0, 1 o 2." << std::endl;
      }
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Menu::menuAdministrador()
// Purpose:    Implementation of Menu::menuAdministrador()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Bucle del menú de administrador: se repite hasta elegir 0 (volver).
void Menu::menuAdministrador(void)
{
   std::string linea;
   while (true)
   {
      std::cout << std::endl;
      std::cout << "--- ADMINISTRADOR ---" << std::endl;
      std::cout << "1. Registrar profesor" << std::endl;
      std::cout << "2. Asignar franja a un profesor" << std::endl;
      std::cout << "3. Ver todas las asistencias" << std::endl;
      std::cout << "4. Ver asistencias de un profesor" << std::endl;
      std::cout << "5. Ver profesores" << std::endl;
      std::cout << "0. Volver" << std::endl;
      std::cout << "Opción: ";

      if (!std::getline(std::cin, linea))
      {
         return;   // se terminó la entrada
      }
      if (!Validador::validarOpcionMenu(linea))
      {
         std::cout << "Opción inválida: escriba solo el número de la opción." << std::endl;
         continue;
      }
      int opcion = std::stoi(linea);

      if (opcion == 0)
      {
         return;
      }
      else if (opcion == 1)
      {
         // --- Registrar profesor ---
         std::string cedula = leerCedula();   // ya validada con el algoritmo de módulo 10
         if (cedula.empty())
         {
            return;   // se terminó la entrada
         }

         // Nombre: se repite hasta que pase el Validador (solo letras, tildes y ñ)
         std::string nombre;
         while (true)
         {
            std::cout << "Nombre: ";
            if (!std::getline(std::cin, nombre))
            {
               return;
            }
            if (Validador::validarNombre(nombre))
            {
               break;
            }
            std::cout << "Nombre inválido: use solo letras, separando las palabras con un espacio." << std::endl;
         }

         // Apellido: mismas reglas que el nombre
         std::string apellido;
         while (true)
         {
            std::cout << "Apellido: ";
            if (!std::getline(std::cin, apellido))
            {
               return;
            }
            if (Validador::validarNombre(apellido))
            {
               break;
            }
            std::cout << "Apellido inválido: use solo letras, separando las palabras con un espacio." << std::endl;
         }

         // Los datos ya se validaron, así que la única razón de rechazo es una cédula repetida
         Profesor* profesor = new Profesor(cedula, nombre, apellido);
         if (sistema->agregarProfesor(profesor))
         {
            std::cout << "Profesor registrado correctamente." << std::endl;   // el sistema es su dueño
         }
         else
         {
            std::cout << "No se pudo registrar: ya existe un profesor con esa cédula." << std::endl;
            delete profesor;   // rechazado: el menú sigue siendo su dueño y lo libera (decisión 4)
         }
      }
      else if (opcion == 2)
      {
         // --- Asignar franja (decisión 12: se elige una de las 7 franjas predefinidas) ---
         std::string cedula = leerCedula();
         if (cedula.empty())
         {
            return;
         }

         // Lista numerada de las 7 franjas: la franja n empieza a las 7 + 2*(n-1) horas
         std::cout << "Franjas disponibles:" << std::endl;
         for (int n = 1; n <= 7; n++)
         {
            int inicio = 7 + 2 * (n - 1);
            std::cout << n << ". " << std::setw(2) << std::setfill('0') << inicio << ":00 - "
                      << std::setw(2) << std::setfill('0') << (inicio + 2) << ":00" << std::endl;
         }

         // Número de franja: debe pasar el Validador y estar entre 1 y 7
         int numero = 0;
         while (true)
         {
            std::cout << "Número de franja (1-7): ";
            if (!std::getline(std::cin, linea))
            {
               return;
            }
            if (Validador::validarOpcionMenu(linea))
            {
               numero = std::stoi(linea);
               if (numero >= 1 && numero <= 7)
               {
                  break;
               }
            }
            std::cout << "Número inválido: elija una franja del 1 al 7." << std::endl;
         }

         // Aula: letras o dígitos, con guiones entre ellos
         std::string aula;
         while (true)
         {
            std::cout << "Aula (ej. A-101): ";
            if (!std::getline(std::cin, aula))
            {
               return;
            }
            if (Validador::validarAula(aula))
            {
               break;
            }
            std::cout << "Aula inválida: use letras o números, con guiones solo entre ellos." << std::endl;
         }

         // El menú crea la franja con las horas del bloque elegido; la franja se queda con ambas horas
         int horaInicio = 7 + 2 * (numero - 1);
         FranjaHoraria* franja = new FranjaHoraria(new Hora(horaInicio, 0), new Hora(horaInicio + 2, 0),
                                                   aula, cedula);
         if (sistema->agregarFranja(cedula, franja))
         {
            std::cout << "Franja asignada correctamente." << std::endl;   // el profesor es su dueño
         }
         else
         {
            // La franja es predefinida y el aula es válida: queda profesor inexistente o franja repetida
            std::cout << "No se pudo asignar: verifique que el profesor esté registrado "
                         "y que no tenga ya esa franja." << std::endl;
            delete franja;   // rechazada: el menú la libera, junto con sus dos horas (decisión 4)
         }
      }
      else if (opcion == 3)
      {
         // --- Ver todas las asistencias ---
         sistema->mostrarAsistencias();
      }
      else if (opcion == 4)
      {
         // --- Ver asistencias de un profesor ---
         std::string cedula = leerCedula();
         if (cedula.empty())
         {
            return;
         }
         sistema->mostrarAsistenciaProfesor(cedula);
      }
      else if (opcion == 5)
      {
         // --- Ver profesores (con sus franjas) ---
         sistema->mostrarProfesores();
      }
      else
      {
         std::cout << "Opción fuera de rango: elija un número del 0 al 5." << std::endl;
      }
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Menu::menuProfesor()
// Purpose:    Implementation of Menu::menuProfesor()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Bucle del menú de profesor: se repite hasta elegir 0 (volver).
void Menu::menuProfesor(void)
{
   std::string linea;
   while (true)
   {
      std::cout << std::endl;
      std::cout << "--- PROFESOR ---" << std::endl;
      std::cout << "1. Registrar entrada" << std::endl;
      std::cout << "2. Registrar salida" << std::endl;
      std::cout << "3. Ver mis asistencias" << std::endl;
      std::cout << "0. Volver" << std::endl;
      std::cout << "Opción: ";

      if (!std::getline(std::cin, linea))
      {
         return;   // se terminó la entrada
      }
      if (!Validador::validarOpcionMenu(linea))
      {
         std::cout << "Opción inválida: escriba solo el número de la opción." << std::endl;
         continue;
      }
      int opcion = std::stoi(linea);

      if (opcion == 0)
      {
         return;
      }
      else if (opcion == 1 || opcion == 2)
      {
         // --- Registrar entrada (1) o salida (2): mismos datos, distinta regla ---
         std::string cedula = leerCedula();
         if (cedula.empty())
         {
            return;
         }
         std::string fecha = leerFecha();
         if (fecha.empty())
         {
            return;
         }
         Hora* hora = leerHora();   // objeto nuevo: el menú es su dueño y lo libera al final
         if (hora == nullptr)
         {
            return;
         }

         // El sistema solo consulta la hora (la asistencia guarda su propia copia)
         bool registrado = (opcion == 1)
                         ? sistema->registrarEntrada(cedula, fecha, hora)
                         : sistema->registrarSalida(cedula, fecha, hora);

         if (registrado)
         {
            std::cout << ((opcion == 1) ? "Entrada" : "Salida") << " registrada: "
                      << fecha << " a las " << hora->toString() << "." << std::endl;
         }
         else if (opcion == 1)
         {
            // La cédula, la fecha y la hora ya se validaron: el rechazo es por una regla de negocio
            std::cout << "Registro rechazado: verifique que el profesor esté registrado, que tenga una franja "
                         "que empiece a esa hora, que esté dentro de los 10 minutos de inicio y que no haya "
                         "registrado ya esta entrada." << std::endl;
         }
         else
         {
            std::cout << "Registro rechazado: verifique que el profesor esté registrado, que esté dentro de "
                         "los 10 minutos después del fin de su franja, que haya registrado la entrada de esa "
                         "franja en esa fecha y que no haya registrado ya la salida." << std::endl;
         }

         delete hora;   // el menú creó la hora con leerHora y ya no la necesita
      }
      else if (opcion == 3)
      {
         // --- Ver mis asistencias ---
         std::string cedula = leerCedula();
         if (cedula.empty())
         {
            return;
         }
         sistema->mostrarAsistenciaProfesor(cedula);
      }
      else
      {
         std::cout << "Opción fuera de rango: elija un número del 0 al 3." << std::endl;
      }
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Menu::leerCedula()
// Purpose:    Implementation of Menu::leerCedula()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Pide la cédula hasta que el Validador la acepte como cédula ecuatoriana real.
std::string Menu::leerCedula(void)
{
   std::string cedula;
   while (true)
   {
      std::cout << "Cédula (10 dígitos): ";
      if (!std::getline(std::cin, cedula))
      {
         return "";   // se terminó la entrada: "" le indica a quien llama que debe salir
      }
      if (Validador::validarCedula(cedula))
      {
         return cedula;
      }
      std::cout << "Cédula inválida: debe tener 10 dígitos, una provincia válida y el dígito verificador correcto."
                << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Menu::leerHora()
// Purpose:    Implementation of Menu::leerHora()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

// Lee la hora escrita por el usuario o, si presiona Enter, toma la hora actual del sistema.
Hora* Menu::leerHora(void)
{
   std::string texto;
   while (true)
   {
      std::cout << "Ingrese la hora (HH:MM) o presione Enter para usar la hora actual: ";
      if (!std::getline(std::cin, texto))
      {
         return nullptr;   // se terminó la entrada
      }

      // Enter sin texto: se consulta el reloj. time() da los segundos desde 1970 y
      // localtime() los convierte a la hora local (horas y minutos por separado)
      if (texto.empty())
      {
         std::time_t ahora = std::time(nullptr);
         std::tm* local = std::localtime(&ahora);
         Hora* actual = new Hora(local->tm_hour, local->tm_min);   // quien llama la libera
         std::cout << "Hora actual: " << actual->toString() << std::endl;
         return actual;
      }

      // Hora escrita: debe ser HH:MM de 24 horas (así se puede demostrar 07:11 -> rechazada)
      if (Validador::validarHora(texto))
      {
         return new Hora(texto);   // quien llama la libera
      }
      std::cout << "Hora inválida: use el formato HH:MM de 24 horas (ej. 07:05)." << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Menu::leerFecha()
// Purpose:    Implementation of Menu::leerFecha()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Lee la fecha escrita por el usuario o, si presiona Enter, toma la fecha actual del sistema.
std::string Menu::leerFecha(void)
{
   std::string texto;
   while (true)
   {
      std::cout << "Ingrese la fecha (AAAA-MM-DD) o presione Enter para usar la fecha actual: ";
      if (!std::getline(std::cin, texto))
      {
         return "";   // se terminó la entrada
      }

      // Enter sin texto: se consulta el reloj y se arma el texto AAAA-MM-DD con put_time
      // (%Y = año de 4 dígitos, %m = mes y %d = día, ambos con cero a la izquierda)
      if (texto.empty())
      {
         std::time_t ahora = std::time(nullptr);
         std::tm* local = std::localtime(&ahora);
         std::ostringstream salida;
         salida << std::put_time(local, "%Y-%m-%d");
         std::cout << "Fecha actual: " << salida.str() << std::endl;
         return salida.str();
      }

      // Fecha escrita: formato, mes, días del mes y año bisiesto los revisa el Validador
      if (Validador::validarFecha(texto))
      {
         return texto;
      }
      std::cout << "Fecha inválida: use el formato AAAA-MM-DD con una fecha que exista (ej. 2026-11-03)."
                << std::endl;
   }
}
