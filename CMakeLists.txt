cmake_minimum_required(VERSION 3.20)

project(
  fork_task_runner
  VERSION 0.1.0
  DESCRIPTION "Fork Task Runner: escalonador multiprocessos em C"
  LANGUAGES C)

if(NOT UNIX)
  message(FATAL_ERROR "fork-task-runner-c requer ambiente POSIX. Use Linux, macOS ou WSL 2.")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
set(FTR_INSTALL_DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/fork-task-runner")

option(FTR_ENABLE_STRICT "Enable strict compiler warnings" ON)
option(FTR_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

add_library(
  ftr_core
  src/domain/task.c
  src/structures/task_queue.c
  src/structures/process_list.c
  src/structures/event_stack.c
  src/infrastructure/process_runner.c
  src/infrastructure/task_loader.c
  src/application/scheduler.c
  src/presentation/terminal_dashboard.c)

target_include_directories(ftr_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(ftr_core PUBLIC _POSIX_C_SOURCE=200809L)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  if(FTR_ENABLE_STRICT)
    target_compile_options(ftr_core PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion -Wshadow)
  endif()

  if(FTR_ENABLE_SANITIZERS)
    target_compile_options(ftr_core PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(ftr_core PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
  endif()
endif()

add_executable(fork-task-runner src/main.c)
target_link_libraries(fork-task-runner PRIVATE ftr_core)
target_compile_definitions(fork-task-runner PRIVATE FTR_VERSION="${PROJECT_VERSION}")

if(FTR_ENABLE_STRICT AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(fork-task-runner PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion -Wshadow)
endif()

install(TARGETS fork-task-runner RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES README.md LICENSE CHANGELOG.md DESTINATION ${FTR_INSTALL_DOCDIR})
install(DIRECTORY docs/ DESTINATION ${FTR_INSTALL_DOCDIR}/docs)
install(DIRECTORY examples/ DESTINATION ${FTR_INSTALL_DOCDIR}/examples)

include(CTest)

if(BUILD_TESTING)
  add_executable(test_structures tests/unit/test_structures.c)
  target_link_libraries(test_structures PRIVATE ftr_core)
  target_include_directories(test_structures PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_test(NAME unit.structures COMMAND test_structures)

  add_executable(test_task_loader tests/unit/test_task_loader.c)
  target_link_libraries(test_task_loader PRIVATE ftr_core)
  target_include_directories(test_task_loader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_definitions(
    test_task_loader
    PRIVATE FTR_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures")
  add_test(NAME unit.task_loader COMMAND test_task_loader)

  add_executable(test_scheduler tests/integration/test_scheduler.c)
  target_link_libraries(test_scheduler PRIVATE ftr_core)
  target_include_directories(test_scheduler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_test(NAME integration.scheduler COMMAND test_scheduler)

  add_test(NAME integration.cli_help COMMAND fork-task-runner --help)
  set_tests_properties(integration.cli_help PROPERTIES PASS_REGULAR_EXPRESSION "Uso:")

  add_test(NAME integration.cli_version COMMAND fork-task-runner --version)
  set_tests_properties(integration.cli_version PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

  add_test(NAME integration.cli_invalid_args COMMAND fork-task-runner)
  set_tests_properties(integration.cli_invalid_args PROPERTIES WILL_FAIL TRUE)

  add_test(
    NAME integration.cli_success
    COMMAND fork-task-runner
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/tasks-success.txt
            2
            --no-dashboard)

  add_test(
    NAME integration.cli_invalid_file
    COMMAND fork-task-runner
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/tasks-invalid-timeout.txt
            1
            --no-dashboard)
  set_tests_properties(integration.cli_invalid_file PROPERTIES WILL_FAIL TRUE)
endif()
