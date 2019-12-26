.PHONY:all
all:catch analyse

catch:catch.c
	gcc $^ -o $@ -lpcap

analyse:analyse.c
	gcc $^ -o $@ -lpcap

.PHONY:clean
clean:
	rm catch analyse
