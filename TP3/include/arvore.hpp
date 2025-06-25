#ifndef ARVORE_HPP
#define ARVORE_HPP

#include <iostream>
#include <string>
#include "estatisticas.hpp"
#include "lista.hpp"
using namespace std;

// Classe Árvore AVL
template <typename node>
class ArvoreAVL {
public:
    ArvoreAVL();
    ~ArvoreAVL();

    void Insere(string n);
    void Insere (string chave, EventoEstatisticas elemento);
    void Remove(string n);
    node* Buscar(string chave);
    void EmOrdem();
    void PreOrdem();
    void PosOrdem();
    node* GetRaiz();

private:
    node* raiz;

    int Altura(node* n);
    int FatorBalanceamento(node* n);
    void AtualizarAltura(node*& n);
    void InsereRecursivo(node*& p, string n);
    void InsereRecursivo(node*& p, string chave, EventoEstatisticas elemento);
    void RemoveRecursivo(node*& p, string chave);
    void RotacaoDireita(node*& y);
    void RotacaoEsquerda(node*& x);
    void Balancear(node*& n);
    node* MaiorChave(node* n);
    void EmOrdem(node* n);
    void PreOrdem(node* n);
    void PosOrdem(node* n);
    void Limpa(node* n);
};

class nodePacote {
public:
    string chave;
    int height;
    string remetente;
    string destinatario;
    EventoEstatisticas evento_atual;
    ListaEncadeada<EventoEstatisticas> historico;  //Lista encadeada de eventos relacionados ao pacote e e suas estatísticas
    nodePacote* leftChild;
    nodePacote* rightChild;
    nodePacote* parent;

    nodePacote(string id) : chave(id), height(1), 
        leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}
};

//Nó evento
class nodeEvento {
public:
    string chave;
    int height;
    nodeEvento* leftChild;
    nodeEvento* rightChild;
    nodeEvento* parent;
    ListaEncadeada<EventoEstatisticas> eventos; // Lista de eventos 

    nodeEvento(string tempo) : chave(tempo), height(1), 
        leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}
    
    ~nodeEvento(){}
};


//Nó cliente
class nodeCliente {
    public:
        string chave;
        ListaEncadeada<EventoEstatisticas> pacotes_registrados;
        int height;
        nodeCliente* leftChild;
        nodeCliente* rightChild;
        nodeCliente* parent;

        nodeCliente(string nome) : chave(nome), height(1),
            leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}

        ~nodeCliente() {}
};

template <typename node>
ArvoreAVL<node>::ArvoreAVL() {
    raiz = nullptr;
}

template <typename node>
ArvoreAVL<node>::~ArvoreAVL() {
    Limpa(raiz);
}

template <typename node>
node* ArvoreAVL<node>::GetRaiz() {
    return raiz;
}

template <typename node>
int ArvoreAVL<node>::Altura(node* n) {
    return (n == nullptr) ? 0 : n->height;
}

template <typename node>
int ArvoreAVL<node>::FatorBalanceamento(node* n) {
    if (n == nullptr) return 0;

    int alturaEsq = (n->leftChild != nullptr) ? n->leftChild->height : 0;
    int alturaDir = (n->rightChild != nullptr) ? n->rightChild->height : 0;

    return alturaDir - alturaEsq;
}

template <typename node>
void ArvoreAVL<node>::AtualizarAltura(node*& n) {
    if (n != nullptr) {
        int alturaEsq = Altura(n->leftChild);
        int alturaDir = Altura(n->rightChild);
        n->height = max(alturaEsq, alturaDir) + 1;
    }
}

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

template <typename node>
void ArvoreAVL<node>::Balancear(node*& n) {
    AtualizarAltura(n);

    int fb = FatorBalanceamento(n);

    if (fb > 1) {
        if (FatorBalanceamento(n->rightChild) < 0) {
            RotacaoDireita(n->rightChild);
        }
        RotacaoEsquerda(n);
    }
    else if (fb < -1) {
        if (FatorBalanceamento(n->leftChild) > 0) {
            RotacaoEsquerda(n->leftChild);
        }
        RotacaoDireita(n);
    }
}

template <typename node>
void ArvoreAVL<node>::InsereRecursivo(node*& p, string n) {
    if (p == nullptr) {
        p = new node(n);
    }
    else {
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
void ArvoreAVL<node>::InsereRecursivo(node*& p, string n, EventoEstatisticas elemento) {
    if (p == nullptr) {
        p = new node(n);
        p->eventos.InsereOrdenadoPorIdPacote(elemento);  // Cria nó e já insere na lista
    }
    else {
        if (n < p->chave) {
            InsereRecursivo(p->leftChild, n, elemento);
            p->leftChild->parent = p;
        }
        else if (n > p->chave) {
            InsereRecursivo(p->rightChild, n, elemento);
            p->rightChild->parent = p;
        }
        else {
            // Se a chave já existe, apenas adiciona na lista de eventos
            p->eventos.InsereOrdenadoPorIdPacote(elemento);
            return; // Já retorna aqui, pois não há alteração na árvore
        }
    }

    Balancear(p);
}


template <typename node>
void ArvoreAVL<node>::Insere(string n) {
    InsereRecursivo(raiz, n);
}

template <typename node>
void ArvoreAVL<node>::Insere(string chave, EventoEstatisticas elemento) {
    InsereRecursivo(raiz, chave, elemento);
}


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

template <typename node>
node* ArvoreAVL<node>::MaiorChave(node* n) {
    node* atual = n;
    while (atual->rightChild != nullptr) {
        atual = atual->rightChild;
    }
    return atual;
}

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
        if (p->leftChild == nullptr || p->rightChild == nullptr) {
            node* temp = (p->leftChild != nullptr) ? p->leftChild : p->rightChild;
            if (temp) temp->parent = p->parent;
            delete p;
            p = temp;
        }
        else {
            node* temp = MaiorChave(p->leftChild);
            p->chave = temp->chave;
            RemoveRecursivo(p->leftChild, temp->chave);
        }
    }

    if (p != nullptr){
        Balancear(p);
    }
}

template <typename node>
void ArvoreAVL<node>::Limpa(node* n) {
    if (n != nullptr) {
        Limpa(n->leftChild);
        Limpa(n->rightChild);
        delete n;
    }
}

template <typename node>
void ArvoreAVL<node>::EmOrdem() {
    EmOrdem(raiz);
}

//Adaptado pra tratar evento estatisticas
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

//Adaptado pra tratar evento estatisticas
template <typename node>
void ArvoreAVL<node>::PreOrdem(node* n) {
    if (n != nullptr) {
        cout << n->chave << " ";
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
        cout << n->chave << " ";
    }
}

#endif