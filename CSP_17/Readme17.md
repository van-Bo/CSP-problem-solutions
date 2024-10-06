> 第17次CCF计算机软件能力认证
> 模拟认证 CSP（100 + ）
> 模拟认证 AcWing（(13/13), ）

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

# Q3 字符画
## 算法思路

# Q4 推荐系统
## 算法思路

# Q5 城市规划
## 算法思路