.PHONY: build
build: setup
	docker build -t denise .

.PHONY: test
test:
	cd par && ./test.sh

.PHONY: setup
setup:
	git submodule init
