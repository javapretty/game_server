CREATE DATABASE IF NOT EXISTS `game` DEFAULT CHARACTER SET utf8;

USE `game`;

DROP TABLE IF EXISTS `global`;
CREATE TABLE `global` (
  type varchar(120) NOT NULL,
  value bigint(20) NOT NULL default '0',
  PRIMARY KEY (`type`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `role`;
CREATE TABLE `role` (
  role_id bigint(20) NOT NULL auto_increment,
  role_name varchar(120) NOT NULL default '', 
  account varchar(120) NOT NULL default '',
  client_ip varchar(120) NOT NULL default '',
  agent_num int(11) NOT NULL default '0',
  server_num int(11) NOT NULL default '0',
  level int(11) NOT NULL default '0',
  exp int(11) NOT NULL default '0',
  gender int(11) NOT NULL default '0',
  career int(11) NOT NULL default '0',
  create_time int(11) NOT NULL default '0',
  login_time int(11) NOT NULL default '0',
  logout_time int(11) NOT NULL default '0',
  vitality int(11) NOT NULL default '0',
  buy_vitality_times int(11) NOT NULL default '0',
  vip_level int(11) NOT NULL default '0',
  vip_exp int(11) NOT NULL default '0',
  charge_gold int(11) NOT NULL default '0',
  guild_id int(11) NOT NULL default '0',
  guild_name varchar(120) NOT NULL default '',
  PRIMARY KEY (role_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `hero`;
CREATE TABLE `hero` (
  role_id bigint(20) NOT NULL auto_increment,
  hero_map blob NOT NULL, 
  PRIMARY KEY (role_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `bag`;
CREATE TABLE `bag` (
  role_id bigint(20) NOT NULL auto_increment,
  copper int(11) NOT NULL default '0',
  gold int(11) NOT NULL default '0',
  item_map blob NOT NULL, 
  PRIMARY KEY (role_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `mail`;
CREATE TABLE `mail` (
  role_id bigint(20) NOT NULL auto_increment,
  total_count int(11) NOT NULL default '0',
  mail_map blob NOT NULL, 
  PRIMARY KEY (role_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `shop`;
CREATE TABLE `shop` (
  role_id bigint(20) NOT NULL auto_increment,
  shop_map blob NOT NULL, 
  PRIMARY KEY (role_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `guild`;
CREATE TABLE `guild` (
  guild_id bigint(20) NOT NULL auto_increment,
  guild_name varchar(120) NOT NULL default '',
  chief_id bigint(20) NOT NULL default '0',
  is_change tinyint(2) NOT NULL default '0',
  applicant_list blob NOT NULL,
  member_list blob NOT NULL, 
  PRIMARY KEY (guild_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `offline`;
CREATE TABLE `offline` (
  role_id bigint(20) NOT NULL auto_increment,
  guild_id bigint(20) NOT NULL default '0',
  guild_name varchar(120) NOT NULL default '',
  flag int(11), 
  PRIMARY KEY (role_id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `rank`;
CREATE TABLE `rank` (
  rank_type bigint(20) NOT NULL auto_increment,
  min_value int(11) NOT NULL default '0',
  min_role_id bigint(20) NOT NULL default '0',
  member_map blob NOT NULL, 
  PRIMARY KEY (rank_type)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;