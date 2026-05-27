# 第37次CCF计算机软件能力认证

> smqyOJ Judge((10/10) + (20/20))

## Q1 数值积分

### Q1 算法思路

- 模拟即可
- 该题解可以通过 smqyOJ (10/10) 的测试点，得分 100 分

### Q1 代码实现

```cpp
#include <bits/stdc++.h>
using namespace std;

int b, c, l, r;

int deal()
{
    int res = 0;
    if (l & 1) l += 1;
    for (int i = l; i <= r; i += 2)
    {
        res += i * i + b * i + c;
    }
    return res * 2;
}

int main()
{
    scanf("%d%d%d%d", &b, &c, &l, &r);
    cout << deal();
}
```

## Q2 机器人饲养指南

### Q2 算法思路(demo1, AC)

- 完全背包问题
- 该题解可以通过 smqyOJ (20/20) 的测试点，得分 100 分

### Q2 代码实现(demo1, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> PII;
const int M = 110, N = 1e+4 + 10;

int n, m;
int a[M];
int f[M][N];

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= m; i ++) cin >> a[i];

    for (int i = 1; i <= n; i ++)   // 容量
    {
        for (int j = 1; j <= m; j ++)   // 物品
        {
            for (int k = 0; k <= (i / j); k ++) // 个数
                f[j][i] = max(f[j][i], f[j - 1][i - k * j] + k * a[j]);
        }
    }
    cout << f[m][n];
    return 0;
}
```

### Q2 算法思路(demo2, 降维优化)

- 参考链接: [AcWing Notes](https://www.acwing.com/activity/content/code/content/7238497/)
- 该题解可以通过 smqyOJ (20/20) 的测试点，得分 100 分

### Q2 算法思路(demo2, 降维优化)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> PII;
const int M = 110, N = 1e+4 + 10;

int n, m;
int a[M];
int f[N];

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= m; i ++) cin >> a[i];

    for (int i = 1; i <= m; i ++)   // 物品
    {
        for (int j = i; j <= n; j ++)   // 容量
        {
            f[j] = max(f[j], f[j - i] + a[i]);
        }
    }
    cout << f[n];
    return 0;
}
```

## Q3 模板展开

### Q3 算法思路(demo1, RTE)

- 模拟即可，遇到 `$` 递归展开变量，否则直接拼接字符串
- `dic1<string var, string value>` 记录直接赋值变量 `var` 所对应的值 `value`
- `dic2<string var, string parameters>` 记录间接赋值变量 `var` 所对应的参数列表 `parameters`(以空格为间隔)
- `st[string var, bool state]` 记录变量 `var` 是否为间接字符串
- `get_dic2(string k)` 获取间接赋值变量 `k` 所对应的字符串值，获取过程中会遍历 `k` 所对应的参数列表 `dic2[k]`
- `Hack` 分析: 用 `string` 去承接真实地拼接出最终的字符串。来看如下一个极端的例子:

```text
2 b $a $a
2 c $b $b
2 d $c $c
...
2 z $y $y
3 z
```

- 在间接赋值语句互相引用的情况下，字符串的长度是呈指数级($O(2^n)$)增长的。当嵌套层数稍微多一点时，真实的字符串长度会迅速突破几 GB，这会直接导致 `C++` 的 `std::string` 申请内存失败(抛出 `std::bad_alloc` 异常)，从而引发程序的 `RTE`（崩溃退出）。
- 该题解可以通过 smqyOJ (4/10) 的测试点，得分 40 分

### Q3 代码实现(demo1, RTE)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int mod = 1e+9 + 7;
int n;
unordered_map<string, string> dic1; // 存储直接赋值变量
unordered_map<string, string> dic2; // 存储间接赋值变量
unordered_map<string, bool> st;    // 标记变量是否是间接赋值变量

string get_dic2(string k)   // 获取间接变量 k 的值
{
    string res, temp;
    stringstream ss(dic2[k]);

    while (ss >> temp)
    {
        if (temp[0] != '$')
            res += temp;
        else
        {
            temp = temp.substr(1);
            if (st[temp]) res += get_dic2(temp);
            else res += dic1[temp];
        }
    }
    return res;
}

void deal1(string s)    // 直接赋值语句
{
    stringstream ss(s);
    string k, v, temp;

    ss >> k;    // 读入变量名
    st[k] = false;

    while (ss >> temp)
    {
        if (temp[0] != '$')
            v += temp;
        else
        {
            temp = temp.substr(1);  // 跳过 $
            if (st[temp]) v += get_dic2(temp);
            else v += dic1[temp];
        }
    }
    dic1[k] = v;
}

void deal2(string s)    // 间接赋值语句
{
    stringstream ss(s);
    string k, v, temp;

    ss >> k;
    st[k] = true;
    dic2[k] = s.substr(k.length() + 1);
    
    while (ss >> temp)
    {
        if (temp[0] != '$')
            v += temp;
        else
        {
            temp = temp.substr(1);
            if (st[temp]) v += get_dic2(temp);
            else v += dic1[temp];
        }
    }
}

