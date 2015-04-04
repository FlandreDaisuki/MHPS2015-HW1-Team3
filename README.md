##Minimizing Makespan in Permutation Flow Shops using Three Trajectory-based Metaheuristics

###問題定義
有一排程(Schedule)有j個工作、m台機器，每台機器對個別工作的工作時間為Tmj。
每個工作必須依固定順序給m台機器做，每台機器也只能做一件事，而且是先進入先處理(First In First Serve)。
現有一工作序列則可求出該工作序列的完成時間(或持續時間)，求在固定機器順序的情況下，可得最小完成時間的工作序列。

###演算法

####禁忌搜尋法(Tabu Search)
禁忌搜尋法會藉由參考禁忌列表(tabu list)接受比現在還要差的解，作為跳脫區域最佳值(local optima)的手段

#####Psuedo Code

    初始化禁忌列表
    迴圈特定代數 {
        在目前的排程序列中參考禁忌列表找到最好的鄰居
        拜訪該鄰居
        將該鄰居放入禁忌列表
    }

    tabulist.init()
    for i = 0:iterations {
        nb = schedule.find_best_neighbor(tabulist);
        schedule.visit_to(nb);
        tabulist.push(nb);
    }
    
#####演算法參數

- 解表示法：最短工作時程(minimum makespan)
- 鄰域函式：交換任兩個工作
- 鄰域大小：CJob取2
- 接受條件：當解未在禁忌列表中並且在鄰域中取最佳解
- 渴望條件：解比禁忌列表中的解都還要好(小)
- 終止條件：演化1000代

#####實作細節

- Neighbor
  - 在此鄰域函式中，鄰居為一解與目前解相差一次工作的交換，而且鄰居必有其目標值
	```
	Neighbor
	- job1
	- job2 //使job1恆小於job2
	- obj_value
	```
- Tabulist
  - 禁忌列表為鄰居的Queue
  - 禁忌列表會在加入新Neighbor時檢查並紀錄最佳解，使檢查渴望條件(aspiration criterion)時更快
  - 禁忌列表能知道現在禁止的種類是目標值(Objective value)或是工作對(Job pair/Swap pair)來檢查鄰居是否被禁止

