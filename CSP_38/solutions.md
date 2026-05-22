# 第38次CCF计算机软件能力认证

> smqyOJ Judge((10/10) + (20/20) + )

## Q1 正态分布

### Q1 算法思路(demo1, WA)

- 由于计算机底层(IEEE 754 标准)表示浮点数时存在精度误差，这导致了如下的题解在某些数据下被"暗算"
- 错误原因：浮点数精度与向下取整的冲突在 `C++` 中，`double` 类型并不能精确表示所有的纯小数。例如，当 $z - x = 4, y = 25$ 时，`baseline = 0.16`。在计算机内部，它可能会被表示为 `0.1599999999999999`。当你执行强制类型转换 `(int)` 时，`C++` 的规则是直接抹掉小数部分(向下取整)，而不是四舍五入！
  - `(int)(baseline * 100)` $\rightarrow$ `(int)(15.99999...)` $\rightarrow$ `res = 15`，而不是我们期望的 `16`。这就导致算出来的列数 `col` 比正确答案少 1，从而报 `WA`(答案错误)。
- 该题解可以通过 smqyOJ (7/10) 的数据点，得分 70 分
  
### Q1 代码实现(demo1, WA)

```cpp
#include <bits/stdc++.h>
using namespace std;

int k;

int main()
{
    cin >> k;
    for (int i = 0; i < k; i ++)
    {
        double x, y, z;
        cin >> x >> y >> z;
        double baseline = (z - x) / y;

        int row = (int)(baseline * 10) + 1;
        int col = (int)(baseline * 100) - (int)(baseline * 10) * 10 + 1;
        printf("%d %d\n", row, col);
    }
    return 0;
}
```

### Q1 算法思路(demo2, AC)

- 在 `Q1 demo1` 算法思路所给定的反例中，由于浮点数误差通常是使得原本的 $0.16$ 变成了 $0.15999999...$，加上一个极小的 $\epsilon$（如 $1e-8$）后变成了 $0.16000001...$，此时再向下取整就能完美得到 16。这在算法竞赛中被称为防御性编程。
- 使用浮点数且要使用 `(int)` 截断时，常见的补救做法是加一个极小的常数(例如 `(int)(baseline * 100 + 1e-8)`)，利用四舍五入的机制去弥补那 `0.000000001` 的精度缺失。不过这题用纯整数算才是最优雅的官方正解，见如下题解 `Q1 demo3`
- 该题解可以通过 smqyOJ (10/10) 的数据点，得分 100 分

### Q1 代码实现(demo2, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;

int k;

int main()
{
    cin >> k;
    for (int i = 0; i < k; i ++)
    {
        double x, y, z;
        cin >> x >> y >> z;
        double baseline = (z - x) / y + 1e-8;

        int row = (int)(baseline * 10) + 1;
        int col = (int)(baseline * 100) - (int)(baseline * 10) * 10 + 1;
        printf("%d %d\n", row, col);
    }
    return 0;
}
```

### Q1 算法思路(demo3, AC)

- 破局点：题目给的隐藏提示中，子任务约束条件中有一句非常突兀的话："标准差 $\sigma$ 是 $100$ 的因子"，这绝对不是一句废话！
- 既然 $\sigma$ (`y`) 是 100 的因子，这就意味着：$ \frac{(n - \mu)}{\sigma} \times 100 = \frac{(n - \mu) \times 100}{\sigma} $ 这个结果一定是一个完美的整数！绝对不会有小数！利用这一点，我们可以全程使用整数运算，彻底抛弃 `double`，从根源上消灭浮点数精度误差
- 该题解可以通过 smqyOJ (10/10) 的数据点，得分 100 分

### Q1 代码实现(demo3, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;

int k;

int main()
{
    cin >> k;
    for (int i = 0; i < k; i ++)
    {
        int x, y, z;
        cin >> x >> y >> z;
        int baseline = (z - x) * 100 / y ;

        int row = baseline / 10 + 1;
        int col = baseline % 10 + 1;
        printf("%d %d\n", row, col);
    }
    return 0;
}
```

### Q1 算法思路(demo4, AC)

- 使用 `double` 搭配标准库 `round()`
- 标准库里的 `round()` 函数专门用于浮点数的四舍五入操作，在 `C++` 的 `<cmath>` 库中，`round()` 函数的返回值其实依然是浮点数类型(`double` 等)，所以外面嵌套一层 `(int)` 进行显式类型转换是非常严谨且必要的。
- 该题解可以通过 smqyOJ (10/10) 的数据点，得分 100 分

