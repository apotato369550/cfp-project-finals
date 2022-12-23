#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "items.h"
#include "transactions.h"
#include "supplier_actions.h"

void create_supplier_receipt(TRANSACTION receipt_transactions[], int item_total, char receipt_id[]);

/*
FILE: supplier_actions.c
AUTHOR: John Andre Saberon Yap, Franchesca Kho
DESCRIPTION: a module of CICS that contains the actions that can be performed for the supplier namely:
recording transactions and creating receipts
COPYRIGHT: 2022
*/

/*
FUNCTION: void log_supplier_restock()
DESCRIPTION: Logs a supplier's restocking. Takes note of the item, quantity, and price of what the supplier is selling.
Records transaction/s in transactions.txt, creates a receipt for the company and the supplier
ARGUMENTS: none
RETURN: none (void)
*/

void log_supplier_restock()
{
	// load the items from "items.txt"
	int total_items = get_total_items();
	ITEM items[total_items];
	load_items(items);
	
	// display customer logging art
	print_art("record_supplier_art");
	
	// display items that can be restocked
	display_items_quantity(items, total_items);
	
	// load the transactions from "transactions.txt"
	int total_transactions = get_total_transactions();
	TRANSACTION transactions[total_transactions + 100];
	load_transactions(transactions);
	
	// create a TRANSACTIONs array to hold the current transactions
	TRANSACTION receipt_transactions[100];
	
	// create variables to hold information about the current items
	char item[100];
	int quantity, item_total = 0;
	float price, supplier_price;
	char cont;	
	int max_storage = 100;
	
	// enter into a 'shopping loop' which stops when the user wants to:
	// - cancel the transaction
	// - confirms their transaction after selling all the items that they wish
	while(cont != '1')
	{
		
		// prompt the user to enter the name of the item whey want to purchase
		printf("Please enter the name of the item the supplier wishes to restock: ");
		scanf("%s", &item);
		// if that item does not exist, prompt the user to either try again or return to the main menu
		if(!item_exists(item, items, total_items))
		{
			printf("Sorry, that item does not exist in our catalog. Would you like to try again? (y/n)");
			fflush(stdin);
			char input;
			scanf("%c", &input);
			if(input == 'y')
			{
				continue;
			} else {
				printf("Transaction denied.");
				return;
			}
		}
		
		// prompt the user to enter the quantity of the item they want to purchase
		printf("Please enter the quantity of the item you wish to supply (it must not exceed the max capacity of 100 per item in stock): ");
		scanf("%d", &quantity);
		
		// get the number of items left in stock
		int left_quantity = get_left_quantity(item, items, total_items);
		
		// if the new items will exceed max storage capacity, inform the user
		// then have them decide whether to try again or cancel the transaction
		if(left_quantity + quantity > max_storage)
		{
			printf("Sorry, but we don't need that much %s as it would exceed our max storage (100 max). Would you like to try again? (y/n)", item, max_storage);
			fflush(stdin);
			char input;
			scanf("%c", &input);
			if(input == 'y')
			{
				continue;
			} else {
				printf("Transaction denied.");
				return;
			}
		}
		
		// get the price at which the supplier is selling that item
		printf("Please enter the price you are selling the item at (per unit): ");
		scanf("%f", &supplier_price);
		
		// get the price of the item the store is selling it at and the total price
		float price = get_price(item, items, total_items);
		float total_price = supplier_price * quantity;
		
		// if the supplier's price is higher than the store's selling price, inform the user
		// let them either try again or cancel the transaction alltogether
		if(supplier_price >= price)
		{
			printf("Sorry, you aren't selling that item at a reasonable enough price (Your selling price = %.2f vs Our selling price = %.2f). Your selling price should be lower than our selling price. Would you like to try again? (y/n)", supplier_price, price);
			fflush(stdin);
			char input;
			scanf("%c", &input);
			if(input == 'y')
			{
				continue;
			} else {
				printf("Transaction denied.");
				return;
			}
		}
		
		// display information about the user's sale onto the screen namely:
		// the name of the item, quantity, price per unit, and total price
		system("cls");
		
		// print shopping cart design
		print_art("shopping_cart");
		
		printf("Item: %s\n", item);
		printf("Quantity: %d\n", quantity);
		printf("Price per unit: %.2f\n", supplier_price);
		printf("Total Cost: %.2f\n", total_price);
		
		// give the user the option to proceed, add another item, or to cancel the transaction
		printf("Enter 1 if you wish to proceed with the transaction\n");
		printf("Enter 2 if you wish to add another item/s\n");
		printf("Enter x if you wish to cancel the transaction\n");
		
		printf("Please select an option: ");
		
		// clear the input buffer then recieve user input
		fflush(stdin);
		scanf("%c", &cont);
		
		// if the user does not wish to continue, go back to the main menu
		if(cont == 'x')
		{
			printf("Transaction cancelled.");
			return;
		}
		
		// set the type of transaction to "BUY"
		char type[] = "SELL";
		
		// update items[] and transactions[] arrays
		edit_item_quantity(items, total_items, item, left_quantity + quantity);
		add_transaction(transactions, total_transactions + item_total, item, quantity, total_price, type);
		add_transaction(receipt_transactions, item_total, item, quantity, total_price, type);
		
		// add one to the number of unique item/s sold
		item_total++;
		
		// clear screen
		system("cls");
		
		// if user continues to restock items, display items again
		if(cont == '2')
		{
			display_items_quantity(items, total_items);
		}
	}
	
	// print transaction complete
	print_art("transaction_complete_art");
	
	// update "transactions.txt" and "items.txt" using the items[] and transactions[] arrays
	update_items(items, total_items);
	update_transactions(transactions, total_transactions + item_total);
	
	// create a receipt for the customer
	char receipt_id[8];
	create_supplier_receipt(receipt_transactions, item_total, receipt_id);
	
	// display transaction id and bid the customer goodbye
	printf("Your Transaction ID: %s\n", receipt_id);
	printf("Transaction Complete. Have a nice day!\n\n");
	
	// set all values to 0 or an empty space to get ready for the next transaction ' ' 
	int i;
	for(i = 0; i < 100; i++)
	{
		item[i] = ' ';
	}
	for(i = 0; i < 8; i++)
	{
		receipt_id[i] = ' ';
	}
	quantity = 0;
	item_total = 0;
	cont = ' ';
	price = 0;
	supplier_price = 0;
}

