#include "Fabrica.h"
#include "Itens.h"
#include "Maquinas.h"
#include "Settings.h"
#include <iostream>
#include <string>

namespace {
int falhas = 0;

void verificar(bool condicao, const std::string &descricao) {
    std::cout << (condicao ? "[OK] " : "[FALHOU] ") << descricao << '\n';
    if (!condicao) ++falhas;
}

bool inserir(Maquina &maquina, Item *item, int instante) {
    if (maquina.recebe(item, instante)) return true;
    delete item;
    return false;
}

class ItemTeste : public Item {
public:
    explicit ItemTeste(int &vivos)
        : Item("item de teste", 'x', 1), vivos(vivos) {
        ++vivos;
    }
    ~ItemTeste() override { --vivos; }
private:
    int &vivos;
};

void testarFila() {
    Silo silo(0, 0, 'd');
    verificar(inserir(silo, new MinerioFerro(), 0), "Recebe minerio de ferro");
    verificar(inserir(silo, new SilicioBruto(), 0), "Recebe silicio bruto");
    verificar(inserir(silo, new Gelo(), 0), "Recebe gelo");
    verificar(inserir(silo, new PlacaFerro(), 0), "Recebe placa de ferro");
    verificar(inserir(silo, new SilicioPuro(), 0), "Recebe silicio puro");
    verificar(inserir(silo, new Agua(), 0), "Recebe agua");
    verificar(inserir(silo, new KitHabitat(), 0), "Recebe kit de habitat");
    verificar(silo.getNumeroItens() == 7, "Guarda os sete tipos de item");
    std::string ordem;
    while (silo.getNumeroItens() > 0) {
        Item *item = silo.retiraItem();
        if (item == nullptr) {
            verificar(false, "Contador e fila coerentes");
            break;
        }
        ordem += item->getSimbolo();
        delete item;
    }
    verificar(ordem == "fsgpiak", "Saida FIFO dos sete itens");
    verificar(silo.retiraItem() == nullptr, "Retirada de uma fila vazia");
    verificar(inserir(silo, new Gelo(), 1), "Reutilizacao depois de esvaziar");
}

void testarTransferencias() {
    Silo origem(0, 0, 'd');
    Transportador meio(0, 1, 'd');
    Silo destino(0, 2, 'd');
    Silo distante(2, 2, 'd');
    verificar(inserir(origem, new MinerioFerro(), 0), "Prepara a origem");
    verificar(!origem.tentaEntregar(meio, 0), "Nao entrega no instante de rececao");
    verificar(!origem.tentaEntregar(distante, 1), "Rejeita destino nao vizinho");
    verificar(origem.getNumeroItens() == 1, "Preserva item apos entrega rejeitada");
    verificar(origem.tentaEntregar(meio, 1), "Entrega ao transportador no instante 1");
    verificar(meio.getSimbolo() == 'f', "Transportador mostra o item");
    verificar(!meio.tentaEntregar(destino, 1), "Impede segunda deslocacao no instante 1");
    verificar(meio.tentaEntregar(destino, 2), "Entrega ao destino no instante 2");
    verificar(destino.getNumeroItens() == 1, "Destino guarda o item");
    verificar(meio.getSimbolo() == '>', "Transportador vazio volta a mostrar a seta");

    Silo bloqueado(1, 0, 'd');
    Transportador cheio(1, 1, 'd');
    verificar(inserir(bloqueado, new Gelo(), 0), "Prepara origem bloqueada");
    verificar(inserir(cheio, new Agua(), 0), "Preenche o destino");
    verificar(!bloqueado.tentaEntregar(cheio, 1), "Destino cheio bloqueia entrega");
    verificar(bloqueado.getNumeroItens() == 1, "Bloqueio nao elimina o item");
}

void testarDestruicaoEVenda() {
    int vivos = 0;
    {
        Silo silo(0, 0, 'd');
        Transportador transportador(0, 1, 'd');
        inserir(silo, new ItemTeste(vivos), 0);
        inserir(silo, new ItemTeste(vivos), 0);
        inserir(transportador, new ItemTeste(vivos), 0);
        verificar(vivos == 3, "Objetos de teste foram criados");
    }
    verificar(vivos == 0, "Destrutores libertam os itens guardados");

    {
        Fabrica fabrica;
        std::string erro;
        if (!fabrica.criar(5, 5)) {
            verificar(false, "Criacao da fabrica para testar venda");
            return;
        }
        int linha = -1;
        int coluna = -1;
        for (int l = 0; l < fabrica.getLinhas() && linha == -1; ++l) {
            for (int c = 0; c < fabrica.getColunas(); ++c) {
                if (fabrica.getTerreno(l, c).getTipo() != TipoTerreno::Cratera) {
                    linha = l;
                    coluna = c;
                    break;
                }
            }
        }
        if (!fabrica.comprar('s', linha, coluna, 'd', erro)) {
            verificar(false, "Compra do Silo para testar venda");
            return;
        }
        Maquina *silo = fabrica.getMaquina(linha, coluna);
        verificar(inserir(*silo, new ItemTeste(vivos), 0), "Coloca item no Silo comprado");
        verificar(fabrica.vender(linha, coluna, erro), "Vende Silo com conteudo");
        verificar(vivos == 0, "Venda do Silo destroi o item");
        if (!fabrica.comprar('t', linha, coluna, 'd', erro)) {
            verificar(false, "Compra de transportador para testar destruicao");
            return;
        }
        Maquina *transportador = fabrica.getMaquina(linha, coluna);
        verificar(inserir(*transportador, new ItemTeste(vivos), 0), "Deixa item na fabrica ao terminar");
    }
    verificar(vivos == 0, "Destruicao da fabrica liberta maquina e item");
}

void testarPerdas() {
    int vivos = 0;
    int perdas = 0;
    bool coerente = true;
    for (int i = 0; i < 100; ++i) {
        {
            Silo silo(0, 0, 'd');
            if (!inserir(silo, new ItemTeste(vivos), 0)) {
                coerente = false;
                continue;
            }
            const std::string evento = silo.atua(1);
            const bool perdeu = !evento.empty();
            if (perdeu) ++perdas;
            if (silo.getNumeroItens() != (perdeu ? 0u : 1u) ||
                vivos != (perdeu ? 0 : 1)) {
                coerente = false;
            }
        }
        if (vivos != 0) coerente = false;
    }
    verificar(coerente, "Perdas, contador, mensagens e destruicao coerentes");
    if (Settings::silo_prob_perda == 0)
        verificar(perdas == 0, "Probabilidade zero nao perde itens");
    if (Settings::silo_prob_perda == 100)
        verificar(perdas == 100, "Probabilidade cem perde sempre o item elegivel");
    std::cout << "Perdas observadas: " << perdas << "/100; configuracao: "
              << Settings::silo_prob_perda << "% (nao se exige uma contagem exata com 20%).\n";
}
}

int main() {
    testarFila();
    testarTransferencias();
    testarDestruicaoEVenda();
    testarPerdas();
    std::cout << "\nFalhas detetadas: " << falhas << '\n';
    return falhas == 0 ? 0 : 1;
}
