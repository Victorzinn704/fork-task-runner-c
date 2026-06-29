# Publicação E Redes

## Objetivo

Publicar o Fork Task Runner C como um repositório técnico independente, com foco em
processos POSIX, estruturas de dados, testes e engenharia de qualidade.

## Checklist Do GitHub

- Repositório sugerido: `fork-task-runner-c`.
- Descrição curta: `Executor multiprocessos em C com fork, fila, lista, pilha, testes e CI`.
- Topics sugeridos: `c`, `posix`, `fork`, `scheduler`, `data-structures`, `linux`, `systems-programming`.
- README com execução rápida e arquitetura.
- Workflows verdes antes de divulgar.
- Licença MIT.
- Issues iniciais para roadmap.

## Roadmap Público

| Fase | Entrega |
|---|---|
| 0.1 | Scheduler FCFS com concorrência limitada. |
| 0.2 | Exportação de relatório em JSON. |
| 0.3 | Modo observável com logs estruturados. |
| 0.4 | Documento PDF ABNT gerado por pipeline. |

## Post Técnico Curto

```txt
Publiquei um projeto em C para demonstrar fork() em um processo real:

- fila FIFO para tarefas pendentes
- lista encadeada para processos ativos
- pilha para eventos recentes
- fork/exec/waitpid
- timeout
- testes, sanitizers, CodeQL e Valgrind

A proposta é conectar estruturas de dados com sistemas operacionais em um projeto pequeno, testável e documentado.
```

## Post Técnico Longo

```txt
Criei o Fork Task Runner C, um executor multiprocessos em C baseado em POSIX.

A ideia não é mostrar fork() como exemplo isolado de pai e filho imprimindo PID. O objetivo é colocar fork() dentro de um fluxo real:

1. tarefas entram em uma fila FIFO;
2. o scheduler respeita um limite de concorrência;
3. cada tarefa vira um processo filho;
4. a lista encadeada acompanha PIDs ativos;
5. waitpid() coleta o resultado;
6. timeouts encerram processos demorados;
7. a pilha mostra os eventos recentes no terminal.

Também inclui CMake, CTest, clang-format, clang-tidy, sanitizers, CodeQL, Valgrind e documentação técnica em português.
```
