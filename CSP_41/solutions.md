# CSP(第41次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + (22/22) + (23/23) + (13/25))

## Q1 平衡数

### Q1 算法思路

- 模拟即可，`solve(x)` 统计 x 的二进制表示中0和1的个数，判断是否相等
- 该题解可以通过 smqyOJ (10/10)的测试点

### Q1 代码实现

```C++
#include <bits/stdc++.h>
using namespace std;
int n, ans;

bool solve(int x)
{
    int n0 = 0, n1 = 0;
    while (x)
    {
        if (x & 1) n1 ++;
        else n0 ++;
        x >>= 1;
    }
    if (n0 == n1) return true;
    else return false;
}

int main()
{
    cin >> n;
    for (int i = 0; i < n; i ++)
    {
        int x;
        cin >> x;
        bool flag = solve(x);
        if (flag) ans ++;
    }
    cout << ans << endl;
    return 0;
}
```

## Q2 机器人项目管理

### Q2 算法思路(demo1, WA)

- 贪心处理，`cost` 统计每个任务的单位咖啡的减时效果，降序处理后，优先处理单位咖啡减时效果大的任务
- 处理任务时，考虑两种情况：灵活型任务和普通型任务
- 该题解可以通过 smqyOJ (18/22)的测试点，得分 82 分

### Q2 代码实现(demo1, WA)

```C++
#include <bits/stdc++.h>
using namespace std;
const int N = 210;
typedef pair<int, int> PII;

double n, m;
struct Task
{
    int o;
    double t, a, b;
}task[N];
struct Cost
{
    int idx;
    double c;
    bool operator < (const struct Cost e)
    {
        return c > e.c;
    }    
}cost[N];
bool st[N]; // st[i] 标记 是否是灵活性任务

int main()
{
    cin >> n >> m;
    double time = 0;
    for (int i = 0; i < n; i ++)
    {
        cin >> task[i].o >> task[i].t >> task[i].a >> task[i].b;
        time += task[i].t;
        cost[i].idx = i;
        cost[i].c = task[i].b / task[i].a;
        if (task[i].o == 0) st[i] = true;
        else st[i] = false;
    }
    sort(cost, cost + (int)n);

    for (int i = 0; i < n; i ++)
    {
        if (m <= 0) break;

        int index = cost[i].idx;
        if (st[index])  // 灵活型
        {
            double v = min(m, task[index].a);
            m -= v;
            time -= v * cost[i].c;
        }
        else    // 普通型
        {
            if (m >= task[index].a)
            {
                m -= task[index].a;
                time -= task[index].b;
            }
        }
    }
    printf("%.6lf\n", time);
    return 0;
}
```

### Q2 算法思路(demo2, AC)

- 灵活型任务是允许分割的任务，对应经典的**部分背包问题**，采用贪心解法（按性价比降序排列优先选择）即可
- 普通型任务是不可分割的任务，对应经典的**0-1背包问题**，采用动态规划解法
- 该题解可以通过 smqyOJ (22/22)的测试点，得分 100 分

前述的 demo1 代码中，贪心处理了所有任务，导致部分测试点无法通过。反例如下所示：
假设你现在的咖啡预算是 $m = 10$。
有 3 个普通型任务：
    任务 A：耗咖啡 $a = 6$，减时 $b = 12$。性价比：$12 / 6 = 2.0$
    任务 B：耗咖啡 $a = 5$，减时 $b = 9$。性价比：$9 / 5 = 1.8$
    任务 C：耗咖啡 $a = 5$，减时 $b = 9$。性价比：$9 / 5 = 1.8$
按照贪心逻辑：优先把性价比最高的任务 A 拿走，消耗 6 杯咖啡，减时 12。剩余咖啡 $10 - 6 = 4$ 杯。剩下的 4 杯咖啡不够执行任务 B 或 C（因为是普通型，必须 $\ge 5$ 杯才能起效）。贪心最终结果：总减时 = 12。
而实际上的最优解：放弃任务 A，直接选择任务 B 和任务 C。消耗 $5 + 5 = 10$ 杯咖啡，刚好用完。最优解结果：总减时 = $9 + 9 = 18$。

