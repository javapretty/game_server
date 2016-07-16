CREATE DATABASE IF NOT EXISTS `log` DEFAULT CHARACTER SET utf8;

USE `log`;

DROP TABLE IF EXISTS `test`;
CREATE TABLE `test` (
  `role_id` int(11),
  `role_name` varchar(120)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `loginout`;
CREATE TABLE `loginout` (
  role_id int(11) ,
  role_name varchar(120), 
  account varchar(120),
  level int(10),
  client_ip varchar(120), 
  login_time int(11),
  logout_time int(11), 
  online_time int(11) 
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

