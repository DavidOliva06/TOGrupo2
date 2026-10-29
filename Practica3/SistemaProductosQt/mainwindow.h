#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "producto.h"

class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;
class QTableWidget;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QLineEdit *nombreEdit;
    QDoubleSpinBox *precioSpin;
    QSpinBox *cantidadSpin;
    QDoubleSpinBox *descuentoSpin;
    QDoubleSpinBox *filtroSpin;
    QTableWidget *tabla;
    QLabel *estado;

    std::vector<Producto> productos;
    // Relaciona cada fila visible con el índice real de productos.
    std::vector<int> indicesVisibles;

    void agregarProducto();
    void actualizarTabla();
    void mostrarIndices(const std::vector<int>& indices);
    std::vector<int> indicesSeleccionados() const;
    void filtrarProductos();
    void compararSeleccionados();
    void combinarSeleccionados();
    void mostrarDetalles();
};

#endif // MAINWINDOW_H
