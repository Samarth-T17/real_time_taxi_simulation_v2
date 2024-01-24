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
struct node_queue
{
    struct my_pair data;
    struct node_queue * next;
};
struct queue
{
    struct node_queue* front;
    struct node_queue* rear;
};
int get_number(char str[100], int* i) {
    int ans = 0;
    for(; str[*i] < 58 && str[*i] > 47 ; *i= *i + 1){
        ans = ans * 10 + str[*i] - 48;
    }
    return ans;
}
void initializeQueue(struct queue* q)
{
    q->front = NULL;
    q->rear = NULL;
}
int isEmpty(struct queue* q)
{
    return q->front == NULL;
}
void enqueue(struct queue* q, struct my_pair newData)
{
    struct node_queue* newNode = (struct node_queue*)malloc(sizeof(struct node_queue));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = newData;
    newNode->next = NULL;
    if (isEmpty(q))
    {
        q->front = newNode;
        q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}
struct my_pair dequeue(struct queue* q)
{
    if (isEmpty(q))
    {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    struct my_pair dequeuedData = q->front->data;
    struct node_queue* temp = q->front;
    q->front = q->front->next;
    free(temp);
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    return dequeuedData;
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
    int n;
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        int j = 0;
        int len = strlen(line);
        graph[i]->n = 0;
        fscanf(inputFile2, "%d", &n);
        graph[i]->landmarks = (int *) malloc(sizeof(int) * n);
        for(int k = 0; k < n ; k++)
        {
            fscanf(inputFile2, "%d", &graph[i]->landmarks[k]);
        }
        printf("\n %d", i);
        for(int k = 0; k < n ; k++)
        {
            printf(" %d", graph[i]->landmarks[k]);
        }
        graph[i]->n_landmarks = n;
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
void bfs(int* landmarks, int num_landmarks, struct list * graph[num_locations], int source, struct my_pair* ans)
{
    bool visited[num_locations];
    for(int i = 0; i < num_locations; i++)
    {
        visited[i] = false;
    }
    visited[source] = true;
    struct queue q;
    initializeQueue(&q);
    struct my_pair node_distance;
    node_distance.first = source;
    node_distance.second = 0;
    enqueue(&q, node_distance);
    while(!isEmpty(&q))
    {
        node_distance = dequeue(&q);
        for(int i = 0; i < graph[node_distance.first]->n; i++)
        {
            if(!visited[graph[node_distance.first]->pairs[i].first] && graph[node_distance.first]->possible[i])
            {
                struct my_pair new_node_distance;
                new_node_distance.first = graph[node_distance.first]->pairs[i].first;
                new_node_distance.second = node_distance.second + graph[node_distance.first]->pairs[i].second;
                enqueue(&q, new_node_distance);
                visited[new_node_distance.first] = true;
                for(int j = 0; j < graph[node_distance.first]->n_landmarks; j++)
                {
                    for(int k = 0; k < num_landmarks; k++)
                    {
                        if(graph[node_distance.first]->landmarks[j] == landmarks[k])
                        {
                            if(node_distance.second + graph[node_distance.first]->pairs[i].second < ans[k].first)
                            {printf("hrek");
                                ans[k].first = node_distance.second + graph[node_distance.first]->pairs[i].second;
                                ans[k].second = node_distance.first;
                            }
                        }

                    }
                }
            }
        }
    }
}
int main() {
    int adj_mat[num_locations][num_locations];
    struct list *graph[num_locations];
    for (int i = 0; i < num_locations; i++) {
        graph[i] = (struct list *) malloc(sizeof(struct list));
    }
    take_map_input(adj_mat, graph);

    int landmarks[] = {2, 6};
    int n_landmarks = 2;
    struct my_pair ans[] = {{INT_MAX, INT_MAX},{INT_MAX, INT_MAX}};
    printf("jfre");
    bfs(landmarks, n_landmarks, graph, 14, ans);
    for(int i = 0; i < 2; i++) {
        printf("\n%d ", ans[i].second);
    }
}