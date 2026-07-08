# CSP(第42次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + (22/22) + (11/28) + (21/42))
> CCF CSP(100 + 100 + 40 + 60)

## Q1 银行家舍入

### Q1 算法思路

- 模拟即可
- `get(x)` 实现四舍五入的舍入处理
- 该题解可以通过 smqyOJ (10/10) 的测试点，CCF 认证系统得分 100 分

### Q2 代码实现

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 110;

int n;
double a[N];

int get(double x)
{
	int base = (int)x;
	int flag = (int)(x * 10) % 10;
	if (flag >= 0 && flag <= 4) return base;
	else return base + 1;
}

int main()
{
	cin >> n;
	for (int i = 0; i < n; i ++) cin >> a[i];
	
	for (int i = 0; i < n; i ++)
	{
		double x = a[i];
		printf("%d ", get(x));
	}
	cout << "\n";
	for (int i = 0; i < n; i ++)
	{
		double x = a[i];
		if ((int)(x * 10) % 10 != 5) printf("%d ", get(x));
		else
		{
			int base = (int)x;
			if (base % 2 == 0) printf("%d ", base);
			else printf("%d ", base + 1);
		}
	}
	return 0;	
}
```

## Q2 机器人宿管指南

### Q2 算法思路

- 二分答案(机器人的数量)，机器人数量越少，满足够吃 `m` 天的条件就越有可能；反之，则越不可能。呈现出单调性，适合二分
- `check(cnt)` 判断当机器人数量为 `cnt` 时，是否满足吃 `m` 天的条件
- 该题解可以通过 smqyOJ (22/22) 的测试点，CCF 认证系统得分 100 分

### Q2 代码实现

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

LL n, k, m;	// 初始苹果数量、每日损耗比例，食用天数 

LL getOut(LL x)	// 上取整获取损耗值 
{
	LL res = (x * k + 99) / 100;
	return res;
}

bool check(int cnt)
{
	LL cntApple = n;
	for (int i = 0; i < m; i ++)
	{
		cntApple -= getOut(cntApple);
		cntApple -= cnt;
		if (cntApple < 0) return false;
	}
	return true;
}

int main()
{
	scanf("%lld%lld%lld", &n, &k, &m);
	LL l = 1, r = 1e+9;
	while(l < r)
	{
		int mid = (l + r + 1) / 2;
		if (check(mid)) l = mid;
		else r = mid - 1;	
	} 
	printf("%lld", l); 
	return 0;
}
```

## Q3 死锁优化

### Q3 算法思路(demo1, subtask-40%)

- 考场上的模拟做法，仅针对前 40% 的数据，数据结构定义有点凌乱~
- 每段时间内，枚举每个进程，判断是否能够执行当前任务；如果能执行，则占有资源，收益增加；如果不能执行，则等待，收益不变
- 每段时间结束后，检查是否有进程完成，如果有，则释放资源
- 该题解可以通过 smqyOJ (11/28) 的测试点，CCF 认证系统得分 40 分

### Q3 代码实现(demo1, subtask-40%)

