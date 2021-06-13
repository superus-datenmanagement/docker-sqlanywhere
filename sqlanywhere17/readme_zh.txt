SQL Anywhere 17.0 发行说明(用于 Unix 和 Mac OS X)

(c) 2019 SAP SE 或其关联公司版权所有。保留所有权利。


安装 SQL Anywhere 17
--------------------

1. 切换至已创建的目录，并通过运行
以下命令启动安装程序脚本:
        cd ga1700
        ./setup

   有关可用安装选项的完整列表，
请运行以下命令:
./setup -h

2. 按照安装程序中的说明操作。


安装说明
------------------

o 现在没有内容。


文档
-------------

文档可在 DocCommentXchange 上获取，网址为:
    http://dcx.sap.com

DocCommentXchange 是在 Web 上访问和讨论
SQL Anywhere 文档的在线社区。DocCommentXchange 是
SQL Anywhere 17 的缺省文档格式。


MobiLink Deprecated Feature - As of this release, support for running MobiLink on IBM AIX is deprecated.
MobiLink customers who are running on IBM AIX can continue to do so with SQL Anywhere 17.

MobiLink Deprecated Feature - As of this release, support for IBM DB2 consolidated databases is deprecated.
MobiLink customers can continue using IBM DB2 as their consolidated database with SQL Anywhere 17.



SQL Anywhere 论坛
-----------------

SQL Anywhere 论坛是一个 Web 站点，您可以
在其中提出及回答关于 SQL Anywhere 软件的问题，
并对他人的问题及回答进行评论和投票。可以通过以下网址访问 SQL Anywhere 论坛:
    http://sqlanywhere-forum.sap.com。


设置 SQL Anywhere 17 的环境变量
-------------------------------

每个使用该软件的用户都必须设置必要的 SQL Anywhere
环境变量。这些变量的设置取决于您使用的特定操作系统，
具体内容在文档中的“SQL Anywhere 服务器 - 数据库管理 >
数据库配置 > SQL Anywhere 环境变量”中讨论。


SQL Anywhere 17 的发行说明
--------------------------


SQL Anywhere 服务器
-------------------

o 现在没有内容。


管理工具
--------

o 在 64 位 Linux 计算机中安装 SQL Anywhere 时，缺省选项为
  安装 64 位版本的图形管理工具
  (SQL Central、Interactive SQL 以及 MobiLink 分析器)。

  也可以选择安装 32 位管理工具。
  此选项仅适用于需要 32 位文件进行重新分发的 OEM 厂商。

  不支持在 64 位的 Linux 上运行 32 位管理工具。

o 要针对管理工具启用 Java Access Bridge，
  请编辑辅助功能属性文件并取消最后两行的注释。

  文件显示形式如下:
#
  # Load the Java Access Bridge class into the JVM
  #
  #assistive_technologies=com.sun.java.accessibility.AccessBridge
  #screen_magnifier_present=true

o 要在 Mac OS X 发布版本中使用管理工具，请安装
  Java 1.7。可以从以下位置获取:

    http://www.oracle.com/technetwork/java/index.html

o 图形管理工具(Interactive SQL、SQL Central 和
  MobiLink 分析器)若在 Solaris
  SPARC 10 计算机中运行，则可能会在启动时崩溃。如果用户使用简体
  中文、UTF-8 语言设置(zh_CN.UTF-8)登录则会出现此行为，并且这与用于该语言的输入方法编辑器(IME)
  有关。

有两种解决方法:

  选项 1:
使用其它语言设置登录，如 "zh_CN.GB18030"。

选项 2:
    如果必须使用 zh_CN.UTF-8，则需在运行图形管理工具前终止 IME 进程。
    通过运行以下命令可从终端
    窗口终止这些进程:
        pkill iiim*


o 如果在 SuSE Linux 系统中从 UTF8BIN 数据库输入
  数据时，字符在 Interactive SQL 实用程序中未正确显示，则需要
  安装 Unicode 字体。

   1. 转到“所有设置”。
   2. 在“系统”类别中，单击 "YaST"(其图标为土豚)。
      提供根口令以启动 YaST。
   3. 在 YaST 中，单击“软件管理”(其图标为一个方块，
      半边为白色，半边为绿色，上面有 Novell 红色字母 "N")。
   4. 在 "YaST 2" 窗口中，键入 "unicode font"，然后
      单击“搜索”。
   5. 在“包”列表中(窗口的右上角)，
      选中所有内容("efont-unicode-bitmap-fonts"、"arphic-ukai-fonts"
      等)，然后单击窗口右下角的
     “接受”按钮。
   6. 重新启动并再次尝试执行操作。


MobiLink
--------

o MobiLink 服务器需要 ODBC 驱动程序才能与
  统一数据库通信。可通过以下链接找到推荐用于
  所支持的统一数据库的 ODBC 驱动程序:
    http://scn.sap.com/docs/DOC-63337

o 有关 MobiLink 支持的平台的信息，请参见:
    http://scn.sap.com/docs/DOC-35654

