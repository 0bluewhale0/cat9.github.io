## 关于visual studio 缺少组件而卸载重装过程中遇到的一些问题
### 遇到的问题
卸载完了之后仍然安装在C盘，而无法自主更改安装路径。
以及因为文件缺失不能在**控制面版-程序**里正确卸载
### 一些总结
* 一个据说可以完全卸载vs的东东 是github的资源，还没搞懂咋用，先码在这，以备不时之需
[visual studio totaluninstaller](https://github.com/Microsoft/VisualStudioUninstaller)
* 使用mklink命令，瞒天过海，在其他盘建一个名字框架完全相同的文件夹，涉及命令提示符的一句命令(mklink)，有点点黑科技，由于不知道这样会不会在后续使用中造成一些奇怪的bug，加之系统盘内存还相对充裕，未尝试，但也先码在这[Visual Stdio 2019(或其它版本)怎样全部安装到非系统盘](https://zhuanlan.zhihu.com/p/87509706)
* 修改注册表
*ps:此法需谨慎，经更改注册表完毕后，桌面多个快捷方式显示找不到指定路径，而改程序正常存在，但是快捷方式的路径被统一修改了，进入属性更改为正常路径才可以访问。*
首先是注册表咋打开，可以windows键+Q(触控板三指轻按)搜索‘注册表’，也可以Windows+R 输入regedit(注册表编辑器)。
1. 然后按照图片所示把sharedinstallationpath改成想要的本地路径。
![tupian](https://img-blog.csdnimg.cn/20210122093019664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N3anR1c3o=,size_16,color_FFFFFF,t_70)
2. 




* 关于**系统错误内存转储文件**
是可以删除的，(在使用属性中的磁盘清理的时候)，但是删除之后还会继续生成，是个比较大的文件，主要是储存一些电脑蓝屏的错误原因，特别是在Windows更新的时候容易出现错误( 。[这个教程](https://www.bkqs.com.cn/contant1/18p74j26p4.html)可以设置不让电脑再生成~
* 还意外发现了两个可以**付费墙**的东东，小白不懂得这个咋用，考虑到之后大概会用到，先码一下。
[superfast](http://www.app-superfast.com/zh/)(30+/月 多设备) 和 [bitz net](https://ca.bitznet.app/?_gl=1*i2rj0u*_gcl_aw*R0NMLjE2MzA0NzQ3MTYuRUFJYUlRb2JDaE1JclpUWDRkcmE4Z0lWOTBnVkNCMlNIUUMxRUFFWUFTQUFFZ0xnWmZEX0J3RQ..#/dashboard)
(25rmb/月 只能自己用)
* 以及[这是一个可以使用国外免费服务器的网站yupage](https://cpanel.yupage.com/index.php)
 
Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/0bluewhale0/cat9.github.io/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
