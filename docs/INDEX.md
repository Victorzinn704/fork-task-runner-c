# Índice Da Documentação

Este diretório concentra a documentação canônica do Fork Task Runner C.

## Leitura Recomendada

1. [Arquitetura](arquitetura.md)
2. [Regras de negócio](regras-de-negocio.md)
3. [Estratégia de testes](estrategia-de-testes.md)
4. [Controle de regressão](controle-de-regressao.md)
5. [Guia de uso open source](guia-de-uso-open-source.md)
6. [Auditoria de funcionamento](auditoria-funcionamento/README.md)
7. [Pesquisa em formato ABNT](pesquisa-abnt.md)
8. [Auditoria de prontidão acadêmica](auditoria-prontidao-academica.md)
9. [PDF acadêmico ABNT](dist/fork-task-runner-abnt.pdf)
10. [Referências](referencias.md)
11. [Radar de repositórios](research/repository-radar.md)
12. [Publicação e redes](publicacao-e-redes.md)

## Critério De Pronto

Uma mudança só é considerada pronta quando:

- compila com GCC ou Clang;
- passa nos testes automatizados;
- preserva as regras de negócio;
- não aumenta complexidade sem justificativa;
- atualiza documentação quando altera comportamento;
- mantém rastreabilidade entre regra, código e teste.
