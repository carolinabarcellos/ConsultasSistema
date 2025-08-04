# Trabalho Prático – Estrutura de Dados (DCC-UFMG)

## Módulo de Consultas – Armazéns Hanoi

Após a implementação do sistema de escalonamento logístico dos **Armazéns Hanoi** no segundo trabalho da disciplina *Estrutura de Dados*, este novo trabalho prático tem como objetivo a construção do **módulo de consultas ao sistema**.

O sistema é alimentado por uma sequência temporal de eventos logísticos, como registros, armazenamentos, transportes e entregas de pacotes. A partir desses eventos, o sistema responde a dois tipos principais de consultas:

---

### Consulta por Pacote

Retorna todos os eventos ocorridos com um determinado pacote até o momento da consulta, incluindo:

- Tempo do evento  
- Tipo de evento

---

### Consulta por Cliente

Lista todos os pacotes em que o cliente aparece como:

- Remetente  
- Destinatário  

Com as seguintes informações:

- Identificador do pacote  
- Papel do cliente (remetente ou destinatário)  
- Estado atual do pacote  
- Tempo da última atualização

---

Este módulo garante o rastreamento completo dos pacotes, permitindo um acompanhamento transparente e eficiente por parte de todos os envolvidos. 
