SQL Anywhere 17.0 UNIX 版および Mac OS X 版リリースノート

Copyright (c) 2019 SAP SE or an SAP affiliate company. All rights reserved.


SQL Anywhere 17 のインストール
--------------------------

1. 作成されたディレクトリに移動し、次のコマンドを実行して
   設定スクリプトを開始します。
        cd ga1700
        ./setup

   使用可能な設定オプションのリストを表示するには、
   次のコマンドを実行します。
./setup -h

2. セットアッププログラムの指示に従います。


インストールに関する注意事項
------------------

o 現時点では何もありません。


マニュアル
-------------

マニュアルは DocCommentXchange にあります。アドレスは次のとおりです。
    http://dcx.sap.com

DocCommentXchange は Web 上の SQL Anywhere マニュアルを参照して
議論するためのオンラインコミュニティです。DocCommentXchange は 
SQL Anywhere 17 のデフォルトのマニュアルフォーマットです。


MobiLink Deprecated Feature - As of this release, support for running MobiLink on IBM AIX is deprecated.
MobiLink customers who are running on IBM AIX can continue to do so with SQL Anywhere 17.

MobiLink Deprecated Feature - As of this release, support for IBM DB2 consolidated databases is deprecated.
MobiLink customers can continue using IBM DB2 as their consolidated database with SQL Anywhere 17.



SQL Anywhere フォーラム
------------------

SQL Anywhere フォーラムは、SQL Anywhere ソフトウェアに関する質問や
回答を投稿できる Web サイトです。他の投稿者の質問やその回答にコメントや
評価を加えることもできます。SQL Anywhere フォーラムの URL は次のとおりです。
    http://sqlanywhere-forum.sap.com


SQL Anywhere 17 の環境変数の設定
-------------------------------------------------

ソフトウェアを使用するユーザごとに、SQL Anywhere の環境変数を設定する
必要があります。必要な環境変数はオペレーティングシステムによって異なり、
"SQL Anywhere サーバ - データベース管理 >
データベースの設定 > SQL Anywhere の環境変数" で説明しています。


SQL Anywhere 17 のリリースノート
---------------------------------


SQL Anywhere サーバ
-------------------

o 現時点では何もありません。


管理ツール
--------------------

o 64 ビットの Linux マシン上に SQL Anywhere をインストールする場合、デフォルトの
  オプションでは、64 ビット版のグラフィカル管理ツール (SQL Central、
  Interactive SQL、Mobile Link プロファイラ) がインストールされます。

  32 ビットの管理ツールをインストールするというオプションもあります。
 ただし、このオプションは、32 ビットファイルの再ディストリビューションを必要とする OEM の場合に限ります。

64 ビット Linux での 32 ビットの管理ツールの実行はサポートされていません。

o 管理ツール用の Java Access Bridge を有効にするには、
  accessibility.properties ファイルを編集して最後の 2 行をコメント解除します。

  ファイルには次のように表示されます。
  #
  # Load the Java Access Bridge class into the JVM
  #
  #assistive_technologies=com.sun.java.accessibility.AccessBridge
  #screen_magnifier_present=true

o Mac OS X ディストリビューションで管理ツールを使用するには、
  Java 1.7 をインストールします。これは以下のサイトから入手できます。

    http://www.oracle.com/technetwork/java/index.html

o グラフィカル管理ツール (Interactive SQL、SQL Central、Mobile Link プロファイラ) 
  を Solaris SPARC 10 コンピュータで実行している場合は、起動時にクラッシュすることが
  あります。この問題が発生するのは、ユーザが簡体中国語、UTF-8 言語設定 
  (zh_CN.UTF-8) を使用してログインしている場合です。この問題は、その言語で
  使用されている Input Method Editor (IME) に関連しています。

  回避策は 2 つあります。

  オプション 1:
"zh_CN.GB18030" などの別の言語設定を使用してログインします。

  オプション 2:
    zh_CN.UTF-8 を使用する必要がある場合は、IME プロセスを終了してから、
    グラフィカル管理ツールを実行します。この処理は、以下のコマンドを実行して、
    端末ウィンドウから実行できます。
        pkill iiim*


o SuSE Linux システム上で UTF8BIN からデータを入力したときに Interactive SQL 
  ユーティリティで文字が正しく表示されない場合は、ユニコードフォントを
  インストールする必要があります。

   1. "すべての設定" に移動します。
   2. "システム" カテゴリで、"YaST" (アイコンはツチブタです) 
      をクリックします。ルートパスワードを入力して YaST を起動します。
   3. YaST で、"ソフトウェア管理" (アイコンは半分が白、半分が緑のボックスで、
      Novell の赤い文字 "N" が示されています) をクリックします。
   4. "YaST 2" ウィンドウで、"unicode font" と入力し、
      "検索" をクリックします。
   5. "パッケージ" リスト (ウィンドウの右上隅) で、
      すべて ("efont-unicode-bitmap-fonts"、"arphic-ukai-fonts" など) 
      にチェックし、ウィンドウの右下隅で "確定" ボタンを
      クリックします。
   6. 再起動し、操作を再試行します。


Mobile Link
--------

o Mobile Link サーバでは、統合データベースと通信するために、
  ODBC ドライバが必要です。サポートされている統合データベースの推奨
  ODBC ドライバは、次から入手できます。
    http://scn.sap.com/docs/DOC-63337

o Mobile Link でサポートされているプラットフォームの詳細については、
    http://scn.sap.com/docs/DOC-35654
    を参照してください。

o Mac OS X で Java 1.7 と共に Mobile Link サーバ,を実行するには、-jrepath オプションを
  libjvm.dylib ファイルのロケーション (フルパス) に設定します。たとえば、次のようになります。

  -sljava\(-jrepath `/usr/libexec/java_home -v 1.7`/jre/lib/server/libjvm.dylib\)


