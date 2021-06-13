SQL Anywhere 17.0 - Notes de mise à jour pour Unix, Linux et Mac OS X

Copyright (c) 2019 SAP SE ou société affiliée SAP. Tous droits réservés.


Installation de SQL Anywhere 17
-------------------------------

1. Dans le répertoire créé, lancez le script d'installation en exécutant
   les commandes suivantes :
        cd ga1700
        ./setup

   Pour obtenir la liste complète des options d'installation disponibles,
   exécutez cette commande :
        ./setup -h

2. Suivez les instructions du programme d'installation.


Notes d'installation
--------------------

o Aucune information disponible.


Documentation
-------------

La documentation est disponible sur DocCommentXchange :
    http://dcx.sap.com

DocCommentXchange est un site communautaire sur lequel vous pouvez consulter
et commenter la documentation de SQL Anywhere. DocCommentXchange est le format
de documentation par défaut pour SQL Anywhere 17.


MobiLink Deprecated Feature - As of this release, support for running MobiLink on IBM AIX is deprecated.
MobiLink customers who are running on IBM AIX can continue to do so with SQL Anywhere 17.

MobiLink Deprecated Feature - As of this release, support for IBM DB2 consolidated databases is deprecated.
MobiLink customers can continue using IBM DB2 as their consolidated database with SQL Anywhere 17.



Forum SQL Anywhere
------------------

Le forum SQL Anywhere est un site Web sur lequel vous pouvez poser
des questions sur le logiciel SQL Anywhere et apporter des réponses,
ainsi que commenter les questions et réponses des autres participants.
Rendez-vous sur le forum SQL Anywhere à l'adresse :
    http://sqlanywhere-forum.sap.com.


Configuration des variables d'environnement pour SQL Anywhere 17
----------------------------------------------------------------

Les variables d'environnement de SQL Anywhere doivent être définies
préalablement à l'utilisation du logiciel. Leur paramétrage dépend du système
d'exploitation. Pour le connaître, consultez la section "Database
Configuration" > "SQL Anywhere environment variables" du manuel "SQL Anywhere
Server - Database Administration".


SQL Anywhere 17 - Notes de mise à jour
--------------------------------------


Serveur SQL Anywhere
--------------------

o Aucune information disponible.


Outils d'administration
-----------------------

o Lorsque vous installez SQL Anywhere sur des machines Linux 64 bits, les
  outils d'administration graphiques (SQL Central, Interactive SQL et le
  Profileur MobiLink) s'installent par défaut en version 64 bits.

  Vous avez également la possibilité de les installer en version 32 bits,
   cette option étant réservée aux OEM dont la redistribution inclut des
  fichiers 32 bits.

  Les outils d'administration 32 bits ne sont pas pris en charge sur
  Linux 64 bits.

o Pour activer Java Access Bridge pour les outils d'administration,
  modifiez le fichier accessibility.properties en supprimant la mise en
  commentaire des deux dernières lignes.

  Le fichier se présente ainsi :
  #
  # Load the Java Access Bridge class into the JVM
  #
  #assistive_technologies=com.sun.java.accessibility.AccessBridge
  #screen_magnifier_present=true

o Pour utiliser les outils d'administration sur les distributions
  Mac OS X, installez Java 1.7.
  Cette version est téléchargeable depuis :

    http://www.oracle.com/technetwork/java/index.html

o Les outils d'administration graphiques (Interactive SQL, SQL Central
  et le Profileur MobiLink) peuvent subir un incident au démarrage
  lorsqu'ils sont exécutés sur un ordinateur Solaris SPARC 10.
  Cela se produit lorsque l'utilisateur se connecte en utilisant le paramètre
  de langue Chinois simplifié en UTF-8 (zh_CN.UTF-8),
  en raison de l'éditeur de méthode de saisie utilisé pour cette langue.

  Il existe deux contournements :

  Option 1 :
    Se connecter en utilisant un autre paramètre de langue, comme "zh_CN.GB18030".

