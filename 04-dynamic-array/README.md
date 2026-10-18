# 项目 3：动态数组

## 🎯 你的任务

实现 `da.c` 里的 **8 个函数**。

**核心是 `da_push` 里的「扩容」**——**这就是 C++ `vector` 的原理。**

---

## 📁 文件说明

| 文件 | 你要动吗 | 说明 |
|---|---|---|
| `da.h` | ❌ 不要动 | 结构体 + 声明 |
| `da.c` | ✅ **只改这个** | 8 个函数 |
| `test.c` | ❌ 不要动 | 30+ 个测试 |
| `run.bat` | ❌ 不要动 | 双击运行 |

**起点**：友好提示（因为 `da_create` 还没写）
**目标**：`PASS = 全部, FAIL = 0`

---

## 🧠 什么是动态数组

**普通数组的问题：** 长度定死了，写 `int a[10]` 就只能装 10 个。

**动态数组的做法：** 先申请一块，**装满了就申请一块更大的，把数据搬过去。**

```
struct {
    int *data;        /* 指向真正存数据的数组 */
    int  size;        /* 现在装了几个 */
    int  capacity;    /* 最多能装几个 */
}
```

**`size` 和 `capacity` 的区别是理解这个项目的关键：**

```
data ──→ [10][20][30][ ][ ][ ][ ][ ]
          └── size = 3 ──┘
          └────── capacity = 8 ──────┘
```

---

## ⭐ 核心：扩容过程（画出图就好懂了）

### 扩容前（size=4, capacity=4）

```
data ──→ [10][20][30][40]
          └── 满了 ──────┘
```

### 现在 push(50)

**第 1 步：计算新容量**
```c
int newCap = a->capacity * 2;      /* 4 -> 8 */
```

**第 2 步：申请更大的空间，把旧数据搬过去**
```c
int *tmp = realloc(a->data, newCap * sizeof(int));
```

```
realloc 之前:
  data ──→ [10][20][30][40]      （旧的 4 格）

realloc 之后:
  data ──→ [10][20][30][40][ ][ ][ ][ ]   （新的 8 格，前 4 格数据保留）
                            ↑ 新空间，内容是垃圾
```

**⚠️ `realloc` 可能会把数据搬到一块全新的内存地址去**——所以旧的 `data` 指针不能再用，必须用 `realloc` 返回的新地址。

**第 3 步：更新容量，写入新元素**
```c
a->capacity = newCap;
a->data[a->size] = val;      /* 写到第 5 格（下标 4） */
a->size++;
```

```
data ──→ [10][20][30][40][50][ ][ ][ ]
          └──── size = 5 ────┘
          └────── capacity = 8 ────────┘
```

### 为什么要"翻倍"而不是"每次 +1"？

| 策略 | 连续 push n 个的总代价 |
|---|---|
| 每次 +1 | **O(n²)** ——每次都要搬 n 个元素 |
| **每次 ×2** | **O(n)** ——平均每次只搬 1 个元素 |

**这就是"摊还分析"（amortized analysis）的经典例子。** 选 ×2 不是随便定的。

---

## ⚠️ `realloc` 的致命陷阱

```c
/* ❌ 危险写法 */
a->data = realloc(a->data, newCap * sizeof(int));
if (a->data == NULL) return;
```

**如果 `realloc` 失败返回 `NULL`，你原来的 `a->data` 就被覆盖成 `NULL` 了——旧数组的地址丢了，永远无法 free → 内存泄漏。**

```c
/* ✅ 正确写法 */
int *tmp = realloc(a->data, newCap * sizeof(int));
if (tmp == NULL) return;          /* 失败就原样退出，a->data 没被动 */
a->data = tmp;                    /* 成功了才更新 */
```

> **规则：`realloc` 的返回值必须先存到临时变量里，成功了再赋值回去。**

---

## 📝 八个函数提示

### 1. `da_create`
```c
DynArray *a = malloc(sizeof(DynArray));
if (a == NULL) return NULL;
a->data = malloc(DA_INIT_CAPACITY * sizeof(int));
if (a->data == NULL) { free(a); return NULL; }   /* 失败要把前面申请的也释放 */
a->size = 0;
a->capacity = DA_INIT_CAPACITY;
return a;
```

### 2. `da_push`（核心）
**先判断满没满，满了就扩容，然后再写入。**

### 3. `da_get`
```c
if (i < 0 || i >= a->size) return -1;    /* ⚠️ 用 size，不是 capacity */
return a->data[i];
```

**为什么用 `size` 不用 `capacity`？** 因为下标 `4` 到 `7` 那几格虽然分配了，但**还没有数据**——读它们是没意义的。

### 4. `da_set`
同理，越界返回 0，成功返回 1。

### 5-6. `da_size` / `da_capacity`
直接返回结构体里的字段。

### 7. `da_print`
```c
printf("[");
for (int i = 0; i < a->size; i++) {
    printf("%d", a->data[i]);
    if (i != a->size - 1) printf(", ");
}
printf("]\n");
```

### 8. `da_free`
```c
if (a == NULL) return;
free(a->data);      /* 先释放内部的数组 */
free(a);            /* 再释放结构体本身 */
```

**⚠️ 顺序不能反！** 先 `free(a)` 的话，`a->data` 就找不到了 → 内存泄漏。

---

## ✅ 完成标准

- [ ] `run.bat` 显示 **FAIL = 0**
- [ ] 编译 **0 警告**
- [ ] `da_print` 输出 `[10, 20, 30]`
- [ ] **扩容轨迹是 `4 -> 8 -> 16 -> 32`**（测试会检查）
- [ ] 提交 GitHub

---

## 💡 常见坑

| 坑 | 后果 |
|---|---|
| `realloc` 返回值直接赋给 `a->data` | 失败时内存泄漏 |
| 扩容后忘了更新 `capacity` | 下次又扩容，容量不涨 |
| `da_get` 用 `capacity` 判断越界 | 读到未初始化的垃圾数据 |
| `da_free` 顺序反了 | 内存泄漏 |
| 扩容放在写入**之后** | 写了一格越界的内存 |

---

## 🆘 卡住了怎么办

1. **先把扩容的三步在纸上写清楚**：算新容量 → realloc → 更新结构体
2. 看 `test.c` 里"扩容轨迹"那一段，它告诉你期望的容量序列
3. 还是不行，把**代码 + 报错/失败信息 + 你试过什么**发我

---

## 🎁 做完这个项目你会懂

- **`malloc` vs `realloc` 的区别和陷阱**
- **为什么 `vector` 的 `push_back` 是 O(1) 摊还**
- **C++ `vector`、Java `ArrayList`、Python `list` 的底层原理**

**这是"数据结构"里第一个真正体现"设计权衡"的例子。**
