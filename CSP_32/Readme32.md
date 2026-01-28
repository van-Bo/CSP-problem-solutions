# 第32次CCF计算机软件能力认证

> 模拟认证 CSP（100 + 100 + 60 + 35 + 0）
> 模拟认证 AcWing（(10/10), (11/11), (3/11), (8/21), (2/10)）

## Q1 仓库规划

### Q1 算法思路

- 模拟即可
- 时间复杂度：$O(m \times n^2)$
- 该题解可以通过 AcWing 官网评测系统下 (10/10) 的数据点

```C++
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;
const int N = 1010;

int n, m;
int w[N][11];
int p[N];

void deal()
{
    for (int i = 1; i <= n; i ++)
    {
        int father = 0;

        for (int j = 1; j <= n; j ++)
        {
            bool isValid = true;
            for (int k = 1; k <= m; k ++)
                if (w[i][k] >= w[j][k])
                {
                    isValid = false;
                    break;
                }
                
            if (isValid)
            {
                father = j;
                break;
            }
        }
        p[i] = father;
    }
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++)
        for (int j = 1; j <= m; j ++)
            scanf("%d", &w[i][j]);
            
    deal();
    
    for (int i = 1; i <= n; i ++)
        printf("%d\n", p[i]);
    return 0;
}
```

## Q2 因子化简

### Q2 算法思路

- 分解质因数、快速幂
- 对于任意一个数 n，其最多仅有一个大于 $sqrt(n)$ 的质因子，因为阈值 k 的数据范围大于 1，所以在分解质因子的过程中，便可以省去针对此情况的特殊讨论
- 本题中的输入数据范围超出 int 最大范围，数据类型使用 long long，避免出现爆 int 的情况
- 该题解可以通过 AcWing 官网评测系统下 (11/11) 的数据点
- 此处的时间复杂度感觉很玄妙：👇
- n 中最多只包含一个大于 $sqrt(n)$ 的质因子，枚举所有数2-n -> 枚举2-$sqrt(n)$，-> $O(sqrt(n))$优化
- 若 n = $2 ^ k$，时间复杂度为 $logn$
- 综上，$O(logn-sqrt(n))$

```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;

int n;

LL qmi(LL a, LL b)
{
    LL res = 1;
    while (b)
    {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

LL deal(LL x, LL k)
{
    LL res = 1;
    for (LL i = 2; i <= x / i; i ++)
    {
        if (x % i == 0)
        {
            int cnt = 0;
            while (x % i == 0)
            {
                cnt ++;
                x /= i;
            }
            if (cnt >= k) res *= qmi(i, cnt);
        }
    }
    return res;
}

int main()
{
    scanf("%d", &n);
    
    while (n --)
    {
        LL x, k;
        scanf("%lld%lld", &x, &k);
        
        LL res = deal(x, k);
        printf("%lld\n", res);
    }
    return 0;
}
```

## Q3 树上搜索（Hack😔😕）

### Q3 算法思路

- 朴素暴力模拟
- 邻接表存储树的相关信息
- 输出的序列是在有效的节点数量大于 1 的情况下，依据二分策略，所选择的分界点
- `weights` 维护每次二分策略时，每个有效节点的相对权值差，之后针对 `weighs` 进行双关键字排序 `sort` 即可得到本次二分策略的分界点；
- `getWeightBranch(tar, rangeValid)` 返回在 `rangeValid` 集合（有效节点集）中，以 `tar` 为根节点的子树的权值总和
- `getPointBranch(tar, search)` 求解在 `search` 集合（有效节点集）中，以 `tar` 为根节点的子树的所有节点集合，并将结果存储在全局变量 `des` 中
- `res` 记录每次二分策略下所选择的分界点
- 该题解可以通过 AcWing 官网 (3/11) 的数据点

