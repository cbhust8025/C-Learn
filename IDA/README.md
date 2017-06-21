# IDA 逆向神器
> IDA是一款极其强大的反汇编软件，特有的IDA视图和交叉引用，可以方便我们理解程序逻辑和快速定位代码片断，方便我们重现别人的软件和从二进制码来改写程序的逻辑。
## 简单的使用流程
>IDA的使用较为复杂，我们先从最简单的代码开始演示使用流程。
```C++
#include <iostream>
using namespace std;

int main(){
	int n;
	cin >> n;
	if( n > 0 ){
		cout << "a > 0" << endl;
	}
	else{
		cout << "n < 0" << endl;
	}
	system( "pause" );
}
```
* 第一步，打开IDApro，选取```Disassemble a new file```
[起始界面]()
*
