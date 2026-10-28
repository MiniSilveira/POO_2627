#ifndef ECRA_H
#define ECRA_H

#include "Settings.h"
#include <string>

class Ecra {
public:
    

    Ecra();

    void limpa();
    void moveCursorPara(int linha, int coluna);
    void escreveCaracter(char c);
    void escreveString(const std::string &texto);
    void desenhaMoldura(int linha, int coluna, int altura, int largura);
    void mostra() const;

private:
    char buffer[Settings::ecra_max_linhas][Settings::ecra_max_colunas];
    int cursorLinha;
    int cursorColuna;
};

#endif