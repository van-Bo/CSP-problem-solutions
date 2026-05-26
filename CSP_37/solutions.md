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
