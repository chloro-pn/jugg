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
* 类型系统初步设计

##### 2020-10-18
* 修改类型系统与变量系统的绑定方式。
* 完成type块的解析。
* return语句现在可以跟表达式作为返回值。
* 修复ParseType函数括号不匹配的bug。
* 增加注释的语法，目前仅支持c/c++的行注释规则，及//开头的行会被忽略。
* 修复ParseVariableDefinition函数(parser.cpp/412)和ParseFuncCallExpr函数(parser.cpp/65)的bug，解析如下结构：
   func_name / variable_name(expr1, expr2, ...)时，只能根据index匹配(对应的)，因为表达式中可能含有)，故
   如果直接使用TOKEN匹配可能匹配到不match的括号对，导致解析错误。
* TODO：目前的解析可能导致产生空表达式，虽然逻辑上没有问题但可以优化掉。