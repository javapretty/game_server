CREATE DATABASE IF NOT EXISTS `account` DEFAULT CHARACTER SET utf8;
USE `account`;

DROP TABLE IF EXISTS `account_info`;
CREATE TABLE `account_info` (
  account varchar(120) not null,
  password varchar(120) not null,
  PRIMARY KEY (`account`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;


