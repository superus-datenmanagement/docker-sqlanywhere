SQL Anywhere 17.0 - Versionshinweise für Unix und Mac OS X

(c) 2019 SAP SE oder ein SAP-Konzernunternehmen. Alle Rechte vorbehalten.


SQL Anywhere 17 installieren
----------------------------

1. Wechseln Sie zu dem erstellten Verzeichnis und starten Sie das
   Installationsskript, indem Sie die folgenden Befehle eingeben:
        cd ga1700
        ./setup

   Eine vollständige Liste der verfügbaren Installationsoptionen erhalten 
   Sie, indem Sie den folgenden Befehl eingeben:
        ./setup -h

2. Befolgen Sie die Anweisungen des Installationsprogramms.


Installationshinweise 
---------------------

o Derzeit keine Einträge.


Dokumentation
-------------

Die Dokumentation finden Sie auf DocCommentXchange unter der folgenden Adresse:
    http://dcx.sap.com

DocCommentXchange ist eine Online-Community für den Zugang zur
SQL Anywhere-Dokumentation im Internet mit Diskussionsforum. DocCommentXchange ist das
Standarddokumentationsformat für SQL Anywhere 17.


MobiLink Deprecated Feature - As of this release, support for running MobiLink on IBM AIX is deprecated.
MobiLink customers who are running on IBM AIX can continue to do so with SQL Anywhere 17.

MobiLink Deprecated Feature - As of this release, support for IBM DB2 consolidated databases is deprecated.
MobiLink customers can continue using IBM DB2 as their consolidated database with SQL Anywhere 17.



SQL Anywhere-Forum
------------------

Das SQL Anywhere-Forum ist eine Website zum Austausch von Fragen und Antworten
über die SQL Anywhere-Software sowie zum Kommentieren von und Abstimmen über
die Fragen und Antworten anderer Benutzer. Besuchen Sie das SQL Anywhere-Forum unter:
    http://sqlanywhere-forum.sap.com.


Umgebungsvariablen für SQL Anywhere 17 festlegen
------------------------------------------------

Jeder Benutzer der Software muss die erforderlichen SQL Anywhere-
Umgebungsvariablen festlegen. Diese hängen vom jeweiligen Betriebssystem ab und werden
in der Dokumentation unter "SQL Anywhere-Server - Datenbankadministration >
Konfiguration Ihrer Datenbank > SQL Anywhere-Umgebungsvariablen" beschrieben.


Versionshinweise für SQL Anywhere 17
------------------------------------


SQL Anywhere-Server
-------------------

o Derzeit keine Einträge.


Administrationstools
--------------------

o Beim Installieren von SQL Anywhere unter 64-Bit-Linux-Systemen wird
  standardmäßig die 64-Bit-Version der grafischen Administrationstools
  (SQL Central, Interactive SQL, MobiLink-Profiler) installiert.

  Sie können auch 32-Bit-Administrationstools installieren. Dies Option gilt nur für
  OEMs, die die 32-Bit-Dateien für den Weitervertrieb benötigen.

  Das Ausführen der 32-Bit-Administrationstools unter 64-Bit-Linux-Systemen
wird nicht unterstützt.

o Um Java Access Bridge für die Administrationstools zu aktivieren,
  bearbeiten Sie die Datei "accessibility.properties"
  und entkommentieren Sie die letzten zwei Zeilen.

  Die Datei wird wie folgt angezeigt:
  #
  # Load the Java Access Bridge class into the JVM
  #
  #assistive_technologies=com.sun.java.accessibility.AccessBridge
  #screen_magnifier_present=true

o Um die Administrationstools unter Mac OS X-Distributionen zu verwenden,
  installieren Sie Java 1.7, das Sie hier finden:

    http://www.oracle.com/technetwork/java/index.html

o Die grafischen Administrationstools (Interactive SQL, SQL Central und
  der MobiLink-Profiler) können beim Starten auf einem Solaris SPARC 10-
  Computer abstürzen, nämlich wenn der Benutzer mit der Spracheinstellung für
  vereinfachtes Chinesisch in UTF-8 (zh_CN.UTF-8) angemeldet ist. Dies
  hängt mit dem IME (Input Method Editor) für diese Sprache zusammen.

  Es gibt zwei Behelfslösungen:

  Option 1:
    Melden Sie sich mit einer anderen Spracheinstellung an, z.B. "zh_CN.GB18030".