o 要通过 Mac OS X 中的 Java 1.7 运行 MobiLink 服务器，
  将 -jrepath 选项设置为 libjvm.dylib 文件的完整路径位置。例如:

  -sljava\(-jrepath `/usr/libexec/java_home -v 1.7`/jre/lib/server/libjvm.dylib\)


中继服务器
------------

o 不建议将中继服务器用于 Apache 2.4，因为这样
  会触发 Apache Bugzilla 问题 53555 中标识的行为(请参见
  https://bz.apache.org/bugzilla/show_bug.cgi?id=53555)。
  推荐使用 Apache 2.2 版本。


UltraLite
---------

o 现在没有内容。


操作系统支持
------------------------

o Non-threaded client application support is deprecated.

o RedHat Enterprise Linux 6 Direct I/O 和 THP 支持 - 当和 Direct I/O 一起
  使用时，Red Hat Linux 6 在该操作系统版本所引入的大内存页 (THP) 功能中
  可能存在错误。该错误在 SQL Anywhere 中最有可能的表现形式为
  声明 200505(页面 X 上的校验和失败)。
已创建 Red Hat 错误 891857 跟踪该问题。

  为了解决该问题，SQL Anywhere 不在
  该操作系统上使用 Direct I/O。要使用 Direct I/O，
  请运行以下命令以禁用 THP:
echo never > /sys/kernel/mm/redhat_transparent_hugepage/enabled

o 64 位 Linux 支持 – 某些 64 位 Linux
  操作系统不包含预安装的 32 位兼容库。要使用 32 位软件，
  需要为您的 Linux 发布版本安装 32 位兼容库。
  例如，在 Ubuntu 上运行以下命令:
	sudo apt-get install ia32-libs

  在 RedHat 上运行:
	yum install glibc.i686
	yum install libXrenderer.so.1
	yum install libpk-gtk-module.so
	yum install libcanberra-gtk2.i686
	yum install gtk2-engines.i686

o Linux 对 dbsvc 的支持 - dbsvc 实用程序需要使用 LSB 初始化函数。
某些 Linux 操作系统在缺省情况下不预安装这些函数。
要使用 dbsvc，需要为 Linux 发布版本安装这些函数。
例如，在 Fedora 上运行以下命令:
	yum install redhat-lsb redhat-lsb.i686

o SELinux 支持 – 如果在 SELinux 上运行 SQL Anywhere 时出现问题，
  您有以下几种选择:

o 重新标记共享库，以便可以加载。该解决方案
    在 Red Hat Enterprise Linux 5 上有效，但缺点是不使用
    SELinux 功能。
	find $SQLANY17 -name "*.so" | xargs chcon -t textrel_shlib_t 2>/dev/null

o 安装随 SQL Anywhere 17 提供的策略。在安装的 selinux 目录
    中有策略源。请参见此目录中
    的 README 文件来了解构建和安装此策略的说明。

o 编写您自己的策略。您可以将随 SQL Anywhere 17 
    提供的策略作为基础进行编写。

  o 禁用 SELinux:
/usr/sbin/setenforce 0

o 线程和信号 – 软件中使用的线程和信号的类型
非常重要，因为某些系统可能耗尽这些资源。


    o 在 Linux、AIX、HP-UX 和 Mac OS X 中，SQL Anywhere 使用
pthreads(POSIX 线程)和系统 V 信号。

注意: 在使用系统 V 信号的平台上，
      如果使用 SIGKILL 终止数据库服务器或客户端应用程序，
      则系统 V 信号会发生泄漏。可以使用 ipcrm 命令
      进行清理。此外，使用 _exit() 系统调用终止的
      客户端应用程序也将泄漏系统 V 信号，
      除非 SQL Anywhere 客户端库(如 ODBC 和 DBLib)
      在此调用前已卸载。

o 警报处理 – 仅当开发非线程应用程序
  并使用 SIGALRM 或 SIGIO 处理程序时，该功能才有用。

  SQL Anywhere 在非线程客户端使用 SIGALRM 和 SIGIO 处理程序并启动重复警报(每 200 毫秒一次)。
  为了实施正确的行为，
  必须允许 SQL Anywhere 处理这些信号。

  如果在装载任何 SQL Anywhere 库之前定义 SIGALRM 或 SIGIO 处理
  程序，则 SQL Anywhere 会链接到这些处理程序。
  如果在装载任何 SQL Anywhere 库之后定义处理程序，
  则从 SQL Anywhere 处理程序进行链接。

  如果使用 TCP/IP 通信协议，则 SQL Anywhere 将只在
  非线程客户端使用 SIGIO 处理程序。该处理程序始终都会安装，
  但只在您的应用程序使用 TCP/IP 时才使用。

o 在 Red Hat Enterprise Linux 上，某些专用字符
  在 SQL Central、Interactive SQL (dbisql)、MobiLink 分析器或 SQL
  Anywhere 监控器中可能不显示。

  对于 Unicode 代码点 "U+E844" 和 "U+E863"(指定为专用字符)，
在随 Red Hat Linux 发布版本提供的任何 truetype 字体中，
均不提供轮廓。上述字符是简体中文字符，在 Red Flag (中文版 Linux)发布版本
中作为 zysong.ttf (DongWen-Song) 字体的一部分提供。


