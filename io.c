#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>

#define ALPHABET_SIZE (26)

// Structure for passenger information
struct value_struct_passenger 
{
    char* pass; // Passenger name or identifier
    int id;     // Passenger ID
};

// Node structure for a linked list inside the hash map for passenger
struct node_passenger
{
    char* key; // Key (could be the name or identifier)
    struct value_struct_passenger* value; // Pointer to passenger information
    struct node_passenger* next; // Pointer to the next node in the linked list
};

// Hash map structure for passenger
struct hashMap_passenger 
{
    int numOfElements; // Number of elements in the hash map
    int capacity;      // Capacity of the hash map
    struct node_passenger** arr; // Array of pointers to nodes
};

// Trie Node structure for taxi information
struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE]; // Array of pointers to child nodes (26 for each alphabet letter)
	bool isEndOfWord; // Flag indicating if the node represents the end of a word
    int graph_node;   // Information about the node in a graph (related to taxi)
};

// Structure for taxi information
struct value_struct_taxi 
{
    int journey_completed; // Number of completed journeys
    double rating;         // Rating of the taxi
};

// Node structure for a linked list inside the hash map for taxi
struct node_taxi
{
    int key; // Key (unique identifier for taxi)
    struct value_struct_taxi* value; // Pointer to taxi information
    struct node_taxi* next; // Pointer to the next node in the linked list
};

// Hash map structure for taxi
struct hashMap_taxi
{
    int numOfElements; // Number of elements in the hash map
    int capacity;      // Capacity of the hash map
    struct node_taxi** arr; // Array of pointers to nodes
};

// Function:     setNode_map_taxi
// Description:  Sets the values of a node in the taxi hashmap.
// Input params: node - pointer to the node_taxi struct
//               key - integer key for the taxi
//               jc - integer representing the number of journeys completed
//               rating - double representing the taxi's rating
// Return Type:  void
void setNode_map_taxi(struct node_taxi* node, int key, int jc, double rating)
{
    // Initialize index i for loop
    int i = 0;

    // Set key value for the node
    node->key = key;

    // Allocate memory for the value_struct_taxi and assign it to the node
    struct value_struct_taxi * v = (struct value_struct_taxi*)malloc(sizeof(struct value_struct_taxi));
    node->value = v;

    // Set journey_completed and rating values for the node
    node->value->journey_completed = jc;
    node->value->rating = rating;

    // Set the next pointer of the node to NULL
    node->next = NULL;

    // Return from the function
    return;
}

// Function:     initializeHashMap_taxi
// Description:  Initializes the taxi hashmap with a given capacity.
// Input param:  mp - pointer to the hashMap_taxi struct
// Return Type:  void
void initializeHashMap_taxi(struct hashMap_taxi* mp)
{
    // Set capacity, number of elements, and allocate memory for the array
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (struct node_taxi**)malloc(sizeof(struct node_taxi*) * mp->capacity);

    // Initialize each bucket in the array to NULL
    for(int i = 0; i < mp->capacity; i++) {
        mp->arr[i] = NULL;
    }
}

// Function:     hashFunction_taxi
// Description:  Calculates the hash value for a given key in the taxi hashmap.
// Input params: mp - pointer to the hashMap_taxi struct
//               key - integer key for the taxi
// Return Type:  int - hash value for the given key
int hashFunction_taxi(struct hashMap_taxi* mp, int key)
{
    // Calculate and return the hash value
    return key % mp->capacity;
}

// Function:     insert_map_taxi
// Description:  Inserts a new node with the specified key, jc, and rating into the hash map.
// Input params: 
//               mp - pointer to the hashMap_taxi structure representing the hash map
//               key - integer key to identify the node
//               jc - integer value representing some property of the node
//               rating - double value representing the rating associated with the node
// Return Type:  void

void insert_map_taxi(struct hashMap_taxi* mp, int key, int jc, double rating)
{
    // Calculate the bucket index using the hash function
    int bucketIndex = hashFunction_taxi(mp, key);
    
    // Allocate memory for a new node
    struct node_taxi* newNode = (struct node_taxi*)malloc(sizeof(struct node_taxi));
    
    // Set the values of the new node
    setNode_map_taxi(newNode, key, jc, rating);
    
    // Check if the bucket is empty
    if (mp->arr[bucketIndex] == NULL)
    {
        mp->arr[bucketIndex] = newNode;
    }
    else 
    {   
        // Add the new node to the beginning of the linked list in the bucket
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }
    
    // Increment the number of elements in the hash map
    mp->numOfElements++;
    
    return;
}

