# Fork Task Runner C: quick start

[Português](../README.md) · [MIT license](../LICENSE)

A small C11 project for learning POSIX process management through a working task
runner. A FIFO queue holds pending tasks, a linked list tracks active child
processes, and a stack stores recent scheduler events. You choose how many child
processes may run at once; `fork`, `exec` and `waitpid` perform the actual work.

## Requirements

Use Linux, macOS, or an Ubuntu terminal in WSL 2 on Windows. Native Windows is not
supported because the runner uses POSIX processes and signals. You need Git,
CMake 3.20 or newer, GCC or Clang, and a build tool such as Make.

On Ubuntu, Debian, or Ubuntu in WSL 2:

```bash
sudo apt-get update
sudo apt-get install -y git cmake gcc make
```

On macOS with Homebrew:

```bash
brew install git cmake
```

The macOS build also needs Apple's command-line developer tools (`xcode-select
--install` if they are not already installed).

## Build and run

Run these commands in your POSIX terminal:

```bash
git clone https://github.com/Victorzinn704/fork-task-runner-c.git
cd fork-task-runner-c
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DFTR_ENABLE_STRICT=ON
cmake --build build
./build/fork-task-runner examples/tasks.sample.txt 2 --no-dashboard
```

`2` limits concurrency to two child processes. `--no-dashboard` disables the ANSI
dashboard; commands still print their own output. The final report includes these
counts; elapsed time is reported separately and varies:

```text
Total tasks: 4
Started:     4
Finished:    4
Failed:      0
Timeout:     0
```

Run from the repository root: one sample task reads `README.md` using a relative
path. Remove `--no-dashboard` to see the live terminal dashboard.

## Change the workload

A task file has one task per line: `id|name|timeout_seconds|command`. Lines starting
with `#` are comments. The command is the final field, so it may contain shell
pipelines. For example, save this as `my-tasks.txt`:

```text
1|Print a message|5|printf 'hello\n'
2|Wait briefly|3|sleep 1
3|Count words|5|printf 'queue list stack\n' | wc -w
```

```bash
./build/fork-task-runner my-tasks.txt 2 --no-dashboard
```

Task commands run through `/bin/sh -c` with your user permissions. Use task files
you trust; child processes are not a security sandbox.

## Check success, failure and timeout

```bash
ctest --test-dir build --output-on-failure
./build/fork-task-runner examples/tasks.failure-timeout.txt 2 --no-dashboard
echo "exit=$?"
```

CTest runs the automated test suite. The second command deliberately runs one
successful task, one failing task, and one task that exceeds its timeout; expect
`Finished: 1`, `Failed: 1`, `Timeout: 1`, and `exit=1`. This nonzero exit is the
expected result of that demonstration.

| Exit code | Meaning |
|---|---|
| `0` | All tasks succeeded. |
| `1` | A task failed or timed out, or the task file could not be loaded. |
| `2` | Invalid command-line arguments. |

`make test` configures, builds and runs CTest; it does not run the interactive
sample. Use `./build/fork-task-runner --help` for CLI options and `--version` for
the version. CLI help and diagnostic messages are currently in Portuguese.

## Read the implementation

Start with [src/main.c](../src/main.c), then follow the
[scheduler](../src/application/scheduler.c),
[process runner](../src/infrastructure/process_runner.c), and
[data structures](../src/structures/).
[Unit and integration tests](../tests/) show the expected behavior.

The [documentation index](INDEX.md), [architecture](arquitetura.md), and
[contribution guide](../CONTRIBUTING.md) provide more detail in Portuguese.
The project is MIT licensed; see [LICENSE](../LICENSE).
