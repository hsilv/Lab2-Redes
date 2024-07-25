# Instrucciones
Para este laboratorio se implementó como algoritmo de corrección de errores los Códigos de Hamming, que aceptan cadenas de cualquier longitud y combinación, y por el lado de detección de errores, se optó por CRC-32, el laboratorio completo se puede ejecutar con un solo comando, pero también está la opción de ejecutar cada escenario por separado:

Cabe destacar que para poder ejecutarlo solo hace falta tener el compilador `g++`, `python3` y una terminal `bash`.

### Laboratorio Completo
```
./main.sh
```

### Escenario 1
Este escenario corresponde a una codificación y decodificación de los mensajes limpiamente.
```
./scene1.sh
```

### Escenario 2
Este escenario corresponde a una codificación y decodificación de los mensajes con un bit erróneo (al azar).
```
./scene2.sh
```

### Escenario 3
Este escenario corresponde a una codificación y decodificación de los mensajes con dos o más bits erróneos (al azar).
```
./scene3.sh
```

### Escenario 4
Este escenario corresponde a una codificación y decodificación (con Hamming únicamente, porque es el algoritmo de corrección), de un mensaje cuyos errores son indetectables o imposibles de decodificar.
```
./scene4.sh
```