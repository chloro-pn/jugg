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
* 增加注释的语法，目前仅支持c/c++的行注释规则，即//开头的行会被忽略。
* 修复ParseVariableDefinition函数(parser.cpp/412)和ParseFuncCallExpr函数(parser.cpp/65)的bug，解析如下结构：
   func_name / variable_name(expr1, expr2, ...)时，只能根据index匹配(对应的)，因为表达式中可能含有)，故
   如果直接使用TOKEN匹配可能匹配到不match的括号对，导致解析错误。
* TODO：目前的解析可能导致产生空表达式，虽然逻辑上没有问题但可以优化掉。

##### 2020-10-19
* 修复解析参数列表的bug。
* 添加函数FindNextComma的实现。
* 修改表达式解析过程中对函数调用表达式的解析时刻，为了支持id表达式的静态绑定。
* 添加id表达式构造时静态绑定功能。
* 实现部分内置运算符支持的操作数类型与返回类型记录，为后续表达式类型推导过程以及运算符重载过程提供支持。
* 修复Variable初始化的bug（应该根据内置类型还是抽象类型生成不同的子类）。
[done] : func块和type块的变量绑定问题，一变量如何绑定到函数实参和类型成员变量？目前还没有实现。

##### 2020-10-20
* 代码重构，实现func和type的静态绑定时发现当前实现有问题，故重构作用域部分，将method和func进行区分并分别解析。
* 错误的理解了静态绑定的概念，在语义分析阶段还见不到变量的概念，因此不可能执行真正的绑定操作，
改为在每个id表达式被解析的时候通过查询操作找到定位的变量所在的作用域与名字。
* 删除variabld.h/cpp文件，之后写解释器的时候考虑加上。
* 修改作用域部分代码，现在的作用域仅记录变量名称：变量类型的map集合，用来支持静态的类型检查。
* 修改类型定义的语法，方法需要定义在type外面，为了方便成员变量的静态类型绑定。

##### 2020-10-21
* 代码重构，修复函数内局部变量的静态绑定找不到参数列表的bug。
* 表达式解析增加成员调用表达式的处理
* 将不同基本类型的字面量表达式作为不同类型处理
* TODO : 考虑增加变量系统，并为运算符增加处理函数。