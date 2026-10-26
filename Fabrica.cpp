#include "Fabrica.h"
#include "Ecra.h"
#include "Settings.h"

#include <algorithm>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// --------------------------------------------------
// Construtor
// --------------------------------------------------

Fabrica::Fabrica()
    : linhas(0),
      colunas(0),
      creditos(Settings::fabrica_creditos_iniciais),
      instante(0) {
}

// --------------------------------------------------
// Inicialização da grelha
// --------------------------------------------------

void Fabrica::inicializarGrelha(int novasLinhas, int novasColunas) {
    linhas = novasLinhas;
    colunas = novasColunas;

    grelha.assign(
        linhas,
        std::vector<Terreno>(colunas, Terreno())
    );
}

// --------------------------------------------------
// Criação aleatória da fábrica
// --------------------------------------------------

bool Fabrica::criar(int novasLinhas, int novasColunas) {
    // Impede a criação de uma segunda fábrica.
    if (linhas != 0 || colunas != 0) {
        return false;
    }

    if (novasLinhas <= 0 || novasColunas <= 0 ||
        novasLinhas > Settings::fabrica_max_linhas ||
        novasColunas > Settings::fabrica_max_colunas) {
        return false;
    }

    const int totalElementos =
        Settings::fabrica_num_crateras +
        Settings::fabrica_num_depositos_ferro +
        Settings::fabrica_num_depositos_silicio +
        Settings::fabrica_num_depositos_gelo;

    // Todos os elementos configurados têm de caber na grelha.
    if (novasLinhas * novasColunas < totalElementos) {
        return false;
    }

    inicializarGrelha(novasLinhas, novasColunas);
    colocarElementosAleatorios();

    return true;
}

void Fabrica::colocarElementosAleatorios() {
    std::random_device rd;
    std::mt19937 gerador(rd());

    std::vector<int> posicoes;

    for (int i = 0; i < linhas * colunas; ++i) {
        posicoes.push_back(i);
    }

    std::shuffle(posicoes.begin(), posicoes.end(), gerador);

    std::uniform_int_distribution<int> distQuantidade(
        Settings::deposito_quantidade_min,
        Settings::deposito_quantidade_max
    );

    int proximaPosicao = 0;

    auto colocar = [&](TipoTerreno tipo, int numero) {
        for (int i = 0; i < numero; ++i) {
            const int indice = posicoes[proximaPosicao];
            ++proximaPosicao;

            const int linha = indice / colunas;
            const int coluna = indice % colunas;

            if (tipo == TipoTerreno::Cratera) {
                grelha[linha][coluna] = Terreno(tipo);
            } else {
                grelha[linha][coluna].setDeposito(
                    tipo,
                    distQuantidade(gerador)
                );
            }
        }
    };

    colocar(
        TipoTerreno::Cratera,
        Settings::fabrica_num_crateras
    );

    colocar(
        TipoTerreno::DepositoFerro,
        Settings::fabrica_num_depositos_ferro
    );

    colocar(
        TipoTerreno::DepositoSilicio,
        Settings::fabrica_num_depositos_silicio
    );

    colocar(
        TipoTerreno::DepositoGelo,
        Settings::fabrica_num_depositos_gelo
    );
}

// --------------------------------------------------
// Carregamento da fábrica a partir de um ficheiro
// --------------------------------------------------

