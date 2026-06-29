# Segurança

Este projeto executa comandos locais informados em arquivo de tarefas. Ele deve ser
usado apenas com arquivos confiáveis.

## Modelo de Ameaça

- O runner não faz sandbox de comandos.
- O comando é executado por `/bin/sh -c`.
- O timeout encerra o grupo de processo do filho, mas não substitui isolamento de
  sistema operacional.
- Não use arquivos de tarefas recebidos de fontes não confiáveis.

## Recomendações

- Execute em ambiente de laboratório, container ou VM quando os comandos não forem
  totalmente controlados.
- Não rode como `root`.
- Revise tarefas antes de executar.
- Use CodeQL, Valgrind e sanitizers nos fluxos de CI.