### Q2 代码实现(demo2, AC)

```C++
#include <bits/stdc++.h>
using namespace std;
const int N = 210;

int n, m;
struct Task
{
    double t, a, b;
};
struct Task task0[N];
struct Task task1[N];
struct Cost
{
    int idx;
    double c;
    bool operator < (const struct Cost &e)
    {
        return c > e.c;
    }    
}cost[N];
int dp[1010];

int main()
{
    cin >> n >> m;
    double time = 0, ans = 0;
    int num0 = 0, num1 = 0;
    for (int i = 0; i < n; i ++)    // 按照任务种类分别存储任务信息
    {
        int o;
        double t, a, b;
        cin >> o >> t >> a >> b;
        time += t;
        if (o == 0)
        {
            task0[num0] = {t, a, b};
            cost[num0] = {num0, b / a};
            num0 ++;
        }
        else task1[num1 ++] = {t, a, b};
    }
    ans = time;
    sort(cost, cost + num0);    // 灵活型任务按照性价比降序排序

    for (int i = 0; i < num1; i ++) // 普通型任务采用 0-1 背包的动态规划解法，dp[j] 表示分配 j 杯咖啡能获得的最大减时效果
    {
        for (int j = m; j >= task1[i].a; j --)
            dp[j] = max(dp[j], dp[j - (int)task1[i].a] + (int)task1[i].b);
    }

    for (int i = 0; i <= m; i ++)   // 普通型任务分配 i 杯咖啡
    {
        double task1_save = dp[i];
        double task0_save = 0;
        int k = m - i;
        for (int j = 0; j < num0; j ++) // 灵活型任务分配剩余的 m - i 杯咖啡
        {
            if (k <= 0) break;
            int v = min(k, (int)task0[cost[j].idx].a);
            k -= v;
            task0_save += v * cost[j].c;
        }
        ans = min(ans, time - task0_save - task1_save);
    }
    printf("%.6lf\n", ans);
    return 0;
}
```

## Q3 进程通信

### Q3 算法思路(demo1, subtask-40%)

- 针对前 40% 不存在 delete 操作的测试点，模拟即可
- 使用 `freePos` 维护当前的空闲段，标记是否占用和是否存储对象的数组 `e` 和 `ele` 似乎没有必要，可以直接通过 `freePos` 来维护空闲段信息
- 使用 `pro[p].chain` 维护进程 p 的链表信息
- `op_new` 是 first fit 分配方式，而不是题目所要求的 best fit 分配方式，因为不存在 delete 操作，所以 first fit 和 best fit 的结果是一样的
- 该题解可以通过 smqyOJ (8/23)的测试点，得分 40 分

### Q3 代码实现(demo1, subtask-40%)

```C++
#include <bits/stdc++.h>
using namespace std;
const int N = 8e+5 + 10;
typedef pair<int, int> PII;
#define x first
#define y second

int n, q;
bool e[N];  // 是否占用
bool ele[N];  // 是否存储对象
vector<PII> freePos;
int numFreePos;
struct Node
{
    int l, r, pos = -1; // pos 为当前已经存储的位置
};
struct Process
{
    int numNode = 0;
    vector<Node> chain;
}pro[110];

void op_new(int p, int l)
{
    for (int i = 0; i < numFreePos; i ++)   // 寻找空闲段
    {
        int start = freePos[i].x, end = freePos[i].y;
        if (end - start + 1 >= l)
        {
            // 维护进程接口
            Node temp = {start, start + l - 1, -1};
            pro[p].chain.push_back(temp);
            pro[p].numNode ++;

            // 维护空闲段
            if (end - start + 1 > l)
                freePos[i] = {start + l, end};
            else
                freePos.erase(freePos.begin() + i);

            printf("%d\n", start);
            break;
        }
    }
}

void op_send(int p)
{
    int ans = 0;
    for (int i = 0; i < pro[p].numNode; i ++)
    {
        auto &t = pro[p].chain[i];
        int insertPos = t.pos;
        if (insertPos == -1 || insertPos == t.r) t.pos = t.l;
        else t.pos = insertPos + 1;
        ans += t.pos;
    }
    printf("%d\n", ans);
}

void op_delete(int p, int k)
{

}

int main()
{
    cin >> n >> q;
    freePos.push_back({0, 0x3f3f3f3f});
    numFreePos = 1;
    for (int i = 0; i < q; i ++)
    {
        string op;
        cin >> op;
        int p, l;
        if (op == "new")
        {
            cin >> p >> l;
            op_new(p, l);
        }
        else if (op == "delete")
        {
            cin >> p >> l;
            op_delete(p, l);
        }
        else
        {
            cin >> p;
            op_send(p);
        }
    }
    return 0;
}
```

