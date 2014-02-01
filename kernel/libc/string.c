#include <kernel.h>
#include <string.h>

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    do {
      *dest++ = *src++;
    } while (*src != 0);

	return dest;
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
    while (*dest != 0) {
        dest++;
    }

    do {
        *dest++ = *src++;
    } while (*src != 0);

    return dest;
}

int strlen(const char *src)
{
    int i = 0;
    while (*src++)
        i++;
    return i;
}

// Find the first occurence of c in s
char *strchr(char *s, char c)
{
	while (*s) {
		if (*s == c)
			return s;
		s++;
	}
	return 0;
}

// Copy n chars from src to dest and return dest
char *strncpy(char *dest, const char *src, u32 n)
{
	u32 len = strlen(src);
	if (n > len)
		n = len;
	for (u32 i = 0; i < n; i++) {
		*dest++ = *src++;
	}
	*dest++ = 0;
	return dest;
}
