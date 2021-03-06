# DLL（动态链接库）创建及其使用
> 平台：VS2015 + win 10
## 简介
###　1、说明
自从Microsoft Windows 第一个版本诞生之日起，动态链接库（dynamic-link library, DLL）一直以来都是该操作系统的基石。 Windows应用程序编程接口（application programming interface, API）提供的所有函数都包含在DLL中。其中三个最重要的DLL分别是：
* Kernel32.dll: 包含的函数用来管理内存、进程以及线程
* User32.dll：包含的函数用来执行与用户界面相关的任务，如创建窗口和发送信息。
* GDI32.dll: 包含的函数用来绘制图像和显示文字。  
#### Windows还提供其他的一些DLL，用来执行更加专门的任务。例如：
* AdvAPI32.dll：包含的函数与对象的安全性、注册表的操控以及事件日志有关。
* ComDlg32.dll: 包含了一些常用的对话框（如打开文件和保存文件）
* ComCtl32.dll: 支持所有常用的窗口控件。

### 2、优势
我们为什么要使用dll来进行开发：
* 1、 它们扩展了应用程序的特性：由于DLL可以被动态的载入到进程的地址控件中，应用程序可以在运行的时候检测应该执行何种操作，并在需要的时候载入DLL来执行这些操作。
* 2、它们简化了项目管理：如果再开发过程中让不同的开发团队来开发不同的模块，那么项目将更加易于管理。但是， 应用程序应该发布尽可能少的文件。如果一个产品中包含了上百个DLL文件，则在程序真正开始运行之前，系统必须打开一百多个磁盘文件，初始化时间将会变得难以忍受。
* 3、它们有助于节省内存：如果两个或两个以上的应用程序使用同一个DLL， 那么该DLL只需载入内存一次，之后所有的应用程序就可以共享该DLL在内存中的页面。C、C++运行库就是一个绝佳的例子。许多应用程序都会用到C/C++运行库，如果所有的应用程序都连接到C/C++运行库的静态版本，那么诸如_testpy,malloc之类的函数会在内存中出现多次。但是如果这些应用程序都链接到C、C++运行库的DLL版本，那么这些函数都只会在内存中出现一次，这意味着内存的使用率更高。
* 它们促进了资源的共享：DLL能够包含诸如对话框模板、字符串、图标、以及位图之间的资源。多个应用程序可以使用DLL来共享这些资源。
* 它们促进了本地化：DLL文件常用来才对应用程序进行本地化。例如，一个应用程序可以只包含代码但不包含用户界面组件，DLL用来存放本地化的用户界面组件，供应用程序载入并使用。
* 它们有助于解决平台间的差异
* 它们可以用于特殊目的

### 3、一般步骤：
* 1、构建DLL需要以下步骤：
>* 必须先创建一个头文件，在其中包含我们想要在DLL中到处的函数原型、结构以及符号。为了构建该DLL，DLL的所有源文件需要包含这个头文件。正如我们稍后可以看到，在构建可执行文件的时候需要用到这同一个头文件。
>* 创建C/C++源文件来实现想要在DLL模块中导出的函数和变量。由于在构建可执行模块的时候不需要这些源文件，因此创建该DLL的公司可以将这些源代码作为公司的机密。
>* 在构建该DLL模块的时候，编译器会对每个源文件进行处理并产生一个obj模块（每个源文件对应一个obj模块）。
>* 当所有obj模块都创建完毕后，链接器会将所有的obj模块的内容合并起来，产生一个单独的DLL映像文件。这个映像文件（或模块）包含DLL中所有的二进制代码以及全局/静态变量。为了可执行模块，这个文件是必须的。
>* 如果链接器检测到DLL的源文件输出了至少一个函数或者变量，那么链接器还会生成一个lib文件。这个lib文件非常小，这是因为它并不包含任何函数或者变量，它只是列出所有被导出的函数和变量的符号名，为了构建可执行模块，这个文件也是必须的。

