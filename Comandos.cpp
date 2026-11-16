#include "Comandos.h"

#include "Ecra.h"
#include "Fabrica.h"
#include "Interpretador.h"
#include "Settings.h"
#include "Terreno.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>


namespace {

bool converterInteiro(const std::string &texto, int &valor) {
    try {
        std::size_t fim = 0;
        const int convertido = std::stoi(texto, &fim);

        if (fim != texto.size()) {
            return false;
        }

        valor = convertido;
        return true;
    } catch (const std::invalid_argument &) {
        return false;
    } catch (const std::out_of_range &) {
        return false;
    }
}

bool fabricaCriada(const Fabrica &fabrica) {
    return fabrica.getLinhas() > 0 &&
           fabrica.getColunas() > 0;
}

    void mostrarTerreno(
        const Fabrica &fabrica,
        int linha,
        int coluna
    ) {
    const Terreno &terreno = fabrica.getTerreno(linha, coluna);

    std::cout
        << "Posicao: "
        << static_cast<char>('a' + linha)
        << static_cast<char>('a' + coluna)
        << '\n';

    if (terreno.eDeposito()) {
        std::cout
            << "Terreno: deposito de "
            << terreno.getNomeMateriaPrima()
            << "\nQuantidade: "
            << terreno.getQuantidade()
            << '\n';
    } else if (terreno.getTipo() == TipoTerreno::Cratera) {
        std::cout << "Terreno: cratera\n";
    } else {
        std::cout << "Terreno: solo\n";
    }

    const Maquina *maquina = fabrica.getMaquina(linha, coluna);

    if (maquina != nullptr) {
        std::cout
            << "Maquina:\n"
            << maquina->getDescricao();
    } else {
        std::cout << "Maquina: nenhuma\n";
    }

    std::cout << '\n';
}

}

// --------------------------------------------------
// fabrica
// --------------------------------------------------

std::string ComandoFabrica::getNome() const {
    return "fabrica";
}

std::string ComandoFabrica::getAjuda() const {
    return
        "Sintaxe: fabrica <linhas> <colunas>\n"
        "Cria uma fabrica com terrenos distribuidos aleatoriamente.\n"
        "Os parametros devem ser inteiros positivos, dentro dos\n"
        "limites de Settings, e permitir colocar todos os terrenos.\n"
        "Falha se ja existir uma fabrica ou se as dimensoes forem invalidas.\n"
        "Exemplo: fabrica 12 20\n";
}

ResultadoComando ComandoFabrica::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (argumentos.size() != 2) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    int linhas = 0;
    int colunas = 0;

    if (!converterInteiro(argumentos[0], linhas) ||
        !converterInteiro(argumentos[1], colunas)) {
        std::cout << "As dimensoes devem ser numeros inteiros.\n";
        return ResultadoComando::Erro;
    }

    if (fabricaCriada(fabrica)) {
        std::cout << "A fabrica ja foi criada.\n";
        return ResultadoComando::Erro;
    }

    if (linhas <= 0 || colunas <= 0 ||
        linhas > Settings::fabrica_max_linhas ||
        colunas > Settings::fabrica_max_colunas) {
        std::cout
            << "Dimensoes invalidas. Maximo: "
            << Settings::fabrica_max_linhas
            << " linhas e "
            << Settings::fabrica_max_colunas
            << " colunas.\n";

        return ResultadoComando::Erro;
    }

    if (!fabrica.criar(linhas, colunas)) {
        std::cout
            << "Nao foi possivel criar a fabrica. "
            << "Verifica se existe espaco para todos os terrenos.\n";

        return ResultadoComando::Erro;
    }

    fabrica.mostra(ecra);
    return ResultadoComando::Sucesso;
}

// --------------------------------------------------
// carrega
// --------------------------------------------------

std::string ComandoCarrega::getNome() const {
    return "carrega";
}

