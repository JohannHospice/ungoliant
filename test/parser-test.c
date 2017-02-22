#include "../src/parser/parser.h"
#include <curl.h>
int main(int argc, char **argv){
  char *url = "https://en.wikipedia.org/wiki/Artificial_neural_network";
  CURL* curl = download(url);
  parser(curl);
  return 0;
}