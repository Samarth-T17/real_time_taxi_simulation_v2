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
struct node
{
    int key;
    struct node* next;
};
struct hashtable
{
    int numOfElements;
    int capacity;
    struct node** arr;
};
void setNode_table(struct node* node, int key)
{
    int i = 0;
    node->key = key;
    node->next = NULL;
}
void initializeHashtable(struct hashtable* mp)
{
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (struct node**)malloc(sizeof(struct node*) * mp->capacity);
    for(int i = 0; i < mp->capacity; i++) {
        mp->arr[i] = NULL;
    }
}
int hashFunction(struct hashtable* mp, int key)
{
    return key % mp->capacity;
}
void insert_table(struct hashtable* mp, int key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    setNode_table(newNode, key);
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
bool search_table(struct hashtable* mp, int key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {
        if (bucketHead->key == key) {
            return true;
        }
        bucketHead = bucketHead->next;
    }
    return false;
}
int main(){
    struct hashtable t;
    initializeHashtable(&t);
    int landmarks[] = {1, 2, 3, 4};
    for(int i = 0; i < 4; i++)
    {
        insert_table(&t, landmarks[i]);
    }
    for(int i = 0; i < 4; i++) {
        if(search_table(&t, 7)) {
            printf("\n true");
        }
    }
}