Option 2:
    Wenn Sie zh_CN.UTF-8 verwenden müssen, beenden Sie die IME-Prozesse,
    bevor Sie die grafischen Administrationstools starten, indem Sie in einem
    Terminalfenster den folgenden Befehl ausführen:
        pkill iiim*


o Wenn Zeichen beim Eingeben von Daten aus UTF8BIN-Datenbanken unter
  SuSE Linux-Systemen im Interactive SQL-Dienstprogramm nicht richtig
  angezeigt werden, installieren Sie eine Unicode-Schriftart.

   1. Wählen Sie "Alle Einstellungen".
   2. Klicken Sie in der Kategorie "System" auf "YaST" (das Erdferkel-Symbol).
      Geben Sie das Root-Kennwort an, um YaST zu starten.
   3. Klicken Sie in YaST auf "Software Management" (das weiß-grüne Feld
      mit dem roten Novell-Buchstaben "N").
   4. Geben Sie im Fenster "YaST 2" den Begriff "unicode font" ein und klicken
      Sie auf die Suchschaltfläche.
   5. Aktivieren Sie in der Liste "Package" (oben rechts im Fenster)
      alle Optionen ("efont-unicode-bitmap-fonts", "arphic-ukai-fonts" usw.)
      und klicken Sie anschließend unten rechts im Fenster auf "Accept".
   6. Starten Sie das System neu, und versuchen Sie es erneut.


MobiLink
--------

o Der MobiLink-Server benötigt einen ODBC-Treiber für die Kommunikation
  mit konsolidierten Datenbanken. Die für eine unterstützte konsolidierte
  Datenbank empfohlenen ODBC-Treiber finden Sie unter:
    http://scn.sap.com/docs/DOC-63337

o Informationen zu den von MobiLink unterstützten Plattformen finden Sie unter:
    http://scn.sap.com/docs/DOC-35654

o Wenn Sie den MobiLink-Server mit Java 1.7 unter Mac OS X ausführen möchten,
  setzen Sie die Option -jrepath auf den vollständigen Pfad zur Datei libjvm.dylib. Beispiel:

  -sljava\(-jrepath `/usr/libexec/java_home -v 1.7`/jre/lib/server/libjvm.dylib\)


Relay Server
------------