- Schedule
  - Schedule為一種排序排程解，由於這個鄰域函式可以使編碼解碼(encoding/decoding scheme)可以幾乎不做
  - Schedule直接以2D vector實作
  - 目標函式(Objective function)以DP實作

    ```
	for m = 1:machine {
		for j = 1:job {
			if( j==1 && m==1 ) {
				obj[m][j] = Schedule[m][j]
			} else if ( j==1 && m!=1 ) {
				obj[m][j] = Schedule[m][j] + obj[m-1][j]
			} else if ( j!=1 && m==1 ) {
				obj[m][j] = Schedule[m][j] + obj[m][j-1]
			} else {
				obj[m][j] = Schedule[m][j] + max(obj[m][j-1], obj[m-1][j])
			}
		}
	}
	return obj[machine][job]

    ```
  - 故[Psuedo Code](#psuedo-code)中拜訪鄰居的實作流程為下

	```
	min_obj_value = 1e9
	for j1 = 1:job-1 {
		for j2 = j1+1:job {
			Schedule.Swap(j1, j2)                             // 拜訪鄰居
			obj_value = Schedule.Calculate()                  // 計算鄰居目標值
			if(Neighbor(j1, j2, obj_value) NOT in tabulist
			    or obj_value better than the best one of tabulist) {
				                                              // 鄰居不在禁忌列表中或鄰居優於禁忌列表中的最佳解 為合法鄰居
				min_obj_value = min(min_obj_value, obj_value) // 紀錄合法鄰居中最佳解
			}
			Schedule.Swap(j1, j2)                             // 道別鄰居
		}	
	}
    return the neighbor has min_obj_value
	```

###實驗與結果

#####實驗參數

- 放入禁忌列表的種類
  - 目標值(Objective value)
  - 工作對/交換對(Job pair/Swap pair)

- 禁忌列表的長度
  - 小型(約為鄰域大小的1/8)
  - 中型(約為鄰域大小的1/4)
  - 大型(約為鄰域大小的1/2)
  - 動態(從小型到大型規律動態地變化長度)

- 測資規模：
  - 工作：20, 50, 100
  - 機器：5, 10, 20

- 測資出處
  -  Taillard E (1993) Benchmarks for basic scheduling problems. Eur. J. Oper. Res. 64(2), 278–285

- 測試環境
  - OS：Linux Mint 17.1 MATE 64-bit
  - Processor：AMD FX(tm)-4170 Quad-Core Processor x 2
  - RAM：7.8 GiB

#####實驗結果
表示法：Max / min / average / standard deviation

**禁忌目標值**

|測資大小\\禁忌列表長度| 小型 | 中型 | 大型 | 動態 |
|:--------------------:|:----:|:----:|:----:|:----:|
| jobs_machines |               20              |               50              |              100              |             20~100            |
|     20_5      | 1297 / 1278 / 1295.2  /  5.37 | 1297 / 1294 / 1296.94 /  0.42 | 1297 / 1286 / 1296.78 / 15.56 | 1297 / 1279 / 1296.08 /  3.71 |
|     20_10     | 1611 / 1582 / 1590.34 /  6.54 | 1618 / 1583 / 1596.18 /  7.29 | 1620 / 1582 / 1602.44 / 10.32 | 1613 / 1586 / 1599.18 /  7.26 |
|     20_20     | 2359 / 2298 / 2326.5  / 19.12 | 2362 / 2297 / 2317.36 / 11.57 | 2349 / 2308 / 2325.94 /  9.38 | 2360 / 2305 / 2324.38 / 10.66 |
| jobs_machines |              100              |               300             |              600              |            100~600            |
|     50_5      | 2752 / 2729 / 2740.34 / 10.38 | 2774 / 2724 / 2753.3  / 16.11 | 2774 / 2729 / 2749.1  / 15.44 | 2795 / 2733 / 2755.14 / 14.33 |
|     50_10     | 3139 / 3052 / 3098.52 / 19.69 | 3193 / 3063 / 3135.66 / 22.64 | 3176 / 3080 / 3120.26 / 23.38 | 3201 / 3063 / 3125.58 / 28.79 |
|     50_20     | 4041 / 3966 / 3997.16 / 15.28 | 4104 / 3963 / 4023.1  / 28.09 | 4066 / 3971 / 4016.68 / 20.61 | 4080 / 3975 / 4020.14 / 27.38 |
| jobs_machines |              500              |              1000             |             2000              |           500~2000            |
|     100_5     | 5565 / 5495 / 5527.2  / 16.91 | 5603 / 5500 / 5540.86 / 23.54 | 5565 / 5495 / 5543.68 / 20.06 | 5577 / 5505 / 5540.44 / 19.28 |
|     100_10    | 5949 / 5802 / 5879.38 / 31.71 | 5990 / 5814 / 5895.38 / 36.77 | 5979 / 5813 / 5907.02 / 36.90 | 5997 / 5830 / 5904.82 / 40.76 |
|     100_20    | 6636 / 6476 / 6538.7  / 36.30 | 6666 / 6454 / 6556.74 / 48.56 | 6653 / 6478 / 6561.06 / 35.56 | 6647 / 6464 / 6546.82 / 38.53 |

**禁忌工作對**

|測資大小\\禁忌列表長度| 小型 | 中型 | 大型 | 動態 |
|:--------------------:|:----:|:----:|:----:|:----:|
| jobs_machines |               20              |               50              |              100              |             20~100            |
|     20_5      | 1297 / 1278 / 1295.86 /  4.56 | 1297 / 1278 / 1295.68 /  4.58 | 1297 / 1278 / 1295.86 /  4.56 | 1297 / 1278 / 1295.42 /  4.95 |
|     20_10     | 1600 / 1583 / 1589.26 /  4.65 | 1611 / 1583 / 1596.8  /  5.47 | 1624 / 1586 / 1607.4  /  8.42 | 1614 / 1585 / 1599.52 /  6.86 |
|     20_20     | 2351 / 2297 / 2318.08 / 13.90 | 2356 / 2297 / 2319.72 / 10.48 | 2346 / 2303 / 2326.74 /  9.74 | 2363 / 2305 / 2324.96 / 10.56 |
| jobs_machines |              100              |               300             |              600              |            100~600            |
|     50_5      | 2752 / 2724 / 2735.9  /  8.87 | 2752 / 2724 / 2730.04 /  6.23 | 2742 / 2724 / 2729.94 /  5.39 | 2745 / 2724 / 2729.04 /  4.03 |
|     50_10     | 3126 / 3032 / 3063.48 / 16.04 | 3096 / 3042 / 3059.34 / 10.13 | 3108 / 3046 / 3065    / 11.59 | 3108 / 3041 / 3064.98 / 12.74 |
|     50_20     | 3962 / 3917 / 3941.86 / 11.62 | 3976 / 3927 / 3956.1  / 11.37 | 3985 / 3939 / 3966.42 / 10.93 | 3989 / 3928 / 3961.36 / 13.05 |
| jobs_machines |              500              |              1000             |             2000              |           500~2000            |
|     100_5     | 5546 / 5495 / 5518.88 / 14.39 | 5527 / 5493 / 5518.64 / 14.05 | 5527 / 5495 / 5520.6  / 12.93 | 5527 / 5495 / 5522.7  / 10.40 |
|     100_10    | 5912 / 5785 / 5827.2  / 27.75 | 5879 / 5784 / 5825.82 / 23.82 | 5912 / 5784 / 5817.02 / 25.22 | 5876 / 5786 / 5822.22 / 24.52 |
|     100_20    | 6470 / 6348 / 6417.66 / 24.59 | 6448 / 6363 / 6407.38 / 22.44 | 6468 / 6358 / 6408.98 / 23.40 | 6467 / 6358 / 6404.8  / 25.02 |

#####實驗小結

_禁忌種類在不同禁忌列表長度中的影響_
- 小型

   我們可以觀察到兩者求出來的最小值相近，尤其在小測資時幾乎沒有影響
- 中型

   和小型結論差不多，值得一提的是在禁忌目標值20_5的測資中，幾乎清一色都是1297
- 大型

   同上，即使將禁忌列表的長度拉到鄰域函式個數的1/2，這兩種禁忌種類還是互有勝負
- 動態

   在動態的時候發現使用禁忌工作對求得的最佳值皆不輸禁忌目標值了

其實可以看的出來禁忌工作對出來的目標值整體而言是較優於禁忌目標值的。
個人推測是因為工作對的範圍是Cjob取2種，而我設計的禁忌列表長度也是起此為基準去調整；
但目標值的範圍是1種到job!種，禁忌列表長度遠小於此才會比較容易在區域最佳解附近跳來跳去。

###結論
