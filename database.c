/*
 *  C Database 
 *  Copyright (c) 2021 Leonardo Zamboni
 * 
 *  this program is free software: you can redistribute it and/or modify
 *  it under the terms of the gnu general public license as published by
 *  the free software foundation, either version 3 of the license, or
 *  (at your option) any later version.
 *  
 *  this program is distributed in the hope that it will be useful,
 *  but without any warranty; without even the implied warranty of
 *  merchantability or fitness for a particular purpose.  see the
 *  gnu general public license for more details.
 *  
 *  you should have received a copy of the gnu general public license
 *  along with this program.  if not, see <http://www.gnu.org/licenses/>.
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DB "database.csv"

enum {CREATE,PRINT,REMOVE,DESTROY};

typedef uint8_t boolean;

void create(FILE *f, char **argv);
void print(FILE *f, char c);
boolean remove_id(FILE *f, size_t i);
void destroy(void);
boolean data_access_object(FILE *f, int argc, char **argv, uint8_t cmd);
uint8_t get_cmd(const char **cmds, char *argv);
size_t get_lines(FILE *f);

int main(int argc,char **argv) 
{
	if (argc < 2) return 1;

	FILE *f = fopen(DB,"a+");
	if (!f) return 1; 

	fflush(stdout);

	const char *cmds[] = {"-c","-p","-r","-d",NULL};
	boolean r = data_access_object(f,argc,argv,get_cmd(cmds,argv[1]));
	if (r) return 1;

	return 0;
}

boolean data_access_object(FILE *f, int argc, char **argv, uint8_t cmd)
{
	switch (cmd) 
	{
		case CREATE:
			if (argc < 4) return 1;
			fprintf(f,"\"%ld\",",get_lines(f));
			create(f,argv+2);
			break;	
		case PRINT:
			print(f,fgetc(f));
			break;	
		case REMOVE:
			if (argc < 3) return 1;
			if (remove_id(f,strtol(argv[2],NULL,10))) return 1;
			break;
		case DESTROY:
			destroy();
			break;			
		default: return 1;
	}
	fclose(f);	
	return 0;
}

uint8_t get_cmd(const char **cmds, char *argv)
{
	if (strcmp(argv,(*cmds))==0) return 0;
	cmds++;
	return 1 + get_cmd(cmds,argv);	
}

size_t get_lines(FILE *f)
{
	char c;
	size_t cnt = 0;
	do
	{
		c = fgetc(f);
		if (c == '\n') cnt++;
	} while (c != EOF);
	return cnt;
}

void destroy(void) { remove(DB); }

boolean remove_id(FILE *f, size_t i)
{
	size_t lines = get_lines(f);
	if (i > (lines-1)) return 1;

	FILE *tmp = tmpfile();
	if (!tmp) return 1;

	rewind(f);

	size_t l=0;
	char c,aux[5];
	fgets(aux,5,f);
	while (!feof(f))
	{		
		if (c=='\n') 
		{
			l++;
			fgets(aux,5,f);
		}
		c=fgetc(f);
		if (l==i) continue;
		fputc(c,tmp);
	}
	rewind(tmp);
	
	fclose(f);
	f=fopen(DB,"w");

	l=0;
	if ((lines-1)) fprintf(f,"\"%ld\",",l++);
	while (!feof(tmp))
	{
		c=fgetc(tmp);
		fputc(c,f);
		if (c=='\n'&&l<(lines-1))
		{ 
			fprintf(f,"\"%ld\",",l++);
		}
	}

	fclose(tmp);
	return 0;
}

void print(FILE *f, char c)
{
	if (c == EOF) return;
	printf("%c",c);
	print(f,fgetc(f));
}

void create(FILE *f, char **argv)
{
	if (!(*argv)) return;
	fprintf(f,"\"%s\"",(*argv));
	argv++;
	fprintf(f,"%c",(!(*argv)?'\n':','));
	create(f,argv);
}
