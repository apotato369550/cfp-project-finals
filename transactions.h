// create a custom struct TRANSACTION
// which has the following properties:
// char date[100] - holds the string representation of the current date
// char type[10] - holds the type of transaction
// char item[100] - holds the name of the item being transacted
// int quantity - holds the quantity of the item being transacted
// float price - holds the total price of the items being transacted
typedef struct
{
	char date[100];
	char type[10];
	char item[100];
	int quantity;
	float price;
} __attribute__((__packed__))
TRANSACTION;

// transactions.c function prototypes
void load_transactions(TRANSACTION transactions[]);
int get_total_transactions();
void update_transactions(TRANSACTION transactions[], int new_length);
void add_transaction(TRANSACTION transactions[], int index, char item[], int quantity, float total_price, char type[]);
void display_transaction_history(TRANSACTION transactions[], int total_transactions);
