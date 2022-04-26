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

    q.arr = (int *)malloc(q.size * sizeof(int));
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

// question 8
typedef struct stack
{
    int arr[400];
    int top;
}stack_t;

static void init(stack_t* s)
{
    s->top=-1;
}

static int full(stack_t* s)
{
    return s->top==400-1;
}

static void push(stack_t* s,int ele)
{
    if(!full(s))
    {
        s->top=s->top+1;
        s->arr[s->top]=ele;
    }
}

static int pop(stack_t* s)
{
    
    return s->arr[s->top--];
}

static int top(stack_t* s)
{
    
    return s->arr[s->top];
}

static void insert(int cycles[25][25],int v,int* cycle_no)
{
    int i=*cycle_no;
    int index=-1;
    for(int j=0;j<25 && index == -1;j++)
    {
        if(cycles[i][j]==-1)
        {
            index=j;
        }
    }
    cycles[i][index]=v;
}
static void cycle(stack_t* s,int v,int cycles[25][25],int* cycle_no)
{
    stack_t s2;
    init(&s2);
    push(&s2,top(s));
    pop(s);
    while(top(&s2)!=v)
    {
        push(&s2,top(s));
        pop(s);
    }
    while(s2.top!=-1)
    {
        insert(cycles,top(&s2),cycle_no);
        push(s,top(&s2));
        pop(&s2);
    }
}
static void DFS_tree(int n,const connection_t connections[n][n],stack_t* s,int visited[n],int cycles[25][25],int* cycle_no)
{
    int u=top(s);
    for(int i=0;i<n;i++)
    {
        if(connections[u][i].distance!=0 && connections[u][i].distance!=INT_MAX &&visited[i]==0)
        {
            *cycle_no=*cycle_no+1;
            cycle(s,i,cycles,cycle_no);
        }
        else if(connections[u][i].distance!=0 && connections[u][i].distance!=INT_MAX &&visited[i]==-1)
        {
            push(s,i);
            visited[i]=0;
            DFS_tree(n,connections,s,visited,cycles,cycle_no);
        }
    }
    visited[top(s)]=1;
    pop(s);
}
static void find_cycles(int n,const connection_t connections[n][n],int visited[n],int cycles[25][25],int* cycle_no)
{
    for(int i=0;i<n;i++)
    {
        if(visited[i]==-1)
        {
            stack_t s;
            init(&s);
            push(&s,i);
            visited[i]=0;
            DFS_tree(n,connections,&s,visited,cycles,cycle_no);
            // for(int j=i+1;j<n;j++)
            // {
            //     visited[j]=-1;
            // }
        }
    }
}



int q8(int n, int trip_order[n - 1], const connection_t connections[n][n])
{
    int visited[n];
    int cycles[25][25];
    int cycle_no = 0;
    int min_index;
    int min = INT_MAX;
    int cost=0;
    int p,q;
    for(int i=0;i<n;i++)
    {
        visited[i]=-1;
    }
    for(int a=0;a<25;a++)
    {
        for(int b=0;b<25;b++)
        {
            cycles[a][b]=-1;
        }
    }
    find_cycles(n,connections,visited,cycles,&cycle_no);
    int count[cycle_no+1];
    for(int i=0;i<cycle_no+1;i++)
    {
        count[i]=0;
    }
    int res[cycle_no+1][n];
    int x=0;
    int y=0;
    for(int a=0;a<cycle_no+1;a++)
    {
        for(int b=0;b<n;b++)
        {
            res[a][b]=-1;
        }
    }
    for(int a=0;a<25;a++)
    {
        y=0;
        int flag=0;
        for(int b=0;b<25;b++)
        {
            if(cycles[a][b]!=-1)
            {
                if(!flag)
                {
                    x++;
                    flag=1;
                }
                res[x][y]=cycles[a][b];
                count[x]=count[x]+1;
                // printf("%d %d %d %d %d\t\t\t",cycles[a][b],res[x][y],x,y,count[x]);
                y++;
            }
        }
        // x++;
        // printf("\n");
    }
    /* for(int a=1;a<cycle_no+1;a++)
    {
        for(int b=0;b<n;b++)
        {
            printf("%d\t",res[a][b]);
        }
        printf("\n");
    } */
    /* for(int i=0;i<cycle_no+1;i++)
    {
        // printf("%d\t",count[i]);
    } */
    for(int a=1;a<cycle_no+1;a++)
    {
        if(count[a]==n-1)
        {
            cost=0;
            // last_index=n-1;
            for(int b=0;b<n-1;b++)
            {
                if(res[a][b]!=-1 && res[a][b+1]!=-1)
                {
                    p=res[a][b];
                    q= res[a][b+1];
                    cost+= connections[p][q].distance;
                    // printf(" c : %d\t",cost);
                }
            }
            cost+=connections[q][res[a][0]].distance;
            if(cost<min)
            {
                min=cost;
                min_index=a;
            }
        }
    }
    if(min==INT_MAX)
    {
        return -1;
    }
    else
    {
        // printf("\n\n%d",min);
        for(int i=0;i<n-1;i++)
        {
            trip_order[i]=cycles[min_index][i];
        }
        return min;
    }


}

