# Sistema de Productos — C++ y Qt

## Descripción
Aplicación de escritorio desarrollada en C++ y Qt Widgets para registrar, filtrar, comparar y combinar productos mediante una interfaz gráfica diseñada con Qt Designer.

## Lógica de las funciones

### Clase `Producto`
- **`precioConDescuento()`**: calcula el precio final aplicando el descuento (`inline`).
- **`comparar()`**: compara los precios de dos productos mediante acceso `friend`.
- **`mostrarDatos()`**: devuelve los datos del producto sin modificarlos (`const`).
- **`operator+()`**: combina dos productos en un nuevo objeto.
- **`operator<<()`**: permite mostrar los datos del producto en un flujo de salida.

### Clase `MainWindow`
- **`agregarProducto()`**: valida los datos y registra un producto.
- **`actualizarTabla()`**: muestra todos los productos registrados.
- **`mostrarIndices()`**: actualiza la tabla con los productos indicados.
- **`indicesSeleccionados()`**: obtiene los productos seleccionados en la tabla.
- **`filtrarProductos()`**: filtra productos por precio utilizando una expresión lambda.
- **`compararProductos()`**: compara dos productos seleccionados.
- **`combinarProductos()`**: crea un nuevo producto a partir de dos seleccionados.
- **`mostrarDetalles()`**: presenta los datos de un producto seleccionado.

## Tecnologías
- C++17
- Qt Widgets y Qt Designer
- CMake

## Objetivo
Aplicar conceptos de programación orientada a objetos y características de C++, como `inline`, `friend`, `const`, expresiones lambda y sobrecarga de operadores.
