#include "Fabrica.h"
#include "Ecra.h"
#include "Settings.h"
#include "Maquinas.h"

#include <memory>
#include <utility>

#include <algorithm>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <limits>

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
                simbolo != '#' &&
                simbolo != '%' &&
                simbolo != '$' &&
                simbolo != '~') {
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

                case '#':
                    grelha[l][c] = Terreno(TipoTerreno::Cratera);
                    break;

                case '%':
                    grelha[l][c].setDeposito(
                        TipoTerreno::DepositoFerro,
                        distQuantidade(gerador)
                    );
                    break;

                case '$':
                    grelha[l][c].setDeposito(
                        TipoTerreno::DepositoSilicio,
                        distQuantidade(gerador)
                    );
                    break;

                case '~':
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

const Maquina *Fabrica::getMaquina(int linha, int coluna) const {
    for (const auto &maquina : maquinas) {
        if (maquina->getLinha() == linha &&
            maquina->getColuna() == coluna) {
            return maquina.get();
        }
    }

    return nullptr;
}

std::size_t Fabrica::getNumeroMaquinas() const {
    return maquinas.size();
}

bool Fabrica::comprar(
    char tipo,
    int linha,
    int coluna,
    char direcao,
    std::string &erro
) {
    erro.clear();

    if (linhas == 0 || colunas == 0) {
        erro = "Cria ou carrega primeiro a fabrica.";
        return false;
    }

    if (!posicaoValida(linha, coluna)) {
        erro = "Posicao fora da fabrica.";
        return false;
    }

    if (direcao != 'c' && direcao != 'b' &&
        direcao != 'e' && direcao != 'd') {
        erro = "Direcao invalida. Usa c, b, e ou d.";
        return false;
    }

    if (getTerreno(linha, coluna).getTipo() == TipoTerreno::Cratera) {
        erro = "Nao podes colocar uma maquina numa cratera.";
        return false;
    }

    if (getMaquina(linha, coluna) != nullptr) {
        erro = "Ja existe uma maquina nessa posicao.";
        return false;
    }

    std::unique_ptr<Maquina> nova;

    switch (tipo) {
        case 'p':
            nova = std::make_unique<Perfuradora>(
                linha, coluna, direcao
            );
            break;

        case 't':
            nova = std::make_unique<Transportador>(
                linha, coluna, direcao
            );
            break;

        case 'r':
            nova = std::make_unique<Refinaria>(
                linha, coluna, direcao
            );
            break;

        case 'o':
            nova = std::make_unique<Oficina>(
                linha, coluna, direcao
            );
            break;

        case 's':
            nova = std::make_unique<Silo>(
                linha, coluna, direcao
            );
            break;

        case 'l':
            nova = std::make_unique<Lancador>(
                linha, coluna, direcao
            );
            break;

        default:
            erro = "Tipo de maquina invalido.";
            return false;
    }

    const int custo = nova->getCusto();

    if (creditos < custo) {
        erro = "Creditos insuficientes.";
        return false;
    }

    maquinas.push_back(std::move(nova));
    creditos -= custo;

    return true;
}
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
            const Maquina *maquina = getMaquina(l, c);

            if (maquina != nullptr) {
                ecra.escreveCaracter(maquina->getSimbolo());
            } else {
                ecra.escreveCaracter(grelha[l][c].getSimbolo());
            }
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
    ecra.escreveString("Instante: " + std::to_string(instante));

    ecra.moveCursorPara(2, colunaPainel + 2);
    ecra.escreveString("Creditos: " + std::to_string(creditos));

    ecra.moveCursorPara(3, colunaPainel + 2);
    ecra.escreveString("Maquinas: " + std::to_string(getNumeroMaquinas()));

    ecra.moveCursorPara(4, colunaPainel + 2);
    ecra.escreveString("Engenheiros: 0");

    ecra.moveCursorPara(6, colunaPainel + 2);

    if (acontecimentos.empty()) {
        ecra.escreveString("Sem eventos no ultimo instante.");
    } else {
        ecra.escreveString(
            "Eventos: " + std::to_string(acontecimentos.size())
        );

        ecra.moveCursorPara(7, colunaPainel + 2);

        ecra.escreveString(
            acontecimentos.back().substr(0, 30)
        );
    }

    ecra.mostra();
}

bool Fabrica::vender(
    int linha,
    int coluna,
    std::string &erro
) {
    erro.clear();

    if (linhas == 0 || colunas == 0) {
        erro = "Cria ou carrega primeiro a fabrica.";
        return false;
    }

    if (!posicaoValida(linha, coluna)) {
        erro = "Posicao fora da fabrica.";
        return false;
    }

    for (auto it = maquinas.begin(); it != maquinas.end(); ++it) {
        if ((*it)->getLinha() == linha &&
            (*it)->getColuna() == coluna) {

            const int valorVenda =
                (*it)->getCusto() *
                Settings::maquina_percentagem_venda / 100;

            maquinas.erase(it);
            creditos += valorVenda;

            return true;
            }
    }

    erro = "Nao existe nenhuma maquina nessa posicao.";
    return false;
}

