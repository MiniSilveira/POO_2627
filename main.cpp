#include "Ecra.h"
#include "Fabrica.h"
#include "Interpretador.h"

#include <iostream>
#include <string>




int main() {


    Fabrica fabrica;
    Ecra ecra;
    Interpretador interpretador;



    std::string linha;

    std::cout << "Simulador de fabrica em Marte\n";

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, linha)) {
            break;
        }

        const ResultadoComando resultado =
            interpretador.processaLinha(linha, fabrica, ecra);

        if (resultado == ResultadoComando::Terminar) {
            break;
        }
    }

    return 0;
}