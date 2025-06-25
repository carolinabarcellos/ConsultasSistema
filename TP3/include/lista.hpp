#ifndef LISTA_HPP
#define LISTA_HPP

#include "iostream"

using namespace std;

//Adaptação do algoritmo de lista encadeada aprendido em sala de aula 
//Foi escolhida uma implementação genérica da lista com templates

template <typename T> class TipoCelula; // forward declaration

template <typename T>
class ListaEncadeada{
    public:
        ListaEncadeada();
        ~ListaEncadeada();
        int GetTamanho();
        bool ListaVazia();
        T GetItem(int pos);
        void InsereFinal(T item);
        void InsereInicio(T item);
        void InserePosicao(T item, int pos);
        T Pesquisa(T c);
        void Limpa();
        void Imprime();
        void InsereOrdenadoPorIdPacote(EventoEstatisticas &elemento);
        TipoCelula<T> *primeiro; //Aponta para a célula cabeça
        TipoCelula<T> *ultimo;

    private:
        int tamanho;
        TipoCelula<T> *Posiciona(int pos, bool antes);


};

template <typename T>
class TipoCelula{
    public:
        TipoCelula(T n);
        TipoCelula();
        T item;
        TipoCelula<T> *prox;

};


// Classe TipoCelula

template <typename T>
TipoCelula<T>::TipoCelula(T n)
{
    item = n;
    prox = nullptr;
}

template <typename T>
TipoCelula<T>::TipoCelula()
{
    prox = nullptr;
}

// Classe ListaEncadeada

template <typename T>
ListaEncadeada<T>::ListaEncadeada()
{
    tamanho = 0;
    primeiro = new TipoCelula<T>();
    ultimo = primeiro;
}

template <typename T>
ListaEncadeada<T>::~ListaEncadeada()
{
    Limpa();
    delete primeiro;
}

template <typename T>
int ListaEncadeada<T>::GetTamanho()
{
    return tamanho;
}

template <typename T>
T ListaEncadeada<T>::Pesquisa(T c)
{
    T aux;
    TipoCelula<T> *p;
    p = primeiro->prox;

    while (p != nullptr)
    {
        if (p->item == c)
        {
            aux = p->item;
            break;
        }

        p = p->prox;
    }

    return aux;
}

template <typename T>
bool ListaEncadeada<T>::ListaVazia()
{
    return tamanho == 0;
}

template <typename T>
void ListaEncadeada<T>::InsereFinal(T item)
{

    TipoCelula<T> *nova;
    nova = new TipoCelula<T>(item);
    ultimo->prox = nova;
    ultimo = nova;
    tamanho++;
}

template <typename T>
void ListaEncadeada<T>::InsereInicio(T item){

    TipoCelula<T> *nova;
    nova = new TipoCelula<T>();
    nova->item = item; 
    nova->prox = primeiro->prox; 
    primeiro->prox = nova; 
    tamanho++;

    if(nova->prox == nullptr){
        ultimo = nova;
    }
}

template <typename T>
void ListaEncadeada<T>::InserePosicao(T item, int pos){
    TipoCelula<T> *p, *nova;

    p = Posiciona(pos,true); // posiciona na célula anterior

    nova = new TipoCelula<T>();
    nova->item = item;
    nova->prox = p->prox;
    p->prox = nova;
    tamanho++;

    if(nova->prox == nullptr){
        ultimo = nova;
    }
}


template <typename T>
void ListaEncadeada<T>::Limpa()
{
    TipoCelula<T> *p;
    p = primeiro->prox;
    while (p != nullptr)
    {
        primeiro->prox = p->prox;
        delete p;
        p = primeiro->prox;
    }
    ultimo = primeiro;
    tamanho = 0;
}

//Método utilizado pra imprimir as estatisticas dos eventos relacionados ao pacote
template <typename T>
void ListaEncadeada<T>::Imprime(){ //adaptada pra imprimir apenas o que for relevante dependendo do tipo do evento

    TipoCelula<T> *p;
    p = primeiro->prox;

    while (p != nullptr){
        cout<<p->item.GetTempo()<<" ";   
        cout<<"EV ";
        if (p->item.GetTipo()==RM || p->item.GetTipo()==UR || p->item.GetTipo()==AR){

            cout<<TipoEventoParaString(p->item.GetTipo())<<" ";   
            cout<<p->item.GetChave()<<" ";
            cout<<p->item.GetArmazemDestino()<<" ";
            cout<<p->item.GetSecaoDestino()<<endl; 
        }

        else if (p->item.GetTipo()==TR){
            
            cout<<TipoEventoParaString(p->item.GetTipo())<<" ";  
            cout<<p->item.GetChave()<<" "; 
            cout<<p->item.GetArmazemOrigem()<<" ";
            cout<<p->item.GetArmazemDestino()<<endl; 
        }

        else if (p->item.GetTipo()==RG){

            cout<<TipoEventoParaString(p->item.GetTipo())<<" "; 
            cout<<p->item.GetChave()<<" ";  
            cout<<p->item.GetNomeRemetente()<<" "; 
            cout<<p->item.GetNomeDestinatario()<< " ";  
            cout<<p->item.GetArmazemOrigem()<<" ";
            cout<<p->item.GetArmazemDestino()<<endl; 
        }

        else if(p->item.GetTipo()==EN){
            cout<<TipoEventoParaString(p->item.GetTipo())<<" ";  
            cout<<p->item.GetChave()<<" "; 
            cout<<p->item.GetArmazemDestino()<<endl; 

        }
        
        p=p->prox;
    }

}

template <typename T>
TipoCelula<T> * ListaEncadeada<T>::Posiciona(int pos, bool antes){
    TipoCelula<T> *p;
    int i;
    if ((pos > tamanho) || (pos <= 0))
        throw "ERRO: Posicao Invalida!";

    // Posiciona na célula anterior a desejada
    p = primeiro;
    for (i = 1; i < pos; i++){
        
        p = p->prox;
    }

    if (!antes){
        p = p->prox;
    }

    return p;
}

template <typename T>
T ListaEncadeada<T>::GetItem(int pos)
{
    TipoCelula<T> *p;
    p = Posiciona(pos, false);
    return p->item;
}

template <typename T>
void ListaEncadeada<T>::InsereOrdenadoPorIdPacote(EventoEstatisticas& elemento) {
    TipoCelula<EventoEstatisticas>* nova = new TipoCelula<EventoEstatisticas>(elemento);

    TipoCelula<EventoEstatisticas>* anterior = primeiro;
    TipoCelula<EventoEstatisticas>* atual = primeiro->prox;

    while (atual != nullptr && atual->item.GetChave() < elemento.GetChave()) {
        anterior = atual;
        atual = atual->prox;
    }

    // Inserção entre anterior e atual
    anterior->prox = nova;
    nova->prox = atual;

    if (nova->prox == nullptr) {
        ultimo = nova;
    }

    tamanho++;
}



#endif