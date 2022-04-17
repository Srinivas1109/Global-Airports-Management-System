#include "header.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANY STATIC FUNCTIONS ARE UP HERE

// YOUR SOLUTIONS BELOW

int q[20], visited[20], f = -1, r = -1;

static void bfs(int v, int n, const connection_t a[n][n])
{
    int i;
    for (i = 0; i < n; i++) // check all the vertices in the graph
    {
        if ((a[v][i].distance != INT_MAX && a[v][i].time != INT_MAX) && visited[i] == 0) // adjacent to v and not visited
        {
            r = r + 1;
            q[r] = i;       // insert them into queue
            visited[i] = 1; // mark the vertex visited
            // printf("%d\t", i);
        }
    }
    f = f + 1;           // remove the vertex at front of the queue
    if (f <= r)          // as long as there are elements in the queue
        bfs(q[f], n, a); // peform bfs again on the vertex at front of the queue
}

int q1(int n, const connection_t connections[n][n])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) // mark all the vertices as not visited
        {
            visited[j] = 0;
        }
        f = r = 0;
        q[r] = i;
        // printf("BFS traversal for starting vertex %d is:\n", i);
        visited[i] = 1; // mark the starting vertex as visited
        // printf("%d\t", i);
        bfs(i, n, connections);
        // printf("\n");
        if (r != n - 1)
            return 0;
    }
    return 1;
}

struct queue
{
    int size;
    int f;
    int r;
    int *arr;
};

static int isEmpty(struct queue *q)
{
    if (q->r == q->f)
    {
        return 1;
    }
    return 0;
}

static int isFull(struct queue *q)
{
    if (q->r == q->size - 1)
    {
        return 1;
    }
    return 0;
}

static int enqueue(struct queue *q, int val)
{
    if (isFull(q))
    {
        return 1;
    }
    else
    {
        q->r++;
        q->arr[q->r] = val;
        // printf("Enqued element: %d\n", val);
    }
    return 0;
}

static int dequeue(struct queue *q)
{
    int a = -1;
    if (isEmpty(q))
    {
        a = -1;
    }
    else
    {
        q->f++;
        a = q->arr[q->f];
    }
    return a;
}

static int bfs_Q2(int v, int src, int dest, int n, const connection_t a[n][n])
{
    struct queue q;
    q.size = 400;
    q.f = q.r = 0;
    q.arr = (int *)malloc(q.size * sizeof(int));

    int distance[n];
    int visited[n];
    int pred[n];
    for (int i = 0; i < n; i++)
    {
        distance[i] = -1;
        pred[i] = -1;
        visited[i] = 0;
    }

    visited[src] = 1;
    distance[src] = 0;
    enqueue(&q, src);
    while (!isEmpty(&q))
    {
        int node = dequeue(&q);
        for (int j = 0; j < n; j++)
        {
            if ((a[node][j].distance != INT_MAX && a[node][j].distance != 0) && visited[j] == 0)
            {
                visited[j] = 1;
                distance[j] = distance[node] + 1;
                pred[j] = node;
                enqueue(&q, j);

                if (j == dest)
                    break;
            }
        }
    }

    int crawl = dest;
    struct queue path;
    path.size = 400;
    path.f = path.r = 0;
    path.arr = (int *)malloc(path.size * sizeof(int));
    enqueue(&path, crawl);
    while (pred[crawl] != -1)
    {
        enqueue(&path, pred[crawl]);
        crawl = pred[crawl];
    }

    return distance[dest];
}

int q2(const airport_t *src, const airport_t *dest, int n, int k,
       const connection_t connections[n][n])
{
    int hops = bfs_Q2(src->num_id, src->num_id, dest->num_id, n, connections);
    if (hops <= k)
        return 1;
    return 0;
}

int q3(const airport_t *src, int n, const connection_t connections[n][n])
{
    int home = 0;
    for (int i = 0; i < n; i++)
    {
        if (src->num_id != i)
            if (connections[src->num_id][i].distance != INT_MAX)
                if (connections[i][src->num_id].distance != INT_MAX)
                    home = home + 1;
    }
    if (home != 0)
        return 1;
    else
        return 0;
}

