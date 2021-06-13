SQL Anywhere 17.0 Release Notes for Unix and Mac OS X

Copyright (c) 2019 SAP SE or an SAP affiliate company. All rights reserved.


Installing SQL Anywhere 17
--------------------------

1. Change to the created directory and start the setup script by running
   the following commands:
        cd ga1700
        ./setup

   For a complete list of the available setup options, run the following
   command:
        ./setup -h

2. Follow the instructions in the setup program.


Installation notes
------------------

o There are no items at this time.
   
   
Documentation
-------------

The documentation is available on the SAP Help Portal at:
    https://help.sap.com/viewer/product/SAP_SQL_Anywhere/17.0/en-US

You can provide feedback on topics in the SAP Help Portal: in the right 
pane, click the Yes or No button to indicate whether a topic was helpful, 
and then add your feedback in the comment box.


SQL Anywhere Forum
------------------

The SQL Anywhere Forum is a web site where you can ask and answer questions 
about the SQL Anywhere software and comment and vote on the questions of 
others and their answers. Visit the SQL Anywhere Forum at: 
    http://sqlanywhere-forum.sap.com. 


Setting environment variables for SQL Anywhere 17
-------------------------------------------------

Each user who uses the software must set the necessary SQL Anywhere environment
variables. These depend on your particular operating system, and are discussed
in the documentation in "SQL Anywhere Server - Database Administration > 
Database Configuration > SQL Anywhere environment variables".


Release notes for SQL Anywhere 17
---------------------------------


SQL Anywhere Server
-------------------

o There are no items at this time.

  
Administration tools
--------------------

o When installing SQL Anywhere on 64-bit Linux machines, the default option 
  is to install the 64-bit version of the graphical administration tools 
  (SQL Central, Interactive SQL, and the MobiLink Profiler).  
      
  You also have the option to install 32-bit administration tools. This 
  option is only for OEMs who need the 32-bit files for redistribution.  
	    
  Running the 32-bit administration tools on 64-bit Linux is not supported. 
		
o To enable the Java Access Bridge for the administration tools, 
  edit the accessibility.properties file and uncomment the last two lines.

  The file appears as follows:
  #
  # Load the Java Access Bridge class into the JVM
  #
  #assistive_technologies=com.sun.java.accessibility.AccessBridge
  #screen_magnifier_present=true

o To use the administration tools on Mac OS X distributions, install 
  Java 1.7. It is available from:
  
    http://www.oracle.com/technetwork/java/index.html
  
o The graphical administration tools Interactive SQL, SQL Central, and 
  the MobiLink Profiler) can crash on startup if they are run on a Solaris 
  SPARC 10 computer. This behavior occurs if the user is logged in with a 
  Simplified Chinese, UTF-8 language setting (zh_CN.UTF-8) and is related 
  o the input method editor (IME) that is used for that language. 

  There are two workarounds:

  Option 1:
    Log in using another language setting, such as "zh_CN.GB18030".

  Option 2:
    If you have to use zh_CN.UTF-8, then terminate the IME processes before 
    running the graphical administration tools by running the following 
    command from a Terminal window:
        pkill iiim*


o If characters do not display correctly in the Interactive SQL utility when 
  you input data from a UTF8BIN database on SuSE Linux systems, you need to 
  install a Unicode font.
        
   1. Go to "All Settings".
   2. In the "System" category, click "YaST" (its icon 
      is an aardvark). Provide the root password to start YaST.
   3. In YaST, click "Software Management" (its icon is a box that is half 
      white and half green with a Novell red letter "N" on it).
   4. In the "YaST 2" window, type "unicode font", and then
      click "Search".
   5. In the "Package" list (top right corner of the window), 
      check everything ("efont-unicode-bitmap-fonts", "arphic-ukai-fonts", 
      and so on), and then click the "Accept" button in the bottom right 
      corner of the window.
   6. Reboot and retry the operation.

     
MobiLink
--------

o The MobiLink server requires an ODBC driver to communicate with the 
  consolidated databases. The recommended ODBC drivers for a supported 
  consolidated database can be found at:
    http://scn.sap.com/docs/DOC-63337

o For information about the platforms supported by MobiLink, see:
    http://scn.sap.com/docs/DOC-35654
  
o To run the MobiLink server with Java 1.7 on Mac OS X, set the -jrepath option
  to the full path location of the libjvm.dylib file. For example:
  
  -sljava\(-jrepath `/usr/libexec/java_home -v 1.7`/jre/lib/server/libjvm.dylib\)


