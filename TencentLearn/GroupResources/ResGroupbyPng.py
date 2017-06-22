# encoding: gbk
'''
为所在文件夹极其子目录下的所有Png、GFT文件建立聚类，相同的图像内容对应保存其路径。
'''

import os, sys  # 系统库
import re  # 正则表达式
import hashlib

resgroup = {}  # 保存分类结果, 以md5值为key，对应的绝对路径名为value
# schpath = []  # 保存已经遍历过的路径, 目前未用到
unschpath = [sys.path[0]]  # 保存未遍历过的路径，初始化当前python文件所在路径

def GetHashMd5(file):  # 获取文件file的HashMd5值
    hashl = hashlib.md5()
    for line in open(file, "r+").readlines():
        # print hashl.hexdigest()
        hashl.update(line)
    return hashl.hexdigest()

def IsEqual(file1_md5, file2):  # 通过hashlib生成MD5来验证两个文件是否相同
    return file1_md5 == GetHashMd5(file2)

def AddtoGroup(imgfile):  # 将检测到的png、gft文件放入到分类结果中
    for key in resgroup.keys():
        if IsEqual(key, imgfile):  # 如果两个分类结果已经保存有相同的图片，我们直接append即可
            resgroup[key].append(imgfile)
            return True  # 返回True表示已经有相同的图片存储
    resgroup[GetHashMd5(imgfile)] = [imgfile]
    return False  # 返回false表示还没有相同的图片存储

def SearchCurDirFile(curfilepath):  # 遍历当前文件夹所有子文件
    allfiles = os.listdir(curfilepath)
    fmatptn = r"\S*\s*\.+\S*\s*"  # 匹配所有含"."的文件名，除此之外的文件名就是我们要找的子文件夹名
    pfmatptn = r"\S*\s*(.jpg)$"  # 匹配所有以".png结尾的文件名"
    gfmatptn = r"\S*\s*(.gft)$"  # 匹配所有以".gft结尾的文件名"
    for file in allfiles:
        print "Processing the " + file
        if (re.match(gfmatptn, file) or re.match(pfmatptn, file)):  # 匹配gft、png文件
            AddtoGroup('\\'.join([curfilepath, file]))
        if (re.match(fmatptn, file)):  # 过滤掉其他文件
            continue
        unschpath.append('\\'.join([curfilepath, file]))
    return True

def OutputRes():  # 将检测结果输出到当前路径下的output.txt中
    tfile = open('\\'.join([sys.path[0], "output.txt"]), "w+")
    for key in resgroup.keys():
        for file in resgroup[key]:
            tfile.write(file + "\n")
        tfile.write("\n\n")


if __name__ == "__main__":  # 主程序入口
    # print os.listdir(sys.path)
    # print
    while unschpath:
        curpath = unschpath.pop()
        SearchCurDirFile(curpath)
    print resgroup
    OutputRes()