* 2、构建可执行文件（例如：.exe）需要以下步骤：
>* 在所有引用了导出的函数、变量、数据结构或符号的源文件中，必须包含由DLL的开发人员所创建的头文件。
>* 创建C/C++源文件来实现想要包含在可执行模块中的函数和变量。当然代码可以引用在DLL的头文件中定义的函数和变量。
>* 在构建可执行模块的时候，编译器会对每个源文件进行处理并产生obj模块（每个源文件对应一个obj模块）。
>* 当所有的obj模块都创建完毕后，链接器会将所有的obj模块的内容合并起来，产生一个单独的可执行映像文件。这个映像文件（或模块）包含了可执行文件中的所有的二进制代码以及全局、静态变量。该可执行模块还包含了一个导入段（import section），其中列出了所有它需要的DLL模块的名称。此外，对列出的每个DLL，该段还记录了可执行文件的二进制代码从中引用的函数和变量的符号名。操作系统的加载程序会解析这个导入段。
一旦DLL和可执行模块都已经构建完毕，进程就可以开始执行了，当我们试图运行可执行模块的时候，操作系统的加载程序会执行下面的步骤。
>* 加载程序先为新的进程创建一个虚拟地址空间，并将可执行模块映射到新进程的地址空间中，加载程序接着解析可执行模块的导入段。对导入段中列出的每个DLL，加载程序会在用户的系统中对该DLL模块进行定位，并将该DLL映射到进程的地址空间中。注意，由于DLL模块可以从其他的DLL模块中导入函数和变量，因此DLL模块可能有自己的导入段并需要将它所需的DLL模块映射到进程的地址空间中，所以，初始化一个进程可能需要耗费比较长的时间。

### 4、实战演练：
#### 1、构建DLL：
* 创建Win32 dll项目
>![创建win32 dll项目](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/001.png)
* 勾选预编译头：VS 默认情况下会创建并使用预编译头文件(也就是自动创建 StdAfx.h 和 StdAfx.cpp 这两个文件)，以便在编译时加快编译速度，预编译器将它编译后，会生成一个 Pre-compiled header ，也就是 pch 文件，这样下次就可以直接使用这里已经编译好了的代码了。
>![勾选预编译头](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/002.png)
* 创建完成后如下所示空项目：
>![空项目](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/003.png)
* 空项目中在DLLTest.cpp中添加我们想要封装的函数或者变量，并将debug修改为release：
>![修改cpp](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/004.png)
* 新建DLLTest.h,声明函数定义并声明此函数为导出函数：
>![创建DLLTest.h](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/005.png)
>![修改DLLTest.h](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/006.png)
* 注意：```__stdcall```定义导出函数入口点调用约定为 ```_stdcall```
> extern ```"C"```说明导出函数使用C编译器，则函数名遵循C编译器的函数名修饰规则，不加extern ```"C"```说明使用C++编译器的函数名修饰规则，两种规则区别如下：
>* C编译器的函数名修饰规则:
>> 对于```__stdcall```调用约定，编译器和链接器会在输出函数名前加上一个下划线前缀，函数名后面加上一个```“@”```符号和其参数的字节数，例如             ```_functionname@number```。```__cdecl```调用约定仅在输出函数名前加上一个下划线前缀，例如```_functionname```。```__fastcall```调用约定在输出函数名前加上一个```“@”```符号，后面也是一个```“@”```符号和其参数的字节数，例如```@functionname@number```。
>* C++编译器的函数名修饰规则:
>> C++的函数名修饰规则有些复杂，但是信息更充分，通过分析修饰名不仅能够知道函数的调用方式，返回值类型，甚至参数个数、参数类                           型。不管```__cdecl```，```__fastcall```还是```__stdcall```调用方式，函数修饰都是以一个```“?”```开始，后面紧跟函数的名字，再后面是参数表的开始标识和按照参数类型代号拼出的参数表。对于```__stdcall```方式，参数表的开始标识是```“@@YG”```，对于```__cdecl```方式则是```“@@YA”```,对于           ```__fastcall```方式则是```“@@YI”```。
* 我们需要覆盖掉修饰规则产生的奇怪函数名或者变量名，所以需要额外添加一个def文件：
>![创建def文件](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/007.png)
>![覆盖函数名](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/008.png)
* 在完成以上步骤后，生成即可生完成我们的DLL文件生成：(注意：我们之后调用DLL中的函数需要用到：dll文件、lib文件、h文件)
>![生成](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/009.png)

#### 2、调用DLL
* 创建空项目：
>![创建空项目](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/010.png)
* 将上一步生成的DLLTest.dll、DLLTest.lib、DLLTest.h三个文件拷贝到项目路径下：
>![拷贝文件](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/011.png)
* ```#include "DLLTest.h"```调用函数即可：
>![InvokeDLL.cpp](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/012.png)
* 结果如下：
>![res](https://github.com/cbhust8025/C-Learn/blob/master/TencentLearn/DLLLearn/pngfiles/013.png)