### Q3 算法思路(demo2, subtask-80%)

- 添加 delete 操作的处理，维护空闲段时需要考虑合并相邻的空闲段，合理利用 `vector insert/erase` 库函数
- `op_new` 需要按照 best fit 的方式分配内存段，遍历 `freePos` 寻找最适合的空闲段
- 该题解可以通过 smqyOJ (21/23)的测试点，得分 80 分

### Q3 代码实现(demo2, subtask-80%)

```C++
#include <bits/stdc++.h>
using namespace std;
const int N = 8e+5 + 10;
typedef pair<int, int> PII;
#define x first
#define y second

int n, q;
bool e[N];  // 是否占用
bool ele[N];  // 是否存储对象
vector<PII> freePos;
int numFreePos;
struct Node
{
    int l, r, pos = -1; // pos 为当前已经存储的位置
};
struct Process
{
    int numNode = 0;
    vector<Node> chain;
}pro[110];

void op_new(int p, int l)
{
    int flag = -1, min_len = 0;
    for (int i = 0; i < numFreePos; i ++)   // 寻找最佳空闲段
    {
        int start = freePos[i].x, end = freePos[i].y;
        int length = end - start + 1;
        if (flag == -1 && length >= l)
            flag = i, min_len = length;
        if (length >= l && length < min_len)
            flag = i, min_len = length;
    }

    int start = freePos[flag].x, end = freePos[flag].y;
    Node temp = {start, start + l - 1, -1};
    pro[p].chain.push_back(temp);
    pro[p].numNode ++;

    if (end - start + 1 > l)
        freePos[flag] = {start + l, end};
    else
        freePos.erase(freePos.begin() + flag), numFreePos --;

    printf("%d\n", start);
}

void op_send(int p)
{
    int ans = 0;
    for (int i = 0; i < pro[p].numNode; i ++)
    {
        auto &t = pro[p].chain[i];
        int insertPos = t.pos;
        if (insertPos == -1 || insertPos == t.r) t.pos = t.l;
        else t.pos = insertPos + 1;
        ans += t.pos;
    }
    printf("%d\n", ans);
}

void op_delete(int p, int k)
{
    auto &t = pro[p].chain[k - 1];
    int start = t.l, end = t.r;
    // 删除进程接口
    pro[p].chain.erase(pro[p].chain.begin() + k - 1);
    pro[p].numNode --;
    // 维护空闲段
        // 找到插入位置
    int loc = 0;
    while (loc < numFreePos && freePos[loc].x < start) loc ++;
        // 插入新段
    freePos.insert(freePos.begin() + loc, {start, end});
    numFreePos ++;
        // 合并左边
    if (loc > 0)
    {
        auto &prev = freePos[loc - 1];
        auto &curr = freePos[loc];
        if (prev.y == curr.x - 1)
        {
            prev.y = curr.y;
            freePos.erase(freePos.begin() + loc);
            loc --;   // 合并后位置左移
            numFreePos --;
        }
    }
        // 合并右边
    if (loc <= numFreePos - 2)
    {
        auto &curr = freePos[loc];
        auto &next = freePos[loc + 1];
        if (curr.y == next.x - 1)
        {
            curr.y = next.y;
            freePos.erase(freePos.begin() + loc + 1);
            numFreePos --;
        }
    }
}

int main()
{
    cin >> n >> q;
    freePos.push_back({0, 0x3f3f3f3f});
    numFreePos = 1;
    for (int i = 0; i < q; i ++)
    {
        string op;
        cin >> op;
        int p, l;
        if (op == "new")
        {
            cin >> p >> l;
            op_new(p, l);
        }
        else if (op == "delete")
        {
            cin >> p >> l;
            op_delete(p, l);
        }
        else
        {
            cin >> p;
            op_send(p);
        }
    }
    return 0;
}
```

