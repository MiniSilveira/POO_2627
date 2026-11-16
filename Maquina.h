#ifndef MAQUINA_H
#define MAQUINA_H

#include <string>

class Item;

class Maquina {
public:
    Maquina(int linha, int coluna, char direcao, int custo);

    virtual ~Maquina() = default;

    int getLinha() const;
    int getColuna() const;
    char getDirecao() const;
    int getCusto() const;

    virtual char getSimbolo() const = 0;
    virtual std::string getNome() const = 0;

    bool roda(char novaDirecao);
    virtual std::string getDescricao() const;

    virtual bool aceita(const Item &item) const;

    virtual bool recebe(Item *item, int instante);

    virtual Item *itemParaEntregar(int instante) const;

    virtual Item *retiraItem();

    bool tentaEntregar(Maquina &destino, int instante);

    virtual std::string atua(int instante);

private:
    int linha;
    int coluna;
    char direcao;
    int custo;
};

#endif