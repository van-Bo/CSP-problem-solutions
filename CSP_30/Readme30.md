> 第30次CCF计算机软件能力认证
> 模拟认证 CSP（100 + 100 + ）
> 模拟认证 AcWing（(11/11), (10/10)）

# Q1 重复局面
## 算法思路
- 使用哈希表 `unordered_map<string, int>` 来维护局面出现的次数
- 该题解可以通过 AcWing 官网评测系统下 (11/11) 的数据点，CSP 官网评测系统下得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

using namespace std;
const int N = 110;

int n;
unordered_map<string, int> pr;

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++)
    {
        string state, subState;
        for (int j = 0; j < 8; j ++)
        {
            cin >> subState;
            state += subState;
        }
        
        pr[state] ++;
        printf("%d\n", pr[state]);
    }
    return 0;
}
```

# Q2 矩阵运算
## 算法思路
- 按照给定的矩阵计算公式模拟
- 时间复杂度：$O(m \times n^2)$，CSP 官网的评测没有卡时间复杂度
- 该题解可以通过 AcWing 评测系统下 (5/10) 的数据点，CSP 官网评测系统下得分为 70 分
## HINT
- 在此题解中，若储存结果矩阵元素值的数据类型为 `int`，而不是 `long long`，会出现错解的情况，得分为 70 分
- 在此题解中，若使用 `long long` 作为结果矩阵元素的数据类型，对于数组空间为 N x N 的矩阵会出现超出内存限制的情况，得分依然为 70 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;
typedef long long LL;
const int N = 1e+4 + 10, M = 25;

int n, m;   // 矩阵 n 行 m 列
int q[N][M], k[N][M], kt[M][N], v[N][M];
int w[N];

LL res[N][N];

void getMatrix(int MatrixType)
{
    if (MatrixType == 0)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%d", &q[i][j]);
    }
    else if (MatrixType == 1)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%d", &k[i][j]);
    }
    else if (MatrixType == 2)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%d", &v[i][j]);
    }
}

int main()
{
    scanf("%d%d", &n, &m);
    
    // get Matrix q k v w
    for (int i = 0; i < 3; i ++) getMatrix(i);
    for (int i = 0; i < n; i ++) scanf("%d", &w[i]);
    
    // make Matrix kt
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < m; j ++)
            kt[j][i] = k[i][j];
    
    // q x kt
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < n; j ++)
        {
            LL mark = 0;
            for (int k = 0; k < m; k ++)
                mark += (LL)q[i][k] * kt[k][j];
            res[i][j] = mark;
        }
    
    // w · (q x kt)
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < n; j ++)
            res[i][j] *= w[i];
    
    // w · (q x kt) x v
    LL ans[N][M];
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < m; j ++)
        {
            LL mark = 0;
            for (int k = 0; k < n; k ++)
                mark += res[i][k] * v[k][j];
            ans[i][j] = mark;
        }
        
    // print answer
    for (int i = 0; i < n; i ++)
    {
        for (int j = 0; j < m; j ++)
            printf("%lld ", ans[i][j]);
        puts("");
    }
    return 0;
}
```
## 算法思路
- 空间优化，针对 `long long res[N][N]`，在处理过程中分步操作（逐行进行）。此外，该题解并没有进行时间优化
- 该题解可以通过 AcWing 评测系统下 (5/10) 的数据点（报错提示为 TLE），CSP 官网评测系统下得分为 100 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;
typedef long long LL;
const int N = 1e+4 + 10, M = 25;

int n, m;   // 矩阵 n 行 m 列
int q[N][M], k[N][M], kt[M][N], v[N][M];
int w[N];

LL res[N];

void getMatrix(int MatrixType)
{
    if (MatrixType == 0)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%d", &q[i][j]);
    }
    else if (MatrixType == 1)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%d", &k[i][j]);
    }
    else if (MatrixType == 2)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%d", &v[i][j]);
    }
}

int main()
{
    scanf("%d%d", &n, &m);
    
    // get Matrix q k v w
    for (int i = 0; i < 3; i ++) getMatrix(i);
    for (int i = 0; i < n; i ++) scanf("%d", &w[i]);
    
    // make Matrix kt
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < m; j ++)
            kt[j][i] = k[i][j];
    
    LL ans[N][M];
    for (int i = 0; i < n; i ++)
    {
        // w · (q x kt) 逐行记录至 res[]，得到原先 res[][] 第 i 行的结果
        for (int j = 0; j < n; j ++)
        {
            LL mark = 0;
            for (int k = 0; k < m; k ++)
                mark += (LL)q[i][k] * kt[k][j];
            res[j] = mark * w[i];
        }
        
        // w · (q x kt) x v 利用 res[]，逐行计算 ans[][] 的结果
        for (int j = 0; j < m; j ++)
        {
            LL mark = 0;
            for (int k = 0; k < n; k ++)
                mark += res[k] * v[k][j];
            ans[i][j] = mark;
        }
    }
        
    // print answer
    for (int i = 0; i < n; i ++)
    {
        for (int j = 0; j < m; j ++)
            printf("%lld ", ans[i][j]);
        puts("");
    }
    return 0;
}
```
## 算法思路
- 两个矩阵相乘 `Matrix1 x Matrix2`，其中 `Matrix1` 为 n x m，`Matrix2` 为 m x p，那么模拟计算的时间复杂度为 $O(n \times m \times p)$，故优化方式在于矩阵计算的先后顺序
- 时间优化，调整矩阵的计算顺序。此外，该题解并没有进行空间优化
- 时间优化后的时间复杂度为：$O(n \times m^2)$
- 该题解可以通过 AcWing 官网评测系统下 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;
typedef long long LL;
const int N = 1e+4 + 10, M = 25;

int n, m;   // 矩阵 n 行 m 列
LL q[N][M], k[N][M], kt[M][N], v[N][M];
LL w[N];

LL res[M][M], ans[N][M];

void getMatrix(int MatrixType)
{
    if (MatrixType == 0)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%lld", &q[i][j]);
    }
    else if (MatrixType == 1)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%lld", &k[i][j]);
    }
    else if (MatrixType == 2)
    {
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                scanf("%lld", &v[i][j]);
    }
}

int main()
{
    scanf("%d%d", &n, &m);
    
    // get Matrix q k v w
    for (int i = 0; i < 3; i ++) getMatrix(i);
    for (int i = 0; i < n; i ++) scanf("%lld", &w[i]);
    
    // make Matrix kt
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < m; j ++)
            kt[j][i] = k[i][j];
    
    // kt x v
    for (int i = 0; i < m; i ++)
        for (int j = 0; j < m; j ++)
        {
            int mark = 0;
            for (int k = 0; k < n; k ++)
                mark += kt[i][k] * v[k][j];
            res[i][j] = mark;
        }
    
    // q x (kt x v)
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < m; j ++)
        {
            int mark = 0;
            for (int k = 0; k < m; k ++)
                mark += q[i][k] * res[k][j];
            ans[i][j] = mark;
        }
    
    // w · (q x (kt x v))
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < m; j ++)
            ans[i][j] *= w[i];
        
    // print answer
    for (int i = 0; i < n; i ++)
    {
        for (int j = 0; j < m; j ++)
            printf("%lld ", ans[i][j]);
        puts("");
    }
    
    return 0;
}
```

# Q3 解压缩
## 算法思路
```C++

```