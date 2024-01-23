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

// Structure to represent a pair of integers
struct my_pair
{
    int first;
    int second;
};

// Structure to represent a list of pairs
struct list
{
    int n;
    struct my_pair pairs[10];
};



// Structure to represent a node in a linked list
struct node
{
    int data;
    struct node * next;
};

// Structure to represent a location with additional information
struct location
{
    int part;               // Represents a part of the system
    int node;               // Represents a node in the graph
    int prev_node;          // Represents the previous node in the shortest path
    struct node * head;     // Points to the head of a linked list
    struct node * destination;  // Points to the destination node in the linked list
    int remaining_time;     // Represents the remaining time to reach the destination
};

// Structure to represent a passenger with details
struct passenger
{
    char* name;              // Name of the passenger
    int id;                  // Passenger's ID
    int cur_location;        // Current location of the passenger
    int required_rating;     // Required rating for the taxi
    bool required_order;     // Required order of locations for the journey
    int num_given_locations; // Number of given locations for the journey
    int locations[num_locations]; // Array to store the given locations
};

// Structure to represent a taxi with details
struct taxi
{
    int taxi_id;             // Taxi's ID
    int fare;                // Fare for the taxi ride
    struct location* path_location; // Path information for the taxi
    bool is_engaged;         // Indicates if the taxi is engaged
    double rating;           // Rating of the taxi
    int journey_completed;   // Number of journeys completed by the taxi
    struct passenger* p;     // Pointer to the passenger currently in the taxi
};

// Structure to represent a fleet of taxis
struct fleet
{
    struct taxi * fleet[MAX_TAXIS]; // Array of taxi pointers
    int num_taxis;                   // Number of taxis in the fleet
};

// Function:      get_number
// Description:   extracts and converts a number from the given character array
// Input param:   str - character array containing numbers
//                i   - pointer to an integer storing the current index in the array
// Return Type:   integer - the extracted number
int get_number(char str[100], int* i) {
    int ans = 0;
    // Iterate through the characters in the array until a non-digit character is encountered
    for (; str[*i] < 58 && str[*i] > 47 ; *i= *i + 1) {
        ans = ans * 10 + str[*i] - 48;  // Convert character to integer and accumulate the number
    }
    return ans;
}

// Function:      take_map_input
// Description:   reads a map from a file and populates an adjacency matrix with edge weights
// Input param:   adj_mat - 2D array representing the adjacency matrix
// Return Type:   void
void take_map_input(int adj_mat[num_locations][num_locations]) 
{
    // Initialize the adjacency matrix with infinity as the default edge weight
    for (int i = 0; i < num_locations; i++)
    {
        for (int j = 0; j < num_locations; j++) {
            adj_mat[i][j] = INF;
        }
    }

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

    // Read each line from the file
    while (fgets(line, sizeof(line), inputFile) != NULL) 
    {   
        int j = 0;
        int len = strlen(line);

        // Extract the numbers from the line and populate the adjacency matrix
        while(j < len) {
            num = get_number(line, &j);
            j++;
            edge = get_number(line, &j);
            j++;
            adj_mat[i][num] = edge;
        }
        adj_mat[i][i] = 0;  // Set diagonal elements to 0
        i++;
    }

    fclose(inputFile);
}
// Function:     calculate_floyd
// Description:  calculates the shortest paths between locations using Floyd-Warshall algorithm
// Input param:  adj_mat - 2D array representing the adjacency matrix of locations
//               floyd - 2D array of struct my_pair to store shortest distances and intermediate vertices
// Return Type:  void
void calculate_floyd(int adj_mat[num_locations][num_locations], struct my_pair floyd[num_locations][num_locations]) 
{
    // Initialize floyd array with input adjacency matrix values
    for(int i = 0; i < num_locations; i++) 
    {
        for(int j = 0; j < num_locations; j++) 
        {
            floyd[i][j].first = adj_mat[i][j];
            floyd[i][j].second = -1;
        }
    }

    // Perform Floyd-Warshall algorithm to find shortest paths
    for (int k = 0; k < num_locations; k++) 
    {
        for (int i = 0; i < num_locations; i++) 
        {
            for (int j = 0; j < num_locations; j++) 
            {
                // Update if a shorter path is found through vertex k
                if (floyd[i][k].first != INF && floyd[k][j].first != INF && floyd[i][k].first + floyd[k][j].first < floyd[i][j].first) 
                {
                    floyd[i][j].first = floyd[i][k].first + floyd[k][j].first;
                    floyd[i][j].second = k;
                }
            }
        }
    }
}

