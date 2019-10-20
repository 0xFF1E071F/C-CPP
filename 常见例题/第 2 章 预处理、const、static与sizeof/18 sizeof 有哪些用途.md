# sizeof 有哪些用途

考点：理解sizeof的用途

1. 存储分配

例如，

```
1 void* malloc(size_t size);

```

2. 和I/O系统那样的例程进行通信

```
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
```

3. 查看某类型的对象在内存中所占的单元字节

```
void* memset(void* s, int c, sizeof(s));
```

在动态分配一对象时，可以让系统知道要分配多少内存。

便于一些类型的扩充，在 Windows 中很多结构类型就有一个专用的字段是用来放该类型的字节大小的。

由于操作数的字节数在实现时可能出现变化，建议在涉及操作数字节大小时用 sizeof 来代替常量计算。

如果操作数是函数中的`数组形参`或`函数类型的形参`，则 sizeof 给出`其指针的大小`。

