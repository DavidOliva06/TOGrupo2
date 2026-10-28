# Practica 2 — Conversor de texto a número

## ¿Qué hace el programa?

Es una ventana pequeña.
Tu escribes un número y el programa lo revisa.
Si esta bien escrito, te lo muestra y te dice cuanto es el doble.
Si no, te avisa que algo salio mal.


---

## ¿Cómo se usa?

1. Abres el programa.
2. Escribes un numero en la caja de texto. Por ejemplo, `42`.
3. Presionas **Convertir** o la tecla **Enter**.
4. Abajo aparece el resultado.

Si quieres empezar de nuevo, presionas **Limpiar**.

### Algunos ejemplos

| Lo que escribes | Lo que pasa |
|---|---|
| `42` | Muestra `Resultado: 42 (el doble es 84)` en verde |
| `-7` | Muestra `Resultado: -7 (el doble es -14)` en verde |
| `  15  ` | Ignora los espacios y muestra `15` |
| `12abc` | Sale un aviso de error y el texto se pone en rojo |
| `hola` | Sale un aviso de error |
| *(nada)* | También sale el aviso de error |

---

## El código, por partes

Todo el programa está en un solo archivo: [`main.cpp`](conversornumero/main.cpp).
Lo podemos dividir en cuatro partes.

### 1. La parte que revisa el número

```cpp
int convertirAEntero(const QString &texto)
{
    std::string s = texto.trimmed().toStdString();
    if (s.empty())
        throw std::invalid_argument("vacio");

    std::size_t pos = 0;
    int valor = std::stoi(s, &pos);

    if (pos != s.size())
        throw std::invalid_argument("caracteres extra");

    return valor;
}
```

Esta es la parte más importante.
Recibe lo que escribiste y hace tres cosas:

- Primero quita los espacios del inicio y del final.
- Si no queda nada, dice "esto está vacío" y se detiene.
- Luego intenta leer el número.

También revisa que no haya nada sobrando después del número.
Por eso `12abc` no pasa: lee el `12`, pero ve que quedan letras.

Si todo está bien, devuelve el numero.

### 2. La ventana

```cpp
setWindowTitle("Conversor de texto a número");
resize(380, 160);

auto *lblInstruccion = new QLabel("Ingrese un número entero:");
entrada = new QLineEdit;
entrada->setPlaceholderText("Ej. 42");

auto *btnConvertir = new QPushButton("Convertir");
auto *btnLimpiar   = new QPushButton("Limpiar");

lblResultado = new QLabel("Resultado: —");
```

Aquí se arma lo que ves en pantalla.
Se le pone un título a la ventana y un tamaño.

Después se crean las piezas, una por una:

- Un texto que dice qué hacer.
- La caja donde escribes. Trae un ejemplo en gris: "Ej. 42".
- Dos botones: **Convertir** y **Limpiar**.
- Una línea abajo donde sale el resultado.

Luego se ordenan.
Los dos botones van uno al lado del otro.
Todo lo demás va de arriba hacia abajo.

### 3. Qué pasa al presionar cada cosa

```cpp
connect(btnConvertir, &QPushButton::clicked, this, [this] { convertir(); });
connect(entrada, &QLineEdit::returnPressed, this, [this] { convertir(); });
connect(btnLimpiar, &QPushButton::clicked, this, [this] {
    entrada->clear();
    lblResultado->setText("Resultado: —");
    ...
});
```

Aquí se le dice al programa como reaccionar.

- Si presionas **Convertir**, revisa el número.
- Si presionas **Enter**, hace lo mismo. Es un atajo.
- Si presionas **Limpiar**, borra lo que escribiste y deja el resultado como al inicio. Además deja el cursor listo para escribir otra vez.

### 4. Mostrar el resultado o el error

```cpp
void convertir()
{
    try {
        int numero = convertirAEntero(entrada->text());
        lblResultado->setText(QString("Resultado: %1 (el doble es %2)")
                                  .arg(numero)
                                  .arg(static_cast<long long>(numero) * 2));
        lblResultado->setStyleSheet("font-weight: bold; color: green;");
    }
    catch (const std::exception &) {
        lblResultado->setText("Resultado: error");
        lblResultado->setStyleSheet("font-weight: bold; color: red;");
        QMessageBox::warning(this, "Error", "Debe ingresar un número válido");
        entrada->selectAll();
        entrada->setFocus();
    }
}
```

Esta parte junta todo.
Toma lo que escribiste y se lo pasa a la parte que revisa (la número 1).

**Si salió bien:**
muestra el número y su doble, en letras verdes.

**Si salió mal:**
pone "error" en letras rojas.
Saca una ventanita que dice *"Debe ingresar un número válido"*.
Y deja tu texto marcado, para que lo corrijas sin tener que borrarlo a mano.