// Function:     createnode
// Description:  creates a new node for a linked list with given element
// Input param:  e - element to be stored in the node
// Return Type:  pointer to the newly created node
struct node * createnode(int e) 
{
    // Allocate memory for the new node
    struct node * p = (struct node *)(malloc(sizeof(struct node)));
    
    // Initialize node with input element and NULL next pointer
    p->data = e;
    p->next = NULL;
    
    return p;
}

// Function:     add
// Description:  adds a new node with the given element to the linked list
// Input param:  e - element to be added to the linked list
//               head - pointer to the head of the linked list
// Return Type:  void
void add(int e, struct node * head) 
{
    // Create a new node with the given element
    struct node * p = createnode(e);
    
    // Insert the new node at the beginning of the linked list
    p->next = head->next;
    head->next = p;
}
// Function:     floyd_path
// Description:  Recursively generates the path between source (s) and destination (d) using Floyd-Warshall algorithm
// Input param:  floyd - 2D array storing intermediate nodes and distances between locations
//               s - source location index
//               d - destination location index
//               head - pointer to the head of the linked list representing the path
// Return Type:  void

void floyd_path(struct my_pair floyd[num_locations][num_locations], int s, int d, struct node * head) 
{
    // Base case: If the intermediate node for the current pair is -1, the path generation is complete
    if(floyd[s][d].second == -1) {
        return;
    }

    // Determine the intermediate node in the path and add it to the linked list
    int via_node = floyd[s][d].second;
    add(via_node, head);

    // Recursively generate the path segments between source and intermediate node, and intermediate node and destination
    floyd_path(floyd, s, via_node, head);
    floyd_path(floyd, via_node, d, head->next);
}

// Function:     track_path
// Description:  Creates a linked list representing the path between source (s) and destination (d) and updates taxi information
// Input param:  floyd - 2D array storing intermediate nodes and distances between locations
//               s - source location index
//               d - destination location index
//               head - pointer to the pointer of the head of the linked list representing the path
//               a - pointer to the taxi structure containing path and time information
// Return Type:  void

void track_path(struct my_pair floyd[num_locations][num_locations], int s, int d, struct node ** head , struct taxi *a) 
{
    // Base case: If source and destination are the same, no path needs to be tracked
    if(s == d) {
        return;
    }

    // If the linked list representing the path is empty, create a node for the source
    if(*head == NULL){
        *head = createnode(s);
    }

    // Add the destination to the linked list and update taxi information
    add(d, *head);
    a->path_location->destination = (*head)->next;
    a->path_location->remaining_time += floyd[s][d].first;

    // Recursively generate the path segments using Floyd-Warshall algorithm
    floyd_path(floyd, s, d, *head);
}

// Function:     swap
// Description:  Swaps the values of two integers
// Input param:  a - pointer to the first integer
//               b - pointer to the second integer
// Return Type:  void

void swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function:     generate_permutations
// Description:  Generates all permutations of an array and updates the minimum distance and corresponding path
// Input param:  arr - array of integers to be permuted
//               start - starting index for permutation
//               end - ending index for permutation
//               ans - array to store the current permutation resulting in minimum distance
//               floyd - 2D array storing intermediate nodes and distances between locations
//               min_distance - reference to the minimum distance variable
// Return Type:  void

