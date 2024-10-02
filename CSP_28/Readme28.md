> CSP（第28次CCF计算机软件能力认证）
> 模拟认证（）

# Q1 现值计算
## 算法思路

# Q2 训练计划
## 算法思路

# Q3 JPEG编码
## 算法思路
- 模拟即可
- `getM()` 将扫描序列 `scan[]` 按照 z 形填充至 `m[][]`
- `getOrder()` 获取 `m[x][y]` 元素在扫描序列中的序号位置，涉及数学推导
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <math.h>

using namespace std;
const int N = 10;

int q[N][N], m[N][N], ans[N][N];
int T, n;
int scan[N * N];
double mt[N][N];

int getOrder(int x, int y)
{
    int res = 0;
    
    int mark = x + y;   // 该位置的斜线标记为 mark，前面完整的斜线有 mark 条 
    if (mark <= 8) res += (1 + mark) * (mark) / 2;
    else res += (1 + 8) * 8 / 2 + (7 + 7 - (mark - 9)) * (mark - 8) / 2;
    
    if (mark % 2 == 0)
    {
        int x_max = -1;
        if (mark <= 7) x_max = mark;
        else x_max = 7;
        res += x_max - x + 1;
    }
    else
    {
        int x_min = -1;
        if (mark <= 7) x_min = 0;
        else x_min = mark - 7;
        res += x - x_min + 1;
    }   
    return res;
}

void getM()
{
    for (int i = 0; i < 8; i ++)
    {
        for (int j = 0; j < 8; j ++)
        {
            int order = getOrder(i, j);
            if (order > n) continue;
            m[i][j] = scan[order];
        }
    }
}

double getAlpha(int v)
{
    double root = 0.5;
    if (v == 0) return sqrt(root);
    else return 1;
}

void getMt()
{
    for (int i = 0; i < 8; i ++)
    {
        for (int j = 0; j < 8; j ++)
        {
            double res = 0;
            for (int u = 0; u < 8; u ++)
                for (int v = 0; v < 8; v ++)
                {
                    double pi = acos(-1);
                    res += getAlpha(u) * getAlpha(v) * m[u][v] * cos(pi * (i + 0.5) * u / 8) * cos(pi * (j + 0.5) * v / 8);
                }
            mt[i][j] = res / 4;
        }
    }
}

void printMatrix()
{
    if (T == 0 || T == 1)
    {
        for (int i = 0; i < 8; i ++)
        {
            for (int j = 0; j < 8; j ++)
                printf("%d ", m[i][j]);
            puts("");
        }
    }
    else if (T == 2)
    {
        for (int i = 0; i < 8; i ++)
        {
            for (int j = 0; j < 8; j ++)
                printf("%d ", ans[i][j]);
            puts("");
        }
    }
}

int main()
{
    // step 1
    for (int i = 0; i < 8; i ++)
        for (int j = 0; j < 8; j ++)
            scanf("%d", &q[i][j]);
    
    scanf("%d%d", &n, &T);
    for (int i = 1; i <= n; i ++) scanf("%d", &scan[i]);
    
    // step 3        
    getM();
    if (T == 0)
        printMatrix();
    
    // step 4
    for (int i = 0; i < 8; i ++)
        for (int j = 0; j < 8; j ++)
            m[i][j] *= q[i][j];
    if (T == 1)
        printMatrix();

    // step 5
    getMt();
    
    // step 6
    for (int i = 0; i < 8; i ++)
        for (int j = 0; j < 8; j ++)
        {
            mt[i][j] += 128;
            int res = round(mt[i][j]);
            res = max(res, 0);
            res = min(res, 255);
            ans[i][j] = res;
        }
    if (T == 2)
        printMatrix();
    
    return 0;
}
```

# Q4 聚集方差（Hack）
## 算法思路
- 朴素模拟
- 数的信息使用邻接表存储
- `dfs` 可以以递归的方式求解以 `u` 为根节点子树的节点集合 T，此处使用 `vector<int> T[x]` 来存储以节点 `x` 为根节点子树的节点集合。其中 `N` 按照最大规模 `3e+5` 计算，最坏情况下，内存占用约为 45000 x 4MB (`4.5e+10 x 4` )，存储规模过大，导致出现 $Memory\ Limit\ Exceeded$ 的错误
- 该题解 AcWing 官网可以通过 (2/10) 的数据点，CSP 官网评测系统下得分为 0 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>

using namespace std;
const int N = 3e+5 + 10;
typedef long long LL;

int n;
int h[N], e[N], ne[N], idx;
int a[N];
vector<int> T[N];

void add(int p, int k)
{
    e[idx] = k, ne[idx] = h[p], h[p] = idx ++;
}

// 获取以 u 为根节点子树的节点集合 T
void dfs(int u)
{
    T[u].push_back(u);
    
    for (int i = h[u]; ~i; i = ne[i])
    {
        int j = e[i];
        dfs(j);
        T[u].insert(T[u].end(), T[j].begin(), T[j].end());
    }
}

LL getAns(int u)
{
    LL res = 0;
    int sz = T[u].size();
    if (sz == 1) return 0;
    
    for (int i = 0; i < sz; i ++)
    {
        int y = T[u][i];
        LL mnValue = 0x3f3f3f3f;
        for (int j = 0; j < sz; j ++)
        {
            if (i == j) continue;
            int z = T[u][j];
            mnValue = min(mnValue, (LL)(a[z] - a[y]) * (a[z] - a[y]));
        }
        res += mnValue;
    }
    return res;
}

int main()
{
    scanf("%d", &n);
    memset(h, -1, sizeof h);
    for (int i = 2; i <= n; i ++)
    {
        int p;
        scanf("%d", &p);
        add(p, i);
    }
    for (int i = 1; i <= n; i ++)
        scanf("%d", &a[i]);
    
    dfs(1);
    for (int i = 1; i <= n; i ++)
        printf("%lld\n", getAns(i));

    return 0;
}
```
- 目标为 `N <= 3000` 的小规模数据
- `st[p][k]` 标记节点 `p` 是否可以支配 `k`
- `getSt` 通过枚举子节点，通过父节点标记数组 `p[]` 逆向回溯，直到到达根节点，以此达到填充状态数组 `st[][]` 的目的
- 时间复杂度：$O(n^3)$
- 该题解 AcWing 可以通过 (2/10) 的数据点，CSP 官网评测系统下的得分为 15 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>

