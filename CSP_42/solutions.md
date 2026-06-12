# CSP(第42次CCF计算机软件能力认证)

> smqyOJ Judge((10/10) + (22/22) + (11/28))
> CCF CSP(100 + 100 + 40)

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