std::string ComandoCarrega::getAjuda() const {
    return
        "Sintaxe: carrega <ficheiro>\n"
        "Cria uma fabrica a partir de um mapa de texto.\n"
        "O parametro indica o nome ou caminho do ficheiro.\n"
        "Nesta versao, o caminho nao pode conter espacos.\n"
        "Falha se ja existir uma fabrica, se o ficheiro nao abrir\n"
        "ou se o mapa nao respeitar o formato esperado.\n"
        "Exemplo: carrega mapa1.txt\n";
}

ResultadoComando ComandoCarrega::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (argumentos.size() != 1) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    if (fabricaCriada(fabrica)) {
        std::cout << "A fabrica ja foi criada.\n";
        return ResultadoComando::Erro;
    }

    if (!fabrica.carregar(argumentos[0])) {
        std::cout
            << "Nao foi possivel carregar o mapa. "
            << "Verifica o ficheiro e o seu formato.\n";

        return ResultadoComando::Erro;
    }

    fabrica.mostra(ecra);
    return ResultadoComando::Sucesso;
}

// --------------------------------------------------
// lposicao
// --------------------------------------------------

std::string ComandoLPosicao::getNome() const {
    return "lposicao";
}

std::string ComandoLPosicao::getAjuda() const {
    return
        "Sintaxe: lposicao <lc> [raio]\n"
        "Mostra o terreno, a quantidade dos depositos\n"
        "e a descricao da maquina que existir na posicao.\n"
        "lc sao duas letras minusculas: linha e coluna.\n"
        "O raio e um inteiro nao negativo; por omissao e zero.\n"
        "Com raio, consulta o quadrado centrado na posicao,\n"
        "limitado as posicoes existentes na fabrica.\n"
        "Os Silos e os transportadores apresentam o seu conteudo.\n"
        "As restantes maquinas ainda apresentam informacao parcial.\n"
        "Falha se nao existir fabrica ou se os parametros forem invalidos.\n"
        "Exemplo: lposicao ce 2\n";
}

ResultadoComando ComandoLPosicao::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (argumentos.size() != 1 && argumentos.size() != 2) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    const std::string &posicao = argumentos[0];

    if (posicao.size() != 2 ||
        posicao[0] < 'a' || posicao[0] > 'z' ||
        posicao[1] < 'a' || posicao[1] > 'z') {
        std::cout << "A posicao deve ter duas letras minusculas.\n";
        return ResultadoComando::Erro;
    }

    int raio = 0;

    if (argumentos.size() == 2) {
        if (!converterInteiro(argumentos[1], raio) || raio < 0) {
            std::cout << "O raio deve ser um inteiro nao negativo.\n";
            return ResultadoComando::Erro;
        }
    }

    if (!fabricaCriada(fabrica)) {
        std::cout << "Cria ou carrega primeiro a fabrica.\n";
        return ResultadoComando::Erro;
    }

    const int linha = posicao[0] - 'a';
    const int coluna = posicao[1] - 'a';

    if (!fabrica.posicaoValida(linha, coluna)) {
        std::cout << "Posicao fora da fabrica.\n";
        return ResultadoComando::Erro;
    }

    // Um raio superior a este limite ja cobre toda a fabrica.
    const int raioUtil = std::min(
        raio,
        std::max(fabrica.getLinhas(), fabrica.getColunas())
    );

    const int primeiraLinha = std::max(0, linha - raioUtil);
    const int ultimaLinha = std::min(
        fabrica.getLinhas() - 1,
        linha + raioUtil
    );

    const int primeiraColuna = std::max(0, coluna - raioUtil);
    const int ultimaColuna = std::min(
        fabrica.getColunas() - 1,
        coluna + raioUtil
    );

    for (int l = primeiraLinha; l <= ultimaLinha; ++l) {
        for (int c = primeiraColuna; c <= ultimaColuna; ++c) {
            mostrarTerreno(fabrica, l, c);
        }
    }

    return ResultadoComando::Sucesso;
}

// --------------------------------------------------
// fim
// --------------------------------------------------

std::string ComandoFim::getNome() const {
    return "fim";
}

