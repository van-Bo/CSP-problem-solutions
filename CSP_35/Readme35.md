> CSP(第35次CCF计算机软件能力认证)
> 正式认证(100 + 80 + 0 + 30 + 35)
> 模拟认证(100 + 100 + 0 + 60 + 35)

# Q1 密码
## 算法思路
- 模拟即可
- `pr` 用于标记每种字符出现的次数
- `f1`、`f2`、`f3` 分别用于标记是否出现过数字, 字母, 特殊字符
- `flag` 用于标记是否满足同一字符的出现次数不超过 2 次
- 该题解 CSP 官网得分 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

#define x first
#define y second

using namespace std;
unordered_map<char, int> pr;
int n;

int main()
{
    scanf("%d", &n);

    while (n --)
    {
        string s;
        cin >> s;
        int len = s.length();

        pr.clear();

        bool f1 = false, f2 = false, f3 = false;
        for (int i = 0; i < len; i ++)
        {
            if (isdigit(s[i])) f1 = true;
            if (s[i] >= 'a' && s[i] <= 'z') f2 = true;
            if (s[i] >= 'A' && s[i] <= 'Z') f2 = true;
            if (s[i] == '*' || s[i] == '#') f3 = true;

            pr[s[i]] ++;
        }

        bool flag = true;
        for (auto k : pr)
        {
            if (k.y > 2) 
            {
                flag = false;
                break;
            }
        }

        if (f1 && f2 && f3 && flag)
            puts("2");
        else if (f1 && f2 && f3)
            puts("1");
        else 
            puts("0");
    }
    return 0;
}
```

# Q2 字符串变换
## 算法思路
- 字符之间的替换函数使用哈希表 `pr` 存储
- 暴力模拟替换
- 时间复杂度 $O(m \times k \times 100 \times \log n)$, 其中 `m` 为测试查询数量, `k` 为变换次数, `n` 为变换字符对数
- 该题解 CSP 官网评测得分 80 分, 报错提示为 `TLE`
## 输入读取字符（串）那些事儿
- `std::getline(std::cin, str)` 从标准输入 `std::cin` 读取一行字符串（包括空格）到 `std::string str`，直到换行符或文件末尾为止
- `cin >> str` 输入数据，遇到空格、换行符等分隔符便会停止
- `scanf("%c", &ch)` 、`scanf("%s", str)` 读取单个字符，会读取任何字符，包括空格、换行符、制表符等。如果在读取前有输入的换行符，它会直接读取换行符，而不是用户期望的字符，此为导致输入问题的关键所在
- `cin.get()`、`getchar()` 可用来读取、丢弃换行符
## HINT
- `cin.get()` 用于读取并丢弃缓冲区中的第一个字符, 在这种情况下, 用于清除 `scanf` 读取整数后残留在输入流中的换行符
- 若不加 `cin.get()`, 换行符将留在输入流中, 导致 `getline()` 在第一次循环读取时读取的是换行符, 而不是输入的数据行, 现场评测时调试半天 >_<
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

using namespace std;
unordered_map<char, char> pr;

string str;
int n, m;

int main()
{
    getline(cin, str);

    scanf("%d", &n);
    cin.get();  // HINT POINT

    for (int i = 0; i < n; i ++)
    {
        string s;
        getline(cin, s);
        pr[s[1]] = s[2];
    }

    scanf("%d", &m);
    while (m --)
    {
        int x;
        scanf("%d", &x);

        string temp = str;
        for (int i = 0; i < x; i ++)
        {
            for (int j = 1; j <= temp.length() - 2; j ++)
            {
                if (pr.find(temp[j]) != pr.end())
                    temp[j] = pr[temp[j]];
            }
        }
        cout << temp << endl;
    }
    return 0;
}
```
## 算法思路
- 标记每个字符的替换循环节大小, 数据范围中变换次数 `k` 便可看出端倪
- `initReccuringTimes` 函数处理字符变换过程, 默认一定存在循环, 而不存在收敛(Hack)
- `reccuringTimes` 记录字符完成一整个轮回的变换次数, 该集合使用函数 `initReccuringTimes` 来维护
- 将时间复杂度中的 `k` 通过循环节处理缩小至 `n`(最坏情况遍历下, 轮回过程遍历所有的变换字符对)
- 时间复杂度：$O(m \times 100 \times n)$
- 该题解 CSP 官网评测得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

