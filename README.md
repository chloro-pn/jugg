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
* |done|目前的解析可能导致产生空表达式，虽然逻辑上没有问题但可以优化掉。

##### 2020-10-19
* 修复解析参数列表的bug。
* 添加函数FindNextComma的实现。
* 修改表达式解析过程中对函数调用表达式的解析时刻，为了支持id表达式的静态绑定。
* 添加id表达式构造时静态绑定功能。
* 实现部分内置运算符支持的操作数类型与返回类型记录，为后续表达式类型推导过程以及运算符重载过程提供支持。
* 修复Variable初始化的bug（应该根据内置类型还是抽象类型生成不同的子类）。
* |done|func块和type块的变量绑定问题，一变量如何绑定到函数实参和类型成员变量？目前还没有实现。

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
* |doing|考虑增加变量系统，并为运算符增加处理函数。

##### 2020-10-22
* 实现复杂度已经超出了我的控制范围，由于一次性想要支持的特性太多，导致代码结构非常难设计，
注 : 应该从解释器解释执行的角度设计代码结构。
* 本项目的词法分析和语法分析部分没有问题，能够得到准确的语法树。但是语义分析部分没想清楚该怎么做。
* 留到日后重构，不再更新。

##### 2020-10-23
* 重构，删除作用域模块，解释器执行可以根据运行时上下文动态识别。

##### 2020-10-24
* 重新起航，实现解释器的部分，实现变量模块机制，考虑实现语句执行的逻辑
* 实际上为了支持continue，break和return的流程控制关键字，语句是需要返回某个代表返回状态的数据的，
当然这个返回值对语言使用者不可见。

##### 2020-10-25
* 修复参数列表解析的bug
* |done|:考虑实现函数返回值的优雅处理方法。
* 修复从表达式构造成员变量时没考虑左右值导致重复释放的bug。

##### 2020-10-26
* 完成CallMethod函数
* |done|:重新梳理变量的左值/右值规则，目前有bug，典型的一个变量的左/右值应该和其数据成员保持一致。

##### 2020-10-27
* 修复关于左值、右值的bug，以及修复Copy函数bug。
* 增加内置函数print的支持，
* |done|:内置函数根据参数位置定位参数而不是名字，因此应该使用vector存储参数列表而不是unordered_map。
* 补充其他基本类型的部分实现。

##### 2020-10-28
* 修复while语句循环跳出的bug
* 修正函数返回值传递机制，以支持递归调用。

##### 2020-10-29
* 修复字符串显示双引号的bug
* 代码重构
* |done|:实现默认构造
* TODO:实现数组类型
* |done|:修复非逻辑运算符是双元运算符的bug
* |dont|:由于c++栈的特点，无法对元素进行遍历，只能通过pop,top,push等操作访问修改元素，因此
每次在上下文栈中查找id时会产生复制操作，可以提供自定义的支持遍历的栈以优化程序。
* TODO:增加类型分析阶段，尽早发现语义问题。

##### 2020-10-30
* 修复变量作用域绑定的bug

##### 2020-11-[01--02]
* 尝试增加类型修饰符，以实现数组和指针类型。
* 将原有的只支持基本类型的系统替换为扩展的综合类型，并可正常运行程序。
* 表达式解析中加入单元运算符的解析规则，为之前的非逻辑运算与解引用运算提供支持。
* 支持单元运算符!和解引用*，取地址运算符&。
* 支持指针的基本使用， |done|目前的二元运算符仅仅考虑了基础类型的运算法则，没考虑带有类型修饰符的类型变量，例如指针的赋值运算，等待解决。

##### 2020-11-03
* 完成对指针类型的比较和赋值操作
* TODO:代码重构，添加注释，整理代码结构，添加单元测试

##### 2020-11-04
* 重构ast_node.h和variable模块的代码，以及人肉debug。
* 重构心得:资源管理者应该负责资源的检索，验证，查找等抽象行为，有利于代码复用和解耦。
* 继续重构，偷懒一时爽，重构火葬场:)
* 修复bug，处理表达式解析的左括号问题，以及解析参数列表时的逗号匹配问题。
* |修复|:当函数递归调用时，在函数体内无法看到自己的定义，因此没办法解析到本函数，等待解决。
解决办法：/目前通过在解析函数体之前向FuncSet注册只有函数名字的func对象，使得函数体解析过程中可以看到本函数的名字。
* 延申问题，如何处理函数相互调用问题，要么引入c/c++的定义，声明规则，要么采用多趟解析方法，首先生成全局的函数集合，然后进行解析，倾向于第二种处理方法。
* |修复|:上下文返回return_var_的时候，函数/方法上下文上面可能还有块上下文，导致返回nullptr。
