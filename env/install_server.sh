#! /bin/sh 
#环境：安装Vm 10.0以上版本，Centos 7.0以上版本
#使用root账号进入系统，执行以下步骤

ENV_PATH=`pwd`

#安装系统编译环境
yum -y install net-tools.x86_64
yum -y install gcc-c++
yum -y install vim
yum -y install git
yum -y install bzip2
yum -y install wget.x86_64
yum -y install firewalld firewall-config

cd $ENV_PATH
#从这里下载64位eclipse for c++，放到/root 目录下面，执行下面命令
cp -f eclipse-cpp-mars-1-linux-gtk-x86_64.tar.gz /root
cd /root
tar -zxvf eclipse-cpp-mars-1-linux-gtk-x86_64.tar.gz
rm -f eclipse-cpp-mars-1-linux-gtk-x86_64.tar.gz

#安装boost
yum -y install boost  
yum -y install boost-devel

#安装openssl
#1、安装zlib
yum -y install zlib-devel
#2、源码安装openssl
rm /usr/bin/pod2man
cd $ENV_PATH
tar -zxvf openssl-1.0.1g.tar.gz
cd openssl-1.0.1g
./config shared zlib-dynamic
make && make install

#重命名原来的openssl
mv /usr/bin/openssl  /usr/bin/openssl.old
#将安装好的openssl 的openssl命令软连到/usr/bin/openssl
ln -s /usr/local/ssl/bin/openssl  /usr/bin/openssl
#将安装好的openssl 的openssl目录软连到/usr/include/openssl
ln -s /usr/local/ssl/include/openssl  /usr/include/openssl
#修改系统自带的openssl库文件，如/usr/local/lib64/libssl.so(根据机器环境而定) 软链到升级后的libssl.so
ln -s /usr/local/ssl/lib/libssl.so /usr/local/lib64/libssl.so
ln -s /usr/local/ssl/lib/libcrypto.so /usr/local/lib64/libcrypto.so
#在/etc/ld.so.conf文件中写入openssl库文件的搜索路径
echo "/usr/local/ssl/lib" >> /etc/ld.so.conf
echo "/usr/local/lib64" >> /etc/ld.so.conf
#使修改后的/etc/ld.so.conf生效 
ldconfig
cd $ENV_PATH
rm -rf openssl-1.0.1g

#安装curl
tar -zxf curl-7.50.1.tar.gz
cd curl-7.50.1
./configure --prefix=/usr/local/curl
make && make install
cp -rf /usr/local/curl/include /usr/local/include
cp -rf /usr/local/curl/lib/libcurl.so.4.4.0 /usr/local/curl/lib/libcurl.so.4 
cp -rf /usr/local/curl/lib/libcurl.so.4.4.0 /usr/local/lib64
cp -rf /usr/local/curl/lib/libcurl.so /usr/local/lib64
ldconfig
cd $ENV_PATH
rm -rf curl-7.50.1

#安装jsoncpp
tar -jxvf jsoncpp-bin-0.5.0-release.tar.bz2
cd jsoncpp-bin-0.5.0-release
cp -rf include/json /usr/local/include
cd libs/linux-gcc-4.4.7  
cp libjson_linux-gcc-4.4.7_libmt.so /usr/local/lib64
#建立软连接
cd /usr/local/lib64
rm -f libjsoncpp.so
ln -s libjson_linux-gcc-4.4.7_libmt.so libjsoncpp.so
ldconfig
cd $ENV_PATH
rm -rf jsoncpp-bin-0.5.0-release

