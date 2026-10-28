#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <QString>
#include <QVector>

struct Libro
{
    QString codigo;
    QString titulo;
    QString autor;
    int     anio = 0;
    bool    disponible = true;
};


class Biblioteca
{
public:

    static Libro crearLibro(const QString &codigo, const QString &titulo,
                            const QString &autor, const QString &anioTexto);

    void agregar(const Libro &libro);           
    void eliminar(const QString &codigo);       
    void prestar(const QString &codigo);        
    void devolver(const QString &codigo);       

    void cargar(const QString &ruta);           
    void guardar(const QString &ruta) const;    

    const QVector<Libro> &libros() const { return libros_; }

private:
    int indiceDe(const QString &codigo) const; 
    Libro &buscar(const QString &codigo);      

    QVector<Libro> libros_;
};

#endif // BIBLIOTECA_H
