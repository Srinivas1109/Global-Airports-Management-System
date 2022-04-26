// Name: Y Srinivas
// SRN: PES1UG20CS517
// Section: I

#include "header.h"
#include <limits.h>
#include <stdlib.h>

// ANY STATIC FUNCTIONS ARE UP HERE

// YOUR SOLUTIONS BELOW

// userdefined function to calculate the length of the string
static int strlength(const char *string)
{
    int length = 0;
    while (*string)
    {
        length++;
        string++;
    }

    return length;
}

// userdefined data structure for BFS and other algorithms
typedef struct queue
{
    int size;
    int front;
    int rear;
    int *arr;
} queue_t;

// userdefined function to check whether queue data structure is empty or not
static int isEmpty(queue_t *q)
{
    if (q->rear == q->front)
    {
        return 1;
    }
    return 0;
}

// check whether the queue is full
static int isFull(queue_t *q)
{
    if (q->rear == q->size - 1)
    {
        return 1;
    }
    return 0;
}

// To insert elements into a data structures
static int enqueue(queue_t *q, int val)
{
    if (isFull(q))
    {
        return 1;
    }
    else
    {
        q->arr[q->rear] = val;
        q->rear++;
    }
    return 0;
}

static int dequeue(queue_t *q)
{
    int a = -1;
    if (isEmpty(q))
    {
        a = -1;
    }
    else
    {
        a = q->arr[q->front];
        q->front++;
    }
    return a;
}

static int visitAll_BFS(int n, const connection_t a[n][n])
{
    queue_t q;
    q.size = 1000;

    q.arr = (int *)malloc(q.size * sizeof(int));

    int visited[q.size];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) // mark all the vertices as not visited
        {
            visited[j] = 0;
        }
        q.front = 0;
        q.rear = 0;

        visited[i] = 1; // mark the starting vertex as visited
        enqueue(&q, i);

        while (!isEmpty(&q))
        {
            int node = dequeue(&q);

            for (int j = 0; j < n; j++)
            {
                if ((a[node][j].distance != INT_MAX && a[node][j].distance != 0) && visited[j] == 0)
                {
                    visited[j] = 1;
                    enqueue(&q, j);
                }
            }
        }
    }
    free(q.arr);
    if (q.rear == n)
        return 1;
    return 0;
}

int q1(int n, const connection_t connections[n][n])
{
    return visitAll_BFS(n, connections);
}

static int NumberOfHops(int source, int destination, int n, const connection_t a[n][n])
{
    queue_t q;
    q.size = 1000;
    q.front = q.rear = 0;
    q.arr = (int *)malloc(q.size * sizeof(int));

    int distance[n];
    int visited[n];
    int predicate[n];
    for (int i = 0; i < n; i++)
    {
        distance[i] = -1;
        predicate[i] = -1;
        visited[i] = 0;
    }

    visited[source] = 1;
    distance[source] = 0;
    enqueue(&q, source);
    while (!isEmpty(&q))
    {
        int node = dequeue(&q);
        for (int j = 0; j < n; j++)
        {
            if ((a[node][j].distance != INT_MAX && a[node][j].distance != 0) && visited[j] == 0)
            {
                visited[j] = 1;
                distance[j] = distance[node] + 1;
                predicate[j] = node;
                enqueue(&q, j);

                if (j == destination)
                    j = n;
            }
        }
    }

    int crawl = destination;
    queue_t path;
    path.size = 400;
    path.front = path.rear = 0;
    path.arr = (int *)malloc(path.size * sizeof(int));
    enqueue(&path, crawl);
    while (predicate[crawl] != -1)
    {
        enqueue(&path, predicate[crawl]);
        crawl = predicate[crawl];
    }
    free(q.arr);
    free(path.arr);
    return distance[destination];
}

int q2(const airport_t *src, const airport_t *destination, int n, int k,
       const connection_t connections[n][n])
{
    int hops = NumberOfHops(src->num_id, destination->num_id, n, connections);
    if (hops <= k)
        return 1;
    return 0;
}

static int cycleFinder(int source, int destination, int n, int *visit, const connection_t connections[n][n])
{
    int u;
    visit[source] = 1;

    for (u = 0; u < n; u++)
    {
        if ((connections[source][u].distance != INT_MAX) && (visit[u] == 0))
        {
            if ((u == destination) || cycleFinder(u, destination, n, visit, connections))
                return 1;
        }
    }
    return 0;
}

