# CSP(第39次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + (20/20) + ())

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

## Q2 水印检查

### Q2 算法思路(demo1, subtask-80%)

- 朴素模拟
- 该题解可以通过 smqyOJ (16/20) 的测试点，得分 80 分

### Q2 代码实现(demo1, subtask-80%)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 210;

int n, L;
int g[N][N];
bool t[N][N];
bool st[5][9];

bool match(int x, int y)    // 匹配以左上角坐标为锚点的图像
{
    for (int i = 0; i < 5; i ++)
    {
        for (int j = 0; j < 9; j ++)
        {
            if (t[x + i][y + j] != st[i][j]) return false;
        }
    }
    return true;
}

bool check(int k)
{
    // 根据 k 将图像二值化
    for (int i = 0; i < n; i ++)
    {
        for (int j = 0; j < n; j ++)
        {
            if (g[i][j] >= k) t[i][j] = true;
            else t[i][j] = false;
        }
    }

    // 遍历图像，寻找水印
    for (int i = 0; i <= n - 5; i ++)
    {
        for (int j = 0; j <= n - 9; j ++)
        {
            bool flag = match(i, j);
            if (flag) return true;
        }
    }
    return false;
}

int main()
{
    cin >> n >> L;
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < n; j ++)
            scanf("%d", &g[i][j]);
    
    // 初始化水印
    memset(st, true, sizeof st);    // >=k 标记白色 true
    st[1][1] = st[1][2] = st[1][4] = st[1][5] = st[1][7] = false;
    st[2][1] = st[2][2] = st[2][8] = false;
    st[3][1] = st[3][2] = st[3][3] = st[3][4] = st[3][7] = st[3][8] = false;
    st[4][7] = st[4][8] = false;

    for (int i = 0; i < L; i ++)
    {
        bool flag = check(i);
        if (flag) printf("%d\n", i);
    }

    return 0;
}
```

### Q2 算法思路(demo2, AC)

- 对于每一个 $5 \times 9$ 的子区域，按照期待白色像素、期待黑色像素的要求，分别枚举计算出该子区域的存在水印条件下，`k` 的取值范围 `[blackK, whiteK]`，其中 `whiteK` 是该子区域中所有期待为白色的像素值的最小值，`blackK` 是该子区域中所有期待为黑色的像素值的最大值加一，$ g_{black} < k \leqslant g_{white}$
- 有些子区域的存在水印条件是无解的，即 `blackK > whiteK`，直接跳过
- 有些子区域可能存在相同的备选解，例如一大堆子区域都可能以 `[0, L - 1]` 作为备选解，重复统计会让复杂度爆炸
- 使用 ``std::set`` 来维护**尚未成为备选解的数字**，初始时将所有的 `k` 都加入到集合中，每次剔除掉一个子区域的存在水印条件下的备选解区间 `[blackK, whiteK]`(从 `set` 中删除掉对应区间查到的迭代器即可)，最后剩下的就是所有满足条件的 `k`
- 该题解可以通过 smqyOJ (20/20) 的测试点，得分 100 分

### `std::set` 区间快速删除

1. 初始化全集 `for (int i = 0; i < L; ++i) not_ans.insert(i);`
    一开始，把所有可能的答案 candidate（$0$ 到 $L-1$）全部放进了 `not_ans` 集合中，此时集合是满的
2. 定位区间边界 (lower_bound) `std::set<int>::iterator st = not_ans.lower_bound(blackK), ed = not_ans.lower_bound(whiteK + 1);`
    当计算出当前窗口的 blackK 和 whiteK，并且满足 whiteK >= blackK 时，说明区间 [blackK, whiteK] 内的数字都有可能是答案。这里用到了 `set` 的两个极其重要的方法：`lower_bound(val)`：返回集合中第一个大于或等于 `val` 的元素的迭代器。`st` 获取了刚好 $\ge blackK$ 的起点位置。`ed` 获取了刚好 $\ge whiteK + 1$ 的终点位置。
3. 区间批量删除 (erase 的重载) `not_ans.erase(st, ed);`
    多数情况是传一个值给 `erase`（比如 `set.erase(5)`），但这里调用的是迭代器区间删除版本 `erase(iterator first, iterator last)`。它的作用是删除区间 `[st, ed)` 内的所有元素。结合上面，这段代码的作用：在集合中，把所有处于 $blackK$ 到 $whiteK$ 之间的数字一次性全部删掉！

### Q2 代码实现(demo2, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> PII;
#define x first
#define y second
const int N = 210;

int n, L;
int g[N][N];
bool st[5][9];
set<int> not_ans;

PII match(int x, int y)    // 匹配以左上角坐标为锚点的图像
{
    int whiteK = L, blackK = -1;
    for (int i = 0; i < 5; i ++)
    {
        for (int j = 0; j < 9; j ++)
        {
            if (st[i][j] == true)   // 期待为 white
                whiteK = min(whiteK, g[x + i][y + j]);
            else    // 期待为 black
                blackK = max(blackK, g[x + i][y + j] + 1);
        }
    }
    return {blackK, whiteK};
}

void check()
{
    for (int i = 0; i <= n - 5; i ++)
    {
        for (int j = 0; j <= n - 9; j ++)
        {
            PII flag = match(i, j);
            if (flag.x > flag.y) continue;
            set<int>::iterator st = not_ans.lower_bound(flag.x), ed = not_ans.lower_bound(flag.y + 1);
            not_ans.erase(st, ed);  // 剔除区间 [st, ed)
        }
    }
}

int main()
{
    cin >> n >> L;
    for (int i = 0; i < n; i ++)
        for (int j = 0; j < n; j ++)
            scanf("%d", &g[i][j]);
    
    // 初始化非答案
    for (int i = 0; i < L; i ++) not_ans.insert(i);

    // 初始化水印
    memset(st, true, sizeof st);    // >=k 标记白色 true
    st[1][1] = st[1][2] = st[1][4] = st[1][5] = st[1][7] = false;
    st[2][1] = st[2][2] = st[2][8] = false;
    st[3][1] = st[3][2] = st[3][3] = st[3][4] = st[3][7] = st[3][8] = false;
    st[4][7] = st[4][8] = false;

    check();

    for (int i = 0; i < L; i ++)
        if (!not_ans.count(i))
            printf("%d\n", i);
    return 0;
}
```