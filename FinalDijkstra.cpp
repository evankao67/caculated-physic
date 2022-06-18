#include "pbPlots.hpp"
#include "supportLib.hpp"
// #include <bits/stdc++.h>
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#define maprange 100
#define node 100
using namespace std;
bool check[maprange][maprange];
int table[node][node];
int dist[node][node];
int annealing_dist[node][node];
bool visit[node];
vector<int> adj[node];
map<int, int> times;
int counter = 0;
int getrand(int range)
{
    return rand() % range;
}

void Dijkstra(int n, int v)
{
    bool s[node];
    for (int i = 0; i < n; i++)
    {
        dist[v][i] = table[v][i];
        s[i] = 0;
    }
    dist[v][v] = 0;
    s[v] = 1;
    for (int i = 1; i < n; i++)
    {
        int tmp = INT_MAX;
        int u = v;
        for (int j = 0; j < n; j++) // Find min
        {
            if (!s[j] && dist[v][j] < tmp)
            {
                u = j;
                tmp = dist[v][j];
            }
        }
        s[u] = 1;
        for (int j = 0; j < n; j++)
        {
            if ((!s[j]) && table[u][j] < INT_MAX)
            {
                int newdist = dist[v][u] + table[u][j]; // Decrease key
                if (newdist < dist[v][j])
                {
                    dist[v][j] = newdist;
                }
            }
        }
    }
}
int search(int from, int to)
{
    return dist[from][to];
}
int search1(int from, int to)
{
    return annealing_dist[from][to];
}
const int maxnodes = 200000;
const int maxedges = 1000000;
int edges;
int last[maxnodes], head[maxedges], previous[maxedges], len[maxedges];
int pred[maxnodes];
int prio[node][node];
void graphClear()
{
    fill(last, last + maxnodes, -1);
    edges = 0;
}

void addEdge(int u, int v, int length)
{
    head[edges] = v;
    len[edges] = length;
    previous[edges] = last[u];
    last[u] = edges++;
}

// heap
int h[maxnodes];
int pos2Id[maxnodes];
int id2Pos[maxnodes];
int hsize;

void hswap(int i, int j)
{
    swap(h[i], h[j]);
    swap(pos2Id[i], pos2Id[j]);
    swap(id2Pos[pos2Id[i]], id2Pos[pos2Id[j]]);
}

void moveUp(int pos)
{
    while (pos > 0)
    {
        int parent = (pos - 1) >> 1;
        if (h[pos] >= h[parent])
        {
            break;
        }
        hswap(pos, parent);
        pos = parent;
    }
}

void add(int id, int prio)
{
    h[hsize] = prio;
    pos2Id[hsize] = id;
    id2Pos[id] = hsize;
    moveUp(hsize++);
}

void increasePriority(int id, int prio)
{
    int pos = id2Pos[id];
    h[pos] = prio;
    moveUp(pos);
}

void moveDown(int pos)
{
    while (pos < (hsize >> 1))
    {
        int child = 2 * pos + 1;
        if (child + 1 < hsize && h[child + 1] < h[child])
        {
            ++child;
        }
        if (h[pos] <= h[child])
        {
            break;
        }
        hswap(pos, child);
        pos = child;
    }
}

int removeMin()
{
    int res = pos2Id[0];
    int lastNode = h[--hsize];
    if (hsize > 0)
    {
        h[0] = lastNode;
        int id = pos2Id[hsize];
        id2Pos[id] = 0;
        pos2Id[0] = id;
        moveDown(0);
    }
    return res;
}

