#include "Maquinas.h"
#include "Settings.h"

#include <sstream>

// Perfuradora

Perfuradora::Perfuradora(int linha, int coluna, char direcao)
    : Maquina(linha, coluna, direcao, Settings::perfuradora_custo) {
}

char Perfuradora::getSimbolo() const {
    return 'P';
}

std::string Perfuradora::getNome() const {
    return "perfuradora";
}

// Transportador

// --------------------------------------------------
// Transportador
// --------------------------------------------------

Transportador::Transportador(
    int linha,
    int coluna,
    char direcao
)
    : Maquina(
        linha,
        coluna,
        direcao,
        Settings::transportador_custo
    ),
      item(nullptr),
      instanteRececao(-1) {
}

Transportador::~Transportador() {
    delete item;
}

char Transportador::getSimbolo() const {
    if (item != nullptr) {
        return item->getSimbolo();
    }

    switch (getDirecao()) {
        case 'c': return '^';
        case 'b': return 'v';
        case 'e': return '<';
        case 'd': return '>';
        default: return '?';
    }
}

std::string Transportador::getNome() const {
    return "transportador";
}

bool Transportador::aceita(const Item &) const {
    return item == nullptr;
}

bool Transportador::recebe(Item *novoItem, int instante) {
    if (novoItem == nullptr ||
        instante < 0 ||
        !aceita(*novoItem)) {
        return false;
    }

    item = novoItem;
    instanteRececao = instante;

    return true;
}

Item *Transportador::itemParaEntregar(int instante) const {
    if (item == nullptr || instante <= instanteRececao) {
        return nullptr;
    }

    return item;
}

Item *Transportador::retiraItem() {
    Item *retirado = item;

    item = nullptr;
    instanteRececao = -1;

    return retirado;
}

std::string Transportador::getDescricao() const {
    std::ostringstream texto;

    texto
        << "Tipo: " << getNome() << '\n'
        << "Posicao: "
        << static_cast<char>('a' + getLinha())
        << static_cast<char>('a' + getColuna())
        << '\n'
        << "Direcao: " << getDirecao() << '\n'
        << "Custo de compra: " << getCusto() << '\n';

    if (item == nullptr) {
        texto << "Conteudo: vazio\n";
    } else {
        texto
            << "Conteudo: " << item->getNome()
            << " [" << item->getSimbolo() << "]\n"
            << "Valor: " << item->getValor() << '\n'
            << "Instante de rececao: " << instanteRececao << '\n';
    }

    return texto.str();
}

// Refinaria

Refinaria::Refinaria(int linha, int coluna, char direcao)
    : Maquina(linha, coluna, direcao, Settings::refinaria_custo) {
}

char Refinaria::getSimbolo() const {
    return 'R';
}

std::string Refinaria::getNome() const {
    return "refinaria";
}

// Oficina

Oficina::Oficina(int linha, int coluna, char direcao)
    : Maquina(linha, coluna, direcao, Settings::oficina_custo) {
}

char Oficina::getSimbolo() const {
    return 'O';
}

std::string Oficina::getNome() const {
    return "oficina";
}

// --------------------------------------------------
// Silo
// --------------------------------------------------

Silo::Silo(int linha, int coluna, char direcao)
    : Maquina(linha, coluna, direcao, Settings::silo_custo),
      primeiro(nullptr),
      ultimo(nullptr),
      numeroItens(0),
      gerador(std::random_device{}()) {
}

Silo::~Silo() {
    while (primeiro != nullptr) {
        Item *item = retiraItem();
        delete item;
    }
}

char Silo::getSimbolo() const {
    return 'S';
}

std::string Silo::getNome() const {
    return "silo";
}

bool Silo::aceita(const Item &) const {
    return true;
}

bool Silo::recebe(Item *item, int instante) {
    if (item == nullptr || instante < 0) {
        return false;
    }

    No *novo = new No{item, instante, nullptr};

    if (ultimo == nullptr) {
        primeiro = novo;
    } else {
        ultimo->seguinte = novo;
    }

    ultimo = novo;
    ++numeroItens;

    return true;
}

Item *Silo::itemParaEntregar(int instante) const {
    if (primeiro == nullptr ||
        instante <= primeiro->instanteRececao) {
        return nullptr;
        }

    return primeiro->item;
}

Item *Silo::retiraItem() {
    if (primeiro == nullptr) {
        return nullptr;
    }

    No *remover = primeiro;
    Item *item = remover->item;

    primeiro = remover->seguinte;

    if (primeiro == nullptr) {
        ultimo = nullptr;
    }

    delete remover;
    --numeroItens;

    return item;
}



const Item *Silo::consultaPrimeiro() const {
    if (primeiro == nullptr) {
        return nullptr;
    }

    return primeiro->item;
}

std::size_t Silo::getNumeroItens() const {
    return numeroItens;
}

std::string Silo::getDescricao() const {
    std::ostringstream texto;

    texto
        << "Tipo: " << getNome() << '\n'
        << "Posicao: "
        << static_cast<char>('a' + getLinha())
        << static_cast<char>('a' + getColuna())
        << '\n'
        << "Direcao: " << getDirecao() << '\n'
        << "Custo de compra: " << getCusto() << '\n'
        << "Numero de itens: " << numeroItens << '\n';

    if (primeiro == nullptr) {
        texto << "Conteudo: vazio\n";
    } else {
        texto << "Conteudo, do mais antigo para o mais recente:\n";

        const No *atual = primeiro;

        while (atual != nullptr) {
            texto
        << "  " << atual->item->getNome()
            << " [" << atual->item->getSimbolo() << "]"
        << " - valor: " << atual->item->getValor()
        << " - recebido no instante: " << atual->instanteRececao
        << '\n';

            atual = atual->seguinte;
        }
    }


    texto
    << "Probabilidade de perda: "
    << Settings::silo_prob_perda
    << "% por instante.\n";

    return texto.str();
}

std::string Silo::atua(int instante) {
    if (primeiro == nullptr ||
        primeiro->instanteRececao >= instante) {
        return "";
        }

    std::uniform_int_distribution<int> sorteio(1, 100);

    if (sorteio(gerador) > Settings::silo_prob_perda) {
        return "";
    }

    Item *perdido = retiraItem();

    std::ostringstream mensagem;

    mensagem
        << "Perda de " << perdido->getNome()
        << " no Silo "
        << static_cast<char>('a' + getLinha())
        << static_cast<char>('a' + getColuna());

    delete perdido;

    return mensagem.str();
}

// Lancador

Lancador::Lancador(int linha, int coluna, char direcao)
    : Maquina(linha, coluna, direcao, Settings::lancador_custo) {
}

char Lancador::getSimbolo() const {
    return 'L';
}

std::string Lancador::getNome() const {
    return "lancador";
}