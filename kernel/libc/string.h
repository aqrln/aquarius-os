#ifndef _STRING_H_
#define _STRING_H_

int strcmp(char *str1, char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strlen(const char *src);
char *strchr(char *s, char c);
char *strncpy(char *dest, const char *src, u32 n);

#endif
