> 第31次CCF计算机软件能力认证
> 模拟认证CSP（100 + 100 + 100 + 40 + 4）
> 模拟认证AcWing（(10/10), (10/10), (12/12), (5/11), ?）

# Q1 坐标变换（其一）
## 算法思路
- 针对点坐标进行平移操作
- 操作处理使用前缀和，此处的数据范围较小，暴力操作也可
- 该题解可以通过 AcWing 官网评测系统下 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 110;

int n, m;
PII op[N], point[N];

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1 ;i <= n; i ++)
        scanf("%d%d", &op[i].x, &op[i].y);
    for (int i = 1; i <= m; i ++)
        scanf("%d%d", &point[i].x, &point[i].y);
        
    for (int i = 1; i <= n; i ++)
    {
        op[i].x += op[i - 1].x;
        op[i].y += op[i - 1].y;
    }
    
    for (int i = 1; i <= m; i ++)
    {
        int x = point[i].x + op[n].x;
        int y = point[i].y + op[n].y;
        printf("%d %d\n", x, y);
    }
    return 0;
}
```

# Q2 坐标变换（其二）
## 算法思路
- 针对二维坐标进行拉伸、旋转的操作，可以使用矩阵乘法来进行模拟
- 注意这里并没有针对二维坐标的平移操作，若存在，则矩阵要拓展至齐次坐标的形式（CG）
- 考虑每次询问时，所针对的是操作序列中的一个区间，故引出逆矩阵的使用，2x2 逆矩阵公式要会
- 使用前缀和处理操作序列，此时的操作是由累积效果的
- 记矩阵 M 的逆矩阵（如果存在逆矩阵的话）为 `IM(M)`，最终的操作矩阵为 `IM(op[l - 1]) * op[r]`
- 在题解中并没有针对矩阵是否可逆进行判定和处理，感觉给定的数据定有解（感觉后续的在 CSP 官网评测系统中无法通过的数据点应该是出在此处）
- 该题解可以通过 AcWing 官网评测系统下 (10/10) 的数据点，CSP 官网评测系统下的得分为 80 分，通过 (16/20) 的数据点
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <math.h>

using namespace std;
const int N = 1e+5 + 10;

int n, m;
struct Matrix
{
    double x11, x12, x21, x22;
}op[N];

Matrix cal(Matrix a, Matrix b)
{
    Matrix res;
    res.x11 = a.x11 * b.x11 + a.x12 * b.x21;
    res.x12 = a.x11 * b.x12 + a.x12 * b.x22;
    res.x21 = a.x21 * b.x11 + a.x22 * b.x21;
    res.x22 = a.x21 * b.x12 + a.x22 * b.x22;
    return res;
}

int main()
{
    scanf("%d%d", &n, &m);
    op[0] = {1, 0, 0, 1};
    for (int i = 1; i <= n; i ++)
    {
        int x;
        double v;
        scanf("%d%lf", &x, &v);
        
        if (x == 1)
            op[i] = {v, 0, 0, v};
        else if (x == 2)
            op[i] = {cos(v), -sin(v), sin(v), cos(v)};
    }
    
    for (int i = 1; i <= n; i ++)
        op[i] = cal(op[i], op[i - 1]);
    
    for (int i = 1; i <= m; i ++)
    {
        int l, r;
        double x, y;
        scanf("%d%d%lf%lf", &l, &r, &x, &y);
        
        Matrix finalOp;
        Matrix t = op[l - 1];
        double d = t.x11 * t.x22 - t.x12 * t.x21;
        
        finalOp.x11 = t.x22 / d;
        finalOp.x12 = -t.x12 / d;
        finalOp.x21 = -t.x21 / d;
        finalOp.x22 = t.x11 / d;
        
        finalOp = cal(finalOp, op[r]);
        
        double resX = finalOp.x11 * x + finalOp.x12 * y;
        double resY = finalOp.x21 * x + finalOp.x22 * y;
        printf("%.3lf %.3lf\n", resX, resY);
    }
    return 0;
}
```
## 算法思路
- 旋转操作 `rotate`与拉伸操作 `scale`，最终坐标变换的结果与两类操作的先后顺序无关
- 前缀和处理累积的变换操作，旋转角度可以累加，拉伸系数可以累乘
- 该题解可以通过 AcWing 官网评测系统下 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <math.h>