### Q1 代码实现(demo4, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;

int k;

int main()
{
    cin >> k;
    for (int i = 0; i < k; i ++)
    {
        double x, y, z;
        cin >> x >> y >> z;
        double baseline = (z - x) / y ;
        int baseline_100 = (int)round(baseline * 100);
        
        int row = baseline_100 / 10 + 1;
        int col = baseline_100 % 10 + 1;
        printf("%d %d\n", row, col);
    }
    return 0;
}
```

## Q2 机器人复健指南

### Q2 算法思路(demo1, WA)

- 使用深度优先搜索(DFS) 配合简单的 `bool` 访问数组求解，虽然能通过题面中给定的简单测试样例，但在更复杂的隐藏案例中会得到错误答案。这里踩到了一个在算法竞赛中非常经典的坑：使用纯粹的 DFS(深度优先搜索)配合简单的 `bool` 访问数组来求最短可达步数
- DFS 逻辑下的错因:
  - 问题出在`if (st[newX][newY]) continue;`
  - 在深度优先搜索中，一旦我们访问过某个方格，就会把它标记为 `true`，防止无限死循环。但在"限定步数可达性"的问题中，这会导致一条绕远的路径，堵死了一条更优的路径。
- 举个具体的例子：
  - 假设要去方格 `C`，步数限制 `k = 3`
  - DFS 首先沿着一条较长的路径探测，花了 3 步到达 `C`。此时 `st[C] = true`。因为到达了步数上限，DFS 停止从 `C` 继续向外扩展。
  - 随后的 DFS 回溯过程中，沿着另一条捷径探测，仅仅花了 1 步就再次来到了 `C`。
  - 按照常理，现在还剩 2 步，完全可以从 `C` 出发继续探索周围的方格。但是，由于 `C` 已经被之前的长路径标记为 `true`，程序会直接 `continue` 跳过它，这就导致原本在步数限制内可以到达的方格，被错误地忽略了！
- 该题解可以通过 smqyOJ (2/20) 的数据点，得分 0 分

### Q2 代码实现(demo1, WA)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 110;
typedef pair<int, int> PII;
#define x first
#define y second
int n, k, p, q;
bool st[N][N];

PII direction[] = {
    {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, 
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}   
};

void search(PII point, int step)
{
    if (step > k) return;
    int u = point.x, v = point.y;
    st[u][v] = true;

    for (int i = 0; i < 8; i ++)
    {
        int dx = direction[i].x, dy = direction[i].y;
        int newX = u + dx, newY = v + dy;

        if (newX < 0 || newX >= n || newY < 0 || newY >= n) continue;   // 越界
        if (st[newX][newY]) continue;   // 已访问过 (Bug Point)
        search({newX, newY}, step + 1);
    }
}

int main()
{
    cin >> n >> k >> p >> q;

    search({p - 1, q - 1}, 0);
    int cnt = 0;
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < n; j ++)
            if (st[i][j]) 
                cnt ++;
    cout << cnt << endl;

    return 0;
}
```

### Q2 算法思路(demo2, AC)

- 使用广度优先搜索(BFS)来求解最短可达步数问题。BFS 的核心思想是层次遍历，能够保证在访问一个方格时，已经以最少的步数到达了它。这样就不会出现 DFS 中的"过早标记访问"问题，因为 BFS 是按照步数层层推进的
- 在 BFS 中，为了保证每个节点绝对只进队一次，必须在将它 `push` 进队列的瞬间，立刻将其标记为已访问 `st[point.x][point.y] = true`。
- 该题解可以通过 smqyOJ (20/20) 的数据点，得分 100 分

### Q2 代码实现(demo2, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 110;
typedef pair<int, int> PII;
#define x first
#define y second
int n, k, p, q;
bool st[N][N];
int dist[N][N];

PII direction[] = {
    {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, 
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}   
};

void search(PII point)
{
    queue<PII> bfs_queue;
    bfs_queue.push(point);
    dist[point.x][point.y] = 0;
    st[point.x][point.y] = true;

    while (bfs_queue.size())
    {
        PII t = bfs_queue.front();
        bfs_queue.pop();

        if (dist[t.x][t.y] == k) continue;  // 停止向队列中纳入新的点(后续的点 dist 值均大于 k)

        for (int i = 0; i < 8; i ++)
        {
            int dx = direction[i].x, dy = direction[i].y;
            int u = dx + t.x, v = dy + t.y;
            if (u < 0 || u >= n || v < 0 || v >= n) continue;   // 越界
            if (!st[u][v]) 
            {
                dist[u][v] = dist[t.x][t.y] + 1;
                st[u][v] = true;
                bfs_queue.push({u, v});
            }
        }
    }
}

