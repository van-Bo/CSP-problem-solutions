> CSP（第33次CCF计算机软件能力认证）
> 正式认证（100 + 100 + 0 + 0 + 0）
> 模拟认证 CSP（100 + 100 + 70 + 100 + 30）
> 模拟认证 AcWing（(10/10), (10/10), (10/12), (11/11), (4/10)）

#### Q1 词频统计
##### 算法思路
- **unordered\_set**无序容器的使用，`insert()`插入，`size()`返回元素个数
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;
const int N = 110;

int n, m;
int cnt[N];
unordered_set<int> work[N];

int main()
{
    scanf("%d%d", &n, &m);
    for (int cases = 1; cases <= n; cases ++)
    {
        int k;
        scanf("%d", &k);
        for (int i = 0; i < k; i ++)
        {
            int x;
            scanf("%d", &x);
            cnt[x] ++;
            work[x].insert(cases);
        }
    }

    for (int i = 1; i <= m; i ++)
        printf("%d %d\n", work[i].size(), cnt[i]);
    return 0;
}
```

#### Q2 相似度计算
##### 算法思路
- 单词大小写转化
- $unordered\_set$无序容器的使用
- 容斥原理、由交求并
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;

int n, m;
unordered_set<string> A, B;

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; i ++)
    {
        string x;
        cin >> x;
        int len = x.length();
        for (int j = 0; j < len; j ++)
            if (x[j] >= 'A' && x[j] <= 'Z')
                x[j] = x[j] - 'A' + 'a';
        A.insert(x);
    }

    for (int i = 0; i < m; i ++)
    {
        string x;
        cin >> x;
        int len = x.length();
        for (int j = 0; j < len; j ++)
            if (x[j] >= 'A' && x[j] <= 'Z')
                x[j] = x[j] - 'A' + 'a';
        B.insert(x);
    }

    int x = 0;
    int y = A.size() + B.size();

    for (auto it : A)
    {
        if (B.count(it) == 1)
            x ++;
    }

    y -= x;
    printf("%d\n%d\n", x, y);
    return 0;
}
```

#### Q3 化学方程式配平（HACK）
##### 算法思路
- 高斯消元法
- 该题解可以过掉$CSP$官网评测系统下的$7/10$的数据点，得分$70$分；该题解可以过掉$AcWing$评测系统下的$(10/12)$的数据点
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <math.h>
#include <unordered_set>
#include <unordered_map>

using namespace std;
const int N = 45;
const double esp = 1e-8;

int n, m;
unordered_set<string> element;
unordered_map<string, int> matter[N];
double w[N][N];

void initElement()
{
    for (int i = 0; i < m; i ++)
    {
        string s;
        cin >> s;
        
        int len = s.length();
        string emt = "", num = "";
        
        for (int j = 0; j < len; j ++)
            if (s[j] >= 'a' && s[j] <= 'z')
            {
                for (int k = j; k < len; k ++)
                {
                    if (s[k] >= 'a' && s[k] <= 'z')
                        emt += s[k];
                    else
                    {
                        j = k - 1;
                        break;
                    }
                }
                element.insert(emt);
            }
            else
            {
                for (int k = j; k < len; k ++)
                {
                    if (s[k] >= '0' && s[k] <= '9')
                        num += s[k];
                    else
                    {
                        j = k - 1;
                        break;
                    }
                }
                matter[i][emt] = stoi(num);
                emt = "", num = "";
            }
    }
}

void initMatrix()
{
    unordered_map<string, int> row;
    int mark = 0;
    for (auto emt : element)
        row[emt] = mark ++;
        
    for (int i = 0; i < m; i ++)
    {
        for (auto mat : matter[i])
            w[row[mat.first]][i] = (double)mat.second;
    }
}

bool gauss()
{
    int emtSize = element.size();
    
    int r, c;
    for (r = 0, c = 0; c < m; c ++)
    {
        int t = r;
        for (int i = r; i < emtSize; i ++)
            if (fabs(w[i][c]) > fabs(w[t][c]))
                t = i;
        
        if (fabs(w[t][c]) < esp) continue;
        
        for (int i = c; i < m; i ++)
            swap(w[r][i], w[t][i]);
            
        for (int i = m - 1; i >= c; i --)
            w[r][i] /= w[r][c];
            
        for (int i = r + 1; i < emtSize; i ++)
            for (int j = m - 1; j >= c; j --)
                if (fabs(w[i][c]) > esp)
                    w[i][j] -= w[i][c] * w[r][j];
        r ++;
    }
    return r < m;
}

