# Fork Task Runner C

Executor multiprocessos em C para demonstrar `fork()` em um processo real, usando fila,
lista encadeada e pilha com regras de negócio claras, testes automatizados e pipelines
de qualidade.

O projeto usa um algoritmo **FCFS com concorrência limitada**: tarefas entram em uma
fila FIFO, o processo pai cria filhos com `fork()`, a lista encadeada acompanha os
processos ativos e a pilha mantém os eventos recentes exibidos no terminal.

## Objetivo

Demonstrar, de forma profissional e auditável, como estruturas de dados clássicas
podem coordenar processos reais em C:

- **Fila:** tarefas pendentes em ordem de chegada.
- **Lista encadeada:** processos filhos ativos.
- **Pilha:** eventos recentes do scheduler.
- **fork/exec:** criação e execução real de processos.
- **waitpid:** coleta de término e prevenção de processos zumbi.
- **timeout:** encerramento controlado de tarefas demoradas.

## Requisitos

- Linux, macOS ou WSL 2.
- CMake 3.20 ou superior.
- GCC ou Clang.
- Ninja opcional.

`fork()` é uma chamada POSIX; por isso, o projeto não foi desenhado para Windows
nativo.

## Execução Rápida

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/fork-task-runner examples/tasks.sample.txt 2
```

Atalho equivalente:

```bash
make test
```

Execução sem painel ANSI, útil para CI:

```bash
./build/fork-task-runner examples/tasks.sample.txt 2 --no-dashboard
```

Formato do arquivo de tarefas:

```txt
id|nome|timeout_segundos|comando
1|Listar arquivos|5|ls -la
2|Simular processamento|4|sleep 2
3|Pipeline POSIX|5|printf 'fork\nqueue\nstack\n' | wc -l
```

## Testes

```bash
cmake -S . -B build -DFTR_ENABLE_STRICT=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Com sanitizers:

```bash
cmake -S . -B build-sanitize -DFTR_ENABLE_SANITIZERS=ON
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

## Pipelines

O repositório possui workflows preparados para GitHub Actions:

- `ci.yml`: build com GCC/Clang, testes e smoke da CLI.
- `quality.yml`: `clang-format`, `clang-tidy`, `cppcheck`, sanitizers e code health.
- `security.yml`: CodeQL e Valgrind.
- `docs.yml`: lint de documentação Markdown.

## Documentação

- [Índice](docs/INDEX.md)
- [Arquitetura](docs/arquitetura.md)
- [Regras de negócio](docs/regras-de-negocio.md)
- [Estratégia de testes](docs/estrategia-de-testes.md)
- [Controle de regressão](docs/controle-de-regressao.md)
- [Pesquisa em formato ABNT](docs/pesquisa-abnt.md)
- [Referências](docs/referencias.md)
- [Radar de repositórios](docs/research/repository-radar.md)

## Síntese Técnica

O pai é a única fonte de verdade das estruturas de dados. Os filhos executam comandos
isolados e devolvem estado por término de processo. Essa decisão mantém o projeto
didático, real e seguro contra complexidade desnecessária de memória compartilhada.

## Licença

MIT. Consulte [LICENSE](LICENSE).
