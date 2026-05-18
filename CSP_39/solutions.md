# CSP(第39次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + (20/20) + (10/10))

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

## Q3 HTTP 头信息

### Q3 算法思路(demo1, subtask-40%)

- 针对 40% 的测试点，不涉及哈夫曼树编码的字符串处理，直接模拟指令即可
- 该题解对于字面量并索引指令的更新动态列表和读取操作存在问题，但前 40% 的测试点并不涉及这些操作
- 该题解可以通过 smqyOJ (4/10) 的测试点，得分 40 分

### Q3 代码实现(demo1, subtask-40%)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<string, string> pss;
#define x first
#define y second

unordered_map<char, string> hp;

struct Node {
    char data;
    shared_ptr<Node> left;
    shared_ptr<Node> right;

    Node(char d) : data(d), left(nullptr), right(nullptr) {}
    Node() : data('\0'), left(nullptr), right(nullptr) {}
};

shared_ptr<Node> rebuildHuffmanTree(const string& s, int& index) {
    if (index >= (int)s.length()) return nullptr;

    if (s[index] == '1') {
        index++; // 跳过'1'
        char ch = s[index++]; // 读取字符
        return make_shared<Node>(ch);
    } else if (s[index] == '0') {
        index++; // 跳过'0'
        auto node = make_shared<Node>();
        node->left = rebuildHuffmanTree(s, index);
        node->right = rebuildHuffmanTree(s, index);
        return node;
    }
    return nullptr;
}

// 辅助函数：打印树结构（前序遍历）
void printTree(const shared_ptr<Node>& root, string prefix = "", string prestr = "") {
    if (!root) return;

    if (root->data != '\0') {
        // cout << prefix << "Leaf: " << root->data << endl;
        hp[root->data] = prestr;

    } else {
        // cout << prefix << "Internal Node" << endl;
    }
    printTree(root->left, prefix + "  ", prestr + "0");
    printTree(root->right, prefix + "  ", prestr + "1");
}

const int S = 69, D = 125;

pair<string, string> sl[S], dl[D];
int dl_pos = -1;
int s, d, n;

string deal(string str)
{
    string res = "";
    if (str[0] != 'H')
        res = str;
    else if (str[0] == 'H' && str[1] == 'H')
        res = str.substr(1, str.length() - 1);
    else
    {
        string t = str.substr(1, str.length() - 1);

    }
    return res;
}

void work1(int mark)
{
    string k, v;
    if (mark >= 1 && mark <= s)     // 静态列表-key
        k = sl[mark - 1].x, v = sl[mark - 1].y;
    else
        k = dl[mark - s - 1].x, v = dl[mark - s - 1].y;
    cout << k << ": " << v << endl;
}

void work2(string k, string v)
{
    cout << k << ": " << v << endl;
    dl_pos = (dl_pos + 1) % d;  // 锁定动态列表的插入位置
    dl[dl_pos] = {k, v};
}

void work3(string k, string v)
{
    cout << k << ": " << v << endl;
}

