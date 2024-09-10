> 第29次CCF计算机软件能力认证
> 模拟认证 CSP（）
> 模拟认证 AcWing（）

# Q1 田地丈量
## 算法思路
```C++

```

# Q2 垦田计划
## 算法思路
```C++

```

# Q3 LDAP
## 算法思路
```C++

```

# Q4 星际网络Ⅱ
## 算法思路
```C++

```

# Q5 施肥
## 算法思路
- 暴力枚举（枚举对象为施肥车），针对测试点 `1, 2, 3`
- `forceEnumeration()` 使用二进制枚举
- `isValid(v)` 在 `v` 表示下的合法的枚举对象集合中，进行区间合并，判定区间是否可以连续
- 区间合并时，若区间可以合并，则将施肥区间的情况纳入 `ans` 中，此处使用 `string` 来映射施肥区间，`PII` 没有对应的哈希函数
- 区间合并时，若区间无法合并，则直接结束该种枚举对象情况的枚举
- 时间复杂度：$O(m \log m + 2^m \times m)$
- 该题解可以通过 AcWing 官网 (4/21) 的数据点，CSP 官网评测系统下得分为 15 分
## HINT
- 此处的区间合并是在整数层面的连续，判定可以合并的条件为 `e[i].x <= ed + 1`，判定无法合并的条件为：`ed + 1 < e[i].x`
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 2e+5 + 10;

int n, m;
PII e[N];
unordered_set<string> ans;
bool st[N];

bool isValid(int v)  // 判定区间是否可以连续
{
    bool flag = true;
    
    int start = -1, ed = -1;
    for (int i = 0; i < m; i ++)
        if (v >> i & 1)
        {
            if (ed == -1) 
            {
                start = e[i].x;
                ed = e[i].y;
                continue;
            }
            
            if (e[i].x <= ed + 1)
                ed = max(ed, e[i].y);
            else if (ed + 1 < e[i].x)
            {
                flag = false;
                break;
            }
        }
    
    if (ed == -1) flag = false;
    
    if (flag)
    {
        string s = to_string(start) + "+" + to_string(ed);
        ans.insert(s);
    }
    return flag;
}

void forceEnumeration()
{
    for (int i = 0; i <= (1 << m) - 1; i ++)
        isValid(i);
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i ++)
        scanf("%d%d", &e[i].x, &e[i].y);
    
    sort(e, e + m);
    
    forceEnumeration();
    printf("%d\n", ans.size());
    return 0;
}
```
## 算法思路
- 暴力枚举（枚举对象为施肥区间的边界值）
- `check(l, r)` 针对区间 `[l, r]` 的合法性进行判定
- 时间复杂度为：$O(n^2 \times m)$ - $O(n^3 \times m)$
- 该题解可以通过 AcWing 官网 (8/21) 的数据点，CSP 官网评测系统下得分为 35 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
typedef long long LL;
const int N = 2e+5 + 10;

int n, m;
PII e[N];
bool st[N];
int res;

bool check(int l, int r)
{
    memset(st, 0, sizeof st);
    for (int i = 0; i < m; i ++)
        if (e[i].x >= l && e[i].y <= r)
            for (int j = e[i].x; j <= e[i].y; j ++)
                st[j] = true;
                
    for (int i = l; i <= r; i ++)
        if (!st[i]) return false;
    return true;
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i ++)
        scanf("%d%d", &e[i].x, &e[i].y);
    
    for (int l = 1; l <= n; l ++)
        for (int r = l + 1; r <= n; r ++)
            if (check(l, r))
                res ++;
                
    printf("%d\n", res);
    return 0;
}
```