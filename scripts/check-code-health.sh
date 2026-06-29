#!/usr/bin/env bash
set -euo pipefail

max_lines=300
status=0

while IFS= read -r file; do
  lines="$(wc -l < "$file" | tr -d ' ')"
  if [ "$lines" -gt "$max_lines" ]; then
    echo "code-health: $file has $lines lines; limit is $max_lines"
    status=1
  fi
done < <(find src include tests -type f \( -name '*.c' -o -name '*.h' \) | sort)

exit "$status"