int main()
{
    cin >> n >> k >> p >> q;

    search({p - 1, q - 1});
    int cnt = 0;
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < n; j ++)
            if (st[i][j]) 
                cnt ++;
    cout << cnt << endl;

    return 0;
}
```

## Q3 消息解码

### Q3 算法思路(demo1, subtask-80%, TLE)

- `deque<pus> nums` 用于存储历史消息信息，每个元素的意义为 `<代号的数字表示，代号>`
- 所有的代号均有数字表示，散列值便是根据数字表示计算出来的，此外，典型代号还有短数字表示
- `getHash` 函数用于计算数字表示所对应的 `k` 位散列值，注意类型的数据溢出(使用 `unsigned __int128`)，乘除 $2^n$ 可以采用移位运算，取模操作可以通过 `&` 操作等效
- `deal0` 中对于接收方代号字段、发送方代号字段的是否为散列值的判断，直接通过与 $2^{25}$ 的比较来实现。只比对字段中从低位开始的 `25` 号位 `x[25]` 是否为 `1` ，此处存在 `Bug`
  - 在 `deal0` 中，通过 `if (rcv[2] == '0')` 来区分散列值。以为短数字加上 $2^{25}$ 就只会改变那一个比特位，所以直接检查那个位就行了。
  - 大错特错！ 短数字本身最大可达 $\approx 2.68 \times 10^8$，远远超过了 $2^{25} \approx 3.35 \times 10^7$。因此它不仅仅会影响第 25 位，还会向高位进位。
- 确定好本次输出的代号情况之后，才对 `nums` 进行填充操作，防止对输出代号的确定操作造成干扰
- 该题解可以通过 smqyOJ (8/10) 的数据点，得分 80 分

### Q3 代码实现(demo1, subtask-80%, TLE)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
typedef pair<ull, string> pus;
#define x first
#define y second
int n;

deque<pus> nums;    // <代号的数字表示，代号>

ull getHash(ull v, int k)   // 获取数字表示 v 的 k 位散列值
{
    unsigned __int128 prod = (unsigned __int128)v * 47055833459ull;
    unsigned __int128 q = prod >> (64 - k);
    return (ull)(q & ((1ull << k) - 1));
}

string getNameFromShortNum(ull v)   // 从短数字表示转换为代号
{
    string res;
    ull mark;
    for (int i = 6; i >= 4; i --)   // 处理第 6~4 个字符
    {
        res = (char)('A' + (v % 26)) + res;
        v /= 26;
    }
    // 处理第 3 个字符
    res = (char)('0' + (v % 10)) + res;
    v /= 10;
    // 处理第 2 个字符
    mark = v % 36;
    if (mark >= 0 && mark <= 9) res = (char)('0' + mark) + res;
    else res = (char)('A' + mark - 10) + res;
    v /= 36;
    // 处理第 1 个字符
    mark = v % 37;
    if (mark >= 1 && mark <= 10) res = (char)('0' + mark - 1) + res;
    else if (mark >= 11 && mark <= 36) res = (char)('A' + mark - 11) + res;

    return res;
}

string getNameFromLongNum(ull v)    // 从长数字表示转换为代号
{
    string res;
    ull mark;
    for (int i = 11; i >= 1; i --)
    {
        mark = v % 38;
        if (mark == 0) 
        {
            v /= 38;
            continue;
        }
        else if (mark == 37) res = '_' + res;
        else if (mark >= 1 && mark <= 10) res = (char)('0' + mark - 1) + res;
        else res = (char)('A' + mark - 11) + res;
        v /= 38;
    }
    return res;
}

string hashToNum(ull v, int k)  // 通过 k 位散列值 v 检查历史信息，以获取代号
{
    bool flag = false;
    string res = "";
    int len = nums.size();
    for (int i = 0; i < len; i ++)
    {
        if (getHash(nums[i].x, k) == v)
        {
            flag = true;
            res = nums[i].y;
            break;
        }
    }
    if (flag) return "#" + res;
    else return "###";
}

ull getLongNumFromName(string s)
{
    ull res = 0;
    int len = s.length();
    if (len < 11)   // 空格填充，保证长度达到 11 位 
    {
        for (int i = 0; i < 11 - len; i ++) s += ' ';
    }

    for (int i = 0; i < 11; i ++)
    {
        res *= 38;
        if (s[i] == ' ') res += 0;
        else if (s[i] == '_') res += 37;
        else if (s[i] >= '0' && s[i] <= '9') res += s[i] - '0' + 1;
        else res += s[i] - 'A' + 11;
    }
    return res;
}

void deal1(string s)
{
    string n1 = s.substr(1, 58), n2 = s.substr(59, 12), state = s.substr(71, 1);

    string res1, res2, res3;
    ull v1, v2;
    v1 = stoull(n1, nullptr, 2);
    res1 = getNameFromLongNum(v1);

    v2 = stoull(n2, nullptr, 2);
    res2 = hashToNum(v2, 12);

    nums.push_front({v1, res1});

    if (state == "1")
        cout << res1 << " " << res2 << "\n";
    else
        cout << res2 << " " << res1 << "\n";
}

void deal0(string s)
{
    string rcv = s.substr(1, 28), snd = s.substr(29, 28), loc = s.substr(57, 15);

    string res1, res2, res3;
    ull v1 = stoull(rcv, nullptr, 2), v2 = stoull(snd, nullptr, 2);
    bool flag1 = false, flag2 = false;  // 标记是否为短数字表示

    if (v1 < (1ull << 25))  // rcv 为 25 位散列值
        res1 = hashToNum(v1, 25);
    else    // rcv 为 典型代号的短数字表示 
    {
        flag1 = true;
        v1 -= 1ull << 25;
        res1 = getNameFromShortNum(v1);
        v1 = getLongNumFromName(res1);
    }

    if (v2 < (1ull << 25))  // rcv 为 25 位散列值
        res2 = hashToNum(v2, 25);
    else    // rcv 为 典型代号的短数字表示 
    {
        flag2 = true;
        v2 -= 1ull << 25;
        res2 = getNameFromShortNum(v2);
        v2 = getLongNumFromName(res2);
    }

    ull locNum = stoull(loc, nullptr, 2);
    if (locNum == 0) res3 = "";
    else res3 = to_string(locNum);

    if (flag1) nums.push_front({v1, res1});
    if (flag2) nums.push_front({v2, res2});

    if (res3 == "")
        cout << res1 << " " << res2 << "\n";
    else
        cout << res1 << " " << res2 << " " << res3 << "\n";
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i ++)
    {
        string s;
        cin >> s;
        if (s[0] == '0') deal0(s);
        else deal1(s);
    }
    return 0;
}
```

