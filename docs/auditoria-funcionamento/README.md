# Auditoria De Funcionamento

Esta auditoria registra uma execução real do Fork Task Runner C em ambiente POSIX,
com evidências brutas e screenshots de terminal.

## Ambiente Auditado

| Item | Valor |
| --- | --- |
| Data da execução | 2026-06-29 |
| Ambiente | WSL 2 com Ubuntu 22.04 |
| Compilador observado | GCC 11.4.0 |
| Build | CMake + Ninja |
| Diretório de build | `build-audit` |
| Modo da CLI | `--no-dashboard` para saída estável em auditoria |

O painel ANSI interativo não foi usado nos screenshots porque ele atualiza a tela
dinamicamente. A auditoria usa a saída final estável para permitir comparação,
registro e revisão posterior.

## Como Reproduzir

Execute na raiz do repositório em Linux, macOS ou WSL 2:

```bash
bash scripts/run-functional-audit.sh
```

O script recria `build-audit`, executa os cenários abaixo e grava as evidências em
`docs/auditoria-funcionamento/evidencias/`.

## Resultado Executivo

| Área | Resultado | Evidência |
| --- | --- | --- |
| Build limpo | Aprovado | `cmake`, build Ninja e CTest concluídos |
| Testes automatizados | Aprovado | 8/8 testes passaram |
| CLI de ajuda | Aprovado | `--help` retorna uso e opções |
| CLI de versão | Aprovado | `--version` retorna `fork-task-runner 0.1.0` |
| Execução de sucesso | Aprovado | 4 tarefas iniciadas, 4 concluídas, 0 falhas |
| Falha e timeout | Aprovado | 1 tarefa falhou, 1 tarefa expirou, exit code 1 |
| Arquivo inválido | Aprovado | loader rejeitou entrada inválida, exit code 1 |
| Instalação CMake | Aprovado | binário, docs e PDF instalados no prefixo local |

## Evidências E Screenshots

| Caso | O que valida | Log bruto | Screenshot |
| --- | --- | --- | --- |
| 01 | Build, linkagem e CTest | [01-build-test.txt](evidencias/01-build-test.txt) | [01-build-test.png](screenshots/01-build-test.png) |
| 02 | Ajuda da CLI | [02-cli-help.txt](evidencias/02-cli-help.txt) | [02-cli-help.png](screenshots/02-cli-help.png) |
| 03 | Versão da CLI | [03-cli-version.txt](evidencias/03-cli-version.txt) | [03-cli-version.png](screenshots/03-cli-version.png) |
| 04 | Execução real com sucesso | [04-success-run.txt](evidencias/04-success-run.txt) | [04-success-run.png](screenshots/04-success-run.png) |
| 05 | Classificação de falha e timeout | [05-failure-timeout-run.txt](evidencias/05-failure-timeout-run.txt) | [05-failure-timeout-run.png](screenshots/05-failure-timeout-run.png) |
| 06 | Rejeição de arquivo inválido | [06-invalid-file.txt](evidencias/06-invalid-file.txt) | [06-invalid-file.png](screenshots/06-invalid-file.png) |
| 07 | Instalação local com CMake | [07-install.txt](evidencias/07-install.txt) | [07-install.png](screenshots/07-install.png) |

## O Que Realmente Funciona

- O projeto compila do zero em ambiente POSIX com CMake e Ninja.
- Os testes unitários e de integração passam no build auditado.
- O binário aceita `--help` e `--version`.
- O executor cria processos reais, executa comandos e consolida o relatório final.
- O cenário de sucesso finaliza todas as tarefas sem falha nem timeout.
- O cenário negativo diferencia falha de comando e timeout.
- O loader rejeita arquivo com timeout inválido antes de executar tarefas.
- A instalação local via CMake coloca o binário e a documentação no prefixo esperado.

## Limites Da Auditoria

- Windows nativo não foi auditado porque `fork()` é POSIX e o projeto declara suporte
  para Linux, macOS e WSL 2.
- A auditoria local foi executada em WSL 2; a cobertura em GitHub Actions complementa
  isso com Ubuntu e macOS.
- Os screenshots registram a saída final estável. O painel ANSI dinâmico é funcional
  para uso manual, mas não é o melhor formato para evidência versionada.

## Conclusão

O repositório está funcional para entrega acadêmica e uso open source no escopo
declarado: pesquisa aplicada sobre `fork()`, implementação real em C, algoritmo FCFS
com concorrência limitada, estruturas de dados clássicas, testes, CI e documentação.
