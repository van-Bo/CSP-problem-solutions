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