// For Question 4
// function to swap elements
static void swap(airport_t *a, airport_t *b)
{
    airport_t t = *a;
    *a = *b;
    *b = t;
}

// function to find the partition position
static int partition(int n, airport_t airport_list[n], int low, int high, int (*predicate_func)(const airport_t *, const airport_t *))
{

    // select the rightmost element as pivot
    airport_t pivot = airport_list[high];

    // pointer for greater element
    int i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++)
    {
        if (predicate_func(&airport_list[j], &pivot))
        {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&airport_list[i], &airport_list[j]);
        }
    }

    // swap the pivot element with the greater element at i
    swap(&airport_list[i + 1], &airport_list[high]);

    // return the partition point
    return (i + 1);
}

static void quickSort(int n, airport_t airport_list[n], int low, int high, int (*predicate_func)(const airport_t *, const airport_t *))
{
    if (low < high)
    {

        // find the pivot element such that
        // elements smaller than pivot are on left of pivot
        // elements greater than pivot are on right of pivot
        int pi = partition(n, airport_list, low, high, predicate_func);

        // recursive call on the left of pivot
        quickSort(n, airport_list, low, pi - 1, predicate_func);

        // recursive call on the right of pivot
        quickSort(n, airport_list, pi + 1, high, predicate_func);
    }
}

// function to print array elements
static void printArray(int n, airport_t airport_list[n])
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d\t%s\n", airport_list[i].num_id, airport_list[i].airport_name);
    }
    printf("\n");
}


void q4(int n, int (*predicate_func)(const airport_t *, const airport_t *),
        airport_t airport_list[n])
{

    // printf("Unsorted Array\n");
    // printArray(n, airport_list);

    // perform quicksort on data
    quickSort( n, airport_list, 0, n-1, predicate_func);

    // printf("Sorted array in ascending order: \n");
    // printArray(n, airport_list);
    
}

// For Question 5
void commonPrefixUtil(char *str1, char *str2, int id1, int id2, int *prev_len, pair_t *res)
{
	char *result = (char*)malloc(sizeof(char)*50);
    int  k = 0;
	int n1 = strlen(str1), n2 = strlen(str2);
	//printf("n1 %d, n2 %d\n", n1, n2);

	// Compare str1 and str2
	for (int i=0, j=0; i<=n1-1&&j<=n2-1; i++,j++)
	{
		//printf("str1: %c\t str2: %c\n", str1[i], str2[j]);
		if (str1[i] != str2[j])
			break;
		// printf("%s\n", result);
		result[k++] = str1[i];
	}
    result[k] = '\0';
	if(strlen(result) > *prev_len){
		*prev_len = strlen(result);
		res->first = id1;
		res->second = id2;

	}
	// printf("%s %d and %s %d is %s %d\n", str1, id1, str2, id2, result, strlen(result));
}

// A Function that returns the longest common prefix
// from the array of strings

// {0, "KOC"}, {1, "KIA"}, {2, "KOL"}

pair_t q5(int n, airport_t airports[n])
{
    pair_t result;
    int prev = 0;
	result.first = -1;
	result.second = -1;
    char *str1 = (char*)malloc(sizeof(char)*50);
    char *str2 = (char*)malloc(sizeof(char)*50);
    for(int i = 0; i< n; i++)
		for (int j = i+1; j < n; j++)
		{
			strcpy(str1, airports[i].airport_name);
			strcpy(str2, airports[j].airport_name);
            commonPrefixUtil(str1, str2, airports[i].num_id, airports[j].num_id, &prev, &result);
		}
	printf("longest prefix is at postions {%d, %d}\n", result.first, result.second);
    return result;
}

int q6(int n, int amount, const int entry_fee[n])
{
    return 0;
}

void q7(int n, const char *pat, int contains[n], const airport_t airports[n])
{
}

int q8(int n, int trip_order[n - 1], const connection_t connections[n][n])
{
    return 0;
}

int q9(int n, pair_t edges[n - 1], const connection_t connections[n][n])
{
    return 0;
}

void q10(int n, int k, const airport_t *src,
         const connection_t connections[n][n], const int destinations[k],
         int costs[k])
{
}

// END
