#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<limits.h>
#include<string.h>
#include<math.h>
#include <unistd.h>
#define INF 99999
#define MAX_TAXIS 100
#define num_locations 17
struct value_struct_location 
{
    char* value;
};
struct node_location
{
    int key;
    struct value_struct_location* value;
    struct node_location* next;
};
struct hashMap_location
{
    int numOfElements;
    int capacity;
    struct node_location** arr;
};
void setNode_map_location(struct node_location* node, int key, char* value)
{
    int i = 0;
    node->key = key;
    struct value_struct_location * v = (struct value_struct_location*)malloc(sizeof(struct value_struct_location));
    node->value = v;
    node->value->value = (char*)malloc(15);
    strcpy(node->value->value, value);
    node->next = NULL;
    return;
}
void initializeHashMap_location(struct hashMap_location* mp)
{
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (struct node_location**)malloc(sizeof(struct node_location*) * mp->capacity);
    for(int i = 0; i < mp->capacity; i++) {
        mp->arr[i] = NULL;
    }
}
int hashFunction_location(struct hashMap_location* mp, int key)
{
    return key % mp->capacity;
}
void insert_map_location(struct hashMap_location* mp, int key, char * value)
{
    int bucketIndex = hashFunction_location(mp, key);
    struct node_location* newNode = (struct node_location*)malloc(sizeof(struct node_location));
    setNode_map_location(newNode, key, value);
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
struct value_struct_location * search_map_location(struct hashMap_location* mp, int key)
{
    int bucketIndex = hashFunction_location(mp, key);
    struct node_location* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {
        if (bucketHead->key == key) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }
    return NULL;
}
void printHashMap_location(struct hashMap_location *map)
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
        struct node_location *current = map->arr[i];
        while (current != NULL) 
        {
            printf("id: %d\n", current->key);
            printf("Value - %s\n", current->value->value);
            printf("\n");
            current = current->next;
        }
    }
}
struct my_pair
{
    int first;
    int second;
};
struct list
{
    int n;
    struct my_pair pairs[10];
    bool possible[10];
    int* landmarks;
    int n_landmarks;
};

struct node
{
    int data;
    struct node * next;
};
struct location
{
    int part;
    int node;
    int prev_node;
    struct node * head;
    struct node * destination;
    int remaining_time;
};
struct passenger
{
    char* name;
    int id;
    int cur_location;
    int required_rating;
    bool required_order;
    int num_given_locations;
    int locations[num_locations];
};
struct taxi
{
    int taxi_id;
    int fare;
    struct location* path_location;
    bool is_engaged;
    double rating;
    int journey_completed;
    struct passenger* p;
};
struct fleet
{
    struct taxi * fleet[MAX_TAXIS];
    int num_taxis;
};
int get_number(char str[100], int* i) {
    int ans = 0;
    for(; str[*i] < 58 && str[*i] > 47 ; *i= *i + 1){
        ans = ans * 10 + str[*i] - 48;
    }
    return ans;
}

