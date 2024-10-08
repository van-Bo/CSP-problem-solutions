> 第24次CCF计算机软件能力认证
> 模拟认证 CSP（100 + 100 + 40 + 25 + 24）
> 模拟认证 AcWing（(10/10), (11/11), (5/12), (9/20), (9/33)）

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

# Q2 序列查询新解
## 算法思路
- 暴力枚举
- 二分求取 `f(x)`
- 时间复杂度：$O(N \times \log n)$
- 该题解可以通过 AcWing 官网 (6/11) 的数据点，CSP 官网评测系统下得分为 70 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int M = 1e+5 + 10;

int n, N;
int w[M];

bool check(int index, int v)
{
    return w[index] <= v;
}

int get(int v)
{
    int l = 0, r = n;
    while (l < r)
    {
        int mid = (l + r + 1) / 2;
        if (check(mid, v)) l = mid;
        else r = mid - 1;
    }
    return l;
}

int main()
{
    scanf("%d%d", &n, &N);
    for (int i = 1; i <= n; i ++)
        scanf("%d", &w[i]);
    
    int r = N /(n + 1);
    LL res = 0;
    for (int i = 1; i <= N - 1; i ++)
    {
        int f = get(i);
        int g = i / r;
        res += abs(f - g);
    }
    printf("%lld\n", res);
    return 0;
}
```
## 算法思路
- 分类讨论
- 处理操作中每次处理的目标区间的端点值为 `l` 和 `r`，其中 `l` 的值为 `w[i]`，`r` 的值为 `w[i + 1] - 1`
- 在目标区间中 $l \leqslant x \leqslant r$，`f[x]` 的值恒定为 `i`，故确定 `g[x]` 的值即可。`g[x]` 的取值是一个总体呈上升趋势的阶梯函数，因为求解的结果值含有取绝对值，所以此处进行一次分类讨论，其中分类的依据为 `f[x]` 与 `g[x]` 函数的相对位置。
- 第一、二种情况是 `f[x]` 与 `g[x]` 总体有着明确的大小关系，函数图像一上一下
- 第三种情况是 `f[x]` 与 `g[x]` 存在交界点，交界点两侧的大小关系逆转，该种情况可以一分为二，转化成第一、二种情况分别求解
- `get(l, r)` 计算 `g[l] + g[l + 1] + ... + g[r]` 的值，针对于区间 `[l, r]` 可以分为 首、中间、尾三段，其中，中间段的所有区间的长度均为 `R`，此处在同一区间中，每一元素值的 `g` 值是相同的
- 该题解可以通过 AcWing 官网 (11/11) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int N = 1e+5 + 10;

int n, m, R;
int w[N];

LL get(int l, int r)   // 计算 g[l] + g[l + 1] + ... + g[r]
{
    if (l / R == r / R) return (LL)(l / R) * (r - l + 1);
    
    int x = l / R, y = r / R;   // 首尾段的 g[] 值
    int u = x + 1, v = y - 1;   // 中间段的首尾 g[] 值
    
    LL res = 0;
    res += (LL)(u + v) * (v - u + 1) / 2 * R;   // 中间段
    res += (LL)x * (u * R - l);  // 首段
    res += (LL)y * (r - (v * R + R - 1));  // 尾段
    
    return res;
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++)
        scanf("%d", &w[i]);
    w[n + 1] = m;
    R = m / (n + 1);
    
    LL res = 0;
    for (int i = 0; i <= n; i ++)
    {
        // x = w[i], 1, ..., w[i + 1] - 1
        int l = w[i], r = w[i + 1] - 1;
        
        if (l / R >= i || r / R <= i)
        {
            res += abs((LL)i * (r - l + 1) - get(l, r));
        }
        else
        {
            int lrv = i * R - 1, rlv = i * R + R;
            res += abs((LL)i * (lrv - l + 1) - get(l, lrv));   // 左区间
            res += abs((LL)i * (r - rlv + 1) - get(rlv, r));   // 右区间
        }
    }
    printf("%lld\n", res);
    return 0;
}
```

