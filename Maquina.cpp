#include "Maquina.h"
#include <sstream>
#include "Itens.h"

Maquina::Maquina(
    int linha,
    int coluna,
    char direcao,
    int custo
)
    : linha(linha),
      coluna(coluna),
      direcao(direcao),
      custo(custo) {
}

int Maquina::getLinha() const {
    return linha;
}

int Maquina::getColuna() const {
    return coluna;
}

char Maquina::getDirecao() const {
    return direcao;
}

int Maquina::getCusto() const {
    return custo;
}

bool Maquina::roda(char novaDirecao) {
    if (novaDirecao != 'c' &&
        novaDirecao != 'b' &&
        novaDirecao != 'e' &&
        novaDirecao != 'd') {
        return false;
        }

    direcao = novaDirecao;
    return true;
}

std::string Maquina::getDescricao() const {
    std::ostringstream texto;

    texto
        << "Tipo: " << getNome() << '\n'
        << "Posicao: "
        << static_cast<char>('a' + getLinha())
        << static_cast<char>('a' + getColuna())
        << '\n'
        << "Direcao: " << getDirecao() << '\n'
        << "Custo de compra: " << getCusto() << '\n'
        << "Conteudo: ainda nao implementado nesta etapa.\n";

    return texto.str();
}

bool Maquina::aceita(const Item &) const {
    return false;
}

bool Maquina::recebe(Item *, int) {
    return false;
}

Item *Maquina::itemParaEntregar(int) const {
    return nullptr;
}

Item *Maquina::retiraItem() {
    return nullptr;
}

bool Maquina::tentaEntregar(Maquina &destino, int instante) {
    if (&destino == this || instante < 0) {
        return false;
    }

    int linhaDestino = getLinha();
    int colunaDestino = getColuna();

    switch (getDirecao()) {
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
            return false;
    }

    if (destino.getLinha() != linhaDestino ||
        destino.getColuna() != colunaDestino) {
        return false;
        }

    Item *item = itemParaEntregar(instante);

    if (item == nullptr || !destino.aceita(*item)) {
        return false;
    }

    if (!destino.recebe(item, instante)) {
        return false;
    }

    retiraItem();

    return true;
}

std::string Maquina::atua(int) {
    return "";
}