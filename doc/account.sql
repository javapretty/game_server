CREATE DATABASE IF NOT EXISTS `account` DEFAULT CHARACTER SET utf8;
USE `account`;

DROP TABLE IF EXISTS `account_info`;
CREATE TABLE `account_info` (
  account varchar(120) NOT NULL,
  password varchar(120) NOT NULL default '',
  PRIMARY KEY (`account`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;


