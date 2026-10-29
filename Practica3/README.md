# Practica 3 — Función amiga: comparar cajas

## ¿Qué hace el programa?

Es una ventana con tres casillas.
En cada una escribes el peso de una caja.
El programa te dice cuál de las tres pesa más.
Si las tres pesan igual, también te lo dice.

Detrás de algo tan simple hay una idea importante: la **función amiga**.
Más abajo se explica con calma.

---

## ¿Cómo se usa?

1. Abres el programa.
2. Escribes el peso de la caja 1, la caja 2 y la caja 3.
3. Presionas **Comparar**.
4. Abajo aparece cuál es la más pesada.

Si quieres probar con otros pesos, presionas **Limpiar**.
Se borran las tres casillas y el cursor vuelve a la primera.

### Algunos ejemplos

| Caja 1 | Caja 2 | Caja 3 | Lo que dice |
|:--:|:--:|:--:|---|
| `10` | `4` | `7` | La caja 1 tiene el mayor peso. |
| `2.5` | `8` | `3` | La caja 2 tiene el mayor peso. |
| `1` | `1` | `9` | La caja 3 tiene el mayor peso. |
| `5` | `5` | `5` | Las tres cajas pesan lo mismo. |
| `5` | `5` | `3` | La caja 1 tiene el mayor peso. |
| `-3` | `4` | `2` | Aviso: *"Ingrese pesos validos y positivos."* |
| `hola` | `4` | `2` | El mismo aviso |
| *(vacío)* | `4` | `2` | El mismo aviso |

Un par de detalles:

- Los decimales se escriben con punto: `2.5`. Con coma (`2,5`) sale el aviso.
- El peso `0` sí se acepta. Solo se rechazan los negativos.
- Si dos cajas empatan como las más pesadas, gana la que está más arriba. En el ejemplo `5, 5, 3`, dice "caja 1", aunque la caja 2 pesa lo mismo.

---

## La idea de la práctica: la función amiga

Imagina que cada caja tiene su peso escrito **por dentro**, bajo llave.
Nadie de afuera puede abrirla y mirar.

Eso está bien, porque así nadie cambia el peso por error.
Pero hay un problema: ¿cómo comparamos tres cajas si no podemos ver cuánto pesan?

La solución es darle permiso especial a alguien de confianza.
La caja dice: *"Esta función es mi amiga. Ella sí puede mirar adentro"*.

Esa amiga es la función `comparar`.
No es parte de la caja, está afuera.
Pero tiene la llave.

---

## El código, por partes

El programa está repartido en pocos archivos:

| Archivo | Para qué sirve |
|---|---|
| [`caja.h`](FuncionFriend/caja.h) | La caja y su función amiga |
| [`MainWindow.ui`](FuncionFriend/MainWindow.ui) | El dibujo de la ventana |
| [`MainWindow.h`](FuncionFriend/MainWindow.h) / [`MainWindow.cpp`](FuncionFriend/MainWindow.cpp) | Lo que hacen los botones |
| [`main.cpp`](FuncionFriend/main.cpp) | El arranque |

### 1. La caja

```cpp
class Caja {
private:
    double peso;

public:
    Caja(double p) {
        peso = p;
    }

    friend int comparar(Caja c1, Caja c2, Caja c3);
};
```

Cada caja guarda una sola cosa: su peso.

La palabra `private` es la llave.
Significa que el peso solo se puede ver desde dentro de la caja.

Cuando se crea una caja, se le dice cuánto pesa.
Después ya no se puede cambiar ni leer desde afuera.

La línea con `friend` es el permiso especial.
Ahí la caja nombra a su amiga: la función `comparar`.

### 2. La función amiga

```cpp
inline int comparar(Caja c1, Caja c2, Caja c3) {
    if (c1.peso == c2.peso &&
        c2.peso == c3.peso) {
        return 0;
    }

    if (c1.peso >= c2.peso &&
        c1.peso >= c3.peso) {
        return 1;
    }

    if (c2.peso >= c1.peso &&
        c2.peso >= c3.peso) {
        return 2;
    }

    return 3;
}
```

