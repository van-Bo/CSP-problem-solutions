# 第10次CCF计算机软件能力认证

> smqyOJ Judge((), (), (), (20/20))

## Q4 地铁修建

### Q4 算法思路(demo1, TLE)

- "最小瓶颈路"问题(Minimax Path)
- 使用邻接表存储图信息
- `dist[i]` 表示从 1 号站点到 i 号站点的所有路径中的最大子边权值的最小值
- 暴力搜索，使用 dfs 遍历所有路径，记录路径上的最大边权值，更新最小的最大边权值
- 该题解可以通过 smqyOJ (5/20)的测试点，得分 25 分

### Q4 代码实现(demo1, TLE)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+5 + 10, M = 2e+5 + 10;

int n, m;
int h[N], e[M * 2], ne[M * 2], value[M * 2], idx;
int dist[N];
bool st[N];

void add(int a, int b, int c)
{
    e[idx] = b, value[idx] = c, ne[idx] = h[a], h[a] = idx ++;
}

void dfs(int v, int base)
{
    dist[v] = min(dist[v], base);
    st[v] = true;
    for (int i = h[v]; ~i; i = ne[i])
    {
        int u = e[i];
        if (st[u]) continue;
        int new_base = max(value[i], base);
        dfs(u, new_base);
    }
    st[v] = false;
}

int main()
{
    memset(h, -1, sizeof h);
    cin >> n >> m;
    for (int i = 0; i < m; i ++)
    {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        add(a, b, c), add(b, a, c);
    }

    memset(dist, 0x3f, sizeof dist);
    dfs(1, 0);
    cout << dist[n] << endl;
    return 0;
}
```

### Q4 算法思路(demo2, TLE)

- 使用邻接表存储图信息
- 使用 Dijkstra 算法求解最短路径
- 该题求解的是从 1 号站点到 n 号站点的所有路径中的最大子边权值的最小值，是"最小瓶颈路"问题(Minimax Path)，使用魔改版的 Dijkstra 算法
- 该题解可以通过 smqyOJ (16/20)的测试点，得分 80 分

### Q4 魔改版 Dijkstra 算法分析

- Dijkstra 算法的核心基础是**贪心**策略和**路径权值的单调递增性**
- 在标准的 Dijkstra 求最短路中，路径的权值是把所有边的长度相加：$dist[v] = dist[t] + weight(t, v)$因为边权都是正数，所以路径越长，总和一定越来越大（或至少不减）。这就保证了每次从未知集合中挑出的那个 dist 最小的点，它的最短路已经被找到了，不可能再通过其他更绕的路径变得更短。
- 在的"最小瓶颈路"问题中，路径的权值变成了取最大值：$dist[v] = \max(dist[t], weight(t, v))$，同样地，随着不断向路径中加入新的边，这条路径上的最大边权也一定是越来越大（或保持不变）的。它完美地继承了“单调不减”的特性。因此，Dijkstra 的贪心核心（即局部最优推导全局最优）在这里依然完全成立。一旦一个节点 $t$ 作为当前 dist 最小的节点被选中（标记为 st[t] = true），它到起点的最小瓶颈也就是最终结果了，后续不可能被其他路径更新得更小。
- 时间复杂度为：$n^2$，n 是站点数量

### Q4 代码实现(demo2, TLE)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+5 + 10, M = 2e+5 + 10;

int n, m;
int h[N], e[M * 2], ne[M * 2], value[M * 2], idx;
int dist[N];
bool st[N];

void add(int a, int b, int c)
{
    e[idx] = b, value[idx] = c, ne[idx] = h[a], h[a] = idx ++;
}

int Dijkstra()
{
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;

    for (int i = 0; i < n; i ++)
    {
        int t = -1;
        for (int j = 1; j <= n; j ++)
        {
            if ((!st[j]) && (t == -1 || dist[j] < dist[t]))
                t = j;
        }

        st[t] = true;
        if (t == n) break;

        for (int j = h[t]; ~j; j = ne[j])
        {
            int v = e[j];
            dist[v] = min(dist[v], max(dist[t], value[j]));
        }
    }
    return dist[n];
}

int main()
{
    memset(h, -1, sizeof h);
    cin >> n >> m;
    for (int i = 0; i < m; i ++)
    {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        add(a, b, c), add(b, a, c);
    }

    int res = Dijkstra();
    cout << res << endl;
    return 0;
}
```

