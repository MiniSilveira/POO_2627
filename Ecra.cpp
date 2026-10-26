#include "Ecra.h"
#include <iostream>

Ecra::Ecra() : cursorLinha(0), cursorColuna(0) {
    limpa();
}

void Ecra::limpa() {
    for (int l = 0; l < MAX_LINHAS; ++l)
        for (int c = 0; c < MAX_COLUNAS; ++c)
            buffer[l][c] = ' ';

    cursorLinha = 0;
    cursorColuna = 0;
}

void Ecra::moveCursorPara(int linha, int coluna) {
    if (linha >= 0 && linha < MAX_LINHAS)
        cursorLinha = linha;

    if (coluna >= 0 && coluna < MAX_COLUNAS)
        cursorColuna = coluna;
}

void Ecra::escreveCaracter(char c) {
    if (cursorLinha < 0 || cursorLinha >= MAX_LINHAS ||
        cursorColuna < 0 || cursorColuna >= MAX_COLUNAS) {
        return;
        }

    buffer[cursorLinha][cursorColuna] = c;

    ++cursorColuna;
    if (cursorColuna >= MAX_COLUNAS) {
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
    for (int l = 0; l < MAX_LINHAS; ++l) {
        for (int c = 0; c < MAX_COLUNAS; ++c)
            std::cout << buffer[l][c];
        std::cout << '\n';
    }
}