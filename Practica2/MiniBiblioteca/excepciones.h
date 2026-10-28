#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <stdexcept>
#include <string>


class BibliotecaException : public std::runtime_error
{
public:
    explicit BibliotecaException(const std::string &msg) : std::runtime_error(msg) {}
    virtual std::string titulo() const { return "Error en la biblioteca"; }
};

class EntradaInvalidaException : public BibliotecaException
{
public:
    EntradaInvalidaException(const std::string &campo, const std::string &detalle)
        : BibliotecaException("Campo \"" + campo + "\": " + detalle), campo_(campo) {}
    std::string titulo() const override { return "Entrada inválida"; }
    const std::string &campo() const { return campo_; }
private:
    std::string campo_;
};

class ArchivoException : public BibliotecaException
{
public:
    ArchivoException(const std::string &ruta, const std::string &msg)
        : BibliotecaException(msg), ruta_(ruta) {}
    const std::string &ruta() const { return ruta_; }
private:
    std::string ruta_;
};

class ArchivoNoEncontradoException : public ArchivoException
{
public:
    explicit ArchivoNoEncontradoException(const std::string &ruta)
        : ArchivoException(ruta, "No se encontró o no se pudo abrir el archivo:\n" + ruta) {}
    std::string titulo() const override { return "Archivo no encontrado"; }
};

class ArchivoCorruptoException : public ArchivoException
{
public:
    ArchivoCorruptoException(const std::string &ruta, int linea, const std::string &detalle)
        : ArchivoException(ruta, "El archivo está corrupto (línea " + std::to_string(linea)
                                     + "): " + detalle),
          linea_(linea) {}
    std::string titulo() const override { return "Archivo corrupto"; }
    int linea() const { return linea_; }
private:
    int linea_;
};

class LibroNoEncontradoException : public BibliotecaException
{
public:
    explicit LibroNoEncontradoException(const std::string &codigo)
        : BibliotecaException("No existe un libro con código " + codigo) {}
    std::string titulo() const override { return "Libro no encontrado"; }
};

class LibroDuplicadoException : public BibliotecaException
{
public:
    explicit LibroDuplicadoException(const std::string &codigo)
        : BibliotecaException("Ya existe un libro con código " + codigo) {}
    std::string titulo() const override { return "Libro duplicado"; }
};

class LibroNoDisponibleException : public BibliotecaException
{
public:
    LibroNoDisponibleException(const std::string &codigo, const std::string &motivo)
        : BibliotecaException("El libro " + codigo + " " + motivo) {}
    std::string titulo() const override { return "Operación no permitida"; }
};

#endif // EXCEPCIONES_H