std::string ComandoFim::getAjuda() const {
    return
        "Sintaxe: fim\n"
        "Termina o programa. Pode ser usado antes de criar a fabrica.\n"
        "Nao recebe parametros e falha se forem indicados argumentos.\n"
        "Exemplo: fim\n";
}

ResultadoComando ComandoFim::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &,
    Ecra &
) {
    if (!argumentos.empty()) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    std::cout << "Programa terminado.\n";
    return ResultadoComando::Terminar;
}

ComandoAjuda::ComandoAjuda(const Interpretador &interpretador)
    : interpretador(interpretador) {
}

std::string ComandoAjuda::getNome() const {
    return "ajuda";
}

std::string ComandoAjuda::getAjuda() const {
    return
        "Sintaxe: ajuda [comando]\n"
        "Sem argumentos, apresenta os comandos registados\n"
        "e a sintaxe resumida de cada um.\n"
        "Com um nome, apresenta a ajuda detalhada desse comando.\n"
        "Pode ser usado antes de criar a fabrica.\n"
        "Falha se receber mais de um argumento\n"
        "ou se o comando indicado nao existir.\n"
        "Exemplo: ajuda lposicao\n";
}

ResultadoComando ComandoAjuda::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &,
    Ecra &
) {
    if (argumentos.size() > 1) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    const std::string nome =
        argumentos.empty() ? "" : argumentos[0];

    if (!interpretador.mostraAjuda(nome)) {
        return ResultadoComando::Erro;
    }

    return ResultadoComando::Sucesso;
}

ComandoExecuta::ComandoExecuta(Interpretador &interpretador)
    : interpretador(interpretador) {
}

std::string ComandoExecuta::getNome() const {
    return "executa";
}

std::string ComandoExecuta::getAjuda() const {
    return
        "Sintaxe: executa <ficheiro>\n"
        "Le e executa comandos de um ficheiro de texto,\n"
        "com um comando por linha.\n"
        "O parametro indica o nome ou caminho do ficheiro.\n"
        "Nesta versao, o caminho nao pode conter espacos.\n"
        "Usa o mesmo processamento dos comandos do teclado.\n"
        "Pode ser usado antes de criar a fabrica.\n"
        "Um comando invalido e assinalado e a leitura continua.\n"
        "Um fim valido termina o programa.\n"
        "Falha se os argumentos forem invalidos, se o ficheiro\n"
        "nao puder ser lido ou se um comando falhar.\n"
        "Existe um limite de 32 ficheiros em execucao simultanea.\n"
        "Exemplo: executa teste1.txt\n";
}

ResultadoComando ComandoExecuta::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (argumentos.size() != 1) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    return interpretador.executaFicheiro(
        argumentos[0],
        fabrica,
        ecra
    );
}

std::string ComandoCompra::getNome() const {
    return "compra";
}

std::string ComandoCompra::getAjuda() const {
    return
        "Sintaxe: compra <tipo> <lc> <dir>\n"
        "Compra uma maquina e coloca-a na posicao indicada.\n"
        "Tipos: p perfuradora, t transportador, r refinaria,\n"
        "o oficina, s silo e l lancador.\n"
        "lc sao duas letras minusculas: linha e coluna.\n"
        "Direcoes: c cima, b baixo, e esquerda e d direita.\n"
        "Falha se nao existir fabrica, se os parametros forem\n"
        "invalidos, se a posicao for uma cratera ou estiver\n"
        "ocupada, ou se nao houver creditos suficientes.\n"
        "As maquinas sao compradas vazias.\n"
        "Nesta etapa, o Silo e o transportador recebem e entregam itens.\n"
        "A producao, a transformacao e os lancamentos ainda nao\n"
        "estao implementados.\n"
        "Exemplo: compra s aa d\n";
}

