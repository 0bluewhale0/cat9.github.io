## 关于visual studio 缺少组件而卸载重装过程中遇到的一些问题

<<<<<<< HEAD
* 一个据说可以完全卸载vs的东东 是github的资源，还没搞懂咋用，先码在这，以备不时之需
[visual studio totaluninstaller](https://github.com/Microsoft/VisualStudioUninstaller)
* 使用mklink命令，瞒天过海，在其他盘建一个名字框架完全相同的文件夹，涉及命令提示符的一句命令(mklink)，有点点黑科技，由于不知道这样会不会在后续使用中造成一些奇怪的bug，加之系统盘内存还相对充裕，未尝试，但也先码在这[Visual Stdio 2019(或其它版本)怎样全部安装到非系统盘](https://zhuanlan.zhihu.com/p/87509706)
* 修改注册表
*ps:此法需谨慎，经更改注册表完毕后，桌面多个快捷方式显示找不到指定路径，而改程序正常存在，但是快捷方式的路径被统一修改了，进入属性更改为正常路径才可以访问。*
首先是注册表咋打开，可以windows键+Q(触控板三指轻按)搜索‘注册表’，也可以Windows+R 输入regedit(注册表编辑器)。
1. 然后按照图片所示![tupian](https://img-blog.csdnimg.cn/20210122093019664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N3anR1c3o=,size_16,color_FFFFFF,t_70)
把sharedinstallationpath改成想要的本地路径。

=======
 [editor on GitHub](https://github.com/0bluewhale0/cat9.github.io/edit/gh-pages/index.md) to maintain and preview the content for your website in Markdown files.
>>>>>>> 422d4b2d34776ff658a47a57035b976dd9fbd281

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
