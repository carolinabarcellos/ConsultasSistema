#ifndef ESTATISTICAS_HPP
#define ESTATISTICAS_HPP

#include <string>

using namespace std;


enum TipoEventoEstatisticas{RG=1, AR, RM, UR, TR, EN};
TipoEventoEstatisticas StringParaTipoEvento(const std::string& tipoStr);
string TipoEventoParaString(TipoEventoEstatisticas tipo_evento);

class EventoEstatisticas {
    private:
        TipoEventoEstatisticas tipo_evento;
        string chave_pacote;
        string tempo;
        string armazem_origem;
        string armazem_destino;
        string secao_destino;
        string nome_remetente;
        string nome_destinatario;

    public:
        // Construtor
        EventoEstatisticas();
        void Imprime();

        // Métodos Getters
        TipoEventoEstatisticas GetTipo() const;
        string GetTempo() const;
        string GetArmazemOrigem() const;
        string GetArmazemDestino() const;
        string GetSecaoDestino() const;
        string GetNomeRemetente() const;
        string GetNomeDestinatario() const;
        string GetChave();

        // Métodos Setters
        void SetTipo(TipoEventoEstatisticas tipo);
        void SetTempo(const string& tempo);
        void SetArmazemOrigem(const string& origem);
        void SetArmazemDestino(const string& destino);
        void SetSecaoDestino(const string& destino);
        void SetNomeRemetente(const string& nome);
        void SetNomeDestinatario(const string& nome);
        void SetChave(string chave);
};

#endif