ResultadoComando ComandoCompra::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (argumentos.size() != 3) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    if (argumentos[0].size() != 1 ||
        std::string("ptrosl").find(argumentos[0][0]) ==
            std::string::npos) {
        std::cout << "Tipo invalido. Usa p, t, r, o, s ou l.\n";
        return ResultadoComando::Erro;
    }

    const std::string &posicao = argumentos[1];

    if (posicao.size() != 2 ||
        posicao[0] < 'a' || posicao[0] > 'z' ||
        posicao[1] < 'a' || posicao[1] > 'z') {
        std::cout << "A posicao deve ter duas letras minusculas.\n";
        return ResultadoComando::Erro;
    }

    if (argumentos[2].size() != 1 ||
        std::string("cbed").find(argumentos[2][0]) ==
            std::string::npos) {
        std::cout << "Direcao invalida. Usa c, b, e ou d.\n";
        return ResultadoComando::Erro;
    }

    const char tipo = argumentos[0][0];
    const int linha = posicao[0] - 'a';
    const int coluna = posicao[1] - 'a';
    const char direcao = argumentos[2][0];

    std::string erro;

    if (!fabrica.comprar(tipo, linha, coluna, direcao, erro)) {
        std::cout << erro << '\n';
        return ResultadoComando::Erro;
    }

    std::cout << "Maquina comprada em " << posicao << ".\n";

    fabrica.mostra(ecra);

    return ResultadoComando::Sucesso;
}

namespace {

bool interpretarPosicao(
    const std::string &texto,
    int &linha,
    int &coluna
) {
    if (texto.size() != 2 ||
        texto[0] < 'a' || texto[0] > 'z' ||
        texto[1] < 'a' || texto[1] > 'z') {
        return false;
    }

    linha = texto[0] - 'a';
    coluna = texto[1] - 'a';

    return true;
}

}

// --------------------------------------------------
// vende
// --------------------------------------------------

std::string ComandoVende::getNome() const {
    return "vende";
}

std::string ComandoVende::getAjuda() const {
    return
        "Sintaxe: vende <lc>\n"
        "Vende a maquina da posicao indicada.\n"
        "lc sao duas letras minusculas: linha e coluna.\n"
        "Devolve a percentagem do custo definida em Settings.\n"
        "As fracoes de credito sao descartadas.\n"
        "Falha se nao existir fabrica, se a posicao for invalida\n"
        "ou se nao existir uma maquina nessa posicao.\n"
        "Exemplo: vende aa\n";
}

ResultadoComando ComandoVende::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (argumentos.size() != 1) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    int linha = 0;
    int coluna = 0;

    if (!interpretarPosicao(argumentos[0], linha, coluna)) {
        std::cout << "A posicao deve ter duas letras minusculas.\n";
        return ResultadoComando::Erro;
    }

    const int creditosAntes = fabrica.getCreditos();

    std::string erro;

    if (!fabrica.vender(linha, coluna, erro)) {
        std::cout << erro << '\n';
        return ResultadoComando::Erro;
    }

    const int recebido = fabrica.getCreditos() - creditosAntes;

    std::cout
        << "Maquina vendida em "
        << argumentos[0]
        << ". Recebeste "
        << recebido
        << " creditos.\n";

    fabrica.mostra(ecra);

    return ResultadoComando::Sucesso;
}

// --------------------------------------------------
// roda
// --------------------------------------------------

std::string ComandoRoda::getNome() const {
    return "roda";
}

std::string ComandoRoda::getAjuda() const {
    return
        "Sintaxe: roda <lc> <dir>\n"
        "Altera a direcao de saida da maquina indicada.\n"
        "lc sao duas letras minusculas: linha e coluna.\n"
        "Direcoes: c cima, b baixo, e esquerda e d direita.\n"
        "Nao altera a posicao nem desconta creditos.\n"
        "Falha se nao existir fabrica, se os parametros forem\n"
        "invalidos ou se nao existir uma maquina nessa posicao.\n"
        "Exemplo: roda ab b\n";
}

