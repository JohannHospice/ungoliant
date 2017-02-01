CC=gcc
CFLAGS=-Wall
PROD=prod/
SRC=src/

default:

# Executable
ungoliant: ungoliant.o
	$(CC) -o $(PROD)$@ $(PROD)$^
crawler-test: crawler-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^

# Object
ungoliant.o: $(SRC)ungoliant.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
crawler-test.o: $(SRC)crawler/crawler-test.c $(SRC)crawler/crawler.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)

crawler.o: $(SRC)crawler/crawler.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
index.o: $(SRC)index/index.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
parser.o: $(SRC)parser/parser.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine.o: $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)


# Service
init:
	mkdir $(PROD)
clean:
	rm -rf $(PROD)*.o
mrproper: clean
	rm -rf $(PROD)