int main()
{
    scanf("%d", &n);
    while (n --)
    {
        scanf("%d", &m);
        
        initElement();
        
        initMatrix();
        
        bool res = gauss();
        if (res) puts("Y");
        else puts("N");
        
        memset(w, 0, sizeof w);
        element.clear();
        for (int i = 0; i < m; i ++)
            matter[i].clear();    
    }
    return 0;
}
```

#### Q4 十滴水
##### 算法思路
- 此处的网格记作$cell$，根据题目的数据范围$1 \leqslant c \leqslant 10^9$，$1 \leqslant m \leqslant min(c, 3\times10^5)$，可以开辟长度为 $3\times10^5$、类型为$PII$的$cell$数组来压缩存储网格信息，$first$对应网格在数组中的实际下标位置，$second$对应网格中的现存水滴数
- 既然使用压缩存储，必然存在序号位置与实际存储位置的关联，使用$unordered\_map\ p$来建立映射的关系；不适合开辟数组来建立映射关系，因为序号位置的数据范围太大了
- 使用$set$的特性，自动按照实际存储位置升序维护爆破点
- 要审题清楚，每次的处理操作，对于输入的$p$，保证在对应操作时 $p$内有水
- 此处并没有针对如果水滴爆破的情况下，只是通过暴力枚举的方式，来确定爆破点两边最近的并且有水的位置。该方式可以过掉$CSP$官网评测系统下的前$4$个数据点，得分$55$分；该方式可以过掉$AcWing$评测系统下的$(9/11)$的数据点
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <set>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 3e+5 + 10;

int c, m, n;
PII cell[N];
set<int> tmp;
unordered_map<int, int> p;
int cnt;

// 实际存储位置是否有效
bool isValid(int pos)
{
    if (pos >= 0 && pos < m)
        return true;
    else
        return false;
}

void deal(int pos)
{
    cell[pos].y = 0;
    cnt --;
    tmp.erase(pos);
    
    int l = pos - 1, r = pos + 1;
    
    while (isValid(l) && cell[l].y == 0)
        l --;
    while (isValid(r) && cell[r].y == 0)
        r ++;
        
    if (isValid(l))
    {
        cell[l].y += 1;
        if(cell[l].y >= 5) tmp.insert(l);
    }
    
    if (isValid(r))
    {
        cell[r].y += 1;
        if(cell[r].y >= 5) tmp.insert(r);
    }
}

int main()
{
    scanf("%d%d%d", &c, &m, &n);
    for (int i = 0; i < m; i ++)
    {
        int pos, x;
        scanf("%d %d", &pos, &x);
        cell[i].x = pos, cell[i].y = x;
        
        cnt ++;
    }
    
    sort(cell, cell + m);
    
    for (int i = 0; i < m; i ++)
        p[cell[i].x] = i;
    
    while (n --)
    {
        int pos; 
        scanf("%d", &pos);
        
        int k = p[pos];
        if (cell[k].y < 4)
        {
            cell[k].y += 1;
            printf("%d\n", cnt);
        }
        else
        {
            tmp.insert(k);
            while (tmp.size())
            {
                deal(*(tmp.begin()));
            }
            printf("%d\n", cnt);
        }
    }
    return 0;
}
```
- 优化处理：确定爆破点两边最近的并且有水的位置。抽象处理：快速找到要操作的数的两边最近的非零的数，可以使用$set$将所有含有非零数的水滴的下标放进去，然后在每次要用时使用$set$自带的$find$函数进行查找即可，函数$find$的时间复杂度是$\log n$
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <set>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 3e+5 + 10;

int c, m, n;
PII cell[N];
set<int> tmp;
set<int> unEmpty;
unordered_map<int, int> p;
int cnt;

// 实际存储位置是否有效
bool isValid(int pos)
{
    if (pos >= 0 && pos < m)
        return true;
    else
        return false;
}

void deal(int pos)
{
    cell[pos].y = 0;
    cnt --;
    tmp.erase(pos);
    
    auto it = unEmpty.find(pos);
    auto prevIt = prev(it);
    auto nextIt = next(it);
    unEmpty.erase(pos);
    
    if (prevIt != unEmpty.end())
    {
        int v = *prevIt;
        cell[v].y += 1;
        if (cell[v].y >= 5) tmp.insert(v);
    }
    
    if (nextIt != unEmpty.end())
    {
        int v = *nextIt;
        cell[v].y += 1;
        if (cell[v].y >= 5) tmp.insert(v);
    }
}

