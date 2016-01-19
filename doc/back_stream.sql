CREATE DATABASE IF NOT EXISTS `back_stream` DEFAULT CHARACTER SET utf8;

USE `back_stream`;

DROP TABLE IF EXISTS `log_test`;
CREATE TABLE `log_test` (
  `role_id` int(11),
  `role_name` varchar(120)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `loginout_stream`;
CREATE TABLE `loginout_stream` (
  role_id int(11) ,
  role_name varchar(120), 
  account varchar(120),
  level int(10),
  client_ip varchar(120), 
  login_time int(11),
  logout_time int(11), 
  online_time int(11) 
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

