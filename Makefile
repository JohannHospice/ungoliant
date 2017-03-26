CC=gcc
CFLAGS=-Wall

SRC=src/
LIB=lib/
PROD=prod/
TEST=test/
SCRIPT=script/
INCLUDE=include/

TIDY_NAME=tidy
CURL_NAME=curl

TIDY_DIRNAME=tidy-html5-master/
CURL_DIRNAME=curl-7.52.1/

CURL_INCLUDE_DIR=$(LIB)$(CURL_DIRNAME)$(INCLUDE)
TIDY_INCLUDE_DIR=$(LIB)$(TIDY_DIRNAME)$(INCLUDE)

CURL_LIBRARY_DIR=$(LIB)$(CURL_DIRNAME)$(SRC)
TIDY_LIBRARY_DIR=$(LIB)$(TIDY_DIRNAME)$(SRC)

default:

# Executable
ungoliant: ungoliant.o parser.o
	$(CC) -o $(PROD)$@ $(PROD)$^
## Test
parser-test: parser-test.o parser.o tools.o
	$(CC) -o $(PROD)$@ $(foreach file, $^, $(PROD)$(file))\
		-l$(TIDY_NAME) -L$(TIDY_LIBRARY_DIR) -l$(CURL_NAME) -L$(CURL_LIBRARY_DIR)
crawler-test: crawler-test.o crawler.o parser.o tools.o webpage.o parseurl.o
	$(CC) -o $(PROD)$@ $(foreach file, $^, $(PROD)$(file))\
		-l$(TIDY_NAME) -L$(TIDY_LIBRARY_DIR) -l$(CURL_NAME) -L$(CURL_LIBRARY_DIR)
indexer-test: indexer-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^
queryengine-test: queryengine-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^

# Object
ungoliant.o: $(SRC)ungoliant.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
## Libraries
### Crawler
crawler.o: $(SRC)crawler/crawler.c $(SRC)parser/parser.h $(SRC)tools.h $(SRC)crawler/webpage.h $(SRC)crawler/parseurl.h
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)\
		-I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR)
### Parser
parser.o: $(SRC)parser/parser.c $(SRC)tools.h
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)\
		-I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR)
tools.o: $(SRC)tools.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)\
		-I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR)
webpage.o: $(SRC)crawler/webpage.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)\
		-I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR)
parseurl.o: $(SRC)crawler/parseurl.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)\
		-I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR)
### Indexer
indexer.o: $(SRC)indexer/indexer.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
### QueryEngine
queryengine.o: $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
## Test
crawler-test.o: $(TEST)crawler-test.c $(SRC)crawler/crawler.h
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)\
		-I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR)
parser-test.o: $(TEST)parser-test.c $(SRC)parser/parser.h
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)\
		-I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR)
indexer-test.o: $(TEST)indexer-test.c $(SRC)indexer/indexer-test.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine-test.o: $(TEST)queryengine-test.c $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)

# Service
init: prepare-prod-dir prepare-lib-dir install
clean:
	rm -rf $(PROD)*.o
mrproper: clean
	rm -rf $(PROD) $(LIB)
## Prepare
prepare-lib-dir:
	test -d $(LIB) || mkdir $(LIB)
prepare-prod-dir:
	test -d $(PROD) || mkdir $(PROD)
## Installer
install: install-tidy install-libcurl install-libsoup
install-tidy: prepare-lib-dir
	wget https://github.com/htacg/tidy-html5/archive/master.zip
	unzip master.zip -d $(LIB)
	rm master.zip
install-libcurl: prepare-lib-dir
	wget https://curl.haxx.se/download/curl-7.52.1.tar.gz
	tar zxvf curl-7.52.1.tar.gz -C $(LIB)
	rm curl-7.52.1.tar.gz