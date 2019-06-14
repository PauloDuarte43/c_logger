//logger.c
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"

FILE *fp ;
static int SESSION_TRACKER; //Keeps track of session

char* print_time()
{
    int size = 0;
    time_t t;
    char *buf;
    
    t=time(NULL); /* get current calendar time */
    
    char *timestr = asctime( localtime(&t) );
    timestr[strlen(timestr) - 1] = 0;  //Getting rid of \n
    
    size = strlen(timestr)+ 1 + 2; //Additional +2 for square braces
    buf = (char*)malloc(size);
    
    memset(buf, 0x0, size);
    snprintf(buf,size,"[%s]", timestr);
   
    return buf;
}

static char * get_log_sysdate(void){
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	char date[64];
	memset(date, '\0', 64);
	strftime(date, sizeof(date), "%Y%m%d", tm);
	return strdup(date);
}

void log_print(char* filename, int line, char *fmt,...)
{
    va_list	    list;
    char	    *p, *r;
    int		    e;

    char file_name[254];
    memset(file_name, '\0', 254);
    
    //NomeDaMáquina_rastreabilidade_acesso_YYYYMMDD.log
    const char * log_file_path = "/home/paulo.antunes/workspace/logger/";
    
    strcpy(file_name, log_file_path);
    strcat(file_name, "NomeDaMaquina_");
    strcat(file_name, "rastreabilidade_acesso_");
    strcat(file_name, get_log_sysdate());
    strcat(file_name, ".log");
    
    //if(SESSION_TRACKER > 0)
    fp = fopen (file_name, "a+");
    //else
    //	fp = fopen ("log.txt","w");
    
    //fprintf(fp,"%s ",print_time());
    //fprintf(fp,"[%s][line: %d] ",filename,line);
    va_start( list, fmt );

    for ( p = fmt ; *p ; ++p )
    {
	if ( *p != '%' )//If simple string
	{
	    fputc( *p,fp );
	}
	else
	{
	    switch ( *++p )
	    {
		/* string */
	    case 's':
	    {
		r = va_arg( list, char * );

		fprintf(fp,"%s", r);
		continue;
	    }

	    /* integer */
	    case 'd':
	    {
		e = va_arg( list, int );

		fprintf(fp,"%d", e);
		continue;
	    }

	    default:
		fputc( *p, fp );
	    }
	}
    }
    va_end( list );
    fputc( '\n', fp );
    SESSION_TRACKER++;
    fclose(fp);
}
