struct url{
    char *protocole;
    char *domain;
    char *path;
    char *other;
    int type;
};

struct url *newUrl();
struct url *parseUrl(const char *source);
void freeUrl(struct url **url);
void printUrl(const struct url *url);

char *composeUrl(const struct url *url);
void composeUrlBuff(char **buff, const struct url *url);
int relativeToAbsoluteUrl(struct url *relative, const struct url *base);