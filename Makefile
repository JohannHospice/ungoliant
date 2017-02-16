CC=gcc
CFLAGS=-Wall
PROD=prod/
TEST=test/
SRC=src/
LIB=lib/
SCRIPT=script/
INCLUDE_CURL=curl-7.52.1/include/curl
INCLUDE_TIDY=tidy-html5-master/include/
LIBRARY_CURL=curl-7.52.1/src
LIBRARY_TIDY=tidy-html5-master/src
#NAMES=curl tidy
#LIBRARY=curl-7.52.1/src tidy-html5-master/src
#INCLUDES=curl-7.52.1/include/curl tidy-html5-master/include/


default:

# Executable
ungoliant: ungoliant.o
	$(CC) -o $(PROD)$@ $(PROD)$^
## Test
parser-test: parser-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^ -ltidy -L$(LIB)$(LIBRARY_TIDY) -lcurl -L$(LIB)$(LIBRARY_CURL)
crawler-test: crawler-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^
indexer-test: indexer-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^
queryengine-test: queryengine-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^

# Object
ungoliant.o: $(SRC)ungoliant.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
## Libraries
crawler.o: $(SRC)crawler/crawler.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
parser.o: $(SRC)parser/parser.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
indexer.o: $(SRC)indexer/indexer.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine.o: $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
## Test
crawler-test.o: $(TEST)crawler-test.c $(SRC)crawler/crawler.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
parser-test.o: $(TEST)parser-test.c $(SRC)parser/parser.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS) -I$(LIB)$(INCLUDE_TIDY) -I$(LIB)$(INCLUDE_CURL) 
indexer-test.o: $(TEST)indexer-test.c $(SRC)indexer/indexer-test.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine-test.o: $(TEST)queryengine-test.c $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)

# Service
init:
	mkdir $(PROD); echo "run:\n\tcd script && chmod 777 install.sh && ./install.sh && cd ../"
clean:
	rm -rf $(PROD)*.o
mrproper: clean
	rm -rf $(PROD) $(LIB)
