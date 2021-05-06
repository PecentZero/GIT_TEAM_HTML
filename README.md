# TEAM-HTML-TERM-PROJECT
Software Security 2021 Team HTML Term Project - SNS for exchange students

how to execute:

1. install apache2
    - sudo apt update
    - sudo apt install apache2
2. install mysql
    - follow instruction on this site:
    - https://docs.rackspace.com/support/how-to/install-mysql-server-on-the-ubuntu-operating-system/
    - recommend id : "root" pw: 'root'
3. install cgicc
    - follow "C++ CGI Library" section on this site:
    - tutorialspoint.com/cplusplus/cpp_web_programming.htm
4. install cppconn
    - sudo apt install libmysqlcppconn-dev
5. go to /var/www/ directory
    - cd /var/www
    - if there already exists '/var/www/html' directory, remove it "sudo rm -r html"
6. git-clone to make new html directory and all other necessary files
7. configure apache2 server to run cgi programs.
    - 7-1.We attached "apache2.conf file , if you want, just copy it
    - or you can edit like below
    - => vim /etc/apache2.conf   
    - and add below setence after </Directory> part  

    ServerName localhost
    <Directory /var/www/cgi-bin>
    	AllowOverride None
    	Options +ExecCGI
    	Allow from all
    	AddHandler cgi-script .cgi
    </Directory>

    - 7-2 We attached "serve-cgi-bin.conf" file , if you want, just copy it
      or you can edit like below
      => vim /etc/apache2/conf-available/serve-cgi-bin.conf
      edit <IFDefine ..... > ...... </IfDefine >
      like below

    <IfDefine ENABLE_USR_LIB_CGI_BIN>
      ##ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/
      <Directory "/usr/lib/cgi-bin">
        AllowOverride None
        Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
        Require all granted
      </Directory>

      ##cgi-bin config
      ScriptAlias /cgi-bin/ /var/www/cgi-bin/
      <Directory "/var/www/cgi-bin/">
        AllowOverride None
        Options +ExecCGI
      </Directory>

    </IfDefine>

      - 7-3 <enter below command>
        chmod 755 /var/www/cgi-bin
        a2enmod cgi
        service apache2 restart

8. configure "/var/www/html/uploads" and "/var/www/html/adv_uploads" directory's permissions so apache2 can create files in those directories.
      <enter below command >
        chgrp -R www-data /var/www
        mkdir /var/www/html/uploads
        chmod 775 /var/www/html/uploads
        umask 0002 /var/www/html/uploads
        mkdir /var/www/html/adv_uploads
        chmod 775 /var/www/html/adv_uploads
        umask 0002 /var/www/html/adv_uploads

9. go to "/var/www/cgi-bin" directory
    - cd /var/www/cgi-bin
10. run the "build.sh" file in cgi-bin directory to build all cgi programs
    - sudo ./build.sh
11. configure mysql database
    - open mysql with follow command
    - mysql -u root -p
    - enter following commands to initialize database

CREATE DATABASE HTML_DB;
USE HTML_DB;
CREATE TABLE user_auth (
username varchar(40) NOT NULL default '' PRIMARY KEY,
password varchar(512) NOT NULL default '');

CREATE TABLE user_profile (
username varchar(40) NOT NULL default '' PRIMARY KEY,
fullname  varchar(40),
university varchar(40),
email varchar(320) NOT NULL UNIQUE default '',
nationality varchar(40),
FOREIGN KEY (username) REFERENCES user_auth(username));

use HTML_DB;
create table post_content (
author_id varchar(40) NOT NULL DEFAULT '',
post_id int(7) NOT NULL AUTO_INCREMENT PRIMARY KEY,
content_title varchar(50) NOT NULL,
content_text varchar(1024),
content_img varchar(40) UNIQUE,
time_written  datetime,
location varchar(20),
num_likes int(7),
FOREIGN KEY (author_id) REFERENCES user_profile(username));

use HTML_DB;
create table session(
username varchar(40) NOT NULL PRIMARY KEY DEFAULT '',
cookie varchar(20) UNIQUE,
FOREIGN KEY (username) REFERENCES user_auth(username));

use HTML_DB;
create table adv_content (
author_id varchar(40) NOT NULL DEFAULT '',
adv_id int(7) NOT NULL AUTO_INCREMENT PRIMARY KEY,
content_title varchar(50) NOT NULL,
content_link varchar(200),
content_img varchar(40) UNIQUE,
time_written  datetime,
location varchar(20),
FOREIGN KEY (author_id) REFERENCES user_profile(username));

12. sudo service apache2 start / restart
