#include <tidybuffio.h>

/* curl write callback, to fill tidy's input buffer...  */ 
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out){
  uint r;
  r = size * nmemb;
  tidyBufAppend(out, in, r);
  return r;
}