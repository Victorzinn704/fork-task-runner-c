# Radar De Repositórios

## Objetivo

Este radar registra referências usadas para calibrar boas práticas sem importar
complexidade desnecessária.

## Referências Observadas

| Repositório | Prática Observada | Adaptação No Projeto |
|---|---|---|
| `redis/redis` | Uso real de `fork()` para trabalho em background e copy-on-write. | Documentar por que `fork()` é útil em execução paralela por processo. |
| `libuv/libuv` | Workflows separados para CI Unix/Windows e sanitizers. | Separar CI, qualidade e segurança. |
| `curl/curl` | Gates específicos para source check, docs, CodeQL e múltiplas plataformas. | Criar pipelines pequenos, focados e legíveis. |
| `git/git` | Checks simples de estilo e whitespace junto ao CI principal. | Manter gates de estilo automatizados. |
| Desk Imperial | Documentação canônica, testes como evidência e code health budget. | Trazer índice, regras, estratégia de testes e controle de regressão. |

## Decisão

O projeto não replica a arquitetura interna dessas bases. Ele adapta o processo de
engenharia: documentação rastreável, testes, análise estática, sanitizers, segurança
e separação clara de responsabilidades.
