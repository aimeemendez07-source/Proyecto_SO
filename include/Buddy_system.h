#pragma once
#include <vector>
#include <iostream>
#include "Procesos.h"
using namespace std;

// Si p es NULL esta libre
struct BloqueBuddy{
    int inicio;
    int tam;        // siempre potencia de 2
    Proceso* p;
};

class BuddySystem{
private:
    vector<BloqueBuddy> bloques;
    int total;
    static const int MIN_SPLIT = 32;   // KB
    // Menor potencia de 2 (minimo 32) donde cabe n
    int potencia(int n){
        int t = MIN_SPLIT;
        while(t < n){
            t = t * 2;
        }
        return t;
    }

public:
    BuddySystem(int total){
        this->total = total;
        BloqueBuddy A;
        A.inicio = 0;
        A.tam = total;
        A.p = NULL;
        bloques.push_back(A);
    }

    bool asignar(Proceso* proc){
        int espacio  = potencia(proc->getTam());
        // buscar el bloque libre mas chico donde quepa
        int pos = -1;
        for(int i = 0; i < (int)bloques.size(); i++){
            if(bloques[i].p == NULL && bloques[i].tam >= espacio){
                if(pos == -1 || bloques[i].tam < bloques[pos].tam) pos = i;
            }
        }
        if(pos == -1) return false;

        // dividir a la mitad mientras sobre espacio
        while(bloques[pos].tam > espacio){
            int mitad = bloques[pos].tam / 2;
            BloqueBuddy der;
            der.inicio = bloques[pos].inicio + mitad;
            der.tam = mitad;
            der.p = NULL;
            bloques[pos].tam = mitad;
            bloques.insert(bloques.begin() + pos + 1, der);
        }
        bloques[pos].p = proc;
        return true;
    }

    void liberar(int id){
        int pos = -1;
        for(int i = 0; i < (int)bloques.size(); i++){
            if(bloques[i].p != NULL && bloques[i].p->getID() == id) pos = i;
        }
        if(pos == -1) return;
        bloques[pos].p = NULL;

        // unir con su companero (buddy) mientras este libre y del mismo tamano
        while(bloques[pos].tam < total){
            int tam = bloques[pos].tam;
            int comp;
            if(bloques[pos].inicio % (2 * tam) == 0) comp = pos + 1;   // el companero esta a la derecha
            else comp = pos - 1;                                        // el companero esta a la izquierda

            if(comp < 0 || comp >= (int)bloques.size()) break;
            if(bloques[comp].p != NULL || bloques[comp].tam != tam) break;

            int izq = (pos < comp) ? pos : comp;
            bloques[izq].tam = tam * 2;
            bloques.erase(bloques.begin() + izq + 1);
            pos = izq;
        }
    }

    // Procesos: [id,tam,restante]  Libres: [0,tam,0]
    void imprimir() const {
        for(int i = 0; i < (int)bloques.size(); i++){
            bool esIzq=(bloques[i].inicio % (2 * bloques[i].tam)==0);
            char lado = esIzq ? 'I' : 'D';
            cout<< lado <<":";
            if(bloques[i].p !=NULL){
                bloques[i].p->imprimir();
                
            }
            else {
                cout << "[0," << bloques[i].tam << ",0]";
            }
            cout<< " ";
        }
        cout << "\n";
    }
    
};
