# Actividad — Plantillas (templates)

## ¿Qué hace el programa?

No tiene ventana.
Se abre en la consola, escribe sus resultados y termina.

Adentro hay cuatro ejercicios.
Los cuatro giran alrededor de una misma idea: las **plantillas**.

Una plantilla es como un molde.
Escribes algo una sola vez y sirve para muchos tipos de datos.
Para textos, para números enteros, para decimales, para letras.
No hace falta repetir el código para cada uno.

---

## ¿Cómo se usa?

1. Abres el proyecto en Qt Creator.
2. Presionas **Ejecutar**.
3. En la consola aparecen los cuatro ejercicios, uno debajo del otro.

No hay que escribir nada.
Todos los datos ya están puestos en el código.

### Lo que sale en pantalla

```
===== EJERCICIO 1: Clase generica Par<T1, T2> =====
Par<QString, int>     : (Ana, 20)
Par<int, double>      : (2024001, 17.5)
Par<double, QChar>    : (18.5, A)
Par<QString, QString> : (Arequipa, Peru)
Tras setSegundo(21)   : (Ana, 21)
Acceso individual     : Ana tiene 21 anios

===== EJERCICIO 2: Funcion generica minimo() =====
minimo(8, 3, 5)                 = 3
minimo(2.7, 9.1, 1.4)           = 1.4
minimo('z', 'm', 'q')           = m
minimo("pera","kiwi","uva")     = kiwi
constexpr minimo(10, 4, 7)      = 4

===== EJERCICIO 3: Potencia n^m con constexpr =====
potencia(2, 10)       = 1024
potencia(7, 3)        = 343
Potencia<3, 4>::valor = 81
Potencia<10,6>::valor = 1000000
int arreglo[potencia(2, 3)] -> tamanio = 8

===== EJERCICIO 4: Patron CRTP =====
Interfaz comun (sin virtual):
  Circulo(r=3) -> Area: 28.27 | Perimetro: 18.85
  Rectangulo(4x5) -> Area: 20.00 | Perimetro: 18.00
  Rectangulo(6x6) -> Area: 36.00 | Perimetro: 24.00
  Triangulo(3, 4, 5) -> Area: 6.00 | Perimetro: 12.00
Metodos personalizados de cada derivada:
  Circulo diametro       = 6
  Rectangulo 4x5 cuadrado? No
  Rectangulo 6x6 cuadrado? Si
  Triangulo equilatero?    No
Contador de instancias por clase (CRTP):
  Circulos   : 1
  Rectangulos: 2
  Triangulos : 1
```

---

## Las ideas de la práctica

| Idea | Dónde se usa | Qué significa, en simple |
|---|---|---|
| Plantilla de clase | El `Par` | Una cajita con dos espacios que acepta cualquier tipo de dato |
| Plantilla de función | `minimo` | Una sola función que sirve para números, letras o palabras |
| `constexpr` | `potencia` | La cuenta se hace antes de que el programa arranque |
| Plantilla que se llama a sí misma | `Potencia<N, M>` | Otra forma de hacer la misma cuenta, también por adelantado |
| CRTP | Las figuras | Un "padre" que ya sabe quién es su hijo y lo usa directo |

---

## El código, por partes

Todo está en un solo archivo: [`main.cpp`](EjerciciosTemplates/main.cpp).

### 0. Escribir en la consola

```cpp
static QTextStream& out()
{
    static QTextStream ts(stdout);
    return ts;
}
```

Es un atajo.
Cada vez que el programa quiere escribir algo en pantalla, usa `out()`.
Así no hay que preparar la salida una y otra vez.

---

### Ejercicio 1: El par

```cpp
template <typename T1, typename T2>
class Par
{
public:
    Par(const T1& primero, const T2& segundo)
        : m_primero(primero), m_segundo(segundo) {}

    const T1& primero() const { return m_primero; }
    const T2& segundo() const { return m_segundo; }

    void setPrimero(const T1& valor) { m_primero = valor; }
    void setSegundo(const T2& valor) { m_segundo = valor; }

    void mostrar() const { ... }

private:
    T1 m_primero{};
    T2 m_segundo{};
};
```

