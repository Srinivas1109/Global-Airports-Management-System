#include "header.h"
#include <limits.h>
#include <stdio.h> // To be removed later
#include <stdlib.h>

// ANY STATIC FUNCTIONS ARE UP HERE

// YOUR SOLUTIONS BELOW

static int length(const char *str)
{
    int len = 0;
    while (*str)
    {
        len++;
        str++;
    }

    return len;
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
        q->arr[q->r] = val;
        q->r++;
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
        a = q->arr[q->f];
        q->f++;
    }
    return a;
}

static int bfs(int n, const connection_t a[n][n])
{
    struct queue q;
    // int size = 400;
    q.size = 400;

    q.arr = (int*)malloc(q.size * sizeof(int));
    // int *arr = (int*)malloc(size * sizeof(int));
    // int arr[size];
    // int f = 0, r = 0;

    int visited[q.size];

    // visited[] = 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) // mark all the vertices as not visited
        {
            visited[j] = 0;
        }
        q.f = 0;
        q.r = 0;
        // f = 0;
        // r = 0;
        // printf("%d\t", i);
        visited[i] = 1; // mark the starting vertex as visited
        enqueue(&q, i);
        // r++;
        // arr[r] = i;

        // qinsert(&r, &f, i, q.arr);

        while (!isEmpty(&q))
        {
            int node = dequeue(&q);
            // int node = arr[f];
            // printf("In loop\n");
            // int node = qdelete(&r, &f, arr);
            // f++;
            // printf("Deleted: %d\n", node);
            for (int j = 0; j < n; j++)
            {
                if ((a[node][j].distance != INT_MAX && a[node][j].distance != 0) && visited[j] == 0)
                {
                    // printf("%d\t", j);
                    visited[j] = 1;
                    enqueue(&q, j);
                    // r++;
                    // arr[r] = i;
                    // qinsert(&r, &f, j, arr);
                }
            }
        }
    }
    // q.r == n condition
    free(q.arr);
    // printf("\n");
    if (q.r == n)
        return 1;
    return 0;
}

int q1(int n, const connection_t connections[n][n])
{
    return bfs(n, connections);
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

static int findpath(int s, int d, int n, int *visit, const connection_t connections[n][n])
{
    int u;
    visit[s] = 1;

    for (u = 0; u < n; u++)
    {
        if ((connections[s][u].distance != INT_MAX) && (visit[u] == 0))
        {
            if ((u == d) || findpath(u, d, n, visit, connections))
                return 1;
        }
    }
    return 0;
}

static int path(int src, int dest, int n, const connection_t connections[n][n])
{
    int visited[n];
    int i;

    for (i = 0; i < n; i++)
        visited[i] = 0;
    return findpath(src, dest, n, visited, connections);
}

int q3(const airport_t *src, int n, const connection_t connections[n][n])
{
    int flag = 1;
    for (int i = 0; i < n; i++)
    {
        if (i != src->num_id)
        {
            if (path(src->num_id, i, n, connections))
                if (!path(i, src->num_id, n, connections))
                    flag = 0;
        }
    }
    return flag;
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

    // perform quicksort on data
    quickSort(n, airport_list, 0, n - 1, predicate_func);
}

// For Question 5
void commonPrefixUtil(int n, const airport_t airports[n], int id1, int id2, int *prev_len, pair_t *res, pair_t index)
{
    char *result = (char *)malloc(sizeof(char) * 50);
    int k = 0;
    int n1 = length(airports[index.first].airport_name), n2 = length(airports[index.second].airport_name);
    // printf("n1 %d, n2 %d\n", n1, n2);

    // Compare str1 and str2
    for (int i = 0, j = 0; i <= n1 - 1 && j <= n2 - 1; i++, j++)
    {
        // printf("str1: %c\t str2: %c\n", str1[i], str2[j]);
        if (airports[id1].airport_name[i] != airports[id2].airport_name[j])
            break;
        // printf("%s\n", result);
        result[k++] = airports[id1].airport_name[i];
    }
    result[k] = '\0';
    if (length(result) > *prev_len)
    {
        *prev_len = length(result);
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
    pair_t index;
    int prev = 0;
    result.first = -1;
    result.second = -1;
    char *str1 = (char *)malloc(sizeof(char) * 50);
    char *str2 = (char *)malloc(sizeof(char) * 50);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            // strcpy(str1, airports[i].airport_name);
            // strcpy(str2, airports[j].airport_name);
            index.first = i;
            index.second = j;
            commonPrefixUtil(n, airports, airports[i].num_id, airports[j].num_id, &prev, &result, index);
        }
    // printf("longest prefix is at postions {%d, %d}\n", result.first, result.second);
    return result;
}

// For question 6
static int binarySearch(int arr[], int l, int r, int x, int n)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        // itself

        // printf("mid-1\tx\tmid\n");
        // printf("%d\t%d\t%d\n", arr[mid-1], x, arr[mid]);
        if (arr[mid] == x)
            return mid + 1;
        else if ((arr[mid - 1] < x) && (x < arr[mid]))
            return mid;
        else if (mid == n - 1)
            return mid + 1;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x, n);

        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid + 1, r, x, n);
    }

    // We reach here when element is not
    // present in array
    return -1;
}

int q6(int n, int amount, const int entry_fee[n])
{
    int fees[n];
    for (int i = 0; i < n; i++)
    {
        fees[i] = entry_fee[i];
    }

    int result = binarySearch(fees, 0, n - 1, amount, n);
    return result;
}

static void shifttable(const char *p, int t[])
{
    int i, j, m;
    m = length(p);
    for (i = 0; i < 200; i++)
        t[i] = m;
    for (j = 0; j < m - 1; j++)
        t[p[j]] = m - 1 - j;
    // for(int i = 0; i< 6; i++ )
    // printf("%d\t", t[i]);
}

static int horspool(const char src[], const char *p, int t[], int len)
{
    int i, j, k, m, n;
    n = length(src);
    m = length(p);
    // printf("\nLength of text=%d",n);
    // printf("\n Length of pattern=%d",m);
    i = m - 1;
    while (i < n)
    {
        k = 0;
        while ((k < m) && (p[m - 1 - k] == src[i - k]))
            k++;
        if (k == m)
            return (i - m + 1);
        else
            i += t[src[i]];
    }
    return -1;
}

void q7(int n, const char *pat, int contains[n], const airport_t airports[n])
{
    int *t = (int *)malloc(200 * sizeof(int));

    for (int i = 0; i < 200; i++)
    {
        t[i] = 0;
    }
    // printf("Pattern: %s\n", pat);
    for (int i = 0; i < n; i++)
    {
        shifttable(pat, t);
        if (horspool(airports[i].airport_name, pat, t, n) != -1)
        {
            contains[i] = 1;
        }
        // printf("%d\t", t[i]);
    }

    // printf("\n");
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
