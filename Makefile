.PHONY: release, test, run, build_scanner, build_rust

test:
	cargo test

build_scanner:
	cd cpp && make

build_rust:
	cargo build --release

run: build_scanner
	cp cpp/bin/scanner .
	cargo run

release: build_scanner
	rm -rf bin
	mkdir bin
	cp cpp/bin/scanner bin/
	cargo build --release
	cp target/release/scanner_bridge bin/
	strip bin/scanner_bridge