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
程序运行结果如下：
>![修改前程序运行结果](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/0.png)  
#### 我们需要将错误的```a > 0 ```修正为正确的```n > 0 ```。
* 1、打开IDApro，选取```Disassemble a new file```  
>![起始界面](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/1.png)

* 2、把编译好的exe文件或者需要修改的exe文件拖入软件中，我们的源代码用win32编译，所以选取第一个80386（PE）即可,其他默认：  
>![选项](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/2.png)  
#### 打开之后的界面  
>![刚打开exe文件之后的界面](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/3.png)  

* 3、在菜单栏右键找到```List```：
>![选取List](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/4.png)  
#### 找到Open String Window：  
>![Open String Window](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/5.png)  
#### 点击Open String Window：
>![Click The Open String Window](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/6.png)  
#### 定位代码片断并选中（或者双击）：
>![定位代码片断](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/7.png)  
#### 查看二进制码窗口，选中的代码片断会在二进制码中高亮：  
>![二进制码窗口](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/8.png)

* 4、复制高亮的代码片断对应的二进制码（61 20 3E 20 30 00），接下来我们需要修改exe文件中的对应二进制码来实现修改程序的目的
> 注：我们不能直接修改IDA的二进制码，这个二进制码只是IDA反汇编出来的，与源程序无关。  
#### 利用Sublime 来打开exe文件，我们可以看到二进制码文件，查找我们刚才复制的二进制码（注意Sublime是32位放在一起显示，而我们复制的是16位放在一起，注意取出空格）。
>![Sublime查找](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/9.png)  
#### 注意到我们查到无符合选项，因为二进制码可能在Sublime中换行了，我们减少32位重新search：
>![再次Sublime查找](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/10.png)  
#### 定位了二进制码片断之后，我们注意到6120 3e20，其中a的ACNII码为61，所以我们如果得到```n > 0```，需要将61修改为6e。至此源代码修改工作已完成。
#### 运行之后如下：
>![修改后的运行效果](https://github.com/cbhust8025/C-Learn/blob/master/IDA/pngFiles/READMEPNG/11.png)

