.PHONY: configure build test sanitize clean code-health pdf

BUILD_DIR ?= build

configure:
	cmake -S . -B $(BUILD_DIR) -DFTR_ENABLE_STRICT=ON

build: configure
	cmake --build $(BUILD_DIR)

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

sanitize:
	cmake -S . -B build-sanitize -DFTR_ENABLE_SANITIZERS=ON -DCMAKE_BUILD_TYPE=Debug
	cmake --build build-sanitize
	ctest --test-dir build-sanitize --output-on-failure

code-health:
	bash scripts/check-code-health.sh

pdf:
	wkhtmltopdf --encoding utf-8 --enable-local-file-access \
		docs/academico/fork-task-runner-abnt.html \
		docs/dist/fork-task-runner-abnt.pdf

clean:
	rm -rf build build-sanitize
