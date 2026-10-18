# 项目 2.5：单链表（哨兵版）

## 🎯 你的任务

**用哨兵节点重新实现一遍单链表**，然后和上一版对比。

**这不是重复劳动**——你会亲眼看到**代码少了多少**。

---

## 📁 文件说明

| 文件 | 你要动吗 | 说明 |
|---|---|---|
| `list.h` | ❌ 不要动 | **API 变了**，仔细看注释 |
| `list.c` | ✅ **只改这个** | 9 个函数 |
| `test.c` | ❌ 不要动 | 32 个测试 |
| `run.bat` | ❌ 不要动 | 双击运行 |

**起点**：会看到一条友好提示（因为 `list_create` 还没写）
**目标**：`PASS = 32, FAIL = 0`

⚠️ **先实现 `list_create`！** 它是哨兵版的入口，其他函数都依赖它。

---

## ⭐ 和上一版的关键区别

### 1. `list_create` 不再返回 NULL

```c
/* 上一版 */
Node *list_create(void) { return NULL; }

/* 哨兵版 */
Node *list_create(void)
{
    Node *sentinel = malloc(sizeof(Node));
    sentinel->next = NULL;      /* 还没有数据节点 */
    return sentinel;            /* ★ 返回一个真实节点，永远不为 NULL */
}
```

### 2. 大部分函数的参数从 `Node **` 变成了 `Node *`

| 函数 | 上一版 | 哨兵版 |
|---|---|---|
| `list_push_front` | `Node **head` | **`Node *head`** |
| `list_push_back` | `Node **head` | **`Node *head`** |
| `list_remove` | `Node **head` | **`Node *head`** |
| `list_reverse` | `Node **head` | **`Node *head`** |
| `list_free` | `Node **head` | `Node **head` ← 这个还是要两个星 |

**为什么 `list_free` 例外？** 因为要把**调用方的变量**置为 NULL——这个需求哨兵解决不了。

**为什么其他函数不需要了？** 因为**头指针永远指向哨兵，永远不用改**。

### 3. 数据结构变成这样

```
        ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐
head ──→│ 哨兵      │    │ data:10  │    │ data:20  │    │ data:30  │
        │ data:废  │    │          │    │          │    │          │
        │ next: ───┼───→│ next: ───┼───→│ next: ───┼───→│ next:NULL│
        └──────────┘    └──────────┘    └──────────┘    └──────────┘
         ↑
    永远存在，数据从 head->next 开始
```

### 4. 最爽的地方：`remove` 不用特判了

```c
/* 上一版：三种情况，一路 if-else */
if (*head == NULL) return 0;
if ((*head)->data == val) { ...删头节点... }
Node *p = *head;
while (p->next) { ... }

/* 哨兵版：一个循环，没有任何特判 */
Node *p = head;
while (p->next != NULL && p->next->data != val)
    p = p->next;
if (p->next == NULL) return 0;
Node *old = p->next;
p->next = old->next;
free(old);
return 1;
```

**为什么？** 因为**哨兵永远不会被删**——"删第一个数据节点"和"删中间节点"变成了同一件事。

---

## 📊 对比任务（这是本项目最重要的部分）

写完哨兵版之后，**打开两个文件并排对比**，填下面这张表：

| 对比项 | 项目 2（普通版） | 项目 2.5（哨兵版） |
|---|---|---|
| `list.c` 总行数（不含注释） | | |
| `list_push_back` 行数 | | |
| `list_remove` 行数 | | |
| `list_reverse` 行数 | | |
| 用 `Node **` 的函数个数 | | |
| `list_remove` 里的 `if` 分支数 | | |
| `list_push_back` 里的 `if` 分支数 | | |

**然后回答三个问题：**

1. **一共少了多少行？**
2. **哪些 `if` 分支彻底消失了？**
3. **为什么哨兵能消灭这些分支？**

**把答案写在学习日志里。**

---

## 💡 实现提示

### `list_create`
```c
Node *sentinel = malloc(sizeof(Node));
if (sentinel == NULL) return NULL;
sentinel->next = NULL;
return sentinel;
```

### `list_push_front`
```c
n->next = head->next;     /* 新节点接在原来的第一个数据节点前面 */
head->next = n;           /* 哨兵指向新节点 */
/* 不用特判空链表！head->next 是 NULL 也没关系 */
```

### `list_push_back`
```c
Node *p = head;
while (p->next != NULL) p = p->next;   /* 空链表时一步都不走，p 就是 head */
p->next = n;
/* 没有 if！ */
```

### `list_remove`
```c
Node *p = head;
while (p->next != NULL && p->next->data != val)
    p = p->next;
if (p->next == NULL) return 0;
Node *old = p->next;
p->next = old->next;
free(old);
return 1;
/* 没有 if (*head == NULL)，也没有 if ((*head)->data == val) */
```

### `list_reverse`
```c
Node *prev = NULL;
Node *cur = head->next;         /* 从第一个数据节点开始 */
while (cur != NULL) { ... 三指针 ... }
head->next = prev;              /* 哨兵的 next 指向新的第一个数据节点 */
```

### `list_free`
**和上一版一样**，只是别忘了哨兵本身也要 free。

---

## ✅ 完成标准

- [ ] `run.bat` 显示 **FAIL = 0**（32 个全过）
- [ ] 编译 **0 警告**
- [ ] `list_print` 输出 `10 -> 20 -> 30 -> NULL`（**不能多打印哨兵的值**）
- [ ] **填完上面那张对比表**
- [ ] 两个项目一起提交 GitHub

---

## ⚠️ 三个最容易错的地方

| 坑 | 提醒 |
|---|---|
| **忘了从 `head->next` 开始** | 遍历、查找、打印、size 都要从第二个节点开始 |
| **`list_size` 把哨兵数进去** | 结果会比正确答案多 1 |
| **`list_print` 打印了哨兵的值** | 输出会变成 `0 -> 10 -> 20 -> 30 -> NULL` |

---

## 🎁 做完这个项目，你会得到什么

1. **两种链表实现都会写**——面试时被问"空链表怎么处理"你能答出来
2. **理解"哨兵为什么能消灭边界情况"**——这是数据结构设计的核心思想
3. **知道什么时候用哨兵、什么时候不用**——不是所有场景都值得加哨兵

> **先学难的方式，再用简单的方式优化——这样你两种能力都有。**