```cpp
#include <bits/stdc++.h>
#define x first
#define y second
using namespace std;
typedef pair<int, int> PII;
const int N = 15, M = 45;

int n, m;	// 进程数量、资源数量

char proClass[N];	// 进程种类 
vector<PII> proTasks[N];	// 进程任务队列 <需要资源，任务持续段数> 
int proTasksNum[N]; 	// 进程的任务队列长度

bool isOver[N];
int point[N];	// 工作指针
 
bool state[N];	// 进程当前的状态
int proValue[N];	// 进程的收益
long long cntSeg[N];	// 进程的运行段数 

set<int> holdResList[N];	// 进程持有的资源序列
bool isFree[M];		// 资源是否可用

PII runState[N]; 	//  <当前进程运行到其任务几，该任务还需要运行多少段>

bool isGetRes(int res)	// 能否获取资源 res 
{
	if (isFree[res]) return true;
	else return false;
}

void work()
{
	memset(cntSeg, -1, sizeof cntSeg);
	memset(isFree, true, sizeof isFree);
	
	for (int i = 1; i <= n; i ++)	// 初始化每一个进程的运行状态 
	{
		if (proTasksNum[i] > 0) runState[i] = {0, proTasks[i][0].y};	
		else runState[i] = {-1, -1};
	} 
	
	int value[N] = {0};
	long long seg[N] = {0}; 
	vector<int> overPro;
	for (int i = 1; i <= 8020; i ++)	// 枚举段
	{
		overPro.clear();
		for (int pro = 1; pro <= n; pro ++)	// 枚举进程
		{
			if (runState[pro].x == -1) continue;	// 无任务
			if (isOver[pro]) continue;	// 任务结束
			if (i < point[pro]) continue;	// 进程未开始
			
			int res = proTasks[pro][runState[pro].x].x;
			if (isGetRes(res) || holdResList[pro].count(res) >= 1)	// 资源空闲或本身占有 
			{
				holdResList[pro].insert(res);
				isFree[res] = false;
				
				runState[pro].y --;
				value[pro] += holdResList[pro].size(); 
				seg[pro] ++;
			}
			else	// 无法执行 
			{
				// runState[pro].y ++;	
				seg[pro] ++;
			}
			proValue[pro] = value[pro];
			
			// 切换下一个任务
			if (runState[pro].y == 0)
			{
				runState[pro].x ++;
				if (runState[pro].x >= proTasksNum[pro])	// 进程结束 
				{
					isOver[pro] = true;
					cntSeg[pro] = seg[pro];
					overPro.push_back(pro);
				}
				else
				{
					runState[pro].y = proTasks[pro][runState[pro].x].y;
				}
			}
		}
		
		// 归还资源
		for (int id = 0; id < overPro.size(); id ++)
		{
			int pro = overPro[id];
			for (int res = 1; res <= m; res ++)
				if (holdResList[pro].count(res) >= 1)
					isFree[res] = true;
			holdResList[pro].clear();
		}
	} 
}

int main()
{
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i ++)
	{
		char c;
		cin >> c;
		if (c == 'X')
		{
			proClass[i] = c;	// 类别打标签 
			int startSeg, cntTask;
			cin >> startSeg >> cntTask;
			
			proTasksNum[i] = cntTask; 
			point[i] = startSeg;
			for (int j = 0; j < cntTask; j ++)
			{
				int res, cntSeg;
				cin >> res >> cntSeg;
				PII t;
				t.first = res, t.second = cntSeg;
				proTasks[i].push_back(t);
			}
		}
		else
		{
			proClass[i] = c;
			int startSeg, w, cntTask;
			cin >> startSeg >> w >> cntTask;
			
			proTasksNum[i] = cntTask;
			point[i] = startSeg;
			for (int j = 0; j < cntTask; j ++)
			{
				int res, cntSeg;
				cin >> res >> cntSeg;
				PII t;
				t.first = res, t.second = cntSeg;
				proTasks[i].push_back(t);
			}
		}
	}
	
	work(); 
		
	for (int i = 1; i <= n; i ++)
		printf("%d %lld\n", proValue[i], cntSeg[i]);
	return 0;
}
```

## Q4 石子游戏

### Q4 算法思路(demo1, subtask1)

- 贪心算法(区间调度): 本题本质上是一个经典的"活动安排问题(区间调度问题)"。想要在有限的区间 $[l, r]$ 内切出尽可能多的合法子游戏，遵循贪心思路是: "永远选择结束位置(右端点)最早的合法区间"。因为右端点越靠前，留给后面未分配数组的空间就越大，能容纳后续子游戏的可能性就越高。
- 针对每次查询 $[l, r]$，使用游标 `cur` 表示当前尚未分配的数组起点。贪心处理流程如下: 
  - 设定全局最优目标: 对于当前的剩余区间 $[cur, r]$，定义 `best_r` 来记录能够找到的最早结束的合法右端点，初始值设为无穷大(`r + 1`)。
  - 暴力枚举所有可能(双重循环)：对于外层循环 `bl`，枚举当前剩余区间内所有可能的左端点(从 `cur` 到 `r`)。因为最优解不一定非要从 `cur` 开始，跳过某些必输的元素可能会换来更早的结束时间。对于内层循环 `br`，基于当前确定的左端点 `bl`，以步长为 `2` 向后枚举右端点，并实时维护奇数位置的异或和 `oxr_sum`。
  - 捕获最早右端点: 一旦发现 `oxr_sum == 0`，说明找到了一个合法的子游戏 $[bl, br]$。此时便可以尝试用 `br` 去更新全局最小的 `best_r`。更新后立刻 `break`，因为对于同一个左端点 `bl`，往后找的右端点只会更大，没有比较的意义。
  - 贪心切割与状态转移: 如果遍历结束后 `best_r <= r`(说明至少找到了一个合法区间)，此时，便可以把这一刀切下去：答案 `cnt++`，并且将游标 `cur` 瞬间移动到 `best_r + 1`，开启下一轮寻找。(注：那些在 `cur` 到 `bl-1` 之间被跳过的元素，隐式地成为了长度为 1 且异或和不为 0 的必输废区间，完全符合题意。)如果 `best_r > r`，说明在剩下的所有元素里再也凑不出任何一个必胜区间了，直接 `break` 结束当前查询。