static int pathFinder(int source, int destination, int n, const connection_t connections[n][n])
{
    int visited[n];
    int i;

    for (i = 0; i < n; i++)
        visited[i] = 0;
    return cycleFinder(source, destination, n, visited, connections);
}

int q3(const airport_t *src, int n, const connection_t connections[n][n])
{
    int cycleExists = 1;
    for (int i = 0; i < n; i++)
    {
        if (i != src->num_id)
        {
            if (pathFinder(src->num_id, i, n, connections))
                if (!pathFinder(i, src->num_id, n, connections))
                    cycleExists = 0;
        }
    }
    return cycleExists;
}

// For Question 4
// function to swap elements
static void swapAirports(airport_t *airport1, airport_t *airport2)
{
    airport_t temp = *airport1;
    *airport1 = *airport2;
    *airport2 = temp;
}

// function to find the partition position
static int arrayPartition(int n, airport_t airport_list[n], int low, int high, int (*predicate_func)(const airport_t *, const airport_t *))
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
            swapAirports(&airport_list[i], &airport_list[j]);
        }
    }
    // swap the pivot element with the greater element at i
    swapAirports(&airport_list[i + 1], &airport_list[high]);

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
        int pi = arrayPartition(n, airport_list, low, high, predicate_func);

        // recursive call on the left of pivot
        quickSort(n, airport_list, low, pi - 1, predicate_func);

        // recursive call on the right of pivot
        quickSort(n, airport_list, pi + 1, high, predicate_func);
    }
}

void q4(int n, int (*predicate_func)(const airport_t *, const airport_t *),
        airport_t airport_list[n])
{
    // perform quicksort on data
    quickSort(n, airport_list, 0, n - 1, predicate_func);
}

// For Question 5
static void longestCommonPrefix(int n, const airport_t airports[n], int id1, int id2, int *prev_len, pair_t *res, pair_t index)
{
    char *result = (char *)malloc(sizeof(char) * 50);
    int k = 0;
    int n1 = strlength(airports[index.first].airport_name), n2 = strlength(airports[index.second].airport_name);

    for (int i = 0, j = 0; i <= n1 - 1 && j <= n2 - 1; i++, j++)
    {
        if (airports[id1].airport_name[i] != airports[id2].airport_name[j])
            break;
        result[k++] = airports[id1].airport_name[i];
    }
    result[k] = '\0';
    if (strlength(result) > *prev_len)
    {
        *prev_len = strlength(result);
        res->first = id1;
        res->second = id2;
    }
}

pair_t q5(int n, airport_t airports[n])
{
    pair_t result;
    pair_t index;
    int prev = 0;
    result.first = -1;
    result.second = -1;

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            index.first = i;
            index.second = j;
            longestCommonPrefix(n, airports, airports[i].num_id, airports[j].num_id, &prev, &result, index);
        }
    return result;
}

// For question 6
// it uses the idea of binary search.
// It finds the position of the element to be inserted
static int possibleAirports(int arr[], int left, int right, int ele, int n)
{
    if (right >= left)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == ele)
            return mid + 1;

        else if ((arr[mid - 1] < ele) && (ele < arr[mid]))
            return mid;

        else if (mid == n - 1)
            return mid + 1;

        if (arr[mid] > ele)
            return possibleAirports(arr, left, mid - 1, ele, n);

        return possibleAirports(arr, mid + 1, right, ele, n);
    }

    return -1;
}

int q6(int n, int amount, const int entry_fee[n])
{
    int entry_fee_copy[n];
    for (int i = 0; i < n; i++)
    {
        entry_fee_copy[i] = entry_fee[i];
    }

    int result = possibleAirports(entry_fee_copy, 0, n - 1, amount, n);
    return result;
}

// 7th question
static void shiftTable(const char *pattern, int table[])
{
    int i, j, m;
    m = strlength(pattern);
    for (i = 0; i < 200; i++)
        table[i] = m;
    for (j = 0; j < m - 1; j++)
        table[pattern[j]] = m - 1 - j;
}

static int horspool(const char src[], const char *pattern, int table[])
{
    int i, j, k, m, n;
    n = strlength(src);
    m = strlength(pattern);

    i = m - 1;
    while (i < n)
    {
        k = 0;
        while ((k < m) && (pattern[m - 1 - k] == src[i - k]))
            k++;
        if (k == m)
            return (i - m + 1);
        else
            i += table[src[i]];
    }
    return -1;
}

