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