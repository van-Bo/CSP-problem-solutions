> 第31次CCF计算机软件能力认证
> 模拟认证CSP（）
> 模拟认证AcWing（）

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
- 使用前缀和处理操作序列，此时的操作是由累加效果的
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

# Q4 阴阳龙
## 算法思路
```C++

```