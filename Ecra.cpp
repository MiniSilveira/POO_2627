#include "Ecra.h"
#include <iostream>

Ecra::Ecra() : cursorLinha(0), cursorColuna(0) {
    limpa();
}

void Ecra::limpa() {
    for (int l = 0; l < Settings::ecra_max_linhas; ++l)
        for (int c = 0; c < Settings::ecra_max_colunas; ++c)
            buffer[l][c] = ' ';

    cursorLinha = 0;
    cursorColuna = 0;
}

void Ecra::moveCursorPara(int linha, int coluna) {
    if (linha >= 0 && linha < Settings::ecra_max_linhas)
        cursorLinha = linha;

    if (coluna >= 0 && coluna < Settings::ecra_max_colunas)
        cursorColuna = coluna;
}

void Ecra::escreveCaracter(char c) {
    if (cursorLinha < 0 || cursorLinha >= Settings::ecra_max_linhas ||
        cursorColuna < 0 || cursorColuna >= Settings::ecra_max_colunas) {
        return;
        }

    buffer[cursorLinha][cursorColuna] = c;

    ++cursorColuna;
    if (cursorColuna >= Settings::ecra_max_colunas) {
        cursorColuna = 0;
        ++cursorLinha;
    }
}

void Ecra::escreveString(const std::string &texto) {
    for (char c : texto)
        escreveCaracter(c);
}

void Ecra::desenhaMoldura(int linha, int coluna, int altura, int largura) {
    if (altura < 2 || largura < 2)
        return;

    for (int c = 0; c < largura; ++c) {
        moveCursorPara(linha, coluna + c);
        escreveCaracter('-');

        moveCursorPara(linha + altura - 1, coluna + c);
        escreveCaracter('-');
    }

    for (int l = 0; l < altura; ++l) {
        moveCursorPara(linha + l, coluna);
        escreveCaracter('|');

        moveCursorPara(linha + l, coluna + largura - 1);
        escreveCaracter('|');
    }
}

void Ecra::mostra() const {
    for (int l = 0; l < Settings::ecra_max_linhas; ++l) {
        for (int c = 0; c < Settings::ecra_max_colunas; ++c)
            std::cout << buffer[l][c];
        std::cout << '\n';
    }
}