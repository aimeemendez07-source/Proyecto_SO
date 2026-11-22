
#include "RoundRobin.h"


RoundRobin::RoundRobin(int quantum)
    : quantum(quantum), enEjecucion(nullptr) {
    if (quantum <= 0) this->quantum = 1;
}

void RoundRobin::agregar(Proceso* p) {
    if (p != nullptr) cola.push_back(p);
}

void RoundRobin::reencolar(Proceso* p) {
    if (p != nullptr && !p->terminado()) {
        cola.push_back(p);
    }
}

bool RoundRobin::vacio() const {
    return cola.empty();
}

size_t RoundRobin::tamano() const {
    return cola.size();
}

Proceso* RoundRobin::frente() const {
    if (cola.empty()) return nullptr;
    return cola.front();
}

Proceso* RoundRobin::ejecutar(int& usado) {
    usado = 0;

    if (cola.empty()) {
        enEjecucion = nullptr;
        return nullptr;
    }

    
    Proceso* p = cola.front();
    cola.erase(cola.begin());

    
    int restante = p->gettiempoRestante();
    usado = (restante < quantum) ? restante : quantum;


    p->setTiempoRes(restante - usado);


    if (p->gettiempoRestante() > 0) {
        cola.push_back(p);
    }

    enEjecucion = p;
    return p;
}

void RoundRobin::setEnEjecucion(Proceso* p) {
    enEjecucion = p;
}

Proceso* RoundRobin::getEnEjecucion() const {
    return enEjecucion;
}

int RoundRobin::getQuantum() const {
    return quantum;
}

void RoundRobin::setQuantum(int q) {
    if (q > 0) quantum = q;
}

void RoundRobin::imprimir() const {
    std::cout << "Lista RR ";
    for (size_t i = 0; i < cola.size(); ++i) {
        cola[i]->imprimir();
        std::cout << " ";
    }
    std::cout << "\n";
}

void RoundRobin::imprimirColaEnLinea() const {
    for (size_t i = 0; i < cola.size(); ++i) {
        cola[i]->imprimir();
        std::cout << " ";
    }
}