ResultadoComando ComandoRoda::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (argumentos.size() != 2) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    int linha = 0;
    int coluna = 0;

    if (!interpretarPosicao(argumentos[0], linha, coluna)) {
        std::cout << "A posicao deve ter duas letras minusculas.\n";
        return ResultadoComando::Erro;
    }

    if (argumentos[1].size() != 1 ||
        std::string("cbed").find(argumentos[1][0]) ==
            std::string::npos) {
        std::cout << "Direcao invalida. Usa c, b, e ou d.\n";
        return ResultadoComando::Erro;
    }

    const char direcao = argumentos[1][0];

    std::string erro;

    if (!fabrica.rodar(linha, coluna, direcao, erro)) {
        std::cout << erro << '\n';
        return ResultadoComando::Erro;
    }

    std::cout
        << "Direcao da maquina em "
        << argumentos[0]
        << " alterada para "
        << direcao
        << ".\n";

    fabrica.mostra(ecra);

    return ResultadoComando::Sucesso;
}

// --------------------------------------------------
// estado
// --------------------------------------------------

std::string ComandoEstado::getNome() const {
    return "estado";
}

std::string ComandoEstado::getAjuda() const {
    return
        "Sintaxe: estado\n"
        "Apresenta as dimensoes, o instante, os creditos\n"
        "e as maquinas existentes na fabrica.\n"
        "Inclui a posicao, a direcao e a descricao de cada maquina.\n"
        "Mostra os itens dos Silos e dos transportadores\n"
        "e o resumo do ultimo instante.\n"
        "As restantes descricoes ainda sao parciais.\n"
        "Os engenheiros e os envios para a Terra ainda nao\n"
        "estao implementados.\n"
        "Nao recebe parametros.\n"
        "Falha se receber argumentos ou se nao existir fabrica.\n"
        "Exemplo: estado\n";
}

ResultadoComando ComandoEstado::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (!argumentos.empty()) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    if (!fabricaCriada(fabrica)) {
        std::cout << "Cria ou carrega primeiro a fabrica.\n";
        return ResultadoComando::Erro;
    }

    fabrica.mostraEstado();

    return ResultadoComando::Sucesso;
}

// --------------------------------------------------
// avanca
// --------------------------------------------------

std::string ComandoAvanca::getNome() const {
    return "avanca";
}

std::string ComandoAvanca::getAjuda() const {
    return
        "Sintaxe: avanca [n]\n"
        "Avanca o numero indicado de instantes, um de cada vez.\n"
        "n deve ser um inteiro positivo; por omissao vale 1.\n"
        "Em cada instante, as maquinas atuam e tentam entregar itens.\n"
        "O ecra e apresentado no fim de cada instante.\n"
        "Nesta etapa, estao implementadas as perdas do Silo\n"
        "e as transferencias entre Silos e transportadores.\n"
        "As restantes maquinas ainda nao produzem nem transformam itens.\n"
        "Falha se nao existir fabrica, se os argumentos forem invalidos\n"
        "ou se o contador de instantes ultrapassar o limite permitido.\n"
        "Exemplo: avanca 3\n";
}

ResultadoComando ComandoAvanca::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &ecra
) {
    if (argumentos.size() > 1) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    int numeroInstantes = 1;

    if (!argumentos.empty()) {
        if (!converterInteiro(argumentos[0], numeroInstantes) ||
            numeroInstantes <= 0) {
            std::cout
                << "O numero de instantes deve ser "
                << "um inteiro positivo.\n";

            return ResultadoComando::Erro;
            }
    }

    std::string erro;

    if (!fabrica.avancar(numeroInstantes, ecra, erro)) {
        std::cout << erro << '\n';
        return ResultadoComando::Erro;
    }

    return ResultadoComando::Sucesso;
}

namespace {

    bool exigirFabrica(const Fabrica &fabrica) {
        if (!fabricaCriada(fabrica)) {
            std::cout << "Cria ou carrega primeiro a fabrica.\n";
            return false;
        }

        return true;
    }

    ResultadoComando informarAcaoPendente(const std::string &nome) {
        std::cout
            << "Parametros validos para "
            << nome
            << ", mas a acao ainda nao esta implementada.\n"
            << "O estado da fabrica nao foi alterado.\n";

        return ResultadoComando::Erro;
    }

}

// --------------------------------------------------
// melhora
// --------------------------------------------------

std::string ComandoMelhora::getNome() const {
    return "melhora";
}

