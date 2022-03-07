#ifndef _CEDIS_REPLY_H_
#define _CEDIS_REPLY_H_

char *resp_simple_string_encode(const char *data);
char *resp_bulk_string_encode(const char **data);

#endif
