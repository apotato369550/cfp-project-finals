#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// import custom headers
#include "items.h" 
#include "transactions.h"
#include "manager_actions.h"
#include "customer_actions.h"
#include "supplier_actions.h"
#include "art.h"

// function prototype declaration
void display_options();

/*
FILE: main.c
AUTHOR: Maria Belle Beatrice Ocampo, John Andre Saberon Yap
DESCRIPTION: Main function of the Cashier Inventory Management System 
program (CICS) which lets the user select from options what feature to
access
COPYRIGHT: 2022
*/

int main()
{
	// set the default password to 'admin' for accessing private features
	// password can be changed by the manager in the view_manager_actions() module
	char manager_password[100] = "admin";
	
	// store user input in a character
	char input;
	
	print_art("welcome_art");
	
	// greet the the user and inform him of his options
	printf("Welcome to The Cashier and Inventory Management System (CIMS)!\n We are glad to have you here.\n\n");
	printf("Below are a list of your options.\n\n");
	
	// call display_options() function
	display_options();
	
	// enter into a loop while the user does not wish to exit
	while(input != 'x')
	{
		// set input to be an empty space
		input = ' ';
		
		// clear the input buffer to remove unecessary chars/whitespace
		fflush(stdin);
		
		// prompt user to input a character based on the options given
		printf("Please select from one of the options above: ");
		
		// recieve input from the user
		scanf("%c", &input);
		
		// enter into a switch statement
		switch(input)
		{
			// if user enters 'x', clear the screen, say goodbye to the user, and exit the program
			case 'x':
				system("cls");
				printf("\n\nThank you for using CICS. We hope to see you again soon:> \n\n");
				break;
				
			// if user enters '1', clear the screen, inform user of their choice, then go to display_manager_actions() module
			case '1':
				system("cls");
				printf("You selected: View Manager Actions\n\n");
				view_manager_actions(manager_password);
				break;
				
			// if user enters '2', create a folder to store customer receipts
			// then clear the screen, info// rm the user of their choice,
			// then call the log_customer_transaction();
			case '2':
				system("md customer_receipts");
				system("cls");
				printf("You selected: Record Customer Transaction\n\n");
				log_customer_transaction();
				break;
				
			// if user enters '3', create a folder to store supplier receipts
			// then clear the screen, inform the user of their choice,
			// then call the log_supplier_transaction();
			case '3':
				system("md supplier_receipts");
				system("cls");
				printf("You selected: Record Supplier Restock\n\n");
				log_supplier_restock();
				break;
			// if user did not enter a valid option, inform the user to enter a valid option
			default: 
				printf("Invalid option. Please select an option shown below and try again\n");
				break;
		}
		// display the different options to the user
		display_options();
	}
	
	return 0;
}

/*
FUNCTION: void display_options()
DESCRIPTION: displays the different options to the user
ARGUMENTS: none
RETURN: none (void)
*/
void display_options()
{
	// print options header
	printf("--- Options ---\n\n");
	
	// print the navigation menu
	printf("1 - Display Manager Actions (requires login)\n");
	printf("2 - Record Customer Transaction\n");
	printf("3 - Record Supplier Restock \n");
	printf("x - Exit Program \n\n");
}