### Q3 算法思路(demo2, AC)

- `TLE` 的罪魁祸首是 `hashToNum` 函数以及用于存储历史代号的 `deque<pus> nums`

```cpp
string hashToNum(ull v, int k) {
    bool flag = false;
    string res = "";
    int len = nums.size();
    for (int i = 0; i < len; i ++) {
        if (getHash(nums[i].x, k) == v) { ... }
    }
}
```

- **时间复杂度爆炸**：测试点 9 和 10 的数据规模是 $N = 10^5$。在最坏情况下(比如大量散列值匹配失败)，每收到一条散列值消息，都要遍历一遍长度可能接近 $10^5$ 的 `nums` 队列，并在循环内进行极耗时的 `__int128` 乘法。$10^5 \times 10^5 = 10^{10}$ 次运算。这在 `C++` 中，必定 `TLE`！
- **破局方案**：空间换时间(哈希表)，既然散列的位数只有两种情况(12 位 和 25 位)，完全可以把每次新得到的确切代号，提前算出它的 12 位散列值和 25 位散列值，并把它们存入哈希表(`std::unordered_map`)中。这样一来，查询的时间复杂度就会从 $O(N)$ 暴降到 $O(1)$。
- 更绝妙的是，使用哈希表天然完美契合题目的两种"冲突处理"规则：
  - "如果有多个代号散列值符合，使用最后收到的"：在 `map[hash_val] = name` 时，新的记录会直接覆盖掉旧的记录，自然保留的就是"最后收到的"。
  - "如果同一条消息中收发双方的散列值都符合，使用发送方的"：只需要在更新哈希表时，先更新接收方，再更新发送方。这样如果二者冲突，发送方就会把接收方覆盖掉。
- 该题解可以通过 smqyOJ (10/10) 的数据点，得分 100 分

### Q3 代码实现(demo2, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
typedef pair<ull, string> pus;
#define x first
#define y second
int n;

unordered_map<ull, string> map25;
unordered_map<ull, string> map12;

ull getHash(ull v, int k)   // 获取数字表示 v 的 k 位散列值
{
    unsigned __int128 prod = (unsigned __int128)v * 47055833459ull;
    unsigned __int128 q = prod >> (64 - k);
    return (ull)(q & ((1ull << k) - 1));
}