Un `Par` guarda dos cosas juntas.
Nada más.

Lo especial es que no dice **qué** cosas.
`T1` y `T2` son espacios en blanco.
Se llenan recién cuando creas el par.

```cpp
Par<QString, int> estudiante("Ana", 20);
Par<int, double> codigoNota(2024001, 17.5);
Par<double, QChar> notaLetra(18.5, QChar('A'));
Par<QString, QString> ciudadPais("Arequipa", "Peru");
```

Con el mismo molde salen cuatro pares distintos:

- Un nombre y una edad.
- Un código y una nota.
- Una nota y una letra.
- Una ciudad y un país.

El par también trae lo básico:

- `primero()` y `segundo()` para consultar cada dato.
- `setPrimero()` y `setSegundo()` para cambiarlos.
- `mostrar()` para escribirlos entre paréntesis.

En el ejemplo, Ana cumple años.
Se cambia su edad a 21 y se vuelve a mostrar.

---

### Ejercicio 2: El menor de tres

```cpp
template <typename T>
constexpr const T& minimo(const T& a, const T& b, const T& c)
{
    const T& menorAB = (b < a) ? b : a;
    return (c < menorAB) ? c : menorAB;
}
```

Recibe tres cosas y devuelve la más chica.

Lo hace en dos pasos:

1. Compara las dos primeras y se queda con la menor.
2. Compara esa con la tercera y se queda con la menor.

Funciona con cualquier cosa que se pueda ordenar:

| Lo que recibe | Lo que devuelve | Por qué |
|---|---|---|
| `8, 3, 5` | `3` | Es el número más chico |
| `2.7, 9.1, 1.4` | `1.4` | También con decimales |
| `'z', 'm', 'q'` | `m` | Es la que va primero en el abecedario |
| `"pera", "kiwi", "uva"` | `kiwi` | Es la que va primero en el diccionario |

La última línea del ejercicio usa `constexpr`.
Eso quiere decir que el `4` de `minimo(10, 4, 7)` se calcula antes de que el programa arranque.
Cuando el programa corre, la respuesta ya está lista.

---

### Ejercicio 3: La potencia

Aquí se calcula `n` elevado a `m`.
Por ejemplo, `2` elevado a `10` es `1024`.

Se hace de dos formas.
Las dos tienen algo en común: la cuenta se hace **antes** de ejecutar el programa.

#### Forma 1: con una función

```cpp
constexpr long long potencia(long long n, unsigned int m)
{
    return (m == 0) ? 1 : n * potencia(n, m - 1);
}
```

La idea es simple.
Elevar a la `m` es multiplicar por `n` y luego elevar a la `m - 1`.
Se repite hasta llegar a cero.
Y cualquier número elevado a cero es `1`.

Por ejemplo, `7` a la `3`:

```
7 × 7 × 7 × 1 = 343
```

#### Forma 2: con plantillas

```cpp
template <long long N, unsigned int M>
struct Potencia
{
    static constexpr long long valor = N * Potencia<N, M - 1>::valor;
};

template <long long N>
struct Potencia<N, 0>
{
    static constexpr long long valor = 1;
};
```

Es la misma idea, pero escrita con moldes.
El primer molde dice: *"multiplica por N y pregúntale al molde de abajo"*.
El segundo molde es el final del camino: cuando `M` llega a cero, la respuesta es `1`.

`Potencia<3, 4>` va bajando así: `3 × 3 × 3 × 3 × 1 = 81`.

#### Las pruebas

```cpp
static_assert(potencia(2, 10) == 1024, "Error en potencia constexpr");
static_assert(Potencia<3, 4>::valor == 81, "Error en Potencia<>");
static_assert(potencia(5, 0) == 1, "Error en caso base");
```

Son revisiones automáticas.
Si alguna cuenta saliera mal, el programa ni siquiera se podría armar.
Saldría el error antes de llegar a ejecutarlo.

#### El arreglo

```cpp
int arreglo[potencia(2, 3)];
```