/*
FUNCTION: void create_supplier_receipt(TRANSACTION receipt_transactions[], int item_total, char receipt_id[])
DESCRIPTION: Creates a receipt for the supplier and manager and stores it in a folder
ARGUMENTS: 
TRANSACTION receipt_transactions[] - An array of TRANSACTIONs to be recorded in the receipt
int item_total - The total number of items bought/sold
char receipt_id[] - Custom ID of the receipt
RETURN: none (void)
*/

void create_supplier_receipt(TRANSACTION receipt_transactions[], int item_total, char receipt_id[])
{
	// create random file name that doesn't exist in receipts folder
	char filename[100] = "supplier_receipts/receipt_";
	char ascii[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
	int i;
	
	// create a random receipt id
	for(i = 0; i < 8; i++)
	{
		char random_char = ascii[rand() % strlen(ascii) - 1];
		strncat(receipt_id, &random_char, 1);
		strncat(filename, &random_char, 1);
	}
	
	// get the current date and time
	time_t t;
	time(&t);
	
	// append ".txt" to the end of the filename
	strncat(filename, ".txt", 4);
	FILE *file = fopen(filename, "w");
	
	// print transaction id and date onto the file
	fprintf(file, "Transaction ID: %s\n\n", receipt_id);
	fprintf(file, "Date of transaction: %s\n\n", strtok(ctime(&t), "\n"));
	fprintf(file, "Items sold: \n\n");
	
	// print onto the file the items that were purchased
	fprintf(file, "-------------------------------------\n");
	float total_price = 0;
	for(i = 0; i < item_total; i++)
	{
		fprintf(file, "Date: %s Type: %5s Item: %12s Quantity: %3d Price: %5.2f PHP\n\n", receipt_transactions[i].date, receipt_transactions[i].type, receipt_transactions[i].item, receipt_transactions[i].quantity, receipt_transactions[i].price);
		total_price += receipt_transactions[i].price;
	}
	fprintf(file, "-------------------------------------\n\n");
	
	// print the total price of the items
	fprintf(file, "Total Price: %.2f PHP\n", total_price);
	
	// close the file
	fclose(file);
}

