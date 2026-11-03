# Sistema de Asistencia Docente

Proyecto de la Unidad 1, hecho en C++ con programación orientada a objetos, plantillas (templates)
y una lista doblemente enlazada propia.

## ¿Qué hace?

Un profesor ingresa su **cédula** y el sistema registra si está **iniciando** o **finalizando** su clase,
según la hora y las franjas horarias que tenga asignadas.

## Franjas horarias

Las franjas son fijas: bloques de 2 horas entre las 07:00 y las 21:00.

| Franja | Horario       | Entrada       | Salida        |
|--------|---------------|---------------|---------------|
| 1      | 07:00 – 09:00 | 07:00 – 07:10 | 09:00 – 09:10 |
| 2      | 09:00 – 11:00 | 09:00 – 09:10 | 11:00 – 11:10 |
| 3      | 11:00 – 13:00 | 11:00 – 11:10 | 13:00 – 13:10 |
| 4      | 13:00 – 15:00 | 13:00 – 13:10 | 15:00 – 15:10 |
| 5      | 15:00 – 17:00 | 15:00 – 15:10 | 17:00 – 17:10 |
| 6      | 17:00 – 19:00 | 17:00 – 17:10 | 19:00 – 19:10 |
| 7      | 19:00 – 21:00 | 19:00 – 19:10 | 21:00 – 21:10 |

## Reglas

- La entrada se registra desde la hora de inicio hasta 10 minutos después.
- La salida se registra desde la hora final hasta 10 minutos después.
- Fuera de esos 10 minutos, el registro se rechaza.
- Cada franja se registra por separado.
- No se puede registrar la salida sin haber registrado la entrada.
- Un profesor no puede tener franjas repetidas ni cruzadas.
- No se permiten cédulas repetidas.

## Validaciones

Se hacen con expresiones regulares en la clase `Validador`:

- **Cédula:** 10 dígitos, y además se comprueba que sea una cédula ecuatoriana real (provincia y dígito verificador).
- **Nombre y apellido:** solo letras (con tildes y ñ) y espacios.
- **Hora:** formato `HH:MM` de 24 horas.
- **Fecha:** formato `AAAA-MM-DD`, con control de días por mes y años bisiestos.
- **Opciones del menú:** solo números.

## Estructura del proyecto

```
├── include/   archivos .h (declaraciones de las clases)
├── src/       archivos .cpp (implementación y main.cpp)
└── tests/     pruebas de unidad y la librería doctest
```

| Archivo | Carpeta | Descripción |
|---|---|---|
| `IMostrable.h`, `IIdentificable.h`, `IListaDoble.h` | `include/` | Interfaces |
| `Nodo.h`, `ListaDoble.h` | `include/` | Lista doble genérica (templates, solo `.h`) |
| `Hora` | `include/` y `src/` | Guarda una hora como minutos desde medianoche |
| `Persona`, `Profesor` | `include/` y `src/` | Datos del profesor y sus franjas |
| `FranjaHoraria` | `include/` y `src/` | Horario, aula y reglas de la ventana de 10 minutos |
| `Asistencia` | `include/` y `src/` | Registro de entrada y salida |
| `Validador` | `include/` y `src/` | Validaciones con expresiones regulares |
| `SistemaAsistencia` | `include/` y `src/` | Lógica principal del sistema |
| `Menu` | `include/` y `src/` | Interacción con el usuario |
| `main.cpp` | `src/` | Punto de entrada |
| `test_<Clase>.cpp`, `pruebas_main.cpp` | `tests/` | Pruebas de unidad de cada clase |
| `doctest.h`, `ElementoPrueba.h` | `tests/` | Librería de pruebas y clase auxiliar para probar la lista |

El diagrama de clases se hizo en PowerDesigner.

## Cómo compilar y ejecutar

Se necesita `g++` con soporte para C++17. Los comandos se ejecutan en la terminal, dentro de la carpeta principal del proyecto.

La opción `-Iinclude` le indica al compilador dónde están los `.h`, y `-Itests` dónde está `doctest.h`.

**Programa:**

```
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/Hora.cpp src/Validador.cpp src/Persona.cpp src/FranjaHoraria.cpp src/Profesor.cpp src/Asistencia.cpp src/SistemaAsistencia.cpp src/Menu.cpp -o asistencia.exe
.\asistencia.exe
```

**Pruebas de unidad:**

```
g++ -std=c++17 -Wall -Wextra -Iinclude -Itests tests/pruebas_main.cpp tests/test_*.cpp src/Hora.cpp src/Validador.cpp src/Persona.cpp src/FranjaHoraria.cpp src/Profesor.cpp src/Asistencia.cpp src/SistemaAsistencia.cpp src/Menu.cpp -o pruebas.exe
.\pruebas.exe
```

Opciones útiles de las pruebas:

- `.\pruebas.exe -s` muestra también las pruebas que pasan.
- `.\pruebas.exe -tc="Hora*"` ejecuta solo las pruebas cuyo nombre empieza con `Hora`.

## Uso de la hora y la fecha

Al registrar una entrada o salida, el programa permite:

- Presionar **Enter** para usar la hora y la fecha actuales del sistema.
- Escribir una hora (`HH:MM`) o fecha (`AAAA-MM-DD`) manualmente, útil para probar las reglas de los 10 minutos.

## Tildes y ñ en la consola de Windows

El programa configura la consola en UTF-8 al iniciar, así que no hace falta ejecutar ningún comando antes.