int main()
{
    string encodedTree = ""; // 示例：0表示内部节点，1表示叶子节点
    int index = 0;

    cin >> s >> d;
    for (int i = 0; i < s; i ++)    // 存储静态键值对
    {
        string key, value;
        cin >> key >> value;
        sl[i] = {key, value};
    }

    cin >> encodedTree;
    index = 0;
    auto root = rebuildHuffmanTree(encodedTree, index);
    printTree(root);

    cin >> n;
    for (int i = 0; i < n; i ++)    // 处理指令
    {
        int idx;
        cin >> idx;
        if (idx == 1)
        {
            int k;
            cin >> k;
            work1(k);
        }
        else if (idx == 2)
        {
            int mark;
            cin >> mark;
            if (mark == 0)
            {
                string k, v;
                cin >> k >> v;
                k = deal(k), v = deal(v);
                work2(k, v);
            }
            else
            {
                string k, v;
                if (mark >= 1 && mark <= s)     // 静态列表-key
                    k = sl[mark - 1].x;
                else
                    k = dl[mark - s - 1].x;
                cin >> v;
                v = deal(v);
                work2(k, v);
            }
        }
        else if (idx == 3)
        {
            int mark;
            cin >> mark;
            if (mark == 0)
            {
                string k, v;
                cin >> k >> v;
                k = deal(k), v = deal(v);
                work3(k, v);
            }
            else
            {
                string k, v;
                if (mark >= 1 && mark <= s)     // 静态列表-key
                    k = sl[mark - 1].x;
                else                            // 动态列表-key
                    k = dl[mark - s - 1].x;
                cin >> v;
                v = deal(v);
                work3(k, v);
            }
        }
    }
    return 0;
}
```

### Q3 算法思路(demo2, AC)

- `sl` 数组存储静态列表的键值对，`dl` 数组存储动态列表的键值对
- `dl_pos` 表示动态列表的更新插入点(前向更新、后向读取)，初始时为 `0`，每次更新后 `dl_pos` 向前移动一位`dl_pos = (dl_pos - 1 + d) % d`，读取时，根据 `mark` 向后定位到对应的动态列表表项的位置 `dl[((dl_pos + 1) % d + mark - s - 1) % d]`
- 该题解可以通过 smqyOJ (10/10) 的测试点，得分 100 分

### 哈夫曼树的重建和字符串解码

```cpp
struct Node {
    char data;                 // 存储节点对应的字符
    shared_ptr<Node> left;     // 左子节点
    shared_ptr<Node> right;    // 右子节点

    // 构造函数：如果是叶子节点，传入具体的字符 d
    Node(char d) : data(d), left(nullptr), right(nullptr) {}
    // 构造函数：如果是内部节点，字符为空 '\0'
    Node() : data('\0'), left(nullptr), right(nullptr) {}
};
```

 在 `C++` 中，`std::shared_ptr` 和 `std::make_shared` 是现代 `C++` 内存管理的核心工具。它们属于智能指针，旨在解决传统的裸指针(raw pointers, 例如 new 和 delete)容易导致的内存泄漏、悬空指针等问题。智能指针 `shared_ptr`，而不是传统的裸指针 `Node*`。好处是不需要手动 `delete`，当树不再被使用时，内存会自动释放，彻底杜绝了内存泄漏(Memory Leak)的隐患

可以用 `std::shared_ptr<T> ptr(new T())` 的方式来创建智能指针，但 `C++` 标准库强烈推荐使用 `std::make_shared` 函数。`make_shared` 是一个模板函数，它的作用是在堆上动态分配对象，并直接返回一个管理该对象的 `shared_ptr`

```cpp
// 推荐的写法
std::shared_ptr<MyClass> ptr = std::make_shared<MyClass>();