### Q3 算法思路(demo3, AC)

- 最大操作数 $q \leqslant 8000$，每次最大申请长度 $L_m \leqslant 5 \times 10^5$，最极端情况下，全部都是new 操作，总共需要消耗内存单元数约 $8000 \times 5 \times 10^5 = 4 \times 10^9$。
- 使用 `long long` 来存储内存地址
- 该题解可以通过 smqyOJ (23/23)的测试点，得分 100 分

- `int` 的范围是 $[-2^{31}, 2^{31} - 1]$，约为 $[-2.15 \times 10^9, 2.15 \times 10^9]$，不足以存储 $4 \times 10^9$ 的内存地址，可能会发生溢出。
- `long long` 的范围是 $[-2^{63}, 2^{63} - 1]$，约为 $[-9.22 \times 10^{18}, 9.22 \times 10^{18}]$，远大于 $4 \times 10^9$，因此使用 `long long` 来存储内存地址是安全的，不会发生溢出。

### Q3 代码实现(demo3, AC)

```C++
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef pair<LL, LL> PII;
#define x first
#define y second

LL n, q;
vector<PII> freePos;
int numFreePos;
struct Node
{
    LL l, r, pos = -1; // pos 为当前已经存储的位置
};
struct Process
{
    int numNode = 0;
    vector<Node> chain;
}pro[110];

void op_new(int p, int l)
{
    int flag = -1, min_len = 0;
    for (int i = 0; i < numFreePos; i ++)   // 寻找最佳空闲段
    {
        LL start = freePos[i].x, end = freePos[i].y;
        LL length = end - start + 1;
        if (flag == -1 && length >= l)
            flag = i, min_len = length;
        if (length >= l && length < min_len)
            flag = i, min_len = length;
    }

    LL start = freePos[flag].x, end = freePos[flag].y;
    Node temp = {start, start + l - 1, -1};
    pro[p].chain.push_back(temp);
    pro[p].numNode ++;

    if (end - start + 1 > l)
        freePos[flag] = {start + l, end};
    else
        freePos.erase(freePos.begin() + flag), numFreePos --;

    printf("%lld\n", start);
}

void op_send(int p)
{
    LL ans = 0;
    for (int i = 0; i < pro[p].numNode; i ++)
    {
        auto &t = pro[p].chain[i];
        LL insertPos = t.pos;
        if (insertPos == -1 || insertPos == t.r) t.pos = t.l;
        else t.pos = insertPos + 1;
        ans += t.pos;
    }
    printf("%lld\n", ans);
}

void op_delete(int p, int k)
{
    auto &t = pro[p].chain[k - 1];
    LL start = t.l, end = t.r;
    // 删除进程接口
    pro[p].chain.erase(pro[p].chain.begin() + k - 1);
    pro[p].numNode --;
    // 维护空闲段
        // 找到插入位置
    int loc = 0;
    while (loc < numFreePos && freePos[loc].x < start) loc ++;
        // 插入新段
    freePos.insert(freePos.begin() + loc, {start, end});
    numFreePos ++;
        // 合并左边
    if (loc > 0)
    {
        auto &prev = freePos[loc - 1];
        auto &curr = freePos[loc];
        if (prev.y == curr.x - 1)
        {
            prev.y = curr.y;
            freePos.erase(freePos.begin() + loc);
            loc --;   // 合并后位置左移
            numFreePos --;
        }
    }
        // 合并右边
    if (loc <= numFreePos - 2)
    {
        auto &curr = freePos[loc];
        auto &next = freePos[loc + 1];
        if (curr.y == next.x - 1)
        {
            curr.y = next.y;
            freePos.erase(freePos.begin() + loc + 1);
            numFreePos --;
        }
    }
}

int main()
{
    cin >> n >> q;
    freePos.push_back({0, 2e18});
    numFreePos = 1;
    for (int i = 0; i < q; i ++)
    {
        string op;
        cin >> op;
        int p, l;
        if (op == "new")
        {
            cin >> p >> l;
            op_new(p, l);
        }
        else if (op == "delete")
        {
            cin >> p >> l;
            op_delete(p, l);
        }
        else
        {
            cin >> p;
            op_send(p);
        }
    }
    return 0;
}
```