```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <cstdlib>

#define x first
#define y second

using namespace std;
typedef long long LL;
typedef pair<int, LL> PII;
const int N = 2010;

int n, m;
int w[N], p[N];
int h[N], e[N], ne[N], idx;

vector<PII> weights;
unordered_set<int> des;
vector<int> res;

void add(int p, int k)
{
    e[idx] = k, ne[idx] = h[p], h[p] = idx ++;
}

LL getWeightBranch(int tar, unordered_set<int> rangeValid)
{
    if (rangeValid.find(tar) == rangeValid.end()) return 0;
    
    LL res = w[tar];
    for (int i = h[tar]; ~i; i = ne[i])
    {
        int j = e[i];
        
        if (rangeValid.find(j) != rangeValid.end())
            res += getWeightBranch(j, rangeValid);
    }
    return res;
}

void getPointBranch(int tar, unordered_set<int> search)
{
    des.insert(tar);
    for (int i = h[tar]; ~i; i = ne[i])
    {
        int j = e[i];
        if (search.find(j) == search.end()) continue;
        
        des.insert(j);
        getPointBranch(j, search);
    }
}

// tar 为搜索目标，searsh 为有效的搜索范围
void dfs(int tar, unordered_set<int> search)
{
    if (search.size() == 1) return;
    
    weights.clear();
    
    // 有效节点的权值总和
    LL weightTotal = 0;
    for (auto s : search) weightTotal += w[s];
    
    // 维护二分策略下的选择权值集合
    for (auto s : search)
    {
        LL weightBranch = 0;
        weightBranch = getWeightBranch(s, search);
        weights.push_back({llabs(weightTotal - weightBranch * 2), s});
    }
    
    sort(weights.begin(), weights.end());
    
    auto t = weights[0];
    res.push_back(t.y);
    
    des.clear();
    getPointBranch(t.y, search);

    if (des.find(tar) == des.end())
    {
        for (auto del : des) search.erase(del);
        
        dfs(tar, search);
    }
    else
        dfs(tar, des);

}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++) scanf("%d", &w[i]);
    for (int i = 2; i <= n; i ++) scanf("%d", &p[i]);
    
    memset(h, -1, sizeof h);
    for (int i = 2; i <= n; i ++)
        add(p[i], i);
    
    while (m --)
    {
        int x;
        scanf("%d", &x);
        
        unordered_set<int> search;
        for (int i = 1; i <= n; i ++) search.insert(i);
        
        res.clear();
        dfs(x, search);
        
        for (auto t : res)
            printf("%d ", t);
        puts("");
    }
    return 0;
}
```

## Q4 宝藏（Hack😔😕）

### Q4 算法思路

- 朴素暴力模拟
- 双端队列 `deque` 相关操作库函数的使用
- `cmd[x]` 存储第 x 条命令的相关信息；`dp` 是一个 **double-ended queue** 双端队列，在处理第 l 到 r 条命令并生成的密码时，使用 `dp` 来存储矩阵信息；`vector<int> lastPos` 维护一个 0、1 的序列，在处理第 l 到 r 条命令时，若操作的命令是向队头 push 元素，则在 `lastPos` 末端插入标记数字 0，若操作的命令是向队尾 push 元素，则在 `lastPos` 末端插入标记数字 1，若操作的命令是删除最后插入的元素，则可以参照 `lastPos` 存储的标记数字，来确定在 `dp` 中的删除位置（对头 or 队尾）
- 该题解可以通过 AcWing 评测系统下 (8/21) 的数据点

