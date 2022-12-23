// create a custom struct ITEM
// which has the following properties:
// char name[100] - holds the name of the item in stock
// char description[100] - holds a short description of the item in stock
// int quantity - holds the number of items in stock
// float price - holds the price of that item per unit
typedef struct
{
	char name[100];
	char description[100];
	int quantity;
	float price;
} __attribute__((__packed__))
ITEM;

// items.c function prototypes


void load_items(ITEM items[]);
int get_total_items();
void update_items(ITEM items[], int new_length);
int item_exists(char item[], ITEM items[], int total_items);
int get_left_quantity(char item[], ITEM items[], int total_items);
float get_price(char item[], ITEM items[], int total_items);
void edit_item_quantity(ITEM items[], int total_items, char item[], int quantity);
void display_items_quantity(ITEM items[], int total_items);


