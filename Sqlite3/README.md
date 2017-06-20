# C++与数据库
> SQLite，是一款轻型的数据库，是遵守ACID的关系型数据库管理系统，它包含在一个相对小的C库中。它是D.RichardHipp建立的公有领域项目。它的设计目标是嵌入式的，而且目前已经在很多嵌入式产品中使用了它，它占用资源非常的低，在嵌入式设备中，可能只需要几百K的内存就够了。它能够支持Windows/Linux/Unix等等主流的操作系统，同时能够跟很多程序语言相结合，比如 Tcl、C#、PHP、Java等，还有ODBC接口，同样比起Mysql、PostgreSQL这两款开源的世界著名数据库管理系统来讲，它的处理速度比他们都快。

## 1、工作原理
  不像常见的客户-服务器范例，SQLite引擎不是个程序与之通信的独立进程，而是连接到程序中成为它的一个主要部分。所以主要的通信协议是在编程语言内的直接API调用。这在消耗总量、延迟时间和整体简单性上有积极的作用。整个数据库(定义、表、索引和数据本身)都在宿主主机上存储在一个单一的文件中。它的简单的设计是通过在开始一个事务的时候锁定整个数据文件而完成的。
### 特性：
* 1、零配置 – 无需安装和管理配置
* 2、储存在单一磁盘文件中的一个完整的数据库
* 3、数据库文件可以在不同字节顺序的机器间自由的共享
* 4、支持数据库大小至2TB
* 5、足够小, 大致13万行C代码, 4.43M
* 6、比一些流行的数据库在大部分普通数据库操作要快
* 7、简单, 轻松的API
* 8、包含TCL绑定, 同时通过Wrapper支持其他语言的绑定
* 9、 良好注释的源代码, 并且有着90%以上的测试覆盖率
* 10、独立: 没有额外依赖
* 11、源码完全的开源, 你可以用于任何用途, 包括出售它
* 12、支持多种开发语言，C, C++, PHP, Perl, Java, C#,Python, Ruby等