// Function:     search_map_taxi
// Description:  Searches for a node with the specified key in the hash map.
// Input params: 
//               mp - pointer to the hashMap_taxi structure representing the hash map
//               key - integer key to identify the node
// Return Type:  pointer to struct value_struct_taxi representing the node's value if found, NULL otherwise

struct value_struct_taxi * search_map_taxi(struct hashMap_taxi* mp, int key)
{
    // Calculate the bucket index using the hash function
    int bucketIndex = hashFunction_taxi(mp, key);
    
    // Get the head of the linked list in the specified bucket
    struct node_taxi* bucketHead = mp->arr[bucketIndex];
    
    // Traverse the linked list to find the node with the specified key
    while (bucketHead != NULL) {
        if (bucketHead->key == key) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }
    
    // Node with the specified key not found
    return NULL;
}
// Function:     printHashMap_taxi
// Description:  Prints the contents of a hash map containing taxi information.
// Input param:  map - pointer to the hash map structure
// Return Type:  void

void printHashMap_taxi(struct hashMap_taxi *map)
{
    // Check if the input map is NULL
    if (map == NULL) {
        printf("HashMap is NULL\n");
        return;
    }

    // Print general information about the hash map
    printf("HashMap:\n");
    printf("Number of elements: %d\n", map->numOfElements);
    printf("Capacity: %d\n", map->capacity);

    // Iterate through each bucket in the hash map
    for (int i = 0; i < map->capacity; i++)
    {
        // Traverse the linked list in each bucket
        struct node_taxi *current = map->arr[i];
        while (current != NULL) 
        {
            // Print key and corresponding values
            printf("id: %d\n", current->key);
            printf("Value - journey_completed: %d\n", current->value->journey_completed);
            printf("Value - rating : %lf\n", current->value->rating);
            printf("\n");

            // Move to the next node in the linked list
            current = current->next;
        }
    }
}

// Function:     getNode_trie
// Description:  Allocates memory for and initializes a trie node.
// Input param:  NULL
// Return Type:  pointer to the allocated trie node structure

struct TrieNode *getNode_trie()
{
    // Allocate memory for a new trie node
    struct TrieNode *pNode = NULL;
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));

    // Check if memory allocation is successful
    if (pNode)
    {
        int i;
        pNode->isEndOfWord = false;
        pNode->graph_node = -1;

        // Initialize children pointers to NULL
        for (i = 0; i < ALPHABET_SIZE; i++)
        {
            pNode->children[i] = NULL;
        }
    }

    // Return the pointer to the allocated trie node
    return pNode;
}

// Function:     insert_trie
// Description:  inserts a key into a trie data structure
// Input params: root - pointer to the root of the trie
//               key - string to be inserted into the trie
//               k - integer representing a graph node associated with the key
// Return Type:  void
void insert_trie(struct TrieNode *root, const char *key, int k)
{
    // Variables for loop control and indexing
    int level;
    int length = strlen(key);
    int index;

    // Pointer to traverse the trie
    struct TrieNode *pCrawl = root;

    // Traverse the trie for each character in the key
    for (level = 0; level < length; level++)
    {
        // Calculate the index for the current character
        index = (int)key[level] - 'a';

        // If the node for the character doesn't exist, create a new node
        if (!pCrawl->children[index])
        {
            pCrawl->children[index] = getNode_trie();
        }

        // Move to the next node
        pCrawl = pCrawl->children[index];
    }

    // Set the graph_node and mark the end of the word in the trie
    pCrawl->graph_node = k;
    pCrawl->isEndOfWord = true;
}