void heap(int s)
{
    fill(pred, pred + maxnodes, -1);
    // fill(prio, prio + maxnodes, INT_MAX);
    prio[s][s] = 0;
    hsize = 0;
    add(s, 0);
    while (hsize)
    {
        int u = removeMin();
        for (int e = last[u]; e >= 0; e = previous[e])
        {
            int v = head[e];
            int nprio = prio[s][u] + len[e];
            if (prio[s][v] > nprio)
            {
                if (prio[s][v] == INT_MAX)
                    add(v, nprio);
                else
                    increasePriority(v, nprio);
                prio[s][v] = nprio;
                pred[v] = u;
            }
        }
        counter++;
    }
}
queue<int> q;
void Annealing(int index)
{
    while(!q.empty())
        q.pop();
    int Temperature = node * 2 -10; // initial T
    bool Check_All_Visited = 0;
        srand(time(NULL));
        visit[index] = 1;
        q.push(index);
        while (!q.empty()&&Temperature)
        {
            int top = q.front();
            while (!adj[top].empty()&&Temperature)
            {
                //cerr<<"evan"<<endl;
                int random_choose = rand() % adj[top].size();
                if (!visit[adj[top][random_choose]])
                {
                    visit[adj[top][random_choose]] = 1;
                    q.push(adj[top][random_choose]);
                    annealing_dist[index][adj[top][random_choose]] = min(annealing_dist[index][adj[top][random_choose]], table[index][adj[top][random_choose]]+table[top][adj[top][random_choose]]);
                    //cout<<annealing_dist[top][adj[top][random_choose]]<<endl;
                        swap(adj[top][adj[top].size() - 1], adj[top][random_choose]);
                }
                adj[top].pop_back();
                Temperature--;
            }
            q.pop();
        }
    
}
int main()
{
    for (int i = 0; i < node; i++)
        for (int j = 0; j < node; j++)
        {
            table[i][j] = INT_MAX;
            dist[i][j] = INT_MAX;
            annealing_dist[i][j] = INT_MAX;
        }
    for (int i = 0; i < node; i++)
        for (int j = 0; j < node; j++)
            prio[i][j] = INT_MAX;
    bool success;
    srand(time(NULL));
    graphClear();
    StringReference *errorMessage = new StringReference();
    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();

    double xsb[] = {-8, -8}; // Background Limit
    vector<double> xs2(xsb, xsb + sizeof(xsb) / sizeof(double));
    double ysb[] = {110, -8};
    vector<double> ys2(ysb, ysb + sizeof(ysb) / sizeof(double));
    double xsa[] = {90, 110};
    vector<double> xs(xsa, xsa + sizeof(xsa) / sizeof(double));
    double ysa[] = {113, 110};
    vector<double> ys(ysa, ysa + sizeof(ysa) / sizeof(double)); // Till Here

    double xs4[node];
    double ys4[node];

    vector<pair<double, double>> arr;
    int sum = 0;
    while (sum < node)
    {
        int a = getrand(maprange);
        int b = getrand(maprange);

        if (!check[a][b])
        {
            arr.push_back(make_pair(a, b));
            times[sum] = 0;
            check[a][b] = true;
            sum++;
        }
    }

    for (int i = 0; i < node; i++)
    {
        xs4[i] = arr[i].first;
        ys4[i] = arr[i].second;
    }

    vector<double> xsd(xs4, xs4 + sizeof(xs4) / sizeof(double));
    vector<double> ysd(ys4, ys4 + sizeof(ys4) / sizeof(double));
    vector<double> xse(xs4, xs4 + sizeof(xs4) / sizeof(double));
    vector<double> yse(ys4, ys4 + sizeof(ys4) / sizeof(double));
    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 600;
    settings->height = 400;
    settings->autoBoundaries = true;
    settings->autoPadding = true;
    settings->title = toVector(L"");
    settings->xLabel = toVector(L"");
    settings->yLabel = toVector(L"");

    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings(); // Background Limit
    series->xs = &xs;
    series->ys = &ys;
    series->linearInterpolation = true;
    series->pointType = toVector(L"solid");
    series->color = CreateRGBColor(1, 1, 1);

    ScatterPlotSeries *series2 = GetDefaultScatterPlotSeriesSettings();
    series2->xs = &xs2;
    series2->ys = &ys2;
    series2->linearInterpolation = true;
    series2->lineType = toVector(L"solid");
    series2->lineThickness = 2;
    series2->color = CreateRGBColor(1, 1, 1); // Till Here

    ScatterPlotSeries *series3 = GetDefaultScatterPlotSeriesSettings();
    series3->xs = &xsd;
    series3->ys = &ysd;
    series3->linearInterpolation = false;
    series3->pointType = toVector(L"dots");
    series3->color = CreateRGBColor(0, 0, 0);

    settings->scatterPlotSeries->push_back(series);
    settings->scatterPlotSeries->push_back(series2);
    settings->scatterPlotSeries->push_back(series3);

    int random2;
    vector<double> xsf[node * 2];
    vector<double> ysf[node * 2];
    int i = 0;
    while (i < node * 2)
    {
        random2 = getrand(node);
        while (!xsf[i].empty())
            xsf[i].pop_back();
        while (!ysf[i].empty())
            ysf[i].pop_back();

        xsf[i].push_back(xs4[i / 2]);
        ysf[i].push_back(ys4[i / 2]);
        if (times[random2] <= 2)
        {
            xsf[i].push_back(xs4[random2]);
            ysf[i].push_back(ys4[random2]);
        }
        ScatterPlotSeries *series6 = GetDefaultScatterPlotSeriesSettings();
        if (sqrt((xsf[i][1] - xsf[i][0]) * (xsf[i][1] - xsf[i][0]) + (ysf[i][1] - ysf[i][0]) * (ysf[i][1] - ysf[i][0])) > 25)
            continue;
        series6->xs = &xsf[i];
        series6->ys = &ysf[i];
        series6->linearInterpolation = true;
        series6->pointType = toVector(L"solid");
        series6->color = CreateRGBColor(0, 0, 0);
        settings->scatterPlotSeries->push_back(series6);
        times[i / 2]++;
        table[i / 2][random2] = sqrt((xsf[i][1] - xsf[i][0]) * (xsf[i][1] - xsf[i][0]) + (ysf[i][1] - ysf[i][0]) * (ysf[i][1] - ysf[i][0]));
        table[random2][i / 2] = table[i / 2][random2];
        addEdge(i / 2, random2, table[i / 2][random2]);
        addEdge(random2, i / 2, table[i / 2][random2]);
        adj[i / 2].push_back(random2);
        adj[random2].push_back(i / 2);
        // cout<<i / 2<<' '<<random2<<endl;
        i++;
    }
    /*for (int i = 0; i < node; i++)
    {
        cout << i << ':';
        for (int j = 0; j < adj[i].size(); j++)
            cout << adj[i][j] << ' ';
        cout << endl;
    }*/
    double duration = 0; // Implement Dijkstra with Array
    clock_t Begin, End;
    Begin = clock();
    for (int i = 0; i < node; i++)
        Dijkstra(node, i);
    End = clock();
    duration = double(End - Begin) / CLOCKS_PER_SEC;
    cout << "Dijkstra:" << endl;
    cout << fixed << setprecision(6) << duration << endl;
    int Sum = 0;
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < node; j++)
            Sum += dist[i][j];
    // End of Implementation of Array
    // cout<<Sum/(node*(node-1))<<endl;
    // Implement Dijkstra with Binary Heap
    Begin = clock();
    for (int i = 0; i < node; i++)
    {
        for(int j=0;j<node;j++)
            visit[j]=0;
         Annealing(i);
    }
       
    End = clock();
    duration = double(End - Begin) / CLOCKS_PER_SEC;
    cout << "Annealing:" << endl;
    cout << fixed << setprecision(6) << duration << endl;
    cout << "input from and end : " << endl;
    int from , to;
    cin>>from>>to;
    cout<<"Dijkstra :"<<endl<<search(from , to)<<endl;
    cout<<"Annealing :"<<endl<<search1(from , to)<<endl;
    /*for (int i = 0; i < 30; i++)
     {
         for (int j = 0; j < 30; j++)
         {
             //cout << prio[i][j] << ' ';
             cout << annealing_dist[i][j] << ' ';
         }
         puts("");
     }*/
    success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

    if (success)
    {
        vector<double> *pngdata = ConvertToPNG(imageReference->image);
        WriteToFile(pngdata, "Phi.png");
        DeleteImage(imageReference->image);
    }
    else
    {
        cerr << "Error: ";
        for (wchar_t c : *errorMessage->string)
        {
            cerr << c;
        }
        cerr << endl;
    }

    return success ? 0 : 1;
}