bool Fabrica::rodar(
    int linha,
    int coluna,
    char direcao,
    std::string &erro
) {
    erro.clear();

    if (linhas == 0 || colunas == 0) {
        erro = "Cria ou carrega primeiro a fabrica.";
        return false;
    }

    if (!posicaoValida(linha, coluna)) {
        erro = "Posicao fora da fabrica.";
        return false;
    }

    if (direcao != 'c' && direcao != 'b' &&
        direcao != 'e' && direcao != 'd') {
        erro = "Direcao invalida. Usa c, b, e ou d.";
        return false;
        }

    for (const auto &maquina : maquinas) {
        if (maquina->getLinha() == linha &&
            maquina->getColuna() == coluna) {

            return maquina->roda(direcao);
            }
    }

    erro = "Nao existe nenhuma maquina nessa posicao.";
    return false;
}

void Fabrica::mostraEstado() const {
    if (linhas == 0 || colunas == 0) {
        std::cout << "A fabrica ainda nao foi criada.\n";
        return;
    }

    std::cout
        << "Estado da fabrica\n"
        << "Dimensoes: " << linhas << " x " << colunas << '\n'
        << "Instante: " << instante << '\n'
        << "Creditos: " << creditos << '\n'
        << "Maquinas: " << maquinas.size() << "\n\n";

    if (maquinas.empty()) {
        std::cout << "Nao existem maquinas.\n\n";
    } else {
        for (const auto &maquina : maquinas) {
            std::cout << maquina->getDescricao() << '\n';
        }
    }

    std::cout
        << "Engenheiros: ainda nao implementados nesta etapa.\n"
        << "Envios para a Terra: ainda nao implementados nesta etapa.\n";

    std::cout << "\nResumo do ultimo instante:\n";

    if (acontecimentos.empty()) {
        std::cout << "Sem eventos registados.\n";
    } else {
        for (const std::string &mensagem : acontecimentos) {
            std::cout << "- " << mensagem << '\n';
        }
    }
}

Maquina *Fabrica::getMaquina(int linha, int coluna) {
    for (const auto &maquina : maquinas) {
        if (maquina->getLinha() == linha &&
            maquina->getColuna() == coluna) {
            return maquina.get();
        }
    }

    return nullptr;
}

bool Fabrica::avancar(
    int numeroInstantes,
    Ecra &ecra,
    std::string &erro
) {
    erro.clear();

    if (linhas == 0 || colunas == 0) {
        erro = "Cria ou carrega primeiro a fabrica.";
        return false;
    }

    if (numeroInstantes <= 0) {
        erro = "O numero de instantes deve ser positivo.";
        return false;
    }

    if (numeroInstantes >
        std::numeric_limits<int>::max() - instante) {
        erro = "O numero indicado ultrapassa o limite do contador.";
        return false;
    }

    for (int passo = 0; passo < numeroInstantes; ++passo) {
        ++instante;
        acontecimentos.clear();

        // Fase 1: cada maquina realiza a sua atuacao local.
        for (const auto &maquina : maquinas) {
            const std::string mensagem = maquina->atua(instante);

            if (!mensagem.empty()) {
                acontecimentos.push_back(mensagem);
            }
        }

        // Fase 2: cada maquina tenta uma entrega.
        for (const auto &maquina : maquinas) {
            int linhaDestino = maquina->getLinha();
            int colunaDestino = maquina->getColuna();

            switch (maquina->getDirecao()) {
                case 'c':
                    --linhaDestino;
                    break;

                case 'b':
                    ++linhaDestino;
                    break;

                case 'e':
                    --colunaDestino;
                    break;

                case 'd':
                    ++colunaDestino;
                    break;

                default:
                    continue;
            }

            if (!posicaoValida(linhaDestino, colunaDestino)) {
                continue;
            }

            Maquina *destino = getMaquina(
                linhaDestino,
                colunaDestino
            );

            if (destino == nullptr) {
                continue;
            }

            if (maquina->tentaEntregar(*destino, instante)) {
                std::string mensagem = "Transferencia de ";

                mensagem += static_cast<char>(
                    'a' + maquina->getLinha()
                );

                mensagem += static_cast<char>(
                    'a' + maquina->getColuna()
                );

                mensagem += " para ";

                mensagem += static_cast<char>(
                    'a' + linhaDestino
                );

                mensagem += static_cast<char>(
                    'a' + colunaDestino
                );

                acontecimentos.push_back(mensagem);
            }
        }

        mostra(ecra);
    }

    return true;
}