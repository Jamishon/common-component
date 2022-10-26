#include <memory>
#include <iostream>
#include <map>


class Parent
{
 

};

class Son : public Parent
{
 

};

std::map<int, std::shared_ptr<Parent>> m1;

//void add(int i, const std::shared_ptr<Parent>& spParent) {   
void add(int i, const std::shared_ptr<Parent>&& spParent) {
 
 //m1[1] = spParent;
 m1.emplace(i, const_cast<std::shared_ptr<Parent>&&>(spParent));

 ///问题1: 在当前位置，spParent引用计数是多少？
 std::cout << spParent.use_count() << std::endl;
}


int main()
{
 {
  std::shared_ptr<Son> spSon = std::make_shared<Son>();
  //add(1, spSon);
  add(1, std::move(spSon));
  

  //问题2: 在当前位置，spSon引用计数是多少？
  std::cout << spSon.use_count() << std::endl;
 }

 //问题3: 在当前位置，m1中的智能指针对象引用计数是多少？
 std::cout << m1[1].use_count() << std::endl;

 return 0;
}