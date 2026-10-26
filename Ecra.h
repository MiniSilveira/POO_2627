#ifndef ECRA_H
#define ECRA_H

#include <string>

class Ecra {
public:
    static const int MAX_LINHAS = 40;
    static const int MAX_COLUNAS = 80;

    Ecra();

    void limpa();
    void moveCursorPara(int linha, int coluna);
    void escreveCaracter(char c);
    void escreveString(const std::string &texto);
    void desenhaMoldura(int linha, int coluna, int altura, int largura);
    void mostra() const;

private:
    char buffer[MAX_LINHAS][MAX_COLUNAS];
    int cursorLinha;
    int cursorColuna;
};

#endif