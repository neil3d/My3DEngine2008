/*database nexus_login  create tables which is need*/

/*删除nexus_login.account账号表*/
DROP TABLE IF EXISTS `account`;

/*创建nexus_login.account账号表*/
CREATE TABLE `account` (
`id` INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT COMMENT '账号ID',
`name` CHAR(32) CHARACTER SET UTF8 NOT NULL COMMENT '账号名称',
`psd`  CHAR(16) CHARACTER SET UTF8 NOT NULL COMMENT '账号密码(md5值)',
`privilege` TINYINT(4) UNSIGNED NOT NULL DEFAULT '0' COMMENT'账号权限',
`last_login` INT DEFAULT '0' COMMENT'上次登陆的时间',
`last_ip` INT UNSIGNED ZEROFILL DEFAULT NULL COMMENT'上次登陆IP',
`login_status` TINYINT(3) UNSIGNED COMMENT'角色登录状态信息',
`joindate` DATETIME COMMENT'创建账号的时间',
`mac_0` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mac_1` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mac_2` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mac_3` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mac_4` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mac_5` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mac_6` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mac_7` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac地址',
`mibao` blob COMMENT '密保',
PRIMARY KEY (`id`),
UNIQUE KEY `id`(`id`),
UNIQUE KEY `name`(`name`)
) ENGINE=MYISAM DEFAULT CHARACTER SET UTF8;


/*创建关于nexus_login.account的缓存账号的存储过程*/
DROP PROCEDURE IF EXISTS `account_cache`;
DELIMITER ;;
--	[1]name,[2]psd,[3]privilege
--	[4]last_login,[5]last_ip,[6]login_status,
--	[7]mac_0,[8]mac_1,[9]mac_2,[10]mac_3
--	[11]mac_4,[12]mac_5,[13]mac_6,[14]mac_7
CREATE PROCEDURE `account_cache`(IN idV INT UNSIGNED)
BEGIN
	SELECT	name, psd, privilege,
			last_login, last_ip, login_status,
			mac_0, mac_1, mac_2, mac_3,
			mac_4, mac_5, mac_6, mac_7 FROM `account` WHERE id=idV;
END;;
DELIMITER ;

/*创建关于nexus_login.account的登陆标志的存储过程*/
DROP PROCEDURE IF EXISTS `fix_login_status`;
DELIMITER ;;
CREATE PROCEDURE `fix_login_status`(IN idV INT UNSIGNED, IN login_statusV TINYINT UNSIGNED)
BEGIN
	UPDATE `account` SET login_status=login_statusV WHERE `account`.`id`=idV;
END;;
DELIMITER ;

/*创建关于nexus_login.account的修改所有账号为指定状态的存储过程*/
DROP PROCEDURE IF EXISTS `fix_all_login_status`;
DELIMITER ;;
CREATE PROCEDURE `fix_all_login_status`(IN login_statusV TINYINT UNSIGNED)
BEGIN
	UPDATE `account` SET login_status=login_statusV;
END;;
DELIMITER ;


/*创建关于nexus_login.account的登出的存储过程*/
DROP PROCEDURE IF EXISTS `account_login_out`;
DELIMITER ;;
CREATE PROCEDURE `account_login_out`(IN idV INT UNSIGNED, IN last_ipV INT UNSIGNED)
BEGIN
	UPDATE `account` SET login_status=0, last_login=NOW(), last_ip=last_ipV WHERE `account`.`id`=idV;
END;;
DELIMITER ;


/*创建关于nexus_login.account的账号是否存在的存储过程*/
DROP PROCEDURE IF EXISTS `account_exist`;
DELIMITER ;;
CREATE PROCEDURE `account_exist`(IN idV INT UNSIGNED)
BEGIN
	SELECT id FROM `account` WHERE id=idV;
END;;
DELIMITER ;


/*创建关于nexus_login.account的获取账号密码存储过程*/
DROP PROCEDURE IF EXISTS `account_psd`;
DELIMITER ;;
CREATE PROCEDURE `account_psd`(IN idV INT UNSIGNED)
BEGIN
	SELECT psd FROM `account` WHERE id=idV;
END;;
DELIMITER ;

/*创建关于nexus_login.account的获取账号权限存储过程*/
DROP PROCEDURE IF EXISTS `account_privilege`;
DELIMITER ;;
CREATE PROCEDURE `account_privilege`(IN idV INT UNSIGNED)
BEGIN
	SELECT privilege FROM `account` WHERE id=idV;
END;;
DELIMITER ;



/*创建关于nexus_login.account的获取当前账号数量存储过程*/
DROP PROCEDURE IF EXISTS `account_num`;
DELIMITER ;;
CREATE PROCEDURE `account_num`()
BEGIN
	SELECT COUNT(*) FROM `account`;
END;;
DELIMITER ;


/*创建关于nexus_login.account的创建账号的存储过程*/
DROP PROCEDURE IF EXISTS `account_insert`;
DELIMITER ;;
--	[1]id,[2]name,[3]psd,[4]privilege
--	[5]last_login,[6]last_ip,[7]login_status,
--	[8]mac_0V,[9]mac_1V,[10]mac_2V,[11]mac_3V
--	[12]mac_4V,[13]mac_5V,[14]mac_6V,[15]mac_7V
CREATE PROCEDURE `account_insert`(
IN idV INT UNSIGNED, IN nameV CHAR(36), IN psdV char(16), IN privilegeV TINYINT UNSIGNED,
IN last_loginV INT UNSIGNED, IN last_ipV INT UNSIGNED, IN login_statusV TINYINT UNSIGNED,
IN mac_0V TINYINT UNSIGNED, IN mac_1V TINYINT UNSIGNED, IN mac_2V TINYINT UNSIGNED, IN mac_3V TINYINT UNSIGNED,
IN mac_4V TINYINT UNSIGNED, IN mac_5V TINYINT UNSIGNED, IN mac_6V TINYINT UNSIGNED, IN mac_7V TINYINT UNSIGNED)
BEGIN
	INSERT INTO `account` (	id, name, psd, privilege,
							last_login, last_ip, login_status,
							mac_0,mac_1,mac_2,mac_3,
							mac_4,mac_5,mac_6,mac_7,
							joindate) 
							VALUES (idV, nameV, psdV, privilegeV, 
									last_loginV, last_ipV, login_statusV,
									mac_0V, mac_1V, mac_2V, mac_3V,
									mac_4V, mac_5V, mac_6V, mac_7V,
									NOW());
END;;
DELIMITER ;





























/*删除nexus_login.account_cmp账号表*/
DROP TABLE IF EXISTS `account_cmp`;

/*创建nexus_login.account_cmp账号表，创建该表的目的是为了进行相关的测试*/
CREATE TABLE `account_cmp` (
`id` int(11) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT COMMENT '账号ID',
`name` char(36) CHARACTER SET UTF8 NOT NULL COMMENT '账号名称',
`psd`  char(32) CHARACTER SET UTF8 NOT NULL COMMENT '账号密码',
`privilege` tinyint(4) UNSIGNED NOT NULL DEFAULT '0' COMMENT'账号权限',
`last_login` DATETIME DEFAULT NULL COMMENT'上次登陆的时间',
`last_ip` int UNSIGNED ZEROFILL DEFAULT NULL COMMENT'上次登陆IP',
`login_status` tinyint(3) COMMENT'角色登录状态信息',
`joindate` DATETIME COMMENT'创建账号的时间',
`mibao` blob COMMENT '密保',
PRIMARY KEY (`id`),
UNIQUE KEY `name`(`name`)
) ENGINE=MYISAM DEFAULT CHARACTER SET UTF8;

/*创建关于nexus_login.account_cmp的检测insert的存储过程*/
DROP PROCEDURE IF EXISTS `test_insert`;
DELIMITER ;;
CREATE PROCEDURE `test_insert`(in id int, in name char(36), in privilege int)
BEGIN
	INSERT INTO `account_cmp`(id, name, privilege) VALUES (id, name, privilege);
END;;
DELIMITER ;
