﻿# jugg

词法分析-语法分析-语义分析 实现一个自定义语言的前端分析阶段，目前处于探索阶段

词法分析利用c++11 regex实现

语法分析手写（没有用LL和LR分析，感觉手写更加方便和可控）

##### 2020-10-16：
* 修复了逻辑运算符，与c/c++保持一致。
* 增加关键字void，因为函数调用是一个表达式而不是语句，所以必须有返回值，void类型的作用是当函数没有返回值的时候作为占位符。
* 类型系统中增加了内置类型，用于检测函数定义的返回值类型是否合法。
* 在表达式解析模块中增加函数调用表达式的解析。

##### 2020-10-17
* 增加作用域结构，服务于名字查找与变量绑定。