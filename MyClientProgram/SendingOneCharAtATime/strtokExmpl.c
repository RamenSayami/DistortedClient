/* strtok example */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main ()
{
  char str1[10] ="3:2:X:l\n";
  char str[10];
  bzero(str, 10);
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str1);
  strncpy(str, str1, (strlen(str1)-1));
  int len= strlen(str);
  printf("Strlen: %d\n", len);
  pch = strtok (str,":");
  while (pch != NULL)
  {
    printf ("%d\n",atoi(pch));
    pch = strtok (NULL, ":");
  }
  return 0;
}