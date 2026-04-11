# CSP(第40次CCF计算机软件能力认证)

> smqyOJ Judge((12/12) + (23/23) + 20 + 60 + 35)

## Q1 集合

### Q1 算法思路

- 模拟即可

### Q1 代码实现

```C++
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+4 + 10;
const int LEN = 1e+3 + 10;

int n, m;
int a[N];
int s[105][LEN], t[105][LEN];

bool work(int x)
{
    bool real = true, pre = true;

    if (s[x][0] != t[x][0]) real = false;
    else
    {
        for (int i = 1; i <= s[x][0]; i++)
            if (s[x][i] != t[x][i]) {real = false; break;}
    }

    int ress = 0, rest = 0;
    for (int i = 1; i <= s[x][0]; i ++)
        ress ^= a[s[x][i]];
    for (int i = 1; i <= t[x][0]; i ++)
        rest ^= a[t[x][i]];
    if (ress != rest) pre = false;

    return real == pre;
}

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i ++) cin >> a[i];
    for (int i = 1; i <= m; i ++)
    {
        cin >> s[i][0];
        for (int j = 1; j <= s[i][0]; j ++) cin >> s[i][j];
    }
    for (int i = 1; i <= m; i ++)
    {
        cin >> t[i][0];
        for (int j = 1; j <= t[i][0]; j ++) cin >> t[i][j];
    }

    for (int i = 1; i <= m; i ++)
    {
        if (work(i)) cout << "correct\n";
        else cout << "wrong\n";
    }
    return 0;
}
```

## Q2 数字变换

### Q2 算法思路(demo1, TLE)

- 注意输出格式为 **一行**
- 逆向变换，该题解可以通过 smqyOJ (18/23) 的数据点

### Q2 代码实现(demo1, TLE)

```C++
#include <bits/stdc++.h>
using namespace std;
const int M = 1e+3 + 10;

int n, m;
int k[M];

int fxk(int x, int k)
{
    return ((x * x + k * k) % 8) ^ k;
}

int work(int x)
{
    int high, mid, low;
    for (int i = m; i >= 1; i --)
    {
        low = x % 8, mid = x / 8 % 8, high = x / 64 % 8;
        
        int fbk = fxk(high, k[i]);
        int c = mid ^ fbk;
        int fck = fxk(c, k[i]);
        int a = low ^ fck;

        x = a * 64 + high * 8 + c;
    }
    return x;
}

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= m; i ++) cin >> k[i];
    for (int i = 1; i <= n; i ++)
    {
        int x;
        cin >> x;
        int res = work(x);
        cout << res << " ";
    }
    return 0;
}
```

### Q2 算法思路(demo2, AC)

- 逆向变换，**范围查表优化**: 所有数均为 9 位整数，取值范围 $0 \sim 511$，总状态数仅 $512$ 种。预计算所有值的逆变换结果，查询时直接查表，避免重复计算。
- **复杂度**:
  - 预处理时间复杂度: $O(512 \times m)$
  - 查询时间复杂度: $O(n)$
  - 总时间复杂度: $O(512 \times m + n)$
  - 空间复杂度: $O(512)$
- **总结**: 利用变换的可逆性与状态空间有限的特点，将多次逆变换合并为一次查表，实现高效求解。
- 该题解可以通过 smqyOJ (23/23) 的数据点

### Q2 代码实现(demo2, AC)

```C++
#include <bits/stdc++.h>
using namespace std;
const int M = 1e+3 + 10;

int n, m;
int k[M];
int res[512]; // 范围查表

int fxk(int x, int k)
{
    return ((x * x + k * k) % 8) ^ k;
}

int work(int x)
{
    int high, mid, low;
    for (int i = m; i >= 1; i --)
    {
        low = x % 8, mid = x / 8 % 8, high = x / 64 % 8;
        
        int fbk = fxk(high, k[i]);
        int c = mid ^ fbk;
        int fck = fxk(c, k[i]);
        int a = low ^ fck;

        x = a * 64 + high * 8 + c;
    }
    return x;
}

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= m; i ++) cin >> k[i];
    for (int i = 0; i < 512; i ++) res[i] = work(i);
    for (int i = 1; i <= n; i ++)
    {
        int x;
        cin >> x;
        cout << res[x] << " ";
    }
    return 0;
}
```

## Q3 图片解码

### Q3 算法思路(demo1, Subtask1/2)

- `scanf` 读入数据**不会**过滤掉空白字符(空格、换行、Tab)，导致输入数据错误。改用 `cin` 读入字符数据，会自动过滤掉空白字符，确保正确读取每个字符。

```C++
    // scanf 错误处理输入
    cin >> z;
    for (int i = 0; i < z; i ++)
        for (int j = 0; j < z; j ++)
            scanf("%c", &g[i][j]);

    // scanf 正确处理输入
    cin >> z;
    scanf("\n");
    for (int i = 0; i < z; i ++)
    {
        for (int j = 0; j < z; j ++)
            scanf("%c", &g[i][j]);
        scanf("\n");
    }
```

- `swap` 函数交换字符时，必须使用**引用传递**，否则只能交换函数内的局部变量，无法修改原数组中的字符。
- 该题解只解决 Subtask1/2，可以通过 smqyOJ (11/23) 的数据点

### Q3 代码实现(demo1, Subtask1/2)

```C++
#include <bits/stdc++.h>
using namespace std;
const int N = 405;
const int M = 5e+4 + 10;
char g[N][N];
int z, t;
int ops[M][6];

void swap(char &a, char &b)
{
    char temp;
    temp = a;
    a = b;
    b = temp;
}

void solve(int u, int d, int l, int r, int o)
{
    if (o == 1) // up-down
    {
        for (int i = l; i <= r; i ++)
        {
            int up = u, down = d;
            while (up < down) swap(g[up][i], g[down][i]), up ++, down --;
        }
    }
    else if (o == -1) // left-right
    {
        for (int i = u; i <= d; i ++)
        {
            int left = l, right = r;
            while (left < right) swap(g[i][left], g[i][right]), left ++, right --;
        }
    }
}

int main()
{
    cin >> z;
    for (int i = 0; i < z; i ++)
        for (int j = 0; j < z; j ++)
            cin >> g[i][j];
        
    int ops_num;
    cin >> ops_num >> t;
    
    for (int i = 0; i < t; i ++)
        for (int j = 0; j < 6; j ++)
            cin >> ops[i][j];

    for (int i = t - 1; i >= 0; i --)
        solve(ops[i][1]-1, ops[i][2]-1, ops[i][3]-1, ops[i][4]-1, ops[i][5]);

    int row = 0, col = 0;
    for (int i = 0; i < z; i ++)
    {
        if (g[i][0] != '?') row ++;
        if (g[0][i] != '?') col ++;
    }
    printf("%d %d\n", row, col);
    for (int i = 0; i < row; i ++)
    {
        for (int j = 0; j < col; j ++)
            printf("%c", g[i][j]);
        puts("");
    }
    return 0;
}
```
