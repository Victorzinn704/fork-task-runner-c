# Regras De Negócio

## Escopo

O sistema executa tarefas locais em processos filhos, com ordem previsível,
concorrência limitada e coleta explícita de resultado.

## Regras

| ID | Regra |
|---|---|
| RN-001 | Toda tarefa válida deve possuir `id`, `nome`, `timeout_segundos` e `comando`. |
| RN-002 | A fila de tarefas pendentes segue ordem FIFO. |
| RN-003 | O scheduler não pode ultrapassar `max_parallel`. |
| RN-004 | O processo pai é a única fonte de verdade da fila, lista e pilha. |
| RN-005 | O processo filho executa apenas o comando recebido. |
| RN-006 | Toda tarefa iniciada deve terminar como `FINISHED`, `FAILED` ou `TIMEOUT`. |
| RN-007 | Todo processo filho deve ser coletado com `waitpid()`. |
| RN-008 | Tarefa que excede timeout deve ser encerrada e registrada como `TIMEOUT`. |
| RN-009 | O painel visual não altera estado de negócio. |
| RN-010 | A CLI deve retornar código diferente de zero se houver falha ou timeout. |

## Estados

```txt
QUEUED -> RUNNING -> FINISHED
QUEUED -> RUNNING -> FAILED
QUEUED -> RUNNING -> TIMEOUT
```

## Entradas

Arquivo texto:

```txt
id|nome|timeout_segundos|comando
```

O comando fica no último campo para permitir pipelines shell com `|`.

## Saídas

O runner produz:

- painel terminal com fila, processos ativos, eventos e resumo;
- relatório final;
- exit code de processo para integração com CI.
