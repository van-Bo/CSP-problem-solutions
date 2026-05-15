# CSP(第39次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + () + ())

## Q1 蒙特卡洛

### Q1 算法思路

- 模拟即可
- 该题解可以通过 smqyOJ (10/10) 的测试点，得分 100 分

### Q1 代码实现

```cpp
#include <bits/stdc++.h>
using namespace std;

double n, a;

int main()
{
    cin >> n >> a;
    double m = 0;
    for (int i = 0; i < n; i ++)
    {
        double x, y;
        cin >> x >> y;
        if (x * x + y * y <= a * a) m ++;
    }
    printf("%.6lf\n", 4 * m / n);
    return 0;
}
```