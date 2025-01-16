> CSP(第35次CCF计算机软件能力认证)
> 正式认证(100 + 80 + 0 + 30 + 35)
> 模拟认证(100 + 100 + )

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

# Q4 通讯延迟
## 算法思路
- 对于同一基站可以通信覆盖的节点之间建立无向边, 权值为通讯代价
- 在构建的无向图的节点之间的权值时, 遵循小代价为优的准则(通讯权值无向图使用邻接表维护)
- 采用 `Dijkstra` 算法获取 节点 `1` 到 节点 `n` 之间的最小通讯代价
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