## Q4 异或

### Q4 算法思路(demo1, TLE)

- 模拟处理，`work(u, v)` 计算 `u` 和 `v` 的 `k` 进制表示对应位的和 `mod k` 的结果
- `deal1(l, r, v)` 对区间 `[l, r]` 的每个数执行 `work(a[i], v)` 的操作
- `deal2(l, r)` 计算区间 `[l, r]` 的每个数的阶乘的异或结果，阶乘的计算也需要使用 `work` 函数来处理
- 该题解可以通过 smqyOJ (1/25)的测试点，得分 0 分

### Q4 代码实现(demo1, TLE)

```C++
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 5e+5 + 10;

int n, m, k;
LL a[N];

LL work(LL u, LL v)
{
    LL res = 0;
    LL power = 1;
    while (u || v)
    {
        LL du = u % k, dv = v % k;
        LL sumBit = (du + dv) % k;
        res += sumBit * power;

        power = power * k;
        u /= k, v /= k;
    }
    return res;
}

void deal1(int l, int r, LL v)
{
    for (int i = l; i <= r; i ++)
        a[i] = work(a[i], v);
}

LL deal2(int l, int r)
{
    LL res = 0;
    for (int i = l; i <= r; i ++)
    {
        LL fai = 0;
        for (int j = 1; j <= a[i]; j ++)
            fai = work(fai, j);
        res = work(res, fai);
    }
    return res;
}

int main()
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; i ++) scanf("%lld", &a[i]);
    for (int i = 0; i < m; i ++)
    {
        int op;
        scanf("%d", &op);
        if (op == 1)
        {
            int l, r;
            LL v;
            scanf("%d%d%lld", &l, &r, &v);
            deal1(l, r, v);
        }
        else
        {
            int l, r;
            scanf("%d%d", &l, &r);
            LL res = deal2(l, r);
            printf("%lld\n", res);
        }
    }
}
```

### Q4 算法思路(demo2, subtask1&2)

- $f(n) = 0 \oplus_k 1 \oplus_k 2 \oplus_k \dots \oplus_k n$
- 优化 `f(n)` 函数的求解(把 $O(n)$ 的循环优化成 $O(\log_k n)$)，优化思路借鉴 $k$ 进制列加法的模拟过程
- 假设在 $k = 3$ 的情况下，列竖式计算 `f(n)` 的过程如下所示：

|value|pos 0|pos 1|pos 2|pos 3| ...
|-----|-----|-----|-----|-----|-----
|  0  |  0  |  0  |  0  |  0  | ...
|  1  |  1  |  0  |  0  |  0  | ...
|  2  |  2  |  0  |  0  |  0  | ...
|  3  |  0  |  1  |  0  |  0  | ...
|  4  |  1  |  1  |  0  |  0  | ...
|  5  |  2  |  1  |  0  |  0  | ...
|  6  |  0  |  2  |  0  |  0  | ...    
|  7  |  1  |  2  |  0  |  0  | ...
|  8  |  2  |  2  |  0  |  0  | ...
|  9  |  0  |  0  |  1  |  0  | ...
| 10  |  1  |  0  |  1  |  0  | ...
| 11  |  2  |  0  |  1  |  0  | ...
| 12  |  0  |  1  |  1  |  0  | ...
| 13  |  1  |  1  |  1  |  0  | ...
| 14  |  2  |  1  |  1  |  0  | ...
| 15  |  0  |  2  |  1  |  0  | ...