// Function:     search_trie
// Description:  searches for a key in a trie data structure and returns the associated graph node
// Input params: root - pointer to the root of the trie
//               key - string to be searched in the trie
// Return Type:  int - the associated graph node on success
//                    -1 if the key is not found
int search_trie(struct TrieNode *root, const char *key)
{
    // Variables for loop control and indexing
    int level;
    int length = strlen(key);
    int index;

    // Pointer to traverse the trie
    struct TrieNode *pCrawl = root;

    // Traverse the trie for each character in the key
    for (level = 0; level < length; level++)
    {
        // Calculate the index for the current character
        index = key[level] - 'a';

        // If the node for the character doesn't exist, key is not found
        if (!pCrawl->children[index])
        {
            return -1;
        }

        // Move to the next node
        pCrawl = pCrawl->children[index];
    }

    // If the end of the word flag is not set, key is not found
    if (!pCrawl->isEndOfWord)
    {
        return -1;
    }
    else
    {
        // Return the associated graph node on success
        return pCrawl->graph_node;
    }
}

// Function:     setNode_map_passenger
// Description:  initializes a node_passenger structure with key, value, and id
// Input params: node - pointer to a node_passenger structure to be initialized
//               key - string representing the key for the node
//               value - string representing the value for the node
//               id - integer representing an identifier associated with the node
// Return Type:  void
void setNode_map_passenger(struct node_passenger *node, char *key, char *value, int id)
{
    // Variable for loop control
    int i = 0;

    // Allocate memory for the key and copy the content
    node->key = (char *)malloc(strlen(key) + 1);
    strcpy(node->key, key);

    // Allocate memory for the value structure
    struct value_struct_passenger *v = (struct value_struct_passenger *)malloc(sizeof(struct value_struct_passenger));

    // Set the value structure and assign the id
    node->value = v;
    node->value->id = id;

    // Allocate memory for the pass and copy the content
    i = 0;
    node->value->pass = (char *)malloc(strlen(value) + 1);
    strcpy(node->value->pass, value);

    // Set next pointer to NULL
    node->next = NULL;

    return;
}
// Function:    initializeHashMap_passenger
// Description: Initializes a hash map for passenger data.
// Input param: A pointer to the struct hashMap_passenger to be initialized.
// Return Type: Void

void initializeHashMap_passenger(struct hashMap_passenger* mp)
{
    // Set initial capacity of the hash map
    mp->capacity = 100;

    // Initialize the number of elements in the hash map
    mp->numOfElements = 0;

    // Allocate memory for the array of linked list nodes
    mp->arr = (struct node_passenger**)malloc(sizeof(struct node_passenger*) * mp->capacity);

    // Initialize each array element to NULL
    for(int i = 0; i < mp->capacity; i++) {
        mp->arr[i] = NULL;
    }
}


// Function:    hashFunction_passenger
// Description: Computes the hash value for a given key in the passenger hash map.
// Input param: A pointer to the struct hashMap_passenger containing capacity information
//              A character array representing the key for which hash needs to be computed.
// Return Type: Integer representing the bucket index in the hash map.

int hashFunction_passenger(struct hashMap_passenger* mp, char* key)
{
    int bucketIndex;
    int sum = 0, factor = 31;

    // Iterate through each character in the key
    for (int i = 0; i < strlen(key); i++) {
        // Update the sum using the ASCII value of the character and a factor
        sum = ((sum % mp->capacity) + (((int)key[i]) * factor) % mp->capacity) % mp->capacity;

        // Update the factor for the next character
        factor = ((factor % __INT16_MAX__) * (31 % __INT16_MAX__)) % __INT16_MAX__;
    }

    // Set the bucket index to the computed sum
    bucketIndex = sum;
    return bucketIndex;
}


// Function:    insert_map_passenger
// Description: Inserts a key-value pair into the passenger hash map.
// Input param: A pointer to the struct hashMap_passenger where the key-value pair will be inserted.
//              A character array representing the key to be inserted.
//              A character array representing the value associated with the key.
//              An integer ID associated with the key-value pair.
// Return Type: Void

