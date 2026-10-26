#include "Terreno.h"

Terreno::Terreno() : tipo(TipoTerreno::Solo), quantidade(0) {}

Terreno::Terreno(TipoTerreno tipoInicial)
    : tipo(tipoInicial), quantidade(0) {}

TipoTerreno Terreno::getTipo() const {
    return tipo;
}

char Terreno::getSimbolo() const {
    switch (tipo) {
        case TipoTerreno::Cratera: return '*';
        case TipoTerreno::DepositoFerro: return 'f';
        case TipoTerreno::DepositoSilicio: return 's';
        case TipoTerreno::DepositoGelo: return 'g';
        default: return ' ';
    }
}

bool Terreno::eDeposito() const {
    return tipo == TipoTerreno::DepositoFerro ||
           tipo == TipoTerreno::DepositoSilicio ||
           tipo == TipoTerreno::DepositoGelo;
}

std::string Terreno::getNomeMateriaPrima() const {
    switch (tipo) {
        case TipoTerreno::DepositoFerro: return "ferro";
        case TipoTerreno::DepositoSilicio: return "silicio";
        case TipoTerreno::DepositoGelo: return "gelo";
        default: return "";
    }
}

int Terreno::getQuantidade() const {
    return quantidade;
}

void Terreno::setDeposito(TipoTerreno novoTipo, int novaQuantidade) {
    tipo = novoTipo;
    quantidade = novaQuantidade;
}