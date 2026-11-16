#ifndef MAQUINAS_H
#define MAQUINAS_H

#include "Maquina.h"
#include "Itens.h"

#include <random>
#include <cstddef>

class Perfuradora : public Maquina {
public:
    Perfuradora(int linha, int coluna, char direcao);

    char getSimbolo() const override;
    std::string getNome() const override;
};

class Transportador : public Maquina {
public:
    Transportador(int linha, int coluna, char direcao);
    ~Transportador() override;

    Transportador(const Transportador &) = delete;
    Transportador &operator=(const Transportador &) = delete;

    char getSimbolo() const override;
    std::string getNome() const override;
    std::string getDescricao() const override;

    bool aceita(const Item &item) const override;
    bool recebe(Item *item, int instante) override;

    Item *itemParaEntregar(int instante) const override;
    Item *retiraItem() override;

private:
    Item *item;
    int instanteRececao;
};

class Refinaria : public Maquina {
public:
    Refinaria(int linha, int coluna, char direcao);

    char getSimbolo() const override;
    std::string getNome() const override;
};

class Oficina : public Maquina {
public:
    Oficina(int linha, int coluna, char direcao);

    char getSimbolo() const override;
    std::string getNome() const override;
};

class Silo : public Maquina {
public:
    Silo(int linha, int coluna, char direcao);
    ~Silo() override;

    Silo(const Silo &) = delete;
    Silo &operator=(const Silo &) = delete;

    char getSimbolo() const override;
    std::string getNome() const override;
    std::string getDescricao() const override;

    bool aceita(const Item &item) const override;
    bool recebe(Item *item, int instante) override;

    Item *itemParaEntregar(int instante) const override;
    Item *retiraItem() override;

    const Item *consultaPrimeiro() const;
    std::size_t getNumeroItens() const;
    std::string atua(int instante) override;

private:
    struct No {
        Item *item;
        int instanteRececao;
        No *seguinte;
    };

    No *primeiro;
    No *ultimo;
    std::size_t numeroItens;
    std::mt19937 gerador;
};

class Lancador : public Maquina {
public:
    Lancador(int linha, int coluna, char direcao);

    char getSimbolo() const override;
    std::string getNome() const override;
};

#endif