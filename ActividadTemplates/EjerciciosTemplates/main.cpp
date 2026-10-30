#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QChar>
#include <cmath>

constexpr double PI = 3.14159265358979323846;
static QTextStream& out()
{
    static QTextStream ts(stdout);
    return ts;
}

// EJERCICIO 1: Clase genérica Par<T1, T2>
template <typename T1, typename T2>
class Par
{
public:
    Par() = default;
    Par(const T1& primero, const T2& segundo)
        : m_primero(primero), m_segundo(segundo) {}

    // Getters
    const T1& primero() const { return m_primero; }
    const T2& segundo() const { return m_segundo; }

    // Setters
    void setPrimero(const T1& valor) { m_primero = valor; }
    void setSegundo(const T2& valor) { m_segundo = valor; }

    void mostrar() const
    {
        out() << "(" << m_primero << ", " << m_segundo << ")" << Qt::endl;
    }

private:
    T1 m_primero{};
    T2 m_segundo{};
};


template <typename T>
constexpr const T& minimo(const T& a, const T& b, const T& c)
{
    const T& menorAB = (b < a) ? b : a;
    return (c < menorAB) ? c : menorAB;
}

// EJERCICIO 3: Meta-programación - potencia en tiempo de compilación

// (a) Con función constexpr (recursiva)
constexpr long long potencia(long long n, unsigned int m)
{
    return (m == 0) ? 1 : n * potencia(n, m - 1);
}

// (b) Con plantillas (template metaprogramming clásico)
template <long long N, unsigned int M>
struct Potencia
{
    static constexpr long long valor = N * Potencia<N, M - 1>::valor;
};

// Especialización: caso base n^0 = 1
template <long long N>
struct Potencia<N, 0>
{
    static constexpr long long valor = 1;
};

static_assert(potencia(2, 10) == 1024, "Error en potencia constexpr");
static_assert(Potencia<3, 4>::valor == 81, "Error en Potencia<>");
static_assert(potencia(5, 0) == 1, "Error en caso base");

// EJERCICIO 4: Patrón CRTP (Curiously Recurring Template Pattern)
template <typename Derivada>
class Figura
{
public:
    // Interfaz común: llama a la implementación de la clase derivada
    // sin funciones virtuales (polimorfismo estático)
    double area() const      { return derivada().areaImpl(); }
    double perimetro() const { return derivada().perimetroImpl(); }
    QString nombre() const   { return derivada().nombreImpl(); }

    void describir() const
    {
        out() << nombre()
              << " -> Area: "      << QString::number(area(), 'f', 2)
              << " | Perimetro: "  << QString::number(perimetro(), 'f', 2)
              << Qt::endl;
    }

    static int instancias() { return s_contador; }

protected:
    Figura()  { ++s_contador; }
    ~Figura() { --s_contador; }
    Figura(const Figura&) { ++s_contador; }

private:
    const Derivada& derivada() const
    {
        return static_cast<const Derivada&>(*this);
    }

    static inline int s_contador = 0;  // contador independiente por cada derivada
};

class Circulo : public Figura<Circulo>
{
public:
    explicit Circulo(double radio) : m_radio(radio) {}

    double areaImpl() const      { return PI * m_radio * m_radio; }
    double perimetroImpl() const { return 2 * PI * m_radio; }
    QString nombreImpl() const   { return QString("Circulo(r=%1)").arg(m_radio); }

    // Método personalizado propio de Circulo
    double diametro() const { return 2 * m_radio; }

private:
    double m_radio;
};

class Rectangulo : public Figura<Rectangulo>
{
public:
    Rectangulo(double base, double altura) : m_base(base), m_altura(altura) {}

    double areaImpl() const      { return m_base * m_altura; }
    double perimetroImpl() const { return 2 * (m_base + m_altura); }
    QString nombreImpl() const   { return QString("Rectangulo(%1x%2)").arg(m_base).arg(m_altura); }

    // Método personalizado propio de Rectangulo
    bool esCuadrado() const { return qFuzzyCompare(m_base, m_altura); }

private:
    double m_base, m_altura;
};

class Triangulo : public Figura<Triangulo>
{
public:
    Triangulo(double a, double b, double c) : m_a(a), m_b(b), m_c(c) {}

    double areaImpl() const
    {
        const double s = perimetroImpl() / 2;  // Fórmula de Herón
        return std::sqrt(s * (s - m_a) * (s - m_b) * (s - m_c));
    }
    double perimetroImpl() const { return m_a + m_b + m_c; }
    QString nombreImpl() const
    {
        return QString("Triangulo(%1, %2, %3)").arg(m_a).arg(m_b).arg(m_c);
    }

