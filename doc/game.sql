CREATE DATABASE IF NOT EXISTS `game` DEFAULT CHARACTER SET utf8;

USE `game`;

DROP TABLE IF EXISTS `global`;
CREATE TABLE `global` (
  type varchar(120) not null,
  value bigint(20) not null,
  PRIMARY KEY (`type`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `role`;
CREATE TABLE `role` (
  role_id bigint(20),
  role_name varchar(120), 
  account varchar(120),
  client_ip varchar(120),
  agent_num int(10),
  server_num int(10),
  level int(10),
  exp int(10),
  gender int(10),
  career int(10),
  create_time int(11),
  last_sign_in_time int(11),
  last_sign_out_time int(11),
  vitality int(10),
  buy_vitality_times int(10),
  vip_level int(10),
  vip_exp int(10),
  charge_gold int(10),
  guild_id int(10),
  guild_name varchar(120),
  PRIMARY KEY (role_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;