void deal3(string s)
{
    string res;
    if (st[s]) res = get_dic2(s);
    else res = dic1[s];

    LL len = res.length();
    printf("%lld\n", len % mod);
}

int main()
{
    scanf("%d", &n);
    getchar();  // Key point

    for (int i = 0; i < n; i ++)
    {
        string order;
        getline(cin, order);

        if (order[0] == '1')
            deal1(order.substr(2));
        else if (order[0] == '2')
            deal2(order.substr(2));
        else 
            deal3(order.substr(2));
    }
    return 0;
}
```

### Q3 算法思路(demo2, AC)

- 题目的终极要求："输出一行，内容为变量的值的长度除以 $10^9+7$ 的余数。"出题人在这里留了一个巨大的后门：我们全程根本不需要知道字符串到底长什么样，我们只需要知道它有多长！
- 因此，算法可以做如下的优化：
  - 对于直接赋值操作 `deal1`：在赋值的那一刻，直接算出表达式的总长度，并仅仅存储这个 `long long` 类型的长度即可。
  - 对于间接赋值变量 `deal2`：不拼接字符串，而是把表达式按空格拆分成一个个"操作数"(存储为一个 `vector<string>`)。
  - 对于输出操作 `deal3`：递归求长度。遇到字符串，长度就是 `temp.length()`；遇到变量，就去查它的长度。所有的加法都时刻对 $10^9+7$ 取模，彻底告别内存爆炸。
- `get_len(string k)` 获取变量名为 `k` 的长度(该变量可能是间接、直接变量(均不含 `$`))，可能存在递归操作。
- 另一个隐藏陷阱：`TLE`(超时)，如果仅仅是把求字符串换成了求长度，可能会从 `RTE` 变成 `TLE`。因为递归计算长度时，同一个变量可能会在树中被反复展开无数次。解决方案(**记忆化搜索**)：在单次求值(执行 `deal1` 或 `deal3`)的过程中，各个变量的值是不会变的。我们可以准备一个 `cache` 字典。如果某个间接变量的长度在这一次求值中已经被算过了，直接返回缓存结果即可(每次执行语句前清空 `cache`)。
- 该题解可以通过 smqyOJ (10/10) 的测试点，得分 100 分

### Q3 代码实现(demo2, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int mod = 1e+9 + 7;
int n;
unordered_map<string, LL> dic1; // 存储直接赋值变量对应的长度
unordered_map<string, vector<string>> dic2; // 存储间接赋值变量

unordered_map<string, bool> st;    // 标记变量是否是间接赋值变量
unordered_map<string, LL> cache;

LL get_len(string k) // 获取变量名为 k 的长度(可能是间接、直接变量(不含 $))
{
    if (cache.count(k)) return cache[k];

    LL len = 0;
    if (st[k])  // 间接赋值变量
    {
        for (string t : dic2[k])
        {
            if (t[0] != '$') len += t.length();
            else len += get_len(t.substr(1));
            len %= mod;
        }
    }
    else    // 直接赋值变量
    {   
        len = dic1[k];
    }
    cache[k] = len;
    return len;
}

void deal1(string s)    // 直接赋值语句
{
    LL len = 0;
    stringstream ss(s);
    string k, temp;

    ss >> k;    // 读入变量名
    cache.clear();

    while (ss >> temp)  // 读入操作数
    {
        if (temp[0] != '$') len += temp.length();
        else len += get_len(temp.substr(1));
        len %= mod;
    }

    st[k] = false;
    dic1[k] = len;
}

void deal2(string s)    // 间接赋值语句
{
    stringstream ss(s);
    string k, temp;

    ss >> k;
    vector<string> expr;
    while (ss >> temp) expr.push_back(temp);

    st[k] = true;
    dic2[k] = expr;
}

void deal3(string s)
{
    cache.clear();
    LL len = get_len(s);
    printf("%lld\n", len % mod);
}

int main()
{
    scanf("%d", &n);
    getchar();  // Key point

    for (int i = 0; i < n; i ++)
    {
        string order;
        getline(cin, order);

        if (order[0] == '1')
            deal1(order.substr(2));
        else if (order[0] == '2')
            deal2(order.substr(2));
        else 
            deal3(order.substr(2));
    }
    return 0;
}
```

## Q4 集体锻炼

### Q4 算法思路(demo1, subtask1)

- 题目要求求出所有区间 $[l, r]$ 的体育价值之和。最直观的方法就是枚举所有的左右端点 $l$ 和 $r$。如果直接写三个 `for` 循环(外两层枚举 $l$ 和 $r$，最内层再从 $l$ 到 $r$ 重新计算 $\gcd$)，时间复杂度会高达 $O(n^3)$，这在 $n=1000$ 时就会超时。
- 优化点(滚动计算 $\gcd$)：当固定左端点 $l$，让右端点 $r$ 从 $l$ 开始逐步向右扩展时，区间 $[l, r]$ 的 $\gcd$ 其实就等于 区间 $[l, r-1]$ 的 $\gcd$ 与 $a_r$ 的最大公约数。即：$\gcd(a_l, \dots, a_r) = \gcd(\text{pre} \gcd, a_r)$。这样，只需要两层循环，一边向右扩展 $r$，一边维护当前的 $\gcd$，时间复杂度降为 $O(n^2)$。
- 该题解可以通过 smqyOJ (7/40) 的测试点，得分 30 分

