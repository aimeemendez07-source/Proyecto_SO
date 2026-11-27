//Simulador de colocacion de memoria y Round Robin
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#include "Procesos.h"
#include "PrimerAjuste.h"
#include "Buddy_system.h"
#include "RoundRobin.h"
using namespace std;

vector<int> llegadas(1, 0);   // llegadas[id] = instante en que el proceso entro a memoria

int leerEntero(string mensaje, int minimo, int maximo){
    int v;
    while(true){
        cout << mensaje << " (" << minimo << "-" << maximo << "): ";
        if(cin >> v && v >= minimo && v <= maximo) return v;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Valor invalido\n";
    }
}

// 1 = primer ajuste, 2 = buddy system
bool asignar(int alg, PrimerAjuste &a, BuddySystem &b, Proceso* p){
    if(alg == 2) return b.asignar(p);
    return a.asignar(p);
}

void liberar(int alg, PrimerAjuste &a, BuddySystem &b, int id){
    if(alg == 2) b.liberar(id);
    else a.liberar(id);
}

void imprimirMemoria(int alg, PrimerAjuste &a, BuddySystem &b){
    if(alg == 2) b.imprimir();
    else a.imprimir();
}

// Genera un proceso aleatorio con base en los parametros
Proceso* nuevoProceso(int id, int maxMem, int maxCuanto){
    llegadas.push_back(0);
    return new Proceso(id, rand() % maxMem + 1, rand() % maxCuanto + 1);
}

int main(){
    srand(time(0));
    // Configuracion de parametros
    cout << "SIMULADOR DE MEMORIA Y ROUND ROBIN \n";
    cout << "1) Primer ajuste\n2) Buddy System\n";
    int alg = leerEntero("Algoritmo", 1, 2);

    int mb = 0;
    while(mb != 1 && mb != 4 && mb != 8) mb = leerEntero("Tamano de memoria en MB (1, 4 u 8)", 1, 8);
    int memoria = mb * 1024;   // en KB

    int quantum   = leerEntero("Cuanto del sistema", 1, 100);
    int maxMem    = leerEntero("Tamano maximo de memoria de proceso (KB)", 1, memoria);
    int maxCuanto = leerEntero("Tamano maximo del cuanto por proceso", 1, 100);

    PrimerAjuste ajuste(memoria);   // se usa si alg es 1
    BuddySystem buddy(memoria);     // se usa si alg es 2
    RoundRobin rr(quantum);

    cout << "\nSalida inicial\n";
    imprimirMemoria(alg, ajuste, buddy);

    // Simulacion -----
    int idSig = 1;
    Proceso* pendiente = nuevoProceso(idSig++, maxMem, maxCuanto);   // siempre hay uno listo para entrar
    int reloj = 0, ciclo = 0;
    int atendidos = 0, sumaAtencion = 0;
    bool parar = false;

    while(!parar){
        ciclo++;
        cout << "\n========== CICLO " << ciclo << " ==========\n";

        // avisar cuando hay 3 o mas huecos donde cabe el proceso entrante
        if(alg == 1){
            int huecos = ajuste.contarHuecos(pendiente->getTam());
            if(huecos >= 3){
                cout << "AVISO: hay " << huecos << " huecos donde cabe el proceso ";
                pendiente->imprimir();
                cout << ": ";
                ajuste.imprimirHuecos(pendiente->getTam());
                cout << "\nPrimer ajuste lo coloca en el primero\n";
            }
        }

        // Entrada de proceso a memoria
        if(asignar(alg, ajuste, buddy, pendiente)){
            llegadas[pendiente->getID()] = reloj;
            cout << "Entra Proceso ";
            pendiente->imprimir();
            cout << "\n";
            rr.agregar(pendiente);
            imprimirMemoria(alg, ajuste, buddy);
            pendiente = nuevoProceso(idSig++, maxMem, maxCuanto);
        }
        else{
            cout << "Proceso ";
            pendiente->imprimir();
            cout << " en espera (no hay espacio)\n";
        }

        // Round Robin
        if(!rr.vacio()){
            cout << "Atendiendo proceso ";
            rr.frente()->imprimir();
            cout << "\n";

            int usado;
            Proceso* p = rr.ejecutar(usado);
            reloj = reloj + usado;
            rr.imprimir();   // Lista RR

            if(p->terminado()){
                atendidos++;
                sumaAtencion = sumaAtencion + (reloj - llegadas[p->getID()]);
                cout << "Descargando proceso [" << p->getID() << "," << p->getTam() << ",0]\n";
                liberar(alg, ajuste, buddy, p->getID());
                delete p;
                imprimirMemoria(alg, ajuste, buddy);

                if(alg == 1 && ajuste.juntarHuecos()){
                    cout << "Condensando huecos\n";
                    imprimirMemoria(alg, ajuste, buddy);
                }
            }
        }
        else{
            reloj++;
            cout << "No hay procesos en memoria\n";
        }

        // Impresion final de ciclo
        cout << "Fin de ciclo: ";
        imprimirMemoria(alg, ajuste, buddy);
        cout << "Proceso listo para entrar: ";
        pendiente->imprimir();
        cout << "\n";
        
        if(ciclo >= 10){
            cout << "\nSe ejecutaron " << ciclo << " ciclos. Desea continuar? (s = si, otra tecla = no)\n";
            char tecla = _getch();
            if(tecla != 's' && tecla != 'S') parar = true;
        }
        
    }

    // Estadisticas de la simulacion
    cout << "\nESTADISTICAS\n";
    cout << "Numero total de procesos atendidos: " << atendidos << "\n";
    if(atendidos > 0)
        cout << "Tiempo medio de atencion: " << (double)sumaAtencion / atendidos << "\n";
    else
        cout << "Tiempo medio de atencion: no hubo procesos terminados\n";

    return 0;
}