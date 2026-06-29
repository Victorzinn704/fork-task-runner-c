# Contribuição

Este repositório segue uma regra simples: toda mudança deve preservar clareza,
testabilidade e rastreabilidade.

## Antes de Alterar Código

- Leia `docs/arquitetura.md`.
- Verifique as regras de negócio em `docs/regras-de-negocio.md`.
- Defina qual teste será adicionado ou ajustado.
- Evite arquivos acima de 300 linhas e funções acima de 70 linhas.

## Gates Locais

```bash
cmake -S . -B build -DFTR_ENABLE_STRICT=ON
cmake --build build
ctest --test-dir build --output-on-failure
bash scripts/check-code-health.sh
```

## Padrão de PR

O PR deve informar:

- problema resolvido;
- arquivos principais alterados;
- regra de negócio impactada;
- testes executados;
- risco residual, se houver.
