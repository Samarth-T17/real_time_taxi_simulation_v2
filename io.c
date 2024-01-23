#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#define ALPHABET_SIZE (26)
struct value_struct_passenger 
{
    char* pass;
    int id;
};
struct node_passenger
{
    char* key;
    struct value_struct_passenger* value;
    struct node_passenger* next;
};
struct hashMap_passenger 
{
    int numOfElements;
    int capacity;
    struct node_passenger** arr;
};
struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];
	bool isEndOfWord;
    int graph_node;
};
struct value_struct_taxi 
{
    int journey_completed;
    double rating;
};
struct node_taxi
{
    int key;
    struct value_struct_taxi* value;
    struct node_taxi* next;
};
struct hashMap_taxi
{
    int numOfElements;
    int capacity;
    struct node_taxi** arr;
};
void setNode_map_taxi(struct node_taxi* node, int key, int jc, double rating)
{
    int i = 0;
    node->key = key;
    struct value_struct_taxi * v = (struct value_struct_taxi*)malloc(sizeof(struct value_struct_taxi));
    node->value = v;
    node->value->journey_completed = jc;
    node->value->rating = rating;
    node->next = NULL;
    return;
}
void initializeHashMap_taxi(struct hashMap_taxi* mp)
{
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (struct node_taxi**)malloc(sizeof(struct node_taxi*) * mp->capacity);
    for(int i = 0; i < mp->capacity; i++) {
        mp->arr[i] = NULL;
    }
}
int hashFunction_taxi(struct hashMap_taxi* mp, int key)
{
    return key % mp->capacity;
}
void insert_map_taxi(struct hashMap_taxi* mp, int key, int jc, double rating)
{
    int bucketIndex = hashFunction_taxi(mp, key);
    struct node_taxi* newNode = (struct node_taxi*)malloc(sizeof(struct node_taxi));
    setNode_map_taxi(newNode, key, jc, rating);
    if (mp->arr[bucketIndex] == NULL)
    {
        mp->arr[bucketIndex] = newNode;
    }
    else 
    {   
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }
    mp->numOfElements++;
    return;
}
struct value_struct_taxi * search_map_taxi(struct hashMap_taxi* mp, int key)
{
    int bucketIndex = hashFunction_taxi(mp, key);
    struct node_taxi* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {
        if (bucketHead->key == key) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }
    return NULL;
}
void printHashMap_taxi(struct hashMap_taxi *map)
{
    if (map == NULL) {
        printf("HashMap is NULL\n");
        return;
    }
    printf("HashMap:\n");
    printf("Number of elements: %d\n", map->numOfElements);
    printf("Capacity: %d\n", map->capacity);
    for (int i = 0; i < map->capacity; i++)
    {
        struct node_taxi *current = map->arr[i];
        while (current != NULL) 
        {
            printf("id: %d\n", current->key);
            printf("Value - journey_completed: %d\n", current->value->journey_completed);
            printf("Value - rating : %lf\n", current->value->rating);
            printf("\n");
            current = current->next;
        }
    }
}
struct TrieNode * getNode_trie()
{
	struct TrieNode *pNode = NULL;
	pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
	if (pNode)
	{
		int i;
		pNode->isEndOfWord = false;
        pNode->graph_node = -1;
		for (i = 0; i < ALPHABET_SIZE; i++)
		{
			pNode->children[i] = NULL;
        }
	}
	return pNode;
}
void insert_trie(struct TrieNode *root, const char *key, int k)
{
	int level;
	int length = strlen(key);
	int index;
	struct TrieNode *pCrawl = root;

	for (level = 0; level < length; level++)
	{
		index = (int)key[level] - 'a';
		if (!pCrawl->children[index])
		{
            pCrawl->children[index] = getNode_trie();
        }
		pCrawl = pCrawl->children[index];
	}
    pCrawl->graph_node = k;
	pCrawl->isEndOfWord = true;
}
int search_trie(struct TrieNode *root, const char *key)
{
	int level;
	int length = strlen(key);
	int index;
	struct TrieNode *pCrawl = root;
	for (level = 0; level < length; level++)
	{
		index = key[level] - 'a';
		if (!pCrawl->children[index])
        {
            return -1;
        }
		pCrawl = pCrawl->children[index];
	}
	if(!pCrawl->isEndOfWord)
    {
        return -1;
    }
    else
    {
        return pCrawl->graph_node;
    }
}
void setNode_map_passenger(struct node_passenger* node, char* key, char* value, int id)
{
    int i = 0;
    node->key = (char*)malloc(strlen(key) + 1);
    strcpy(node->key, key);
    struct value_struct_passenger * v = (struct value_struct_passenger*)malloc(sizeof(struct value_struct_passenger));
    node->value = v;
    node->value->id = id;
    i = 0;
    node->value->pass = (char *)malloc(strlen(value) + 1);
    strcpy(node->value->pass, value);
    node->next = NULL;
    return;
}
void initializeHashMap_passenger(struct hashMap_passenger* mp)
{
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (struct node_passenger**)malloc(sizeof(struct node_passenger*) * mp->capacity);
    for(int i = 0; i < mp->capacity; i++) {
        mp->arr[i] = NULL;
    }
}
int hashFunction_passenger(struct hashMap_passenger* mp, char* key)
{
    int bucketIndex;
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(key); i++) {
        sum = ((sum % mp->capacity) + (((int)key[i]) * factor) % mp->capacity) % mp->capacity;
        factor = ((factor % __INT16_MAX__) * (31 % __INT16_MAX__)) % __INT16_MAX__;
    }
    bucketIndex = sum;
    return bucketIndex;
}
void insert_map_passenger(struct hashMap_passenger* mp, char* key, char* value, int id)
{
    for(int i = 0; i < strlen(key); i++) 
    {
        key[i] = tolower(key[i]);
    }
    int bucketIndex = hashFunction_passenger(mp, key);
    struct node_passenger* newNode = (struct node_passenger*)malloc(sizeof(struct node_passenger));
    setNode_map_passenger(newNode, key, value, id);
    if (mp->arr[bucketIndex] == NULL)
    {
        mp->arr[bucketIndex] = newNode;
    }
    else 
    {   
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }
    mp->numOfElements++;
    return;
}
struct value_struct_passenger * search_map_passenger(struct hashMap_passenger* mp, char* key)
{
    for(int i = 0; i < strlen(key); i++) 
    {
        key[i] = tolower(key[i]);
    }
    int bucketIndex = hashFunction_passenger(mp, key);
    struct node_passenger* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {
        if (strcasecmp(bucketHead->key, key) == 0) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }
    return NULL;
}
void take_passenger_from_file(struct hashMap_passenger * passengers) 
{
    FILE *inputFile = fopen("input_texts/passenger.txt", "r");
    if (inputFile == NULL)
    {
        perror("Error opening file");
        return;
    }
    char name[50];
    char password[50];
    int id;
    while (fscanf(inputFile, "%s %s %d", name, password, &id) == 3) 
    {
        insert_map_passenger(passengers, name, password, id);
    }
    fclose(inputFile);
}
int size_hashmap_passenger(struct hashMap_passenger * mp) 
{
    return mp->numOfElements;
}
void take_passenger_input(struct hashMap_passenger * passengers, char* passenger_file_name, struct TrieNode* root) 
{
    char name[30];
    printf("\nEnter Name : ");
    scanf("%*c%s", name);
    struct value_struct_passenger * ans = search_map_passenger(passengers, name);
    int ch;
    int id;
    printf("\n%p", ans);
    if(ans == NULL) 
    {
        printf("\nNo account with %s exists press 1 to create account : ", name);
        scanf("%d", &ch);
        if(ch != 1) 
        {
            return;
        }
        else 
        {
            id = size_hashmap_passenger(passengers) + 1;
            FILE *inputFile = fopen("input_texts/passenger.txt", "a");
            fprintf(inputFile, "\n%s %s %d", name, "12345", id);
            insert_map_passenger(passengers, name, "12345", id);
            fclose(inputFile);
        }
    }
    else
    {
        id = ans->id;
    }
    printf("\nEnter current Location : ");
    char location[20];
    int a, graph_node;
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
    printf("\nEnter required cab rating : ");
    double rating;
    scanf("%lf", &rating);
    printf("\nEnter Number of Locations : ");
    scanf("%d", &a);
    int loc_to_process[a + 1];
    loc_to_process[0] = graph_node;
    for(int i = 1; i < a + 1; i++)
    {
        printf("\nEnter Location %d : ", i - 1);
        scanf("%*c%s", location);
        graph_node = search_trie(root, location);
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
    char order;
    printf("\nEnter whether you want to travel in specified order (y, n) : ");
    scanf("%*c%c", &order);
    FILE *filePointer;
    filePointer = fopen(passenger_file_name, "w");
    if (filePointer == NULL) {
        printf("Unable to create the file.\n");
    }
    fprintf(filePointer, "%s %d %c %lf %d", name, id, order, rating, a + 1);
    for(int i = 0; i < a + 1; i++) {
        fprintf(filePointer, " %d", loc_to_process[i]);
    }
    fclose(filePointer);
}
void take_location_input(struct TrieNode * root) 
{
    FILE *inputFile = fopen("input_texts/locations.txt", "r");
    if (inputFile == NULL)
    {
        perror("Error opening file");
        return;
    }
    char location_name[50];
    int i = 1;
	while (fscanf(inputFile, " %[^\n]", location_name) != EOF) 
	{
    	insert_trie(root, location_name, i);
    	i++;
	}
    fclose(inputFile);
}
void convert_int_to_string(char * string, int n) 
{
    int digits = (int)log10(n);
    string[digits + 1] = '\0';
    for(;digits >= 0; digits--) 
    {
        string[digits] = n % 10 + 48;
        n = n / 10;
    }
}
void printHashMap_passenger(struct hashMap_passenger *map)
 {
    if (map == NULL) {
        printf("HashMap is NULL\n");
        return;
    }
    printf("HashMap:\n");
    printf("Number of elements: %d\n", map->numOfElements);
    printf("Capacity: %d\n", map->capacity);
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
void take_taxi_input(struct hashMap_taxi* taxi, char * taxi_file_input_name, struct TrieNode * root) 
{
    int search_id;
    struct value_struct_taxi* v = NULL;
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
    FILE *filePointer;
    filePointer = fopen(taxi_file_input_name, "w");
    if (filePointer == NULL) {
        printf("Unable to create the file.\n");
    }
    fprintf(filePointer, "%d %d %lf %d", search_id, graph_node, v->rating, v->journey_completed);
    fclose(filePointer);
}
void take_taxi_input_file(struct hashMap_taxi* taxi) 
{
    FILE *inputFile = fopen("input_texts/taxi.txt", "r");
    if (inputFile == NULL)
    {
        perror("Error opening file");
        return;
    }
    int taxi_id;
    int jc;
    double taxi_rating;
    while (fscanf(inputFile, "%d %d %lf", &taxi_id, &jc, &taxi_rating) != EOF) 
	{
        insert_map_taxi(taxi, taxi_id, jc, taxi_rating);
	}
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
        switch(ch) {
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