void take_map_input(int adj_mat[num_locations][num_locations], struct list* graph[num_locations])
{    
    for(int i = 0; i < num_locations; i++)
    {
        for(int j = 0; j < num_locations; j++) {
            adj_mat[i][j] = INF;
        }
    }
    FILE *inputFile2 = fopen("input_texts/landmarks.txt", "r");
    FILE *inputFile = fopen("input_texts/in1.txt", "r");
    if (inputFile == NULL)
    {
        perror("Error opening file");
        return;
    }
    char line[100]; 
    int num;
    int i = 1;
    int edge;
    while (fgets(line, sizeof(line), inputFile) != NULL) 
    {   
        int j = 0;
        int len = strlen(line);
        graph[i]->n = 0;
        while(j < len) 
        {
            num = get_number(line, &j);
            j++;
            edge = get_number(line, &j);
            j++;
            graph[i]->pairs[graph[i]->n].first = num;
            graph[i]->pairs[graph[i]->n].second = edge;
            graph[i]->possible[graph[i]->n] = true;
            adj_mat[i][num] = edge;
            graph[i]->n++;
        }
        adj_mat[i][i] = 0;
        i++;
    }
    fclose(inputFile);
}
void calculate_floyd(int adj_mat[num_locations][num_locations], struct my_pair floyd[num_locations][num_locations]) 
{
    for(int i = 0; i < num_locations; i++) 
    {
        for(int j = 0; j < num_locations; j++) 
        {
            floyd[i][j].first = adj_mat[i][j];
            floyd[i][j].second = -1;
        }
    }
    for (int k = 0; k < num_locations; k++) 
    {
        for (int i = 0; i < num_locations; i++) 
        {
            for (int j = 0; j < num_locations; j++) 
            {
                if (floyd[i][k].first != INF && floyd[k][j].first != INF && floyd[i][k].first + floyd[k][j].first < floyd[i][j].first) 
                {
                    floyd[i][j].first = floyd[i][k].first + floyd[k][j].first;
                    floyd[i][j].second = k;
                }
            }
        }
    }
}
struct node * createnode(int e) 
{
    struct node * p = (struct node *)(malloc(sizeof(struct node)));
    p->data = e;
    p->next = NULL;
    return p;
}
void add(int e, struct node * head) 
{
    struct node * p = createnode(e);
    p->next = head->next;
    head->next = p;
} 
void floyd_path(struct my_pair floyd[num_locations][num_locations], int s, int d, struct node * head) 
{
    if(floyd[s][d].second == -1) {
        return;
    }
    int via_node = floyd[s][d].second;
    add(via_node, head);
    floyd_path(floyd, s, via_node, head);
    floyd_path(floyd, via_node, d, head->next);
}
void track_path(struct my_pair floyd[num_locations][num_locations], int s, int d, struct node ** head , struct taxi *a) 
{
    if(s == d) {
        return;
    }
    if(*head == NULL){
        *head = createnode(s);
    }
    struct node * temp = *head;
    add(d, *head);
    a->path_location->destination = (*head)->next;
    a->path_location->remaining_time += floyd[s][d].first;
    floyd_path(floyd, s, d, temp);
}
void swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void generate_permutations(int arr[], int start, int end, int ans[], struct my_pair floyd[num_locations][num_locations], int min_distance) 
{
    if (start == end) 
    {
        int cur_cost = 0;
        for (int i = 1; i <= end + 1; i++) 
        {
            cur_cost += floyd[i][i - 1].first;
        }
        if(cur_cost < min_distance) 
        {
            min_distance = cur_cost;
            for(int i = 0; i <= end + 1; i++)
            {
                ans[i] = arr[i];
            }
        }
    } 
    else 
    {
        for (int i = start; i <= end; i++) 
        {
            swap(&arr[start], &arr[i]);
            generate_permutations(arr, start + 1, end, ans, floyd, min_distance);
            swap(&arr[start], &arr[i]);
        }
    }
}
void allocate_taxi(struct fleet * fleet, struct passenger *p, struct my_pair floyd[num_locations][num_locations])
{
    struct taxi *min;
    int min_time = INT_MAX;
    struct taxi *taxi_cur;
    int p_location = p->cur_location;
    int required_rating = p->required_rating;
    int cur_time;
    for(int i = 0; i < fleet->num_taxis; i++) 
    {
        taxi_cur = fleet->fleet[i];
        if(taxi_cur->rating < required_rating) 
        {
            printf("\nNo taxi with required rating found");
            return;
        }
        if(taxi_cur->path_location->destination != NULL) {
            cur_time = taxi_cur->path_location->remaining_time + floyd[taxi_cur->path_location->destination->data][p_location].first;
        }
        else 
        {
            cur_time = taxi_cur->path_location->remaining_time + floyd[taxi_cur->path_location->node][p_location].first;
        }
        printf("cur_time %d", cur_time);
        if(cur_time < min_time) 
        {
            min_time = cur_time;
            min = taxi_cur;
        }
    }
    if(min->path_location->head == NULL) 
    {     
        track_path(floyd, min->path_location->node, p_location, &min->path_location->head, min);
    }
    else
    {
        track_path(floyd, min->path_location->destination->data, p_location, &min->path_location->destination, min);
    }
    if(p->required_order || p->num_given_locations <= 2) 
    { 
        for(int i = 0; i < p->num_given_locations; i++) 
        {
            if(min->path_location->head == NULL) 
            {
                track_path(floyd, min->path_location->node, p->locations[i], &min->path_location->head, min);
            }
            else
            {
                track_path(floyd, min->path_location->destination->data, p->locations[i], &min->path_location->destination, min);
            }
        }
    }
    else 
    {
        int ans[p->num_given_locations];
        generate_permutations(p->locations, 1, p->num_given_locations - 2, ans, floyd, INT_MAX);
        for(int i = 1; i < p->num_given_locations; i++) 
        {
            if(min->path_location->head == NULL) 
            {
                track_path(floyd, min->path_location->node, ans[i], &min->path_location->head, min);
            }
            else
            {
                track_path(floyd, min->path_location->destination->data, ans[i], &min->path_location->destination, min);
            }
        }
    }
    min->fare = min->path_location->remaining_time;
    struct node * cur_ptr = min->path_location->head;
    printf("\n");
    while(cur_ptr) 
    {
        printf("%d ->", cur_ptr->data);
        cur_ptr = cur_ptr->next;
    }
}   
void take_location_input(struct hashMap_location * map) 
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
    	insert_map_location(map, i, location_name);
    	i++;
	}
    fclose(inputFile);
}
int partition(struct fleet * fleet, int low, int high)
{
    double pivot = fleet->fleet[high]->rating;
    int i = low - 1;
    struct taxi * temp;
    for(int j = low; j <= high; j++)
    {
        if(fleet->fleet[j]->rating > pivot)
        {
            i++;
            temp = fleet->fleet[i];
            fleet->fleet[i] = fleet->fleet[j];
            temp = fleet->fleet[j];
        }
    }
    temp = fleet->fleet[i + 1];
    fleet->fleet[i + 1] = fleet->fleet[high];
    temp = fleet->fleet[high];
    return i + 1;
}
void quickSort(struct fleet * fleet, int low, int high)
{
    if(low < high)
    {
        int pi = partition(fleet, low, high);
        quickSort(fleet, low, pi-1);
        quickSort(fleet, pi+1, high);
    }
}
void take_taxi_input(struct fleet * fleet, FILE * inputFile) 
{
    struct location* taxi_location = (struct location*)malloc(sizeof(struct location));
    taxi_location->part = 0;
    taxi_location->head = NULL;
    taxi_location->prev_node = 0;
    taxi_location->remaining_time = 0;
    taxi_location->destination = NULL;
    struct taxi * a = (struct taxi*)malloc(sizeof(struct taxi));
    a->path_location = taxi_location;
    fscanf(inputFile, "%d %d %lf %d", &a->taxi_id, &a->path_location->node, &a->rating, &a->journey_completed);
    a->fare = 0;
    a->is_engaged = false;
    a->journey_completed = 0;
    a->p = NULL;
    fleet->num_taxis++;
    fleet->fleet[fleet->num_taxis - 1] = a;
    quickSort(fleet, 0, fleet->num_taxis - 1);
}
void take_passenger_input(struct fleet * fleet, struct my_pair floyd[num_locations][num_locations], FILE * inputFile)
{
    char order;
    struct passenger* p = (struct passenger *)malloc(sizeof(struct passenger));
    p->name = (char *)malloc(40);
    fscanf(inputFile, "%s %d %c %lf %d", p->name, &p->cur_location, &order, &p->required_rating, &p->num_given_locations);
    if(order == 'y') 
    {
        p->required_order = false;
    }
    else
    {
        p->required_order = true;
    }
    for(int i = 0; i < p->num_given_locations; i++) 
    {
        fscanf(inputFile, "%d", &p->locations[i]);
    }
    allocate_taxi(fleet, p, floyd);
}
void move_taxi(struct fleet * fleet, struct my_pair floyd[num_locations][num_locations], struct hashMap_location *map) 
{
    struct taxi * cur;
    struct location * cur_path_locaction;
    struct node* a;
    for(int i = 0; i < fleet->num_taxis; i++) 
    {
        cur = fleet->fleet[i];
        cur_path_locaction = cur->path_location;
        if(cur_path_locaction->head != NULL)
        {
            cur_path_locaction->part++;
            cur_path_locaction->remaining_time--;
            if(floyd[cur_path_locaction->node][cur_path_locaction->head->next->data].first == cur_path_locaction->part) 
            {
                cur_path_locaction->part = 0;
                a = cur_path_locaction->head;
                cur_path_locaction->head = cur_path_locaction->head->next;
                cur_path_locaction->prev_node = cur_path_locaction->node;
                cur_path_locaction->node = cur_path_locaction->head->data;
                free(a);
            }
            if(cur_path_locaction->head->next == NULL && cur_path_locaction->part == 0) 
            {
                double rating;
                free(cur_path_locaction->head);
                cur_path_locaction->head = NULL;
                cur_path_locaction->destination = NULL;
                printf("\nTaxi %d reached destination", cur->taxi_id);
                printf("\nFare for passenger %d\n", (cur->fare * 10));
                printf("\nEnter rating of taxi ID %d : ", cur->taxi_id);
                scanf("%lf", &rating);
                cur->rating = (cur->journey_completed * cur->rating + rating) / (cur->journey_completed + 1);
                cur->journey_completed++;
                quickSort(fleet, 0, fleet->num_taxis - 1);
            }
            else 
            {
                if(cur_path_locaction->part == 0) 
                {
                    printf("\nTaxi %d at %s", cur->taxi_id, search_map_location(map, cur_path_locaction->node)->value);
                }
                else 
                {
                    printf("\nTaxi %d at %d / %d way from %s", cur->taxi_id, cur_path_locaction->part, floyd[cur_path_locaction->node][cur_path_locaction->head->next->data].first, search_map_location(map, cur_path_locaction->head->next->data)->value);
                }
            }
        }
    }
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
void process(struct fleet *fleet, struct my_pair floyd[num_locations][num_locations], struct hashMap_location * map) 
{
    int num_passengers = 1;
    int num_taxis = 1;
    int length;
    char passenger_file_name[40] = "communication/passenger_input";
    char taxi_file_name[40] = "communication/taxi_input";
    char number[10];
    convert_int_to_string(number, num_passengers);
    strcat(passenger_file_name, number);
    convert_int_to_string(number, num_taxis);
    strcat(taxi_file_name, number);   
    FILE *inputFile;
    strcat(passenger_file_name, ".txt");
    strcat(taxi_file_name, ".txt");
    while(true) 
    {
        move_taxi(fleet, floyd, map);
        printf("\n\n\n");
        while(true)
        {
            inputFile = fopen(taxi_file_name, "r");
            if(inputFile != NULL) 
            {
                take_taxi_input(fleet, inputFile);
                fclose(inputFile);
                remove(taxi_file_name);
                printf("%s", taxi_file_name);
                taxi_file_name[strlen(taxi_file_name) - 4] = '\0';
                length =  strlen(taxi_file_name) - strlen(number);
                taxi_file_name[length] = '\0';
                num_taxis++;
                convert_int_to_string(number, num_taxis);
                strcat(taxi_file_name, number);
                strcat(taxi_file_name, ".txt");
            }
            else
            {
                break;
            }
        }
        while(true)
        {
            inputFile = fopen(passenger_file_name, "r");
            if(inputFile != NULL) 
            {
                take_passenger_input(fleet, floyd, inputFile);
                fclose(inputFile);
                remove(passenger_file_name);
                passenger_file_name[strlen(passenger_file_name) - 4] = '\0';
                length =  strlen(passenger_file_name) - strlen(number);
                passenger_file_name[length] = '\0';
                num_passengers++;
                convert_int_to_string(number, num_passengers);
                strcat(passenger_file_name, number);
                strcat(passenger_file_name, ".txt");
            }
            else
            {
                break;
            }
        }
        sleep(1); 
    }
}

int main() 
{
    int adj_mat[num_locations][num_locations];
    struct list* graph[num_locations];
    for(int i = 0; i < num_locations; i++) {
        graph[i] = (struct list *) malloc(sizeof(struct list));
    }
    take_map_input(adj_mat, graph);
    for(int i = 0; i < num_locations; i++) {
        for(int j = 0; j < num_locations; j++) {
            printf("%d ", adj_mat[i][j]);
        }
        printf("\n");
    }
    for(int i = 0; i < num_locations; i++) {
        for(int j = 0; j < graph[i]->n; j++) {
            printf("\n node %d connected with %d with weight %d", i, graph[i]->pairs[j].first , graph[i]->pairs[j].second);
        }
    }
//    struct my_pair floyd[num_locations][num_locations];
//    calculate_floyd(adj_mat, floyd);
//    struct hashMap_location map;
//    initializeHashMap_location(&map);
//    take_location_input(&map);
//    struct fleet fleet;
//    fleet.num_taxis = 0;
//    process(&fleet, floyd, &map);
}