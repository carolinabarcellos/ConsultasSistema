#include "arvore.hpp"
#include "estatisticas.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Nenhum arquivo foi fornecido." << endl;
        return 1;
    }

    ifstream arquivo(argv[1]); // Abrindo com ifstream

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return 1;
    }
    
    //Verifica se o arquivo está vazio
    arquivo.seekg(0, ios::end);
    if (arquivo.tellg() == 0) {
        cout << "O arquivo está vazio." << endl;
        return 1;
    }
    arquivo.seekg(0, ios::beg);

    string linha;
    ArvoreAVL<nodeCliente> arvore_clientes;
    ArvoreAVL<nodePacote> arvore_pacotes;

    // Leitura da entrada
    while (getline(arquivo, linha) && !linha.empty()) {

        istringstream ss(linha);
        string tempo, tipoRegistro;
        ss >> tempo >> tipoRegistro;

        if (tipoRegistro == "EV") {
            EventoEstatisticas e;
            e.SetTempo(tempo);

            string tipo_evento, id_pacote;
            ss >> tipo_evento >> id_pacote; //lê o id do pacote e o tipo do evento
            e.SetTipo(StringParaTipoEvento(tipo_evento));
            e.SetChave(id_pacote);

            // Se não existe um nó para esse pacote, cria e insere na avl de pacotes
            if (arvore_pacotes.Buscar(id_pacote) == nullptr) {
                arvore_pacotes.Insere(id_pacote);
            }

            if (tipo_evento == "RG") {
                string nome_remetente, nome_destinatario, armazemOrigem, armazemDestino;
                ss >> nome_remetente >> nome_destinatario >> armazemOrigem >> armazemDestino;

                e.SetNomeRemetente(nome_remetente);
                e.SetNomeDestinatario(nome_destinatario);
                e.SetArmazemOrigem(armazemOrigem);
                e.SetArmazemDestino(armazemDestino);

                // Inserir clientes na avl de clientes se não existem
                if (arvore_clientes.Buscar(nome_remetente) == nullptr) {
                    arvore_clientes.Insere(nome_remetente);
                }
                if (arvore_clientes.Buscar(nome_destinatario) == nullptr) {
                    arvore_clientes.Insere(nome_destinatario);
                }

                // Atualizar remetente e destinatário do pacote
                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote != nullptr) {
                    pacote->remetente = nome_remetente;
                    pacote->destinatario = nome_destinatario;
                    pacote->historico.InsereFinal(e);
                    pacote->evento_atual=e;
                }

                // Inserir nos pacotes registrados dos respectivos clientes
                auto remetente = arvore_clientes.Buscar(nome_remetente);
                auto destinatario = arvore_clientes.Buscar(nome_destinatario);

                if (remetente) remetente->pacotes_registrados.InsereFinal(e);
                if (destinatario) destinatario->pacotes_registrados.InsereFinal(e);
            }

            else if (tipo_evento == "TR") {
                string armazemOrigem, armazemDestino;
                ss >> armazemOrigem >> armazemDestino;

                e.SetArmazemOrigem(armazemOrigem);
                e.SetArmazemDestino(armazemDestino);

                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote) {
                    pacote->historico.InsereFinal(e);
                    pacote->evento_atual=e;
                    
                }
            }

            else if (tipo_evento == "EN") {
                string armazemDestino;
                ss >> armazemDestino;

                e.SetArmazemDestino(armazemDestino);

                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote) {
                    pacote->historico.InsereFinal(e);
                    pacote->evento_atual=e;
                    
                }
            }

            else if (tipo_evento == "RM" || tipo_evento == "UR" || tipo_evento == "AR") {
                string armazemDestino, secaoDestino;
                ss >> armazemDestino >> secaoDestino;

                e.SetArmazemDestino(armazemDestino);
                e.SetSecaoDestino(secaoDestino);

                auto pacote = arvore_pacotes.Buscar(id_pacote);
                if (pacote) {
                    pacote->historico.InsereFinal(e);
                    pacote->evento_atual=e;
                    
                }
            }
        }

        else if (tipoRegistro == "CL") {
            string nome_cliente;
            ss >> nome_cliente;

            auto cliente = arvore_clientes.Buscar(nome_cliente);
            cout << setw(6) << setfill('0') << stoi(tempo) << " CL " << nome_cliente << endl;

            ArvoreAVL<nodeEvento> * arvore_eventos=new ArvoreAVL<nodeEvento>(); //crio uma avl de eventos

            if (cliente) {
                cout << cliente->pacotes_registrados.GetTamanho() * 2 <<endl;
                cliente->pacotes_registrados.Imprime();
                
                auto no_pacote = cliente->pacotes_registrados.primeiro;
            
                while (no_pacote != nullptr) {
                    string id_pacote = no_pacote->item.GetChave();

                    // Buscar na AVL global de pacotes
                    auto pacote = arvore_pacotes.Buscar(id_pacote);

                    if (pacote) {   
                        string tempo_pacote=pacote->evento_atual.GetTempo();
                        arvore_eventos->Insere(tempo_pacote,pacote->evento_atual);
                        
                    }
                
                    no_pacote = no_pacote->prox;
                }
                
                arvore_eventos->EmOrdem();
                delete arvore_eventos;
            }
            else{
                cout<<0<<endl;
            }

        }

        else if (tipoRegistro == "PC") {
            string id_pacote;
            ss >> id_pacote;

            auto pacote = arvore_pacotes.Buscar(id_pacote);
            cout << setw(6) << setfill('0') << stoi(tempo) << " PC " << id_pacote << endl;

            if (pacote) {
                cout << pacote->historico.GetTamanho() << endl;
                pacote->historico.Imprime();
            } 
            else{
                cout<<0<<endl;
            }
        }
    }

    return 0;
}
