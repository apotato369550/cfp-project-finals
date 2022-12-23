#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// import custom headers
#include "items.h"
#include "transactions.h"
#include "manager_actions.h"
#include "art.h"

// declare function prototypes
int verify_user(char manager_password[100], int max_attempts);
void display_manager_options();
void change_password(char manager_password[100]);

/*
FILE: manager_actions.c
AUTHOR: John Andre Saberon Yap, Don King Ababa
DESCRIPTION: view_manager_actions module of the CICS that allows the manager
of the store to view records & transactions, stock quantities and prices,
and to change the terminal password
COPYRIGHT: 2022
*/


/*
FUNCTION: void display_options()
DESCRIPTION: Displays the different possible actions the manager can
take in managing his store (transaction records, receipts, stock
prices and quantities, etc.)
ARGUMENTS: 
char manager_password[] - The password necessary to access the manager features
RETURN: none (void)
*/

void view_manager_actions(char manager_password[])
{
	// create variable to hold max attempts and i (to hold the number of tries)
	int max_attempts = 3, i;
	
	// load the items in stock from "items.txt" into an array
	int total_items = get_total_items();
	ITEM items[total_items];
	load_items(items);
	
	// load all previous transactions from "transactions.txt" into an array
	int total_transactions = get_total_transactions();
	TRANSACTION transactions[total_transactions];
	load_transactions(transactions);
	
	// verify the user by asking them their password
	// if the user did not enter the password correctly, exit this part of the program
	if(verify_user(manager_password, max_attempts))
	{
		system("cls");
		printf("Login Successful!\n\n");
	} else {
		system("cls");
		printf("Login Failed! Exiting program...\n\n");
		return;
	}
	
	// print manager actions
	print_art("manager_actions_art");
	
	// create a variable to store user input
	char input;
	
	// display the manager options to the user
	display_manager_options();
	
	while(input != 'x')
	{
		// clear the input buffer and recieve input from the user
		fflush(stdin);
		printf("Please select an option: ");
		scanf("%c", &input);
		
		// call the corresponding module/function based on what the user selected
		switch(input)
		{
			case 'x':
				break;
			case '1':
				system("cls");
				printf("You selected 'Change Password'\n\n");
				change_password(manager_password);
				print_art("manager_actions_art");
				break;
			case '2':
				system("cls");
				printf("You selected 'View Items In Stock'\n\n");
				display_items_quantity(items, total_items);
				break;
			case '3':
				system("cls");
				printf("You selected 'View Created Receipts'\n\n");
				system("start customer_receipts");
				system("start supplier_receipts");
				print_art("manager_actions_art");
				break;
			case '4':
				system("cls");
				printf("You selected 'View Transaction History'\n\n");
				display_transaction_history(transactions, total_transactions);
				break;
			default:
				printf("Please enter a valid option: ");
				break;
		}
		display_manager_options();
	}
	system("cls");
}

/*
FUNCTION: verify_user(char manager_password[100], int max_attempts)
DESCRIPTION: verifies and checks if the user is authorized to access this module
by asking for the password
ARGUMENTS: 
char manager_password[] - the password that the user needs to enter to access the different manager actions
int max_attemts - An integer representing the number of max attempts the user has to enter the correct password
RETURN: int (an integer)
1 - if the user entered the correct password
0 - if the user entered the wrong password
*/
int verify_user(char manager_password[100], int max_attempts)
{
	// create variables to hold user password input and number of attempts
	int i;
	char entered_password[100];
	
	// print login attempt art
	print_art("login_art");
	
	// prompt the user to enter their password
	// if they do not suceed within 3 tries, the function returns 0 and
	// they are sent back to the main menu
	printf("Welcome! To verify that you are allowed to view these records, you will be given %d attempts to enter your password\n\n\n", max_attempts);
	for(i = 0; i < max_attempts  && strcmp(manager_password, entered_password) != 0; i++)
	{
		// display number of attempts left and recieve password input from the user
		printf("Attempts left: %d\n", max_attempts - i);
		printf("Please enter your password: ");
		scanf("%s", &entered_password);
		
		// compare the two passwords if they match
		if(strcmp(manager_password, entered_password) != 0)
		{
			// if they don't match, tell the user
			printf("Incorrect password.");
			
			// if there is still an attempt left, have the user try again
			if(i < max_attempts - 1)
			{
				printf("Please try again. ");
			}
			printf("\n");
		}
	}
	// if the user has reached/exceeded max attempts, the function returns 0
	// otherwise, it returns 1 and the user may proceed
	return (i == max_attempts) && strcmp(manager_password, entered_password) != 0 ? 0 : 1;	
}

/*
FUNCTION: void display_manager_options()
DESCRIPTION: displays the possible options the user can select to
navigate the manager actions module
ARGUMENTS: none
RETURN: none (void)
*/
void display_manager_options()
{
	// display the options the user may navigate
	printf("1 - Change password\n");
	printf("2 - View items in stock\n");
	printf("3 - View Created Receipts\n");
	printf("4 - View Transaction History\n");
	printf("x - Exit\n\n");
}

/*
FUNCTION: void change_password(char manager_password[])
DESCRIPTION: changes the manager password to a new password with at least 8 
characters, 1 special character, and 1 number
ARGUMENTS: 
char manager_password[] - the password that the user needs to enter to access the different manager actions
RETURN: none (void)
*/
void change_password(char manager_password[])
{
	// create variables to hold password input
	char old_password[100], new_password[100];
	
	// recieve the user's old password
	printf("Please enter your old password: ");
	scanf("%s", &old_password);
	
	// if it does not match the current manager's password,
	// they are sent back to the manager navigation menu
	if(strcmp(old_password, manager_password) != 0)
	{
		printf("The password you entered is incorrect. \n\n");
		return;
	}
	
	// recieve input from the user again, this time for the new password
	printf("Please enter your new password: ");
	scanf("%s", &new_password);
	
	// comparee the old password and the new password if they are the same
	// if they are the same, tell the user that they can't be the same
	// and return to the manager naviagation menu
	if(strcmp(old_password, new_password) == 0)
	{
		printf("Your old password can't be the same as your old password. \n\n");
		return;
	}
	
	// if the length of the new password is less than 8 characters
	// tell the user that it needs to be longer
	// and return to the manager naviagation menu
	if(strlen(new_password) < 8)
	{
		printf("Your new password must be at least 8 characters long. \n\n");
		return;
	}
	
	
	// create variables to hold the index (i) and
	// if the new password will have a number and a special character
	int has_number = 0, has_special = 0, i;
	
	// loop through the string to check for numbers & special characters
	for(i = 0; i < strlen(new_password); i++)
	{
		char current = new_password[i];
		// if the current character is a digit, change the value
		// of has_number to 1
		if(isdigit(current))
		{
			has_number = 1;
		}  
		
		// if the current character is a special character
		// set the value of has_special to 1 (true)
		if((current >= 'A' && current <= 'Z') || (current >= 'a' && current <= 'z'))
		{
			continue;
		} else {
			has_special = 1;
		}
		
		// if the string has special character and number,
		// break out of the loop
		if(has_special && has_number)
		{
			break;
		}
	}
	
	// if both of these conditions are not met, inform the user
	// that the new password needs to have at least 1 number and special character
	// then return to the navigation menu
	if(!has_number || !has_special)
	{
		printf("Your new password must have at least 1 number and 1 special character.\n\n");
		return;
	}
	
	// change the manager's password to the new password
	// inform the user, then return to the navigation menu
	strcpy(manager_password, new_password);
	printf("Your password has been sucessfully changed: %s\n\n", manager_password);
}

