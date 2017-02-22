#include <stdio.h>
#include <tidy.h>
#include <tidybuffio.h>
#include <curl.h>
#include "tools.h"

/* Traverse the document tree */ 
void dumpNode(TidyDoc doc, TidyNode tnod, int indent)
{
  TidyNode child;
  for(child = tidyGetChild(tnod); child; child = tidyGetNext(child) ) {
    ctmbstr name = tidyNodeGetName(child);
    if(name) {
      /* if it has a name, then it's an HTML tag ... */ 
      TidyAttr attr;
      printf("%*.*s%s ", indent, indent, "<", name);
      /* walk the attribute list */ 
      for(attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
        printf("%s",tidyAttrName(attr));
        tidyAttrValue(attr)?printf("=\"%s\" ",
                                   tidyAttrValue(attr)):printf(" ");
      }
      printf(">\n");
    }
    else {
      /* if it doesn't have a name, then it's probably text, cdata, etc... */ 
      TidyBuffer buf;
      tidyBufInit(&buf);
      tidyNodeGetText(doc, child, &buf);
      printf("%*.*s\n", indent, indent, buf.bp?(char *)buf.bp:"");
      tidyBufFree(&buf);
    }
    dumpNode(doc, child, indent + 4); /* recursive */ 
  }
}

CURL *download(char *url){
	CURL *curl;
	char curl_errbuf[CURL_ERROR_SIZE];
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
	return curl;
}

int parser(CURL *curl){
	TidyDoc tdoc;
	TidyBuffer docbuf = {0};
	TidyBuffer tidy_errbuf = {0};
	int err;
	tdoc = tidyCreate();
	tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */ 
	tidyOptSetInt(tdoc, TidyWrapLen, 4096);
	tidySetErrorBuffer(tdoc, &tidy_errbuf);
	tidyBufInit(&docbuf);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
	err=curl_easy_perform(curl);
	if(!err) {
		err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */ 
			if(err >= 0) {
				err = tidyCleanAndRepair(tdoc); /* fix any problems */ 
				if(err >= 0) {
					err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */ 
				if(err >= 0) {
					dumpNode(tdoc, tidyGetRoot(tdoc), 0); /* walk the tree */ 
					fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */ 
				}
			}
		}
	}
	else
	fprintf(stderr, "%s\n", curl_errbuf);

	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyBufFree(&tidy_errbuf);
	tidyRelease(tdoc);
	return err;
}