#安装mysql
#1. 下载mysql的repo源
wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm
#2. 安装mysql-community-release-el7-5.noarch.rpm包
rpm -ivh mysql-community-release-el7-5.noarch.rpm
#安装这个包后，会获得两个mysql的yum repo源：/etc/yum.repos.d/mysql-community.repo，/etc/yum.repos.d/mysql-community-source.repo。
#3. 安装mysql
yum -y install mysql-server
chown -R mysql:mysql /var/lib/mysql
service mysqld start
#根据步骤安装就可以了，不过安装完成后，没有密码，需要重置密码。
#4. 重置密码
mysqladmin -u root password 123456
mysql -uroot -p123456 < sql/game.sql
mysql -uroot -p123456 < sql/log.sql
#登录时有可能报这样的错：ERROR 2002 (HY000): Can‘t connect to local MySQL server through socket ‘/var/lib/mysql/mysql.sock‘ (2)，原因是/var/lib/mysql的访问权限问题。下面的命令把/var/lib/mysql的拥有者改为当前用户：
#然后，重启服务：

#5. 开放3306端口
echo "-A INPUT -p tcp -m state --state NEW -m tcp --dport 3306 -j ACCEPT" >> /etc/sysconfig/iptables
#保存后重启防火墙：
systemctl restart firewalld.service
#这样从其它客户机也可以连接上mysql服务了。
cd $ENV_PATH
rm -f mysql-community-release-el7-5.noarch.rpm*

#安装mongodb（安装到/root/mongodb）
#1、安装mongodb服务器
tar -zxvf mongodb-bin-linux-x86_64-3.0.8.tar.gz
mv mongodb-bin-linux-x86_64-3.0.8 /root/mongodb
chmod -R 755 /root/mongodb/bin
mkdir /data
mkdir /data/db
mkdir /data/log

#开启mongodb服务器 
/root/mongodb/bin/mongod -dbpath=/data/db --fork --port 27017 --logpath=/data/log/work.log --logappend
#将该命令添加到/etc/rc.local,保存，即可实现开机自动启动

#2、搭建mongodb编译环境
cd $ENV_PATH
tar -zxvf mongodb-lib-3.0.8.tar.gz
cd mongodb-lib-3.0.8/
cp -rf include/mongo /usr/local/include
cp -f lib/libmongoclient.so /usr/local/lib64
cp -f lib/libmongoclient.a /usr/local/lib
ldconfig
cd $ENV_PATH
rm -rf mongodb-lib-3.0.8/

#拷贝netlib库 mysql-connector库  v8库
#进入server/doc目录下面
#1、拷贝nodelib
tar -zxvf nodelib.tar.gz
cd nodelib/
cp -rf libnodelib.so /usr/local/lib64
test -d /usr/local/include/nodelib || mkdir /usr/local/include/nodelib
cp -rf ./* /usr/local/include/nodelib

cd $ENV_PATH
rm -rf nodelib/

#2、拷贝mysql-connector
tar -zxvf mysql-connector-c++-1.1.7-linux-el7-x86-64bit.tar.gz
cd mysql-connector-c++-1.1.7-linux-el7-x86-64bit/
cp -rf include/* /usr/local/include
cp -f lib/libmysqlcppconn.so.7.1.1.7 /usr/local/lib64
ln -s /usr/local/lib64/libmysqlcppconn.so.7.1.1.7 /usr/local/lib64/libmysqlcppconn.so.7
ln -s /usr/local/lib64/libmysqlcppconn.so.7 /usr/local/lib64/libmysqlcppconn.so
cp -f lib/libmysqlcppconn-static.a /usr/local/lib
cd $ENV_PATH
rm -rf mysql-connector-c++-1.1.7-linux-el7-x86-64bit/

#3、拷贝v8运行环境
tar -zxvf v8.tar.gz
#将所有so文件拷贝到/usr/local/lib64
cd v8
cp -rf *.so /usr/local/lib64
#将所有a文件拷贝到/usr/local/lib
cp -rf *.a /usr/local/lib
#将include文件夹拷贝到/usr/local/include
cp -rf include/* /usr/local/include/include
#将natives_blob.bin snapshot_blob.bin拷贝到server同目录
cp -f natives_blob.bin $ENV_PATH
cp -f snapshot_blob.bin $ENV_PATH
#执行命令
ldconfig
cd $ENV_PATH
rm -rf v8

echo 'install compleate'

