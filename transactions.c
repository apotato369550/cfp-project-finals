#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "transactions.h"

/*
FILE: transactions.c
AUTHOR: John Andre Saberon Yap, Franchesca Kho
DESCRIPTION: a module of CICS that holds the different functions that
can be performed with the custom struct TRANSACTION
COPYRIGHT: 2022
*/

/*
FUNCTION: void load_transactions(TRANSACTION transactions[])
DESCRIPTION: Recieves an empty array of TRANSACTION and fills the array with
transactions stored in the separate file "transactions.txt"
ARGUMENTS: 
TRANSACTION transactions[] - An empty array to be filled with items
RETURN: none (void)
*/

void load_transactions(TRANSACTION transactions[])
{
	// declare line variables, index, and open "transactions.txt" as "read"
	char line[1000];
	FILE *file = fopen("transactions.txt", "r");
	int i = 0;
	
	// loop through each line in "transactions.txt"
	while(fgets(line, 1000, file))
	{
		// declare a pointer to look at the first word separated by a ";" in that line
		char *ptr;
		ptr = strtok(line, ";");
		int j = 0;
		
		while(ptr != NULL)
		{
			switch(j)
			{
				// copy the date of the transaction to the transactions array
				case 0:
					strcpy(transactions[i].date, ptr);
					break;
				// copy the type of transactions into the array
				case 1:
					strcpy(transactions[i].type, ptr);
					break;
				// copy the name of the item into the transactions array
				case 2:
					strcpy(transactions[i].item, ptr);
					break;
				// copy the quantity of the item as an integer into the items array
				case 3: 
					transactions[i].quantity = atoi(ptr);
					break;
				// copy the price of the item as a float into the items array
				case 4:
					transactions[i].price = atof(ptr);
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
	// close "transactions.txt"
	fclose(file);
}

/*
FUNCTION: int get_total_transactions()
DESCRIPTION: Reads the number of lines in "transactions.txt" and returns that number
ARGUMENTS: none
RETURN: an integer (int) - the number of lines (items) in "transactions.txt"
*/

int get_total_transactions()
{	
	// declare line and index variables and open "transactions.txt" as "read"
	char line[1000];
	FILE *file = fopen("transactions.txt", "r");
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
FUNCTION: void update_items(TRANSACTION transactions[], int new_length)
DESCRIPTION: Updates "transactions.txt" given a new array of TRANSACTIONs
ARGUMENTS: 
TRANSACTION transactions[] - an array of TRANSACTIONs to be written onto "transactions.txt"
int new_length - an integer containing the total number of transactions
RETURN: none (void)
*/

void update_transactions(TRANSACTION transactions[], int new_length)
{
	// open "transactions.txt" as "write"
	FILE *file = fopen("transactions.txt", "w");
	
	// use a loop to print onto the file and loop through the array of transactions[]
	int i;
	for(i = 0; i < new_length; i++)
	{
		// print the details of the transaction onto the file
		fprintf(file, "%s;%s;%s;%d;%f\n", transactions[i].date, transactions[i].type, transactions[i].item, transactions[i].quantity, transactions[i].price);
	}
	
	// close "transactions.txt"
	fclose(file);
	return;
}

/*
FUNCTION: void add_transaction(TRANSACTION transactions[], int index, char item[], int quantity, float total_price, char type[])
DESCRIPTION: Adds a new transaction to the end of a TRANSACTION array
ARGUMENTS: 
TRANSACTION transactions[] - An array holding all current transactions loaded from "transactions.txt"
int index - The index where to insert the new transaction
char item[] - The name of the item
int quantity - The quantity of the item being transacted
float total_price - The amount of money that has exchanged hands
char type[] - The transaction type (can be either "BUY" or "SELL")
RETURN: none (void)
*/

void add_transaction(TRANSACTION transactions[], int index, char item[], int quantity, float total_price, char type[])
{
	// get the current date and time
	time_t t;
	time(&t);
	
	// copy the date & time onto the curreent transaction
	strcpy(transactions[index].date, strtok(ctime(&t), "\n"));
	
	// copy the transaction type, name of the item, quantity, and price of the item onto the TRANSACTIION in the array
	strcpy(transactions[index].type, type);
	strcpy(transactions[index].item, item);
	transactions[index].quantity = quantity;
	transactions[index].price = total_price;
}

/*
FUNCTION: void display_transaction_history(TRANSACTION transactions[], int total_transactions)
DESCRIPTION: Reads, formats, and displays all transactions in a TRANSACTIONs array loaded from "transactions.txt"
ARGUMENTS: 
TRANSACTION transactions[] - An array holding all current transactions loaded from "transactions.txt"
int total_transactions - the number of transactions/lines in "transactions.txt"
RETURN: none (void)
*/

void display_transaction_history(TRANSACTION transactions[], int total_transactions)
{
	// loop through each TRANSACTION in the transactions[] array
	int i;
	for(i = 0; i < total_transactions; i++)
	{
		// display the date, type, item/s, quantity, and price of the transaction
		printf("-------------------------------------------------------------\n");
		printf("Date of transaction: %s\n", transactions[i].date);
		printf("Type of Transaction: %s\n", transactions[i].type);
		printf("Item: %s\n", transactions[i].item);
		printf("Quantity: %d\n", transactions[i].quantity);
		printf("Price: %.2f\n", transactions[i].price);
	}
	printf("-------------------------------------------------------------\n\n");
}
