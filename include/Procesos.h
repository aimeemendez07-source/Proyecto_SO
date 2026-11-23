// procesos
#pragma once

#include <iostream>

class Proceso {
private:
    int id;
    int tam;
    int cuantum;
    int tiempoRestante;

public:
    Proceso(int id, int tam, int tiempo)
        : id(id), tam(tam), cuantum(tiempo), tiempoRestante(tiempo) {}

    int getID() const { return id; }
    int getTam() const { return tam; }
    int getCuantum() const { return cuantum; }
    int gettiempoRestante() const { return tiempoRestante; }

    void setTiempoRes(int t) { tiempoRestante = t; }

    bool terminado() const { return tiempoRestante <= 0; }

    void imprimir() const {
        std::cout << "[" << id << "," << tam << "," << tiempoRestante << "]";
    }
};