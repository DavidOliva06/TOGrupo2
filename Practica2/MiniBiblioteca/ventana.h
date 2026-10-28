#ifndef VENTANA_H
#define VENTANA_H

#include "biblioteca.h"
#include <QWidget>
#include <functional>

class QLineEdit;
class QTableWidget;
class QLabel;

class Ventana : public QWidget
{
public:
    explicit Ventana(QWidget *parent = nullptr);

private:
    
    void ejecutar(const std::function<void()> &accion, const QString &exito);

    void agregarLibro();
    void prestarLibro();
    void devolverLibro();
    void eliminarLibro();
    void cargarArchivo(const QString &ruta);
    void guardarArchivo(const QString &ruta);

    QString codigoSeleccionado() const;   
    void refrescarTabla();
    void limpiarFormulario();

    Biblioteca bib;
    QString rutaActual;

    QLineEdit *txtCodigo, *txtTitulo, *txtAutor, *txtAnio;
    QTableWidget *tabla;
    QLabel *lblEstado;
};

#endif 
