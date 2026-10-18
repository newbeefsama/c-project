# 项目 2：单链表

## 🎯 你的任务

实现 `list.c` 里的 **9 个函数**。

**这是你第一个"动态数据结构"** —— 节点用 `malloc` 一个个申请出来，用指针串起来。

---

## 📁 文件说明

| 文件 | 你要动吗 | 说明 |
|---|---|---|
| `list.h` | ❌ 不要动 | 结构体定义 + 函数声明 |
| `list.c` | ✅ **只改这个** | 9 个空函数 |
| `test.c` | ❌ 不要动 | 34 个测试用例 |
| `run.bat` | ❌ 不要动 | 双击编译+运行 |

---

## 🚀 怎么用

1. 双击 **`run.bat`**
2. 起点：`PASS = 13, FAIL = 21`
3. 目标：**`PASS = 34, FAIL = 0`**
4. 建议按 1→9 的顺序做，**做完一个跑一次**

---

## 🧠 先理解链表长什么样

```
Node *head
   │
   ▼
┌─────────┐    ┌─────────┐    ┌─────────┐
│ data:10 │    │ data:20 │    │ data:30 │
│ next: ──┼───▶│ next: ──┼───▶│ next:NULL│
└─────────┘    └─────────┘    └─────────┘
```

**每个节点是一块 `malloc` 出来的内存**，里面装：
- 一个数据 `data`
- 一个指针 `next`，指向下一个节点

**最后一个节点的 `next` 是 `NULL`** —— 这就是"链表结束"的标志。

---

## ⭐ 关键概念：为什么有的参数是 `Node **head`

```c
void list_push_front(Node **head, int val);   /* 两个星！ */
int  list_size(Node *head);                    /* 一个星 */
```

**判断方法：这个函数需不需要"改 head 本身"？**

| 函数 | 要改 head 吗 | 参数 |
|---|---|---|
| `list_size` | 不改，只是读 | `Node *head` |
| `list_push_front` | **要改**（新节点成为新头） | `Node **head` |
| `list_remove` | **可能要改**（删的正好是头节点） | `Node **head` |
| `list_reverse` | **要改**（尾变成头） | `Node **head` |
| `list_free` | **要改**（置为 NULL） | `Node **head` |

> **C 是值传递。要改一个东西，就得传它的地址。**
> 改 `int` → 传 `int*`；改 `Node*` → 传 `Node**`。**这个道理和 LeetCode 的 `*returnSize = 2` 一模一样。**

---

## 📝 九个函数逐个说明

### 1. `list_create` —— 创建空链表（✅ 已提供，不用写）
```c
Node *list_create(void);
```
**空链表就是头指针是 `NULL`**，只有一行。这个我已经给你写好了，**你从第 2 个函数开始做**。

---

### 2. `list_push_front` —— 头插
```c
void list_push_front(Node **head, int val);
```
**三步：**
1. `Node *n = malloc(sizeof(Node));`
2. `n->data = val;  n->next = *head;`
3. `*head = n;`

⚠️ 顺序很重要：**先让新节点的 `next` 指向原来的头，再让 `*head` 指向新节点**。反了就把链表弄丢了。

---

### 3. `list_push_back` —— 尾插
```c
void list_push_back(Node **head, int val);
```
**分两种情况：**
- **空链表** → 新节点直接成为头（和头插一样）
- **非空** → 从头走到最后一个节点（`p->next == NULL`），把它的 `next` 指向新节点

⚠️ **用临时指针 `p` 走，别拿 `*head` 去走**——否则头指针就丢了。

---

### 4. `list_size` —— 节点个数
```c
int list_size(Node *head);
```
```c
int n = 0;
for (Node *p = head; p != NULL; p = p->next) n++;
return n;
```

---

### 5. `list_print` —— 打印
```c
void list_print(Node *head);
```
**格式要求：** `10 -> 20 -> 30 -> NULL`

```c
for (Node *p = head; p != NULL; p = p->next)
    printf("%d -> ", p->data);
printf("NULL\n");
```

---

### 6. `list_find` —— 查找
```c
Node *list_find(Node *head, int val);
```
找到就**返回那个节点的地址**，找不到返回 `NULL`。

⚠️ 返回的是**指针**，不是下标——链表没有下标。

---

### 7. `list_remove` —— 删除（最难）⭐
```c
int list_remove(Node **head, int val);
```
**三种情况，缺一不可：**

| 情况 | 怎么做 |
|---|---|
| **A. 链表为空** | `return 0;` |
| **B. 要删的是头节点** | `*head = (*head)->next;` 然后 `free` 掉旧的 |
| **C. 要删的在中间/末尾** | 用 `p` 走到**前一个**节点，`p->next = p->next->next;` 再 `free` |

⚠️ **`free` 之前一定要先把链子接好**，否则就断了。

**画图！** 在纸上画出 `10 -> 20 -> 30`，然后一步步画指针怎么变。

---

### 8. `list_reverse` —— 反转 ⭐
```c
void list_reverse(Node **head);
```
**经典三指针法：**
```c
prev = NULL;
cur  = *head;
while (cur != NULL) {
    next = cur->next;     /* ① 先存住下一个 */
    cur->next = prev;     /* ② 把箭头反过来 */
    prev = cur;           /* ③ prev 前进 */
    cur  = next;          /* ④ cur 前进 */
}
*head = prev;             /* ⑤ 最后 prev 就是新头 */
```

⚠️ **这四步的顺序绝对不能乱。**
- 如果先做 ② 再做 ①，你就找不到"下一个"了（`cur->next` 已经被改掉了）
- 在纸上画三个节点走一遍，就明白了

---

### 9. `list_free` —— 释放整条链表
```c
void list_free(Node **head);
```
```c
Node *cur = *head;
while (cur != NULL) {
    Node *next = cur->next;   /* 先记住下一个 */
    free(cur);
    cur = next;
}
*head = NULL;                 /* 别忘了置空 */
```

⚠️ **必须先存 `next` 再 `free`**——`free` 之后那块内存就不能再访问了。

---

## ✅ 完成标准

- [ ] `run.bat` 显示 **FAIL = 0**（34 个全过）
- [ ] 编译 **0 警告**
- [ ] `list_print` 那行输出是 `10 -> 20 -> 30 -> NULL`（人眼检查）
- [ ] 把这 4 个文件提交到 GitHub

---

## 💡 常见坑

| 坑 | 提醒 |
|---|---|
| **忘记 malloc 判空** | `if (n == NULL) return;` |
| **用 head 直接走** | 用临时指针 `Node *p = head;` |
| **先 free 再取 next** | 先存 next！否则访问已释放内存 |
| **反转顺序搞错** | 四步顺序：存 next → 改箭头 → 移 prev → 移 cur |
| **删除时忘了接链子** | `free` 前先 `p->next = p->next->next` |
| **内存泄漏** | 每个 `malloc` 都要有对应的 `free` |

---

## 🆘 卡住了怎么办

1. **在纸上画图** —— 链表题 90% 靠画图解决
2. 看 `test.c` 里对应的测试用例
3. 还是不行，把**代码 + 报错/失败信息 + 你试过什么**发我

**这个项目做完，你就真正理解指针了。**
