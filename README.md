# Entregable 2: XML — Estructura de Datos
**Integrantes:** Ignacio Jara y Danitza Ávila  

---
## Archivos

| Archivo | Descripción |
|---|---|
| `procesarlibros.py` | Procesa los 10.000 archivos XML y genera el archivo `books.json` |
| `json.hpp` | Librería externa nlohmann/json para leer el JSON en C++ |
| `tree.hpp` | Se declaran los structs y la clase del árbol |
| `tree.cpp` | Se implementa el árbol y sus funciones |
| `main.cpp` | Punto de entrada del programa |

---


## Instrucciones de uso

### Paso 1 — Generar el JSON desde los XML



```bash
python procesarlibros.py books_xml books.json
```

Esto genera el archivo `books.json` en la misma carpeta del proyecto.

### Paso 2 — Compilar el proyecto

```bash
g++ -std=c++17 -o programa main.cpp tree.cpp
```

### Paso 3 — Ejecutar el programa

```bash
./programa
```

## Dependencias externas

- **nlohmann/json** (`json.hpp`): librería de cabecera única para leer JSON en C++.  
  Repositorio oficial: https://github.com/nlohmann/json