## 2、使用步骤
> 开发环境：win7+vs2005+C++11
### 1、下载sqlite3
[下载地址](http://www.sqlite.org/download.html)，下载以下三个文件：
* sqlite-amalgamation-3190300.zip --- 源码文件，利用到了里面sqlite3.h
* sqlite-dll-win32-x86-3190300.zip --- dll库文件，利用到了里面的sqlite3.def、sqlite3.dll文件
* sqlite-tools-win32-x86-3190300.zip -- 包含了sqlite3.exe文件

### 2、生成sqlite3.lib依赖文件
* 解压sqlite-dll-win32-x86-3190300.zip文件到D:\sqlite
* 运行Visual Studio 2005 Command Prompt命令行程序。
> 启动位置：开始->Microsoft Visual Studio 2005->Visual Studio Tools->Visual Studio 2005 Command Prompt。
* 依次执行下列控制台命令：
```CMD
cd D:\sqlite\sqlite-dll-win32-x86-3190300
D:
lib /def:sqlite3.def /machine:ix86
```
最后生成sqlite3.lib文件。

### 3、包含sqlite3.h头文件
* 将我们生成的sqlite3.lib和sqlite3.h(sqlite-amalgamation-3190300文件夹中)还有sqlite3.dll文件一起拷贝到项目目录下，然后设置依赖：
> 右键vs2005中项目->Properties->Configuration Properties->Linker->Input->Additional Dependencies->添加sqlite3.lib即可
* 在项目中需要sqlite3的地方：
```C++
#include "sqlite3.h"
```

### 4、sqlite3语法
* 数据库对象：不管我们想要读取一个数据库内容，或者写一个数据库，我们都需要首先生成一个数据库对象，来进行接下来的操作
```C++
sqlite3 * pDB;  // 定义一个sqlite3 类对象，保存读取的db文件内容
```

* 返回值类型：对于sqlite3中大多数声明的函数，其返回值均为```SQLITE_API int```，对于此类型的返回值，基本上返回0表示执行成功，其他的则均有错误，下面列出全部错误代码。
```C++
//sqlite3.h
/*
** CAPI3REF: Result Codes
** KEYWORDS: {result code definitions}
**
** Many SQLite functions return an integer result code from the set shown
** here in order to indicate success or failure.
**
** New error codes may be added in future versions of SQLite.
**
** See also: [extended result code definitions]
*/
#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Database is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Auxiliary database format error */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */
/* end-of-error-codes */
```

* 打开数据库文件：我们大多数时候是对已有的.db文件进行操作，或者需要创建一个新的.db文件，这时候需要用到sqlite3_open系列函数，定义如下：
```C++
//sqlite3.h
** <b>Note to Windows users:</b>  The encoding used for the filename argument
** of sqlite3_open() and sqlite3_open_v2() must be UTF-8, not whatever
** codepage is currently defined.  Filenames containing international
** characters must be converted to UTF-8 prior to passing them into
** sqlite3_open() or sqlite3_open_v2().
**
** <b>Note to Windows Runtime users:</b>  The temporary directory must be set
** prior to calling sqlite3_open() or sqlite3_open_v2().  Otherwise, various
** features that require the use of temporary files may fail.
**
** See also: [sqlite3_temp_directory]
*/
SQLITE_API int sqlite3_open(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb          /* OUT: SQLite db handle */
);
SQLITE_API int sqlite3_open16(
  const void *filename,   /* Database filename (UTF-16) */
  sqlite3 **ppDb          /* OUT: SQLite db handle */
);
SQLITE_API int sqlite3_open_v2(
  const char *filename,   /* Database filename (UTF-8) */
  sqlite3 **ppDb,         /* OUT: SQLite db handle */
  int flags,              /* Flags */
  const char *zVfs        /* Name of VFS module to use */
);
```
从定义来看，具体是传入文件名还有一个数据库对象（sqlite3类对象）两个参数来进行调用，我们来看sqlite3_open函数的实现
```C++
//sqlite3.c
SQLITE_API int sqlite3_open(
  const char *zFilename, 
  sqlite3 **ppDb 
){
  return openDatabase(zFilename, ppDb,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
}
```
可以很明显的发现是调用了openDatabase,由于其实现太长，我们来看一下其具体的接口
```C++
//sqlite3.c
/*
** This routine does the work of opening a database on behalf of
** sqlite3_open() and sqlite3_open16(). The database filename "zFilename"  
** is UTF-8 encoded.
*/
static int openDatabase(
  const char *zFilename, /* Database filename UTF-8 encoded */
  sqlite3 **ppDb,        /* OUT: Returned database handle */
  unsigned int flags,    /* Operational flags */
  const char *zVfs       /* Name of the VFS to use */
)
```

* 利用sql语句来对数据库对象进行操作：sqlite3中定义了一个函数来进行调用sql语句对数据库文件进行操作```sqlite3_exec```,定义如下：
```C++
/*
** CAPI3REF: One-Step Query Execution Interface
** METHOD: sqlite3
**
** The sqlite3_exec() interface is a convenience wrapper around
** [sqlite3_prepare_v2()], [sqlite3_step()], and [sqlite3_finalize()],
** that allows an application to run multiple statements of SQL
** without having to use a lot of C code. 
**
** ^The sqlite3_exec() interface runs zero or more UTF-8 encoded,
** semicolon-separate SQL statements passed into its 2nd argument,
** in the context of the [database connection] passed in as its 1st
** argument.  ^If the callback function of the 3rd argument to
** sqlite3_exec() is not NULL, then it is invoked for each result row
** coming out of the evaluated SQL statements.  ^The 4th argument to
** sqlite3_exec() is relayed through to the 1st argument of each
** callback invocation.  ^If the callback pointer to sqlite3_exec()
** is NULL, then no callback is ever invoked and result rows are
** ignored.
**
** ^If an error occurs while evaluating the SQL statements passed into
** sqlite3_exec(), then execution of the current statement stops and
** subsequent statements are skipped.  ^If the 5th parameter to sqlite3_exec()
** is not NULL then any error message is written into memory obtained
** from [sqlite3_malloc()] and passed back through the 5th parameter.
** To avoid memory leaks, the application should invoke [sqlite3_free()]
** on error message strings returned through the 5th parameter of
** sqlite3_exec() after the error message string is no longer needed.
** ^If the 5th parameter to sqlite3_exec() is not NULL and no errors
** occur, then sqlite3_exec() sets the pointer in its 5th parameter to
** NULL before returning.
**
** ^If an sqlite3_exec() callback returns non-zero, the sqlite3_exec()
** routine returns SQLITE_ABORT without invoking the callback again and
** without running any subsequent SQL statements.
**
** ^The 2nd argument to the sqlite3_exec() callback function is the
** number of columns in the result.  ^The 3rd argument to the sqlite3_exec()
** callback is an array of pointers to strings obtained as if from
** [sqlite3_column_text()], one for each column.  ^If an element of a
** result row is NULL then the corresponding string pointer for the
** sqlite3_exec() callback is a NULL pointer.  ^The 4th argument to the
** sqlite3_exec() callback is an array of pointers to strings where each
** entry represents the name of corresponding result column as obtained
** from [sqlite3_column_name()].
**
** ^If the 2nd parameter to sqlite3_exec() is a NULL pointer, a pointer
** to an empty string, or a pointer that contains only whitespace and/or 
** SQL comments, then no SQL statements are evaluated and the database
** is not changed.
**
** Restrictions:
**
** <ul>
** <li> The application must ensure that the 1st parameter to sqlite3_exec()
**      is a valid and open [database connection].
** <li> The application must not close the [database connection] specified by
**      the 1st parameter to sqlite3_exec() while sqlite3_exec() is running.
** <li> The application must not modify the SQL statement text passed into
**      the 2nd parameter of sqlite3_exec() while sqlite3_exec() is running.
** </ul>
*/
SQLITE_API int sqlite3_exec(
  sqlite3*,                                  /* An open database */
  const char *sql,                           /* SQL to be evaluated */
  int (*callback)(void*,int,char**,char**),  /* Callback function */
  void *,                                    /* 1st argument to callback */
  char **errmsg                              /* Error msg written here */
);
```
参数解释：
* sqlite3*:读取数据库文件，来进行操作，也即数据库对象(sqlite3*)
* const char *sql：我们需要执行的sql语句，可以利用拼接来进行定义此语句
* int (*callback)(void*,int,char**,char**) : 回调函数，在调用了sql语句之后的回调函数
* void *：回调函数的第一个参数
* char **errmsg： 保存错误信息

* 演示代码：
```C++
//Sqlite3Learn.cpp
#include <iostream>
#include <string>
#include "sqlite3.h"
using namespace std;

sqlite3 * pDB;  // 定义一个sqlite3 类对象，保存读取的db文件内容

bool AddUser(const string& sName, const string& sAge)  
{  
	string createtable = 
		"CREATE TABLE user \
		( \
		id integer primary key autoincrement, \
		name varchar(64), \
		age integer \
		)"; // 定义一个sql语句来创建数据库文件
	char* cErrMsg;  
	// #define SQLITE_OK           0   /* Successful result */
	int cRes = sqlite3_exec(pDB , createtable.c_str() ,0 ,0, &cErrMsg);  // 返回值为0表示成功
	if (cRes != SQLITE_OK)    
	{  
		cout << "create table fail! " << cErrMsg << endl;  
		return false;  
	}  
	else  
	{  
		cout << "cteate table success! " << endl;  
	}  
	// 利用利用sqlite3_exec来执行sql语句，其中函数定义如下：
	//SQLITE_API int sqlite3_exec(
	//	sqlite3*,                                  /* An open database */  --- 已经打开的数据库文件
	//	const char *sql,                           /* SQL to be evaluated */ --- 执行的sql语句
	//	int (*callback)(void*,int,char**,char**),  /* Callback function */ --- 回调函数
	//	void *,                                    /* 1st argument to callback */  --- 回调函数的第一个参数
	//	char **errmsg                              /* Error msg written here */  --- 反馈的错误信息返回
	//	);
	//char* cErrMsg;  
	string strSql = "";  // 定义一个sql语句来实现对数据库文件的操作
	// 先使用字符串拼接的方式来补全sql语句
	// 插入一行语句：insert into user(name,age)values("s-", "50");
	strSql += "insert into user(name,age)";  
	strSql += "values('";  
	strSql += sName;  
	strSql += "',";  
	strSql += sAge;  
	strSql += ");";  
	// #define SQLITE_OK           0   /* Successful result */
	int nRes = sqlite3_exec(pDB , strSql.c_str() ,0 ,0, &cErrMsg);  // 返回值为0表示成功
	if (nRes != SQLITE_OK)    
	{  
		cout<<"add user fail: "<<cErrMsg<<endl;  
		return false;  
	}  
	else  
	{  
		cout<<"add user success: "<<sName.c_str()<<"\t"<<sAge.c_str()<<endl;  
	}  

	return true;  
} 

//*/
//SQLITE_API int sqlite3_open(
//							const char *zFilename, 
//							sqlite3 **ppDb 
//							){
//								return openDatabase(zFilename, ppDb,
//									SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
//}
//SQLITE_API int sqlite3_open_v2(
//							   const char *filename,   /* Database filename (UTF-8) */
//							   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//							   int flags,              /* Flags */
//							   const char *zVfs        /* Name of VFS module to use */
//							   ){
//								   return openDatabase(filename, ppDb, (unsigned int)flags, zVfs);
//}

int main(){
	int res = sqlite3_open( "D:\\test1.db" , &pDB); // 读取数据库文件（.db），保存读取的文件内容到pDB对象中
	if( res ){
		cout << "Can't open database: " << sqlite3_errmsg(pDB) << endl;
		sqlite3_close( pDB );
		system( "pause" );
		return -1;
	}
	int res_add = AddUser("chengbo", "24");
	cout << res << endl;
	sqlite3_close( pDB );
	system( "pause" );
	return 0;
}
```
