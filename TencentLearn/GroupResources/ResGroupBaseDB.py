# encoding: utf8
'''
为所在文件夹极其子目录下的所有配置文件指定的类型文件建立聚类，输出具有重复文件的具体路径
relevant file:
'''

import os, sys  # 系统库
import re  # 正则表达式
import hashlib  # md5值求取
import sqlite3  # 数据库操作
import ConfigParser  # 配置文件读取

IniFileName = "Config.ini"
conf = ConfigParser.ConfigParser()  # 初始化config解析对象
conf.read('\\'.join([sys.path[0], IniFileName]))  # 读取配置文件
unschpath = [sys.path[0]]  # 保存未遍历过的路径，初始化当前python文件所在路径
MatchFileSuffix = [str(name.strip('"')) for name in conf.get("Config", "MatchFileSuffix").split(';')]#[".png", ".gft"]  # 保存所有需要匹配的文件后缀名
DataFileName = conf.get("Config", "DataFileName").strip('"')
OutputFileName = conf.get("Config", "OutputFileName").strip('"')

print MatchFileSuffix
print DataFileName
print OutputFileName


def GetHashMd5(ProFileName):  # 获取文件file的HashMd5值
    hashl = hashlib.md5()
    File = open(ProFileName, "rb")
    line = File.readline()
    while line:
        # print hashl.hexdigest()
        hashl.update(line)
        line = File.readline()
    return hashl.hexdigest()  # 返回md5对象的16进制摘要

def AddtoGroup(ProFileName, DataBase):  # 将检测到的png、gft文件求取Md5值后连带路径名存入db文件中
    CurFileMd5 = GetHashMd5(ProFileName)
    DataBase.execute("insert into catalog values (?, ?)", (ProFileName, CurFileMd5))
    return True

def SearchCurDirFile(curfilepath, DataBase):  # 遍历当前文件夹所有子文件
    allfiles = os.listdir(curfilepath)
    fmatptn = r"\S*\s*\.+\S*\s*"  # 匹配所有含"."的文件名，除此之外的文件名就是我们要找的子文件夹名
    MatchFilePattern = "\S*\s*(%s)$" % '|'.join(MatchFileSuffix)  # 匹配所有以.png/.gft结尾的文件名
    for file in allfiles:
        print "Processing the " + file
        if re.match(MatchFilePattern, file):  # 匹配gft、png文件
            AddtoGroup('\\'.join([curfilepath, file]), DataBase)
        if (re.match(fmatptn, file)):  # 过滤掉其他文件
            continue
        unschpath.append('\\'.join([curfilepath, file]))
    return True

def SelectMd5FromDB(DataBase, MD5):  # 从数据库中挑选出Md5值等于MD5的所有条目，返回其对应的文件名，以列表形式封装
    cursor = DataBase.execute("SELECT FileName FROM catalog WHERE MD5 = ?", (MD5,))
    return [name[0] for name in cursor.fetchall()]

def OutputRes(DataBase):  # 从db文件中将检测结果输出到当前路径下的output.txt中
    # 我们首先找出所有重复MD5值一次以上的所有MD5值
    print "Output the Results......"
    cursor = DataBase.execute("SELECT * FROM catalog GROUP by MD5 HAVING Count(*) > 1")
    OutputContents = ""
    for item in cursor.fetchall():
        files = SelectMd5FromDB(DataBase, item[1])  # 找出当前md5值对应的所有文件名，长度肯定大于1
        OutputContents += "\n".join(files) + "\n\n"
    Output = open('\\'.join([sys.path[0], OutputFileName]), "w+")
    Output.writelines(OutputContents)
    Output.close()
    print "Done..."

def OpenDB(DataFile):
    cx = sqlite3.connect(DataFile, isolation_level=None)
    try:
        cx.execute("create table catalog(FileName varchar UNIQUE,\
            MD5 text)")
        print("Create the Table Successed!")
    except:
        cx.execute("DELETE FROM catalog")
        print("Open the Table Successed!")
    return cx.cursor()
if __name__ == "__main__":  # 主程序入口
    # 创建数据库 or 打开数据库文件  指针类型
    DataBase = OpenDB('\\'.join([sys.path[0], DataFileName]))
    while unschpath:
        curpath = unschpath.pop()
        SearchCurDirFile(curpath, DataBase)
    # print resgroup
    OutputRes(DataBase)