void generate_permutations(int arr[], int start, int end, int ans[], struct my_pair floyd[num_locations][num_locations], int min_distance) 
{
    // Base case: If all elements are permuted, calculate the current cost and update minimum distance and path
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
    else {
        // Recursively generate permutations by swapping elements
        for (int i = start; i <= end; i++) 
        {
            swap(&arr[start], &arr[i]);
            generate_permutations(arr, start + 1, end, ans, floyd, min_distance);
            swap(&arr[start], &arr[i]);
        }
    }
}
// Function:     allocate_taxi
// Description:  allocates a taxi for a passenger based on certain criteria
// Input param:  struct fleet *fleet - pointer to the fleet of taxis
//               struct passenger *p - pointer to the passenger
//               struct my_pair floyd[num_locations][num_locations] - 2D array representing the shortest paths between locations
// Return Type:  void
void allocate_taxi(struct fleet *fleet, struct passenger *p, struct my_pair floyd[num_locations][num_locations])
{
    // Variables to keep track of the minimum time and corresponding taxi
    struct taxi *min;
    int min_time = INT_MAX;
    struct taxi *taxi_cur;
    int p_location = p->cur_location;
    int required_rating = p->required_rating;
    int cur_time;

    // Iterate through each taxi in the fleet
    for(int i = 0; i < fleet->num_taxis; i++) 
    {
        taxi_cur = fleet->fleet[i];

        // Check if the taxi meets the required rating
        if(taxi_cur->rating < required_rating) 
        {
            printf("\nNo taxi with required rating found");
            return;
        }

        // Calculate the current time based on the taxi's path and Floyd's algorithm
        if(taxi_cur->path_location->destination != NULL) {
            cur_time = taxi_cur->path_location->remaining_time + floyd[taxi_cur->path_location->destination->data][p_location].first;
        }
        else {
            cur_time = taxi_cur->path_location->remaining_time + floyd[taxi_cur->path_location->node][p_location].first;
        }

        printf("cur_time %d", cur_time);

        // Update the minimum time and corresponding taxi if a better option is found
        if(cur_time < min_time) 
        {
            min_time = cur_time;
            min = taxi_cur;
        }
    }

    // Track the path for the selected taxi based on passenger's locations
    if(min->path_location->head == NULL) 
    {     
        track_path(floyd, min->path_location->node, p_location, &min->path_location->head, min);
    }
    else
    {
        track_path(floyd, min->path_location->destination->data, p_location, &min->path_location->destination, min);
    }

    // Handle special cases based on passenger's order and number of given locations
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
        // Generate permutations and track paths for the remaining locations
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
    // Print the selected path
    struct node * cur_ptr = min->path_location->head;
    printf("\n");
    while(cur_ptr) {
        printf("%d ->", cur_ptr->data);
        cur_ptr = cur_ptr->next;
    }
}   

// Function:     partition
// Description:  partitions the fleet of taxis based on their ratings for quicksort
// Input param:  struct fleet *fleet - pointer to the fleet of taxis
//               int low - starting index for partition
//               int high - ending index for partition
// Return Type:  int - index of the pivot after partitioning
int partition(struct fleet *fleet, int low, int high)
{
    double pivot = fleet->fleet[high]->rating;
    int i = low - 1;
    struct taxi * temp;

    // Iterate through the elements and rearrange based on the pivot
    for(int j = low; j <= high; j++)
    {
        if(fleet->fleet[j]->rating < pivot)
        {
            i++;
            temp = fleet->fleet[i];
            fleet->fleet[i] = fleet->fleet[j];
            temp = fleet->fleet[j];
        }
    }

    // Swap the pivot to its correct position
    temp = fleet->fleet[i + 1];
    fleet->fleet[i + 1] = fleet->fleet[high];
    temp = fleet->fleet[high];
    return i + 1;
}

