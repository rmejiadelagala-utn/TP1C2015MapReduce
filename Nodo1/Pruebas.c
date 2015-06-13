/*
 ============================================================================
 Name        : Pruebas.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <sys/mman.h>

/*
simple function use with mmap and munmap
look the man from mmap,if you have questions...

*/
// return size of bytes on file
long FileSize(const char *file)
{
  long ret;
  FILE *fh = fopen(file, "r");
  if(!fh)
  {
   return 0;
  }
  fseek(fh, 0, SEEK_END);
  ret = ftell(fh);
  fclose(fh);
  return ret;
}

/* example
 to use with mmap
*/
char *fmmap(FILE *fd,long long FileSize)
{
 fd=(FILE *)fileno(fd);

 return mmap(0, FileSize, PROT_READ, MAP_PRIVATE, (int)fd, 0);
}

int pruebas() {
 FILE *fd;
 char *str;

 fd = fopen("/home/utnso/Testeo.txt", "r");
 str = fmmap(fd,FileSize("/home/utnso/Testeo.txt"));
 puts(str);
 munmap(str,sizeof(str));
 fclose(fd);

 return 0;
}