// 如果构造函数需要参数，直接传给 make_shared 即可：
std::shared_ptr<MyClass> ptr = std::make_shared<MyClass>(arg1, arg2);
```

```cpp
shared_ptr<Node> rebuildHuffmanTree(const string& s, int& index) {
    // 边界条件：如果字符串读完了，返回空指针
    if (index >= (int)s.length()) return nullptr;

    if (s[index] == '1') {
        // 【情况 A：遇到叶子节点】
        index ++;               // 跳过标志位 '1'
        char ch = s[index ++];  // 紧接着的那个字符就是真实数据，读取后下标再后移
        return make_shared<Node>(ch); // 创建叶子节点并返回
    } 
    else if (s[index] == '0') {
        // 【情况 B：遇到内部节点】
        index ++;               // 跳过标志位 '0'
        auto node = make_shared<Node>(); // 创建一个内部节点（没有真实字符数据）
        
        // 关键所在：递归构建左右子树
        node->left = rebuildHuffmanTree(s, index);  // 先建左子树
        node->right = rebuildHuffmanTree(s, index); // 再建右子树
        
        return node; // 左右都建好了，把当前节点返回给上一层
    }
    return nullptr;
}
```

字符串解码过程就是依此遍历哈夫曼树的过程，遇到 '0' 就往左走，遇到 '1' 就往右走，直到走到叶子节点就把对应的字符输出，并回到根节点继续解码下一个字符(408考研数据结构真题)

### Q3 代码实现(demo2, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<string, string> pss;
#define x first
#define y second

// unordered_map<char, string> hp;

struct Node {
    char data;
    shared_ptr<Node> left;
    shared_ptr<Node> right;

    Node(char d) : data(d), left(nullptr), right(nullptr) {}
    Node() : data('\0'), left(nullptr), right(nullptr) {}
};

shared_ptr<Node> rebuildHuffmanTree(const string& s, int& index) {
    if (index >= (int)s.length()) return nullptr;

    if (s[index] == '1') {
        index ++; // 跳过'1'
        char ch = s[index ++]; // 读取字符
        return make_shared<Node>(ch);
    } else if (s[index] == '0') {
        index ++; // 跳过'0'
        auto node = make_shared<Node>();
        node->left = rebuildHuffmanTree(s, index);
        node->right = rebuildHuffmanTree(s, index);
        return node;
    }
    return nullptr;
}

// 辅助函数：打印树结构（前序遍历）
void printTree(const shared_ptr<Node>& root, string prefix = "", string prestr = "") {
    if (!root) return;

    if (root->data != '\0') {
        // cout << prefix << "Leaf: " << root->data << endl;
        // hp[root->data] = prestr;

    } else {
        // cout << prefix << "Internal Node" << endl;
    }
    printTree(root->left, prefix + "  ", prestr + "0");
    printTree(root->right, prefix + "  ", prestr + "1");
}

const int S = 69, D = 125;
pair<string, string> sl[S], dl[D];
int dl_pos = 0; // 动态列表的首位置（更新位置）
int s, d, n;
shared_ptr<Node> root;

string hexToBin(char c) // 十六进制向二进制转变
{
    int val = 0;
    if (c >= '0' && c <= '9') val = c - '0';
    else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;

    string res = "";
    for (int i = 3; i >= 0; i --)
        res += (val >> i) & 1 ? "1" : "0";
    return res;
}

string deal(string str) // 解码字符串
{
    string res = "";
    if (str.length() >= 1 && str[0] != 'H')
        res = str;
    else if (str.length() >= 2 && str[0] == 'H' && str[1] == 'H')
        res = str.substr(1, str.length() - 1);
    else
    {
        // 获取二进制序列的补零个数
        string p_str = str.substr(str.length() - 2);
        int p_value = stoi(p_str, nullptr, 16);
        
        string hexStr = str.substr(1, str.length() - 3);    // Hex 序列
        string binStr = ""; // Bin 序列
        for (char c : hexStr) binStr += hexToBin(c);

        // 去除后补零
        binStr = binStr.substr(0, binStr.length() - p_value);

        auto curr = root;
        for (char c : binStr)
        {
            if (c == '0') curr = curr->left;
            else curr = curr->right;

            if (curr->data != '\0')
            {
                res += curr->data;
                curr = root;
            } 
        }
    }
    return res;
}

void work1(int mark)
{
    string k, v;
    if (mark >= 1 && mark <= s)     // 静态列表-key
        k = sl[mark - 1].x, v = sl[mark - 1].y;
    else
    {
        k = dl[((dl_pos + 1) % d + mark - s - 1) % d].x;
        v = dl[((dl_pos + 1) % d + mark - s - 1) % d].y;
    }
        
    cout << k << ": " << v << endl;
}

void work2(string k, string v)
{
    cout << k << ": " << v << endl;
    dl[dl_pos] = {k, v};
    dl_pos = (dl_pos - 1 + d) % d;  // 动态列表的下次更新的插入位置
}

void work3(string k, string v)
{
    cout << k << ": " << v << endl;
}

int main()
{
    string encodedTree = ""; // 示例：0表示内部节点，1表示叶子节点
    int index = 0;

    cin >> s >> d;
    for (int i = 0; i < s; i ++)    // 存储静态键值对
    {
        string key, value;
        cin >> key >> value;
        sl[i] = {key, value};
    }

    cin >> encodedTree;
    index = 0;
    root = rebuildHuffmanTree(encodedTree, index);
    // printTree(root);

    cin >> n;
    for (int i = 0; i < n; i ++)    // 处理指令
    {
        int idx;
        cin >> idx;
        if (idx == 1)
        {
            int k;
            cin >> k;
            work1(k);
        }
        else if (idx == 2)
        {
            int mark;
            cin >> mark;
            if (mark == 0)
            {
                string k, v;
                cin >> k >> v;
                k = deal(k), v = deal(v);
                work2(k, v);
            }
            else
            {
                string k, v;
                if (mark >= 1 && mark <= s)     // 静态列表-key
                    k = sl[mark - 1].x;
                else
                    k = dl[((dl_pos + 1) % d + mark - s - 1) % d].x;
                cin >> v;
                v = deal(v);
                work2(k, v);
            }
        }
        else if (idx == 3)
        {
            int mark;
            cin >> mark;
            
            if (mark == 0)
            {
                
                string k, v;
                cin >> k >> v;
                k = deal(k), v = deal(v);
                work3(k, v);
            }
            else
            {
                string k, v;
                if (mark >= 1 && mark <= s)     // 静态列表-key
                    k = sl[mark - 1].x;
                else                            // 动态列表-key
                    k = dl[((dl_pos + 1) % d + mark - s - 1) % d].x;
                cin >> v;
                v = deal(v);
                work3(k, v);
            }
        }
    }
    return 0;
}
```

