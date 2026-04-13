# CSP(第41次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + (18/22))

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