std::string ComandoMelhora::getAjuda() const {
    return
        "Sintaxe: melhora <lc>\n"
        "lc sao duas letras minusculas: linha e coluna.\n"
        "Acao prevista: melhorar a maquina dessa posicao,\n"
        "pagando o custo definido para o seu tipo em Settings.\n"
        "A melhoria depende do tipo de maquina e dos seus limites.\n"
        "Sem melhoria disponivel, nao existe alteracao.\n"
        "Falha se a posicao for invalida, nao existir maquina\n"
        "ou nao houver creditos para uma melhoria disponivel.\n"
        "Nesta etapa, apenas valida os parametros e a posicao;\n"
        "nao aplica melhorias nem desconta creditos.\n"
        "Exemplo: melhora aa\n";
}

ResultadoComando ComandoMelhora::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (argumentos.size() != 1) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    int linha = 0;
    int coluna = 0;

    if (!interpretarPosicao(argumentos[0], linha, coluna)) {
        std::cout << "A posicao deve ter duas letras minusculas.\n";
        return ResultadoComando::Erro;
    }

    if (!exigirFabrica(fabrica)) {
        return ResultadoComando::Erro;
    }

    if (!fabrica.posicaoValida(linha, coluna)) {
        std::cout << "Posicao fora da fabrica.\n";
        return ResultadoComando::Erro;
    }

    if (fabrica.getMaquina(linha, coluna) == nullptr) {
        std::cout << "Nao existe nenhuma maquina nessa posicao.\n";
        return ResultadoComando::Erro;
    }

    return informarAcaoPendente(getNome());
}

// --------------------------------------------------
// contrata
// --------------------------------------------------

std::string ComandoContrata::getNome() const {
    return "contrata";
}

std::string ComandoContrata::getAjuda() const {
    return
        "Sintaxe: contrata <nome>\n"
        "O nome identifica o novo engenheiro e pode ser repetido.\n"
        "Nesta versao, deve ser indicado numa unica palavra.\n"
        "Acao prevista: contratar um engenheiro livre,\n"
        "com identificador automatico e unico.\n"
        "A contratacao e o salario usam os valores de Settings.\n"
        "Falha se nao existir fabrica ou nao houver creditos\n"
        "suficientes para a contratacao.\n"
        "Nesta etapa, apenas valida os parametros;\n"
        "nao cria engenheiros nem desconta creditos.\n"
        "Exemplo: contrata joaquim\n";
}

ResultadoComando ComandoContrata::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (argumentos.size() != 1 || argumentos[0].empty()) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    if (!exigirFabrica(fabrica)) {
        return ResultadoComando::Erro;
    }

    return informarAcaoPendente(getNome());
}

// --------------------------------------------------
// atribui
// --------------------------------------------------

std::string ComandoAtribui::getNome() const {
    return "atribui";
}

std::string ComandoAtribui::getAjuda() const {
    return
        "Sintaxe: atribui <id> <lc>\n"
        "id e um inteiro positivo que identifica um engenheiro.\n"
        "lc sao duas letras minusculas: linha e coluna.\n"
        "Acao prevista: associar o engenheiro a essa maquina,\n"
        "desfazendo uma eventual associacao anterior.\n"
        "Falha se nao existir fabrica, engenheiro ou maquina,\n"
        "ou se os parametros forem invalidos.\n"
        "Nesta etapa, valida o formato do id e a posicao da maquina;\n"
        "a existencia do engenheiro ainda nao e verificada\n"
        "e nenhuma associacao e alterada.\n"
        "Exemplo: atribui 1 aa\n";
}

ResultadoComando ComandoAtribui::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (argumentos.size() != 2) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    int id = 0;

    if (!converterInteiro(argumentos[0], id) || id <= 0) {
        std::cout << "O id deve ser um inteiro positivo.\n";
        return ResultadoComando::Erro;
    }

    int linha = 0;
    int coluna = 0;

    if (!interpretarPosicao(argumentos[1], linha, coluna)) {
        std::cout << "A posicao deve ter duas letras minusculas.\n";
        return ResultadoComando::Erro;
    }

    if (!exigirFabrica(fabrica)) {
        return ResultadoComando::Erro;
    }

    if (!fabrica.posicaoValida(linha, coluna)) {
        std::cout << "Posicao fora da fabrica.\n";
        return ResultadoComando::Erro;
    }

    if (fabrica.getMaquina(linha, coluna) == nullptr) {
        std::cout << "Nao existe nenhuma maquina nessa posicao.\n";
        return ResultadoComando::Erro;
    }

    return informarAcaoPendente(getNome());
}