- 时间复杂度: $O(q \times n^3)$
- 该题解可以通过 smqyOJ (11/42) 的测试点，得分 30 分

### Q4 代码实现(demo1, subtask1)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N = 1e+6 + 10;

int n, q;
int l, r;
int b[N];

int main()
{
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i ++) scanf("%d", &b[i]);

    while (q --)
    {
        int cnt = 0;
        scanf("%d%d", &l, &r);

        int cur = l;	// 处理起点 cur
        while (cur <= r)
        {
            int best_r = r + 1;
            for (int bl = cur; bl <= r; bl ++)  // 枚举所有可能的左端点
            {
                int oxr_sum = 0;
                for (int br = bl; br <= r; br += 2)	// 枚举所有可能的右端点
                {
                    oxr_sum ^= b[br];
                    if (oxr_sum == 0)	// 判定 [bl, br] 是否合法
                    {
                        best_r = min(best_r, br);
                        break;
                    }
                }
            }

            if (best_r <= r) 
            {
                cur = best_r + 1;
                cnt ++;
            }
            else break;
        }

        printf("%d\n", cnt);
    }
    return 0;
}
```

### Q4 算法思路(demo2, subtask1&2)

- 在 `demo1` 的纯暴力算法中，每次处理查询 $[l, r]$ 时，都需要在区间内反复通过双重 `for` 循环寻找"最早合法的右端点"。考虑到仓库数组 `b` 在整个生命周期中是静态不修改的，同一段区间的答案无论被查询多少次都不会改变。可以引入动态规划(DP)思想，在查询开始前，花一次性的时间将所有的"最优切割点"预处理出来，实现查询时的 $O(1)$ 查表跳跃。
- `R[i]` 数组是局部最优解：强制以第 `i` 个仓库作为起点，向后能够找到的最早合法结束位置(即 $b_i \oplus b_{i+2} \dots = 0$ 成立的最小下标)。若找不到则记为无穷大 `INF`。
- `nxt[i]` 数组是全局最优解：在所有大于等于 `i` 的可能起点中，能够产生的全局最早合法右端点。
- 状态转移方程：`nxt[i] = min(R[i], nxt[i + 1])`：既然可以跳过某些必输的元素，那么在 $\ge i$ 的范围内，最优切法只有两种可能：要么以当前的 `i` 作为起点切(对应 `R[i]`)，要么把 `i` 当作废块抛弃，沿用 `i + 1` 往后的最优切法(对应 nxt[i + 1])。
- 时间复杂度: $O(n^2 + q \times n)$
- 该题解可以通过 smqyOJ (21/42) 的测试点，得分 60 分

### Q4 代码实现(demo2, subtask1&2)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int inf = 1e+6 + 1;
const int N = 1e+6 + 10;

int n, q;
int l, r;
int b[N];
int R[N];	// R[i] 表示严格以 i 为左端点，最早合法的右端点，
int nxt[N];	// nxt[i] 表示左端点 >= i 的范围里，最早合法的右端点

int main()
{
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i ++) scanf("%d", &b[i]);

    // 预处理 R[]
    for (int i = 1; i <= n; i ++)
    {
        R[i] = inf;
        int xor_sum = 0;
        for (int j = i; j <= n; j += 2)
        {
            xor_sum ^= b[j];
            if (xor_sum == 0)
            {
                R[i] = j;
                break;
            }
        }
    }

	// 预处理 nxt[]
    nxt[n + 1] =  inf;
    for (int i = n; i >= 1; i --)
        nxt[i] = min(R[i], nxt[i + 1]);	// 状态转移: 要么用当前 i 为起点的最优解，要么用 i 之后的最优解

    while (q --)
    {
        int cnt = 0;
        scanf("%d%d", &l, &r);

        int cur = l;
        while (cur <= r && nxt[cur] <= r)
        {
            cnt ++;
            cur = nxt[cur] + 1;
        }
        printf("%d\n", cnt);
    }
    return 0;
}
```
