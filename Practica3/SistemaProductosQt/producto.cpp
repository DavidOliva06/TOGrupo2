#include "producto.h"
#include <sstream>
#include <iomanip>

Producto::Producto(const std::string& nombre, double precio,
                   int cantidad, double descuento)
    : nombre(nombre), precio(precio), cantidad(cantidad), descuento(descuento)
{
}

const std::string& Producto::getNombre() const { return nombre; }
double Producto::getPrecio() const { return precio; }
int Producto::getCantidad() const { return cantidad; }
double Producto::getDescuento() const { return descuento; }

std::string Producto::mostrarDatos() const
{
    std::ostringstream salida;
    salida << std::fixed << std::setprecision(2)
           << "Producto: " << nombre
           << "\nPrecio unitario: S/ " << precio
           << "\nCantidad: " << cantidad
           << "\nDescuento: " << descuento << "%"
           << "\nPrecio con descuento: S/ " << precioConDescuento();
    return salida.str();
}

std::string comparar(const Producto& a, const Producto& b)
{
    std::ostringstream salida;
    salida << std::fixed << std::setprecision(2);

    if (a.precio > b.precio)
        salida << "El producto más caro es: " << a.nombre
               << " (S/ " << a.precio << ")";
    else if (b.precio > a.precio)
        salida << "El producto más caro es: " << b.nombre
               << " (S/ " << b.precio << ")";
    else
        salida << "Ambos productos tienen el mismo precio.";

    return salida.str();
}

Producto Producto::operator+(const Producto& otro) const
{
    return Producto(nombre + " + " + otro.nombre,
                    precio + otro.precio,
                    cantidad + otro.cantidad,
                    0.0);
}

std::ostream& operator<<(std::ostream& salida, const Producto& p)
{
    salida << std::fixed << std::setprecision(2)
           << p.nombre
           << " | Precio: S/ " << p.precio
           << " | Cantidad: " << p.cantidad
           << " | Descuento: " << p.descuento << "%"
           << " | Precio con descuento: S/ " << p.precioConDescuento();
    return salida;
}