Esta línea demuestra que la cuenta de verdad se hizo por adelantado.
El tamaño de un arreglo se tiene que conocer desde el inicio.
Y aquí se usa `potencia(2, 3)`, que da `8`.
Si la cuenta no estuviera lista a tiempo, esta línea no funcionaría.

---

### Ejercicio 4: Las figuras (CRTP)

Hay tres figuras: círculo, rectángulo y triángulo.
Todas saben decir su nombre, su área y su perímetro.

Lo normal sería tener una clase "padre" con funciones que los hijos rellenan.
Aquí se hace distinto.
El padre recibe al hijo como dato.
Así ya sabe desde el principio con quién está trabajando.

#### El padre: Figura

```cpp
template <typename Derivada>
class Figura
{
public:
    double area() const      { return derivada().areaImpl(); }
    double perimetro() const { return derivada().perimetroImpl(); }
    QString nombre() const   { return derivada().nombreImpl(); }

    void describir() const { ... }

    static int instancias() { return s_contador; }

protected:
    Figura()  { ++s_contador; }
    ~Figura() { --s_contador; }
    ...
private:
    static inline int s_contador = 0;
};
```

El padre no sabe calcular nada por su cuenta.
Cuando le piden el área, se la pide al hijo.
Lo mismo con el perímetro y el nombre.

`describir()` junta todo en una línea con dos decimales.
Esa línea es igual para todas las figuras.

Además, el padre lleva la cuenta de cuántas figuras hay.
Cada vez que se crea una, suma uno.
Cuando una desaparece, resta uno.

Lo curioso es que cada tipo de figura tiene su propio contador.
Los círculos cuentan aparte de los rectángulos.
Por eso al final sale `Rectangulos: 2` y `Circulos: 1`.

#### Los hijos

```cpp
class Circulo : public Figura<Circulo>
{
public:
    explicit Circulo(double radio) : m_radio(radio) {}

    double areaImpl() const      { return PI * m_radio * m_radio; }
    double perimetroImpl() const { return 2 * PI * m_radio; }
    QString nombreImpl() const   { return QString("Circulo(r=%1)").arg(m_radio); }

    double diametro() const { return 2 * m_radio; }
    ...
};
```

Fíjate en la primera línea: `Circulo : public Figura<Circulo>`.
El círculo se pone a sí mismo dentro del molde del padre.
Es como decir: *"soy una figura, y la figura soy yo"*.
De ahí viene el nombre raro del patrón.

Cada hijo pone sus propias fórmulas:

| Figura | Área | Perímetro | Algo propio |
|---|---|---|---|
| Círculo | π × radio × radio | 2 × π × radio | `diametro()`: el doble del radio |
| Rectángulo | base × altura | 2 × (base + altura) | `esCuadrado()`: si los dos lados miden igual |
| Triángulo | Fórmula de Herón | Suma de los tres lados | `esEquilatero()`: si los tres lados miden igual |

La fórmula de Herón sirve para sacar el área de un triángulo solo con sus lados.
Primero se saca la mitad del perímetro.
Luego se hace una cuenta con eso y cada lado.
Para el triángulo `3, 4, 5`, da `6`.

#### Una función para todas

```cpp
template <typename Derivada>
void imprimirFigura(const Figura<Derivada>& figura)
{
    out() << "  ";
    figura.describir();
}
```

Esta función recibe cualquier figura.
No le importa si es círculo, rectángulo o triángulo.
Solo le pide que se describa.

---

### El main: todo junto

```cpp
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Ejercicio 1
    Par<QString, int> estudiante("Ana", 20);
    ...

    // Ejercicio 4
    Circulo c(3.0);
    Rectangulo r1(4.0, 5.0);
    Rectangulo r2(6.0, 6.0);
    Triangulo t(3.0, 4.0, 5.0);
    ...
    return 0;
}
```

Es la parte que arranca el programa.
Va ejercicio por ejercicio, en orden.

En cada uno:

1. Escribe un título.
2. Crea los datos de ejemplo.
3. Muestra los resultados.

Al terminar el cuarto ejercicio, el programa se cierra solo.
