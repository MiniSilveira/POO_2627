#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include "Comando.h"

#include <memory>
#include <string>
#include <vector>

class Fabrica;
class Ecra;

class Interpretador {
public:
    Interpretador();

    Interpretador(const Interpretador &) = delete;
    Interpretador &operator=(const Interpretador &) = delete;

    ResultadoComando processaLinha(
        const std::string &linha,
        Fabrica &fabrica,
        Ecra &ecra
    );

    bool mostraAjuda(const std::string &nome) const;

    ResultadoComando executaFicheiro(
        const std::string &nomeFicheiro,
        Fabrica &fabrica,
        Ecra &ecra
    );

private:
    std::vector<std::unique_ptr<Comando>> comandos;

    int ficheirosEmExecucao = 0;

    static constexpr int MAX_FICHEIROS_EM_EXECUCAO = 32;
};

#endif