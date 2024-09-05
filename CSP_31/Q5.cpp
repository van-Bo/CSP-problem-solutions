#include <iostream>
#include <cstring>
#include <vector>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 305;

int n, m;
int h[N], e[N], ne[N], edgeID[N], idx;
PII edge[N];
bool st[N];

int hurt[N][N];
int res;

void add(int u, int v, int k)
{
    edgeID[idx] = k, e[idx] = v, ne[idx] = h[u], h[u] = idx ++; 
}

void dfs(int u, int v, int cnt)
{
    st[v] = true;
    for (int i = h[v]; ~i; i = ne[i])
    {
        int j = e[i], id = edgeID[i];
        if (st[j]) continue;

        hurt[u][j] = cnt + (edge[id].y - edge[id].x);
        dfs(u, j, hurt[u][j]);
    }
}

void getHurt()
{
    for (int start = 1; start <= n; start ++)
    {
        memset(st, 0, sizeof st);
        dfs(start, start, 0);
    }
}

int getMaxHurt()
{
    int res = 0;
    for (int i = 1; i <= n; i ++)
        for (int j = i + 1; j <= n; j ++)
            if (res < hurt[i][j])
                res = hurt[i][j];
    return res;
}

int main()
{
    scanf("%d%d", &n, &m);

    if (n > 300 || m > 300) return 0;

    memset(h, -1, sizeof h);
    for (int i = 1; i <= n - 1; i ++)
    {
        int u, v, w, b;
        scanf("%d%d%d%d", &u, &v, &w, &b);

        edge[i] = {w, b};
        add(u, v, i), add(v, u, i);
    }

    getHurt();
    res = getMaxHurt();
    printf("%d\n", res);

    while (m --)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        edge[x].x = y;

        memset(hurt, 0, sizeof hurt);
        getHurt();
        res = getMaxHurt();
        printf("%d\n", res);
    }
    system("pause");
    return 0;
}