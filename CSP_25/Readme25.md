# 第25次CCF计算机软件能力认证

> 模拟认证 CSP（100 + 100 + ? + ?）
> 模拟认证 AcWing（(10/10), (10/10), ?, (2/21)）

## Q1 未初始化警告

### Q1 算法思路

- 使用 `unordered_set` 来存储已经可以作为赋值公式右值的变量或常量的下标
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分

```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;

int n, k;
unordered_set<int> validIndex;

int main()
{
    scanf("%d%d", &n, &k);
    
    int res = 0;
    validIndex.insert(0);
    for (int i = 0; i < k; i ++)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        if (validIndex.find(y) == validIndex.end())
            res ++;
            
        validIndex.insert(x);
    }
    printf("%d\n", res);
    return 0;
}
```

## Q2 出行计划

### Q2 算法思路

- 反向考虑、对于每一次出行计划，根据 `t` 和 `c` 值，可以反推出 哪些时刻进行核算检测可以完成此次的出行计划
- 差分、前缀和
- 若求出的区间值的上界为负值，要抛弃；若不抛弃，原先的区间会变成 `[1, N - 1]`，导致答案错误
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分

```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
const int N = 2e+5 + 10;

int n, m, k;
int cnt[N];

int main()
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 0; i < n; i ++)
    {
        int t, c;
        scanf("%d%d", &t, &c);
        
        if (t - k <= 0) continue;
        
        int l = max(1, t - k - c + 1);
        int r = min(t - k, N - 1);
        cnt[l] ++, cnt[r + 1] --;
    }
    
    for (int i = 1; i < N; i ++) 
        cnt[i] += cnt[i - 1];
        
    for (int i = 0; i < m; i ++)
    {
        int x;
        scanf("%d", &x);
        printf("%d\n", cnt[x]);
    }
    return 0;
}
```

## Q4 通信系统管理

### Q4 算法思路

- 使用 `vector<app> App` 来存储有效的申请信息，每天都根据申请信息更新资源图 `G` 和主要通信对象 `map`，并删除过期的申请信息
- 通过对 `G` 的行/列遍历，更新 `map` 中每台计算机的主要通信对象
- 利用 `map` 来统计通信孤岛和通信对的数量
- 该题解可以通过 AcWing 官网 (2/21) 的数据点

```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

typedef long long LL;
const int N = 2010; // 申请数量上限
const int M = 1010; // 计算机数量上限

using namespace std;

struct app
{
    int u, v, day;
    LL mb;
};
vector<app> App;
long long G[M][M];  // 某一天的资源图
int map[M];         // 记录主要通信对象
int n, m;

void work()
{
    // G
    memset(G, 0, sizeof(G));
    for(int i = App.size() - 1; i >= 0; i --)
    {
        int x = App[i].u, y = App[i].v;
        G[x][y] += App[i].mb;
        G[y][x] += App[i].mb;
        
        App[i].day --;
        if (App[i].day == 0)
            App.erase(App.begin() + i);
    }
    
    // map
    memset(map, 0, sizeof(map));
    for (int i = 1; i <= n; i ++)
    {
        int target = 0;
        LL res = 0;
        for (int j = 1; j <= n; j ++)
            if (G[i][j] > res)
            {
                res = G[i][j];
                target = j;
            }
        map[i] = target;
    }
}

int main()
{
    cin >> n >> m;  // n 机器数，m 天数
    while (m --)
    {
        int k;
        cin >> k;
        for (int i = 0; i < k; i ++)
        {
            int u, v, y;
            LL x;
            cin >> u >> v >> x >> y;
            App.push_back({u, v, y, x});
        }
        
        work(); // 更新 G, map
        
        // search 主要通信对象
        int l;
        cin >> l;
        for (int i = 0; i < l; i ++)
        {
            int tar;
            cin >> tar;
            cout << map[tar] << endl;
        }
        
        // search 通信孤岛和通信对
        int p, q;
        cin >> p >> q;
        if (p == 1)
        {
            int cnt = 0;
            for (int i = 1; i <= n; i ++)
                if (map[i] == 0)
                    cnt ++;
            cout << cnt << endl;
        }
        if (q == 1)
        {
            int cnt = 0;
            for (int i = 1; i <= n; i ++)
                if (map[map[i]] == i && map[i] > i)
                    cnt ++;
            cout << cnt << endl;
        }
    }
    return 0;
}
```