using namespace std;
unordered_map<char, char> pr;
unordered_map<char, int> recurringTimes;
string str;
int n, m;

void initReccuringTimes()
{
    char set[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
    for (int i = 0; i < 63; i ++)
    {
        char c = set[i];
        char tmp = set[i];
        int times = 0;
        bool isSearch = true;

        if (pr.find(tmp) != pr.end() && pr[tmp] == c) // #HH#
        {
            recurringTimes[c] = 0;
            isSearch = false;
            continue;
        }
        if (pr.find(tmp) == pr.end())
        {
            recurringTimes[c] = 0;
            isSearch = false;
            continue;
        }

        while (isSearch)
        {
            tmp = pr[tmp];
            times ++;
            if (tmp == c) 
            {
                recurringTimes[c] = times;
                break;
            }
        }
        
    }
}

int main()
{
    getline(cin, str);

    scanf("%d", &n);
    cin.get();
    for (int i = 0; i < n; i ++)
    {
        string s;
        getline(cin, s);
        pr[s[1]] = s[2];
    }

    initReccuringTimes();

    scanf("%d", &m);
    while (m --)
    {
        int x;
        scanf("%d", &x);

        string temp = str;
        
        for (int i = 1; i < temp.size() - 1; i ++)
        {
             int toggles = recurringTimes[temp[i]];
             int shiftTimes = x;
             if (toggles != 0) // 存在循环节
             {
                 shiftTimes %= toggles;
                 for (int j = 0; j < shiftTimes; j ++)
                 {
                    temp[i] = pr[temp[i]];
                 }
             }
             else // 不存在循环节
             {
                continue;
             }
        }
        cout << temp << endl;
    }
    return 0;
    
}
```

# Q3 补丁应用
## 算法思路
- 只考虑一个补丁的情况, 进行模拟(不考虑补丁损坏)
- `scanf` 读取原始文件行数后, `getline` 依次按行读取后续的输入数据
- `countLine` 标记已经成功存储到 `orignText` 的原始文件行数
- 读取到 `@@ * @@` 块时, 获取 `MM`, `mm` 值, 其中, `MM = sum(count(space), count(-))`, `mm = sum(count(space), count(+))`; 之后, 额外处理一行补丁, 此处先定位, 再处理 `space` 或 `-`(此处关于 `NN`, `nn` 的定位没咋看懂 >_<)
- 逐行处理补丁, 同时用 `checkMM`, `checkmm` 来维护已经处理过的 `space`, `-`, `+` 的个数, 一旦处理完毕后, 直接输出原始文本中的剩余数据即可
- 该题解 CSP 官网评测系统下得分为 20 分, 通过 (2/7) 的测试点, 评测报错提示 `Wrong Answer`
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
const int N = 2010;

int n, countLine;
string orignText[N];
string temp;

int countMM = 0, countmm = 0;
int idx = -1;
int checkMM = 0, checkmm = 0;

int main()
{
    scanf("%d", &n);
    cin.get();  

    while (getline(cin, temp))
    {
        if (countLine < n)  // 读取文本
            orignText[countLine ++] = temp;
        
        if (temp[0] == '@')
        {
            for (int i = 0; i < temp.size(); i ++) // 获取 MM mm 值
            {
                if (temp[i] == ',') // 锁定第一个逗号
                {
                    int j = i + 1;
                    while (temp[j] >= '0' && temp[j] <= '9')
                    {
                        countMM = countMM * 10 + temp[j] - '0';
                        j ++;
                    }
                    while (temp[j] != ',')  // 锁定第二个逗号
                    {
                        j ++;
                    }
                    j ++;
                    while (temp[j] >= '0' && temp[j] <= '9')
                    {
                        countmm = countmm * 10 + temp[j] - '0';
                        j ++;
                    }
                    break;
                }
            }
            
            // 定位, 确定 index 值
            string t;
            getline(cin, t);
            char op = t[0];
            t.erase(0, 1);
            for (int i = 0; i < countLine; i ++)
            {
                if (t != orignText[i])
                {
                    cout << orignText[i] << endl;
                }
                else 
                {
                    idx = i;
                    break;
                }
            }
            if (op == ' ')
            {
                cout << orignText[idx ++] << endl;
                checkMM ++, checkmm ++;
            }
            if (op == '-')
            {
                idx ++;
                checkMM ++;
            }
        }
        if (temp[0] == '+')
        {
            temp.erase(0, 1);
            cout << temp << endl;
            checkmm ++;
        }
        if (temp[0] == ' ')
        {
            cout << orignText[idx ++] << endl;
            checkMM ++, checkmm ++;
        }
        if (temp[0] == '-')
        {
            idx ++;
            checkMM ++;
        }

        if (checkMM == countMM && checkmm == countmm && countMM != 0 && countmm != 0)
        {
            for (int i = idx; i < countLine; i ++)
            {
                cout << orignText[i] << endl;
            }
        }
    }
    return 0;
}
```

# Q4 通讯延迟
## 算法思路
- 对于同一基站可以通信覆盖的节点之间建立无向边, 权值为通讯代价
- 在构建的无向图的节点之间的权值时, 遵循小代价为优的准则(通讯权值无向图使用邻接表维护)
- 采用 `Dijkstra` 算法获取 节点 `1` 到 节点 `n` 之间的最小通讯代价
- 读取基站数据并初始化建图过程的时间复杂度: $m \times (n + 20^2 \times X)$, 其中 `X` 为 `add(p1, p2, t)` 中与节点 `p1` 通信联通的节点数量(未知量), 题面中只是给定了每个通讯基站至多覆盖 20 个节点, 而一个节点可能被多个基站覆盖, 超时问题原因部分在此
- `Dijkstra` 的时间复杂度: $O(n \times max(n, X))$, 其中, `X` 的数据规模同上
- 该题解 CSP 官网评测系统下的得分 30 分, 通过 (6/20) 的测试点, 评测报错提示为 `TLE`
```C++
#include <iostream>
#include <cstring>
#include <cstring>
#include <unordered_set>
#include <queue>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 5010;

int n, m;
PII p[N];
int h[N], e[N * 2], ne[N * 2], idx, w[N * 2];
bool st[N][N];  // st[i][j] 表示 i j 之间可以连通

bool isValid(PII sp, int r, PII point)
{
    bool flag = false;
    if (point.x >= sp.x - r && point.x <= sp.x + r && point.y >= sp.y - r && point.y <= sp.y + r)
        flag = true;
    return flag;
}

void add(int p1, int p2, int t)
{
    if (st[p1][p2])
    {
        for (int i = h[p1]; ~i; i = ne[i])
        {
            int j = e[i];
            if (j == p2)
                w[i] = min(w[i], t);
        }
    }
    else
    {
        e[idx] = p2, w[idx] = t,  ne[idx] = h[p1], h[p1] = idx ++;
        st[p1][p2] = true;
    }
}

bool state[N];
int dist[N];

bool Dijkstra()
{
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;
    
    for (int i = 0; i < n; i ++)
    {
        int t = -1;
        for (int j = 1; j <= n; j ++)
        {
            if (!state[j] && (t == -1 || dist[j] < dist[t]))
                t = j;
        }

        state[t] = true;
        if (t == n) break;

        for (int j = h[t]; ~j; j = ne[j])
        {
            int k = e[j];
            dist[k] = min(dist[k], dist[t] + w[j]);
        }
    }

    if (dist[n] != 0x3f3f3f3f)
        return true;
    else
        return false;
}

int main()
{
    memset(h, -1, sizeof h);

    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++)
        scanf("%d%d", &p[i].x, &p[i].y);

    for (int i = 1; i <= m; i ++)    // 枚举基站
    {
        PII sp;
        int r, t;
        scanf("%d%d%d%d", &sp.x, &sp.y, &r, &t);
        unordered_set<int> temp;    // 该基站可以覆盖到的有效点
        for (int j = 1; j <= n; j ++)
            if (isValid(sp, r, p[j]))
                temp.insert(j);

        for (auto p1 : temp)
            for (auto p2 : temp)
            {
                if (p1 == p2) continue;
                add(p1, p2, t), add(p2, p1, t);
            }
    }

    if (Dijkstra())
        printf("%d\n", dist[n]);
    else
        puts("Nan");
    return 0;
}
```
## 存储方式优化
- 图的存储方式进行优化(邻接表 --> 邻接矩阵)
- 枚举读取基站数据并初始化建图过程的时间复杂度: $O(m \times (n + 20^2))$
- `Dijkstra` 的时间复杂度: $O(n^2)$
- 该题解 CSP 官网评测系统下的得分为 60 分, 通过 (12/20) 的测试点, 评测报错提示为 `TLE`
```C++
#include <iostream>
#include <cstring>
#include <cstring>
#include <unordered_set>
#include <queue>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 5010;

int n, m;
PII p[N];
int g[N][N];
bool st[N][N];  // st[i][j] 表示 i j 之间可以连通

bool isValid(PII sp, int r, PII point)
{
    bool flag = false;
    if (point.x >= sp.x - r && point.x <= sp.x + r && point.y >= sp.y - r && point.y <= sp.y + r)
        flag = true;
    return flag;
}

void add(int p1, int p2, int t)
{
    if (st[p1][p2]) 
        g[p1][p2] = min(g[p1][p2], t);
    else
        g[p1][p2] = t, st[p1][p2] = true;
}

bool state[N];
int dist[N];

bool Dijkstra()
{
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;
    
    for (int i = 0; i < n; i ++)
    {
        int t = -1;
        for (int j = 1; j <= n; j ++)
        {
            if (!state[j] && (t == -1 || dist[j] < dist[t]))
                t = j;
        }

        state[t] = true;
        if (t == n) break;

        for (int j = 1; j <= n; j ++)
        {
            if (!st[t][j]) continue;
            dist[j] = min(dist[j], dist[t] + g[t][j]);
        }
    }

    if (dist[n] != 0x3f3f3f3f)
        return true;
    else
        return false;
}

int main()
{
    memset(g, -1, sizeof g);

    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++)
        scanf("%d%d", &p[i].x, &p[i].y);

    for (int i = 1; i <= m; i ++)    // 枚举基站
    {
        PII sp;
        int r, t;
        scanf("%d%d%d%d", &sp.x, &sp.y, &r, &t);
        unordered_set<int> temp;    // 该基站可以覆盖到的有效点
        for (int j = 1; j <= n; j ++)
            if (isValid(sp, r, p[j]))
                temp.insert(j);

        for (auto p1 : temp)
            for (auto p2 : temp)
            {
                if (p1 == p2) continue;
                add(p1, p2, t), add(p2, p1, t);
            }
    }

    if (Dijkstra())
        printf("%d\n", dist[n]);
    else
        puts("Nan");
    return 0;
}
```

# Q5 木板切割
## 算法思路
- 朴素模拟
- `set<int> p[i]` 用于存储 `i` 号木板所含的木板段编号
- `st[i]` 用于在每次询问计算输出值时, 标记 `i` 号颜色值是否存在
- `cnt1` 是切下木板中的不同颜色数, `cnt2` 是切下木板中的颜色段数
- 该题解 CSP 官网评测系统下得分为 35 分, 可以通过 (7/20) 的数据点, 评测报错提示为 `TLE`
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <set>

using namespace std;
const int N = 1e+5 + 10;

int n, m, k;
int c[N];

set<int> p[N];
bool st[N];

int main()
{
    scanf("%d%d%d",  &n, &m, &k);
    for (int i = 1; i <= n; i ++)
        scanf("%d", &c[i]);
    
    for (int i = 1; i <= n; i ++)
        p[1].insert(i);

    int cntID = 2;
    while (k --)
    {
        int id, l, r;
        scanf("%d%d%d", &id, &l, &r);

        // 拆分
        set<int> s,  temp;
        for (auto cs : p[id])
        {
            if (cs >= l && cs <= r)
                s.insert(cs);
            else
                temp.insert(cs);
        }

        p[id] = temp;
        p[cntID] = s;

        int cnt1  = 0, cnt2 = 0;
        memset(st, 0, sizeof st);

        int preColor = -1;
        for (auto cs : p[cntID])
        {
            st[c[cs]] = true;
            if (c[cs] != preColor)
            {
                cnt2 ++;
                preColor = c[cs];
            }
        }

        for (int i = 1; i <= m; i ++)
            if (st[i])
                cnt1 ++;
        
        cntID ++;
        printf("%d %d\n", cnt1, cnt2);
    }
    return 0;
}
```