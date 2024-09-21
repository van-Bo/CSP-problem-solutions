# Q1 报数
## 算法思路
- 模拟即可
- 寻找数值中是否含有数字 `7`，可以使用 `string` 来处理
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int n;
int cnt[4];

int main()
{
    scanf("%d", &n);
    
    int mark = 0;
    int v = 1;
    for (int i = 1; i <= n; i ++)
    {
        if (v % 7 == 0 || to_string(v).find('7') != -1)
            cnt[mark] ++, i --;
        v ++;
        mark = (mark + 1) % 4;
    }
    
    for (int i = 0; i < 4; i ++)
        printf("%d\n", cnt[i]);
    return 0;
}
```
- 寻找数值中是否含有数字 `7`，也可以额外构造查询函数
- 该题解可以通过 AcWing 官网 (10/10) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int n;
int cnt[4];

bool checkHas7(int v)
{
    bool flag = false;
    while (v)
    {
        if (v % 10 == 7)
        {
            flag = true;
            break;
        }
        v /= 10;
    }
    return flag;
}

int main()
{
    scanf("%d", &n);
    
    int mark = 0;
    int v = 1;
    for (int i = 1; i <= n; i ++)
    {
        if (v % 7 == 0 || checkHas7(v))
            cnt[mark] ++, i --;
        v ++;
        mark = (mark + 1) % 4;
    }
    
    for (int i = 0; i < 4; i ++)
        printf("%d\n", cnt[i]);
    return 0;
}
```

# Q2 回收站选址
## 算法思路
- 暴力枚举即可
- 时间复杂度：$O(8n^2)$
- 该题解可以通过 AcWing 官网 (11/11) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>

#define x first
#define y second

using namespace std;
typedef pair<int, int> PII;
const int N = 1010;

int n, ans[5];
PII p[N];

int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i ++)
        scanf("%d%d", &p[i].x, &p[i].y);
        
    for (int i = 0; i < n; i ++)
    {
        int cnt[8] = {0};
        for (int j = 0; j < n; j ++)
        {
            for (int k = 0; k < 8; k ++)
                if (p[i].x + dx[k] == p[j].x && p[i].y + dy[k] == p[j].y)
                {
                    cnt[k] ++;
                    break;
                }
        }
        
        if (cnt[1] && cnt[3] && cnt[5] && cnt[7])
            ans[cnt[0] + cnt[2] + cnt[4] + cnt[6]] ++;
    }
    
    for (int i = 0; i <= 4; i ++)
        printf("%d\n", ans[i]);
    return 0;
}
```

# Q3 化学方程式
## 算法思路
- 处理的的化学方程式与真实的化学方程式相同，不存在如 `CSP_33 Q3` 中化学式的简化形式
- 使用哈希表 `unordered_map<string, int>` 来统计元素出现的次数
- 整个化学方程式可以一分为二，变成等号前、后两部分 `f`、`g`
- `work()` 函数将等号前后两部分，按照 `+` 再次划分，变成最小的结构，该结构的形式为 `c * item`，其中 `c` 为物质前的系数，`item` 为对应物质的化学式
- `dfs()` 函数递归处理化学式字符串，`dfs(item, k)` 表示递归处理 `item`，其中的处理位置从 `k` 开始。这里的参数 `k` 使用引用的方式，下一次递归函数的 `k` 值会延续上一次的递归函数的 `k` 值。
- 该题解可以通过 AcWing 官网 (11/11) 的数据点，CSP 官网评测系统下的得分为 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

#define x first
#define y second

using namespace std;
typedef unordered_map<string, int> mpsi;

int n;
string str;

mpsi dfs(string& item, int& k)
{
    mpsi subRes;
    int len = item.length();
    while (k < len)
    {
        if (item[k] >= 'A' && item[k] <= 'Z')
        {
            // 处理元素
            int mark = k;
            k ++; // 跳过首字母
            while (k < len && item[k] >= 'a' && item[k] <= 'z') k ++;
            string e = item.substr(mark, k - mark);
            
            // 处理元素底数
            int cnt = 1;
            mark = k;
            while (k < len && isdigit(item[k])) k ++;
            if (k > mark) cnt = stoi(item.substr(mark, k - mark));
            
            subRes[e] += cnt;
        }
        else if (item[k] == '(')
        {
            k ++;  // 跳过 '('
            mpsi es = dfs(item, k);
            k ++;  // 跳过 ')'
            
            int cnt = 1, mark = k;
            while (k < len && isdigit(item[k])) k ++;
            if (k > mark) cnt = stoi(item.substr(mark, k - mark));
            
            for (auto e : es)
                subRes[e.x] += e.y * cnt;
        }
        else if (item[k] == ')')
            break;
    }
    return subRes;
}

mpsi work(string s)     // 处理方程式前、后两段
{
    mpsi res;
    int len = s.length();

    for (int i = 0; i < len; i ++)
    {
        int k = i;
        while (k < len && s[k] != '+') k ++;
        string item = s.substr(i, k - i);   // 获取单个物质

        i = k;  // k 指向 '+' 
        
        // 处理单个物质前的系数
        int cnt = 1;
        k = 0;
        while (k < item.length() && isdigit(item[k])) k ++;
        if (k) cnt = stoi(item.substr(0, k));
        
        mpsi subRes = dfs(item, k);
        for (auto e : subRes)
            res[e.x] += e.y * cnt;
    }
    return res;
}

int main()
{
    scanf("%d", &n);
    while (n --)
    {
        cin >> str;
        
        int k = str.find('=');
        string f = str.substr(0, k), g = str.substr(k + 1);
        
        mpsi resF = work(f);
        mpsi resG = work(g);
        
        if (resF == resG) puts("Y");
        else puts("N");
    }
    return 0;
}
```