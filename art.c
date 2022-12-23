#include <stdio.h>
#include <string.h>

#include "art.h"

/*
FILE: art.c
AUTHOR: John Andre Saberon Yap
DESCRIPTION: Used to access and display art for the UI of the program
COPYRIGHT: 2022
*/


/*
FUNCTION: void print_art(char art[])
DESCRIPTION: Displays art from a folder of ".txt" files containing ASCII art.
ARGUMENTS: 
char art[] - The name of the vile without an extension
RETURN: none (void)
*/

void print_art(char art[])
{
	char file_name[100] = "art/";
	strcat(file_name, art);
	strcat(file_name, ".txt");
	
	FILE *file = fopen(file_name, "r");
	
	if (file == NULL)
	{
		return;
	}

	char line[1000];
	
	while(fgets(line, 1000, file))
	{
		printf("%s", line);
	}
	return;
}
