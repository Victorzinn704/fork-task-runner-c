# Documento Acadêmico

Este diretório contém a fonte HTML do PDF acadêmico em estilo ABNT.

## Gerar PDF

Instale o conversor:

```bash
sudo apt-get install -y wkhtmltopdf fonts-liberation
```

Gere o PDF:

```bash
make pdf
```

Saída esperada:

```txt
docs/dist/fork-task-runner-abnt.pdf
```

O HTML usa Arial como fonte principal e Liberation Sans como fallback para ambientes
Linux. Ajuste capa, instituição e dados autorais conforme o modelo exigido pela sua
instituição.
