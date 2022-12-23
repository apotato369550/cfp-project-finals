#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "items.h"

/*
FILE: items.c
AUTHOR: John Andre Saberon Yap, Franchesca Kho
DESCRIPTION: a module of CICS that holds the different functions that
can be performed with the custom struct ITEM
COPYRIGHT: 2022
*/

/*
FUNCTION: void load_items(ITEM items[])
DESCRIPTION: Recieves an empty array of ITEM and fills the array with
items stored in the separate file "items.txt"
ARGUMENTS: 
ITEM items[] - An empty array to be filled with items
RETURN: none (void)
*/

void load_items(ITEM items[])
{
	// declare line variables, index, and open "items.txt" as "read"
	char line[1000];
	FILE *file = fopen("items.txt", "r");
	int i = 0;
	
	// loop through each line in "items.txt"
	while(fgets(line, 1000, file))
	{	
		// declare a pointer to look at the first word separated by a ";" in that line
		char *ptr;
		ptr = strtok(line, ";");
		int j = 0;
		
		// loop through the words in that line separated by a ";"
		while(ptr != NULL)
		{
			switch(j)
			{
				// copy the name of the item into the items array
				case 0:
					strcpy(items[i].name, ptr);
					break;
				// copy the description of the item into items array
				case 1:
					strcpy(items[i].description, ptr);
					break;
				// copy the quantity of the item as an integer into the items array
				case 2:
					items[i].quantity = atoi(ptr);
					break;
				// copy the price of the item as a float into the items array
				case 3: 
					items[i].price = atof(ptr);
					break;
				default:
					break;
			}
			// move on to the next word in the line separated by a ";" and increment j
			ptr = strtok(NULL, ";");
			j++;
		}
		// increment i after each line
		i++;
	}
	
	// close "items.txt"
	fclose(file);
}


/*
FUNCTION: int get_total_items()
DESCRIPTION: Reads the number of lines in "items.txt" and returns that number
ARGUMENTS: none
RETURN: an integer (int) - the number of lines (items) in "items.txt"
*/


int get_total_items()
{
	// declare line and index variables and open "items.txt" as "read"
	char line[1000];
	FILE *file = fopen("items.txt", "r");
	int i = 0;
	
	// loop through each of the lines of the file and increment i for each line 
	while(fgets(line, 1000, file))
	{
		i++;
	}
	
	// close the file
	fclose(file);
	
	// return the number of lines
	return i;
}


/*
FUNCTION: void update_items(ITEM items[], int new_length)
DESCRIPTION: Updates "items.txt" given a new array of ITEMs
ARGUMENTS: 
ITEM items[] - an array of ITEM to be written onto "items.txt"
int new_length - an integer containing the total number of items
RETURN: none (void)
*/

void update_items(ITEM items[], int new_length)
{
	// open "items.txt" as "write"
	FILE *file = fopen("items.txt", "w");
	
	// use a loop to print onto the file and loop through items[] array
	int i;
	for(i = 0; i < new_length; i++)
	{
		// print the item's name, description, new quantity, and other details
		fprintf(file, "%s;%s;%d;%f\n", items[i].name, items[i].description, items[i].quantity, items[i].price);
	}
	
	// close "items.txt"
	fclose(file);
	return;
}


/*
FUNCTION: int item_exists(char item[], ITEM items[], int total_items)
DESCRIPTION: Checks if an item exists in the catalog
ARGUMENTS: 
char item[] - an array of characters. the name of the item being searched for
ITEM items[] - an array of ITEM containing all the items in the catalog "items.txt"
int total_items - the number of lines in "items.txt"
RETURN: 1 if the item is found, 0 if the item is not found
*/

int item_exists(char item[], ITEM items[], int total_items)
{
	// declare variables
	int found = 0;
	int i;
	
	// loop through the items[] array to check if item[] exists
	for(i = 0; i < total_items; i++)
	{	
		found = 1;
		char name[100];
		
		// copy the name of the current item into the name[] variable
		strcpy(name, items[i].name);
		
		// loop through each character of the item
		int j;
		for(j = 0; j < strlen(item); j++)
		{
			// if the compared characters are not the same, found = 0
			if(tolower(item[j]) != tolower(name[j]))
			{
				found = 0;
			}
		}
		
		// the item is found if found = 1, then the function returns 1
		if (found == 1)
		{
			return 1;
		}
	}
	
	// the function returns found = 0 if the item is not found
	return found;
}

