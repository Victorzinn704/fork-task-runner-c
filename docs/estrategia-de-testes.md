# Estratégia De Testes

## Objetivo

Os testes funcionam como documentação executável das regras de negócio.

## Camadas

| Camada | Objetivo |
|---|---|
| Unitário | Validar fila, pilha e lista sem depender de processos reais. |
| Integração | Validar `fork()`, `waitpid()`, falha e timeout. |
| Smoke CLI | Garantir que o binário executa arquivo real de tarefas. |
| Sanitizers | Capturar uso indevido de memória e comportamento indefinido. |
| Valgrind | Detectar vazamentos em cenários críticos. |

## Casos Automatizados

| Caso | Cobertura |
|---|---|
| `unit.structures` | FIFO, LIFO e remoção por PID. |
| `integration.scheduler` | sucesso, falha e timeout em processos reais. |
| `integration.cli_success` | execução de fixture real pela CLI. |

## Comandos

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

## Critério De Aceitação

Uma alteração que mexe em regra de negócio precisa ter:

- um caso feliz;
- um caso de erro;
- um caso de regressão se o bug já ocorreu;
- atualização documental quando mudar comportamento.
