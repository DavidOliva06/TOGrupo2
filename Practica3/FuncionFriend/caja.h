#ifndef CAJA_H
#define CAJA_H

class Caja {
private:
    double peso;

public:
    Caja(double p) {
        peso = p;
    }

    friend int comparar(Caja c1, Caja c2, Caja c3);
};

inline int comparar(Caja c1, Caja c2, Caja c3) {
    if (c1.peso == c2.peso &&
        c2.peso == c3.peso) {
        return 0;
    }

    if (c1.peso >= c2.peso &&
        c1.peso >= c3.peso) {
        return 1;
    }

    if (c2.peso >= c1.peso &&
        c2.peso >= c3.peso) {
        return 2;
    }

    return 3;
}

#endif
