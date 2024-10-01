> CSP（第28次CCF计算机软件能力认证）
> 模拟认证（）

# Q1 现值计算
## 算法思路

# Q2 训练计划
## 算法思路

# Q3 JPEG编码
## 算法思路
- 模拟即可
- `getM()` 将扫描序列 `scan[]` 按照 z 形填充至 `m[][]`
- `getOrder()` 获取 `m[x][y]` 元素在扫描序列中的序号位置，涉及数学推导
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <math.h>

using namespace std;
const int N = 10;

int q[N][N], m[N][N], ans[N][N];
int T, n;
int scan[N * N];
double mt[N][N];

int getOrder(int x, int y)
{
    int res = 0;
    
    int mark = x + y;   // 该位置的斜线标记为 mark，前面完整的斜线有 mark 条 
    if (mark <= 8) res += (1 + mark) * (mark) / 2;
    else res += (1 + 8) * 8 / 2 + (7 + 7 - (mark - 9)) * (mark - 8) / 2;
    
    if (mark % 2 == 0)
    {
        int x_max = -1;
        if (mark <= 7) x_max = mark;
        else x_max = 7;
        res += x_max - x + 1;
    }
    else
    {
        int x_min = -1;
        if (mark <= 7) x_min = 0;
        else x_min = mark - 7;
        res += x - x_min + 1;
    }   
    return res;
}

void getM()
{
    for (int i = 0; i < 8; i ++)
    {
        for (int j = 0; j < 8; j ++)
        {
            int order = getOrder(i, j);
            if (order > n) continue;
            m[i][j] = scan[order];
        }
    }
}

double getAlpha(int v)
{
    double root = 0.5;
    if (v == 0) return sqrt(root);
    else return 1;
}

void getMt()
{
    for (int i = 0; i < 8; i ++)
    {
        for (int j = 0; j < 8; j ++)
        {
            double res = 0;
            for (int u = 0; u < 8; u ++)
                for (int v = 0; v < 8; v ++)
                {
                    double pi = acos(-1);
                    res += getAlpha(u) * getAlpha(v) * m[u][v] * cos(pi * (i + 0.5) * u / 8) * cos(pi * (j + 0.5) * v / 8);
                }
            mt[i][j] = res / 4;
        }
    }
}

void printMatrix()
{
    if (T == 0 || T == 1)
    {
        for (int i = 0; i < 8; i ++)
        {
            for (int j = 0; j < 8; j ++)
                printf("%d ", m[i][j]);
            puts("");
        }
    }
    else if (T == 2)
    {
        for (int i = 0; i < 8; i ++)
        {
            for (int j = 0; j < 8; j ++)
                printf("%d ", ans[i][j]);
            puts("");
        }
    }
}

int main()
{
    // step 1
    for (int i = 0; i < 8; i ++)
        for (int j = 0; j < 8; j ++)
            scanf("%d", &q[i][j]);
    
    scanf("%d%d", &n, &T);
    for (int i = 1; i <= n; i ++) scanf("%d", &scan[i]);
    
    // step 3        
    getM();
    if (T == 0)
        printMatrix();
    
    // step 4
    for (int i = 0; i < 8; i ++)
        for (int j = 0; j < 8; j ++)
            m[i][j] *= q[i][j];
    if (T == 1)
        printMatrix();

    // step 5
    getMt();
    
    // step 6
    for (int i = 0; i < 8; i ++)
        for (int j = 0; j < 8; j ++)
        {
            mt[i][j] += 128;
            int res = round(mt[i][j]);
            res = max(res, 0);
            res = min(res, 255);
            ans[i][j] = res;
        }
    if (T == 2)
        printMatrix();
    
    return 0;
}
```

# Q4 聚集方差
## 算法思路
