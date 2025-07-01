#include "lista.hpp"
#include "arvore.hpp"
#include "estatisticas.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

EventoEstatisticas lista_eventos[500]; //Vetor que armazena os eventos
using namespace std;

int main(int argc, char* argv[]) {

    // Verifica se um arquivo foi passado como argumento
    if (argc < 2) {
        cout << "Nenhum arquivo foi fornecido." << endl;
        return 1;
    }

    ifstream arquivo(argv[1]); // Abre o arquivo

    // Verifica se conseguiu abrir
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    // Verifica se o arquivo está vazio
    arquivo.seekg(0, ios::end);
    if (arquivo.tellg() == 0) {
        cout << "O arquivo está vazio." << endl;
        return 1;
    }
    arquivo.seekg(0, ios::beg); // Retorna para o início do arquivo

    string linha;
    ArvoreAVL<nodeCliente> arvore_clientes; // AVL de clientes
    ArvoreAVL<nodePacote> arvore_pacotes;   // AVL de pacotes

    int indice=0;

    // Processamento linha por linha
    while (getline(arquivo, linha) && !linha.empty()) {

        istringstream ss(linha);
        string tempo, tipoRegistro;
        ss >> tempo >> tipoRegistro;

        // Registro de evento
        if (tipoRegistro == "EV") {
            EventoEstatisticas e;
            e.SetTempo(tempo);

            string tipo_evento, id_pacote;
            ss >> tipo_evento >> id_pacote;
            e.SetTipo(StringParaTipoEvento(tipo_evento));
            e.SetChave(id_pacote);

            // Se o pacote ainda não existe, insere na AVL de pacotes
            if (arvore_pacotes.Buscar(id_pacote) == nullptr) {
                arvore_pacotes.Insere(id_pacote);
            }

            // Evento de registro de pacote
            if (tipo_evento == "RG") {
                string nome_remetente, nome_destinatario, armazemOrigem, armazemDestino;
                ss >> nome_remetente >> nome_destinatario >> armazemOrigem >> armazemDestino;

                e.SetNomeRemetente(nome_remetente);
                e.SetNomeDestinatario(nome_destinatario);
                e.SetArmazemOrigem(armazemOrigem);
                e.SetArmazemDestino(armazemDestino);

                // Insere clientes se ainda não existirem
                if (arvore_clientes.Buscar(nome_remetente) == nullptr)
                    arvore_clientes.Insere(nome_remetente);
                if (arvore_clientes.Buscar(nome_destinatario) == nullptr)
                    arvore_clientes.Insere(nome_destinatario);

                // Atualiza dados do pacote e adiciona o indice do evento ao histórico
                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote != nullptr) {
                    pacote->remetente = nome_remetente;
                    pacote->destinatario = nome_destinatario;
                    pacote->historico.InsereFinal(indice);
                }

                // Adiciona evento aos pacotes registrados de remetente e destinatário
                auto remetente = arvore_clientes.Buscar(nome_remetente);
                auto destinatario = arvore_clientes.Buscar(nome_destinatario);
                if (remetente) remetente->pacotes_registrados.InsereFinal(indice);
                if (destinatario) destinatario->pacotes_registrados.InsereFinal(indice);
            }

            // Evento de transporte
            else if (tipo_evento == "TR") {
                string armazemOrigem, armazemDestino;
                ss >> armazemOrigem >> armazemDestino;
                e.SetArmazemOrigem(armazemOrigem);
                e.SetArmazemDestino(armazemDestino);

                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote)
                    pacote->historico.InsereFinal(indice);
            }

            // Evento de entrega
            else if (tipo_evento == "EN") {
                string armazemDestino;
                ss >> armazemDestino;
                e.SetArmazemDestino(armazemDestino);

                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote)
                    pacote->historico.InsereFinal(indice);
            }

            // Eventos de Remoção, Rearmazenamento e Armazenamento
            else if (tipo_evento == "RM" || tipo_evento == "UR" || tipo_evento == "AR") {
                string armazemDestino, secaoDestino;
                ss >> armazemDestino >> secaoDestino;
                e.SetArmazemDestino(armazemDestino);
                e.SetSecaoDestino(secaoDestino);

                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote)
                    pacote->historico.InsereFinal(indice);
            }

            lista_eventos[indice]=e;
            indice++;
        }

        // Consulta por cliente
        else if (tipoRegistro == "CL") {
            string nome_cliente;
            ss >> nome_cliente;

            auto cliente = arvore_clientes.Buscar(nome_cliente);
            cout << setw(6) << setfill('0') << stoi(tempo) << " CL " << nome_cliente << endl;


            if (cliente) {
                cout << cliente->pacotes_registrados.GetTamanho() * 2 << endl;
                cliente->pacotes_registrados.Imprime(); 

                const int MAX_EVENTOS = 500;
                int indices_eventos[MAX_EVENTOS];
                int qtd = 0;

                // Percorre todos os pacotes registrados pelo cliente
                auto no_pacote = cliente->pacotes_registrados.primeiro->prox;
                while (no_pacote != nullptr && qtd < MAX_EVENTOS) {
                    string id_pacote = lista_eventos[no_pacote->item].GetChave();
                    auto pacote = arvore_pacotes.Buscar(id_pacote);

                    if (pacote && pacote->historico.ultimo != nullptr) {
                        int indice_ultimo_evento = pacote->historico.ultimo->item;
                        indices_eventos[qtd++] = indice_ultimo_evento;
                    }

                    no_pacote = no_pacote->prox;
                }

                // Ordena os índices manualmente por tempo (Insertion Sort)
                for (int i = 1; i < qtd; ++i) {
                    int chave = indices_eventos[i];
                    int j = i - 1;

                    while (j >= 0 && (lista_eventos[indices_eventos[j]].GetTempo() > lista_eventos[chave].GetTempo() ||
                    (lista_eventos[indices_eventos[j]].GetTempo() == lista_eventos[chave].GetTempo() &&
                    lista_eventos[indices_eventos[j]].GetChave() > lista_eventos[chave].GetChave()) )) {
                        indices_eventos[j + 1] = indices_eventos[j];
                        j--;
                    }
                    
                    indices_eventos[j + 1] = chave;
                }

                // Imprime os eventos ordenados
                for (int i = 0; i < qtd; ++i) {
                    EventoEstatisticas& evento = lista_eventos[indices_eventos[i]];
                    evento.Imprime();
                    
                }
            }

            else{
                cout << 0 << endl;
            }
        }

        // Consulta por pacote
        else if (tipoRegistro == "PC") {
            string id_pacote;
            ss >> id_pacote;

            auto pacote = arvore_pacotes.Buscar(id_pacote);
            cout << setw(6) << setfill('0') << stoi(tempo) << " PC " << id_pacote << endl;

            if (pacote){
                cout<< pacote->historico.GetTamanho() << endl;
                pacote->historico.Imprime(); // Imprime todos os eventos do pacote
            } 
            else {
                cout << 0 << endl;
            }
        }
    }

    return 0;
}

