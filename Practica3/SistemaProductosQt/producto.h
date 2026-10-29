#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>
#include <ostream>

class Producto
{
private:
    std::string nombre;
    double precio;
    int cantidad;
    double descuento;

public:
    Producto(const std::string& nombre = "Producto combinado",
             double precio = 0.0, int cantidad = 0, double descuento = 0.0);

    // inline: calcula el precio unitario después del descuento.
    inline double precioConDescuento() const
    {
        return precio * (1.0 - descuento / 100.0);
    }

    // Getters const: consultan datos sin modificar el objeto.
    const std::string& getNombre() const;
    double getPrecio() const;
    int getCantidad() const;
    double getDescuento() const;

    std::string mostrarDatos() const;

    // friend: puede consultar los atributos privados de ambos productos.
    friend std::string comparar(const Producto& a, const Producto& b);

    // operator+: suma los precios unitarios y las cantidades.
    Producto operator+(const Producto& otro) const;

    // Reto adicional: salida legible con cout.
    friend std::ostream& operator<<(std::ostream& salida, const Producto& p);
};

#endif // PRODUCTO_H