Option 2 :
    Si vous devez utiliser zh_CN.UTF-8, mettez fin aux processus de l'éditeur de
    méthode de saisie avant d'exécuter les outils d'administration
    graphiques. Pour ce faire, exécutez la commande suivante dans une fenêtre
    de terminal :
        pkill iiim*


o Si les caractères ne s'affichent pas correctement dans l'utilitaire
  Interactive SQL lorsque vous récupérez les données depuis une base UTF8BIN
   sur systèmes SuSE Linux, vous devez installer un police Unicode.

1. Affichez les paramètres.
   2. Dans la catégorie "System, cliquez sur "YaST"
       (icône représentant un fourmilier). Saisissez
       le mot de passe administrateur pour démarrer YaST.
   3. Dans YaST, cliquez sur "Software Management"
      (icône représentant une boîte blanche et verte sur laquelle
      figure l'initiale N de Novell en rouge).
   4. Dans la fenêtre "YaST 2", saisissez "unicode font", puis cliquez
      sur "Search".
   5. Dans l'angle supérieur droit de la fenêtre,
      cochez tous les éléments de la liste "Package" ("efont-unicode-bitmap-fonts",
      "arphic-ukai-fonts", etc.),
      puis cliquez sur le bouton "Accept"
      dans l'angle inférieur droit.
   6. Redémarrez et renouvelez l'opération.


MobiLink
--------

o Le serveur MobiLink nécessite un pilote ODBC pour communiquer avec
  les bases de données consolidées. Les pilotes ODBC recommandés pour assurer
  la prise en charge des bases consolidées sont répertoriés à la page :
    http://scn.sap.com/docs/DOC-63337

o Pour connaître les plates-formes prises en charge par MobiLink, consultez :
    http://scn.sap.com/docs/DOC-35654

o Pour exécuter le serveur MobiLink avec Java 1.7 sur Mac OS X, définissez
  le chemin complet du fichier libjvm.dylib comme valeur de l'option -jrepath. Par exemple :

  -sljava\(-jrepath `/usr/libexec/java_home -v 1.7`/jre/lib/server/libjvm.dylib\)


Serveur relais
--------------

o Il est déconseillé d'utiliser le serveur relais avec Apache 2.4 car
  cela déclenche le comportement identifié sur Apache Bugzilla,
  problème 53555 (voir
  https://bz.apache.org/bugzilla/show_bug.cgi?id=53555).
  La version recommandée est Apache 2.2.


UltraLite
---------

o Aucune information disponible.


Système d'exploitation requis
-----------------------------

o Non-threaded client application support is deprecated.

o Prise en charge des THP et des E/S directes de RedHat EnterpriseLinux 6 - Il
  est possible qu'un bug se produise avec la nouvelle fonctionnalité THP
  (transparent huge pages, pages très volumineuses transparentes) de cette
  version de système d'exploitation lorsqu'elle est utilisée avec des E/S
  directes. Ce bug se traduira probablement par une assertion 200505 dans SQL
  Anywhere (erreur de checksum à la page X). Pour le suivi du problème, le bug
  Red Hat 891857 a été créé.

Pour contourner le problème, SQL Anywhere évite d'utiliser des E/S directes
  sur ce système d'exploitation. Pour en utiliser, vous devez désactiver
  les THP à l'aide de la commande suivante :
       echo never > /sys/kernel/mm/redhat_transparent_hugepage/enabled

o Prise en charge de Linux 64 bits - Les bibliothèques de compatibilité
  32 bits ne sont pas pré-installées sur certains systèmes d'exploitation
  Linux 64 bits. Pour utiliser des logiciels 32 bits, vous devrez probablement
  installer les bibliothèques de compatibilité 32 bits adaptées à votre
  distribution Linux. Sur Ubuntu, par exemple, exécutez cette commande :
	sudo apt-get install ia32-libs

  Sur RedHat, exécutez :
	yum install glibc.i686
	yum install libXrenderer.so.1
	yum install libpk-gtk-module.so
	yum install libcanberra-gtk2.i686
	yum install gtk2-engines.i686

o Prise en charge de dbsvc sur Linux - Les fonctions LSB init-functions sont
  nécessaires pour cet utilitaire. Or, certains systèmes d'exploitation ne
  les pré-installent pas par défaut. Pour utiliser dbsvc, vous devez donc
  installer les fonctions adaptées à votre distribution Linux. Par exemple,
  sur Fedora, exécutez cette commande :
	yum install redhat-lsb redhat-lsb.i686

o Prise en charge de SELinux - Si vous rencontrez des problèmes à l'exécution
  de SQL Anywhere sur SELinux, les possibilités suivantes s'offrent à vous :

o Modifiez l'étiquette des bibliothèques partagées pour en permettre le
    chargement. Si cette solution fonctionne sur Red Hat Enterprise Linux 5,
    elle a pour inconvénient de ne pas utiliser les fonctionnalités SELinux.
    find $SQLANY17 -name "*.so" | xargs chcon -t textrel_shlib_t 2>/dev/null

o Installez la stratégie fournie avec SQL Anywhere 17. Vous trouverez des
    sources de stratégie dans le répertoire selinux de votre installation.
    Le fichier README présent dans ce répertoire fournit des instructions de
    construction et d'installation de stratégie.

o Rédigez votre propre stratégie. Vous pouvez vous servir de celle fournie 
    avec SQL Anywhere 17 comme point de départ.

  o Désactivez SELinux :
        /usr/sbin/setenforce 0

o Threads et sémaphores - Les types de thread et de sémaphore utilisés dans
  le logiciel sont assez déterminants dans la mesure où ces ressources peuvent
  s'épuiser sur certains systèmes.

    o Sur Linux, AIX, HP-UX et Mac OS X, SQL Anywhere utilise des threads
      pthreads (threads POSIX) et des sémaphores System V.

Remarque : Sur les plates-formes qui utilisent des sémaphores System V,
      si l'arrêt du serveur de base de données ou d'une application cliente
      est exécuté par SIGKILL, une perte se produit au niveau des sémaphores
      System V. Nettoyez-les manuellement ceux-ci à l'aide de la
      commande ipcrm. En outre, les applications clientes arrêtées à l'aide
      du système _exit() peuvent également entraîner une telle perte, sauf si
      les bibliothèques clientes SQL Anywhere (comme ODBC et DBLib) sont
      déchargées avant l'appel.

o Gestion des alarmes - Cette fonctionnalité vous concerne uniquement si vous
  développez des applications sans thread et utilisez les gestionnaires
  SIGALRM ou SIGIO.

  SQL Anywhere utilise un gestionnaire SIGALRM et SIGIO pour les clients sans
  thread et démarre une alarme avec signaux répétitifs (toutes les 200 ms).
  Pour un fonctionnement correct, il doit être autorisé à gérer ces signaux.

  Si vous paramétrez un gestionnaire SIGALRM ou SIGIO avant de charger les
  bibliothèques SQL Anywhere, le logiciel enchaîne à partir de ce
  gestionnaire. Si vous paramétrez le gestionnaire après le chargement d'une
  quelconque bibliothèque SQL Anywhere, enchaînez à partir des
  gestionnaires SQL Anywhere.

  Avec le protocole de communication TCP/IP, SQL Anywhere utilise le
  gestionnaire SIGIO pour les clients sans thread uniquement. Celui-ci est
  toujours installé, mais il ne sert que si votre application utilise le
  protocole TCP/IP.

o Sur Red Hat Enterprise Linux, certains caractères d'usage privé ne
  s'affichent pas dans SQL Central, Interactive SQL (dbisql), le Profileur
  MobiLink ou le Moniteur SQL Anywhere.

  Concernant les points de code Unicode "U+E844" et "U+E863" (désignés comme
  des caractères d'usage privé), aucun glyphe n'est fourni dans aucune des
  polices TrueType fournies avec la distribution Red Hat Linux. Il s'agit de
  caractères chinois simplifiés disponibles dans la distribution Red Flag
  (Linux en chinois) et qui font partie de la police zysong.ttf (DongWen-Song).

