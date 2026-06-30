#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build-audit"
INSTALL_DIR="${ROOT_DIR}/.cache/audit-install"
EVIDENCE_DIR="${ROOT_DIR}/docs/auditoria-funcionamento/evidencias"

rm -rf "${BUILD_DIR}" "${INSTALL_DIR}"
mkdir -p "${EVIDENCE_DIR}"

run_case() {
  local name="$1"
  shift

  local outfile="${EVIDENCE_DIR}/${name}.txt"
  {
    printf 'Caso: %s\n' "${name}"
    printf 'Data: %s\n' "$(date -Iseconds)"
    printf 'Diretorio: %s\n' "${ROOT_DIR}"
    printf 'Comando: %s\n\n' "$*"

    set +e
    "$@"
    local code=$?
    set -e

    printf '\nExit code: %s\n' "${code}"
  } >"${outfile}" 2>&1
}

run_case 01-build-test bash -lc \
  "cmake -S '${ROOT_DIR}' -B '${BUILD_DIR}' -G Ninja -DFTR_ENABLE_STRICT=ON -DCMAKE_BUILD_TYPE=Debug && cmake --build '${BUILD_DIR}' && ctest --test-dir '${BUILD_DIR}' --output-on-failure"

run_case 02-cli-help "${BUILD_DIR}/fork-task-runner" --help

run_case 03-cli-version "${BUILD_DIR}/fork-task-runner" --version

run_case 04-success-run \
  "${BUILD_DIR}/fork-task-runner" "${ROOT_DIR}/examples/tasks.sample.txt" 2 --no-dashboard

run_case 05-failure-timeout-run \
  "${BUILD_DIR}/fork-task-runner" "${ROOT_DIR}/examples/tasks.failure-timeout.txt" 2 --no-dashboard

run_case 06-invalid-file \
  "${BUILD_DIR}/fork-task-runner" "${ROOT_DIR}/tests/fixtures/tasks-invalid-timeout.txt" 1 --no-dashboard

run_case 07-install bash -lc \
  "cmake --install '${BUILD_DIR}' --prefix '${INSTALL_DIR}' && '${INSTALL_DIR}/bin/fork-task-runner' --version && test -f '${INSTALL_DIR}/share/doc/fork-task-runner/docs/dist/fork-task-runner-abnt.pdf'"

printf 'Evidencias geradas em: %s\n' "${EVIDENCE_DIR}"
