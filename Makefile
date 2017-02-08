CC=gcc
CFLAGS=-Wall
PROD=prod/
SRC=src/

default:

# Executable
ungoliant: ungoliant.o
	$(CC) -o $(PROD)$@ $(PROD)$^
## Test
crawler-test: crawler-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^
parser-test: parser-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^
queryengine-test: queryengine-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^
indexer-test: indexer-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^

# Object
ungoliant.o: $(SRC)ungoliant.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
## Libraries
crawler.o: $(SRC)crawler/crawler.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
indexer.o: $(SRC)indexer/indexer.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
parser.o: $(SRC)parser/parser.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine.o: $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
## Test
indexer-test.o: $(SRC)indexer/indexer-test.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
crawler-test.o: $(SRC)crawler/crawler-test.c $(SRC)crawler/crawler.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
parser-test.o: $(SRC)parser/parser-test.c $(SRC)parser/parser.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine-test.o: $(SRC)queryengine/queryengine-test.c $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)

# Service
init:
	mkdir $(PROD)
clean:
	rm -rf $(PROD)*.o
mrproper: clean
	rm -rf $(PROD)
