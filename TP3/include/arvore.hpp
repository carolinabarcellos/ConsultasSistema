#ifndef ARVORE_HPP
#define ARVORE_HPP

#include <iostream>
#include <string>
#include "estatisticas.hpp"
#include "lista.hpp"
using namespace std;

// Classe genérica de árvore AVL
template <typename node>
class ArvoreAVL {
public:
    ArvoreAVL();
    ~ArvoreAVL();

    void Insere(string n); // Inserção padrão
    void Remove(string n); // Remoção por chave
    node* Buscar(string chave); // Busca por chave
    void EmOrdem();  // Impressão em ordem
    void PreOrdem(); // Impressão pré-ordem
    void PosOrdem(); // Impressão pós-ordem
    node* GetRaiz(); // Retorna a raiz da árvore

private:
    node* raiz;

    // Funções auxiliares internas
    int Altura(node* n);
    int FatorBalanceamento(node* n);
    void AtualizarAltura(node*& n);
    void InsereRecursivo(node*& p, string n);
    void RemoveRecursivo(node*& p, string chave);
    void RotacaoDireita(node*& y);
    void RotacaoEsquerda(node*& x);
    void Balancear(node*& n);
    node* MaiorChave(node* n);
    void EmOrdem(node* n);
    void PreOrdem(node* n);
    void PosOrdem(node* n);
    void Limpa(node* n); // Libera memória recursivamente
};

// Nó que representa um pacote
class nodePacote {
public:
    string chave; // ID do pacote
    int height;
    string remetente, destinatario;
    ListaEncadeada<int> historico; // Histórico de indices para os eventos
    nodePacote *leftChild, *rightChild, *parent;

    nodePacote(string id) : chave(id), height(1),
        leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}
};

// Nó que representa um cliente
class nodeCliente {
public:
    string chave; // Nome do cliente
    ListaEncadeada<int> pacotes_registrados; // Eventos onde foi remetente/destinatário
    int height;
    nodeCliente *leftChild, *rightChild, *parent;

    nodeCliente(string nome) : chave(nome), height(1),
        leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}

    ~nodeCliente() {}
};

// Construtor: inicia árvore vazia
template <typename node>
ArvoreAVL<node>::ArvoreAVL() {
    raiz = nullptr;
}

// Destrutor: libera todos os nós
template <typename node>
ArvoreAVL<node>::~ArvoreAVL() {
    Limpa(raiz);
}

// Retorna raiz
template <typename node>
node* ArvoreAVL<node>::GetRaiz() {
    return raiz;
}

// Retorna altura do nó
template <typename node>
int ArvoreAVL<node>::Altura(node* n) {
    return (n == nullptr) ? 0 : n->height;
}

// Calcula fator de balanceamento (FB = dir - esq)
template <typename node>
int ArvoreAVL<node>::FatorBalanceamento(node* n) {
    if (n == nullptr) return 0;

    int alturaEsq = (n->leftChild != nullptr) ? n->leftChild->height : 0;
    int alturaDir = (n->rightChild != nullptr) ? n->rightChild->height : 0;

    return alturaDir - alturaEsq;
}

// Atualiza a altura do nó baseado nos filhos
template <typename node>
void ArvoreAVL<node>::AtualizarAltura(node*& n) {
    if (n != nullptr) {
        int alturaEsq = Altura(n->leftChild);
        int alturaDir = Altura(n->rightChild);
        n->height = max(alturaEsq, alturaDir) + 1;
    }
}

// Rotação simples à direita
template <typename node>
void ArvoreAVL<node>::RotacaoDireita(node*& y) {
    node* x = y->leftChild;
    node* z = x->rightChild;

    x->rightChild = y;
    y->leftChild = z;

    if (z) z->parent = y;
    x->parent = y->parent;
    y->parent = x;

    AtualizarAltura(y);
    AtualizarAltura(x);

    y = x;
}

// Rotação simples à esquerda
template <typename node>
void ArvoreAVL<node>::RotacaoEsquerda(node*& x) {
    node* y = x->rightChild;
    node* z = y->leftChild;

    y->leftChild = x;
    x->rightChild = z;

    if (z) z->parent = x;
    y->parent = x->parent;
    x->parent = y;

    AtualizarAltura(x);
    AtualizarAltura(y);

    x = y;
}

