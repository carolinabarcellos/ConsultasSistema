#include "estatisticas.hpp"
#include <iostream>
using namespace std;

TipoEventoEstatisticas StringParaTipoEvento(const string& tipoStr) {
    if (tipoStr == "RG") return RG;
    if (tipoStr == "AR") return AR;  
    if (tipoStr == "RM") return RM;
    if (tipoStr == "UR") return UR;
    if (tipoStr == "TR") return TR;
    if (tipoStr == "EN") return EN;

    cout << "Erro: tipo de evento desconhecido -> " << tipoStr << endl;
    exit(1);
}

string TipoEventoParaString(TipoEventoEstatisticas tipo_evento) {
    if (tipo_evento == RG) return "RG";
    if (tipo_evento == AR) return "AR";  
    if (tipo_evento == RM) return "RM";
    if (tipo_evento == UR) return "UR";
    if (tipo_evento == TR) return "TR";
    if (tipo_evento == EN) return "EN";

    cout << "Erro: tipo de evento desconhecido -> " << tipo_evento << endl;
    exit(1);
}

//Construtor
EventoEstatisticas::EventoEstatisticas(){
    tempo="0000000"; 
    armazem_origem="";
    armazem_destino="";
    nome_remetente="";
    nome_destinatario="";
    chave_pacote="";
}

void EventoEstatisticas::Imprime(){
        cout<<tempo<<" ";   
        cout<<"EV ";
        
        if (tipo_evento==RM || tipo_evento==UR || tipo_evento==AR){

            cout<<TipoEventoParaString(tipo_evento)<<" ";   
            cout<<chave_pacote<<" ";
            cout<<armazem_destino<<" ";
            cout<<secao_destino<<endl; 
        }

        else if (tipo_evento==TR){

            cout<<TipoEventoParaString(tipo_evento)<<" "; 
            cout<<chave_pacote<<" ";  
            cout<<armazem_origem<<" ";
            cout<<armazem_destino<<endl; 
        }

        else if (tipo_evento==RG){
            
            cout<<TipoEventoParaString(tipo_evento)<<" ";  
            cout<<chave_pacote<<" "; 
            cout<<nome_remetente<<" "; 
            cout<<nome_destinatario<< " ";  
            cout<<armazem_origem<<" ";
            cout<<armazem_destino<<endl; 
        }

        else if(tipo_evento==EN){
            
            cout<<TipoEventoParaString(tipo_evento)<<" ";  
            cout<<chave_pacote<<" ";
            cout<<armazem_destino<<endl; 

        }
}

// Getters
TipoEventoEstatisticas EventoEstatisticas::GetTipo() const {
    return tipo_evento;
}

string EventoEstatisticas::GetTempo() const {
    return tempo;
}

string EventoEstatisticas::GetArmazemOrigem() const {
    return armazem_origem;
}

string EventoEstatisticas::GetSecaoDestino() const{
    return secao_destino;
}

string EventoEstatisticas::GetArmazemDestino() const {
    return armazem_destino;
}

string EventoEstatisticas::GetNomeRemetente() const {
    return nome_remetente;
}

string EventoEstatisticas::GetNomeDestinatario() const {
    return nome_destinatario;
}

string EventoEstatisticas::GetChave(){
    return chave_pacote;
}

// Setters
void EventoEstatisticas::SetTipo(TipoEventoEstatisticas tipo) {
    this->tipo_evento = tipo;
}

void EventoEstatisticas::SetTempo(const string& tempo) {
    this->tempo = tempo;
}

void EventoEstatisticas::SetArmazemOrigem(const string& origem) {
    this->armazem_origem = origem;
}

void EventoEstatisticas::SetArmazemDestino(const string& destino) {
    this->armazem_destino = destino;
}

void EventoEstatisticas::SetSecaoDestino(const string& destino){
    this->secao_destino = destino;
}

void EventoEstatisticas::SetNomeRemetente(const string& nome) {
    this->nome_remetente = nome;
}

void EventoEstatisticas::SetNomeDestinatario(const string& nome) {
    this->nome_destinatario = nome;
}

void EventoEstatisticas::SetChave(string chave){
    this->chave_pacote=chave;
}