### Q3 算法思路(demo3, AC)

- 使用 `std::deque` (双端队列) 维护动态列表，`deque` 的特性完美契合题目要求的“每次新来的放在最前面，老的数据往后挤，挤出限制了就扔掉最后面那个”，使用 `deque`，不需要自己维护任何头尾指针和取模运算：
  - 插入新元素： `dl.push_front({k, v})` (永远插在最前面，下标 0)
  - 淘汰老元素： `if (dl.size() > d) dl.pop_back()` (超过长度直接扔掉尾巴)
  - 随机存取访问： 最新的永远在下标 0，次新的在下标 1，所以只要想访问动态表第 `mark - s` 项，直接用 `dl[mark - s - 1]` 即可
- 该题解可以通过 smqyOJ (10/10) 的测试点，得分 100 分

### Q3 代码实现(demo3, AC)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef pair<string, string> pss;
#define x first
#define y second

// unordered_map<char, string> hp;

struct Node {
    char data;
    shared_ptr<Node> left;
    shared_ptr<Node> right;

    Node(char d) : data(d), left(nullptr), right(nullptr) {}
    Node() : data('\0'), left(nullptr), right(nullptr) {}
};

shared_ptr<Node> rebuildHuffmanTree(const string& s, int& index) {
    if (index >= (int)s.length()) return nullptr;

    if (s[index] == '1') {
        index ++; // 跳过'1'
        char ch = s[index ++]; // 读取字符
        return make_shared<Node>(ch);
    } else if (s[index] == '0') {
        index++; // 跳过'0'
        auto node = make_shared<Node>();
        node->left = rebuildHuffmanTree(s, index);
        node->right = rebuildHuffmanTree(s, index);
        return node;
    }
    return nullptr;
}

// 辅助函数：打印树结构（前序遍历）
void printTree(const shared_ptr<Node>& root, string prefix = "", string prestr = "") {
    if (!root) return;

    if (root->data != '\0') {
        // cout << prefix << "Leaf: " << root->data << endl;
        // hp[root->data] = prestr;

    } else {
        // cout << prefix << "Internal Node" << endl;
    }
    printTree(root->left, prefix + "  ", prestr + "0");
    printTree(root->right, prefix + "  ", prestr + "1");
}

