scws_t scws_new();
void scws_free(scws_t s);
void scws_set_charset(scws_t s, const char *cs);
int scws_add_dict(scws_t s, const char *fpath, int mode);
int scws_set_dict(scws_t s, const char *fpath, int mode);
void scws_set_rule(scws_t s, const char *fpath);
void scws_set_ignore(scws_t s, int yes);
void scws_set_multi(scws_t s, int mode);
void scws_set_duality(scws_t s, int yes);
void scws_set_debug(scws_t s, int yes);
void scws_send_text(scws_t s, const char *text, int len);
scws_res_t scws_get_result(scws_t s);
void scws_free_result(scws_res_t result);
scws_top_t scws_get_tops(scws_t s, int limit, char *xattr);
void scws_free_tops(scws_top_t tops);
int scws_has_word(scws_t s, char *xattr);
scws_top_t scws_get_words(scws_t s, char *xattr);