int main()
{
    scanf("%d%d%d", &c, &m, &n);
    for (int i = 0; i < m; i ++)
    {
        int pos, x;
        scanf("%d %d", &pos, &x);
        cell[i].x = pos, cell[i].y = x;
        
        cnt ++;
        
        unEmpty.insert(i);
    }
    
    sort(cell, cell + m);
    
    for (int i = 0; i < m; i ++)
        p[cell[i].x] = i;

    while (n --)
    {
        int pos; 
        scanf("%d", &pos);
        
        int k = p[pos];
        if (cell[k].y < 4)
        {
            cell[k].y += 1;
            printf("%d\n", cnt);
        }
        else
        {
            tmp.insert(k);
            while (tmp.size())
                deal(*(tmp.begin()));
            printf("%d\n", cnt);
        }
    }
    return 0;
}
```

#### Q5 文件夹合并（HACK）
##### 算法思路
- 双向链表
- $query$操作时，从头节点开始深搜
- 该题解可以过掉$CSP$官网评测系统中的$1, 2$的子任务数据点，得分$25$分；该题解可以过掉$AcWing$评测系统下的$(3/10)$的数据点
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;
const int N = 5e+5 + 10;

int n, m;
int ffByte[N];
unordered_set<int> chd[N];
int steps = 1;

bool query(int pos, int tar)
{
    for (auto k : chd[pos])
        if (query(k, tar))
        {
            steps ++;
            return true;
        }
        
    if (pos == tar) 
        return true;
    else
        return false;
}

void merge(int id)
{
    unordered_set<int> temp;
    for (auto k1 : chd[id])
    {
        ffByte[id] += ffByte[k1];
        for (auto k2 : chd[k1])
        {
            temp.insert(k2);
        }
    }
    chd[id].clear();
    for (auto k : temp)
        chd[id].insert(k);
        
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 2; i <= n; i ++)
    {
        int fa;
        scanf("%d", &fa);
        chd[fa].insert(i);
    }
    for (int i = 1; i <= n; i ++)
    {
        int x;
        scanf("%d", &x);
        ffByte[i] = x;
    }
    
    while (m --)
    {
        int op, id;
        scanf("%d%d", &op, &id);
        
        if (op == 1)
        {
            merge(id);
            printf("%d %d\n", chd[id].size(), ffByte[id]);
        }
        else 
        {
            query(1, id);
            printf("%d\n", steps);
            steps = 1;
        }
    }
    return 0;
}
```
- 针对$query$操作优化，额外记录每个文件的上级文件，由此可以从目标文件夹$tar$，一路逆向溯源即可
- 该题解可以过掉$CSP$官网评测系统中的$1, 2, 5$的子任务数据点，得分$30$分；该题解可以过掉$AcWing$评测系统下的$(4/10)$的数据点
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;
const int N = 5e+5 + 10;

int n, m;
int ffByte[N];
unordered_set<int> chd[N];
int f[N];
int cnt;

void query_r(int tar)
{
    cnt ++;
    if (tar == 1) 
        return;
        
    query_r(f[tar]);
}

void merge(int id)
{
    unordered_set<int> temp;
    for (auto k1 : chd[id])
    {
        ffByte[id] += ffByte[k1];
        for (auto k2 : chd[k1])
        {
            temp.insert(k2);
        }
        f[k1] = 0;
    }
    
    chd[id].clear();
    for (auto k : temp)
        chd[id].insert(k), f[k] = id;
    
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 2; i <= n; i ++)
    {
        int fa;
        scanf("%d", &fa);
        chd[fa].insert(i);
        f[i] = fa;
    }
    for (int i = 1; i <= n; i ++)
    {
        int x;
        scanf("%d", &x);
        ffByte[i] = x;
    }
    
    while (m --)
    {
        int op, id;
        scanf("%d%d", &op, &id);
        
        if (op == 1)
        {
            merge(id);
            printf("%d %d\n", chd[id].size(), ffByte[id]);
        }
        else 
        {
            query_r(id);
            printf("%d\n", cnt);
            cnt = 0;
        }
    }
    return 0;
}
```