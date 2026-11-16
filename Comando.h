#ifndef COMANDO_H
#define COMANDO_H

#include <string>
#include <vector>

class Fabrica;
class Ecra;

enum class ResultadoComando {
    Erro,
    Sucesso,
    Terminar
};

class Comando {
public:
    virtual ~Comando() = default;

    virtual std::string getNome() const = 0;
    virtual std::string getAjuda() const = 0;

    virtual ResultadoComando executa(
        const std::vector<std::string> &argumentos,
        Fabrica &fabrica,
        Ecra &ecra
    ) = 0;
};

#endif