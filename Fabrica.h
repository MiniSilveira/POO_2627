#ifndef FABRICA_H
#define FABRICA_H

#include "Maquina.h"
#include "Terreno.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

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

    const Maquina *getMaquina(int linha, int coluna) const;
    std::size_t getNumeroMaquinas() const;

    bool comprar(
        char tipo,
        int linha,
        int coluna,
        char direcao,
        std::string &erro
    );

    bool vender(
    int linha,
    int coluna,
    std::string &erro
);

    bool rodar(
        int linha,
        int coluna,
        char direcao,
        std::string &erro
    );

    void mostra(Ecra &ecra) const;

    void mostraEstado() const;


    Maquina *getMaquina(int linha, int coluna);

    bool avancar(
        int numeroInstantes,
        Ecra &ecra,
        std::string &erro
    );



private:
    int linhas;
    int colunas;
    int creditos;
    int instante;

    std::vector<std::vector<Terreno>> grelha;
    std::vector<std::unique_ptr<Maquina>> maquinas;
    std::vector<std::string> acontecimentos;

    void inicializarGrelha(int novasLinhas, int novasColunas);
    void colocarElementosAleatorios();
};

#endif