// Function:     quickSort
// Description:  performs quicksort on the fleet of taxis based on their ratings
// Input param:  struct fleet *fleet - pointer to the fleet of taxis
//               int low - starting index for quicksort
//               int high - ending index for quicksort
// Return Type:  void
void quickSort(struct fleet *fleet, int low, int high)
{
    if(low < high)
    {
        int pi = partition(fleet, low, high);
        quickSort(fleet, low, pi-1);
        quickSort(fleet, pi+1, high);
    }
}
// Function:     take_taxi_input
// Description:  Reads taxi information from the input file and initializes the taxi structure
// Input param:  fleet - pointer to the fleet structure
//               inputFile - pointer to the input file
// Return Type:  void
void take_taxi_input(struct fleet * fleet, FILE * inputFile) 
{
    // Allocate memory for taxi location structure
    struct location* taxi_location = (struct location*)malloc(sizeof(struct location));
    taxi_location->part = 0;
    taxi_location->head = NULL;
    taxi_location->prev_node = 0;
    taxi_location->remaining_time = 0;
    taxi_location->destination = NULL;

    // Allocate memory for taxi structure
    struct taxi * a = (struct taxi*)malloc(sizeof(struct taxi));
    a->path_location = taxi_location;

    // Read taxi information from the file
    fscanf(inputFile, "%d %d %lf %d", &a->taxi_id, &a->path_location->node, &a->rating, &a->journey_completed);
    
    // Initialize other taxi attributes
    a->fare = 0;
    a->is_engaged = false;
    a->journey_completed = 0;
    a->p = NULL;

    // Increment the number of taxis in the fleet and add the taxi to the fleet array
    fleet->num_taxis++;
    fleet->fleet[fleet->num_taxis - 1] = a;

    // Sort the fleet based on taxi IDs
    quickSort(fleet, 0, fleet->num_taxis - 1);
}

// Function:     take_passenger_input
// Description:  Reads passenger information from the input file and allocates a taxi for the passenger
// Input param:  fleet - pointer to the fleet structure
//               floyd - 2D array storing shortest paths between locations
//               inputFile - pointer to the input file
// Return Type:  void
void take_passenger_input(struct fleet * fleet, struct my_pair floyd[num_locations][num_locations], FILE * inputFile)
{
    // Allocate memory for passenger structure
    char order;
    struct passenger* p = (struct passenger *)malloc(sizeof(struct passenger));
    p->name = (char *)malloc(40);

    // Read passenger information from the file
    fscanf(inputFile, "%s %d %c %lf %d", p->name, &p->cur_location, &order, &p->required_rating, &p->num_given_locations);

    // Set the required order attribute based on the input
    p->required_order = (order == 'y') ? false : true;

    // Read given locations for the passenger
    for(int i = 0; i < p->num_given_locations; i++) 
    {
        fscanf(inputFile, "%d", &p->locations[i]);
    }

    // Allocate a taxi for the passenger
    allocate_taxi(fleet, p, floyd);
}

// Function:     move_taxi
// Description:  Moves taxis along their paths, updating their positions and handling completion
// Input param:  fleet - pointer to the fleet structure
//               floyd - 2D array storing shortest paths between locations
// Return Type:  void
void move_taxi(struct fleet * fleet, struct my_pair floyd[num_locations][num_locations]) 
{
    // Iterate through all taxis in the fleet
    struct taxi * cur;
    struct location * cur_path_location;
    struct node* a;
    for(int i = 0; i < fleet->num_taxis; i++) 
    {
        cur = fleet->fleet[i];
        cur_path_location = cur->path_location;

        // Check if the taxi has a path to follow
        if(cur_path_location->head != NULL)
        {
            // Update taxi's position and handle completed paths
            cur_path_location->part++;
            cur_path_location->remaining_time--;
            if(floyd[cur_path_location->node][cur_path_location->head->next->data].first == cur_path_location->part) 
            {
                cur_path_location->part = 0;
                a = cur_path_location->head;
                cur_path_location->head = cur_path_location->head->next;
                cur_path_location->prev_node = cur_path_location->node;
                cur_path_location->node = cur_path_location->head->data;
                free(a);
            }
            // Check for completion of the entire path
            if(cur_path_location->head->next == NULL && cur_path_location->part == 0) 
            {
                double rating;
                free(cur_path_location->head);
                cur_path_location->head = NULL;
                cur_path_location->destination = NULL;
                printf("\nTaxi %d reached destination", cur->taxi_id);
                printf("\nFare for passenger %s is %d\n", cur->p->name, cur->fare * 10);
                printf("\nEnter rating of taxi ID %d : ", cur->taxi_id);
                scanf("%lf", &rating);
                cur->rating = (cur->journey_completed * cur->rating + rating) / (cur->journey_completed + 1);
                cur->journey_completed++;
                //quickSort(fleet, 0, fleet->num_taxis - 1);
            }
            else 
            {
                // Print taxi's current position or progress along the path
                if(cur_path_location->part == 0) 
                {
                    printf("\nTaxi %d at %d", cur->taxi_id, cur_path_location->node);
                }
                else 
                {
                    printf("\nTaxi %d at %d / %d way from %d", cur->taxi_id, cur_path_location->part, floyd[cur_path_location->node][cur_path_location->head->next->data].first, cur_path_location->head->next->data);
                }
            }
        }
    }
}

