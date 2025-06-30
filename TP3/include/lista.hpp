#ifndef LISTA_HPP
#define LISTA_HPP

#include "iostream"

using namespace std;

//Adaptação do algoritmo de lista encadeada aprendido em sala de aula 
//Foi escolhida uma implementação genérica da lista com templates, porém com alguns métodos adaptados pra lidar com a classe EventoEstatisticas

template <typename T> class TipoCelula; // Forward declaration

template <typename T>
class ListaEncadeada {
public:
    ListaEncadeada();                 // Construtor
    ~ListaEncadeada();                // Destrutor

    int GetTamanho();                 // Retorna número de elementos
    bool ListaVazia();               // Verifica se a lista está vazia
    T GetItem(int pos);              // Retorna o item na posição
    void InsereFinal(T item);        // Insere ao final
    void InsereInicio(T item);       // Insere no início
    void InserePosicao(T item, int pos); // Insere em uma posição específica
    T Pesquisa(T c);                 // Pesquisa item na lista
    void Limpa();                    // Remove todos os elementos
    void Imprime();                  // Imprime todos os eventos
    void InsereOrdenadoPorIdPacote(EventoEstatisticas &elemento); // Inserção ordenada por ID (para nodeEvento)

    TipoCelula<T>* primeiro;         // Ponteiro para célula cabeça
    TipoCelula<T>* ultimo;           // Ponteiro para último elemento

private:
    int tamanho;
    TipoCelula<T>* Posiciona(int pos, bool antes); // Localiza a célula anterior ou exata
};

// Classe célula: nó da lista
template <typename T>
class TipoCelula {
public:
    TipoCelula(T n);                 // Construtor com valor
    TipoCelula();                    // Construtor padrão
    T item;
    TipoCelula<T>* prox;
};

// -------------------- Implementações --------------------

template <typename T>
TipoCelula<T>::TipoCelula(T n) {
    item = n;
    prox = nullptr;
}

template <typename T>
TipoCelula<T>::TipoCelula() {
    prox = nullptr;
}

// Construtor da lista
template <typename T>
ListaEncadeada<T>::ListaEncadeada() {
    tamanho = 0;
    primeiro = new TipoCelula<T>();  // Célula cabeça
    ultimo = primeiro;
}

// Destrutor da lista
template <typename T>
ListaEncadeada<T>::~ListaEncadeada() {
    Limpa();
    delete primeiro;
}

template <typename T>
int ListaEncadeada<T>::GetTamanho() {
    return tamanho;
}

// Pesquisa 
template <typename T>
T ListaEncadeada<T>::Pesquisa(T c) {
    T aux;
    TipoCelula<T>* p = primeiro->prox;

    while (p != nullptr) {
        if (p->item == c) {
            aux = p->item;
            break;
        }
        p = p->prox;
    }

    return aux;
}

// Verifica se lista está vazia
template <typename T>
bool ListaEncadeada<T>::ListaVazia() {
    return tamanho == 0;
}

// Insere no final
template <typename T>
void ListaEncadeada<T>::InsereFinal(T item) {
    TipoCelula<T>* nova = new TipoCelula<T>(item);
    ultimo->prox = nova;
    ultimo = nova;
    tamanho++;
}

// Insere no início da lista
template <typename T>
void ListaEncadeada<T>::InsereInicio(T item) {
    TipoCelula<T>* nova = new TipoCelula<T>();
    nova->item = item;
    nova->prox = primeiro->prox;
    primeiro->prox = nova;
    tamanho++;

    if (nova->prox == nullptr) {
        ultimo = nova;
    }
}

// Insere em posição específica
template <typename T>
void ListaEncadeada<T>::InserePosicao(T item, int pos) {
    TipoCelula<T>* p = Posiciona(pos, true);
    TipoCelula<T>* nova = new TipoCelula<T>();
    nova->item = item;
    nova->prox = p->prox;
    p->prox = nova;
    tamanho++;

    if (nova->prox == nullptr) {
        ultimo = nova;
    }
}

// Remove todos os itens da lista
template <typename T>
void ListaEncadeada<T>::Limpa() {
    TipoCelula<T>* p = primeiro->prox;
    while (p != nullptr) {
        primeiro->prox = p->prox;
        delete p;
        p = primeiro->prox;
    }
    ultimo = primeiro;
    tamanho = 0;
}

// Localiza célula na posição desejada
template <typename T>
TipoCelula<T>* ListaEncadeada<T>::Posiciona(int pos, bool antes) {
    if ((pos > tamanho) || (pos <= 0))
        throw "ERRO: Posicao Invalida!";

    TipoCelula<T>* p = primeiro;
    for (int i = 1; i < pos; i++) {
        p = p->prox;
    }

    if (!antes) {
        p = p->prox;
    }

    return p;
}

// Retorna item na posição
template <typename T>
T ListaEncadeada<T>::GetItem(int pos) {
    TipoCelula<T>* p = Posiciona(pos, false);
    return p->item;
}

// Insere ordenadamente por ID do pacote (usado na árvore de eventos)
template <typename T>
void ListaEncadeada<T>::InsereOrdenadoPorIdPacote(EventoEstatisticas& elemento) {
    TipoCelula<EventoEstatisticas>* nova = new TipoCelula<EventoEstatisticas>(elemento);

    TipoCelula<EventoEstatisticas>* anterior = primeiro;
    TipoCelula<EventoEstatisticas>* atual = primeiro->prox;

    // Ordena com base no ID do pacote (chave)
    while (atual != nullptr && atual->item.GetChave() < elemento.GetChave()) {
        anterior = atual;
        atual = atual->prox;
    }

    anterior->prox = nova;
    nova->prox = atual;

    if (nova->prox == nullptr) {
        ultimo = nova;
    }

    tamanho++;
}

// Imprime a lista de eventos formatando conforme o tipo
template <typename T>
void ListaEncadeada<T>::Imprime() {
    TipoCelula<T>* p = primeiro->prox;

    while (p != nullptr) {
        cout << p->item.GetTempo() << " ";
        cout << "EV ";

        if (p->item.GetTipo() == RM || p->item.GetTipo() == UR || p->item.GetTipo() == AR) {
            cout << TipoEventoParaString(p->item.GetTipo()) << " ";
            cout << p->item.GetChave() << " ";
            cout << p->item.GetArmazemDestino() << " ";
            cout << p->item.GetSecaoDestino() << endl;
        }

        else if (p->item.GetTipo() == TR) {
            cout << TipoEventoParaString(p->item.GetTipo()) << " ";
            cout << p->item.GetChave() << " ";
            cout << p->item.GetArmazemOrigem() << " ";
            cout << p->item.GetArmazemDestino() << endl;
        }

        else if (p->item.GetTipo() == RG) {
            cout << TipoEventoParaString(p->item.GetTipo()) << " ";
            cout << p->item.GetChave() << " ";
            cout << p->item.GetNomeRemetente() << " ";
            cout << p->item.GetNomeDestinatario() << " ";
            cout << p->item.GetArmazemOrigem() << " ";
            cout << p->item.GetArmazemDestino() << endl;
        }

        else if (p->item.GetTipo() == EN) {
            cout << TipoEventoParaString(p->item.GetTipo()) << " ";
            cout << p->item.GetChave() << " ";
            cout << p->item.GetArmazemDestino() << endl;
        }

        p = p->prox;
    }
}

#endif