```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <deque>

using namespace std;
typedef long long LL;
const int N = 1e+5 + 10, P = 998244353;

int n, m;
struct Matrix
{
    int x11, x12, x21, x22;
};
struct Command
{
    int op, x11, x12, x21, x22;
};

struct Command cmd[N];
deque<Matrix> dp;
vector<int> lastPos;

void dealCmd(Command t)
{
    Matrix r = {t.x11, t.x12, t.x21, t.x22};
    if (t.op == 1)
    {
        dp.push_front(r);
        lastPos.push_back(0);
    }
    else if (t.op == 2)
    {
        dp.push_back(r);
        lastPos.push_back(1);
    }
    else 
    {
        if (dp.size())
        {
            if (lastPos.back() == 1)
                dp.pop_back();
            else if (lastPos.back() == 0)
                dp.pop_front();
            
            lastPos.pop_back();
        }
    }
}

Matrix timesMatrix(Matrix a, Matrix b)
{
    Matrix res;
    res.x11 = ((LL)a.x11 * b.x11 % P + (LL)a.x12 * b.x21 % P) % P;
    res.x12 = ((LL)a.x11 * b.x12 % P + (LL)a.x12 * b.x22 % P) % P;
    res.x21 = ((LL)a.x21 * b.x11 % P + (LL)a.x22 * b.x21 % P) % P;
    res.x22 = ((LL)a.x21 * b.x12 % P + (LL)a.x22 * b.x22 % P) % P;
    return res;
}

Matrix calCmd(int l, int r)
{
    dp.clear();
    lastPos.clear();
    
    for (int i = l; i <= r; i ++)
        dealCmd(cmd[i]);
        
    Matrix res = {1, 0, 0, 1};
    
    int sz = dp.size();
    for (int i = 0; i < sz; i ++)
    {
        Matrix &t = dp[i];
        
        Matrix r = {t.x11, t.x12, t.x21, t.x22};
        res = timesMatrix(res, r);
    }
    return res;
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++)
    {
        int op, x11, x12, x21, x22;
        scanf("%d", &op);
        if (op == 3)
            x11 = x12 = x21 = x22 = 0;
        else
            scanf("%d%d%d%d", &x11, &x12, &x21, &x22);
        cmd[i] = {op, x11, x12, x21, x22};
    }
    
    for (int i = 0; i < m; i ++)
    {
        int op;
        scanf("%d", &op);
        if (op == 1)
        {
            int cmdID;
            scanf("%d", &cmdID);

            int op1, x11, x12, x21, x22;
            scanf("%d", &op1);
            
            if (op1 == 3)
                x11 = x12 = x21 = x22 = 0;
            else
                scanf("%d%d%d%d", &x11, &x12, &x21, &x22);

            cmd[cmdID] = {op1, x11, x12, x21, x22};
        }
        else
        {
            int l, r;
            scanf("%d%d", &l, &r);
            
            Matrix res = calCmd(l, r);
            printf("%d %d %d %d\n", res.x11, res.x12, res.x21, res.x22);
        }
    }
    return 0;
}
```

## Q5 彩色路径（Hack😔😕）

### Q5 算法思路

- 使用邻接表存储图的相关信息
- dfs 深搜维护最长的路径长度
- `dfs(x, base)` 的函数表示已经成功遍历到节点 `x`，此时由起始节点 `0` 到当前节点 `x` 的路径长度为 `base`
- 该题解可以过掉 AcWing 官网评测系统下 (2/10) 的数据点

```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;
const int N = 110, M = 5010;

int n, m, l, k;
int c[N], u[M], v[M], d[M];
int h[N], e[M], ne[M], w[M], idx;
int res, temp, step;
unordered_set<int> colorMap;

void add(int p, int chd, int value)
{
    e[idx] = chd, w[idx] = value;
    ne[idx] = h[p], h[p] = idx ++;
}

void dfs(int x, int base)
{
    res = max(res, base);
    if (x == n - 1) return;
    
    step ++;
    colorMap.insert(c[x]);

    for (int i = h[x]; ~i; i = ne[i])
    {
        int j = e[i];
        
        if (colorMap.find(c[j]) != colorMap.end()) continue;
        if (step >= l) continue;
        
        dfs(j, base + w[i]);
    }
    
    step --;
    colorMap.erase(c[x]);
}

int main()
{
    scanf("%d%d%d%d", &n, &m, &l, &k);
    for (int i = 0; i < n; i ++) scanf("%d", &c[i]);
    for (int i = 0; i < m; i ++) scanf("%d", &u[i]);
    for (int i = 0; i < m; i ++) scanf("%d", &v[i]);
    for (int i = 0; i < m; i ++) scanf("%d", &d[i]);
    
    memset(h, -1, sizeof h);
    for (int i = 0; i < m; i ++) add(u[i], v[i], d[i]);
    
    dfs(0, 0);

    printf("%d\n", res);
    return 0;
}
```