using namespace std;
const int N = 1e+5 + 10;
const double esp = 1e-5;

int n, m;
double scale[N], rot[N];

int main()
{
    scanf("%d%d", &n, &m);
    
    // 读入操作
    scale[0] = 1.0;
    for (int i = 1; i <= n; i ++)
    {
        int id;
        double v;
        scanf("%d%lf", &id, &v);
        
        if (id == 1) 
        {
            scale[i] = v;
            rot[i] = 0;
        }
        else if (id == 2)
        {
            scale[i] = 1.0;
            rot[i] = v;
        }
    }
    
    // 前缀和处理
    for (int i = 1; i <= n; i ++)
    {
        scale[i] *= scale[i - 1];
        rot[i] += rot[i - 1];
    }
    
    // 查询处理
    for (int i = 1; i <= m; i ++)
    {
        int l, r;
        double x, y;
        
        scanf("%d%d%lf%lf", &l, &r, &x, &y);
        
        // scale
        x *= scale[r] / scale[l - 1];
        y *= scale[r] / scale[l - 1];
        
        // rotate
        double resX, resY;
        resX = x * cos(rot[r] - rot[l - 1]) - y * sin(rot[r] - rot[l - 1]);
        resY = x * sin(rot[r] - rot[l - 1]) + y * cos(rot[r] - rot[l - 1]);
        
        printf("%.3lf %.3lf\n", resX, resY);
    }
    return 0;
}
```

# Q3 梯度求解
## 算法思路
- 输入处理：从标准输入中读取一行字符串，然后将该字符串解析为单词（以空格为分隔符）。
- 对于任意的一个乘积项使用`vector<int> f`类型存储数据，乘积项的存储大小为 n + 1（n 为变量的个数）
- 其中`f[0]`表示该乘积项的系数，`f[i]`表示变量 $x_i$ 的次数值
- 对于任意的一个多项式即为乘积项的一维数组，使用`vector<vector<int>> formula`的类型存储数据
- 根据读入的后缀表达式，每一次的栈操作便是多项式层面的计算
- 该题解可以通过 AcWing 官网评测系统下 (12/12) 的数据点，CSP 官网评测系统下得分为 100 分
## 多项式处理
- 多项式的加法，将两个多项式的乘积项叠加即可
- 多项式的减法，将后一个多项式的每一个乘积项的系数变成相反数，再进行多项式的加法即可
- 多项式的乘法，一个双层的`for`循环，两个多项式的乘积项两两相乘，乘积项相乘时，系数相乘，变量的次数值累加
- 多项式的求偏导，记偏导变量角标为`i`，所有乘积项`f`中的系数乘`f[i]`，之后`f[i]`自减 1
- 多项式的偏导求值，带入每一个变量的具体值即可
## 输入处理
- 头文件`#include <sstream>`
- `cin.get()`用于读取一个字符（在此处是回车符）并丢弃它，清除之前输入的缓存区
- `getline(cin, formula)`使用`getline()`函数从标准输入中读取一行字符串，并将其存储到`string`类型的`formula`中
- `stringstream oss(formula)`创建名为`oss`的字符串流，并将刚刚读取的`formula`字符串作为其输入
- `while (oss >> str) {}`利用循环，会不断从字符串流中读取单词，直到流中没有内容为止
## HINT
- 结构体`struct`定义结束的末尾要加`;`，不然其后声明的全局变量、函数会出现非法声明的报错
- `str.substr(postionStart)`返回子串
- `stoi(aNumString)`返回字符串转换为的`int`值
- `isdigit(aCharArgument)`返回 **单个字符** 是否是数字
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stack>