// --------------------------------------------------
// despede
// --------------------------------------------------

std::string ComandoDespede::getNome() const {
    return "despede";
}

std::string ComandoDespede::getAjuda() const {
    return
        "Sintaxe: despede <id>\n"
        "id e um inteiro positivo que identifica um engenheiro.\n"
        "Acao prevista: retirar o engenheiro da fabrica,\n"
        "eliminar a sua associacao e deixar de pagar o seu salario.\n"
        "Falha se nao existir fabrica, se o id for invalido\n"
        "ou se nao existir um engenheiro com esse identificador.\n"
        "Nesta etapa, apenas valida o formato do id;\n"
        "nao verifica a existencia nem remove engenheiros.\n"
        "Exemplo: despede 1\n";
}

ResultadoComando ComandoDespede::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (argumentos.size() != 1) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    int id = 0;

    if (!converterInteiro(argumentos[0], id) || id <= 0) {
        std::cout << "O id deve ser um inteiro positivo.\n";
        return ResultadoComando::Erro;
    }

    if (!exigirFabrica(fabrica)) {
        return ResultadoComando::Erro;
    }

    return informarAcaoPendente(getNome());
}

// --------------------------------------------------
// grava
// --------------------------------------------------

std::string ComandoGrava::getNome() const {
    return "grava";
}

std::string ComandoGrava::getAjuda() const {
    return
        "Sintaxe: grava <nome>\n"
        "O nome identifica uma copia em memoria, nao um ficheiro.\n"
        "Nesta versao, deve ser indicado numa unica palavra.\n"
        "Acao prevista: guardar uma copia completa da fabrica,\n"
        "sem memorizar as associacoes entre engenheiros e maquinas.\n"
        "Uma copia anterior com o mesmo nome sera substituida.\n"
        "Falha se nao existir fabrica ou se faltar o nome.\n"
        "Nesta etapa, apenas valida os parametros;\n"
        "nenhuma copia e criada ou substituida.\n"
        "Exemplo: grava antes\n";
}

ResultadoComando ComandoGrava::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (argumentos.size() != 1 || argumentos[0].empty()) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    if (!exigirFabrica(fabrica)) {
        return ResultadoComando::Erro;
    }

    return informarAcaoPendente(getNome());
}

// --------------------------------------------------
// recupera
// --------------------------------------------------

std::string ComandoRecupera::getNome() const {
    return "recupera";
}

std::string ComandoRecupera::getAjuda() const {
    return
        "Sintaxe: recupera <nome>\n"
        "O nome identifica uma copia guardada em memoria.\n"
        "Nesta versao, deve ser indicado numa unica palavra.\n"
        "Acao prevista: repor a fabrica a partir dessa copia.\n"
        "Os engenheiros sao recuperados livres.\n"
        "A copia guardada nao e eliminada.\n"
        "Falha se nao existir fabrica, se faltar o nome\n"
        "ou se nao existir uma copia com esse nome.\n"
        "Nesta etapa, apenas valida os parametros;\n"
        "nao procura copias nem altera a fabrica.\n"
        "Exemplo: recupera antes\n";
}

ResultadoComando ComandoRecupera::executa(
    const std::vector<std::string> &argumentos,
    Fabrica &fabrica,
    Ecra &
) {
    if (argumentos.size() != 1 || argumentos[0].empty()) {
        std::cout << getAjuda();
        return ResultadoComando::Erro;
    }

    if (!exigirFabrica(fabrica)) {
        return ResultadoComando::Erro;
    }

    return informarAcaoPendente(getNome());
}