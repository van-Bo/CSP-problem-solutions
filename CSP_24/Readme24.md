> 第24次CCF计算机软件能力认证
> 模拟认证 CSP（100 + 100）
> 模拟认证 AcWing（(10/10), (11/11)）

# Q1 序列查询
## 算法思路
- 逐行读取数值 `x`，每次读取数值后，补充计算间隙值`f[i]`，其中 $preValue < i \leqslant x - 1$；同时补充计算 `f[x]`
- 时间复杂度：$O(n)$
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下得分为 100 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

int n, N;

int main()
{
    scanf("%d%d", &n, &N);
    
    int res = 0;
    int preValue = 0, preIndex = 0;
    for (int i = 1; i <= n; i ++)
    {
        int x;
        scanf("%d", &x);
        res += (x - preValue - 1) * preIndex;   // 补全空隙
        res += 1 * i;   // 计算当前值
        
        preValue = x;
        preIndex = i;
    }
    res += (N - 1 - preValue) * preIndex;
    printf("%d\n", res);
    return 0;
}
```

# Q2 序列查询新解
## 算法思路
- 暴力枚举
- 二分求取 `f(x)`
- 时间复杂度：$O(N \times \log n)$
- 该题解可以通过 AcWing 官网 (6/11) 的数据点，CSP 官网评测系统下得分为 70 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int M = 1e+5 + 10;

int n, N;
int w[M];

bool check(int index, int v)
{
    return w[index] <= v;
}

int get(int v)
{
    int l = 0, r = n;
    while (l < r)
    {
        int mid = (l + r + 1) / 2;
        if (check(mid, v)) l = mid;
        else r = mid - 1;
    }
    return l;
}

int main()
{
    scanf("%d%d", &n, &N);
    for (int i = 1; i <= n; i ++)
        scanf("%d", &w[i]);
    
    int r = N /(n + 1);
    LL res = 0;
    for (int i = 1; i <= N - 1; i ++)
    {
        int f = get(i);
        int g = i / r;
        res += abs(f - g);
    }
    printf("%lld\n", res);
    return 0;
}
```
## 算法思路
- 分类讨论
- 处理操作中每次处理的目标区间的端点值为 `l` 和 `r`，其中 `l` 的值为 `w[i]`，`r` 的值为 `w[i + 1] - 1`
- 在目标区间中 $l \leqslant x \leqslant r$，`f[x]` 的值恒定为 `i`，故确定 `g[x]` 的值即可。`g[x]` 的取值是一个总体呈上升趋势的阶梯函数，因为求解的结果值含有取绝对值，所以此处进行一次分类讨论，其中分类的依据为 `f[x]` 与 `g[x]` 函数的相对位置。
- 第一、二种情况是 `f[x]` 与 `g[x]` 总体有着明确的大小关系，函数图像一上一下
- 第三种情况是 `f[x]` 与 `g[x]` 存在交界点，交界点两侧的大小关系逆转，该种情况可以一分为二，转化成第一、二种情况分别求解
- `get(l, r)` 计算 `g[l] + g[l + 1] + ... + g[r]` 的值，针对于区间 `[l, r]` 可以分为 首、中间、尾三段，其中，中间段的所有区间的长度均为 `R`，此处在同一区间中，每一元素值的 `g` 值是相同的
- 该题解可以通过 AcWing 官网 (11/11) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int N = 1e+5 + 10;

int n, m, R;
int w[N];

LL get(int l, int r)   // 计算 g[l] + g[l + 1] + ... + g[r]
{
    if (l / R == r / R) return (LL)(l / R) * (r - l + 1);
    
    int x = l / R, y = r / R;   // 首尾段的 g[] 值
    int u = x + 1, v = y - 1;   // 中间段的首尾 g[] 值
    
    LL res = 0;
    res += (LL)(u + v) * (v - u + 1) / 2 * R;   // 中间段
    res += (LL)x * (u * R - l);  // 首段
    res += (LL)y * (r - (v * R + R - 1));  // 尾段
    
    return res;
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++)
        scanf("%d", &w[i]);
    w[n + 1] = m;
    R = m / (n + 1);
    
    LL res = 0;
    for (int i = 0; i <= n; i ++)
    {
        // x = w[i], 1, ..., w[i + 1] - 1
        int l = w[i], r = w[i + 1] - 1;
        
        if (l / R >= i || r / R <= i)
        {
            res += abs((LL)i * (r - l + 1) - get(l, r));
        }
        else
        {
            int lrv = i * R - 1, rlv = i * R + R;
            res += abs((LL)i * (lrv - l + 1) - get(l, lrv));   // 左区间
            res += abs((LL)i * (r - rlv + 1) - get(rlv, r));   // 右区间
        }
    }
    printf("%lld\n", res);
    return 0;
}
```

# Q3 登机牌条码
## 算法思路

# Q4 磁盘文件操作
## 算法思路

# Q5 极差路径
## 算法思路
- 通过枚举起始点，以 `dfs` 的方式进行对推荐路径数的维护
- `dfs(st, preEd, ed, mx, mn)` 函数中，`st` 为枚举的起始点（深搜的中心），`ed` 为此时路径中枚举到的最后的节点，`preEd` 为 `ed` 的前一个枚举点（避免回溯），`mx` 为路径中的最大不满意程度，`mn` 为路径中的最小不满意程度
- `dfs()` 过程中对于同一条路径，会在答案集合中重复维护，同一条路径，`i ---> j` 和 `j ---> i`，故最终经过 `dfs()` 后的答案要除以2
- `dfs()` 过程中不会处理起始点相同的路径，故最终答案要额外添加一个节点数 `n`
- 该题解可以通过 AcWing 官网 (5/33) 的数据点，CSP 官网评测系统下的得分为 15 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int N = 5e+5 + 10, M = N * 2;

int n, k1, k2;
int h[N], e[M], ne[M], idx;
LL ans;

void add(int a, int b)
{
    e[idx] = b, ne[idx] = h[a], h[a] = idx ++;
}

void dfs(int st, int preEd, int ed, int mx, int mn)
{
    if (st != ed)
    {
        if (mn >= min(st, ed) - k1 && mx <= max(st, ed) + k2)
            ans ++;
    }
    
    for (int i = h[ed]; ~i; i = ne[i])
    {
        int j = e[i];
        if (j == preEd) continue;
        dfs(st, ed, j, max(j, mx), min(j, mn));
    }
}

int main()
{
    scanf("%d%d%d", &n, &k1, &k2);
    
    memset(h, -1, sizeof h);
    for (int i = 0; i < n - 1; i ++)
    {
        int p, c;
        scanf("%d%d", &p, &c);
        
        add(p, c), add(c, p);
    }
    
    for (int i = 1; i <= n; i ++)
        dfs(i, i, i, i, i);
        
    printf("%lld\n", ans / 2 + n);
    return 0;
}
```
## 算法思路
```C++

```