using namespace std;
typedef long long LL;
const int N = 110, P = 1e+9 + 7;

int n, m;
int v[N];

LL qmi(LL a, int b)
{
    LL res = 1;
    while (b)
    {
        if (b & 1) res = res * a % P;
        a = a * a % P;
        b >>= 1;
    }
    return res;
}

struct Formula
{
    vector<vector<LL>> f;
    
    Formula(): f{} {}
   
    Formula(int x)
    {
        vector<LL> k(n + 1, 0);
        k[0] = x;
        f.push_back(k);
    }
    
    Formula(string str)
    {
        int mark = stoi(str.substr(1));
        vector<LL> k(n + 1, 0);
        k[0] = 1, k[mark] ++;
        f.push_back(k);
    }
    
    Formula operator + (const Formula &g)
    {
        Formula temp;
        for (auto k : f) temp.f.push_back(k);
        for (auto k : g.f) temp.f.push_back(k);
        return temp;
    }
    
    Formula operator - (const Formula &g)
    {
        Formula temp;
        for (auto k : f) temp.f.push_back(k);
        for (auto k : g.f)
        {
            k[0] *= -1;
            temp.f.push_back(k);
        }
        return temp;
    }
    
    Formula operator * (const Formula &g)
    {
        Formula temp;
        for (auto k1 : f)
        {
            for (auto k2 : g.f)
            {
                vector<LL> k(n + 1, 0);
                k[0] = (k1[0] * k2[0]) % P;
                for (int i = 1; i <= n; i ++)
                    k[i] += k1[i] + k2[i];
                temp.f.push_back(k);
            }
        }
        return temp;
    }
    
    // partial derivative
    Formula dealPD(int x)
    {
        Formula temp;
        for (auto k : f)
        {
            k[0] *= k[x];
            if (k[x]) k[x] --;
            temp.f.push_back(k);
        }
        return temp;
    }
};

stack<Formula> res;

LL calAns(vector<vector<LL>> f)
{
    LL ans = 0;
    for (auto &k : f)
    {
        LL t = k[0];
        for (int i = 1; i <= n; i ++)
            t = t * qmi(v[i], k[i]) % P;
        ans = (ans + t) % P;
    }
    return (ans + P) % P;
}

