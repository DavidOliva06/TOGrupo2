#include "biblioteca.h"
#include "excepciones.h"

#include <QDate>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

static const QString CABECERA = "#MINIBIBLIOTECA v1";
static const QChar   SEP      = ';';



static QString validarTexto(const QString &valor, const std::string &campo, int maxLen)
{
    QString t = valor.trimmed();
    if (t.isEmpty())
        throw EntradaInvalidaException(campo, "no puede estar vacío.");
    if (t.length() > maxLen)
        throw EntradaInvalidaException(campo, "máximo " + std::to_string(maxLen) + " caracteres.");
    if (t.contains(SEP))
        throw EntradaInvalidaException(campo, "no puede contener el carácter ';'.");
    return t;
}

Libro Biblioteca::crearLibro(const QString &codigo, const QString &titulo,
                             const QString &autor, const QString &anioTexto)
{
    Libro l;

    l.codigo = codigo.trimmed().toUpper();
    static const QRegularExpression patron("^[A-Z]{1,3}\\d{1,5}$");
    if (l.codigo.isEmpty())
        throw EntradaInvalidaException("Código", "no puede estar vacío.");
    if (!patron.match(l.codigo).hasMatch())
        throw EntradaInvalidaException("Código", "use 1-3 letras seguidas de 1-5 dígitos (ej. L001).");

    l.titulo = validarTexto(titulo, "Título", 100);
    l.autor  = validarTexto(autor, "Autor", 60);

    bool ok = false;
    l.anio = anioTexto.trimmed().toInt(&ok);
    if (!ok)
        throw EntradaInvalidaException("Año", "debe ingresar un número entero válido.");
    const int actual = QDate::currentDate().year();
    if (l.anio < 1450 || l.anio > actual)
        throw EntradaInvalidaException("Año", "debe estar entre 1450 y " + std::to_string(actual) + ".");

    l.disponible = true;
    return l;
}

// ------------------------------ Operaciones ------------------------------

int Biblioteca::indiceDe(const QString &codigo) const
{
    const QString c = codigo.trimmed().toUpper();
    for (int i = 0; i < libros_.size(); ++i)
        if (libros_[i].codigo == c)
            return i;
    return -1;
}

Libro &Biblioteca::buscar(const QString &codigo)
{
    int i = indiceDe(codigo);
    if (i < 0)
        throw LibroNoEncontradoException(codigo.trimmed().toUpper().toStdString());
    return libros_[i];
}

void Biblioteca::agregar(const Libro &libro)
{
    if (indiceDe(libro.codigo) >= 0)
        throw LibroDuplicadoException(libro.codigo.toStdString());
    libros_.append(libro);
}

void Biblioteca::eliminar(const QString &codigo)
{
    Libro &l = buscar(codigo);
    if (!l.disponible)
        throw LibroNoDisponibleException(l.codigo.toStdString(),
                                         "está prestado; debe devolverse antes de eliminarlo.");
    libros_.remove(indiceDe(codigo));
}

void Biblioteca::prestar(const QString &codigo)
{
    Libro &l = buscar(codigo);
    if (!l.disponible)
        throw LibroNoDisponibleException(l.codigo.toStdString(), "ya se encuentra prestado.");
    l.disponible = false;
}

void Biblioteca::devolver(const QString &codigo)
{
    Libro &l = buscar(codigo);
    if (l.disponible)
        throw LibroNoDisponibleException(l.codigo.toStdString(), "no estaba prestado.");
    l.disponible = true;
}

// -------------------------------- Archivos --------------------------------
// Formato (UTF-8):
//   #MINIBIBLIOTECA v1
//   codigo;titulo;autor;anio;disponible(1/0)

void Biblioteca::cargar(const QString &ruta)
{
    const std::string r = ruta.toStdString();

    QFile f(ruta);
    if (!f.exists() || !f.open(QIODevice::ReadOnly | QIODevice::Text))
        throw ArchivoNoEncontradoException(r);

    QTextStream in(&f);
    QVector<Libro> leidos;      // se carga en una lista temporal: si el archivo
    int nLinea = 0;             // está corrupto, los datos actuales no se pierden

    if (in.atEnd())
        throw ArchivoCorruptoException(r, 1, "el archivo está vacío.");
    QString cab = in.readLine().trimmed();
    ++nLinea;
    if (cab.startsWith(QChar(0xFEFF))) cab.remove(0, 1);   // BOM
    if (cab != CABECERA)
        throw ArchivoCorruptoException(r, nLinea, "cabecera inválida, se esperaba \""
                                                      + CABECERA.toStdString() + "\".");

    while (!in.atEnd()) {
        const QString linea = in.readLine();
        ++nLinea;
        if (linea.trimmed().isEmpty())
            continue;

        const QStringList campos = linea.split(SEP);
        if (campos.size() != 5)
            throw ArchivoCorruptoException(r, nLinea, "se esperaban 5 campos y hay "
                                                          + std::to_string(campos.size()) + ".");

        Libro l;
        try {
            l = crearLibro(campos[0], campos[1], campos[2], campos[3]);
        } catch (const EntradaInvalidaException &e) {
            // Un dato inválido dentro del archivo se reporta como corrupción
            throw ArchivoCorruptoException(r, nLinea, e.what());
        }

        const QString disp = campos[4].trimmed();
        if (disp != "1" && disp != "0")
            throw ArchivoCorruptoException(r, nLinea, "el campo disponible debe ser 1 o 0.");
        l.disponible = (disp == "1");

        for (const Libro &x : leidos)
            if (x.codigo == l.codigo)
                throw ArchivoCorruptoException(r, nLinea, "código repetido " + l.codigo.toStdString() + ".");

        leidos.append(l);
    }

    libros_ = leidos;
}

void Biblioteca::guardar(const QString &ruta) const
{
    QFile f(ruta);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        throw ArchivoNoEncontradoException(ruta.toStdString());

    QTextStream out(&f);
    out << CABECERA << '\n';
    for (const Libro &l : libros_)
        out << l.codigo << SEP << l.titulo << SEP << l.autor << SEP
            << l.anio << SEP << (l.disponible ? '1' : '0') << '\n';
}
