#include "Itens.h"
#include "Settings.h"

// --------------------------------------------------
// Item
// --------------------------------------------------

Item::Item(
    const std::string &nome,
    char simbolo,
    int valor
)
    : nome(nome),
      simbolo(simbolo),
      valor(valor) {
}

const std::string &Item::getNome() const {
    return nome;
}

char Item::getSimbolo() const {
    return simbolo;
}

int Item::getValor() const {
    return valor;
}

bool Item::eMateriaPrima() const {
    return false;
}

// --------------------------------------------------
// Grupos de itens
// --------------------------------------------------

MateriaPrima::MateriaPrima(
    const std::string &nome,
    char simbolo,
    int valor
)
    : Item(nome, simbolo, valor) {
}

bool MateriaPrima::eMateriaPrima() const {
    return true;
}

Produto::Produto(
    const std::string &nome,
    char simbolo,
    int valor
)
    : Item(nome, simbolo, valor) {
}

// --------------------------------------------------
// Materias-primas
// --------------------------------------------------

MinerioFerro::MinerioFerro()
    : MateriaPrima(
        "minerio de ferro",
        'f',
        Settings::valor_minerio_ferro
    ) {
}

SilicioBruto::SilicioBruto()
    : MateriaPrima(
        "silicio bruto",
        's',
        Settings::valor_silicio_bruto
    ) {
}

Gelo::Gelo()
    : MateriaPrima(
        "gelo",
        'g',
        Settings::valor_gelo
    ) {
}

// --------------------------------------------------
// Produtos
// --------------------------------------------------

PlacaFerro::PlacaFerro()
    : Produto(
        "placa de ferro",
        'p',
        Settings::valor_placa_ferro
    ) {
}

SilicioPuro::SilicioPuro()
    : Produto(
        "silicio puro",
        'i',
        Settings::valor_silicio_puro
    ) {
}

Agua::Agua()
    : Produto(
        "agua",
        'a',
        Settings::valor_agua
    ) {
}

KitHabitat::KitHabitat()
    : Produto(
        "kit de habitat",
        'k',
        Settings::valor_kit_habitat
    ) {
}