int main()
{
    scanf("%d%d", &n, &m);
    
    cin.get();
    string strLine;
    getline(cin, strLine);
    
    stringstream oss(strLine);
    
    string s;
    while (oss >> s)
    {
        if (s == "+")
        {
            Formula b = res.top(); res.pop();
            Formula a = res.top(); res.pop();
            res.push(a + b);
        }
        else if (s == "-")
        {
            Formula b = res.top(); res.pop();
            Formula a = res.top(); res.pop();
            res.push(a - b);
        }
        else if (s == "*")
        {
            Formula b = res.top(); res.pop();
            Formula a = res.top(); res.pop();
            res.push(a * b);
        }
        else if (s[0] == 'x')
        {
            Formula t(s);
            res.push(t);
        }
        else 
        {
            Formula t(stoi(s));
            res.push(t);
        }
    }
    
    while (m --)
        {
            int mark = -1;
            scanf("%d", &mark);
            Formula pd = res.top().dealPD(mark);
            
            for (int i = 1; i <= n; i ++)
                scanf("%d", &v[i]);
                
            LL ans = calAns(pd.f);
            printf("%lld\n", ans);
        }
    return 0;
}
```
## 算法思路
- 针对前四个数据点进行分类讨论（骗分）
- 第 1、2 个数据点中变量只有 `x1`，表达式中仅含有一个元素，所以在此数据点下，仅会存在两种情况。其一，唯一的元素是变量 `x1`，该表达式的偏导、代值结果定为 1。其二，唯一的元素是常数，该表达式的偏导、代值结果定为 0。
- 第 3、4 个数据点中仅含有一个运算符，所以分类讨论的第一层便是针对运算符，其次分类讨论具体的运算值是变量 `x1`，还是常数即可
- 该题解可以通过 AcWing 评测官网下 (2/12) 的数据点，CSP 官网评测系统的得分为 40 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

int n, m;

void print(int v)
{
    for (int i = 1; i <= m; i ++)
        printf("%d\n", v);
}

int main()
{
    scanf("%d%d", &n, &m);
    cin.get();
    
    string strLine;
    getline(cin, strLine);
    
    stringstream oss(strLine);
    
    string s;
    vector<string> op;
    while (oss >> s)
    {
        op.push_back(s);
        if (op.size() == 3) break;
    }
    
    if (op.size() == 1)
    {
        if (op[0] == "x1")
            print(1);
        else 
            print(0);
    }
    else if (op.size() == 3)
    {
        if (op[2] == "+") // add
        {
            int cnt = 0;
            if (op[0] == "x1") cnt ++;
            if (op[1] == "x1") cnt ++;
            
            print(cnt);
        }
        else if (op[2] == "-") // sub
        {
            int res;
            if (op[0] == "x1" && op[1] != "x1") res = 1;
            else if (op[0] == "x1" && op[1] == "x1") res = 0;
            else if (op[0] != "x1" && op[1] == "x1") res = -1;
            else res = 0;
            
            print(res);
        }
        else if (op[2] == "*") // mul
        {
            if (op[0] == "x1" && op[1] != "x1")
                print(stoi(op[1]));
            else if (op[0] != "x1" && op[1] == "x1")
                print(stoi(op[0]));
            else if (op[0] == "x1" && op[1] == "x1")
            {
                for (int i = 1; i <= m; i ++)
                {
                    int id, v;
                    scanf("%d%d", &id, &v);
                    printf("%d\n", v * 2);
                }
            }
            else
                print(0);
        }
    }
    return 0;
}
```

# Q4 阴阳龙（Hack）
## 算法思路
- 开辟网格矩阵存储员工的位置，此时的网格矩阵大小为 1010x1010
- 本题的坐标位置有点特殊，坐标原点 `(1, 1)` 在左下角，给定的参数 `n` 和 `m`，表示网格矩阵有 n 列，m 行，其中横坐标值 `x` 的上界为 `n`，纵坐标值 `y` 的上界为 `m`，`g[x][y] = id` 表示 位置 `(x, y)` 存在员工，员工的编号为 `id`
- 对于每个网格的记录值，若对应的位置无员工，则记录值为 0；若对应的位置有员工，则记录值为该员工的编号
- `k2` 的上界值是阴阳龙出现的位置到边界的最小步长
- 在 $1 \leqslant k \leqslant k2$ 的范围内选择可以成功搜索到员工步长的最小值，若搜索不到，则 `k` 值为 0
- 确定 `k` 后，按照要求修改坐标即可（位置转移后，原先位置的编号信息要清零）
- 完成 `q` 组数据处理后，遍历整个网格矩阵，输出累异或的结果值
- 时间复杂度：$10^5 \times (5 \times 10^2 \times 8)$，感觉刚好卡在边界，可以过掉目标数据点
- 该题解可以通过 AcWing 官网评测系统 (5/11) 的数据点，CSP 官网评测系统下得分为 40 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <unordered_map>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 1010;

int n, m, p, q; // n列 m行

int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

int g[N][N];