void q7(int n, const char *pat, int contains[n], const airport_t airports[n])
{
    int *table = (int *)malloc(200 * sizeof(int));

    for (int i = 0; i < 200; i++)
    {
        table[i] = 0;
    }
    // printf("Pattern: %s\n", pat);
    for (int i = 0; i < n; i++)
    {
        shiftTable(pat, table);
        if (horspool(airports[i].airport_name, pat, table) != -1)
        {
            contains[i] = 1;
        }
        // printf("%d\t", t[i]);
    }

    free(table);

    // printf("\n");
}

// question 8
typedef struct stack
{
    int arr[400];
    int top;
} stack_t;

static void init(stack_t *s)
{
    s->top = -1;
}

static int isStackFull(stack_t *s)
{
    return s->top == 400 - 1;
}

static void pushEle(stack_t *s, int ele)
{
    if (!isStackFull(s))
    {
        s->top = s->top + 1;
        s->arr[s->top] = ele;
    }
}

static int popEle(stack_t *s)
{
    return s->arr[s->top--];
}

static int peek(stack_t *s)
{
    return s->arr[s->top];
}

static void insertElements(int cycles[25][25], int vertex, int *cycleNo)
{
    int i = *cycleNo;
    int index = -1;
    for (int j = 0; j < 25 && index == -1; j++)
    {
        if (cycles[i][j] == -1)
        {
            index = j;
        }
    }
    cycles[i][index] = vertex;
}

static void cycle(stack_t *s, int vertex, int cycles[25][25], int *cycleNo)
{
    stack_t s2;
    init(&s2);
    pushEle(&s2, peek(s));
    popEle(s);
    while (peek(&s2) != vertex)
    {
        pushEle(&s2, peek(s));
        popEle(s);
    }
    while (s2.top != -1)
    {
        insertElements(cycles, peek(&s2), cycleNo);
        pushEle(s, peek(&s2));
        popEle(&s2);
    }
}

static void DFS(int n, const connection_t connections[n][n], stack_t *s, int visited[n], int cycles[25][25], int *cycleNo)
{
    int u = peek(s);
    for (int i = 0; i < n; i++)
    {
        if (connections[u][i].distance != 0 && connections[u][i].distance != INT_MAX && visited[i] == 0)
        {
            *cycleNo = *cycleNo + 1;
            cycle(s, i, cycles, cycleNo);
        }
        else if (connections[u][i].distance != 0 && connections[u][i].distance != INT_MAX && visited[i] == -1)
        {
            pushEle(s, i);
            visited[i] = 0;
            DFS(n, connections, s, visited, cycles, cycleNo);
        }
    }
    visited[peek(s)] = 1;
    popEle(s);
}

static void findCycle(int n, const connection_t connections[n][n], int visited[n], int cycles[25][25], int *cycleNo)
{
    for (int i = 0; i < n; i++)
    {
        if (visited[i] == -1)
        {
            stack_t s;
            init(&s);
            pushEle(&s, i);
            visited[i] = 0;
            DFS(n, connections, &s, visited, cycles, cycleNo);
        }
    }
}

int q8(int n, int trip_order[n - 1], const connection_t connections[n][n])
{
    int visited[n];
    int cycles[25][25];
    int cycleNo = 0;
    int minIndex;
    int minimum = INT_MAX;
    int cost = 0;
    int p, q;
    for (int i = 0; i < n; i++)
    {
        visited[i] = -1;
    }
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            cycles[i][j] = -1;
        }
    }
    findCycle(n, connections, visited, cycles, &cycleNo);
    int count[cycleNo + 1];
    for (int i = 0; i < cycleNo + 1; i++)
    {
        count[i] = 0;
    }
    int result[cycleNo + 1][n];
    int x = 0;
    int y = 0;
    for (int i = 0; i < cycleNo + 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result[i][j] = -1;
        }
    }
    for (int i = 0; i < 25; i++)
    {
        y = 0;
        int flag = 0;
        for (int j = 0; j < 25; j++)
        {
            if (cycles[i][j] != -1)
            {
                if (!flag)
                {
                    x++;
                    flag = 1;
                }
                result[x][y] = cycles[i][j];
                count[x] = count[x] + 1;
                y++;
            }
        }
    }

    for (int i = 1; i < cycleNo + 1; i++)
    {
        if (count[i] == n - 1)
        {
            cost = 0;
            // last_index=n-1;
            for (int j = 0; j < n - 1; j++)
            {
                if (result[i][j] != -1 && result[i][j + 1] != -1)
                {
                    p = result[i][j];
                    q = result[i][j + 1];
                    cost += connections[p][q].distance;
                    // printf(" c : %d\t",cost);
                }
            }
            cost += connections[q][result[i][0]].distance;
            if (cost < minimum)
            {
                minimum = cost;
                minIndex = i;
            }
        }
    }
    if (minimum == INT_MAX)
    {
        return -1;
    }
    else
    {
        // printf("\n\n%d",minimum);
        for (int i = 0; i < n - 1; i++)
        {
            trip_order[i] = cycles[minIndex][i];
        }
        return minimum;
    }
}

