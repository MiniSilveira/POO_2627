#ifndef TERRENO_H
#define TERRENO_H

#include <string>

enum class TipoTerreno {
    Solo,
    Cratera,
    DepositoFerro,
    DepositoSilicio,
    DepositoGelo
};

class Terreno {
public:
    Terreno();
    explicit Terreno(TipoTerreno tipo);

    TipoTerreno getTipo() const;
    char getSimbolo() const;

    bool eDeposito() const;
    std::string getNomeMateriaPrima() const;
    int getQuantidade() const;

    void setDeposito(TipoTerreno tipo, int quantidade);

private:
    TipoTerreno tipo;
    int quantidade;
};

#endif