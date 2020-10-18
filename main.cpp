#include <iostream>
#include "scanner.h"
#include "parser.h"

/*
 e://main.pn :

// 这是一个测试文件
func get() int {

}

type Person {
  string name;
  int age;
  func getAge() int {
    return age;
  }
}

func main() void {
  Person ps(a, b+c, get());
}
 */

int main() {
  Scanner scanner("e://main.pn");
  Parse(scanner.tokens());
  TokenPrint(scanner.tokens());
  system("pause");
  return 0;
}