// 9th question
typedef struct edge
{
    int u, v, w;
} edge_t;

typedef struct edge_list
{
    edge_t data[400];
    int n;
} edgeList_t;

static int find(int belongs[], int vertexNo)
{
    return (belongs[vertexNo]);
}

static void edgeUnion(int n, int belongs[], int c1, int c2)
{
    int i;

    for (i = 0; i < n; i++)
        if (belongs[i] == c2)
            belongs[i] = c1;
}

static void sort(edgeList_t *elist)
{
    int i, j;
    edge_t temp;

    for (i = 1; i < elist->n; i++)
        for (j = 0; j < elist->n - 1; j++)
            if (elist->data[j].w > elist->data[j + 1].w)
            {
                temp = elist->data[j];
                elist->data[j] = elist->data[j + 1];
                elist->data[j + 1] = temp;
            }
}

static int openEdges(pair_t edges[], edgeList_t *spanlist)
{
    int i, cost = 0;

    for (i = 0; i < spanlist->n; i++)
    {
        edges[i].first = spanlist->data[i].v;
        edges[i].second = spanlist->data[i].u;
        cost = cost + spanlist->data[i].w;
    }

    return cost;
}

// Applying Krushkal Algo
static void kruskal(int n, const connection_t connections[n][n], edgeList_t *elist, edgeList_t *spanlist)
{
    int belongs[400], i, j, cno1, cno2;

    elist->n = 0;

    for (i = 1; i < n; i++)
        for (j = 0; j < i; j++)
        {
            if (connections[i][j].time != 0)
            {
                elist->data[elist->n].u = i;
                elist->data[elist->n].v = j;
                elist->data[elist->n].w = connections[i][j].time;
                elist->n++;
            }
        }

    sort(elist);

    for (i = 0; i < n; i++)
        belongs[i] = i;

    spanlist->n = 0;

    for (i = 0; i < elist->n; i++)
    {
        cno1 = find(belongs, elist->data[i].u);
        cno2 = find(belongs, elist->data[i].v);

        if (cno1 != cno2)
        {
            spanlist->data[spanlist->n] = elist->data[i];
            spanlist->n = spanlist->n + 1;
            edgeUnion(n, belongs, cno1, cno2);
        }
    }
}

int q9(int n, pair_t edges[n - 1], const connection_t connections[n][n])
{
    edgeList_t elist;
    edgeList_t spanlist;
    kruskal(n, connections, &elist, &spanlist);
    int result = 0;
    result = openEdges(edges, &spanlist);

    return result;
}

// 10th question
static void Dijkstra(int n, const connection_t connections[n][n], int start, const int destination[], int costs[], int k)
{
    int cost[n][n], distance[n], predicate[n];
    int visited[n], count, mindistance, nextVertex, i, j;

    // Creating cost matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (connections[i][j].time == 0)
                cost[i][j] = INT_MAX;
            else
                cost[i][j] = connections[i][j].time;

    for (i = 0; i < n; i++)
    {
        distance[i] = cost[start][i];
        predicate[i] = start;
        visited[i] = 0;
    }

    distance[start] = 0;
    visited[start] = 1;
    count = 1;

    while (count < n - 1)
    {
        mindistance = INT_MAX;

        for (i = 0; i < n; i++)
            if (distance[i] < mindistance && !visited[i])
            {
                mindistance = distance[i];
                nextVertex = i;
            }

        visited[nextVertex] = 1;
        for (i = 0; i < n; i++)
            if (!visited[i])
                if (mindistance + cost[nextVertex][i] < distance[i])
                {
                    distance[i] = mindistance + cost[nextVertex][i];
                    predicate[i] = nextVertex;
                }
        count++;
    }

    for (int i = 0; i < k; i++)
    {
        costs[i] = distance[destination[i]];
    }
}

void q10(int n, int k, const airport_t *src,
         const connection_t connections[n][n], const int destinations[k],
         int costs[k])
{
    Dijkstra(n, connections, src->num_id, destinations, costs, k);
}

// END