int main()
{
    scanf("%d%d%d%d", &n, &m, &p, &q);
    for (int i = 1; i <= p; i ++)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        g[x][y] = i;
    }
    
    if (n > 1000 || m > 1000) return 0;
    
    while (q --)
    {
        int x, y, t;
        scanf("%d%d%d", &x, &y, &t);
        
        // 确定 k2 范围
        int k2 = min(y - 1, min(m - y, min(n - x, x - 1)));
        
        // k1、k2 求交，确定 k
        int k = -1;
        vector<PII> points;
        unordered_map<int, int> pr;
        for (int i = 1; i <= k2; i ++)  // i 为步长
        {
            for (int j = 0; j < 8; j ++)    // j 为方向
            {
                int a = x + dx[j] * i, b = y + dy[j] * i;
                if (g[a][b])
                {
                    points.push_back({a, b});
                    pr[b * n + a] = j;   
                }
            }
            if(points.size() > 0) 
            {
                k = i;
                break;
            }
        }
        if (k == -1) k = 0;
        
        if (k)
        {
            // 变换坐标
            vector<PII> location;
            vector<int> changes;
            for (auto p : points)
            {
                int a = p.x, b = p.y;
                int id = g[a][b];
                g[a][b] = 0;    // HINT point 
                
                int newX = x + k * dx[(pr[b * n + a] + t) % 8];
                int newY = y + k * dy[(pr[b * n + a] + t) % 8];
                
                location.push_back({newX, newY});
                changes.push_back(id);
            }
            
            int sz = location.size();
            for (int i = 0; i < sz; i ++)
                g[location[i].x][location[i].y] = changes[i];
        }
    }
    
    // 输出异或值
    int res = 0;
    for (int i = 1; i <= n; i ++)
        for (int j = 1; j <= m; j ++)
        {
            if (g[i][j])
                res = res ^ (i * g[i][j] + j);
        }
    printf("%d\n", res);
    return 0;
}
```

# Q5 阻击（Hack）
## 算法思路
- `dfs` 暴力搜索
- `hurt[x][y]` 表示节点 `x` 到节点 `y` 的损失，在每次输出结果前，调用 `getMaxHurt()` 获取结果，该函数会利用一个嵌套的双层 `for` 循环来遍历损失矩阵的上三角部分，同时维护最大损失值
- `getHurt()` 用来构建损失矩阵（逐行构建），每次逐行构建调用 `dfs(u, v, cnt)` 函数来进行深搜，其中 `u` 为根节点，`v` 为遍历到的节点，`cnt` 为从 `u` 到 `v` 的损失值
- 该题解 CSP 官网评测系统下得分为 4 分，只过掉了第一个数据点
```C++
#include <iostream>
#include <cstring>
#include <vector>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 305;

int n, m;
int h[N], e[N], ne[N], edgeID[N], idx;
PII edge[N];
bool st[N];

int hurt[N][N];
int res;

void add(int u, int v, int k)
{
    edgeID[idx] = k, e[idx] = v, ne[idx] = h[u], h[u] = idx ++; 
}

void dfs(int u, int v, int cnt)
{
    st[v] = true;
    for (int i = h[v]; ~i; i = ne[i])
    {
        int j = e[i], id = edgeID[i];
        if (st[j]) continue;

        hurt[u][j] = cnt + (edge[id].y - edge[id].x);
        dfs(u, j, hurt[u][j]);
    }
}

void getHurt()
{
    for (int start = 1; start <= n; start ++)
    {
        memset(st, 0, sizeof st);
        dfs(start, start, 0);
    }
}

int getMaxHurt()
{
    int res = 0;
    for (int i = 1; i <= n; i ++)
        for (int j = i + 1; j <= n; j ++)
            if (res < hurt[i][j])
                res = hurt[i][j];
    return res;
}

int main()
{
    scanf("%d%d", &n, &m);

    if (n > 300 || m > 300) return 0;

    memset(h, -1, sizeof h);
    for (int i = 1; i <= n - 1; i ++)
    {
        int u, v, w, b;
        scanf("%d%d%d%d", &u, &v, &w, &b);

        edge[i] = {w, b};
        add(u, v, i), add(v, u, i);
    }

    getHurt();
    res = getMaxHurt();
    printf("%d\n", res);

    while (m --)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        edge[x].x = y;

        memset(hurt, 0, sizeof hurt);
        getHurt();
        res = getMaxHurt();
        printf("%d\n", res);
    }
    return 0;
}
```