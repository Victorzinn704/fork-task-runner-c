# Guia De Uso Open Source

Este guia mostra como clonar, compilar, testar e executar o Fork Task Runner C em
um computador comum.

## 1. Escolha O Ambiente

Use um ambiente POSIX:

- Linux;
- macOS;
- Windows com WSL 2.

Windows nativo não é suportado porque o projeto usa `fork()`, `exec`, `waitpid()` e
sinais POSIX.

## 2. Instale As Dependências

Ubuntu, Debian ou WSL 2:

```bash
sudo apt-get update
sudo apt-get install -y git cmake gcc clang make ninja-build
```

Ferramentas de auditoria:

```bash
sudo apt-get install -y clang-format clang-tidy cppcheck valgrind
```

macOS com Homebrew:

```bash
brew install git cmake ninja llvm cppcheck
```

## 3. Clone O Repositório

```bash
git clone https://github.com/Victorzinn704/fork-task-runner-c.git
cd fork-task-runner-c
```

## 4. Compile

Com CMake padrão:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Com Ninja:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Com warnings rígidos:

```bash
cmake -S . -B build -G Ninja -DFTR_ENABLE_STRICT=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## 5. Execute O Exemplo

```bash
./build/fork-task-runner examples/tasks.sample.txt 2
```

O número `2` define o máximo de processos filhos executando em paralelo.

Para uma saída estável em terminal simples ou CI:

```bash
./build/fork-task-runner examples/tasks.sample.txt 2 --no-dashboard
```

Resultado esperado:

```txt
================== FINAL REPORT ==================
Total tasks: 4
Started:     4
Finished:    4
Failed:      0
Timeout:     0
==================================================
```

Ajuda e versão:

```bash
./build/fork-task-runner --help
./build/fork-task-runner --version
```

Instalação local opcional:

```bash
cmake --install build --prefix ~/.local
~/.local/bin/fork-task-runner --version
```

## 6. Crie Seu Arquivo De Tarefas

Crie `minhas-tarefas.txt`:

```txt
1|Listar arquivos|5|ls -la
2|Simular processamento|4|sleep 2
3|Contar linhas|5|wc -l README.md
4|Pipeline POSIX|5|printf 'fork\nfila\npilha\n' | wc -l
```

Execute:

```bash
./build/fork-task-runner minhas-tarefas.txt 3
```

Para observar falha e timeout:

```bash
./build/fork-task-runner examples/tasks.failure-timeout.txt 2 --no-dashboard
```

Formato:

```txt
id|nome|timeout_segundos|comando
```

O comando fica no último campo para permitir pipelines com `|`.

## 7. Rode Os Testes

```bash
ctest --test-dir build --output-on-failure
```

Atalho:

```bash
make test
```

## 8. Rode Auditoria Local

Formatação:

```bash
find src include tests -type f \( -name '*.c' -o -name '*.h' \) -print0 | \
  xargs -0 clang-format --dry-run --Werror
```

Code health:

```bash
bash scripts/check-code-health.sh
```

Análise estática:

```bash
find src -type f -name '*.c' -print0 | xargs -0 clang-tidy -p build
cppcheck --enable=warning,style,performance,portability --error-exitcode=1 \
  --std=c11 --suppress=missingIncludeSystem -I include src include
```

Sanitizers:

```bash
cmake -S . -B build-sanitize -G Ninja -DFTR_ENABLE_SANITIZERS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

Valgrind:

```bash
valgrind --leak-check=full --error-exitcode=1 ./build/test_structures
```

## 9. Entenda Os Códigos De Saída

| Código | Significado |
|---|---|
| `0` | Todas as tarefas terminaram com sucesso. |
| `1` | Pelo menos uma tarefa falhou ou atingiu timeout. |
| `2` | Uso inválido da CLI. |

## 10. Problemas Comuns

| Problema | Causa provável | Como resolver |
|---|---|---|
| `fork-task-runner requer ambiente POSIX` | Execução em Windows nativo. | Use Linux, macOS ou WSL 2. |
| `cmake: command not found` | CMake não instalado. | Instale as dependências da seção 2. |
| `No such file or directory` | Caminho do arquivo de tarefas errado. | Execute a partir da raiz do repositório ou use caminho absoluto. |
| Tarefa termina como `TIMEOUT` | Comando excedeu o limite configurado. | Aumente `timeout_segundos` no arquivo. |
| Tarefa falha com exit diferente de zero | O comando executado falhou. | Teste o comando diretamente no terminal. |

## 11. Aviso De Segurança

O projeto executa comandos via `/bin/sh -c`. Use apenas arquivos de tarefas confiáveis.
Não execute como `root` e prefira ambiente de laboratório quando estiver testando
comandos de terceiros.
