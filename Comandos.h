#ifndef COMANDOS_H
#define COMANDOS_H

#include "Comando.h"

class Interpretador;

// --------------------------------------------------
// fabrica
// --------------------------------------------------

class ComandoFabrica : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// carrega
// --------------------------------------------------

class ComandoCarrega : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// lposicao
// --------------------------------------------------

class ComandoLPosicao : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// fim
// --------------------------------------------------

class ComandoFim : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// ajuda
// --------------------------------------------------

class ComandoAjuda : public Comando {
public:
    explicit ComandoAjuda(const Interpretador &interpretador);

    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;

private:
    const Interpretador &interpretador;
};

// --------------------------------------------------
// executa
// --------------------------------------------------

class ComandoExecuta : public Comando {
public:
    explicit ComandoExecuta(Interpretador &interpretador);

    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;

private:
    Interpretador &interpretador;
};

// --------------------------------------------------
// compra
// --------------------------------------------------

class ComandoCompra : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// vende
// --------------------------------------------------

class ComandoVende : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// roda
// --------------------------------------------------

class ComandoRoda : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// estado
// --------------------------------------------------

class ComandoEstado : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// avanca
// --------------------------------------------------

class ComandoAvanca : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

// --------------------------------------------------
// Comandos com acao prevista para a entrega final
// --------------------------------------------------

class ComandoMelhora : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

class ComandoContrata : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

class ComandoAtribui : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

class ComandoDespede : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

class ComandoGrava : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

class ComandoRecupera : public Comando {
public:
    std::string getNome() const override;
    std::string getAjuda() const override;

    ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) override;
};

#endif