// Balanceia a subárvore a partir de n
template <typename node>
void ArvoreAVL<node>::Balancear(node*& n) {
    AtualizarAltura(n);

    int fb = FatorBalanceamento(n);

    if (fb > 1) { // desequilíbrio à direita
        if (FatorBalanceamento(n->rightChild) < 0) {
            RotacaoDireita(n->rightChild);
        }
        RotacaoEsquerda(n);
    }
    else if (fb < -1) { // desequilíbrio à esquerda
        if (FatorBalanceamento(n->leftChild) > 0) {
            RotacaoEsquerda(n->leftChild);
        }
        RotacaoDireita(n);
    }
}

// Inserção padrão
template <typename node>
void ArvoreAVL<node>::InsereRecursivo(node*& p, string n) {
    if (p == nullptr) {
        p = new node(n);
    } else {
        if (n < p->chave) {
            InsereRecursivo(p->leftChild, n);
            p->leftChild->parent = p;
        }
        else if (n > p->chave) {
            InsereRecursivo(p->rightChild, n);
            p->rightChild->parent = p;
        }
    }
    Balancear(p);
}

template <typename node>
void ArvoreAVL<node>::Insere(string n) {
    InsereRecursivo(raiz, n);
}

// Busca por chave
template <typename node>
node* ArvoreAVL<node>::Buscar(string chave) {
    node* atual = raiz;
    while (atual != nullptr) {
        if (chave == atual->chave) return atual;
        if (chave < atual->chave) atual = atual->leftChild;
        else atual = atual->rightChild;
    }
    return nullptr;
}

// Retorna o maior nó (usado na remoção)
template <typename node>
node* ArvoreAVL<node>::MaiorChave(node* n) {
    node* atual = n;
    while (atual->rightChild != nullptr) {
        atual = atual->rightChild;
    }
    return atual;
}

// Remove por chave
template <typename node>
void ArvoreAVL<node>::Remove(string n) {
    RemoveRecursivo(raiz, n);
}

template <typename node>
void ArvoreAVL<node>::RemoveRecursivo(node*& p, string chave) {
    if (p == nullptr) return;

    if (chave < p->chave) {
        RemoveRecursivo(p->leftChild, chave);
    }
    else if (chave > p->chave) {
        RemoveRecursivo(p->rightChild, chave);
    }
    else {
        // Nó com um ou nenhum filho
        if (p->leftChild == nullptr || p->rightChild == nullptr) {
            node* temp = (p->leftChild != nullptr) ? p->leftChild : p->rightChild;
            if (temp) temp->parent = p->parent;
            delete p;
            p = temp;
        }
        else {
            // Dois filhos: substitui pelo maior da subárvore esquerda
            node* temp = MaiorChave(p->leftChild);
            p->chave = temp->chave;
            RemoveRecursivo(p->leftChild, temp->chave);
        }
    }

    if (p != nullptr) Balancear(p);
}

// Libera todos os nós da árvore
template <typename node>
void ArvoreAVL<node>::Limpa(node* n) {
    if (n != nullptr) {
        Limpa(n->leftChild);
        Limpa(n->rightChild);
        delete n;
    }
}

// Impressão em ordem
template <typename node>
void ArvoreAVL<node>::EmOrdem() {
    EmOrdem(raiz);
}

// Imprime eventos em ordem crescente de chave
template <typename node>
void ArvoreAVL<node>::EmOrdem(node* n) {
    if (n != nullptr) {
        EmOrdem(n->leftChild);
        n->eventos.Imprime();
        EmOrdem(n->rightChild);
    }
}

template <typename node>
void ArvoreAVL<node>::PreOrdem() {
    PreOrdem(raiz);
}

template <typename node>
void ArvoreAVL<node>::PreOrdem(node* n) {
    if (n != nullptr) {
        n->eventos.Imprime();
        PreOrdem(n->leftChild);
        PreOrdem(n->rightChild);
    }
}

template <typename node>
void ArvoreAVL<node>::PosOrdem() {
    PosOrdem(raiz);
}

template <typename node>
void ArvoreAVL<node>::PosOrdem(node* n) {
    if (n != nullptr) {
        PosOrdem(n->leftChild);
        PosOrdem(n->rightChild);
        n->eventos.Imprime();
    }
}

#endif
