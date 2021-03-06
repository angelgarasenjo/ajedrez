//
// Created by Daniel Sánchez Vicente on 15/5/16.
//

#include "jugador.h"
#include "rey.h"
#include "tablero.h"
#include "casilla.h"
#include "juego.h"

#include <stdexcept>

using namespace std;

jugador::jugador(){}

jugador::jugador(string nombre, rey* miRey, set<pieza*>* misPiezas) : _nombre(nombre), _miRey(miRey), _misPiezas(misPiezas){
    _piezasCapturadas = new set<pieza*>();
}

const string& jugador::nombre() const {
    return _nombre;
}

const set<pieza*>& jugador::misPiezas() const {
    return *_misPiezas;
}

const rey& jugador::miRey() const {
    return *_miRey;
}

const bool jugador::movimiento() const {
    int fila1, fila2;
    char columnaLetra1, columnaLetra2;
    int columna1, columna2;

    string color = "blanco";

    //Color para el prompt de coordenadas
    if(miRey().color()){
        color = "negro";
    }


    bool exito = false;
    bool coordValidas = false;

    //Pedimos coordenadas hasta que las introducidas sean válidas
    while(!coordValidas || !exito){

        coordValidas = true;

        //obtenemos las coordenadas del teclado
        cout << color << " introduzca movimiento: ";
        cin >> columnaLetra1 >> fila1 >> columnaLetra2 >> fila2;


        //Traduce letra de columna a número
        columna1 = (int)(columnaLetra1 - 'a');
        columna2 = (int)(columnaLetra2 - 'a');

        //Traduce índices de fila
        fila1 = 8 - fila1;
        fila2 = 8 - fila2;

        //Bloque try catch para descartar errores

        try
        {
            // Comprobar que haya una pieza y si la hay que sea propiedad del jugador que hace el movimiento
            if (!tablero::getTablero().casillaPorCoord(fila1, columna1).ocupada()){
                cout << "La casilla seleccionada está vacía" << endl;
                coordValidas = false;
            }

                // Si hay pieza comprobamos que sea propiedad del jugador actual
            else if (misPiezas().find(&tablero::getTablero().casillaPorCoord(fila1, columna1).ocupante()) == misPiezas().end()){
                cout << "La pieza es propiedad del otro jugador" << endl;
                coordValidas = false;
            }

            // Comprobar que la casilla de destino no esté ocupada por una pieza del mismo color
            if (tablero::getTablero().casillaPorCoord(fila2, columna2).ocupada() && misPiezas().find(&tablero::getTablero().casillaPorCoord(fila2, columna2).ocupante()) != misPiezas().end()) {
                cout << "La casilla de destino está ocupada por otra de tus piezas" << endl;
                coordValidas = false;
            }

        } catch (out_of_range) {
            // Error de coordinadas fuera del tablero
            cout << "Las coordinadas no son válidas" << endl;
            coordValidas = false;
        }

        if (coordValidas) {
            // Comprobamos que el movimiento sea válido para el tipo de pieza que estamos moviendo
            exito = tablero::getTablero().casillaPorCoord(fila1, columna1).ocupante().moverPieza(*this, tablero::getTablero().casillaPorCoord(fila2, columna2));

            // Si el movimiento no es válido avisamos al jugador y volvemos a pedir coordenadas
            if (!exito) {
                cout << "El movimiento no es legal" << endl;
            }

        }

    }

    return exito;
}

//TODO: temporizador de jugadas

const void jugador::capturar(pieza& pieza) const {
    // Añdimos la pieza al set de piezas capturadas
    _piezasCapturadas->insert(&pieza);

    // Sacamos la pieza del tablero
    pieza.poner(NULL);

    //Ponemos el contador de tablas a 0
    juego::resetContadorTablas();
}