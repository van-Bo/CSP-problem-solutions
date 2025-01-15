> CSP(第35次CCF计算机软件能力认证)
> 正式认证(100 + 80 + 0 + 30 + 35)
> 模拟认证()

# Q1 密码
## 算法思路
- 模拟即可
- `pr` 用于标记每种字符出现的次数
- `f1`、`f2`、`f3` 分别用于标记是否出现过数字, 字母, 特殊字符
- `flag` 用于标记是否满足同一字符的出现次数不超过 2 次
- 该题解 CSP 官网得分 100 分
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

#define x first
#define y second

using namespace std;
unordered_map<char, int> pr;
int n;

int main()
{
    scanf("%d", &n);

    while (n --)
    {
        string s;
        cin >> s;
        int len = s.length();

        pr.clear();

        bool f1 = false, f2 = false, f3 = false;
        for (int i = 0; i < len; i ++)
        {
            if (isdigit(s[i])) f1 = true;
            if (s[i] >= 'a' && s[i] <= 'z') f2 = true;
            if (s[i] >= 'A' && s[i] <= 'Z') f2 = true;
            if (s[i] == '*' || s[i] == '#') f3 = true;

            pr[s[i]] ++;
        }

        bool flag = true;
        for (auto k : pr)
        {
            if (k.y > 2) 
            {
                flag = false;
                break;
            }
        }

        if (f1 && f2 && f3 && flag)
            puts("2");
        else if (f1 && f2 && f3)
            puts("1");
        else 
            puts("0");
    }
    return 0;
}
```

# Q2 字符串变换
## 算法思路
- 字符之间的替换函数使用哈希表 `pr` 存储
- 暴力模拟替换
- 时间复杂度 $O(m \times k \times \log n)$
- 该题解 CSP 官网评测得分 80 分, 报错提示为 `TLE`
## 输入读取字符（串）那些事儿
- `std::getline(std::cin, str)` 从标准输入 `std::cin` 读取一行字符串（包括空格）到 `std::string str`，直到换行符或文件末尾为止
- `cin >> str` 输入数据，遇到空格、换行符等分隔符便会停止
- `scanf("%c", &ch)` 、`scanf("%s", str)` 读取单个字符，会读取任何字符，包括空格、换行符、制表符等。如果在读取前有输入的换行符，它会直接读取换行符，而不是用户期望的字符，此为导致输入问题的关键所在
- `cin.get()`、`getchar()` 可用来读取、丢弃换行符
## HINT
- `cin.get()` 用于读取并丢弃缓冲区中的第一个字符, 在这种情况下, 用于清除 `scanf` 读取整数后残留在输入流中的换行符
- 若不加 `cin.get()`, 换行符将留在输入流中, 导致 `getline()` 在第一次循环读取时读取的是换行符, 而不是输入的数据行, 现场评测时调试半天 >_<
```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

using namespace std;
unordered_map<char, char> pr;

string str;
int n, m;

int main()
{
    getline(cin, str);

    scanf("%d", &n);
    cin.get();  // HINT POINT

    for (int i = 0; i < n; i ++)
    {
        string s;
        getline(cin, s);
        pr[s[1]] = s[2];
    }

    scanf("%d", &m);
    while (m --)
    {
        int x;
        scanf("%d", &x);

        string temp = str;
        for (int i = 0; i < x; i ++)
        {
            for (int j = 1; j <= temp.length() - 2; j ++)
            {
                if (pr.find(temp[j]) != pr.end())
                    temp[j] = pr[temp[j]];
            }
        }
        cout << temp << endl;
    }
    return 0;
}
```
## 算法思路
- 标记每个字符的替换循环节大小, 数据范围中变换次数 `k` 便可看出端倪
```C++

```