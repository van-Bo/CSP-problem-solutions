# 第36次CCF计算机软件能力认证

> smqyOJ Judge((10/10))

## Q1 移动

### Q1 算法思路

- 模拟即可
- `Hack`: 原先首先想到的是横纵向操作进行抵消，但是发现如果存在触碰边界的情况，本身某些操作便是无效操作，更谈不上抵消操作的资格
- 该题解可以通过 smqyOJ (10/10) 的测试点，得分 100 分

### Q1 代码实现

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> PII;
#define x first
#define y second

int n, k;
unordered_map<char, PII> mp;

void deal(int stx, int sty, string op)
{
    int op_len = op.length();
    for (int i = 0; i < op_len; i ++)
    {
        stx += mp[op[i]].x, sty += mp[op[i]].y;
        stx = max(stx, 1), stx = min(stx, n);
        sty = max(sty, 1), sty = min(sty, n);
    }
    printf("%d %d\n", stx, sty);
}

int main()
{
    mp['f'] = {0, 1};
    mp['b'] = {0, -1};
    mp['l'] = {-1, 0};
    mp['r'] = {1, 0};

    cin >> n >> k;
    for (int i = 0; i < k; i ++)
    {
        int stx, sty;
        string op;
        cin >> stx >> sty >> op;
        deal(stx, sty, op);
    }
    return 0;
}
```