Relay Server
------------

o Apache Bugzilla の問題 53555 (https://bz.apache.org/bugzilla/show_bug.cgi?id=53555 を
  参照) で特定されている動作をトリガするため、Relay Server を Apache 2.4 とともに
  使用することはおすすめしません。
  推奨バージョンは Apache 2.2 です。


Ultra Light
---------

o 現時点では何もありません。


オペレーティングシステムのサポート
------------------------

o Non-threaded client application support is deprecated.

o RedHat Enterprise Linux 6 の Direct I/O および THP のサポート - Red Hat Linux 6 では、
  このオペレーティングシステムバージョンで導入された Transparent Huge Page (THP) 機能に
  バグがあり、Direct I/O と使用した場合に発現する可能性があります。SQL Anywhere で
  このバグを表すものとして最も可能性が高いのは、アサーション 200505 (X ページの障害の
  チェックサム) です。Red Hat bug 891857 は、この問題を追跡するために作成されました。

  この問題を回避するため、SQL Anywhere では、このオペレーティングシステムで
  Direct I/O を使用しないようにしています。Direct I/O を使用する場合は、次のコマンドを
  実行して THP を無効にしてください。
       echo never > /sys/kernel/mm/redhat_transparent_hugepage/enabled

o 64 ビット Linux のサポート - 一部の 64 ビット Linux オペレーティングシステムには、
  32 ビット互換性ライブラリがプリインストールされていません。32 ビットのソフトウェア
  を使用するには、お使いの Linux ディストリビューション用の 32 ビット互換性ライブラリ
  をインストールする必要があります。たとえば、Ubuntu では次のコマンドを実行します。
	sudo apt-get install ia32-libs

RedHat で次のコマンドを実行します。
	yum install glibc.i686
	yum install libXrenderer.so.1
	yum install libpk-gtk-module.so
	yum install libcanberra-gtk2.i686
	yum install gtk2-engines.i686

o dbsvc に対する Linux のサポート - dbsvc ユーティリティを使用するには、LSB init ファンクションが必要です。
一部の Linux オペレーティングシステムには、これらのファンクションがデフォルトでプレインストールされていません。
dbsvc を使用するには、Linux ディストリビューション用にこれらをインストールする必要があります。
たとえば、Fedora では次のコマンドを実行します。
	yum install redhat-lsb redhat-lsb.i686

o SELinux のサポート - SELinux で SQL Anywhere を実行できない場合は、
  次の解決方法があります。

o 共有ライブラリをロードできるようにラベルを変更します。この方法は、
    Red Hat Enterprise Linux 5 で機能しますが、SELinux の機能を使用
    できないという欠点があります。
	find $SQLANY17 -name "*.so" | xargs chcon -t textrel_shlib_t 2>/dev/null

o SQL Anywhere 17 に付属するポリシーをインストールします。インストールの
    selinux ディレクトリにポリシーのソースがあります。ポリシーの構築と
    インストールの指示については、そのディレクトリ内の
    README ファイルを参照してください。

o 独自のポリシーを作成します。SQL Anywhere 17 に付属するポリシーを
    テンプレートとして使用できます。

  o 次のように入力して SELinux を無効にします。
/usr/sbin/setenforce 0

o スレッドとセマフォ - ソフトウェアで使用されているスレッドと
  セマフォの種類は重要です。システムによっては、これらの
  リソースが不足する可能性があります。

    o Linux、AIX、HP-UX、Mac OS X では、SQL Anywhere で
      pthreads (POSIX スレッド) と System V のセマフォが使用されます。

      注: System V セマフォを使用しているプラットフォームでは、データベースサーバ
      またはクライアントアプリケーションが SIGKILL で終了する場合、System V セマフォが
      リークされます。これらをクリーンアップするには、ipcrm コマンドを使用します。
      また、システム呼び出し _exit() を使用して終了するクライアントアプリケーションも、
      この呼び出しより前に SQL Anywhere クライアントライブラリ (ODBC、DBLib など) 
      がアンロードされていない限り、System V セマフォをリークします。

o アラーム処理 - この機能は、非スレッド化アプリケーションの
  開発に SIGALRM または SIGIO ハンドラを使用している場合にのみ関係します。

  SQL Anywhere では、非スレッド化クライアントで SIGALRM と SIGIO のハンドラが
  使用され、200 ミリ秒ごとに繰り返しアラームが開始されます。処理が正常に
  行われるには、SQL Anywhere でこれらの信号を処理できる必要があります。

  SQL Anywhere のライブラリをロードする前に SIGALRM または SIGIO のハンドラを
  定義すると、SQL Anywhere はこれらのハンドラに接続されます。
  SQL Anywhere のライブラリのロード後にハンドラを定義した場合は、
  SQL Anywhere のハンドラから接続します。

  TCP/IP 通信プロトコルを使用する場合、SQL Anywhere では、非スレッド化クライアント
  でのみ SIGIO のハンドラが使用されます。このハンドラは常にインストールされますが、
  使用されるのは、アプリケーションで TCP/IP を使用する場合だけです。

o Red Hat Enterprise Linux では、一部の私用文字が SQL Central、Interactive 
  SQL (dbisql)、Mobile Link プロファイラ、または
   SQL Anywhere モニタで表示されない場合があります。

  Red Hat Linux ディストリビューションに付属するどの TrueType
  フォントにも、ユニコードのコードポイント "U+E844" と "U+E863"
  (私用文字) のグリフはありません。問題の文字は簡体字中国語の
  文字で、Red Flag (中国語版 Linux) ディストリビューションで
  zysong.ttf (DongWen-Song) フォントに含まれます。

