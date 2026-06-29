# Auditoria De Prontidão Acadêmica

## Parecer

O projeto está apto para entrega acadêmica como repositório técnico demonstrável.
Ele possui código funcional, documentação, referências, testes automatizados, CI,
análise estática, sanitizers e controle de regressão.

Para uma entrega formal em PDF, recomenda-se exportar `docs/pesquisa-abnt.md` e
complementar com capa, folha de rosto, resumo, sumário e conclusão conforme o modelo
exigido pela instituição.

## Evidências Técnicas

| Critério | Status | Evidência |
|---|---|---|
| Repositório público | Atendido | `Victorzinn704/fork-task-runner-c` |
| Código compila com GCC | Atendido | Workflow `CI` |
| Código compila com Clang | Atendido | Workflow `CI` |
| Testes automatizados | Atendido | `unit.structures`, `integration.scheduler`, `integration.cli_success` |
| Qualidade de código | Atendido | Workflow `Quality` |
| Segurança estática | Atendido | Workflow `Security` com CodeQL |
| Memória | Atendido | Valgrind e sanitizers |
| Documentação técnica | Atendido | `docs/arquitetura.md`, `docs/regras-de-negocio.md` |
| Referências | Atendido | `docs/referencias.md` |
| Guia de reprodução | Atendido | `docs/guia-de-uso-open-source.md` |
| Limitações declaradas | Atendido | `SECURITY.md` e este documento |

## O Que O Projeto Demonstra

- Criação real de processos com `fork()`.
- Execução de comandos com `exec`.
- Coleta de término com `waitpid()`.
- Escalonamento FCFS com concorrência limitada.
- Fila FIFO para tarefas pendentes.
- Lista encadeada para processos ativos.
- Pilha para eventos recentes.
- Timeout e classificação de resultado.
- CI/CD técnico com testes e análise automatizada.

## Limitações Declaradas

- Não suporta Windows nativo.
- Não executa comandos em sandbox.
- Não implementa prioridade, preempção ou memória compartilhada.
- Não substitui ferramentas reais de CI ou orquestração.

Essas limitações são coerentes com o objetivo do projeto: demonstrar processos POSIX
e estruturas de dados com baixa complexidade e alta rastreabilidade.

## Checklist Para Banca Ou Avaliador

1. Clonar o repositório.
2. Instalar dependências conforme `docs/guia-de-uso-open-source.md`.
3. Compilar com CMake.
4. Executar `ctest`.
5. Rodar `examples/tasks.sample.txt`.
6. Ler `docs/arquitetura.md`.
7. Conferir regras em `docs/regras-de-negocio.md`.
8. Conferir referências em `docs/referencias.md`.
9. Conferir workflows verdes no GitHub Actions.

## Comandos De Reprodução

```bash
git clone https://github.com/Victorzinn704/fork-task-runner-c.git
cd fork-task-runner-c
cmake -S . -B build -G Ninja -DFTR_ENABLE_STRICT=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/fork-task-runner examples/tasks.sample.txt 2 --no-dashboard
```

## Conclusão

O repositório está em condição de apresentação como projeto acadêmico e open source.
A próxima evolução recomendada é gerar um PDF acadêmico com formatação ABNT a partir
da pesquisa técnica e adicionar capturas ou logs de execução como anexos.
