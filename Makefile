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

CURL_INCLUDE_DIR=$(LIB)$(CURL_DIRNAME)$(INCLUDE)curl
TIDY_INCLUDE_DIR=$(LIB)$(TIDY_DIRNAME)$(INCLUDE)
CURL_LIBRARY_DIR=$(LIB)$(CURL_DIRNAME)$(SRC)
TIDY_LIBRARY_DIR=$(LIB)$(TIDY_DIRNAME)$(SRC)

default:

# Executable
ungoliant: ungoliant.o
	$(CC) -o $(PROD)$@ $(PROD)$^
## Test
parser-test: parser-test.o
	$(CC) -o $(PROD)$@ $(PROD)$^ -l$(TIDY_NAME) -L$(TIDY_LIBRARY_DIR) -l$(CURL_NAME) -L$(CURL_LIBRARY_DIR)
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
parser.o: $(SRC)parser/parser.c $(SRC)parser/tools.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
indexer.o: $(SRC)indexer/indexer.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine.o: $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
## Test
crawler-test.o: $(TEST)crawler-test.c $(SRC)crawler/crawler.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
parser-test.o: $(TEST)parser-test.c $(SRC)parser/parser.c $(SRC)parser/tools.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS) -I$(TIDY_INCLUDE_DIR) -I$(CURL_INCLUDE_DIR) 
indexer-test.o: $(TEST)indexer-test.c $(SRC)indexer/indexer-test.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)
queryengine-test.o: $(TEST)queryengine-test.c $(SRC)queryengine/queryengine.c
	$(CC) -o $(PROD)$@ -c $< $(CFLAGS)

# Service
init: prepare-prod-dir install
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
install: install-tidy install-libcurl
install-tidy: prepare-lib-dir
	wget https://github.com/htacg/tidy-html5/archive/master.zip
	unzip master.zip -d $(LIB)
	rm master.zip
install-libcurl: prepare-lib-dir
	wget https://curl.haxx.se/download/curl-7.52.1.tar.gz
	tar zxvf curl-7.52.1.tar.gz -C $(LIB)
	rm curl-7.52.1.tar.gz
install-gumbo: prepare-lib-dir
	wget https://github.com/google/gumbo-parser/archive/master.zip
	unzip master.zip -d $(LIB)
	./$(LIB)gumbo-parser-master/autogen.sh
	./$(LIB)gumbo-parser-master/configure
	rm master.zip