# Tinyxml2库（version：2.6.2）
>vs2015+win10
## 1、源码下载：
>[Tinyxml2](https://sourceforge.net/projects/tinyxml/files/latest/download)

## 2、Tinyxml与Tinyxml2

### 相同点：

* 都使用了简单易用的APIs
* 基于DOM树进行解析
* 都支持UTF-8/Unicode编码

### 不同点：

* TinyXml-2优点：
>* 对大部分C、C++项目具有普适性。
>* 使用较少的内存(memory：1/10th or 1/100th)，大约是TinyXml-1的40%，速度更快(~5x on read)。
>* 无C++ 11之后标准的STL要求
>* 更接近现代C++要求，使用了适当的命名空间
>* 适当有效的处理了空白字符

* TinyXml-1优点：
>* 可以报告分析错误的位置
>* 提供一些C++ STL公约支持：流和字符串（string类）
>* 拥有非常成熟和良好的调试代码库。

## 3、建立自己的命名空间和lib
>为了封装源码文件（.cpp），我们需要建立工程，链接成lib文件，配合我们的```tinystr.h```、```tinyxml.h```一起使用，即可进行xml文件的相关操作。