o Die Verwendung des Relay Servers mit Apache 2.4 wird nicht empfohlen, da sie
  das im Apache Bugzilla-Eintrag 53555 beschriebene Verhalten auslöst (siehe
  https://bz.apache.org/bugzilla/show_bug.cgi?id=53555).
  Apache 2.2 ist die empfohlene Version.


UltraLite
---------

o Derzeit keine Einträge.


Betriebssystemunterstützung
---------------------------

o Non-threaded client application support is deprecated.

o Unterstützung von Direct I/O und THP durch RedHat Enterprise Linux 6 -
  RedHat Linux 6 weist einen möglichen Bug in der THP-Funktion (Transparent
  Huge Pages) auf, die in dieser Version des Betriebssystems eingeführt wurde,
  bei Verwendung mit Direct I/O. Das wahrscheinlichste Auftreten dieses Bugs in
  SQL Anywhere ist Assertierung 200505 (Prüfsummenfehler auf Seite X). 
Red Hat-Bug 891857 wurde erstellt, um dieses Problem zu verfolgen.

Um dieses Problem zu umgehen, vermeidet SQL Anywhere unter diesem
  Betriebssystem die Verwendung von Direct I/O. Um Direct I/O zu verwenden,
  deaktivieren Sie THP mit dem folgenden Befehl:
       echo never > /sys/kernel/mm/redhat_transparent_hugepage/enabled

o 64-Bit-Linux-Unterstützung - Einige 64-Bit-Linux-Betriebssysteme enthalten
  keine vorinstallierten 32-Bit-Kompatibilitätsbibliotheken. Um 32-Bit-Software
  zu verwenden, müssen Sie eventuell 32-Bit-Kompatibilitätsbibliotheken für
  Ihre Linux-Distribution installieren. Unter Ubuntu führen Sie einfach
  den folgenden Befehl aus:
	sudo apt-get install ia32-libs

Unter RedHat führen Sie aus:
	yum install glibc.i686
	yum install libXrenderer.so.1
	yum install libpk-gtk-module.so
	yum install libcanberra-gtk2.i686
	yum install gtk2-engines.i686

o Linux-Unterstützung für dbsvc - Das Dienstprogramm dbsvc
erfordert LSB init-Funktionen.
  Bei einigen Linux-Betriebssystemen sind diese Funktionen
nicht standardmäßig vorinstalliert.
  Wenn Sie dbsvc verwenden wollen, müssen diese Funktionen für Ihre
Linux-Distribution installiert werden.  Führen Sie beispielsweise unter Fedora
  den folgenden Befehl aus:
	yum install redhat-lsb redhat-lsb.i686

o SELinux-Unterstützung - Wenn Sie Probleme mit der Ausführung von SQL Anywhere
  unter SELinux haben, gibt es die folgenden Lösungsmöglichkeiten:

o Benennen Sie die gemeinsam genutzten Bibliotheken um, damit sie geladen werden können. 
Dies funktioniert unter Red Hat Enterprise Linux 5, hat aber
    den Nachteil, dass die SELinux-Funktionen nicht verwendet werden.
	find $SQLANY17 -name "*.so" | xargs chcon -t textrel_shlib_t 2>/dev/null

  o Installieren Sie die mit SQL Anywhere 17 bereitgestellte Richtlinie.
    Im selinux-Verzeichnis der Installation befinden sich Quelldateien der
    Richtlinie. Anweisungen zum Erstellen und Installieren dieser Richtlinie
    finden Sie in der README-Datei in diesem Verzeichnis.

  o Schreiben Sie Ihre eigene Richtlinie. Sie können die mit SQL Anywhere 17
    bereitgestellte Richtlinie als Ausgangspunkt verwenden.

  o Deaktivieren Sie SELinux:
        /usr/sbin/setenforce 0

o Threads und Semaphore - Die Art der in der Software verwendeten Threads und
  Semaphore kann ziemlich wichtig sein, da es bei einigen Systemen zu
  Knappheit dieser Ressourcen kommen kann.

    o Unter Linux, AIX, HP-UX und Mac OS X verwendet SQL Anywhere pthreads
      (POSIX-Threads) und System V-Semaphore.

      Hinweis: Auf Plattformen, auf denen System V-Semaphore verwendet werden,
      gehen diese verloren, wenn der Datenbankserver oder eine Clientanwendung
      mit SIGKILL beendet wird. Bereinigen Sie sie mit dem Befehl "ipcrm".
       Außerdem gehen System V-Semaphore verloren, wenn Clientanwendungen mit
       dem _exit()-Systemaufruf beendet werden, es sei denn, die SQL Anywhere-
       Clientbibliotheken (z.B. ODBC und DBLib) werden vorher entladen.

o Verarbeitung von Alarmsignalen - Dies ist nur von Interesse, wenn Sie
  Non-Threaded-Anwendungen entwickeln und SIGALRM- oder SIGIO-Handler verwenden.

  SQL Anywhere verwendet je einen SIGALRM- und SIGIO-Handler in Non-Threaded-
  Clients und startet einen sich wiederholenden Alarm (alle 200 ms). Korrektes
  Verhalten erhalten Sie, wenn SQL Anywhere diese Signale verarbeiten kann.

  Falls Sie einen SIGALRM- oder SIGIO-Handler definieren, bevor Sie
  SQL Anywhere-Bibliotheken laden, hängt sich SQL Anywhere an diese Handler an.
  Falls Sie einen Handler nach dem Laden von SQL Anywhere-Bibliotheken
  definieren, müssen Sie den Handler an SQL Anywhere anhängen.

  Bei Verwendung des TCP/IP-Kommunikationsprotokolls benutzt SQL Anywhere
  SIGIO-Handler nur in Non-Threaded-Clients. Dieser Handler ist immer
  installiert, wird aber nur benutzt, wenn die Anwendung TCP/IP verwendet.

o Unter Red Hat Enterprise Linux werden einige Zeichen des privaten
  Nutzungsbereichs in SQL Central, Interactive SQL (dbisql), dem MobiLink-
  Profiler oder dem SQL Anywhere-Monitor möglicherweise nicht richtig angezeigt.

  Für die Unicode-Codepoints "U+E844" und "U+E863" (als Zeichen für private
  Nutzung reserviert) werden in keiner der mit der Red Hat-Linux-Distribution
  ausgelieferten Truetype-Schriften Glyphen bereitgestellt. Die betreffenden
  Zeichen sind vereinfachte chinesische Schriftzeichen und in der
  Linux-Distribution Red Flag (chinesische Distribution) im Rahmen der Schrift
  zysong.ttf (DongWen-Song) verfügbar.

