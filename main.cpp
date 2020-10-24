#include <iostream>
#include "scanner.h"
#include "parser.h"

/*
// 这是一个测试文件

int a;
int b;
int c(b);
func get() int{

}

type Person{
  string name;
  int age;
}

method(Person) getAge() int{
  return age;
}

func main() void{
  Person ps(a, b + c, get());
}
*/

/*
 * 考虑解释器执行一个变量定义语句， 
 */

int main() {
  Scanner scanner("e://main.pn");
  Parse(scanner.tokens());
  TokenPrint(scanner.tokens());
  Interpreter::instance().Exec();
  system("pause");
  return 0;
}