bool Fabrica::carregar(const std::string &nomeFicheiro) {
    if (linhas != 0 || colunas != 0) {
        return false;
    }

    std::ifstream ficheiro(nomeFicheiro);

    if (!ficheiro.is_open()) {
        return false;
    }

    std::string linhaTexto;

    if (!std::getline(ficheiro, linhaTexto)) {
        return false;
    }

    std::istringstream cabecalho(linhaTexto);

    int novasLinhas = 0;
    int novasColunas = 0;
    std::string excesso;

    if (!(cabecalho >> novasLinhas >> novasColunas)) {
        return false;
    }

    // A primeira linha só pode conter as duas dimensões.
    if (cabecalho >> excesso) {
        return false;
    }

    if (novasLinhas <= 0 || novasColunas <= 0 ||
        novasLinhas > Settings::fabrica_max_linhas ||
        novasColunas > Settings::fabrica_max_colunas) {
        return false;
    }

    std::vector<std::string> mapa;

    for (int l = 0; l < novasLinhas; ++l) {
        if (!std::getline(ficheiro, linhaTexto)) {
            return false;
        }

        // Aceita também ficheiros com terminações de linha CRLF.
        if (!linhaTexto.empty() && linhaTexto.back() == '\r') {
            linhaTexto.pop_back();
        }

        if (static_cast<int>(linhaTexto.size()) != novasColunas) {
            return false;
        }

        for (char simbolo : linhaTexto) {
            if (simbolo != '.' &&
                simbolo != '*' &&
                simbolo != 'f' &&
                simbolo != 's' &&
                simbolo != 'g') {
                return false;
            }
        }

        mapa.push_back(linhaTexto);
    }

    // Depois do mapa, só são aceites linhas em branco.
    while (std::getline(ficheiro, linhaTexto)) {
        if (linhaTexto.find_first_not_of(" \t\r") !=
            std::string::npos) {
            return false;
        }
    }

    if (ficheiro.bad()) {
        return false;
    }

    // A fábrica só é alterada depois de validar todo o ficheiro.
    inicializarGrelha(novasLinhas, novasColunas);

    std::random_device rd;
    std::mt19937 gerador(rd());

    std::uniform_int_distribution<int> distQuantidade(
        Settings::deposito_quantidade_min,
        Settings::deposito_quantidade_max
    );

    for (int l = 0; l < linhas; ++l) {
        for (int c = 0; c < colunas; ++c) {
            const char simbolo = mapa[l][c];

            switch (simbolo) {
                case '.':
                    grelha[l][c] = Terreno(TipoTerreno::Solo);
                    break;

                case '*':
                    grelha[l][c] = Terreno(TipoTerreno::Cratera);
                    break;

                case 'f':
                    grelha[l][c].setDeposito(
                        TipoTerreno::DepositoFerro,
                        distQuantidade(gerador)
                    );
                    break;

                case 's':
                    grelha[l][c].setDeposito(
                        TipoTerreno::DepositoSilicio,
                        distQuantidade(gerador)
                    );
                    break;

                case 'g':
                    grelha[l][c].setDeposito(
                        TipoTerreno::DepositoGelo,
                        distQuantidade(gerador)
                    );
                    break;
            }
        }
    }

    return true;
}

// --------------------------------------------------
// Métodos de consulta
// --------------------------------------------------

int Fabrica::getLinhas() const {
    return linhas;
}

int Fabrica::getColunas() const {
    return colunas;
}

int Fabrica::getCreditos() const {
    return creditos;
}

int Fabrica::getInstante() const {
    return instante;
}

bool Fabrica::posicaoValida(int linha, int coluna) const {
    return linha >= 0 &&
           linha < linhas &&
           coluna >= 0 &&
           coluna < colunas;
}

const Terreno &Fabrica::getTerreno(int linha, int coluna) const {
    return grelha.at(linha).at(coluna);
}

// --------------------------------------------------
// Apresentação da fábrica e do painel de estado
// --------------------------------------------------

void Fabrica::mostra(Ecra &ecra) const {
    ecra.limpa();

    if (linhas == 0 || colunas == 0) {
        ecra.moveCursorPara(0, 0);
        ecra.escreveString("A fabrica ainda nao foi criada.");
        ecra.mostra();
        return;
    }

    // Moldura com espaço para as réguas e para o terreno.
    ecra.desenhaMoldura(
        0,
        0,
        linhas + 3,
        colunas + 3
    );

    // Régua das colunas.
    for (int c = 0; c < colunas; ++c) {
        ecra.moveCursorPara(1, c + 2);
        ecra.escreveCaracter(static_cast<char>('A' + c));
    }

    // Régua das linhas e representação dos terrenos.
    for (int l = 0; l < linhas; ++l) {
        ecra.moveCursorPara(l + 2, 1);
        ecra.escreveCaracter(static_cast<char>('A' + l));

        for (int c = 0; c < colunas; ++c) {
            ecra.moveCursorPara(l + 2, c + 2);
            ecra.escreveCaracter(grelha[l][c].getSimbolo());
        }
    }

    const int colunaPainel = colunas + 5;

    ecra.desenhaMoldura(
        0,
        colunaPainel,
        9,
        34
    );

    ecra.moveCursorPara(1, colunaPainel + 2);
    ecra.escreveString(
        "Instante: " + std::to_string(instante)
    );

    ecra.moveCursorPara(2, colunaPainel + 2);
    ecra.escreveString(
        "Creditos: " + std::to_string(creditos)
    );

    ecra.moveCursorPara(3, colunaPainel + 2);
    ecra.escreveString("Maquinas: 0");

    ecra.moveCursorPara(4, colunaPainel + 2);
    ecra.escreveString("Engenheiros: 0");

    ecra.moveCursorPara(6, colunaPainel + 2);
    ecra.escreveString("Sem atividade de maquinas.");

    ecra.mostra();
}