### Q4 代码实现(demo1, subtask1)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e+6 + 10, mod = 998244353;

int n;
LL a[N];

LL gcd(LL a, LL b)
{
    return b ? gcd(b, a % b) : a;
}

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++) scanf("%lld", &a[i]);

    LL res = 0;
    for (int l = 1; l <= n; l ++)
    {
        LL gcd_value = a[l];
        for (int r = l; r <= n; r ++)
        {
            gcd_value = gcd(gcd_value, a[r]);
            res += gcd_value * l * r;
            res %= mod;
        }
    }
    printf("%lld\n", res % mod);
}
```

## Q5 收费标准评估

### Q5 算法思路(demo1, subtask1&2)

- `cost[x]` 表示以从节点 `x` 入园游览，游览范围为以 `k` 为根的子树的情况下，总费用的最大值
- 预处理(`dfs`)： 利用深度优先搜索自底向上初始化 `cost` 数组。状态转移方程为：`cost[v] = a[v] + sum(max(0, cost[child]))`。只有当子树的总费用大于 `0` 时，才会将其纳入父节点的游览路线中。
- `deal1(u)` 枚举以 `u` 为根的子树中每一个节点的 `cost` 值即可
- `deal2(u, x)` 自下而上更新、维护 `cost`。在树形 `DP` 中，子节点 `u` 对父节点 `p[u]` 的真实贡献是 `max(0, cost[u])`(因为如果是负数，父节点将不会走到 `u` 这里)。当 `cost[u]` 发生变化时，它对父节点产生的影响(贡献值变化量)应该是：`差值 = 修改后的 max(0, cost[u]) - 修改前的 max(0, preCost)`。只要这个差值为 `0`，说明这次修改对上面没有任何影响，可以安心 `break` 提前结束；如果差值不为 `0`，就把这个差值加给父节点，然后继续向上爬。
- 该题解可以通过 smqyOJ (15/30) 的测试点，得分 30 分

### Q5 代码实现(demo1, subtask1&2)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+5 + 10;
typedef long long LL;

int n, m;
int h[N], e[N * 2], ne[N * 2], idx;
int a[N];

void add(int u, int v)
{
    e[idx] = v, ne[idx] = h[u], h[u] = idx ++;
}

int p[N];
LL cost[N], max_cost = -1e+15;

LL dfs(int v, int fa)
{
    p[v] = fa;
    cost[v] = a[v];

    for (int i = h[v]; ~i; i = ne[i])
    {
        int j = e[i];
        if (j == fa) continue;
        LL branch_cost = dfs(j, v);
        if (branch_cost > 0) cost[v] += branch_cost;
    }

    max_cost = max(max_cost, cost[v]);
    return cost[v];
}

void deal1(int u)
{
    LL max_cost_minitree = cost[u];
    queue<int> nodes;
    nodes.push(u);
    
    while (nodes.size())
    {
        int t = nodes.front();
        nodes.pop();

        for (int i = h[t]; ~i; i = ne[i])
        {
            int j = e[i];
            if (j == p[t]) continue;
            max_cost_minitree = max(max_cost_minitree, cost[j]);
            nodes.push(j);
        }
    }
    printf("%lld\n", max_cost_minitree);
}

void deal2(int u, LL x)
{
    LL delta_cost = x - a[u];
    a[u] = x;

    while (u != 0)
    {
        LL preCost = cost[u];
        cost[u] += delta_cost;

        if (u == 1) break;

        // 节点 u 对其父节点 cnt 值的贡献大小的变化量
        LL delta_contribution = max(0LL, cost[u]) - max(0LL, preCost);
        // 如果贡献没变(截断效应)，停止向上传递
        if (delta_contribution == 0) break;
        
        delta_cost = delta_contribution;
        u = p[u];
    }
    printf("%lld\n", cost[1]);
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++) scanf("%d", &a[i]);
    memset(h, -1, sizeof h);
    for (int i = 0; i < n - 1; i ++)
    {
        int u, v;
        scanf("%d%d", &u, &v);
        add(u, v), add(v, u);
    }
    dfs(1, 0);
    printf("%lld\n", cost[1]);

    for (int i = 0; i < m; i ++)
    {
        int op;
        scanf("%d", &op);
        if (op == 1)
        {
            int u;
            scanf("%d", &u);
            deal1(u);
        }
        else
        {
            int u;
            LL x;
            scanf("%d%lld", &u, &x);
            deal2(u, x);
        }
    }
    return 0;
}
```