/*
FUNCTION: int get_left_quantity(char item[], ITEM items[], int total_items)
DESCRIPTION: Gets the quantity of a certain item that is currently in stock
ARGUMENTS: 
char item[] - an array of characters. the name of the item being searched for & the quantity
ITEM items[] - an array of ITEM containing all the items in the catalog "items.txt"
int total_items - the number of lines in "items.txt"
RETURN: an integer (int). returns the quantity of that item. 0 if the item is not found.
*/

int get_left_quantity(char item[], ITEM items[], int total_items)
{
	// check if the item exists (same as item_exists() module)
	int found = 0;
	int i;
	for(i = 0; i < total_items; i++)
	{
		found = 1;
		char name[100];
		strcpy(name, items[i].name);
		int j;
		for(j = 0; j < strlen(item); j++)
		{
			if(tolower(item[j]) != tolower(name[j]))
			{
				found = 0;
			}
		}
		if(found == 1)
		{
			// if the item is found, return that items quantity in stock
			int quantity = items[i].quantity;
			printf("In Stock: %d\n", quantity);
			return quantity;
		}
	}
	printf("Item not found\n");
	return found;
}


/*
FUNCTION: float get_price(char item[], ITEM items[], int total_items)
DESCRIPTION: Gets the price of a certain item that is currently in stock
ARGUMENTS: 
char item[] - an array of characters. the name of the item being searched for and its price
ITEM items[] - an array of ITEM containing all the items in the catalog "items.txt"
int total_items - the number of lines in "items.txt"
RETURN: a float. the price of that item. (if it does not exist, returns 0)
*/

float get_price(char item[], ITEM items[], int total_items)
{
	// check if the item exists (same as item_exists() module)
	int found = 0;
	int i;
	for(i = 0; i < total_items; i++)
	{
		found = 1;
		char name[100];
		strcpy(name, items[i].name);
		int j;
		for(j = 0; j < strlen(item); j++)
		{
			if(tolower(item[j]) != tolower(name[j]))
			{
				found = 0;
			}
		}
		if(found == 1)
		{
			// if the item is found, return that items price
			float price = items[i].price;
			printf("Item price Per Unit: %.2f\n", price);
			return price;
		}
	}
	printf("Item not found\n");
	return found;
}

/*
FUNCTION: float get_price(char item[], ITEM items[], int total_items)
DESCRIPTION: Changes the quantity of a current item in stock after a transaction
ARGUMENTS: 
char item[] - an array of characters. the name of the item being searched for and to access its quantity
ITEM items[] - an array of ITEM containing all the items in the catalog "items.txt"
int total_items - the number of lines in "items.txt"
int quantity - the new quantity of that item
RETURN: 1 if the item is found, 0 if the item is not found
*/

void edit_item_quantity(ITEM items[], int total_items, char item[], int quantity)
{
	// check if the item exists (same as item_exists() module)
	int found = 0;
	int i;
	for(i = 0; i < total_items; i++)
	{
		found = 1;
		char name[100];
		strcpy(name, items[i].name);
		int j;
		for(j = 0; j < strlen(item); j++)
		{
			if(tolower(item[j]) != tolower(name[j]))
			{
				found = 0;
			}
		}
		if(found == 1)
		{
			// if the item is found, update that item's quantity to a new quantity
			items[i].quantity = quantity;
			printf("New quantity of %s: %d\n", item, items[i].quantity);
			return;
		}
	}
}

/*
FUNCTION: void display_items_quantity(ITEM items[], int total_items)
DESCRIPTION: prints out each item and its description, price, and quantity from the catalog
ARGUMENTS: 
char item[] - an array of characters. the name of the item being searched for and to access its quantity
ITEM items[] - an array of ITEM containing all the items in the catalog "items.txt"
RETURN: none (void)
*/

void display_items_quantity(ITEM items[], int total_items)
{
	// loop through the ITEMs in the items[] array
	int i;
	for(i = 0; i < total_items; i++)
	{
		// display each item's name, description, quantity, and price to the user
		printf("-------------------------------------------------------------\n");
		printf("Item Name: %s\n", items[i].name);
		printf("Item Description: %s\n", items[i].description);
		printf("Amount currently in stock: %d\n", items[i].quantity);
		printf("Price per item: %.2f\n\n", items[i].price);
	}
	printf("-------------------------------------------------------------\n\n");
	return;
}

