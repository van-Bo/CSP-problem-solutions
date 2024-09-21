> 第25次CCF计算机软件能力认证
> 模拟认证 CSP（100 + 100）
> 模拟认证 AcWing（(10/10), (10/10)）

# Q1 未初始化警告
## 算法思路
- 使用 `unordered_set` 来存储已经可以作为赋值公式右值的变量或常量的下标
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;

int n, k;
unordered_set<int> validIndex;

int main()
{
    scanf("%d%d", &n, &k);
    
    int res = 0;
    validIndex.insert(0);
    for (int i = 0; i < k; i ++)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        if (validIndex.find(y) == validIndex.end())
            res ++;
            
        validIndex.insert(x);
    }
    printf("%d\n", res);
    return 0;
}
```

# Q2 出行计划
## 算法思路
- 反向考虑、对于每一次出行计划，根据 `t` 和 `c` 值，可以反推出 哪些时刻进行核算检测可以完成此次的出行计划
- 差分、前缀和
- 若求出的区间值的上界为负值，要抛弃；若不抛弃，原先的区间会变成 `[1, N - 1]`，导致答案错误
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
const int N = 2e+5 + 10;

int n, m, k;
int cnt[N];

int main()
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 0; i < n; i ++)
    {
        int t, c;
        scanf("%d%d", &t, &c);
        
        if (t - k <= 0) continue;
        
        int l = max(1, t - k - c + 1);
        int r = min(t - k, N - 1);
        cnt[l] ++, cnt[r + 1] --;
    }
    
    for (int i = 1; i < N; i ++) 
        cnt[i] += cnt[i - 1];
        
    for (int i = 0; i < m; i ++)
    {
        int x;
        scanf("%d", &x);
        printf("%d\n", cnt[x]);
    }
    return 0;
}
```