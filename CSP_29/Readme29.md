> 第29次CCF计算机软件能力认证
> 模拟认证 CSP（100 + 100 + 20 + 20 + 35）
> 模拟认证 AcWing（(10/10), (11/11), (1/10), (5/21), (8/21)）

# Q1 田地丈量
## 算法思路
- 线段求交、面积求交
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下得分为 100 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;
typedef long long LL;

int n, a, b;

int getLine(int x, int y, int u, int v)
{
    return max(0, min(y, v) - max(x, u));
}

int main()
{
    scanf("%d%d%d", &n, &a, &b);
    
    LL res = 0;
    for (int i = 0; i < n; i ++)
    {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        
        res += (LL)getLine(x1, x2, 0, a) * getLine(y1, y2, 0, b);
    }
    printf("%lld\n", res);
    return 0;
}
```

# Q2 垦田计划
## 算法思路
- 二分枚举最小的耗时，每次判定遍历所有开垦区域，通过统计 `cost` 判定是否有效（该目标最小耗时能否实现）
- 时间复杂度：$O(n \log n)$
- 该题解可以通过 AcWing 官网 (11/11) 的数据点，CSP 官网评测系统下得分为 100 分
```C++
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;
typedef long long LL;
const int N = 1e+5 + 10;

int n, m, k;
int t[N], c[N];

bool check(int v)
{
    LL cost = 0;
    for (int i = 0; i < n; i ++)
    {
        if (t[i] > v)
            cost += (LL)(t[i] - v) * c[i];
    }
    return cost <= m;
}

int main()
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 0; i < n; i ++)
        scanf("%d%d", &t[i], &c[i]);
        
    int l = k, r = 1e+5;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (check(mid)) r = mid;
        else l = mid + 1;
    }
    printf("%d\n", l);
    return 0;
}
```

# Q3 LDAP
## 算法思路
- 针对处理的表达式的范式类型为：`BASE_EXPR`，`EASY_EXPR`
- `user` 记录每个用户的标识符
- `pr` 建立由用户的标识符到一个用于标记存储位置的下标的映射关系，此题中用户个数的上限值为 2500
- `userAtr[x]` 记录 `x`（某一用户标识符的映射下标值）下的用户属性编号
- `userAtrValue[x]` 记录 `x` （某一用户标识符的映射下标值）下的用户属性编号与该属性的属性值的映射关系 
- 该题解可以通过 AcWing 官网 (1/10) 的数据（因为范式 `EXPR` 的存在，会出现针对于 `stoi()` 的 `runtime error`），CSP 官网评测系统下得分为 20 分（感觉可以通过前 40% 的数据的，不知道哪里出现了问题）
```C++
#include <iostream>
#include <algorithm>
#include <cstring>
#include <unordered_set>
#include <unordered_map>
#include <set>

using namespace std;
typedef pair<int, int> PII;
const int N = 2505;

unordered_set<int> users;   // 记录 dn
unordered_map<int, int> pr; // 映射 dn --> index in users
unordered_set<int> userAtr[N];
unordered_map<int, int> userAtrValue[N];
int n, m;

set<int> getUsers(string s)
{
    set<int> res;
    
    int id, v, len = s.length();
    char op;
    string num = "";
    for (int i = 0; i < len; i ++)
    {
        if (isdigit(s[i]))
            num += s[i];
        else if (s[i] == ':' || s[i] == '~')
            id = stoi(num), num = "", op = s[i];
    }
    v = stoi(num);

    if (op == ':')
    {
        for (auto u : users)
        {
            if(userAtr[pr[u]].find(id) != userAtr[pr[u]].end() && userAtrValue[pr[u]][id] == v)
                res.insert(u);
        }
    }
    else if (op == '~')
    {
        for (auto u : users)
        {
            if(userAtr[pr[u]].find(id) != userAtr[pr[u]].end() && userAtrValue[pr[u]][id] != v)
                res.insert(u);
        }
    }
    return res;
}


