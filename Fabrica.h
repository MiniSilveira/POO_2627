#ifndef FABRICA_H
#define FABRICA_H

#include <string>
#include <vector>
#include "Terreno.h"

class Ecra;

class Fabrica {
public:
    Fabrica();

    bool criar(int linhas, int colunas);
    bool carregar(const std::string &nomeFicheiro);

    int getLinhas() const;
    int getColunas() const;
    int getCreditos() const;
    int getInstante() const;

    bool posicaoValida(int linha, int coluna) const;

    const Terreno &getTerreno(int linha, int coluna) const;

    void mostra(Ecra &ecra) const;

private:
    int linhas;
    int colunas;
    int creditos;
    int instante;

    std::vector<std::vector<Terreno>> grelha;

    void inicializarGrelha(int novasLinhas, int novasColunas);
    void colocarElementosAleatorios();
};

#endif