# Arquitetura

## Visão Geral

O Fork Task Runner C é um executor de tarefas multiprocessos. O processo pai controla
o estado do sistema e os filhos executam comandos reais via `fork()` e `exec`.

```txt
arquivo de tarefas
       |
       v
fila FIFO ---------> scheduler ---------> fork/exec
       |                 |                    |
       |                 v                    v
       |          lista de ativos       processo filho
       |                 |
       v                 v
pilha de eventos <--- waitpid/timeout
       |
       v
painel terminal
```

## Fronteiras

| Camada | Responsabilidade |
|---|---|
| `domain` | Tipos centrais: tarefa, status e eventos. |
| `structures` | Fila, lista encadeada e pilha. |
| `application` | Regras do scheduler FCFS com concorrência limitada. |
| `infrastructure` | Leitura de arquivos e chamadas POSIX de processo. |
| `presentation` | Painel terminal e relatório final. |

## Decisões Técnicas

O pai é dono das estruturas de dados. Essa escolha evita memória compartilhada e
deixa o comportamento mais fácil de explicar, testar e manter.

O filho executa apenas o comando recebido. Ele não altera fila, lista ou pilha.

O timeout encerra o grupo de processo iniciado pelo filho. Isso cobre comandos
executados por shell que podem criar subprocessos.

O painel terminal é leitura de estado. Ele não contém regra de negócio.

## Algoritmo

O algoritmo é FCFS com concorrência limitada:

1. Enquanto houver espaço livre, remove a próxima tarefa da fila.
2. Cria um processo filho com `fork()`.
3. Registra o PID na lista de processos ativos.
4. Usa `waitpid()` para coletar filhos finalizados.
5. Remove processos concluídos da lista.
6. Registra eventos recentes na pilha.
7. Reexibe o painel terminal.

## Complexidade

| Operação | Estrutura | Complexidade |
|---|---|---|
| Inserir tarefa pendente | Fila | O(1) |
| Remover próxima tarefa | Fila | O(1) |
| Inserir processo ativo | Lista | O(1) |
| Remover processo por PID | Lista | O(n) |
| Inserir evento recente | Pilha | O(1) |
| Remover evento recente | Pilha | O(1) |

Essa complexidade é adequada para o escopo didático-profissional do projeto.