### Q4 算法思路(demo3, AC)

- 使用堆优化的魔改版 Dijkstra 算法，时间复杂度为：$m \log n$，m 是边的数量，n 是站点数量
- 该题解可以通过 smqyOJ (20/20)的测试点，得分 100 分

### Q4 代码实现(demo3, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> PII;
#define x first
#define y second
const int N = 1e+5 + 10, M = 2e+5 + 10;

int n, m;
int h[N], e[M * 2], ne[M * 2], value[M * 2], idx;
int dist[N];
bool st[N];

void add(int a, int b, int c)
{
    e[idx] = b, value[idx] = c, ne[idx] = h[a], h[a] = idx ++;
}

int Dijkstra()
{
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;

    // 优先队列，默认是大根堆，使用 greater 变成小根堆
    priority_queue<PII, vector<PII>, greater<PII>> heap;
    heap.push({0, 1});

    while (heap.size())
    {
        auto temp = heap.top();
        heap.pop();
        int t = temp.y;

        st[t] = true;
        if (t == n) break;

        for (int i = h[t]; ~i; i = ne[i])
        {
            int v = e[i];
            if (st[v]) continue;
            if (dist[v] > max(dist[t], value[i]))
            {
                dist[v] = max(dist[t], value[i]);
                heap.push({dist[v], v});
            }
        }

    }
    return dist[n];
}

int main()
{
    memset(h, -1, sizeof h);
    cin >> n >> m;
    for (int i = 0; i < m; i ++)
    {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        add(a, b, c), add(b, a, c);
    }

    int res = Dijkstra();
    cout << res << endl;
    return 0;
}
```

### Q4 算法分析(demo4, AC)

- 并查集 + 贪心
- 整条路线中的最大边权尽可能小：
    1. 从小到大尝试修建隧道
    2. 将所有候选隧道按修建天数 $c$ 从小到大排序
    3. 依次把这些隧道加入图中（使用并查集维护连通性）
    4. 一旦发现枢纽 $1$ 和枢纽 $n$ 连通了，那么刚才加入的那条隧道的修建天数，就是最终答案
- **可行性分析**：因为我们是按天数从小到大添加隧道的。当 $1$ 和 $n$ 刚刚连通的那一刻，必然是通过一条路径连接起来的，而这条路径上耗时最长的边，就是刚刚加进去的这条边。这就保证了我们找到的“最大耗时”是所有可行路径中最小的。
- 该题解可以通过 smqyOJ (20/20)的测试点，得分 100 分

### Q4 代码实现(demo4, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+5 + 10, M = 2e+5 + 10;

int n, m;
struct Edge
{
    int a, b, c;
    bool operator < (const struct Edge &t)
    {
        return c < t.c;
    }
}edges[M];
int p[N];

int find(int x)
{
    if (p[x] != x) p[x] = find(p[x]);
    return p[x]; 
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < m; i ++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        edges[i] = {a, b, c};
    }
    sort(edges, edges + m);

    for (int i = 1; i <= n; i ++) p[i] = i;

    for (int i = 0; i < m; i ++)
    {
        int a = edges[i].a, b = edges[i].b, c = edges[i].c;

        int pa = find(a), pb = find(b);
        if (pa != pb) p[pb] = pa;

        if (find(1) == find(n))
        {
            cout << c << endl;
            break;
        } 
    }
    return 0;
}
```