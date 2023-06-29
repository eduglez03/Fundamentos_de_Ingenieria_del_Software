# Compilación y uso
### make clean --> Para **borrar** el ejecutable anterior
### make --> Generar el **ejecutable** de nuevo
### ./smart_lock.out --> **Programa** principal

## Aplicación de gestión de cerraduras inteligentes
Esta aplicación se ha desarrollado completamente en C++, simulando una arquitectura enfocada en los microservicios, es decir cada servicio está diseñado para cumplir con una única tarea. Para ello se han simulado 3 servicios:

1. Interfaz Gráfica de Usuario (./src/models/Interfaz.h): todas las interacciones que harán los usuarios del sistema (cliente, administrador, empleado, ...) se llevarán acabo en la interfaz

2. Middleware (./src/middleware/middleware.h): será el servicio encargado de actúar de intermediario entre la base de datos y la interfaz gráfica para ello se definirán un seria de reglas que permitirán la comunicación entre los dos servicios

3. Base de Datos (BaseDatos.DB): almacenado en este directorio estarán todos los ficheros de texto con los datos principales de los usuarios y las cerraduras

## Imagen que muestra la arquitectura de una aplicación web que simularemos en c++

![Imagen que muestra la arquitectura](https://www.astera.com/wp-content/uploads/2020/01/rest.png)