Un detalle: el doble se calcula con un número "más grande" por dentro.
Así, si escribes un número muy alto, el doble no sale mal.

---

# Practica 2 - Minisistema de Biblioteca

## ¿Qué hace el programa?

Es una ventana para llevar la lista de libros de una biblioteca.
Puedes registrar libros nuevos.
Puedes prestarlos, recibirlos de vuelta y quitarlos de la lista.
Y todo se puede guardar en un archivo, para no perderlo al cerrar.

Lo más importante: el programa no se cae cuando algo sale mal.
Si escribes algo raro o el archivo está dañado, te explica qué pasó.

---

## ¿Cómo se ve?

La ventana tiene tres zonas:

- **Arriba:** los botones para abrir y guardar el archivo.
- **A la izquierda:** un formulario para registrar un libro.
- **A la derecha:** la tabla con todos los libros y los botones para prestar, devolver o eliminar.

Abajo del todo hay una línea que te dice qué acaba de pasar.
Se pone verde si todo salió bien, naranja si escribiste algo mal y roja si hubo un problema.

---

## ¿Cómo se usa?

### Registrar un libro

1. Llenas el código, el título, el autor y el año.
2. Presionas **Agregar libro** (o **Enter** estando en el año).
3. El libro aparece en la tabla como *Disponible*.

El formulario se limpia solo, listo para el siguiente.

### Prestar, devolver o eliminar

1. Haces clic en un libro de la tabla.
2. Presionas el botón que necesites.

Antes de eliminar, el programa te pregunta si estás seguro.

### Guardar y abrir

- **Guardar** escribe la lista en el mismo archivo de antes.
- **Guardar como…** te deja elegir otro nombre o lugar.
- **Abrir archivo…** carga una lista guardada.

Si es la primera vez y nunca guardaste, **Guardar** se comporta como **Guardar como…**.

---

## Las reglas del formulario

El programa no acepta cualquier cosa. Revisa cada casilla:

| Casilla | Qué pide | Ejemplo válido |
|---|---|---|
| Código | De 1 a 3 letras y luego de 1 a 5 números | `L001`, `AB12` |
| Título | No vacío, máximo 100 letras | `Los ríos profundos` |
| Autor | No vacío, máximo 60 letras | `José María Arguedas` |
| Año | Un número entre 1450 y el año actual | `1958` |

Un par de detalles:

- Si escribes el código en minúsculas, lo pasa a mayúsculas. `l001` queda como `L001`.
- Ninguna casilla puede llevar punto y coma (`;`). Más abajo se explica por qué.
- El año empieza en 1450 porque más o menos ahí empezaron los libros impresos.

---

## ¿Cuándo te avisa?

| Lo que pasa | Lo que te dice |
|---|---|
| Dejas una casilla vacía | Que esa casilla no puede estar vacía |
| Escribes `hola` en el año | Que debe ser un número |
| Pones un código que ya existe | Que ese libro ya está registrado |
| Presionas Prestar sin elegir un libro | Que primero elijas uno en la tabla |
| Prestas un libro que ya está prestado | Que ya se encuentra prestado |
| Devuelves un libro que no estaba prestado | Que no estaba prestado |
| Quieres eliminar un libro prestado | Que primero lo devuelvan |
| Abres un archivo que no existe | Que no se encontró |
| Abres un archivo dañado | En qué línea está el daño y por qué |

Cuando el error es de una casilla, el programa te lleva a esa casilla.
Así lo corriges al toque.

---

## El archivo de la biblioteca

Los libros se guardan en un archivo de texto normal.
Se puede abrir con el Bloc de notas.
Se ve así:

```
#MINIBIBLIOTECA v1
L001;Cien años de soledad;Gabriel García Márquez;1967;1
L002;La ciudad y los perros;Mario Vargas Llosa;1963;0
```

- La primera línea es como una firma. Sirve para saber que el archivo es de este programa.
- Cada línea después es un libro.
- Los datos van separados por punto y coma. Por eso no se permite `;` en las casillas: se confundiría con el separador.
- El último número dice si el libro está en la biblioteca (`1`) o prestado (`0`).

En la carpeta hay dos archivos para probar:

- [`biblioteca.txt`](MiniBiblioteca/biblioteca.txt): un catálogo bueno, con cuatro libros.
- [`biblioteca_corrupta.txt`](MiniBiblioteca/biblioteca_corrupta.txt): uno dañado a propósito. En la línea 3 el año está escrito con letras y a la línea 4 le faltan datos.

Si abres el dañado, el programa avisa que la línea 3 tiene un problema.
Y no borra lo que ya tenías en pantalla.

---

## El código, por partes

Esta vez el programa está repartido en varios archivos.
Cada uno tiene su tarea:

