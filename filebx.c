/**
*
*  BOXED_FILE_EDITOR is available without a warranty of any kind.
*  No legal restrictions are applied to this software.           
*
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EXIT_NORMAL_CODE
#define EXIT_NORMAL_CODE (0)
#endif

#ifndef NORMAL
#define NORMAL (1)
#endif

char *cgetline(void);
FILE *target;

typedef enum  {
     CREATE='w', READ='r', APP='a',
     UP, RW, OVER,      
} file_operations_t;
typedef file_operations_t fops;

inline void FILE_OPERATION(const char*, fops);

int main(int argc, char *argv[])
{    
    register char *line;
    if (argc > 2) {    /* open file for writing */
       short c, data;
       long count, findex;
       fops fmode;
       while (--argc > 0 && (*++argv)[0] == '-')
             c = *++argv[0];
             switch (c) {
                    case 'a':            // append, (default)                                              
                         while (NORMAL) {
                               target = fopen(argv[0], "a");   
                               line = cgetline();
                               fprintf(target, line);
                               fclose(target);                                         
                         }  
                    break;                    
                    case 'o':                                                       
                         FILE_OPERATION(argv[0], OVER);                                               
                    break;
                    case 'r':           // read from file                         
                         FILE_OPERATION(argv[0], READ);
                    break;
                    case 'c':          // create                         
                         FILE_OPERATION(argv[0], CREATE);
                    break;
             }
    } else if (argc == 2) {
           while (NORMAL) {
                 long findex;
                 line = cgetline();                 
                 target = fopen(argv[1], "a");
                 for (findex = 0; findex < strlen(line); findex++)
                     fputc(line[findex], target);
                 fclose(target);
           }
    } else {           
               printf("Please specify the file name:\n");                
               char *f = cgetline();
               FILE_OPERATION(f, APP);                       
    }    
    return EXIT_NORMAL_CODE;
}

char *cgetline(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    while (1) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            char * linen = realloc(linep, lenmax *= 2);
            len = lenmax;

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

inline void FILE_OPERATION(const char *pfile, fops mode) {
     if (mode == READ) {  // simple file reading...
        int c;
        FILE *ff = fopen(pfile, "r");
        while ((c = fgetc(ff)) != (EOF))
              printf("%c", c);
        fclose(ff);
     } else if (mode == CREATE) {
            FILE *f = fopen(pfile, "w");
            fclose(f);         
     } else if (mode == OVER) {
            FILE *overw = fopen(pfile, "w+");
            fprintf(overw, "");
            fclose(overw);
            register char *ln;
            while (NORMAL) {
                 overw = fopen(pfile, "a");
                 ln = cgetline();
                 fprintf(overw, ln);
                 fclose(overw);                                                              
            }
     } else if (mode == APP) {            
            printf("Opened -> %s\n---------------------------------------------------\n", pfile);
            char *line;
            while (NORMAL) {
                 target = fopen(pfile, "w");   
                 line = cgetline();
                 fprintf(target, line);
                 fclose(target);                                         
            }             
     }                
                         
}
