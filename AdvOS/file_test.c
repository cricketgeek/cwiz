#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc, char **argv){
  int fd;
  ssize_t len;
  char *filename;
  int key, srch_key, new_value, tmp_value;

  if(argc < 4){
    fprintf(stderr, "usage: file_test.o filename key value\n");
    exit(EXIT_FAILURE);
  }

  filename = argv[1];
  srch_key = strtol(argv[2], NULL, 10);
  new_value = strtol(argv[3], NULL, 10);

  /*
   * Open filename, which consists on <key, value> pairs, 
   * both ints written out in binary.  Change the value associated
   * with srch_key to new_value.
   *
   * YOUR CODE HERE...
   */
    FILE *fp;
    if((fp = fopen(filename, "r+")) == NULL)
    {
        printf("Can't open %s\n",filename);
        exit(1); 
    }

    printf("trying to find key:%d\n",srch_key);
    key = getc(fp);
    while(key != EOF)
    {
      key = key - '0';
      printf("key is %d expected value is %d\n",key,srch_key);
      if (key == srch_key)
      {
        fseek(fp,1L, SEEK_CUR);
        tmp_value = getc(fp);
        printf("We found it key:%d val:%c\n", srch_key,tmp_value);        
        fseek(fp,-1L,SEEK_CUR);
        putc((new_value + '0'),fp);
        fseek(fp,1L,SEEK_CUR);
        break;
      }
      else
      {
        fseek(fp,3L, SEEK_CUR);
      }
      key = getc(fp);
    }
    
    fclose(fp);
}

