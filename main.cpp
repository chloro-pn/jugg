#include <iostream>
#include "scanner.h"
#include "parser.h"

/*
// ����һ�������ļ�

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
 * ���ǽ�����ִ��һ������������䣬 
 */

int main() {
  Scanner scanner("e://main.pn");
  Parse(scanner.tokens());
  TokenPrint(scanner.tokens());
  Interpreter::instance().Exec();
  system("pause");
  return 0;
}