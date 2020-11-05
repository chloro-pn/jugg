# jugg

jugg是一个解释型编程语言，通过词法分析-语法分析生成抽象语法树AST，并使用解释器直接在AST上执行。
设计jugg的三个基本原则：
1. 值语义+自定义运算符重载，自定义构造和析构函数
2. 支持模板机制 or 鸭子类型
3. 以c++作为后端，并提供c++ -> jugg的便捷接口

目前实现：
* 关于第一条，jugg的类型系统基于值语义构建，但目前不支持自定义运算符和构造/析构函数。
* 关于第二条，目前没有实现。
* 关于第三条，后端语言c++可以通过实现内置函数提供便捷的底层支撑。

# example
已经实现的功能可以在example文件夹下找到对应的源文件示例代码，例如下面是一个展示函数递归调用的示例代码：
```
func Fibonacci(int n) int {
  if(n == 0) {
    return 0;
  }
  else {
    if(n == 1) {
      return 1;
    }
  }
  return Fibonacci(n - 1) + Fibonacci(n - 2);
}

func main() void {
  print("fibonacci 6th(from 0) num is ", Fibonacci(6));
  return;
}
```
