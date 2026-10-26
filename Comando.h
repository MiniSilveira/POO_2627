#ifndef COMANDO_H
#define COMANDO_H

#include <string>
#include <vector>

class Fabrica;

class Comando {
public:
    virtual ~Comando() = default;

    virtual bool executa(const std::vector<std::string> &argumentos,
                         Fabrica &fabrica) = 0;

    virtual std::string getNome() const = 0;
    virtual std::string getAjuda() const = 0;
};

#endif