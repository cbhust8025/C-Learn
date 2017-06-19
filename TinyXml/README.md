# TinyXml
> C++上用于解析xml文件的类库，具有使用简便、方法丰富等优点。
## 1、使用简单

> * [官方网站](http://www.grinninglizard.com/)

> * [官方文档](http://www.grinninglizard.com/tinyxmldocs/tutorial0.html)

> * [sourceforge下载](https://sourceforge.net/projects/tinyxml/)  

使用步骤：

* 1、首先在sourceforge上下载TinyXml整个库文件，库文件中真正拿来用的是两个头文件 **tinystr.h、tinyxml.h**以及四个cpp文件**tinystr.cpp、tinyxml.cpp、tinyxmlerror.cpp、tinyxmlparser.cpp**。

* 2、建立一个新的工程，将第一步所提的6个文件分别拷贝进工程。如图  
![tinyproject](https://github.com/cbhust8025/C-Learn/blob/master/tinypro.png)  

* 3、在想要解析xml文件的地方```#include "tinyxml.h"```即可。

## 2、方法丰富
TinyXml类库中的主要类具有以下的关系图:  (引用自**tinyXML文档**)
![TinyXml类图](http://dl2.iteye.com/upload/attachment/0015/8691/fd4f4ec6-61fe-34ac-acab-d946c7678029.png)  

>* TiXmlBase类：显而易见，是所有类的虚基类，一般不进行实例化，主要是提供一些方法。

>* TiXmlNode、TiXmlAttribute两个类都继承自TiXmlBase类，其中TiXmlNode类指的是所有被```<>...</>```符号所包含的内容，而TiXmlAttribute类则指的是```<>```内的内容，我们称之为属性，每个属性都有其属性值（形如```<a href="www.baidu.com"></a>```中的href为属性，```"www.baidu.com"```为其属性值）。

>* 对于TiXmlNode，我们对其又进行了详细的划分，分别是注释、声明、文档、元素、以及节点间的文本，因此派生出了继承自TiXmlNode类的TiXmlComment(注释)、TiXmlDeclaration(声明)、TiXmlDocument(文档)、TiXmlElement(元素)、TiXmlText(节点间的文本)、TiXmlUnknown（其他）
```Xml
<?xml version="1.0" encoding="UTF-8"?>  
<phonebook>  
    <!--one item behalfs one contacted person.-->  
    <item>  
        <name>miaomaio</name>  
    <addr>Shaanxi Xi'an</addr>  
    <tel>13759911917</tel>  
    <email>miaomiao@home.com</email>  
    </item>  
    <item>  
        <name>gougou</name>  
    <addr>Liaoning Shenyang</addr>  
    <tel>15840330481</tel>  
    <email>gougou@home.com</email>  
    </item>  
    <!--more contacted persons.-->  
</phonebook>  
```
* TiXmlComment类指的是```<!--one item behalfs one contacted person.--> ```和``` <!--more contacted persons.-->  ```。
* TiXmlDeclaration类指的是```<?xml version="1.0" encoding="UTF-8"?>  ```。
* TiXmlDocument类指的是整个xml文档，用于读取或者存取xml文件时的实例化对象。
* TiXmlElement类指的是某个tag节点例如：```<phonebook>、<item>、<name>、<addr>```这些节点。
* TiXmlText类指的是```Shaanxi Xi'an```、```13759911917```这些夹在TiXmlElement（```<addr>与</addr>```、```<tel>与</tel>```）的文本内容.
* TiXmlAttribute类指的是```<?xml version="1.0" encoding="UTF-8"?> ```节点中的```version、encoding```等属性值。
* 以上除外的就属于TiXmlUnknown类。

```C++
#include <iostream>
#include "tinyxml.h"
using namespace std;

//Test for write a single xml file: 1.xml
bool WriteXmlForTest(const char * writefilename){
	TiXmlDocument doc; //实例化一个文档对象，最后利用此文档对象的SaveFile函数
	// 实例化文档对象之后，在保存文档类型之前，修改文档对象，向其中添加节点，定制我们想要的xml文件中的各个节点
	// 在添加节点之前，需要首先实例化一个节点类对象

	TiXmlElement * e1 = new TiXmlElement( "newElement1" );  // 实例化一个Element类对象
	e1->SetAttribute("href", "www.baidu.com");  // 利用Element类的SetAttribute方法为e1对象设置其属性值
	e1->SetAttribute("Text", "asd");  // 利用Element类的SetAttribute方法为e1对象设置其属性值
	
	TiXmlText * e1text = new TiXmlText( "element1" );  // 没有专门设置文本的方法，所以需要为一个节点添加文本节点才能插入一个节点的文本
	e1->LinkEndChild( e1text );  // 类似stl库容器的append方法。
	
	TiXmlElement * e2 = new TiXmlElement( "newElement2" );  // 实例化一个Element类对象
	e2->SetAttribute("href", "www.google.com");  // 利用Element类的SetAttribute方法为e1对象设置其属性值
	e2->SetAttribute("Text", "dsa");  // 利用Element类的SetAttribute方法为e1对象设置其属性值

	TiXmlElement * e3 = new TiXmlElement( "newElement3" );  // 实例化一个Element类对象
	e3->SetAttribute("href", "www.tencent.com");  // 利用Element类的SetAttribute方法为e1对象设置其属性值
	e3->SetAttribute("Text", "sad");  // 利用Element类的SetAttribute方法为e1对象设置其属性值

	doc.LinkEndChild( e1 );  // 将建立的Element类对象添加到doc类的最后面。
	e1->LinkEndChild( e2 );  // 将建立的Element类对象添加到e1类的最后面。
	e2->LinkEndChild( e3 );  // 将建立的Element类对象添加到e2类的最后面。
	// SaveFile函数具有主要的两种重载形式，一种接受字符串作为保存的文件名，
	// 另一种接受一个FILE对象来保存文档对象到此文件对象中，也是上一种重载函数中实际调用的函数。
	doc.SaveFile(writefilename);
	return true;
}

bool ReadXmlForTest(const char * readfilename, const char * outputfilename){
	// 要读取xml文件，也就是利用文档对象TiXmlDocument的LoadFile方法来进行读取xml文件，生成一个dom树。
	TiXmlDocument doc; // 实例化一个文档对象，用来读取xml文件中的内容
	doc.LoadFile(readfilename);  // 读取xml文件
	//cout << doc.FirstChildElement()->GetText() << endl;  // 获取文档对象的第一个子节点的文本内容
	FILE * cf = fopen(outputfilename, "w+");
	doc.Print( cf, 0);
	return true;
}

int main(){
	if(WriteXmlForTest("1.xml")){
		cout << " Create the 1.xml Successful! " << endl;
	}
	if(ReadXmlForTest( "1.xml", "2.xml" )){
		cout << " Read the 1.xml Successful! " << endl;
	}
	system("pause");
	return 0;
}
```
