
CFLAGS := -std=gnu99 -I. -I./x264 -I./x264/common -I./x264_config

main: src
	gcc src/main.c -o dist/adder $(CFLAGS)

clean:
	rm -rf build/* dist/*