// Function:     convert_int_to_string
// Description:  Converts an integer to a string
// Input param:  string - pointer to the character array to store the result
//               n - integer to be converted
// Return Type:  void
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
// Function:    process
// Description: Simulates the taxi and passenger communication process
// Input param: Pointer to a fleet structure, 2D array representing pairwise distances between locations
// Return Type: None

void process(struct fleet *fleet, struct my_pair floyd[num_locations][num_locations]) 
{
    // Initialize the number of passengers and taxis
    int num_passengers = 1;
    int num_taxis = 1;
    
    // File names for passenger and taxi input files
    char passenger_file_name[40] = "communication/passenger_input";
    char taxi_file_name[40] = "communication/taxi_input";
    
    // Temporary string for number conversion
    char number[10];
    
    // Convert and concatenate the number of passengers to the passenger file name
    convert_int_to_string(number, num_passengers);
    strcat(passenger_file_name, number);
    
    // Convert and concatenate the number of taxis to the taxi file name
    convert_int_to_string(number, num_taxis);
    strcat(taxi_file_name, number);   
    
    // Append ".txt" extension to the file names
    strcat(passenger_file_name, ".txt");
    strcat(taxi_file_name, ".txt");
    
    FILE *inputFile;
    int length;
    // Main loop for simulation
    while(true) 
    {
        // Move taxis based on the fleet and pairwise distances
        move_taxi(fleet, floyd);
        printf("\n\n\n");
        
        // Process taxi input files
        while(true)
        {
            // Open taxi input file for reading
            inputFile = fopen(taxi_file_name, "r");
            
            // Check if the file is successfully opened
            if(inputFile != NULL) 
            {
                // Process taxi input and close the file
                take_taxi_input(fleet, inputFile);
                fclose(inputFile);
                
                // Remove the processed taxi input file
                remove(taxi_file_name);
                
                // Update the taxi file name for the next iteration
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
                // Break the loop if the file is not available
                break;
            }
        }
        
        // Process passenger input files
        while(true)
        {
            // Open passenger input file for reading
            inputFile = fopen(passenger_file_name, "r");
            
            // Check if the file is successfully opened
            if(inputFile != NULL) 
            {
                // Process passenger input and close the file
                take_passenger_input(fleet, floyd, inputFile);
                fclose(inputFile);
                
                // Remove the processed passenger input file
                remove(passenger_file_name);
                
                // Update the passenger file name for the next iteration
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
                // Break the loop if the file is not available
                break;
            }
        }
        
        // Pause for 1 second before the next iteration
        sleep(1); 
    }
}
int main() 
{
    int adj_mat[num_locations][num_locations];
    take_map_input(adj_mat);
    struct my_pair floyd[num_locations][num_locations];
    calculate_floyd(adj_mat, floyd);
    struct fleet fleet;
    fleet.num_taxis = 0;
    process(&fleet, floyd);
}