void insert_map_passenger(struct hashMap_passenger* mp, char* key, char* value, int id)
{
    // Convert the key to lowercase for case-insensitive hashing
    for(int i = 0; i < strlen(key); i++) 
    {
        key[i] = tolower(key[i]);
    }

    // Get the bucket index using the hash function
    int bucketIndex = hashFunction_passenger(mp, key);

    // Allocate memory for a new node and set its values
    struct node_passenger* newNode = (struct node_passenger*)malloc(sizeof(struct node_passenger));
    setNode_map_passenger(newNode, key, value, id);

    // Check if the bucket is empty
    if (mp->arr[bucketIndex] == NULL)
    {
        mp->arr[bucketIndex] = newNode;
    }
    else 
    {   
        // If the bucket is not empty, insert the new node at the beginning of the linked list
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }

    // Increment the number of elements in the hash map
    mp->numOfElements++;
    return;
}
// Function:     search_map_passenger
// Description:  searches for a value in the hashmap associated with the given key
// Input params: mp - pointer to the hashmap structure
//               key - key to search in the hashmap
// Return Type:  pointer to the value structure if key is found
//               NULL otherwise
struct value_struct_passenger * search_map_passenger(struct hashMap_passenger* mp, char* key)
{
    // Convert key to lowercase for case-insensitive search
    for(int i = 0; i < strlen(key); i++) 
    {
        key[i] = tolower(key[i]);
    }

    // Calculate the bucket index using the hash function
    int bucketIndex = hashFunction_passenger(mp, key);

    // Traverse the linked list at the calculated bucket index
    struct node_passenger* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {
        // Compare keys in a case-insensitive manner
        if (strcasecmp(bucketHead->key, key) == 0) {
            // Return the value if key is found
            return bucketHead->value;
        }
        // Move to the next node in the linked list
        bucketHead = bucketHead->next;
    }

    // Return NULL if key is not found in the hashmap
    return NULL;
}

// Function:     take_passenger_from_file
// Description:  reads passenger data from a file and inserts it into the hashmap
// Input params: passengers - pointer to the hashmap structure
// Return Type:  void
void take_passenger_from_file(struct hashMap_passenger * passengers) 
{
    // Open the file for reading
    FILE *inputFile = fopen("input_texts/passenger.txt", "r");
    if (inputFile == NULL)
    {
        // Print an error message and return if file opening fails
        perror("Error opening file");
        return;
    }

    // Variables to store data read from the file
    char name[50];
    char password[50];
    int id;

    // Read data from the file until the end is reached
    while (fscanf(inputFile, "%s %s %d", name, password, &id) == 3) 
    {
        // Insert data into the hashmap
        insert_map_passenger(passengers, name, password, id);
    }

    // Close the file after reading is complete
    fclose(inputFile);
}

// Function:     size_hashmap_passenger
// Description:  returns the number of elements in the hashmap
// Input params: mp - pointer to the hashmap structure
// Return Type:  int - number of elements in the hashmap
int size_hashmap_passenger(struct hashMap_passenger * mp) 
{
    // Return the number of elements stored in the hashmap
    return mp->numOfElements;
}
// Function:     take_passenger_input
// Description:  Takes input from the user for passenger details, including name, current location, cab preferences, etc.
// Input Params: struct hashMap_passenger *passengers - pointer to a hash map storing passenger information
//               char* passenger_file_name - file name to store passenger details
//               struct TrieNode* root - pointer to the root of a trie data structure representing city locations
// Return Type:  void

