#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <stdexcept>
#include <string>

int convertirAEntero(const QString &texto)
{
    std::string s = texto.trimmed().toStdString();
    if (s.empty())
        throw std::invalid_argument("vacio");

    std::size_t pos = 0;
    int valor = std::stoi(s, &pos);   

    if (pos != s.size())              
        throw std::invalid_argument("caracteres extra");

    return valor;
}

class Ventana : public QWidget
{
public:
    Ventana(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("Conversor de texto a número");
        resize(380, 160);

        auto *lblInstruccion = new QLabel("Ingrese un número entero:");
        entrada = new QLineEdit;
        entrada->setPlaceholderText("Ej. 42");

        auto *btnConvertir = new QPushButton("Convertir");
        auto *btnLimpiar   = new QPushButton("Limpiar");

        lblResultado = new QLabel("Resultado: —");
        lblResultado->setStyleSheet("font-weight: bold;");

        auto *botones = new QHBoxLayout;
        botones->addWidget(btnConvertir);
        botones->addWidget(btnLimpiar);

        auto *layout = new QVBoxLayout(this);
        layout->addWidget(lblInstruccion);
        layout->addWidget(entrada);
        layout->addLayout(botones);
        layout->addWidget(lblResultado);

        connect(btnConvertir, &QPushButton::clicked, this, [this] { convertir(); });
        connect(entrada, &QLineEdit::returnPressed, this, [this] { convertir(); });
        connect(btnLimpiar, &QPushButton::clicked, this, [this] {
            entrada->clear();
            lblResultado->setText("Resultado: —");
            lblResultado->setStyleSheet("font-weight: bold;");
            entrada->setFocus();
        });
    }

private:
    void convertir()
    {
        try {
            int numero = convertirAEntero(entrada->text());
            lblResultado->setText(QString("Resultado: %1 (el doble es %2)")
                                      .arg(numero)
                                      .arg(static_cast<long long>(numero) * 2));
            lblResultado->setStyleSheet("font-weight: bold; color: green;");
        }
        catch (const std::exception &) {   
            lblResultado->setText("Resultado: error");
            lblResultado->setStyleSheet("font-weight: bold; color: red;");
            QMessageBox::warning(this, "Error", "Debe ingresar un número válido");
            entrada->selectAll();
            entrada->setFocus();
        }
    }

    QLineEdit *entrada;
    QLabel *lblResultado;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Ventana v;
    v.show();
    return app.exec();
}
