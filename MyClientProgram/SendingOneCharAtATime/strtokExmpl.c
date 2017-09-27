/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str1[10] ="0:10:1:l\n";
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
    printf ("%s\n",pch);
    pch = strtok (NULL, ":");
  }
  return 0;
}