const int S = 69, D = 125;
pair<string, string> sl[S];
deque<pss> dl;  // 使用 deque，抛弃容易算错下标的循环数组
int dl_pos = 0; // 动态列表的首位置（更新位置）
int s, d, n;
shared_ptr<Node> root;

string hexToBin(char c) // 十六进制向二进制转变
{
    int val = 0;
    if (c >= '0' && c <= '9') val = c - '0';
    else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;

    string res = "";
    for (int i = 3; i >= 0; i --)
        res += (val >> i) & 1 ? "1" : "0";
    return res;
}

string deal(string str) // 解码字符串
{
    string res = "";
    if (str.length() >= 1 && str[0] != 'H')
        res = str;
    else if (str.length() >= 2 && str[0] == 'H' && str[1] == 'H')
        res = str.substr(1, str.length() - 1);
    else
    {
        // 获取二进制序列的补零个数
        string p_str = str.substr(str.length() - 2);
        int p_value = stoi(p_str, nullptr, 16);
        
        string hexStr = str.substr(1, str.length() - 3);    // Hex 序列
        string binStr = ""; // Bin 序列
        for (char c : hexStr) binStr += hexToBin(c);

        // 去除后补零
        binStr = binStr.substr(0, binStr.length() - p_value);

        auto curr = root;
        for (char c : binStr)
        {
            if (c == '0') curr = curr->left;
            else curr = curr->right;

            if (curr->data != '\0')
            {
                res += curr->data;
                curr = root;
            } 
        }
    }
    return res;
}

void work1(int mark)
{
    string k, v;
    if (mark >= 1 && mark <= s)     // 静态列表-key
        k = sl[mark - 1].x, v = sl[mark - 1].y;
    else
    {
        k = dl[mark - s - 1].x;
        v = dl[mark - s - 1].y;
    }
        
    cout << k << ": " << v << endl;
}

void work2(string k, string v)
{
    cout << k << ": " << v << endl;
    dl.push_front({k, v});
    if ((int)dl.size() > d) dl.pop_back();
}

void work3(string k, string v)
{
    cout << k << ": " << v << endl;
}

int main()
{
    string encodedTree = ""; // 示例：0表示内部节点，1表示叶子节点
    int index = 0;

    cin >> s >> d;
    for (int i = 0; i < s; i ++)    // 存储静态键值对
    {
        string key, value;
        cin >> key >> value;
        sl[i] = {key, value};
    }

    cin >> encodedTree;
    index = 0;
    root = rebuildHuffmanTree(encodedTree, index);
    // printTree(root);

    cin >> n;
    for (int i = 0; i < n; i ++)    // 处理指令
    {
        int idx;
        cin >> idx;
        if (idx == 1)
        {
            int k;
            cin >> k;
            work1(k);
        }
        else if (idx == 2)
        {
            int mark;
            cin >> mark;
            if (mark == 0)
            {
                string k, v;
                cin >> k >> v;
                k = deal(k), v = deal(v);
                work2(k, v);
            }
            else
            {
                string k, v;
                if (mark >= 1 && mark <= s)     // 静态列表-key
                    k = sl[mark - 1].x;
                else
                    k = dl[mark - s - 1].x;
                cin >> v;
                v = deal(v);
                work2(k, v);
            }
        }
        else if (idx == 3)
        {
            int mark;
            cin >> mark;
            
            if (mark == 0)
            {
                
                string k, v;
                cin >> k >> v;
                k = deal(k), v = deal(v);
                work3(k, v);
            }
            else
            {
                string k, v;
                if (mark >= 1 && mark <= s)     // 静态列表-key
                    k = sl[mark - 1].x;
                else                            // 动态列表-key
                    k = dl[mark - s - 1].x;
                cin >> v;
                v = deal(v);
                work3(k, v);
            }
        }
    }
    return 0;
}
```