/* int q8(int n, int trip_order[n - 1], const connection_t connections[n][n])
{
    return 0;
} */

// 9th question
typedef struct edge {
  int u, v, w;
} edge;

typedef struct edge_list {
  edge data[400];
  int n;
} edge_list;

edge_list elist;

// int Graph[MAX][MAX], n;
edge_list spanlist;

static int find(int belongs[], int vertexno) {
  return (belongs[vertexno]);
}

static void applyUnion(int n, int belongs[], int c1, int c2) {
  int i;

  for (i = 0; i < n; i++)
    if (belongs[i] == c2)
      belongs[i] = c1;
}

static void sort() {
  int i, j;
  edge temp;

  for (i = 1; i < elist.n; i++)
    for (j = 0; j < elist.n - 1; j++)
      if (elist.data[j].w > elist.data[j + 1].w) {
        temp = elist.data[j];
        elist.data[j] = elist.data[j + 1];
        elist.data[j + 1] = temp;
      }
}

static int print(pair_t edges[]) {
  int i, cost = 0;

  for (i = 0; i < spanlist.n; i++) {
    // printf("\n%d - %d : %d", spanlist.data[i].u, spanlist.data[i].v, spanlist.data[i].w);
    edges[i].first = spanlist.data[i].v;
    edges[i].second = spanlist.data[i].u;
    cost = cost + spanlist.data[i].w;
  }

//   printf("\nSpanning tree cost: %d\n", cost);
  return cost;
}
/* void kruskalAlgo();
int find(int belongs[], int vertexno);
void applyUnion(int belongs[], int c1, int c2);
void sort();
void print(); */

// Applying Krushkal Algo
static void kruskalAlgo(int n, const connection_t connections[n][n]) {
  int belongs[400], i, j, cno1, cno2;
  elist.n = 0;

  for (i = 1; i < n; i++)
    for (j = 0; j < i; j++) {
      if (connections[i][j].time != 0) {
        elist.data[elist.n].u = i;
        elist.data[elist.n].v = j;
        elist.data[elist.n].w = connections[i][j].time;
        elist.n++;
      }
    }

  sort();

  for (i = 0; i < n; i++)
    belongs[i] = i;

  spanlist.n = 0;

  for (i = 0; i < elist.n; i++) {
    cno1 = find(belongs, elist.data[i].u);
    cno2 = find(belongs, elist.data[i].v);

    if (cno1 != cno2) {
      spanlist.data[spanlist.n] = elist.data[i];
      spanlist.n = spanlist.n + 1;
      applyUnion(n, belongs, cno1, cno2);
    }
  }
}


// Sorting algo

// Printing the result

int q9(int n, pair_t edges[n - 1], const connection_t connections[n][n])
{
    kruskalAlgo(n, connections);
    int res = 0;
    res = print(edges);
    /* for (int i = 0; i < n-1; i++)
    {
        printf("(%d, %d)\n", edges[i].first, edges[i].second);
    } */
    
    return res;
}

static void Dijkstra(int n, const connection_t connections[n][n], int start, const int dest[], int costs[], int k)
{
    int cost[n][n], distance[n], pred[n];
    int visited[n], count, mindistance, nextnode, i, j;

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
        pred[i] = start;
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
                nextnode = i;
            }

        visited[nextnode] = 1;
        for (i = 0; i < n; i++)
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i])
                {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }

    // Printing the distance

    for (int i = 0; i < k; i++)
    {
        costs[i] = distance[dest[i]];
    }

}

void q10(int n, int k, const airport_t *src,
         const connection_t connections[n][n], const int destinations[k],
         int costs[k])
{
    // Dijkstra(n, connections, 3, destinations, costs, k);
    Dijkstra(n, connections, src->num_id, destinations, costs, k);
    // printf("src : %d\n",);
}

// END