- 若计算 `f(15)`，则进行如上的列式，从 `pos 0` 开始，纵向求和得到 `sumBit`，并将 `sumBit % k` 作为 `f(15)` 在 `pos 0` 的值，依此类推，计算其他位的值
- 可以发现，从上至下，`pos 0` 的值的变化周期为 $k$，从 `0` 到 `k-1` 循环往复
- `pos j` 的值的变动期限(该期限内位值不变)为 $k^j$，例如，`3` 进制下，`pos 1` 的值在 $[0, 2]$ 内为 `0`，在 $[3, 5]$ 内为 `1`，在 $[6, 8]$ 内为 `2`，在 $[9, 11]$ 内为 `0`，在 $[12, 14]$ 内为 `1`，在 $[15, 17]$ 内为 `2`，以此类推
- 假定 $n$ 的 $k$ 进制表示为 $d_m d_{m-1} \dots d_1 d_0$
- 考虑 `pos 0` 处和值的计算：
  - $0 + 1 + 2 + \dots + (k-1) = \frac{(k-1)k}{2}$，题目中 **$k$ 为奇数**，所以 $\frac{(k-1)k}{2}$ 是 $k$ 的倍数，该完整往复区间的和值 $\mod k$ 的结果为 `0`
  - 当 $n$ 不在完整往复区间的末尾时，剩余部分的和值 $\mod k$ 的结果为 $(0 + 1 + 2 + \dots + d_0) \mod k = \frac{d_0 \cdot (d_0 + 1)}{2} \mod k$
- 考虑 `pos j` 处和值的计算：
  - 每 $k^j$ 个数为一个值的变动期限，这 $k^j$ 个数的和值一定是 $k$ 的倍数，所以每个值的变动期限内的和值 $\mod k$ 的结果为 `0`
  - 当 $n$ 不在完整往复区间的末尾时，剩余部分的和值 $\mod k$ 的结果为 $d_j \cdot (n \mod k^j + 1) \mod k$，其中 $d_j$ 是 $n$ 在 `pos j` 处的值，$(n \mod k^j + 1)$ 在模 $k$ 的意义下等效为 $d_0 + 1$，其中 $d_0$ 是 $n$ 在 `pos 0` 处的值
- 该题解可以通过 smqyOJ (13/25)的测试点，得分 50 分

### Q4 代码实现(demo2, subtask1&2)

```C++
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 5e+5 + 10;

int n, m, k;
LL a[N];

LL work(LL u, LL v)
{
    LL res = 0;
    LL power = 1;
    while (u || v)
    {
        LL du = u % k, dv = v % k;
        LL sumBit = (du + dv) % k;
        res += sumBit * power;

        power = power * k;
        u /= k, v /= k;
    }
    return res;
}

void deal1(int l, int r, LL v)
{
    for (int i = l; i <= r; i ++)
        a[i] = work(a[i], v);
}

LL f(LL x)
{
    LL res = 0;
    LL power = 1;

    // pos 0
    LL lowBit = x % k;
    LL d0 = lowBit;
    res += (lowBit + 1) * lowBit / 2 % k * power;
    x /= k;
    power *= k;

    // pos j
    while (x)
    {
        lowBit = x % k;
        res += lowBit * (d0 + 1) % k * power;
        x /= k;
        power *= k;
    }
    return res;
}

LL deal2(int l, int r)
{
    LL res = 0;
    for (int i = l; i <= r; i ++)
    {
        LL fai = f(a[i]);
        res = work(res, fai);
    }
    return res;
}

int main()
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; i ++) scanf("%lld", &a[i]);
    for (int i = 0; i < m; i ++)
    {
        int op;
        scanf("%d", &op);
        if (op == 1)
        {
            int l, r;
            LL v;
            scanf("%d%d%lld", &l, &r, &v);
            deal1(l, r, v);
        }
        else
        {
            int l, r;
            scanf("%d%d", &l, &r);
            LL res = deal2(l, r);
            printf("%lld\n", res);
        }
    }
}
```
