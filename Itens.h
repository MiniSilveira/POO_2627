#ifndef ITENS_H
#define ITENS_H

#include <string>

class Item {
public:
    virtual ~Item() = default;

    const std::string &getNome() const;
    char getSimbolo() const;
    int getValor() const;

    virtual bool eMateriaPrima() const;

protected:
    Item(const std::string &nome, char simbolo, int valor);

private:
    std::string nome;
    char simbolo;
    int valor;
};

class MateriaPrima : public Item {
public:
    bool eMateriaPrima() const override;

protected:
    MateriaPrima(
        const std::string &nome,
        char simbolo,
        int valor
    );
};

class Produto : public Item {
protected:
    Produto(
        const std::string &nome,
        char simbolo,
        int valor
    );
};

class MinerioFerro : public MateriaPrima {
public:
    MinerioFerro();
};

class SilicioBruto : public MateriaPrima {
public:
    SilicioBruto();
};

class Gelo : public MateriaPrima {
public:
    Gelo();
};

class PlacaFerro : public Produto {
public:
    PlacaFerro();
};

class SilicioPuro : public Produto {
public:
    SilicioPuro();
};

class Agua : public Produto {
public:
    Agua();
};

class KitHabitat : public Produto {
public:
    KitHabitat();
};

#endif