Relay Server
------------

o Using the Relay Server with Apache 2.4 is not recommended as it 
  triggers the behavior identified in Apache Bugzilla issue 53555 (see
  https://bz.apache.org/bugzilla/show_bug.cgi?id=53555).
  Apache 2.2 is the recommended version.
    

UltraLite
---------

o There are no items at this time.


Operating system support
------------------------

o Non-threaded client application support is deprecated.

o RedHat Enterprise Linux 6 Direct I/O and THP support - Red Hat Linux 6 has a
  possible bug in the transparent huge pages (THP) feature introduced in this
  operating system version, when used with Direct I/O. The most likely 
  manifestation of this bug in SQL Anywhere is assertion 200505 (checksum 
  failure on page X). Red Hat bug 891857 has been created to track this issue.

  To work around this issue, SQL Anywhere avoids using Direct I/O on this 
  operating system. To use Direct I/O, disable THP by running following command:
       echo never > /sys/kernel/mm/redhat_transparent_hugepage/enabled

o 64-bit Linux support - Some 64-bit Linux operating systems do not include
  preinstalled 32-bit compatibility libraries. To use 32-bit software,
  you may need to install 32-bit compatibility libraries for your Linux
  distribution. For example, on Ubuntu, run the following command:
	sudo apt-get install ia32-libs
	
  On RedHat, run:
	yum install glibc.i686
	yum install libXrenderer.so.1
	yum install libpk-gtk-module.so 
	yum install libcanberra-gtk2.i686 
	yum install gtk2-engines.i686

o Linux support for dbsvc - The dbsvc utility requires the LSB init-functions.
  Some Linux operating systems do not preinstall these functions by default.
  To use dbsvc, you need to install them for your Linux distribution. 
  For example, on Fedora, run the following command:
	yum install redhat-lsb redhat-lsb.i686 
	
o SELinux support - If you are having problems running SQL Anywhere on SELinux,
  then you have the following options:

  o Relabel the shared libraries so that they can be loaded. This solution 
    works on Red Hat Enterprise Linux 5, but has the drawback of not using the
    SELinux features.
	find $SQLANY17 -name "*.so" | xargs chcon -t textrel_shlib_t 2>/dev/null

  o Install the policy provided with SQL Anywhere 17. In the selinux 
    directory of your installation there are policy sources. See the README 
    file in that directory for instructions on building and installing that 
    policy.

  o Write your own policy. You can use the policy provided with 
    SQL Anywhere 17 as a starting point.

  o Disable SELinux:
        /usr/sbin/setenforce 0
	
o Threads and semaphores - The type of threads and semaphores used in
  software can be quite important, as some systems can run out of these
  resources.

    o On Linux, AIX, HP-UX, and Mac OS X, SQL Anywhere uses
      pthreads (POSIX threads) and System V semaphores.
      
      Note: On platforms where System V semaphores are used, if the database
      server or a client application is terminated with SIGKILL, then System V 
      semaphores are leaked. Clean them up by using the ipcrm command.  
      In addition, client applications that terminate using the _exit() system 
      call also leak System V semaphores unless the SQL Anywhere client 
      libraries (such as ODBC and DBLib) are unloaded before this call.

o Alarm handling - This feature is of interest only if you are developing
  non-threaded applications and use SIGALRM or SIGIO handlers.

  SQL Anywhere uses a SIGALRM and a SIGIO handler in non-threaded
  clients and starts up a repeating alarm (every 200 ms). For correct behavior,
  SQL Anywhere must be allowed to handle these signals.

  If you define a SIGALRM or SIGIO handler before loading any SQL Anywhere
  libraries, then SQL Anywhere chains to these handlers.
  If you define a handler after loading any SQL Anywhere libraries,
  then chain from the SQL Anywhere handlers.

  If you use the TCP/IP communication protocol, SQL Anywhere uses
  SIGIO handlers in only non-threaded clients. This handler is always
  installed, but it is used only if your application makes use of TCP/IP.

o On Red Hat Enterprise Linux, some Private Use characters may not display
  in SQL Central, Interactive SQL (dbisql), the MobiLink Profiler, or the SQL
  Anywhere Monitor.
  
  For the Unicode codepoints "U+E844" and "U+E863" (designated as private use
  characters) no glyphs are provided in any of the truetype fonts provided
  with the Red Hat Linux distribution. The characters in question are
  Simplified Chinese characters and are available in the Red Flag (Chinese
  Linux) distribution as part of their zysong.ttf (DongWen-Song) font.
		    