void take_passenger_input(struct hashMap_passenger *passengers, char* passenger_file_name, struct TrieNode* root) 
{
    // Variable to store user's name
    char name[30];
    printf("\nEnter Name : ");
    
    // Read user's name from input
    scanf("%*c%s", name);

    // Search for the user in the passenger hash map
    struct value_struct_passenger *ans = search_map_passenger(passengers, name);

    // Variables to store user's choice, user's ID, and current location
    int ch;
    int id;
    printf("\n%p", ans);

    // Check if user does not exist in the hash map
    if(ans == NULL) 
    {
        printf("\nNo account with %s exists. Press 1 to create an account : ", name);
        
        // Read user's choice
        scanf("%d", &ch);

        // If user chooses not to create an account, return from the function
        if(ch != 1) 
        {
            return;
        }
        else 
        {
            // Create a new user account
            id = size_hashmap_passenger(passengers) + 1;
            FILE *inputFile = fopen("input_texts/passenger.txt", "a");
            
            // Write user details to a file
            fprintf(inputFile, "\n%s %s %d", name, "12345", id);
            insert_map_passenger(passengers, name, "12345", id);
            fclose(inputFile);
        }
    }
    else
    {
        // If user exists, retrieve the user ID
        id = ans->id;
    }

    // Prompt user for their current location
    printf("\nEnter current Location : ");
    char location[20];
    int a, graph_node;

    // Keep asking for location until a valid one is provided
    while(true) 
    {
        scanf("%*c%s", location);
        graph_node = search_trie(root, location);

        // Check if the location is valid
        if(graph_node != -1) 
        {
            break;
        }
        else
        {
            printf("\nNo location in the city with name %s", location);
            printf("\nEnter current Location : ");
        }
    }

    // Prompt user for cab preferences (rating, number of locations, order of travel)
    printf("\nEnter required cab rating : ");
    double rating;
    scanf("%lf", &rating);

    printf("\nEnter Number of Locations : ");
    scanf("%d", &a);
    int loc_to_process[a + 1];
    loc_to_process[0] = graph_node;

    // Prompt user for each location in the travel route
    for(int i = 1; i < a + 1; i++)
    {
        printf("\nEnter Location %d : ", i - 1);
        scanf("%*c%s", location);
        graph_node = search_trie(root, location);

        // Check if the location is valid
        if(graph_node != -1) 
        {
            loc_to_process[i] = graph_node;
        }
        else
        {
            printf("\nNo location in the city with name %s", location);
            printf("\nEnter Location %d : ", i - 1);
            i--;
        }
    }

    // Prompt user for travel order preference
    char order;
    printf("\nEnter whether you want to travel in specified order (y, n) : ");
    scanf("%*c%c", &order);

    // Open file to write user details
    FILE *filePointer;
    filePointer = fopen(passenger_file_name, "w");

    // Check if file creation was successful
    if (filePointer == NULL) {
        printf("Unable to create the file.\n");
    }

    // Write user details to file
    fprintf(filePointer, "%s %d %c %lf %d", name, id, order, rating, a + 1);

    // Write travel route to file
    for(int i = 0; i < a + 1; i++) {
        fprintf(filePointer, " %d", loc_to_process[i]);
    }

    // Close the file
    fclose(filePointer);
}
// Function:     take_location_input
// Description:  reads location names from a file and inserts them into a trie data structure
// Input param:  root - pointer to the root of the TrieNode structure
// Return Type:  void
void take_location_input(struct TrieNode * root) 
{
    // Open the file containing location names
    FILE *inputFile = fopen("input_texts/locations.txt", "r");
    
    // Check if the file was opened successfully
    if (inputFile == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Buffer to store each location name
    char location_name[50];

    // Index variable to assign unique IDs to locations
    int i = 1;

    // Read location names from the file and insert them into the trie
    while (fscanf(inputFile, " %[^\n]", location_name) != EOF) 
    {
        insert_trie(root, location_name, i);
        i++;
    }

    // Close the file after reading
    fclose(inputFile);
}

// Function:     convert_int_to_string
// Description:  converts an integer to a string representation
// Input param:  string - pointer to the character array to store the resulting string
//               n - the integer to be converted
// Return Type:  void
void convert_int_to_string(char * string, int n) 
{
    // Calculate the number of digits in the integer
    int digits = (int)log10(n);

    // Null-terminate the string
    string[digits + 1] = '\0';

    // Convert each digit of the integer to a character and store in the string
    for(; digits >= 0; digits--) 
    {
        string[digits] = n % 10 + 48;
        n = n / 10;
    }
}

// Function:     printHashMap_passenger
// Description:  prints information about the elements in a hash map of passenger data
// Input param:  map - pointer to the hashMap_passenger structure
// Return Type:  void
void printHashMap_passenger(struct hashMap_passenger *map)
 {
    // Check if the hash map is NULL
    if (map == NULL) {
        printf("HashMap is NULL\n");
        return;
    }

    // Print information about the hash map
    printf("HashMap:\n");
    printf("Number of elements: %d\n", map->numOfElements);
    printf("Capacity: %d\n", map->capacity);

    // Iterate through the hash map and print key-value pairs
    for (int i = 0; i < map->capacity; i++)
    {
        struct node_passenger *current = map->arr[i];
        while (current != NULL) 
        {
            printf("Key: %s\n", current->key);
            printf("Value - Password: %s\n", current->value->pass);
            printf("Value - ID: %d\n", current->value->id);
            printf("\n");
            current = current->next;
        }
    }
}
// Function:    take_taxi_input
// Description: Takes input for a taxi, including taxi ID, current location, and stores the information in a file
// Input params: taxi - pointer to the hashMap_taxi structure containing taxi information
//               taxi_file_input_name - name of the file to store taxi information
//               root - pointer to the TrieNode structure representing the trie for city locations
// Return Type: void

void take_taxi_input(struct hashMap_taxi* taxi, char * taxi_file_input_name, struct TrieNode * root) 
{
    // Variables for input and search
    int search_id;
    struct value_struct_taxi* v = NULL;

    // Loop until a valid taxi ID is entered
    while (v == NULL)
    {
        printf("\nEnter Taxi ID : ");
        scanf("%d", &search_id);
        v = search_map_taxi(taxi, search_id);  
        if(v == NULL) 
        {
            printf("\nNo taxi with ID %d found", search_id);
        }
    }

    // Input current location and validate it using the trie
    printf("\nEnter current Location : ");
    char location[20];
    int graph_node;
    while(true) {
        scanf("%*c%s", location);
        graph_node = search_trie(root, location);
        if(graph_node != -1) {
            break;
        }
        else
        {
            printf("\nNo location in the city with name %s", location);
            printf("\nEnter current Location : ");
        }
    }

    // Open file to store taxi information
    FILE *filePointer;
    filePointer = fopen(taxi_file_input_name, "w");
    if (filePointer == NULL) {
        printf("Unable to create the file.\n");
    }

    // Write taxi information to the file
    fprintf(filePointer, "%d %d %lf %d", search_id, graph_node, v->rating, v->journey_completed);
    fclose(filePointer);
}

// Function:    take_taxi_input_file
// Description: Reads taxi information from a file and inserts it into the hashMap_taxi structure
// Input params: taxi - pointer to the hashMap_taxi structure to store taxi information
// Return Type: void

void take_taxi_input_file(struct hashMap_taxi* taxi) 
{
    // Open the input file
    FILE *inputFile = fopen("input_texts/taxi.txt", "r");
    if (inputFile == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Variables to store taxi information
    int taxi_id;
    int jc;
    double taxi_rating;

    // Loop through the file and insert taxi information into the hashMap_taxi structure
    while (fscanf(inputFile, "%d %d %lf", &taxi_id, &jc, &taxi_rating) != EOF) 
    {
        insert_map_taxi(taxi, taxi_id, jc, taxi_rating);
    }

    // Close the input file
    fclose(inputFile);
}
int main() {
    int ch;
    struct hashMap_passenger passengers;
    struct TrieNode *root = getNode_trie();
    int num_passengers = 1;
    int num_taxis = 1;
    int length;
    initializeHashMap_passenger(&passengers);
    struct hashMap_taxi taxi;
    initializeHashMap_taxi(&taxi);
    char int_to_string[4];
    char passenger_file_name[40] = "communication/passenger_input";
    char taxi_file_name[40] = "communication/taxi_input";
    take_passenger_from_file(&passengers);
    take_location_input(root);
    take_taxi_input_file(&taxi);
    printHashMap_passenger(&passengers);
    printHashMap_taxi(&taxi);
    while(1) {
        printf("\nMenu : ");
        printf("\n1. Customer Login\n");
        printf("2. Enter Taxi into system\n");
        printf("3. Report Accident\n");
        printf("\n");
        printf("Enter choice : ");
        scanf("%d", &ch);
        switch (ch) {
            case 1 :
                convert_int_to_string(int_to_string, num_passengers);
                strcat(passenger_file_name, int_to_string);
                strcat(passenger_file_name, ".txt");
                take_passenger_input(&passengers, passenger_file_name, root);
                passenger_file_name[strlen(passenger_file_name) - 4] = '\0';
                length =  strlen(passenger_file_name) - strlen(int_to_string);
                passenger_file_name[length] = '\0';
                num_passengers++;
                break;
            case 2 :
                convert_int_to_string(int_to_string, num_taxis);
                strcat(taxi_file_name, int_to_string);
                strcat(taxi_file_name, ".txt");
                take_taxi_input(&taxi, taxi_file_name, root);
                taxi_file_name[strlen(taxi_file_name) - 4] = '\0';
                length =  strlen(taxi_file_name) - strlen(int_to_string);
                taxi_file_name[length] = '\0';
                num_taxis++;
                break;
        }
    }
}