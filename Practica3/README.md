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