using namespace std;
const int N = 3010;
typedef long long LL;

int n;
int a[N], p[N];
bool st[N][N];

void getSt()
{
    for (int i = 1; i <= n; i ++)
    {
        st[i][i] = true;
        
        int fa = p[i];
        while (fa != -1)
        {
            st[fa][i] = true;
            fa = p[fa];
        }
    }
}

LL getAns(int u)
{
    LL res = 0;
    
    int cnt = 0;
    for (int i = 1; i <= n; i ++)
        if (st[u][i])
            cnt ++;
    if (cnt == 1) 
        return 0;
    
    for (int y = 1; y <= n ; y ++)
    {
        if (!st[u][y]) continue;
        
        LL mnValue = -1;
        for (int z = 1; z <= n; z ++)
        {
            if (y == z || !st[u][z]) continue;

            if (mnValue == -1) mnValue = (LL)(a[z] - a[y]) * (a[z] - a[y]);
            else mnValue = min(mnValue, (LL)(a[z] - a[y]) * (a[z] - a[y]));
        }
        res += mnValue;
    }

    return res;
}

int main()
{
    scanf("%d", &n);
    if (n > 3000) return 0;

    p[1] = -1;
    for (int i = 2; i <= n; i ++) scanf("%d", &p[i]);
    for (int i = 1; i <= n; i ++) scanf("%d", &a[i]);
    
    getSt();
    for (int i = 1; i <= n; i ++)
        printf("%lld\n", getAns(i));

    return 0;
}
```
- 子树期待时间集合存储优化，使用 `multiset` 存储有序序列
- `multiset<int> T[x]` 存储的是以 `x` 为根节点的子树中的所用期待时间值（可能存在重复的值）。调试多次，共计两种错误，其一，使用了 `set` 的数据结构；其二，存储的数据是节点的期待时间，而不是节点的编号（变量名由此显得不太合适，应该是 `A`）
- `multiset` 中若重复存在某一值 `x`，则 `find(x)` 所返回的迭代器是这些重复值中的第一个（即最小的）位置
- 时间复杂度：$O(n^2 \times \log n)$
- 该题解可以通过 AcWing 官网 (2/10) 的数据点，CSP 官网评测系统下的得分为 40 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <set>

using namespace std;
const int N = 3010;
typedef long long LL;

int n;
int a[N], p[N];
bool st[N][N];
multiset<int> T[N];

void getSt()
{
    for (int i = 1; i <= n; i ++)
    {
        st[i][i] = true;
        
        int fa = p[i];
        while (fa != -1)
        {
            st[fa][i] = true;
            fa = p[fa];
        }
    }
}

void getT()
{
    for (int i = 1; i <= n; i ++)
        for (int j = 1; j <= n; j ++)
            if (st[i][j])
                T[i].insert(a[j]);
}

LL getAns(int u)
{
    LL res = 0;
    
    if (T[u].size() == 1) 
        return 0;
    
    for (int y = 1; y <= n ; y ++)
    {
        if (!st[u][y]) continue;
        
        LL mnValue = -1;

        auto it = T[u].find(a[y]);
        auto preIt = (it == T[u].begin()) ? T[u].end() : prev(it);
        auto toIt = next(it);

        if (preIt != T[u].end())
        {
            int v = *preIt;
            if (mnValue == -1) mnValue = (LL)(v - a[y]) * (v - a[y]);
            else mnValue = min(mnValue, (LL)(v - a[y]) * (v - a[y]));
        } 
        if (toIt != T[u].end())
        {
            int v = *toIt;
            if (mnValue == -1) mnValue = (LL)(v - a[y]) * (v - a[y]);
            else mnValue = min(mnValue, (LL)(v - a[y]) * (v - a[y]));
        }

        res += mnValue;
    }

    return res;
}

int main()
{
    scanf("%d", &n);
    if (n > 3000) return 0;

    p[1] = -1;
    for (int i = 2; i <= n; i ++) scanf("%d", &p[i]);
    for (int i = 1; i <= n; i ++) scanf("%d", &a[i]);
    
    getSt();
    getT();

    for (int i = 1; i <= n; i ++) printf("%lld\n", getAns(i));
    return 0;
}
```