# Q3 登机牌条码
## 算法思路
- 通过前 40% 的数据点，此类数据点的特点是无需计算校验码字
- 注意在编码阶段，编码器处于大写字母模式。
- 该题解可以通过 AcWing 官网 (5/12) 的数据点，CSP 官网评测系统下的得分为 40 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
const int N = 2000, M = 1000;

int w, s;
string str;
int num[N], codeWord[M];
int cntNum, cntWord;

void getCodeWord()
{
    int len = str.length();
    int mark = 0; // 0 为大写字母，1 为小写字母，2 为数字
    for (int i = 0; i < len; i ++)
    {
        if (isdigit(str[i]))    // 当前的输入项为数字
        {
            if (mark == 0) // 前一字符是大写字母
            {
                num[cntNum ++] = 28;
                num[cntNum ++] = str[i] - '0';
            }
            else if (mark == 1) // 前一字符是小写字母
            {
                num[cntNum ++] = 28;
                num[cntNum ++] = str[i] - '0';
            }
            else if (mark == 2) // 前一字符是数字
            {
                num[cntNum ++] = str[i] - '0';
            }
            mark = 2;
        }
        else if (str[i] >= 'a' && str[i] <= 'z')    // 当前输入项为小写字母
        {
            if (mark == 0) // 前一字符是大写字母
            {
                num[cntNum ++] = 27;
                num[cntNum ++] = str[i] - 'a';
            }
            else if (mark == 1) // 前一字符是小写字母
            {
                num[cntNum ++] = str[i] - 'a';
            }
            else if (mark == 2) // 前一字符是数字
            {
                num[cntNum ++] = 27;
                num[cntNum ++] = str[i] - 'a';
            }
            mark = 1;
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')    // 当前输入项为大写字母
        {
            if (mark == 0) // 前一字符是大写字母
            {
                num[cntNum ++] = str[i] - 'A';
            }
            else if (mark == 1) // 前一字符是小写字母
            {
                num[cntNum ++] = 28;
                num[cntNum ++] = 28;
                num[cntNum ++] = str[i] - 'A';
            }
            else if (mark == 2) // 前一字符是数字
            {
                num[cntNum ++] = 28;
                num[cntNum ++] = str[i] - 'A';
            }
            mark = 0;
        }
    }
}

int main()
{
    scanf("%d%d", &w, &s);
    if (s != -1) return 0;
    
    cin >> str;
    getCodeWord();
    
    if (cntNum % 2 == 1)
        num[cntNum ++] = 29;
        
    for (int i = 0; i < cntNum; i ++)
    {
        int H = num[i], L = num[i + 1];
        codeWord[cntWord ++] = H * 30 + L;
        i ++;
    }
    
    if ((cntWord + 1) % w == 0)
    {
        printf("%d\n", cntWord + 1);
        for (int i = 0; i < cntWord; i ++)
            printf("%d\n", codeWord[i]);
    }
    else
    {
        int cnt900 = w - (cntWord + 1) % w;
        printf("%d\n", cntWord + 1 + cnt900);
        for (int i = 0; i < cntWord; i ++)
            printf("%d\n", codeWord[i]);
        for (int i = 0; i < cnt900; i ++)
            printf("%d\n", 900);
    }
    return 0;
}
```

# Q4 磁盘文件操作
## 算法思路
- `unordered_map<int, PII> pr` 记录地址单元与占用程序和当前位置存储值之间的映射关系，`pr[x] = {id, v}`表示地址单元的占用程序编号为 `id`（其中程序编号值为零时，则表示无程序占用），该地址单元的存储值为 `v`
- `unordered_map<int, int>` 记录地址单元被占用的前一个程序编号
- 每次按照操作编号，朴素枚举即可
- 该题解可以通过 AcWing 官网 (9/20) 的数据点，CSP 官网评测系统下的得分为 25 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;

int n, m, k;
unordered_map<int, PII> pr;
unordered_map<int, int> preID;

void deal0()
{
    int id, l, r, x;
    scanf("%d%d%d%d", &id, &l, &r, &x);
    
    int mark = -1;
    
    for (int i = l; i <= r; i ++)
    {
        if (pr.find(i) != pr.end()) // 地址存在
        {
            if (pr[i].x == id)
                pr[i].y = x, preID[i] = id, mark = i;
            else if (pr[i].x == 0)
                pr[i].x = id, pr[i].y = x, preID[i] = 0, mark = i;
            else
                break;
        }
        else
        {
            pr[i].x = id;
            pr[i].y = x;
            preID[i] = 0;
            mark = i;
        }
    }
    
    printf("%d\n", mark);
}

void deal1()    // 删除
{
    int id, l, r;
    scanf("%d%d%d", &id, &l, &r);
    
    bool flag = true;
    for (int i = l; i <= r; i ++)
    {
        if (pr.find(i) == pr.end()) // 地址未被占用
        {
            flag = false;
            break;
        }
        else
        {
            if (pr[i].x != id)
            {
                flag = false;
                break;
            }
        }
    }
    
    if (flag)
    {
        puts("OK");
        for (int i = l; i <= r; i ++)
            preID[i] = pr[i].x, pr[i].x = 0;
    }
    else
        puts("FAIL");
}

void deal2() // 恢复
{
    int id, l, r;
    scanf("%d%d%d", &id, &l, &r);
    
    bool flag = true;
    for (int i = l; i <= r; i ++)
        if (preID.find(i) == preID.end() || preID[i] != id)
        {
            flag = false;
            break;
        }
        
    if (flag)
    {
        for (int i = l; i <= r; i ++)
            if (pr.find(i) != pr.end() && pr[i].x != 0)
            {
                flag = false;
                break;
            }
    }
    
    if (flag)
    {
        puts("OK");
        for (int i = l; i <= r; i ++)
            pr[i].x = id;
    }
    else
        puts("FAIL");
}

void deal3()
{
    int p;
    scanf("%d", &p);
    
    if (pr.find(p) != pr.end() && pr[p].x != 0)
        printf("%d %d\n", pr[p].x, pr[p].y);
    else
        printf("0 0\n");
}

int main()
{
    scanf("%d%d%d", &n, &m, &k);
    
    for (int i = 0; i < k; i ++)
    {
        int op;
        scanf("%d", &op);
        
        if (op == 0)
            deal0();
        else if (op == 1)
            deal1();
        else if (op == 2)
            deal2();
        else
            deal3();
            
    }
    return 0;
}
```

# Q5 极差路径
## 算法思路
- 通过枚举起始点，以 `dfs` 的方式进行对推荐路径数的维护
- `dfs(st, preEd, ed, mx, mn)` 函数中，`st` 为枚举的起始点（深搜的中心），`ed` 为此时路径中枚举到的最后的节点，`preEd` 为 `ed` 的前一个枚举点（避免回溯），`mx` 为路径中的最大不满意程度，`mn` 为路径中的最小不满意程度
- `dfs()` 过程中对于同一条路径，会在答案集合中重复维护，同一条路径，`i ---> j` 和 `j ---> i`，故最终经过 `dfs()` 后的答案要除以2
- `dfs()` 过程中不会处理起始点相同的路径，故最终答案要额外添加一个节点数 `n`
- 时间复杂度：$O(n \times m)$，其中 `n` 为点数，`m` 为边数
- 该题解可以通过 AcWing 官网 (5/33) 的数据点，CSP 官网评测系统下的得分为 12 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int N = 5e+5 + 10, M = N * 2;

int n, k1, k2;
int h[N], e[M], ne[M], idx;
LL ans;

void add(int a, int b)
{
    e[idx] = b, ne[idx] = h[a], h[a] = idx ++;
}

void dfs(int st, int preEd, int ed, int mx, int mn)
{
    if (st != ed)
    {
        if (mn >= min(st, ed) - k1 && mx <= max(st, ed) + k2)
            ans ++;
    }
    
    for (int i = h[ed]; ~i; i = ne[i])
    {
        int j = e[i];
        if (j == preEd) continue;
        dfs(st, ed, j, max(j, mx), min(j, mn));
    }
}

int main()
{
    scanf("%d%d%d", &n, &k1, &k2);
    
    memset(h, -1, sizeof h);
    for (int i = 0; i < n - 1; i ++)
    {
        int p, c;
        scanf("%d%d", &p, &c);
        
        add(p, c), add(c, p);
    }
    
    for (int i = 1; i <= n; i ++)
        dfs(i, i, i, i, i);
        
    printf("%lld\n", ans / 2 + n);
    return 0;
}
```
## 算法思路
- 编号为 10、11、12 的数据点拥有性质 `A2`，该性质使得图特殊化为一个星形的图，存在一个 `center` 的节点，其度数为 n - 1
- 接下来可以线性求解（此时路径中的节点数为 2 或 3），首先明确的大前提是每一种路径的起始点为 `x` 和 `y`，并且 $x < y$，那么 `center` 节点就可能与 `x` 或 `y` 相等，也可能都不相等
- 线性求解的逻辑是分类讨论，给定一个确定的 `x` 值，根据 `x` 与 `center` 的大小关系来确定 `y` 的合法值的范围，从而维护答案值 `ans`
- 参考依据的不等式为：$x - k1 \leqslant min P(x, y) \leqslant max P(x, y) \leqslant y + k2$，之后根据分类的条件，会针对此不等式进行化简，从而求解 `y` 的合法值范围
- 第一个分类条件 `x == center`，简化不等式为 $x - k1 \leqslant x \leqslant y \leqslant y + k2$，其中第 1、2、3 个 ≤ 号一定都恒成立，此时 `y` 的取值范围 `[x + 1, n]`
- 第二个分类条件 `x < center`，简化不等式为 $x - k1 \leqslant x \leqslant max P(x, y) \leqslant y + k2$，其中第 1、2 个 ≤ 号恒成立。再次分类确定 `max P(x, y)` 的取值，若 $x + 1 \leqslant y \leqslant center$，第三个比较式简化为 $center \leqslant y + k2$，联合化简可得 $max(x + 1, center - k2) \leqslant y \leqslant center$；另一个再次分类，若 $center + 1 \leqslant y$，第三个比较式恒成立。可以将两个再次分类的结论范围合并，此时 `y` 的取值范围为 `[max(x + 1, center - k2), n]`
- 第三个分类条件 `x > center`，简化不等式为 $x - k1 \leqslant center \leqslant y \leqslant y + k2$，其中第 2、3 个 ≤ 号恒成立，当 $center < x \leqslant center + k1$，此时 `y` 的取值范围为 `[x + 1, n]`
- 最后添加单个节点为一条路径的结果，`ans` 的基础之上加节点数
- 该题解可以通过 AcWing 官网 (9/33) 的数据点，CSP 官网评测系统下的得分为 24 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int N = 5e+5 + 10, M = N * 2;

int n, k1, k2;
int h[N], e[M], ne[M], idx;
LL ans;

int cnt[N]; // 统计节点的度数

void add(int a, int b)
{
    e[idx] = b, ne[idx] = h[a], h[a] = idx ++;
}

void dfs(int st, int preEd, int ed, int mx, int mn)
{
    if (st != ed)
    {
        if (mn >= min(st, ed) - k1 && mx <= max(st, ed) + k2)
            ans ++;
    }
    
    for (int i = h[ed]; ~i; i = ne[i])
    {
        int j = e[i];
        if (j == preEd) continue;
        dfs(st, ed, j, max(j, mx), min(j, mn));
    }
}

int main()
{
    scanf("%d%d%d", &n, &k1, &k2);
    
    memset(h, -1, sizeof h);
    for (int i = 0; i < n - 1; i ++)
    {
        int p, c;
        scanf("%d%d", &p, &c);
        
        add(p, c), add(c, p);
        cnt[p] ++, cnt[c] ++;
    }
    
    bool flag = false;
    int center = -1;
    for (int i = 1; i <= n; i ++)
        if (cnt[i] == n - 1)
            flag = true, center = i;
    
    if (flag)
    {
        for (int i = 1; i <= n; i ++)
        {
            if (i == center)
                ans += n - (i + 1) + 1;
            else if (i < center)
                ans += n - max(i + 1, center - k2) + 1;
            else
            {
                if (i <= center + k1)
                    ans += n - (i + 1) + 1;
            }
        }
        
        printf("%lld\n", ans + n);
    }
    else 
    {
        for (int i = 1; i <= n; i ++)
        dfs(i, i, i, i, i);
        
        printf("%lld\n", ans / 2 + n);
    }
    
    return 0;
}
```