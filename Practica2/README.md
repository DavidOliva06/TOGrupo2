# Práctica 2 — Conversor de texto a número

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

### Y para arrancar

```cpp
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Ventana v;
    v.show();
    return app.exec();
}
```

Esto es lo primero que corre.
Crea la ventana, la muestra y se queda esperando a que hagas algo.
El programa sigue abierto hasta que cierras la ventana.

---

## ¿Cómo abrirlo?

1. Abre **Qt Creator**.
2. Abre el archivo [`ConversorNumero.pro`](conversornumero/ConversorNumero.pro) (o el `CMakeLists.txt`).
3. Presiona el botón verde de **Ejecutar**.

Listo, la ventana debería aparecer.
