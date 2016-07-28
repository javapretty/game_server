CREATE DATABASE IF NOT EXISTS `log` DEFAULT CHARACTER SET utf8;

USE `log`;

DROP TABLE IF EXISTS `logout`;
CREATE TABLE `logout` (
  role_id bigint(20) NOT NULL,
  role_name varchar(120) NOT NULL default '', 
  account varchar(120) NOT NULL default '',
  level int(11) NOT NULL default '0',
  client_ip varchar(120) NOT NULL default '', 
  login_time int(11) NOT NULL default '0',
  logout_time int(11) NOT NULL default '0'
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

