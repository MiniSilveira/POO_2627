#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Ecra.h"
#include "Fabrica.h"
#include "Terreno.h"

bool fabricaFoiCriada = false;

void mostrarTerreno(const Fabrica &fabrica,
                    const std::string &linhaStr,
                    const std::string &colunaStr) {
    if (linhaStr.size() != 1 || colunaStr.size() != 1) {
        std::cout << "Posicao invalida.\n";
        return;
    }

    int linha = linhaStr[0] - 'a';
    int coluna = colunaStr[0] - 'a';

    if (!fabrica.posicaoValida(linha, coluna)) {
        std::cout << "Posicao fora da fabrica.\n";
        return;
    }

    const Terreno &terreno = fabrica.getTerreno(linha, coluna);

    std::cout << "Posicao: " << linhaStr << colunaStr << "\n";

    switch (terreno.getTipo()) {
        case TipoTerreno::Solo:
            std::cout << "Terreno: solo\n";
            break;
        case TipoTerreno::Cratera:
            std::cout << "Terreno: cratera\n";
            break;
        case TipoTerreno::DepositoFerro:
            std::cout << "Terreno: deposito de ferro\n"
                      << "Quantidade: " << terreno.getQuantidade() << "\n";
            break;
        case TipoTerreno::DepositoSilicio:
            std::cout << "Terreno: deposito de silicio\n"
                      << "Quantidade: " << terreno.getQuantidade() << "\n";
            break;
        case TipoTerreno::DepositoGelo:
            std::cout << "Terreno: deposito de gelo\n"
                      << "Quantidade: " << terreno.getQuantidade() << "\n";
            break;
    }
}

int main() {
    Fabrica fabrica;
    Ecra ecra;
    std::string linha;

    std::cout << "Simulador de fabrica em Marte\n";

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, linha))
            break;

        std::istringstream stream(linha);
        std::vector<std::string> partes;
        std::string palavra;

        while (stream >> palavra)
            partes.push_back(palavra);

        if (partes.empty())
            continue;

        const std::string &comando = partes[0];

        if (comando == "fim") {
            std::cout << "Programa terminado.\n";
            break;
        }

        if (comando == "fabrica") {
            if (fabricaFoiCriada) {
                std::cout << "A fabrica ja foi criada.\n";
                continue;
            }

            if (partes.size() != 3) {
                std::cout << "Sintaxe: fabrica <linhas> <colunas>\n";
                continue;
            }

            try {
                int l = std::stoi(partes[1]);
                int c = std::stoi(partes[2]);

                if (fabrica.criar(l, c)) {
                    fabricaFoiCriada = true;
                    fabrica.mostra(ecra);
                } else {
                    std::cout << "Dimensoes invalidas.\n";
                }
            } catch (...) {
                std::cout << "Parametros invalidos.\n";
            }

            continue;
        }

        if (comando == "carrega") {
            if (fabricaFoiCriada) {
                std::cout << "A fabrica ja foi criada.\n";
                continue;
            }

            if (partes.size() != 2) {
                std::cout << "Sintaxe: carrega <ficheiro>\n";
                continue;
            }

            if (fabrica.carregar(partes[1])) {
                fabricaFoiCriada = true;
                fabrica.mostra(ecra);
            } else {
                std::cout << "Ficheiro invalido ou mapa invalido.\n";
            }

            continue;
        }

        if (comando == "lposicao") {
            if (!fabricaFoiCriada) {
                std::cout << "Crie primeiro a fabrica.\n";
                continue;
            }

            if (partes.size() < 2 || partes[1].size() != 2) {
                std::cout << "Sintaxe: lposicao <lc>\n";
                continue;
            }

            mostrarTerreno(fabrica, partes[1].substr(0, 1),
                           partes[1].substr(1, 1));

            continue;
        }

        std::cout << "Comando desconhecido.\n";
    }

    return 0;
}