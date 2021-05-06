#!/bin/sh

cd /var/www/cgi-bin/
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o login.cgi login.cpp cookie.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o logout.cgi logout.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o create_post.cgi create_post.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o index.cgi index.cpp cookie.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o register.cgi register.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o edit_post.cgi edit_post.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o delete_post.cgi delete_post.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o settings.cgi settings.cpp cookie.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o create_adv.cgi create_adv.cpp -lmysqlcppconn -lcgicc
g++ -w -z execstack -fno-stack-protector -z norelro -g -O0 -o profile.cgi profile.cpp cookie.cpp -lmysqlcppconn -lcgicc
