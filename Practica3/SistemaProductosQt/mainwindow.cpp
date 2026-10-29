#include "mainwindow.h"

#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QAbstractItemView>
#include <QWidget>
#include <QString>
#include <algorithm>
#include <iterator>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      nombreEdit(nullptr), precioSpin(nullptr), cantidadSpin(nullptr),
      descuentoSpin(nullptr), filtroSpin(nullptr), tabla(nullptr), estado(nullptr)
{
    setWindowTitle("Sistema de Productos - PRAC03");
    resize(950, 620);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    auto *principal = new QVBoxLayout(central);

    auto *titulo = new QLabel("Sistema de Productos");
    titulo->setStyleSheet("font-size: 22px; font-weight: bold; color: #183B56;");
    principal->addWidget(titulo);

    auto *grupo = new QGroupBox("Registrar producto");
    auto *form = new QFormLayout(grupo);
    nombreEdit = new QLineEdit;
    nombreEdit->setPlaceholderText("Ej. Laptop");
    precioSpin = new QDoubleSpinBox;
    precioSpin->setRange(0.01, 100000000.0);
    precioSpin->setDecimals(2);
    precioSpin->setPrefix("S/ ");
    cantidadSpin = new QSpinBox;
    cantidadSpin->setRange(1, 1000000);
    descuentoSpin = new QDoubleSpinBox;
    descuentoSpin->setRange(0.0, 100.0);
    descuentoSpin->setDecimals(2);
    descuentoSpin->setSuffix(" %");

    form->addRow("Nombre:", nombreEdit);
    form->addRow("Precio unitario:", precioSpin);
    form->addRow("Cantidad:", cantidadSpin);
    form->addRow("Descuento:", descuentoSpin);
    principal->addWidget(grupo);

    auto *agregarBtn = new QPushButton("Agregar producto");
    principal->addWidget(agregarBtn);

    tabla = new QTableWidget(0, 5);
    tabla->setHorizontalHeaderLabels(
        {"Producto", "Precio (S/)", "Cantidad", "Descuento", "Precio final (S/)"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    principal->addWidget(tabla);

    auto *filaFiltro = new QHBoxLayout;
    filtroSpin = new QDoubleSpinBox;
    filtroSpin->setRange(0.0, 100000000.0);
    filtroSpin->setDecimals(2);
    filtroSpin->setPrefix("S/ ");
    auto *filtrarBtn = new QPushButton("Filtrar precio mayor que");
    auto *todosBtn = new QPushButton("Mostrar todos");
    filaFiltro->addWidget(new QLabel("Precio mínimo:"));
    filaFiltro->addWidget(filtroSpin);
    filaFiltro->addWidget(filtrarBtn);
    filaFiltro->addWidget(todosBtn);
    principal->addLayout(filaFiltro);

    auto *filaOps = new QHBoxLayout;
    auto *compararBtn = new QPushButton("Comparar 2 seleccionados");
    auto *combinarBtn = new QPushButton("Combinar 2 seleccionados (+)");
    auto *detalleBtn = new QPushButton("Mostrar detalles (const / <<)");
    filaOps->addWidget(compararBtn);
    filaOps->addWidget(combinarBtn);
    filaOps->addWidget(detalleBtn);
    principal->addLayout(filaOps);

    estado = new QLabel("Registra productos para comenzar.");
    estado->setWordWrap(true);
    principal->addWidget(estado);

    connect(agregarBtn, &QPushButton::clicked, this, [this]() { agregarProducto(); });
    connect(filtrarBtn, &QPushButton::clicked, this, [this]() { filtrarProductos(); });
    connect(todosBtn, &QPushButton::clicked, this, [this]() {
        actualizarTabla();
        estado->setText("Se muestran todos los productos.");
    });
    connect(compararBtn, &QPushButton::clicked, this, [this]() { compararSeleccionados(); });
    connect(combinarBtn, &QPushButton::clicked, this, [this]() { combinarSeleccionados(); });
    connect(detalleBtn, &QPushButton::clicked, this, [this]() { mostrarDetalles(); });
}

void MainWindow::agregarProducto()
{
    const QString nombre = nombreEdit->text().trimmed();
    if (nombre.isEmpty()) {
        QMessageBox::warning(this, "Dato requerido", "Ingresa el nombre del producto.");
        nombreEdit->setFocus();
        return;
    }

    productos.emplace_back(nombre.toStdString(), precioSpin->value(),
                           cantidadSpin->value(), descuentoSpin->value());
    actualizarTabla();
    nombreEdit->clear();
    precioSpin->setValue(0.01);
    cantidadSpin->setValue(1);
    descuentoSpin->setValue(0.0);
    estado->setText("Producto agregado correctamente.");
}

void MainWindow::actualizarTabla()
{
    std::vector<int> indices;
    indices.reserve(productos.size());
    for (int i = 0; i < static_cast<int>(productos.size()); ++i)
        indices.push_back(i);
    mostrarIndices(indices);
}

void MainWindow::mostrarIndices(const std::vector<int>& indices)
{
    indicesVisibles = indices;
    tabla->setRowCount(0);

    for (int indice : indicesVisibles) {
        const Producto& p = productos.at(indice);
        const int fila = tabla->rowCount();
        tabla->insertRow(fila);
        tabla->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(p.getNombre())));
        tabla->setItem(fila, 1, new QTableWidgetItem(QString::number(p.getPrecio(), 'f', 2)));
        tabla->setItem(fila, 2, new QTableWidgetItem(QString::number(p.getCantidad())));
        tabla->setItem(fila, 3, new QTableWidgetItem(QString::number(p.getDescuento(), 'f', 2) + "%"));
        tabla->setItem(fila, 4, new QTableWidgetItem(QString::number(p.precioConDescuento(), 'f', 2)));
    }
}

