#pragma once
#include <vector>
#include <iostream>
#include "Procesos.h"
using namespace std;

// Un bloque de memoria. Si es un hueco [0,tam,0]
struct Bloque{
    int tam;
    Proceso* p;
};

class PrimerAjuste {
private:
    vector<Bloque> bloques;

public:
    PrimerAjuste(int total){
        Bloque A;
        A.tam = total;
        A.p = NULL;
        bloques.push_back(A);
    }

    //se usa el primer hueco donde quepa el proceso
    bool asignar(Proceso* proc){
        int pos = -1;
        for(int i = 0; i < (int)bloques.size() && pos == -1; i++){
            if(bloques[i].p == NULL && bloques[i].tam >= proc->getTam()){
                pos = i;
            }
        }
        if(pos == -1) return false;

        int sobrante = bloques[pos].tam - proc->getTam();
        bloques[pos].tam = proc->getTam();
        bloques[pos].p = proc;
        if(sobrante > 0){
            Bloque hueco;
            hueco.tam = sobrante;
            hueco.p = NULL;
            bloques.insert(bloques.begin() + pos + 1, hueco);
        }
        return true;
    }

    void liberar(int id){
        for(int i = 0; i < (int)bloques.size(); i++){
            if(bloques[i].p != NULL && bloques[i].p->getID() == id){
                bloques[i].p = NULL;
                return;
            }
        }
    }

    // Une huecos contiguos. Regresa true si unio alguno
    bool juntarHuecos(){
        bool cambio = false;
        int i = 0;
        while(i < (int)bloques.size() - 1){
            if(bloques[i].p == NULL && bloques[i + 1].p == NULL){
                bloques[i].tam += bloques[i + 1].tam;
                bloques.erase(bloques.begin() + i + 1);
                cambio = true;
            }
            else i++;
        }
        return cambio;
    }

    // Cuenta los huecos donde cabe un proceso de tamano tam
    int contarHuecos(int tam) const {
        int n = 0;
        for(int i = 0; i < (int)bloques.size(); i++){
            if(bloques[i].p == NULL && bloques[i].tam >= tam) n++;
        }
        return n;
    }

    // Imprime los huecos donde cabe un proceso de tamano tam
    void imprimirHuecos(int tam) const {
        for(int i = 0; i < (int)bloques.size(); i++){
            if(bloques[i].p == NULL && bloques[i].tam >= tam){
                cout << "[0," << bloques[i].tam << ",0] ";
            }
        }
    }

    void imprimir() const {
        for(int i = 0; i < (int)bloques.size(); i++){
            if(bloques[i].p != NULL) bloques[i].p->imprimir();
            else std::cout << "[0," << bloques[i].tam << ",0]";
        }
    cout << "\n";
    }
};