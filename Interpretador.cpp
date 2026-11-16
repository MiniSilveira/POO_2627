#include "Interpretador.h"
#include "Comandos.h"

#include <fstream>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

Interpretador::Interpretador() {
    comandos.push_back(std::make_unique<ComandoFabrica>());
    comandos.push_back(std::make_unique<ComandoCarrega>());
    comandos.push_back(std::make_unique<ComandoLPosicao>());
    comandos.push_back(std::make_unique<ComandoFim>());
    comandos.push_back(std::make_unique<ComandoCompra>());
    comandos.push_back(std::make_unique<ComandoAjuda>(*this));
    comandos.push_back(std::make_unique<ComandoExecuta>(*this));
    comandos.push_back(std::make_unique<ComandoVende>());
    comandos.push_back(std::make_unique<ComandoRoda>());
    comandos.push_back(std::make_unique<ComandoEstado>());
    comandos.push_back(std::make_unique<ComandoAvanca>());
    comandos.push_back(std::make_unique<ComandoMelhora>());
    comandos.push_back(std::make_unique<ComandoContrata>());
    comandos.push_back(std::make_unique<ComandoAtribui>());
    comandos.push_back(std::make_unique<ComandoDespede>());
    comandos.push_back(std::make_unique<ComandoGrava>());
    comandos.push_back(std::make_unique<ComandoRecupera>());
}

ResultadoComando Interpretador::processaLinha(
    const std::string &linha,
    Fabrica &fabrica,
    Ecra &ecra
) {
    std::istringstream entrada(linha);

    std::string nome;

    if (!(entrada >> nome)) {
        return ResultadoComando::Sucesso;
    }

    std::vector<std::string> argumentos;
    std::string argumento;

    while (entrada >> argumento) {
        argumentos.push_back(argumento);
    }

    for (const auto &comando : comandos) {
        if (comando->getNome() == nome) {
            return comando->executa(argumentos, fabrica, ecra);
        }
    }

    std::cout << "Comando desconhecido: " << nome << '\n';
    return ResultadoComando::Erro;
}

bool Interpretador::mostraAjuda(const std::string &nome) const {
    if (nome.empty()) {
        std::cout << "Comandos registados:\n";

        for (const auto &comando : comandos) {
            std::istringstream textoAjuda(comando->getAjuda());

            std::string sintaxe;
            std::getline(textoAjuda, sintaxe);

            std::cout << sintaxe << '\n';
        }

        return true;
    }

    for (const auto &comando : comandos) {
        if (comando->getNome() == nome) {
            std::cout << comando->getAjuda();
            return true;
        }
    }

    std::cout
        << "Nao existe ajuda para o comando: "
        << nome
        << '\n';

    return false;
}

ResultadoComando Interpretador::executaFicheiro(
    const std::string &nomeFicheiro,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (ficheirosEmExecucao >= MAX_FICHEIROS_EM_EXECUCAO) {
        std::cout
            << "Limite de ficheiros em execucao atingido. "
            << "Verifica se existem chamadas circulares de executa.\n";

        return ResultadoComando::Erro;
    }

    std::ifstream ficheiro(nomeFicheiro);

    if (!ficheiro.is_open()) {
        std::cout
            << "Nao foi possivel abrir o ficheiro: "
            << nomeFicheiro
            << '\n';

        return ResultadoComando::Erro;
    }

    struct ControloExecucao {
        int &contador;

        explicit ControloExecucao(int &contador)
            : contador(contador) {
            ++contador;
        }

        ~ControloExecucao() {
            --contador;
        }
    };

    ControloExecucao controlo(ficheirosEmExecucao);

    std::string linha;
    std::size_t numeroLinha = 0;
    bool houveErros = false;

    while (std::getline(ficheiro, linha)) {
        ++numeroLinha;

        if (!linha.empty() && linha.back() == '\r') {
            linha.pop_back();
        }

        // Ignora linhas vazias ou compostas apenas por espacos.
        if (linha.find_first_not_of(" \t\r") == std::string::npos) {
            continue;
        }

        std::cout
            << '[' << nomeFicheiro << ':' << numeroLinha << "] "
            << linha
            << '\n';

        const ResultadoComando resultado =
            processaLinha(linha, fabrica, ecra);

        if (resultado == ResultadoComando::Terminar) {
            return ResultadoComando::Terminar;
        }

        if (resultado == ResultadoComando::Erro) {
            houveErros = true;

            std::cout
                << "Erro no ficheiro "
                << nomeFicheiro
                << ", linha "
                << numeroLinha
                << ".\n";
        }
    }

    if (ficheiro.bad()) {
        std::cout
            << "Ocorreu um erro de leitura no ficheiro: "
            << nomeFicheiro
            << '\n';

        return ResultadoComando::Erro;
    }

    if (houveErros) {
        std::cout
            << "Execucao do ficheiro concluida com erros: "
            << nomeFicheiro
            << '\n';

        return ResultadoComando::Erro;
    }

    std::cout
        << "Execucao do ficheiro concluida: "
        << nomeFicheiro
        << '\n';

    return ResultadoComando::Sucesso;
}