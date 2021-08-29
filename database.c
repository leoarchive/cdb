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
#include <stdint.h>
#include <string.h>

#define DB "database.csv"

enum {
	CREATE,
	PRINT,
	DESTROY,
};

void destroy(FILE *f);
void print(FILE *f, char c);
void create(FILE *f, char **argv);
uint8_t data_access_object(FILE *f, int argc, char **argv, uint8_t cmd);

uint8_t get_cmd(const char **cmds, char *argv)
{
	if (strcmp(argv,(*cmds))==0) return 0;
	cmds++;
	return 1 + get_cmd(cmds,argv);	
}

int main(int argc,char **argv) 
{
	if (argc < 2) return 1;

	const char *cmds[] = {"-c","-p","-d",NULL};

	FILE *f = fopen(DB,"a+");
	if (!f) return 1; 

	fflush(stdout);

	if (data_access_object(f,argc,argv,get_cmd(cmds,argv[1]))) return 1;
		
	return 0;
}

uint8_t data_access_object(FILE *f, int argc, char **argv, uint8_t cmd)
{
	switch (cmd) 
	{
		case CREATE:
			if (argc < 4) return 1;
			create(f,argv);
			break;	
		case PRINT:
			print(f,fgetc(f));
			break;	
		case DESTROY:
			destroy(f);
			break;			
		default: return 1;
	}
	fclose(f);	
	return 0;
}

void destroy(FILE *f)
{
	remove(DB);
}

void print(FILE *f, char c)
{
	if (c == EOF) return;
	printf("%c",c);
	print(f,fgetc(f));
}

void create(FILE *f, char **argv)
{
	fprintf(f,"\"%s\",",argv[2]);
	fprintf(f,"\"%s\" \n",argv[3]);
}
