# Controle De Regressão

## Matriz Principal

| ID | Risco | Teste/Gate |
|---|---|---|
| REG-001 | FIFO quebrado | `unit.structures` |
| REG-002 | Pilha exibindo evento antigo primeiro | `unit.structures` |
| REG-003 | Processo ativo não removido | `unit.structures` e `integration.scheduler` |
| REG-004 | Filho não coletado com `waitpid()` | `integration.scheduler` |
| REG-005 | Timeout sem encerramento | `integration.scheduler` |
| REG-006 | Falha de comando tratada como sucesso | `integration.scheduler` |
| REG-007 | CLI quebrada | `integration.cli_success` |
| REG-008 | Vazamento em estruturas | `security.yml` com Valgrind |
| REG-009 | Comportamento indefinido | `quality.yml` com sanitizers |
| REG-010 | Código fora do padrão | `quality.yml` com clang-format/tidy |

## Política

Toda regressão corrigida deve ganhar um teste que falharia antes da correção.

O objetivo não é aumentar cobertura numérica sem critério. O objetivo é proteger
as regras que definem o comportamento do scheduler.

## Evidência Esperada Em PR

```txt
Regra impactada:
Teste adicionado/alterado:
Comando executado:
Resultado:
Risco residual:
```