string getNameFromShortNum(ull v)
{
    string res;
    ull mark;
    for (int i = 6; i >= 4; i --)   // 处理第 6~4 个字符
    {
        res = (char)('A' + (v % 26)) + res;
        v /= 26;
    }
    // 处理第 3 个字符
    res = (char)('0' + (v % 10)) + res;
    v /= 10;
    // 处理第 2 个字符
    mark = v % 36;
    if (mark >= 0 && mark <= 9) res = (char)('0' + mark) + res;
    else res = (char)('A' + mark - 10) + res;
    v /= 36;
    // 处理第 1 个字符
    mark = v % 37;
    if (mark >= 1 && mark <= 10) res = (char)('0' + mark - 1) + res;
    else if (mark >= 11 && mark <= 36) res = (char)('A' + mark - 11) + res;

    return res;
}

string getNameFromLongNum(ull v)
{
    string res;
    ull mark;
    for (int i = 11; i >= 1; i --)
    {
        mark = v % 38;
        if (mark == 0) 
        {
            v /= 38;
            continue;
        }
        else if (mark == 37) res = '_' + res;
        else if (mark >= 1 && mark <= 10) res = (char)('0' + mark - 1) + res;
        else res = (char)('A' + mark - 11) + res;
        v /= 38;
    }
    return res;
}

string hashToNum(ull v, int k)  // 根据 k 位散列值 v 获取代号
{
    if (k == 25)
    {
        if (map25.count(v)) return "#" + map25[v];
    }
    else if (k == 12)
    {
        if (map12.count(v)) return "#" + map12[v];
    }
    return "###";
}

ull getLongNumFromName(string s)
{
    ull res = 0;
    int len = s.length();
    if (len < 11)   // 空格填充，保证长度达到 11 位 
    {
        for (int i = 0; i < 11 - len; i ++) s += ' ';
    }

    for (int i = 0; i < 11; i ++)
    {
        res *= 38;
        if (s[i] == ' ') res += 0;
        else if (s[i] == '_') res += 37;
        else if (s[i] >= '0' && s[i] <= '9') res += s[i] - '0' + 1;
        else res += s[i] - 'A' + 11;
    }
    return res;
}

void add_history(ull v, string s)   // 代号的数字表示 k，代号 s 存入历史记录
{
    map25[getHash(v, 25)] = s;
    map12[getHash(v, 12)] = s;
}

void deal1(string s)
{
    string n1 = s.substr(1, 58), n2 = s.substr(59, 12), state = s.substr(71, 1);

    string res1, res2, res3;
    ull v1, v2;
    v1 = stoull(n1, nullptr, 2);
    res1 = getNameFromLongNum(v1);

    v2 = stoull(n2, nullptr, 2);
    res2 = hashToNum(v2, 12);

    add_history(v1, res1);

    if (state == "1")
        cout << res1 << " " << res2 << "\n";
    else
        cout << res2 << " " << res1 << "\n";
}

void deal0(string s)
{
    string rcv = s.substr(1, 28), snd = s.substr(29, 28), loc = s.substr(57, 15);

    string res1, res2, res3;
    ull v1 = stoull(rcv, nullptr, 2), v2 = stoull(snd, nullptr, 2);
    bool flag1 = false, flag2 = false;  // 标记是否为短数字表示

    if (v1 < (1ull << 25))  // rcv 为 25 位散列值
        res1 = hashToNum(v1, 25);
    else    // rcv 为 典型代号的短数字表示 
    {
        flag1 = true;
        v1 -= 1ull << 25;
        res1 = getNameFromShortNum(v1);
        v1 = getLongNumFromName(res1);
    }

    if (v2 < (1ull << 25))  // rcv 为 25 位散列值
        res2 = hashToNum(v2, 25);
    else    // rcv 为 典型代号的短数字表示 
    {
        flag2 = true;
        v2 -= 1ull << 25;
        res2 = getNameFromShortNum(v2);
        v2 = getLongNumFromName(res2);
    }

    ull locNum = stoull(loc, nullptr, 2);
    if (locNum == 0) res3 = "";
    else res3 = to_string(locNum);

    if (flag1) add_history(v1, res1);
    if (flag2) add_history(v2, res2);

    if (res3 == "")
        cout << res1 << " " << res2 << "\n";
    else
        cout << res1 << " " << res2 << " " << res3 << "\n";
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i ++)
    {
        string s;
        cin >> s;
        if (s[0] == '0') deal0(s);
        else deal1(s);
    }
    return 0;
}
```
