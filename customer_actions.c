#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// import custom headers
#include "items.h"
#include "transactions.h"
#include "customer_actions.h"

// function prototypes
void create_customer_receipt(TRANSACTION receipt_transactions[], int item_total, char receipt_id[], float cash_received, float cash);

/*
FILE: customer_actions.c
AUTHOR: John Andre Saberon Yap, Franchesca Kho
DESCRIPTION: a module of CICS that contains the actions that can be performed for the customer namely:
recording transactions and creating receipts
COPYRIGHT: 2022
*/

/*
FUNCTION: void log_customer_transaction()
DESCRIPTION: Logs a customer's transaction. Takes note of the item, quantity, and price of what the customer is buying.
Records transaction/s in transactions.txt, creates a receipt for the company and the customer
ARGUMENTS: none
RETURN: none (void)
*/

void log_customer_transaction()
{
	// load the items from "items.txt"
	int total_items = get_total_items();
	ITEM items[total_items];
	load_items(items);
	
	// load the transactions from "transactions.txt"
	int total_transactions = get_total_transactions();
	TRANSACTION transactions[total_transactions + 100];
	load_transactions(transactions);
	
	// display customer logging art
	print_art("record_customer_art");
	
	// display the current items in stock
	display_items_quantity(items, total_items);
	
	// create a TRANSACTIONs array to hold the current transactions
	TRANSACTION receipt_transactions[100];
	
	// create variables to hold information about the current items
	char item[100];
	int quantity, item_total = 0;
	float cash, cash_received;
	char cont;
	
	// recieve the cash amount from the user
	printf("Please enter your current cash: ");
	scanf("%f", &cash);
	cash_received = cash;
	
	// enter into a 'shopping loop' which stops when the user wants to:
	// - cancel the transaction
	// - confirms their transaction after buying all the items that they need
	while(cont != '1')
	{
		
		// prompt the user to enter the name of the item whey want to purchase
		printf("Please enter the name of the item the customer wishes to purchase: ");
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
		printf("Please enter the quantity of the item you wish to purchase: ");
		scanf("%d", &quantity);
		
		// get the number of items left in stock
		int left_quantity = get_left_quantity(item, items, total_items);
		
		// if there is not enough items left in stock, inform the user
		// then have them decide whether to try again or cancel the transaction
		if(left_quantity < quantity)
		{
			printf("Sorry, we don't have enough '%s' in stock (only %d left in stock). Would you like to try again? (y/n)", item, left_quantity);
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
		
		// get the price of each item and the total price of the user's purchase
		float price = get_price(item, items, total_items);
		float total_price = price * quantity;
		
		// if the user does not have enough money, inform the user
		// let them either try again or cancel the transaction alltogether
		if(total_price > cash)
		{
			printf("Sorry, You do not have enough cash remaining to purchase that item/s (Price of item/s: %.2f. Cash remaining: %.2f). Would you like to try again? (y/n)", total_price, cash);
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
		
		// display information about the user's purchase onto the screen namely:
		// the name of the item, quantity, price per unit, and total price
		system("cls");
		
		// print shopping cart design
		print_art("shopping_cart");
		
		printf("Item: %s\n", item);
		printf("Quantity: %d\n", quantity);
		printf("Price per unit: %.2f\n", price);
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
		char type[] = "BUY";
		
		// update items[] and transactions[] arrays
		edit_item_quantity(items, total_items, item, left_quantity - quantity);
		add_transaction(transactions, total_transactions + item_total, item, quantity, total_price, type);
		add_transaction(receipt_transactions, item_total, item, quantity, total_price, type);
		
		// subtract the current cash from the total price of the items bought
		cash -= total_price;
		
		// add one to the number of unique item/s bought
		item_total++;
		
		// clear screen
		system("cls");
		
		// if user continues to buy items, display items again
		if(cont == '2')
		{
			display_items_quantity(items, total_items);
		}
	}
	
	// print transaction complete
	print_art("transaction_complete_art");
	
	// display change to be dispensed to the customer
	printf("Change to be dispensed: %.2f\n", cash);
	
	// update "transactions.txt" and "items.txt" using the items[] and transactions[] arrays
	update_items(items, total_items);
	update_transactions(transactions, total_transactions + item_total);
	
	// create a receipt for the customer
	char receipt_id[8];
	create_customer_receipt(receipt_transactions, item_total, receipt_id, cash_received, cash);
	
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
	cash = 0; 
	cash_received = 0;
	cont = ' ';
	
}


/*
FUNCTION: void create_customer_receipt(TRANSACTION receipt_transactions[], int item_total, char receipt_id[], float cash_received, float cash)
DESCRIPTION: Creates a receipt for the customer and manager and stores it in a folder
ARGUMENTS: 
TRANSACTION receipt_transactions[] - An array of TRANSACTIONs to be recorded in the receipt
int item_total - The total number of items bought/sold
char receipt_id[] - Custom ID of the receipt
float cash_received - Cash received by the cashier
float cash - remaining cash/change to be given to the customer
RETURN: none (void)
*/

void create_customer_receipt(TRANSACTION receipt_transactions[], int item_total, char receipt_id[], float cash_received, float cash)
{
	// create random file name that doesn't exist in receipts folder
	char filename[200] = "customer_receipts/receipt_";
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
	fprintf(file, "Items purchased: \n\n");
	
	// print onto the file the items that were purchased
	fprintf(file, "-------------------------------------\n");
	float total_price = 0;
	for(i = 0; i < item_total; i++)
	{
		fprintf(file, "Date: %s Type: %5s Item: %12s Quantity: %3d Price: %5.2f PHP\n\n", receipt_transactions[i].date, receipt_transactions[i].type, receipt_transactions[i].item, receipt_transactions[i].quantity, receipt_transactions[i].price);
		total_price += receipt_transactions[i].price;
	}
	fprintf(file, "-------------------------------------\n\n");
	
	// print the cash received, dispensed, and total price of the items
	fprintf(file, "Cash received: %.2f\n", cash_received);
	fprintf(file, "Change dispensed: %.2f\n", cash);
	fprintf(file, "Total Price: %.2f PHP\n", total_price);
	
	// close the file
	fclose(file);
}