| Archivo | Para qué sirve |
|---|---|
| [`excepciones.h`](MiniBiblioteca/excepciones.h) | Los tipos de problemas que pueden pasar |
| [`biblioteca.h`](MiniBiblioteca/biblioteca.h) / [`biblioteca.cpp`](MiniBiblioteca/biblioteca.cpp) | Las reglas: cómo se agrega, presta, guarda, etc. |
| [`ventana.h`](MiniBiblioteca/ventana.h) / [`ventana.cpp`](MiniBiblioteca/ventana.cpp) | Lo que ves en pantalla y los botones |
| [`main.cpp`](MiniBiblioteca/main.cpp) | El arranque |

La idea es separar las cosas.
Las reglas de la biblioteca no saben nada de botones.
Y la ventana no decide las reglas, solo las usa.

### 1. La ficha de cada libro

```cpp
struct Libro
{
    QString codigo;
    QString titulo;
    QString autor;
    int     anio = 0;
    bool    disponible = true;
};
```

Es como una ficha de cartulina.
Cada libro tiene su código, título, autor, año y si está disponible o no.
Un libro nuevo siempre empieza como disponible.

### 2. Los tipos de problemas

```cpp
class BibliotecaException : public std::runtime_error { ... };
class EntradaInvalidaException : public BibliotecaException { ... };
class ArchivoException : public BibliotecaException { ... };
class ArchivoNoEncontradoException : public ArchivoException { ... };
class ArchivoCorruptoException : public ArchivoException { ... };
class LibroNoEncontradoException : public BibliotecaException { ... };
class LibroDuplicadoException : public BibliotecaException { ... };
class LibroNoDisponibleException : public BibliotecaException { ... };
```

Aquí se le pone nombre a cada cosa que puede salir mal.
Están ordenados como un árbol familiar:

```
Problema en la biblioteca
├── Dato mal escrito
├── Problema con el archivo
│   ├── No se encontró
│   └── Está dañado
├── El libro no existe
├── El libro ya existe
└── No se puede hacer eso con ese libro
```

¿Para qué tanto orden?
Porque así el programa puede reaccionar distinto según el problema.
Por ejemplo, un dato mal escrito te lleva a la casilla.
Un archivo dañado te dice la línea exacta.

Cada tipo de problema trae su propio título para la ventanita de aviso.
Por eso los avisos dicen cosas como *"Libro duplicado"* o *"Archivo corrupto"*.

### 3. Revisar lo que escribiste

```cpp
Libro Biblioteca::crearLibro(const QString &codigo, const QString &titulo,
                             const QString &autor, const QString &anioTexto)
{
    Libro l;

    l.codigo = codigo.trimmed().toUpper();
    static const QRegularExpression patron("^[A-Z]{1,3}\\d{1,5}$");
    ...
    l.titulo = validarTexto(titulo, "Título", 100);
    l.autor  = validarTexto(autor, "Autor", 60);
    ...
    if (l.anio < 1450 || l.anio > actual)
        throw EntradaInvalidaException("Año", ...);
    ...
}
```

Esta parte toma lo que escribiste en el formulario y lo revisa casilla por casilla.

- Al código le quita espacios y lo pone en mayúsculas. Luego mira que tenga la forma correcta: letras y después números.
- Al título y al autor les revisa que no estén vacíos, que no sean muy largos y que no tengan `;`.
- Al año le revisa que sea un número y que esté en un rango que tenga sentido.

Si algo falla, se detiene y dice qué casilla está mal.
Si todo está bien, arma la ficha del libro.

### 4. Prestar, devolver y eliminar

```cpp
void Biblioteca::prestar(const QString &codigo)
{
    Libro &l = buscar(codigo);
    if (!l.disponible)
        throw LibroNoDisponibleException(l.codigo.toStdString(), "ya se encuentra prestado.");
    l.disponible = false;
}
```

Las tres funcionan parecido.
Primero buscan el libro por su código. Si no está, avisan.
Luego revisan si tiene sentido lo que pides:

- **Prestar:** el libro tiene que estar en la biblioteca.
- **Devolver:** el libro tiene que estar prestado.
- **Eliminar:** el libro no puede estar prestado. Primero hay que recuperarlo.

Si todo cuadra, cambian el estado del libro.

Agregar es aún más simple: solo revisa que el código no esté repetido.

### 5. Abrir un archivo

```cpp
void Biblioteca::cargar(const QString &ruta)
{
    ...
    QVector<Libro> leidos;      // se carga en una lista temporal: si el archivo
    ...                         // está corrupto, los datos actuales no se pierden
    if (cab != CABECERA)
        throw ArchivoCorruptoException(r, nLinea, "cabecera inválida...");

    while (!in.atEnd()) {
        ...
        if (campos.size() != 5)
            throw ArchivoCorruptoException(r, nLinea, "se esperaban 5 campos...");
        ...
        leidos.append(l);
    }

    libros_ = leidos;
}
```