    // Método personalizado propio de Triangulo
    bool esEquilatero() const
    {
        return qFuzzyCompare(m_a, m_b) && qFuzzyCompare(m_b, m_c);
    }

private:
    double m_a, m_b, m_c;
};

// Función genérica que acepta cualquier Figura<Derivada>
template <typename Derivada>
void imprimirFigura(const Figura<Derivada>& figura)
{
    out() << "  ";
    figura.describir();
}

// MAIN
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    out() << "===== EJERCICIO 1: Clase generica Par<T1, T2> =====" << Qt::endl;

    Par<QString, int> estudiante("Ana", 20);
    Par<int, double> codigoNota(2024001, 17.5);
    Par<double, QChar> notaLetra(18.5, QChar('A'));
    Par<QString, QString> ciudadPais("Arequipa", "Peru");

    out() << "Par<QString, int>     : "; estudiante.mostrar();
    out() << "Par<int, double>      : "; codigoNota.mostrar();
    out() << "Par<double, QChar>    : "; notaLetra.mostrar();
    out() << "Par<QString, QString> : "; ciudadPais.mostrar();

    estudiante.setSegundo(21);
    out() << "Tras setSegundo(21)   : "; estudiante.mostrar();
    out() << "Acceso individual     : " << estudiante.primero()
          << " tiene " << estudiante.segundo() << " anios" << Qt::endl;

    out() << Qt::endl << "===== EJERCICIO 2: Funcion generica minimo() =====" << Qt::endl;

    out() << "minimo(8, 3, 5)                 = " << minimo(8, 3, 5) << Qt::endl;
    out() << "minimo(2.7, 9.1, 1.4)           = " << minimo(2.7, 9.1, 1.4) << Qt::endl;
    out() << "minimo('z', 'm', 'q')           = " << minimo('z', 'm', 'q') << Qt::endl;
    out() << "minimo(\"pera\",\"kiwi\",\"uva\")     = "
          << minimo(QString("pera"), QString("kiwi"), QString("uva")) << Qt::endl;

    constexpr int menorCompilacion = minimo(10, 4, 7);  // también en compilación
    out() << "constexpr minimo(10, 4, 7)      = " << menorCompilacion << Qt::endl;

    out() << Qt::endl << "===== EJERCICIO 3: Potencia n^m con constexpr =====" << Qt::endl;

    constexpr long long p1 = potencia(2, 10);       // calculado al compilar
    constexpr long long p2 = potencia(7, 3);
    constexpr long long p3 = Potencia<3, 4>::valor; // metaprogramación con templates
    constexpr long long p4 = Potencia<10, 6>::valor;

    out() << "potencia(2, 10)       = " << p1 << Qt::endl;
    out() << "potencia(7, 3)        = " << p2 << Qt::endl;
    out() << "Potencia<3, 4>::valor = " << p3 << Qt::endl;
    out() << "Potencia<10,6>::valor = " << p4 << Qt::endl;
    int arreglo[potencia(2, 3)];
    out() << "int arreglo[potencia(2, 3)] -> tamanio = "
          << static_cast<int>(sizeof(arreglo) / sizeof(arreglo[0])) << Qt::endl;

    out() << Qt::endl << "===== EJERCICIO 4: Patron CRTP =====" << Qt::endl;

    Circulo c(3.0);
    Rectangulo r1(4.0, 5.0);
    Rectangulo r2(6.0, 6.0);
    Triangulo t(3.0, 4.0, 5.0);

    out() << "Interfaz comun (sin virtual):" << Qt::endl;
    imprimirFigura(c);
    imprimirFigura(r1);
    imprimirFigura(r2);
    imprimirFigura(t);

    out() << "Metodos personalizados de cada derivada:" << Qt::endl;
    out() << "  Circulo diametro       = " << c.diametro() << Qt::endl;
    out() << "  Rectangulo 4x5 cuadrado? " << (r1.esCuadrado() ? "Si" : "No") << Qt::endl;
    out() << "  Rectangulo 6x6 cuadrado? " << (r2.esCuadrado() ? "Si" : "No") << Qt::endl;
    out() << "  Triangulo equilatero?    " << (t.esEquilatero() ? "Si" : "No") << Qt::endl;

    out() << "Contador de instancias por clase (CRTP):" << Qt::endl;
    out() << "  Circulos   : " << Figura<Circulo>::instancias() << Qt::endl;
    out() << "  Rectangulos: " << Figura<Rectangulo>::instancias() << Qt::endl;
    out() << "  Triangulos : " << Figura<Triangulo>::instancias() << Qt::endl;

    return 0;
}
