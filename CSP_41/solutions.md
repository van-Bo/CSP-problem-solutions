# CSP(第41次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + (22/22))

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