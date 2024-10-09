> 第17次CCF计算机软件能力认证
> 模拟认证 CSP（100 + 100 + ）
> 模拟认证 AcWing（(13/13), (13/13), ）

# Q1 小明种苹果
## 算法思路
- 模拟即可
- 该题解可以通过 AcWing 官网 (13/13) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
const int N = 1010;

int n, m;
int cnt[N], cut[N];

int main()
{
    scanf("%d%d", &n, &m);
    
    int sumApple = 0;
    for (int i = 1; i <= n; i ++)
    {
        for (int j = 0; j <= m; j ++)
        {
            int x;
            scanf("%d", &x);
            
            if (j == 0)
            {
                cnt[i] = x;
                sumApple += x;
                continue;
            }
            
            cut[i] -= x;
        }
    }
    
    int sumCut = 0, maxCutSingle = -1;
    int mark = -1;
    for (int i = n; i >= 1; i --)
    {
        sumCut += cut[i];
        if(cut[i] >= maxCutSingle)
        {
            mark = i;
            maxCutSingle = cut[i];
        }
    }
    
    printf("%d %d %d\n", sumApple - sumCut, mark, maxCutSingle);
    return 0;
}
```

# Q2 小明种苹果（续）
## 算法思路
- 理清题意很重要，苹果减少的操作有两种：一种是自然掉落，一种是人为的疏果操作
- `cntApple[x]` 统计第 `x` 棵苹果树上的苹果树
- `st[x]` 表示第 `x` 棵苹果树是否发生自然掉落
- 该题解可以通过 AcWing 官网 (13/13) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
const int N = 1010;

int n, m;
int cntApple[N];
bool st[N * 2];

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++)   // 针对第 i 棵苹果树操作
    {
        scanf("%d%d", &m, &cntApple[i]);
        for (int j = 2; j <= m; j ++)
        {
            int x; 
            scanf("%d", &x);
            
            if (x > 0 && x < cntApple[i])
            {
                cntApple[i] = x;
                st[i] = true, st[i + n] = true;
            }
            else if (x < 0)
                cntApple[i] += x;
        }
    }
    
    int T = 0, D = 0, E = 0;
    for (int i = 1; i <= n; i ++)
        T += cntApple[i];
        
    for (int i = 1; i <= n; i ++)
        if (st[i])
            D += 1;
            
    for (int i = 2; i <= n + 1; i ++)
        if (st[i - 1] && st[i] && st[i + 1])
            E += 1;
            
    printf("%d %d %d\n", T, D, E);
    return 0;
}
```

# Q3 字符画
## 算法思路

# Q4 推荐系统
## 算法思路

# Q5 城市规划
## 算法思路