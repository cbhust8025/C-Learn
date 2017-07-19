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

## 3、建立自己的命名空间和lib -- 支持wchar_t（宽字节）
>为了封装源码文件（.cpp），我们需要建立工程，链接成lib文件，配合我们的```tinystr.h```、```tinyxml.h```一起使用，即可进行xml文件的相关操作。

### 3.1 建立lib工程
>[lib工程](https://github.com/cbhust8025/C-Learn/tree/master/MyTinyxml2/MyTinyxml2Code)
编译生成对应版本（Release/Debug）和位数(32/64位)的lib文件。

### 3.2 建立测试工程
>[测试工程](https://github.com/cbhust8025/C-Learn/tree/master/MyTinyxml2/MyTinyxml2TestCode)
测试运行，调用xml方法进行操作xml文件。

## 4、注意事项

### 4.1 添加命名空间
命名空间添加需要同时修改头文件和cpp文件，一一对应修改。

### 4.2 添加支持wchar_t（宽字节）、wstring（宽字符串类型）
* tinyxml.h修改：
```C++
/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool LoadFile( TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
	/// Save a file using the current document value. Returns true if successful.
	bool SaveFile() const;
	/// Load a file using the given filename. Returns true if successful.
	bool LoadFile( const char * filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
	/// Load a file using the given filename. Returns true if successful.
	bool LoadFile( const wchar_t * filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING);
	/// Save a file using the given filename. Returns true if successful.
	bool SaveFile( const char * filename ) const;
	/// Save a file using the given filename. Returns true if successful.
	bool SaveFile( const wchar_t * filename) const;
	/** Load a file using the given FILE*. Returns true if successful. Note that this method
		doesn't stream - the entire object pointed at by the FILE*
		will be interpreted as an XML file. TinyXML doesn't stream in XML from the current
		file location. Streaming may be added in the future.
	*/
	bool LoadFile( FILE*, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING );
	/// Save a file using the given FILE*. Returns true if successful.
	bool SaveFile( FILE* ) const;

	#ifdef TIXML_USE_STL
	bool LoadFile(const std::wstring& filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING)			///< STL std::string version.
	{
		return LoadFile(filename.c_str(), encoding);
	}
	bool LoadFile( const std::string& filename, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING )			///< STL std::string version.
	{
		return LoadFile( filename.c_str(), encoding );
	}
	bool SaveFile(const std::wstring& filename) const		///< STL std::string version.
	{
		return SaveFile(filename.c_str());
	}
	bool SaveFile( const std::string& filename ) const		///< STL std::string version.
	{
		return SaveFile( filename.c_str() );
	}
	#endif
```

* tinyxml.cpp:中增添对应的函数
```C++
// Microsoft compiler security
FILE* TiXmlFOpen(const wchar_t* filename, const wchar_t* mode)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
	FILE* fp = 0;
	errno_t err = _wfopen_s( &fp, filename, mode);
	if (!err && fp)
		return fp;
	return 0;
#else
	return _wfopen(filename, mode);
#endif
}

```
```C++
bool TiXmlDocument::LoadFile(const wchar_t* _filename, TiXmlEncoding encoding)
{
	char szName[1024] = { 0 };
	if (::WideCharToMultiByte(CP_ACP,
		0,
		_filename,
		(int)(wcslen(_filename)),
		szName,
		sizeof(szName),
		0,
		0) > 0)
	{
		TIXML_STRING filename(szName);
		value = filename;
	}
	//TIXML_STRING filename(_filename);
	//value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = TiXmlFOpen(_filename, L"rb");

	if (file)
	{
		bool result = LoadFile(file, encoding);
		fclose(file);
		return result;
	}
	else
	{
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}
}
```
```C++

bool TiXmlDocument::SaveFile(const wchar_t * filename) const
{
	// The old c stuff lives on...
	FILE* fp = TiXmlFOpen(filename, L"w");
	if (fp)
	{
		bool result = SaveFile(fp);
		fclose(fp);
		return result;
	}
	return false;
}

```
