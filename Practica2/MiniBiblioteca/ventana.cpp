#include "ventana.h"
#include "excepciones.h"

#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

Ventana::Ventana(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Mini Sistema de Biblioteca");
    resize(820, 520);

    // ---- Formulario ----
    txtCodigo = new QLineEdit;  txtCodigo->setPlaceholderText("Ej. L001");
    txtTitulo = new QLineEdit;  txtTitulo->setPlaceholderText("Título del libro");
    txtAutor  = new QLineEdit;  txtAutor->setPlaceholderText("Autor");
    txtAnio   = new QLineEdit;  txtAnio->setPlaceholderText("Ej. 1967");

    auto *form = new QFormLayout;
    form->addRow("Código:", txtCodigo);
    form->addRow("Título:", txtTitulo);
    form->addRow("Autor:",  txtAutor);
    form->addRow("Año:",    txtAnio);

    auto *btnAgregar = new QPushButton("Agregar libro");
    form->addRow(btnAgregar);

    auto *grpForm = new QGroupBox("Registrar libro");
    grpForm->setLayout(form);

    // ---- Tabla ----
    tabla = new QTableWidget(0, 5);
    tabla->setHorizontalHeaderLabels({"Código", "Título", "Autor", "Año", "Estado"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto *btnPrestar  = new QPushButton("Prestar");
    auto *btnDevolver = new QPushButton("Devolver");
    auto *btnEliminar = new QPushButton("Eliminar");
    auto *filaBotones = new QHBoxLayout;
    filaBotones->addWidget(btnPrestar);
    filaBotones->addWidget(btnDevolver);
    filaBotones->addWidget(btnEliminar);

    auto *colTabla = new QVBoxLayout;
    colTabla->addWidget(tabla);
    colTabla->addLayout(filaBotones);
    auto *grpTabla = new QGroupBox("Catálogo");
    grpTabla->setLayout(colTabla);

    // ---- Archivo ----
    auto *btnAbrir     = new QPushButton("Abrir archivo…");
    auto *btnGuardar   = new QPushButton("Guardar");
    auto *btnGuardarC  = new QPushButton("Guardar como…");
    auto *filaArchivo  = new QHBoxLayout;
    filaArchivo->addWidget(btnAbrir);
    filaArchivo->addWidget(btnGuardar);
    filaArchivo->addWidget(btnGuardarC);
    filaArchivo->addStretch();

    lblEstado = new QLabel("Listo.");

    // ---- Layout principal ----
    auto *centro = new QHBoxLayout;
    centro->addWidget(grpForm, 1);
    centro->addWidget(grpTabla, 2);

    auto *principal = new QVBoxLayout(this);
    principal->addLayout(filaArchivo);
    principal->addLayout(centro);
    principal->addWidget(lblEstado);

    // ---- Conexiones ----
    connect(btnAgregar,  &QPushButton::clicked, this, [this] { agregarLibro(); });
    connect(txtAnio, &QLineEdit::returnPressed, this, [this] { agregarLibro(); });
    connect(btnPrestar,  &QPushButton::clicked, this, [this] { prestarLibro(); });
    connect(btnDevolver, &QPushButton::clicked, this, [this] { devolverLibro(); });
    connect(btnEliminar, &QPushButton::clicked, this, [this] { eliminarLibro(); });

    connect(btnAbrir, &QPushButton::clicked, this, [this] {
        QString ruta = QFileDialog::getOpenFileName(this, "Abrir catálogo", QString(),
                                                    "Catálogo (*.txt);;Todos (*)");
        if (!ruta.isEmpty())
            cargarArchivo(ruta);
    });
    connect(btnGuardar, &QPushButton::clicked, this, [this, btnGuardarC] {
        if (rutaActual.isEmpty())
            btnGuardarC->click();
        else
            guardarArchivo(rutaActual);
    });
    connect(btnGuardarC, &QPushButton::clicked, this, [this] {
        QString ruta = QFileDialog::getSaveFileName(this, "Guardar catálogo", "biblioteca.txt",
                                                    "Catálogo (*.txt)");
        if (!ruta.isEmpty())
            guardarArchivo(ruta);
    });

    // Intenta cargar el catálogo por defecto al iniciar
    if (QFile::exists("biblioteca.txt"))
        cargarArchivo("biblioteca.txt");
    else
        lblEstado->setText("No se encontró biblioteca.txt; se inicia con un catálogo vacío.");
}

// ----------------- Manejo centralizado de excepciones -----------------

void Ventana::ejecutar(const std::function<void()> &accion, const QString &exito)
{
    try {
        accion();
        refrescarTabla();
        lblEstado->setStyleSheet("color: green;");
        lblEstado->setText(exito);
    }
    catch (const EntradaInvalidaException &e) {        // datos del usuario
        lblEstado->setStyleSheet("color: #b36b00;");
        lblEstado->setText(QString::fromStdString(e.what()));
        QMessageBox::warning(this, QString::fromStdString(e.titulo()),
                             QString::fromStdString(e.what()));
        // Enfoca el campo con el error
        const std::string &c = e.campo();
        QLineEdit *campo = c == "Código" ? txtCodigo : c == "Título" ? txtTitulo
                         : c == "Autor"  ? txtAutor  : c == "Año"    ? txtAnio : nullptr;
        if (campo) { campo->setFocus(); campo->selectAll(); }
    }
    catch (const ArchivoException &e) {                // archivo inexistente o corrupto
        lblEstado->setStyleSheet("color: red;");
        lblEstado->setText(QString::fromStdString(e.titulo()) + ": se conservan los datos actuales.");
        QMessageBox::critical(this, QString::fromStdString(e.titulo()),
                              QString::fromStdString(e.what()));
    }
    catch (const BibliotecaException &e) {             // reglas de negocio
        lblEstado->setStyleSheet("color: red;");
        lblEstado->setText(QString::fromStdString(e.what()));
        QMessageBox::warning(this, QString::fromStdString(e.titulo()),
                             QString::fromStdString(e.what()));
    }
    catch (const std::exception &e) {                  // cualquier otro error
        lblEstado->setStyleSheet("color: red;");
        lblEstado->setText("Error inesperado.");
        QMessageBox::critical(this, "Error inesperado", QString::fromUtf8(e.what()));
    }
}

// ------------------------------ Acciones ------------------------------

void Ventana::agregarLibro()
{
    ejecutar([this] {
        Libro l = Biblioteca::crearLibro(txtCodigo->text(), txtTitulo->text(),
                                         txtAutor->text(), txtAnio->text());
        bib.agregar(l);
        limpiarFormulario();
    }, "Libro agregado correctamente.");
}

QString Ventana::codigoSeleccionado() const
{
    int fila = tabla->currentRow();
    if (fila < 0 || tabla->selectedItems().isEmpty())
        throw EntradaInvalidaException("Selección", "seleccione un libro en la tabla.");
    return tabla->item(fila, 0)->text();
}

void Ventana::prestarLibro()
{
    ejecutar([this] { bib.prestar(codigoSeleccionado()); }, "Libro prestado.");
}

void Ventana::devolverLibro()
{
    ejecutar([this] { bib.devolver(codigoSeleccionado()); }, "Libro devuelto.");
}

void Ventana::eliminarLibro()
{
    ejecutar([this] {
        QString cod = codigoSeleccionado();
        if (QMessageBox::question(this, "Confirmar", "¿Eliminar el libro " + cod + "?")
            == QMessageBox::Yes)
            bib.eliminar(cod);
    }, "Operación terminada.");
}

void Ventana::cargarArchivo(const QString &ruta)
{
    ejecutar([this, ruta] {
        bib.cargar(ruta);
        rutaActual = ruta;
    }, "Catálogo cargado desde " + ruta);
}

void Ventana::guardarArchivo(const QString &ruta)
{
    ejecutar([this, ruta] {
        bib.guardar(ruta);
        rutaActual = ruta;
    }, "Catálogo guardado en " + ruta);
}

// ------------------------------ Vista ------------------------------

void Ventana::refrescarTabla()
{
    const auto &libros = bib.libros();
    tabla->setRowCount(libros.size());
    for (int i = 0; i < libros.size(); ++i) {
        const Libro &l = libros[i];
        tabla->setItem(i, 0, new QTableWidgetItem(l.codigo));
        tabla->setItem(i, 1, new QTableWidgetItem(l.titulo));
        tabla->setItem(i, 2, new QTableWidgetItem(l.autor));
        tabla->setItem(i, 3, new QTableWidgetItem(QString::number(l.anio)));
        auto *estado = new QTableWidgetItem(l.disponible ? "Disponible" : "Prestado");
        estado->setForeground(l.disponible ? Qt::darkGreen : Qt::red);
        tabla->setItem(i, 4, estado);
    }
}

void Ventana::limpiarFormulario()
{
    txtCodigo->clear();
    txtTitulo->clear();
    txtAutor->clear();
    txtAnio->clear();
    txtCodigo->setFocus();
}