Recibe las tres cajas y mira dentro de cada una.
Puede hacerlo porque es amiga.

Responde con un número:

- `0` si las tres pesan igual.
- `1` si la más pesada es la caja 1.
- `2` si es la caja 2.
- `3` si es la caja 3.

Va en orden.
Primero pregunta si son iguales.
Luego pregunta si la caja 1 es la mayor.
Luego la caja 2.
Si ninguna de esas se cumplió, tiene que ser la caja 3.

Por eso, en un empate, gana la que se revisa primero.

### 3. La ventana

La ventana no está escrita a mano en el código.
Se dibujó con el editor visual de Qt, como quien arma algo arrastrando piezas.
El resultado se guarda en [`MainWindow.ui`](FuncionFriend/MainWindow.ui).

Tiene:

- Tres textos: *Peso de la caja 1*, *2* y *3*.
- Al lado, tres casillas para escribir.
- Dos botones: **Comparar** y **Limpiar**.
- Abajo, una línea para el resultado.

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}
```

Esta parte toma ese dibujo y lo pone en pantalla.
Es todo lo que hace falta.

### 4. El botón Comparar

```cpp
void MainWindow::on_btnComparar_clicked() {
    bool ok1, ok2, ok3;

    double p1 = ui->txtPeso1->text().toDouble(&ok1);
    double p2 = ui->txtPeso2->text().toDouble(&ok2);
    double p3 = ui->txtPeso3->text().toDouble(&ok3);

    if (!ok1 || !ok2 || !ok3 ||
        p1 < 0 || p2 < 0 || p3 < 0) {
        QMessageBox::warning(this, "Error", "Ingrese pesos validos y positivos.");
        return;
    }

    Caja c1(p1);
    Caja c2(p2);
    Caja c3(p3);

    int resultado = comparar(c1, c2, c3);
    ...
}
```

Esto pasa al presionar **Comparar**:

1. Lee lo que escribiste en las tres casillas y trata de convertirlo en números.
2. Revisa que los tres sean números de verdad y que ninguno sea negativo.
3. Si algo falla, saca un aviso y se detiene ahí.
4. Si todo está bien, crea las tres cajas con esos pesos.
5. Le pide a la función amiga que las compare.
6. Según el número que le devuelve, escribe la frase que corresponde.

El nombre del botón importa.
Como se llama `on_btnComparar_clicked`, Qt sabe solo que tiene que conectarlo con el botón `btnComparar`.
No hace falta unirlos a mano.

### 5. El botón Limpiar

```cpp
void MainWindow::on_btnLimpiar_clicked() {
    ui->txtPeso1->clear();
    ui->txtPeso2->clear();
    ui->txtPeso3->clear();

    ui->lblResultado->setText("Resultado:");

    ui->txtPeso1->setFocus();
}
```

Borra las tres casillas.
Deja el resultado en blanco.
Y pone el cursor en la primera casilla, listo para escribir.

---

# Practica 3 — Sistema de Productos

## ¿Qué hace el programa?

Es una ventana para llevar una lista de productos, como en una tienda.
De cada producto anotas el nombre, el precio, cuántos hay y si tiene descuento.
El programa calcula solo cuánto cuesta con el descuento.

Además puedes:

- Ver solo los productos que pasan de cierto precio.
- Comparar dos productos para saber cuál es más caro.
- Juntar dos productos en uno nuevo.
- Ver todos los datos de un producto en una ventanita.

---

## ¿Cómo se ve?

De arriba hacia abajo:

- El título: **Sistema de Productos**.
- Un formulario para registrar un producto y el botón **Agregar producto**.
- La tabla con todos los productos.
- Una fila para filtrar por precio.
- Tres botones: **Comparar**, **Combinar** y **Mostrar detalles**.
- Una línea al final que te cuenta qué acaba de pasar.

---

## ¿Cómo se usa?

### Registrar un producto

1. Escribes el nombre.
2. Eliges el precio, la cantidad y el descuento con las flechitas (o escribiéndolos).
3. Presionas **Agregar producto**.

El producto aparece en la tabla y el formulario vuelve a como estaba al inicio.

Las casillas de números no te dejan poner cualquier cosa:

| Casilla | Qué acepta |
|---|---|
| Nombre | Cualquier texto, pero no vacío |
| Precio unitario | Desde S/ 0.01 |
| Cantidad | Desde 1 |
| Descuento | De 0 % a 100 % |

Si dejas el nombre vacío, sale un aviso y el cursor vuelve a esa casilla.

### Filtrar por precio

1. Pones un precio en **Precio mínimo**.
2. Presionas **Filtrar precio mayor que**.

La tabla muestra solo los productos que cuestan **más** que ese precio.
Si un producto cuesta justo lo mismo, no aparece.

Para ver todo otra vez, presionas **Mostrar todos**.

### Comparar, combinar y ver detalles

Primero eliges productos en la tabla haciendo clic.
Para elegir más de uno, mantienes presionada la tecla **Ctrl**.

- **Comparar 2 seleccionados:** eliges dos. Te dice cuál es más caro.
- **Combinar 2 seleccionados (+):** eliges dos. Crea un producto nuevo que junta a los dos.
- **Mostrar detalles:** eliges uno solo. Te muestra todos sus datos.

Si eliges una cantidad distinta de la que pide el botón, te avisa.

### Un ejemplo completo

Registras estos dos productos:

| Nombre | Precio | Cantidad | Descuento | Precio final |
|---|---|---|---|---|
| Laptop | S/ 2500.00 | 2 | 10 % | S/ 2250.00 |
| Mouse | S/ 50.00 | 5 | 0 % | S/ 50.00 |

- Si **filtras** con `100`, solo queda la Laptop.
- Si los **comparas**, dice: *"El producto más caro es: Laptop (S/ 2500.00)"*.
- Si los **combinas**, aparece un tercero:

| Nombre | Precio | Cantidad | Descuento |
|---|---|---|---|
| Laptop + Mouse | S/ 2550.00 | 7 | 0 % |

Los dos originales no se borran. El combinado se suma a la lista.

---

## Las ideas de la práctica

Este programa junta varias ideas de una sola vez.
Aquí van explicadas sin complicarse.

| Idea | Dónde se usa | Qué significa, en simple |
|---|---|---|
| `inline` | El cálculo del precio con descuento | Una cuenta tan corta que conviene escribirla ahí mismo |
| `const` | Las partes que solo leen datos | Una promesa: "solo voy a mirar, no voy a tocar nada" |
| `friend` | Comparar dos productos | Una función de afuera con permiso para ver lo que está guardado |
| `operator+` | Combinar dos productos | Hacer que el signo **+** sirva para juntar productos |
| `operator<<` | Mostrar los detalles | Enseñarle al producto a escribirse a sí mismo en una línea |
| Lambda | El filtro por precio | Una mini regla escrita en el momento, justo donde se necesita |

---

## El código, por partes

| Archivo | Para qué sirve |
|---|---|
| [`producto.h`](SistemaProductosQt/producto.h) / [`producto.cpp`](SistemaProductosQt/producto.cpp) | Qué es un producto y qué se puede hacer con él |
| [`mainwindow.h`](SistemaProductosQt/mainwindow.h) / [`mainwindow.cpp`](SistemaProductosQt/mainwindow.cpp) | La ventana y lo que hace cada botón |
| [`main.cpp`](SistemaProductosQt/main.cpp) | El arranque |

Igual que en la MiniBiblioteca, se separa el producto de la ventana.
El producto no sabe nada de botones.
La ventana solo lo usa.

### 1. El producto

```cpp
class Producto
{
private:
    std::string nombre;
    double precio;
    int cantidad;
    double descuento;
    ...
};
```

Cada producto guarda cuatro datos: nombre, precio, cantidad y descuento.
Están guardados bajo llave (`private`), como el peso de las cajas.
Desde afuera no se pueden cambiar.

### 2. El precio con descuento

```cpp
inline double precioConDescuento() const
{
    return precio * (1.0 - descuento / 100.0);
}
```

Es una cuenta de toda la vida.
Si algo cuesta 100 y tiene 20 % de descuento, pagas 80.

Tiene dos palabras especiales:

- `inline`: la cuenta es tan cortita que se escribe aquí mismo, sin vueltas.
- `const`: promete que solo mira el precio, no lo cambia.

### 3. Consultar los datos

```cpp
const std::string& getNombre() const;
double getPrecio() const;
int getCantidad() const;
double getDescuento() const;
```

Como los datos están bajo llave, hacen falta "ventanillas" para consultarlos.
Cada una devuelve un dato.
Todas llevan `const`, porque solo muestran, no modifican.

La tabla de la ventana usa estas ventanillas para llenar cada fila.

### 4. Comparar: la función amiga

```cpp
std::string comparar(const Producto& a, const Producto& b)
{
    ...
    if (a.precio > b.precio)
        salida << "El producto más caro es: " << a.nombre ...
    else if (b.precio > a.precio)
        salida << "El producto más caro es: " << b.nombre ...
    else
        salida << "Ambos productos tienen el mismo precio.";
    ...
}
```

Es la misma idea de las cajas.
`comparar` no es parte del producto, pero el producto la nombró su amiga.
Por eso puede mirar directo el precio y el nombre, sin pasar por las ventanillas.

Mira los dos precios y arma la frase con el más caro.
Si cuestan lo mismo, lo dice.

Un detalle: compara el precio **sin** descuento.
Así que un producto puede salir como "más caro" aunque al final, con su descuento, se pague menos.

### 5. Combinar con el signo +

```cpp
Producto Producto::operator+(const Producto& otro) const
{
    return Producto(nombre + " + " + otro.nombre,
                    precio + otro.precio,
                    cantidad + otro.cantidad,
                    0.0);
}
```

Normalmente el **+** solo sirve para sumar números.
Aquí se le enseña a sumar productos.

Cuando haces `laptop + mouse`, sale un producto nuevo:

- El nombre junta los dos: `Laptop + Mouse`.
- El precio es la suma de los dos precios.
- La cantidad es la suma de las dos cantidades.
- El descuento queda en cero.

Los productos originales no cambian. Se crea uno nuevo.

### 6. Mostrar los datos

```cpp
std::string Producto::mostrarDatos() const
{
    ...
    salida << "Producto: " << nombre
           << "\nPrecio unitario: S/ " << precio
           << "\nCantidad: " << cantidad
           << "\nDescuento: " << descuento << "%"
           << "\nPrecio con descuento: S/ " << precioConDescuento();
    ...
}
```

Arma un texto con todos los datos, uno debajo del otro.
Los precios siempre salen con dos decimales.

```cpp
std::ostream& operator<<(std::ostream& salida, const Producto& p)
{
    salida << p.nombre
           << " | Precio: S/ " << p.precio
           << " | Cantidad: " << p.cantidad
           ...
}
```

Esta otra hace lo mismo, pero en una sola línea separada por rayitas.
Es la forma de "enseñarle" al producto a escribirse solo.

Al presionar **Mostrar detalles**, la ventanita muestra las dos versiones, una arriba y otra abajo.

### 7. La ventana

```cpp
setWindowTitle("Sistema de Productos - PRAC03");
resize(950, 620);
...
precioSpin->setRange(0.01, 100000000.0);
precioSpin->setPrefix("S/ ");
cantidadSpin->setRange(1, 1000000);
descuentoSpin->setRange(0.0, 100.0);
descuentoSpin->setSuffix(" %");
...
```

Aquí se arma todo lo que ves.
A diferencia de las cajas, esta ventana sí está escrita a mano en el código.

Lo interesante son las casillas de números.
Ya traen sus límites puestos: el precio no baja de 0.01, la cantidad no baja de 1 y el descuento no pasa de 100.
También muestran `S/` y `%` para que se entienda qué es cada cosa.
Así casi no hace falta revisar errores: la casilla misma no te deja equivocarte.

La tabla es solo para mirar.
Al hacer clic marca toda la fila, y con **Ctrl** puedes marcar varias.

### 8. Agregar un producto

```cpp
void MainWindow::agregarProducto()
{
    const QString nombre = nombreEdit->text().trimmed();
    if (nombre.isEmpty()) {
        QMessageBox::warning(this, "Dato requerido", "Ingresa el nombre del producto.");
        nombreEdit->setFocus();
        return;
    }

    productos.emplace_back(nombre.toStdString(), precioSpin->value(),
                           cantidadSpin->value(), descuentoSpin->value());
    actualizarTabla();
    ...
}
```

Lo único que revisa es el nombre, porque es la única casilla libre.
Si está vacío, avisa.
Si no, crea el producto, lo agrega a la lista y actualiza la tabla.
Luego deja el formulario limpio.

### 9. Saber qué fila es qué producto

```cpp
// Relaciona cada fila visible con el índice real de productos.
std::vector<int> indicesVisibles;
```

Esta parte es pequeña pero importante.

Cuando filtras, la tabla muestra menos productos.
Entonces la fila 1 de la tabla ya no es necesariamente el producto 1 de la lista.

Para no confundirse, el programa lleva una "chuleta".
Anota qué producto real está en cada fila que ves.
Así, cuando eliges una fila y presionas un botón, siempre usa el producto correcto.

### 10. El filtro

```cpp
for (int i = 0; i < static_cast<int>(productos.size()); ++i) {
    const Producto& p = productos.at(i);
    if ([minimo](const Producto& producto) {
            return producto.getPrecio() > minimo;
        }(p)) {
        filtrados.push_back(i);
    }
}
```

Recorre todos los productos, uno por uno.
A cada uno le pregunta: *"¿cuestas más que el mínimo?"*.
Si la respuesta es sí, lo anota.

Esa pregunta está escrita como una **lambda**.
Es una regla chiquita, sin nombre, escrita justo en el lugar donde se usa.
Se lleva consigo el precio mínimo que pusiste, para poder comparar.

Al final, la tabla muestra solo los anotados.
Y abajo dice cuántos productos pasaron el filtro.

Como en el comparar, el filtro mira el precio **sin** descuento.

### 11. Comparar, combinar y detalles

```cpp
void MainWindow::combinarSeleccionados()
{
    const auto indices = indicesSeleccionados();
    if (indices.size() != 2) {
        QMessageBox::information(this, "Selecciona productos",
            "Selecciona exactamente dos filas. Mantén Ctrl para elegir la segunda.");
        return;
    }

    Producto combinado = productos.at(indices[0]) + productos.at(indices[1]);
    productos.push_back(combinado);
    actualizarTabla();
    ...
}
```

Los tres botones empiezan igual.
Miran cuántas filas elegiste.
Si no es la cantidad correcta, te lo dicen y se detienen.

Después, cada uno hace lo suyo:

- **Comparar** llama a la función amiga y muestra la respuesta en una ventanita.
- **Combinar** usa el **+**, agrega el nuevo producto a la lista y vuelve a mostrar todo.
- **Mostrar detalles** usa las dos formas de mostrar datos y las enseña juntas.

Fíjate en la línea del combinar: `productos.at(indices[0]) + productos.at(indices[1])`.
Se lee casi como una suma normal. Esa es la gracia del **+**.
