# Pesquisa Técnica: Fork Task Runner C

## Formatação Recomendada Para PDF

Para versão acadêmica ou empresarial em PDF/DOCX:

- fonte Arial tamanho 12 no corpo;
- espaçamento 1,5;
- texto justificado;
- margens de 3 cm à esquerda/superior e 2 cm à direita/inferior;
- títulos estruturados em seções numeradas;
- capa, folha de rosto e elementos pré-textuais centralizados conforme modelo da instituição;
- referências em padrão ABNT ou ABNT adaptado para fontes web.

## Problema

Exemplos de `fork()` em C costumam mostrar apenas pai e filho imprimindo PIDs. Isso
é insuficiente para explicar como processos participam de um fluxo real. Este projeto
propõe um executor multiprocessos em que `fork()` é usado dentro de um scheduler
com fila, lista encadeada, pilha, timeout, testes e CI.

## Justificativa

O projeto mostra a relação entre sistemas operacionais e estruturas de dados:

- fila para organizar tarefas pendentes;
- lista para acompanhar processos ativos;
- pilha para registrar eventos recentes;
- `fork()` para criar execução paralela por processo;
- `waitpid()` para coletar término e evitar processos zumbi;
- pipelines de CI para tornar o experimento verificável.

## Base Conceitual

`fork()` cria um processo filho a partir do processo chamador. Em sistemas Linux
modernos, a cópia de memória é eficiente por copy-on-write, ou seja, páginas só
são copiadas quando modificadas.

`exec` substitui a imagem do processo pelo comando real. Por isso, shells e runners
costumam combinar `fork()` com `exec`.

`waitpid()` permite que o processo pai observe o término do filho e colete seu status.
Sem esse passo, o sistema pode manter processos zumbi.

## Algoritmo Escolhido

O algoritmo é FCFS com concorrência limitada. Ele foi escolhido porque:

- tem baixa complexidade;
- é comum em escalonamento básico;
- encaixa naturalmente com fila FIFO;
- permite visualizar concorrência sem introduzir prioridade ou preempção;
- demonstra processos reais sem transformar o projeto em um sistema operacional.

## Contribuição Do Projeto

O projeto transforma um conceito isolado de C/POSIX em uma ferramenta pequena,
testável e documentada. Ele não tenta competir com runners reais de CI. O valor está
em tornar explícita a ligação entre estruturas de dados, processos e qualidade de
engenharia.
