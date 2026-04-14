# Batalla Naval (C Language)
Proyecto desarrollado como trabajo práctico grupal para la Tecnicatura Universitaria en Programación (UTN).

## Descripción
Este proyecto es una implementación del juego de mesa "Batalla Naval" que se ejecuta completamente en la terminal. El objetivo principal fue aplicar conceptos avanzados de programación estructurada en C para gestionar la lógica de un tablero bidimensional y el estado de una partida en tiempo real.

## Desafíos técnicos resueltos
Para este desarrollo, nos enfocamos en:
- Gestión de tableros: Uso de matrices para representar el campo de batalla, diferenciando entre posiciones vacías, barcos, impactos y fallos.
- Algoritmos de validación: Implementación de lógica para evitar la superposición de barcos y asegurar que los disparos se realicen dentro de los límites del mapa.
- Modularización: Estructuración del código en funciones independientes para facilitar el mantenimiento y la lectura (entrada de datos, procesamiento lógico y renderizado de consola).
- Sistema de turnos dinámico: Control del flujo del juego hasta detectar la condición de victoria (todos los barcos hundidos).

## Conceptos aplicados
- Estructuras de datos avanzadas: Uso de estructuras anidadas y listas enlazadas dinámicas para la gestión de usuarios y rankings.
- Memoria dinámica: Implementación de nodos y punteros con gestión de ciclo de vida de memoria (malloc/free).
- Persistencia en disco: Serialización y recuperación de datos mediante manejo de archivos (FILE I/O) con parsing de formatos delimitados.
- Programación modular: División de responsabilidades en funciones independientes para lógica de juego, interfaz de consola y gestión de datos.
- Algoritmos de validación: Lógica compleja para la colocación de barcos evitando colisiones y desbordamientos de matriz.

## Instalación y Ejecución
Clonar el repositorio: git clone https://github.com/JuanZunino/batalla-naval-c
Compilar: gcc main.c -o batalla_naval
Ejecutar: ./batalla_naval
