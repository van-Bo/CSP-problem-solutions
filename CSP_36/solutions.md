# 第36次CCF计算机软件能力认证

> smqyOJ Judge((10/10) + (20/20))

## Q1 移动

### Q1 算法思路

- 模拟即可
- `Hack`: 原先首先想到的是横纵向操作进行抵消，但是发现如果存在触碰边界的情况，本身某些操作便是无效操作，更谈不上抵消操作的资格
- 该题解可以通过 smqyOJ (10/10) 的测试点，得分 100 分

### Q1 代码实现

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> PII;
#define x first
#define y second

int n, k;
unordered_map<char, PII> mp;

void deal(int stx, int sty, string op)
{
    int op_len = op.length();
    for (int i = 0; i < op_len; i ++)
    {
        stx += mp[op[i]].x, sty += mp[op[i]].y;
        stx = max(stx, 1), stx = min(stx, n);
        sty = max(sty, 1), sty = min(sty, n);
    }
    printf("%d %d\n", stx, sty);
}

int main()
{
    mp['f'] = {0, 1};
    mp['b'] = {0, -1};
    mp['l'] = {-1, 0};
    mp['r'] = {1, 0};

    cin >> n >> k;
    for (int i = 0; i < k; i ++)
    {
        int stx, sty;
        string op;
        cin >> stx >> sty >> op;
        deal(stx, sty, op);
    }
    return 0;
}
```

## Q2 梦境巡查

### Q2 算法思路(demo1, TLE)

- 使用二分枚举求解最少的初始能量
- 时间复杂度为: $O(n^2 \log V)$
- 该题解可以通过 smqyOJ (16/20) 的测试点，得分 80 分

### Q2 代码实现(demo1, TLE)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+5 + 10;
int a[N], b[N];
int n;

bool check(int v)
{
    for (int i = 0; i <= n; i ++)
    {
        v += b[i];
        // Key: 判断当前拥有的能量是否足以支付前往下一区域的消耗
        if (v < a[i]) return false;
        v -= a[i];
    }
    return true;
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i <= n; i ++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i ++) scanf("%d", &b[i]);

    for (int breakpoint = 1; breakpoint <= n; breakpoint ++)
    {
        int l = a[0], r = 1e8 + 1000;
        int backup = b[breakpoint];
        b[breakpoint] = 0;
        while (l < r)
        {
            int mid = (l + r) / 2;
            if (check(mid)) r = mid;
            else l = mid + 1;
        }
        b[breakpoint] = backup;
        printf("%d ", l);
    }
    return 0;
}
```

### Q2 算法思路(demo2, AC)

- 假设顿顿初始能量为 $0$，且没有任何区域发生意外。走到第 $k$ 步准备前往下一区域时，净能量变化为 $S_k$：

$$S_k = \sum_{j=1}^k b_j - \sum_{j=0}^k a_j$$

- 为了不因为能量耗尽而倒下，自带的初始能量 $w$ 必须填补这个亏空：

$$w + S_k \ge 0 \implies w \ge -S_k$$

- 现在，假设第 $i$ 个区域发生意外($b_i = 0$)，那么对初始能量的要求会发生一定的变化
  - 对于 $k < i$ 的步骤： 还没走到意外区域，一切照常。此时要求 $w \ge -S_k$。
  - 对于 $k \ge i$ 的步骤： 由于少拿了 $b_i$ 的补给，所以后续所有的净能量都会减少 $b_i$。此时要求 $w + S_k - b_i \ge 0 \implies w \ge -S_k + b_i$。
- 综合来看，如果第 $i$ 个区域发生意外，顿顿需要的最小初始能量 $w_i$ 就是所有亏空里的最大值：

$$w_i = \max\left( \max_{0 \le k < i}(-S_k), b_i + \max_{i \le k \le n}(-S_k) \right)$$

- $O(N)$ 终极优化：前缀最大值与后缀最大值
- 会将一个 $O(N)$ 的模拟过程变成了静态数组的查询！只需要预处理出两个数组：
  - 前缀最大值数组 `pre_max[i]`：表示从 $0$ 到 $i$ 之间，$-S_k$ 的最大值。
  - 后缀最大值数组 `suf_max[i]`：表示从 $i$ 到 $n$ 之间，$-S_k$ 的最大值。
- 该题解可以通过 smqyOJ (20/20) 的测试点，得分 100 分
  
### Q2 代码实现(demo2, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+5 + 10;
int a[N], b[N], s[N];
int sa[N], sb[N];
int n;
int pre_max[N], suf_max[N];

int main()
{
    scanf("%d", &n);
    for (int i = 0; i <= n; i ++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i ++) scanf("%d", &b[i]);
    
    sa[0] = a[0];
    for (int i = 1; i <= n; i ++) sa[i] = sa[i - 1] + a[i];
    sb[0] = b[0];
    for (int i = 1; i <= n; i ++) sb[i] = sb[i - 1] + b[i];
    // 净能量变化
    s[0] = -a[0];
    for (int i = 1; i <= n; i ++) s[i] = sb[i] - sa[i];

    // 预处理 pre_max 和 suf_max
    pre_max[0] = -s[0];
    for (int i = 1; i <= n; i ++) pre_max[i] = max(pre_max[i - 1], -s[i]);
    suf_max[n] = -s[n];
    for (int i = n - 1; i >= 0; i --) suf_max[i] = max(suf_max[i + 1], -s[i]); 

    for (int breakpoint = 1; breakpoint <= n; breakpoint ++)
    {
        int res = max(pre_max[breakpoint - 1], b[breakpoint] + suf_max[breakpoint]);
        printf("%d ", res);
    }
    return 0;
}
```