std::vector<int> MainWindow::indicesSeleccionados() const
{
    std::vector<int> indices;
    const auto filas = tabla->selectionModel()->selectedRows();
    for (const auto& fila : filas) {
        const int filaVisible = fila.row();
        if (filaVisible >= 0 && filaVisible < static_cast<int>(indicesVisibles.size()))
            indices.push_back(indicesVisibles.at(filaVisible));
    }
    return indices;
}

void MainWindow::filtrarProductos()
{
    const double minimo = filtroSpin->value();
    std::vector<int> filtrados;

    // Lambda: conserva productos cuyo precio unitario sea mayor al umbral.
    for (int i = 0; i < static_cast<int>(productos.size()); ++i) {
        const Producto& p = productos.at(i);
        if ([minimo](const Producto& producto) {
                return producto.getPrecio() > minimo;
            }(p)) {
            filtrados.push_back(i);
        }
    }

    mostrarIndices(filtrados);
    estado->setText(QString("Filtro lambda: %1 producto(s) con precio mayor que S/ %2.")
                    .arg(filtrados.size()).arg(minimo, 0, 'f', 2));
}

void MainWindow::compararSeleccionados()
{
    const auto indices = indicesSeleccionados();
    if (indices.size() != 2) {
        QMessageBox::information(this, "Selecciona productos",
            "Selecciona exactamente dos filas. Mantén Ctrl para elegir la segunda.");
        return;
    }

    QMessageBox::information(this, "Comparación friend",
        QString::fromStdString(comparar(productos.at(indices[0]), productos.at(indices[1]))));
}

void MainWindow::combinarSeleccionados()
{
    const auto indices = indicesSeleccionados();
    if (indices.size() != 2) {
        QMessageBox::information(this, "Selecciona productos",
            "Selecciona exactamente dos filas. Mantén Ctrl para elegir la segunda.");
        return;
    }

    Producto combinado = productos.at(indices[0]) + productos.at(indices[1]);
    productos.push_back(combinado);
    actualizarTabla();
    estado->setText("Producto combinado con operator+. Se sumaron precios unitarios y cantidades; "
                    "el descuento del producto combinado es 0%.");
}

void MainWindow::mostrarDetalles()
{
    const auto indices = indicesSeleccionados();
    if (indices.size() != 1) {
        QMessageBox::information(this, "Selecciona un producto",
            "Selecciona una sola fila para ver sus detalles.");
        return;
    }

    const Producto& p = productos.at(indices[0]);
    std::ostringstream salida;
    salida << p; // operator<<, reto adicional.

    QMessageBox::information(this, "Datos del producto (const y <<)",
        QString::fromStdString(p.mostrarDatos()) +
        "\n\nSalida del operador <<:\n" + QString::fromStdString(salida.str()));
}
