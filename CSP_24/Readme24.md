> 第24次CCF计算机软件能力认证
> 模拟认证 CSP（100 + ）
> 模拟认证 AcWing（(10/10), ）

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

# 序列查询新解
## 算法思路
```C++

```

# 登机牌条码
## 算法思路

# 磁盘文件操作
## 算法思路

# 极差路径
## 算法思路