Esta es la parte más cuidadosa.
Lee el archivo línea por línea, como quien revisa una lista a mano.

1. Si el archivo no existe, avisa.
2. Si está vacío, avisa.
3. Revisa que la primera línea sea la firma `#MINIBIBLIOTECA v1`.
4. Luego lee cada libro y revisa que tenga sus cinco datos.
5. A cada libro le pasa las mismas reglas del formulario. Así un año mal escrito en el archivo también se detecta.
6. Revisa que no haya dos libros con el mismo código.

Las líneas en blanco las salta sin problema.

Un truco importante: los libros se leen primero en una lista aparte.
Solo cuando todo el archivo está bien, esa lista reemplaza a la que ves.
Si a la mitad aparece un error, no se pierde nada de lo que ya tenías.

### 6. Guardar

```cpp
void Biblioteca::guardar(const QString &ruta) const
{
    ...
    out << CABECERA << '\n';
    for (const Libro &l : libros_)
        out << l.codigo << SEP << l.titulo << SEP << l.autor << SEP
            << l.anio << SEP << (l.disponible ? '1' : '0') << '\n';
}
```

Es lo contrario de abrir.
Escribe la firma y luego una línea por libro, con los datos separados por `;`.
Si no puede escribir en ese lugar, avisa.

### 7. La ventana

```cpp
setWindowTitle("Mini Sistema de Biblioteca");
resize(820, 520);

txtCodigo = new QLineEdit;  txtCodigo->setPlaceholderText("Ej. L001");
...
tabla = new QTableWidget(0, 5);
tabla->setHorizontalHeaderLabels({"Código", "Título", "Autor", "Año", "Estado"});
...
```

Aquí se arma todo lo que ves.
Las cuatro casillas del formulario, cada una con un ejemplo en gris.
La tabla con sus cinco columnas.
Y todos los botones.

La tabla está hecha para mirar, no para escribir.
Solo deja elegir un libro a la vez, y al hacer clic marca toda la fila.

Al abrir el programa, busca un archivo llamado `biblioteca.txt`.
Si lo encuentra, lo carga solo. Si no, empieza con la lista vacía y te lo dice abajo.

### 8. Un solo lugar para atender los problemas

```cpp
void Ventana::ejecutar(const std::function<void()> &accion, const QString &exito)
{
    try {
        accion();
        refrescarTabla();
        lblEstado->setText(exito);
    }
    catch (const EntradaInvalidaException &e) { ... }   // datos del usuario
    catch (const ArchivoException &e) { ... }           // archivo inexistente o corrupto
    catch (const BibliotecaException &e) { ... }        // reglas de negocio
    catch (const std::exception &e) { ... }             // cualquier otro error
}
```

Esta es la idea central de la práctica.
En vez de revisar errores en cada botón, todos los botones pasan por aquí.

Funciona así:
intenta hacer lo que pediste.
Si sale bien, actualiza la tabla y muestra un mensaje verde.
Si sale mal, mira qué tipo de problema fue y responde según eso:

- **Dato mal escrito:** mensaje naranja, aviso, y te lleva a la casilla con el error.
- **Problema con el archivo:** mensaje rojo y un aviso más serio. Te recuerda que tus datos siguen ahí.
- **Algo no permitido con un libro:** mensaje rojo y aviso con la explicación.
- **Cualquier otra cosa:** por si acaso, un aviso de *"Error inesperado"*. Así el programa nunca se cierra de golpe.

El orden importa.
Primero revisa los problemas más específicos y al final los más generales.

### 9. Los botones

```cpp
void Ventana::prestarLibro()
{
    ejecutar([this] { bib.prestar(codigoSeleccionado()); }, "Libro prestado.");
}
```

Gracias a la parte anterior, cada botón queda cortito.
Solo dice qué hacer y qué mensaje mostrar si todo sale bien.

Antes de prestar, devolver o eliminar, el programa mira qué libro elegiste en la tabla.
Si no elegiste ninguno, te pide que lo hagas.

### 10. Actualizar la tabla

```cpp
void Ventana::refrescarTabla()
{
    ...
    auto *estado = new QTableWidgetItem(l.disponible ? "Disponible" : "Prestado");
    estado->setForeground(l.disponible ? Qt::darkGreen : Qt::red);
    ...
}
```

Cada vez que algo cambia, la tabla se vuelve a llenar con la lista actual.
La columna de estado dice *Disponible* en verde o *Prestado* en rojo.
Así se nota de un vistazo qué libros están afuera.
