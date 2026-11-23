#pragma once
#include <vector>
#include <iostream>
#include "Procesos.h"

class RoundRobin {
private:
    std::vector<Proceso*> cola;  
    int quantum;                  
    Proceso* enEjecucion;      
public:

    RoundRobin(int quantum);

    void agregar(Proceso* p);          
    void reencolar(Proceso* p);       
    bool vacio() const;                
    size_t tamano() const;             
    Proceso* frente() const;           

    Proceso* ejecutar(int& usado);

    void setEnEjecucion(Proceso* p);
    Proceso* getEnEjecucion() const;

    int getQuantum() const;
    void setQuantum(int q);


    void imprimir() const;              
    void imprimirColaEnLinea() const;   
};