void deal(string s)
{
    if (s[0] != '&' && s[0] != '|')    // BASE_EXPR
    {
        auto res = getUsers(s);
        for (auto t : res)
            printf("%d ", t);
        puts("");
        
    }
    else    // EASY_EXPR
    {
        set<int> res;   // 记录结果值
        
        int len = s.length();
        if (s[0] == '&')    // 结果值求交操作
        {
            set<int> temp;
            string atom = "";
            
            for (int i = 1; i < len; i ++)
            {
                if (s[i] == '(') continue;
                if (s[i] >= '0' && s[i] <= '9')
                    atom += s[i];
                else if (s[i] == ':' || s[i] == '~')
                    atom += s[i];
                else if (s[i] == ')')
                {
                    temp = getUsers(atom);
                    atom = "";  // Key point
                    
                    if (res.size() == 0) 
                    {
                        res = temp;
                        continue;
                    }
                    
                    for (auto t : res)
                    {
                        if (temp.find(t) == temp.end())
                            res.erase(t);
                    }
                }
            }
            
            for (auto t : res)
                printf("%d ", t);
            puts("");
        }
        else if (s[0] == '|')   // 结果值求并操作
        {
            set<int> temp;
            string atom = "";
            for (int i = 1; i < len; i ++)
            {
                if (s[i] == '(') continue;
                if (s[i] >= '0' && s[i] <= '9')
                    atom += s[i];
                else if (s[i] == ':' || s[i] == '~')
                    atom += s[i];
                else if (s[i] == ')')
                {
                    temp = getUsers(atom);
                    atom = "";  // Key point
                    
                    if (res.size() == 0)
                    {
                        res = temp;
                        continue;
                    }
                    
                    for (auto t : temp)
                    {
                        if (res.find(t) == res.end())
                            res.insert(t);
                    }
                }
            }
            
            for (auto t : res)
                printf("%d ", t);
            puts("");
        }
    }
}

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++)
    {
        int dn, cntAtr;
        scanf("%d%d", &dn, &cntAtr);
        for (int j = 0; j < cntAtr; j ++)
        {
            int id, v;
            scanf("%d%d", &id, &v);
            
            users.insert(dn);
            pr[dn] = i;
            userAtr[i].insert(id);
            userAtrValue[i][id] = v;
        }
    }
    
    scanf("%d", &m);
    for (int i = 0; i < m; i ++)
    {
        string s;
        cin >> s;
        deal(s);
    }
    return 0;
}
```

# Q4 星际网络Ⅱ
## 算法思路
- 操作类型有 3 种，函数 `deal1`、`deal2`、`deal3` 分别实现针对不同操作数 `op` 的处理
- 针对 16 进制形式的地址（形式为：xxxx:xxxx），使用 `string` 存储
- 哈希表 `unordered_map<string, int> adr`存储地址和用户的相关信息，`adr[p] = id` 表示地址 `p` 已经分配给了用户 `id` 
- `getNextAdr(p)` 实现地址 `p` 的自增
- `deal1()` 函数中，`cntNullAdr` 统计给定地址区间中未分配地址的个数，`cntTotalAdr` 统计给定地址区间中总的地址个数，`cntIdAdr` 统计给定地址区间中已分配给用户 `id` 的地址个数；检测成功的条件有两种（程序中有注释标注），满足一种即可，若都不满足，则检测失败
- `deal2()` 函数处理简单，和哈希表定义的意义完全吻合
- `deal3()` 函数要判定给定的地址区间是否完整的分配给了某用户，判定为否的情况有两种，分别对应 `break` 的出口；其一，出现了未分配的地址，其二，地址分配用户不唯一
- 时间复杂度：$O(q \times 2^n)$
- 该题解可以通过 AcWing 官网 (5/21) 的数据点，CSP 官网评测系统下得分为 20 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;
typedef long long LL;
unordered_map<string, int> adr; // address -> user

int n, q;

string getNextAdr(string p)
{
    int len = p.length();
    
    for (int i = len - 1; i >= 0; i --)
    {
        if (p[i] == ':') continue;
        
        if (p[i] >= '0' && p[i] <= '8')
        {
            p[i] = p[i] + 1;
            break;
        }
        else if (p[i] == '9')
        {
            p[i] = 'a';
            break;
        }
        else if (p[i] >= 'a' && p[i] <= 'e')
        {
            p[i] = p[i] + 1;
            break;
        }
        else if (p[i] == 'f')
        {
            p[i] = '0';
        }
    }
    return p;
}

void deal1()
{
    int id;
    string l, r;
    scanf("%d", &id);
    cin >> l >> r;
    
    string p = l;   //working point
    LL cntNullAdr = 0, cntTotalAdr = 0;
    unordered_set<string> nullAdr;
    LL cntIdAdr = 0;
    while (true)
    {
        cntTotalAdr ++;
        if (adr.find(p) != adr.end())   // 地址 p 已分配给了某用户
        {
            if (adr[p] == id)
                cntIdAdr ++;
        }
        else    // 地址 p 未被分配
        {
            nullAdr.insert(p);
            cntNullAdr ++;
        }
        
        if (p == r) break;
        else
            p = getNextAdr(p);
    }
    
    if (cntNullAdr == cntTotalAdr)  //l-r 的地址均未分配
    {
        for (auto t : nullAdr)
            adr[t] = id;
            
        printf("YES\n");
    }
    else if (cntNullAdr && (cntTotalAdr - cntNullAdr) == cntIdAdr)// l-r 存在空地址，非空地址用户均为 id
    {
        for (auto t : nullAdr)
            adr[t] = id;
        printf("YES\n");
    }
    else 
        printf("NO\n");
}

void deal2()
{
    string s;
    cin >> s;
    
    if (adr.find(s) != adr.end())
        printf("%d\n", adr[s]);
    else 
        printf("0\n");
}

void deal3()
{
    string l, r;
    cin >> l >> r;
    
    string p = l;
    int userId = -1;
    bool isOnlyUser = true;
    while (true)
    {
        if (adr.find(p) == adr.end())   // 地址 p 未被分配
        {
            isOnlyUser = false;
            break;
        }

        if (userId == -1 && adr.find(p) != adr.end())
        {
            userId = adr[p];
        }
        else if (userId != -1 && adr.find(p) != adr.end())
        {
            if (adr[p] != userId)
            {
                isOnlyUser = false;
                break;
            }
        }
        
        if (p == r) break;
        else
            p = getNextAdr(p);
    }
    
    if (isOnlyUser)
        printf("%d\n", userId);
    else 
        printf("0\n");
}

int main()
{
    scanf("%d%d", &n, &q);
    
    while (q --)
    {
        int op;
        scanf("%d", &op);
        
        if (op == 1)
            deal1();
        else if (op == 2)
            deal2();
        else if (op == 3) 
            deal3();
    }
    return 0;
}
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