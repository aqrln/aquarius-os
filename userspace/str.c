#include "aq.h"

int strncmp(char *s1, char *s2, int n)
{

  if (n == 0)
    return 0;
  do {
    if (*s1 != *s2++)
      return *(unsigned char *)s1 - *(unsigned char *)--s2;
    if (*s1++ == 0)
      break;
  } while (--n != 0);
  return 0;
}

int strcmp(char *s1, char *s2)
{
	return strncmp(s1, s2, 256);
}

int strlen(char *s)
{
	int n = 0;
	while (*s++) n++;
	return n;
}

char *strchr(char *s, char c)
{
	while (*s) {
		if (*s == c)
			return s;
		s++;
	}
	return 0;
}

