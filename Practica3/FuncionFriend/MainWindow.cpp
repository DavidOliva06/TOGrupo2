
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "caja.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Boton Comparar
void MainWindow::on_btnComparar_clicked() {
    bool ok1, ok2, ok3;

    double p1 = ui->txtPeso1->text().toDouble(&ok1);
    double p2 = ui->txtPeso2->text().toDouble(&ok2);
    double p3 = ui->txtPeso3->text().toDouble(&ok3);

    if (!ok1 || !ok2 || !ok3 ||
        p1 < 0 || p2 < 0 || p3 < 0) {
        QMessageBox::warning(
            this,
            "Error",
            "Ingrese pesos validos y positivos."
            );
        return;
    }

    Caja c1(p1);
    Caja c2(p2);
    Caja c3(p3);

    int resultado = comparar(c1, c2, c3);

    if (resultado == 0) {
        ui->lblResultado->setText(
            "Las tres cajas pesan lo mismo."
            );
    }
    else if (resultado == 1) {
        ui->lblResultado->setText(
            "La caja 1 tiene el mayor peso."
            );
    }
    else if (resultado == 2) {
        ui->lblResultado->setText(
            "La caja 2 tiene el mayor peso."
            );
    }
    else {
        ui->lblResultado->setText(
            "La caja 3 tiene el mayor peso."
            );
    }
}

// Boton Limpiar
void MainWindow::on_btnLimpiar_clicked() {
    ui->txtPeso1->clear();
    ui->txtPeso2->clear();
    ui->txtPeso3->clear();

    ui->lblResultado->setText("Resultado:");

    ui->txtPeso1->setFocus();
}
