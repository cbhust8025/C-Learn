# 将资源文件进行归类
> 基于Python语言，将某个路径下的所有相同的gft文件和相同的png格式的文件分别聚类（包含子文件夹中），并保存进txt文件，记录相应的绝对路径文件名。
## 注意事项：
* 1、虽说指定的是两种图片文件，但是实质上与图像库无关，我们判断两个图片是否相同，在一定意义上是判断两个文件是否相同，我们可以直接利用hashlib中的md5对象的方法，求出文件的md5校验值，如果两个文件的md5值相同，则这两个文件在本质上是相同的。（经过md5的更新，如果两个文件的md5值不相同，则基本不可能相同）
* 2、注意利用正则表达式来筛选我们需要的gft文件，png文件以及子文件夹。
* 3、利用一个全局列表来管理未进行操作的路径或者是文件夹名。
* 4、利用一个数据库文件来存储我们计算的md5值，然后使用数据库操作来输出重复md5的所有文件名。


## 最近更新-0625：
### 输入：Config.ini文件进行配置
### 输出：res.db数据库文件;output.txt文件保存检测结果
