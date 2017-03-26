struct url{
    char *protocole;
    char *domain;
    char *port;
    char *path;
    int type;
};

struct url *newUrl();
void freeUrl(struct url **url);
void printUrl(struct url *url);
struct url *parseUrl(char *url);
char *composeUrl(struct url *url);
int relativeToAbsoluteUrl(struct url *relative, struct url *base);