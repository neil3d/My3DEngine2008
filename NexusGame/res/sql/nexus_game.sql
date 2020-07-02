/*database nexus_game  create tables which is need*/

/*删除nexus_game.item物品表*/
DROP TABLE IF EXISTS `item`;

/*创建nexus_game.item物品表(size = 18)*/
CREATE TABLE `item` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'物品唯一GUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'物品归属的主人',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'模板ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'当前堆叠数目',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'容器中的位置',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'所在容器',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'标志位信息(绑定，鉴定，锁定(非界面操作的锁定))',
`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT '对象删除时间' ,
`destroy` TINYINT(3) ZEROFILL NOT NULL DEFAULT '0' COMMENT'对象是否删除(0:不是删除状态 1:删除状态)',
PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='物品表';

/*删除nexus_game.equip装备表*/
DROP TABLE IF EXISTS `equip`;

/*创建nexus_game.equip装备表((size = 18(item) + 74(equip_extend))*/
CREATE TABLE `equip` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'装备唯一GUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'装备归属的主人',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'模板ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'当前堆叠数目',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'容器中的位置',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'所在容器',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'标志位信息(绑定，鉴定，锁定(非界面操作的锁定))',

`quality` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'品质与强化等级合在一起',
`level` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'强化等级',
`durability` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT'0' COMMENT'耐久',
`weight` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'重量',
`sharp` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'锐利',
`solid` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'坚韧',
`tenacity` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'强韧',

`stat_type0` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type1` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type2` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type3` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type4` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type5` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type6` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type7` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',

`stat_value0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value6` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value7` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',

`gem0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',

`spell0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',

`spell_rate0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',
`spell_rate1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',
`spell_rate2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',

`enchantment` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附魔ID',
`creator` BIGINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'制造者ID',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'对象删除时间',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'对象是否删除(0:不是删除状态 1:删除状态)',

PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='装备表';


/*删除nexus_game.equip装备表*/
DROP TABLE IF EXISTS `equip_cmp`;

/*创建nexus_game.equip装备表((size = 18(item) + 74(equip_extend))*/
CREATE TABLE `equip_cmp` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'物品唯一GUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'装备归属的主人',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'模板ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'当前堆叠数目',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'容器中的位置',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'所在容器',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'标志位信息(绑定，鉴定，锁定(非界面操作的锁定))',

`quality` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'品质与强化等级合在一起',
`level` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'强化等级',
`durability` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT'0' COMMENT'耐久',
`weight` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'重量',
`sharp` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'锐利',
`solid` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'坚韧',
`tenacity` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'强韧',

`stat_type0` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type1` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type2` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type3` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type4` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type5` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type6` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type7` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',

`stat_value0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value6` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value7` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',

`gem0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',

`spell0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',

`spell_rate0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',
`spell_rate1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',
`spell_rate2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',

`enchantment` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附魔ID',
`creator` BIGINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'制造者ID',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'对象删除时间',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'对象是否删除(0:不是删除状态 1:删除状态)',
PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='用于测试目的的装备表';


/*创建关于nexus_login.equip_cmp的检测insert的存储过程*/
DROP PROCEDURE IF EXISTS `test_insert_equip`;

DELIMITER ;;
CREATE PROCEDURE `test_insert_equip`(
 IN guid BIGINT UNSIGNED, IN info_id MEDIUMINT UNSIGNED, IN count SMALLINT UNSIGNED,
 IN slot SMALLINT UNSIGNED, IN bag TINYINT UNSIGNED, IN flag TINYINT UNSIGNED,
 IN quality TINYINT UNSIGNED, IN level TINYINT UNSIGNED, IN durability MEDIUMINT UNSIGNED, 
 IN weight SMALLINT UNSIGNED, IN sharp SMALLINT UNSIGNED, IN solid SMALLINT UNSIGNED, IN tenacity TINYINT UNSIGNED,
 IN stat_type0 TINYINT UNSIGNED,IN stat_type1 TINYINT UNSIGNED,IN stat_type2 TINYINT UNSIGNED,IN stat_type3 TINYINT UNSIGNED,
 IN stat_type4 TINYINT UNSIGNED,IN stat_type5 TINYINT UNSIGNED,IN stat_type6 TINYINT UNSIGNED,IN stat_type7 TINYINT UNSIGNED,
 IN stat_value0 SMALLINT UNSIGNED,IN stat_value1 SMALLINT UNSIGNED,IN stat_value2 SMALLINT UNSIGNED,IN stat_value3 SMALLINT UNSIGNED,
 IN stat_value4 SMALLINT UNSIGNED,IN stat_value5 SMALLINT UNSIGNED,IN stat_value6 SMALLINT UNSIGNED,IN stat_value7 SMALLINT UNSIGNED,
 IN gem0 SMALLINT UNSIGNED,IN gem1 SMALLINT UNSIGNED,IN gem2 SMALLINT UNSIGNED,
 IN gem3 SMALLINT UNSIGNED,IN gem4 SMALLINT UNSIGNED,IN gem5 SMALLINT UNSIGNED,
 IN spell0 SMALLINT UNSIGNED,IN spell1 SMALLINT UNSIGNED,IN spell2 SMALLINT UNSIGNED,IN spell3 SMALLINT UNSIGNED,
 IN spell_rate0 SMALLINT UNSIGNED,IN spell1_rate SMALLINT UNSIGNED,IN spell_rate2 SMALLINT UNSIGNED,
 IN enchantment SMALLINT UNSIGNED,
 IN creator SMALLINT UNSIGNED
 )
	BEGIN
		INSERT INTO `equip_cmp`(guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator) 
					VALUES (	guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator);
	END;;
DELIMITER ;



/*删除nexus_game.equip装备表*/
DROP TABLE IF EXISTS `equip_myisam`;

/*创建nexus_game.equip_innodb装备表((size = 18(item) + 74(equip_extend))*/
CREATE TABLE `equip_myisam` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'物品唯一GUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'装备归属的主人',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'模板ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'当前堆叠数目',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'容器中的位置',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'所在容器',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'标志位信息(绑定，鉴定，锁定(非界面操作的锁定))',

`quality` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'品质与强化等级合在一起',
`level` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'强化等级',
`durability` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT'0' COMMENT'耐久',
`weight` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'重量',
`sharp` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'锐利',
`solid` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'坚韧',
`tenacity` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'强韧',

`stat_type0` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type1` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type2` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type3` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type4` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type5` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type6` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',
`stat_type7` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性类型',

`stat_value0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value6` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',
`stat_value7` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加属性数值',

`gem0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',
`gem5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'镶嵌的宝石',

`spell0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',
`spell3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加技能',

`spell_rate0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',
`spell_rate1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',
`spell_rate2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附加被动技能触发几率',

`enchantment` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'附魔ID',
`creator` BIGINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'制造者ID',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'对象删除时间',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'对象是否删除(0:不是删除状态 1:删除状态)',
PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=MyISAM DEFAULT CHARACTER SET UTF8 COMMENT='通过MyISAM引擎存储，用于和Innodb比较';


/*创建关于nexus_login.equip_cmp的检测insert的存储过程*/
DROP PROCEDURE IF EXISTS `test_insert_equip_myisam`;

DELIMITER ;;
CREATE PROCEDURE `test_insert_equip_myisam`(
 IN guid BIGINT UNSIGNED, IN info_id MEDIUMINT UNSIGNED, IN count SMALLINT UNSIGNED,
 IN slot SMALLINT UNSIGNED, IN bag TINYINT UNSIGNED, IN flag TINYINT UNSIGNED,
 IN quality TINYINT UNSIGNED, IN level TINYINT UNSIGNED, IN durability MEDIUMINT UNSIGNED, 
 IN weight SMALLINT UNSIGNED, IN sharp SMALLINT UNSIGNED, IN solid SMALLINT UNSIGNED, IN tenacity TINYINT UNSIGNED,
 IN stat_type0 TINYINT UNSIGNED,IN stat_type1 TINYINT UNSIGNED,IN stat_type2 TINYINT UNSIGNED,IN stat_type3 TINYINT UNSIGNED,
 IN stat_type4 TINYINT UNSIGNED,IN stat_type5 TINYINT UNSIGNED,IN stat_type6 TINYINT UNSIGNED,IN stat_type7 TINYINT UNSIGNED,
 IN stat_value0 SMALLINT UNSIGNED,IN stat_value1 SMALLINT UNSIGNED,IN stat_value2 SMALLINT UNSIGNED,IN stat_value3 SMALLINT UNSIGNED,
 IN stat_value4 SMALLINT UNSIGNED,IN stat_value5 SMALLINT UNSIGNED,in stat_value6 SMALLINT UNSIGNED,IN stat_value7 SMALLINT UNSIGNED,
 IN gem0 SMALLINT UNSIGNED,IN gem1 SMALLINT UNSIGNED,IN gem2 SMALLINT UNSIGNED,
 IN gem3 SMALLINT UNSIGNED,IN gem4 SMALLINT UNSIGNED,IN gem5 SMALLINT UNSIGNED,
 IN spell0 SMALLINT UNSIGNED,IN spell1 SMALLINT UNSIGNED,IN spell2 SMALLINT UNSIGNED,IN spell3 SMALLINT UNSIGNED,
 IN spell_rate0 SMALLINT UNSIGNED,IN spell1_rate SMALLINT UNSIGNED,IN spell_rate2 SMALLINT UNSIGNED,
 IN enchantment SMALLINT UNSIGNED,
 IN creator SMALLINT UNSIGNED
 )
	BEGIN
		INSERT INTO `equip_myisam`(guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator) 
					VALUES (	guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator);
	END;;
DELIMITER ;


/*删除nexus_game.character角色信息表*/
DROP TABLE IF EXISTS `character`;

/*创建nexus_game.character角色信息表*/
CREATE TABLE `character`(
`role_id` INT(11) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '角色ID',
`account_id` INT(11) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '账号ID',
`name` VARCHAR(32) NOT NULL DEFAULT '' COMMENT '角色名称',
`sex` TINYINT(3) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '0:女 1:男',
`race` TINYINT(3) unsigned NOT NULL DEFAULT '0' COMMENT '种族',
`class_type` TINYINT(3) unsigned NOT NULL DEFAULT '0'COMMENT '职业',

`map_id` SMALLINT(10) NOT NULL DEFAULT '-1' COMMENT '角色所在地图ID',
`x` FLOAT(10) ZEROFILL NOT NULL COMMENT 'X坐标',
`y` FLOAT(10) ZEROFILL NOT NULL COMMENT 'y坐标',
`z` FLOAT(10) ZEROFILL NOT NULL  COMMENT 'z坐标',
`orient` FLOAT(10) ZEROFILL NOT NULL  COMMENT '朝向',

`hp` MEDIUMINT(8) ZEROFILL NOT NULL DEFAULT '0' COMMENT '血量',
`mp` MEDIUMINT(8) ZEROFILL NOT NULL DEFAULT '0' COMMENT '真气量',
`level` TINYINT(3) NOT NULL DEFAULT '1' COMMENT '当前等级',
`cur_exp` INT(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前拥有的经验值',

`create_time` INT(10) NOT NULL COMMENT '角色创建时间',
`login_time` INT(10) NOT NULL DEFAULT '0' COMMENT '上线时间',
`logout_time` INT(10) NOT NULL DEFAULT '0' COMMENT '下线时间',
`online_time` INT(10) NOT NULL DEFAULT '0' COMMENT '总在线时间(单位:秒)',
`cur_online_time` INT(10) NOT NULL DEFAULT '0' COMMENT '当前在线时间(单位:秒)',

`remove_flag` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '是否处于删除状态(该状态玩家不会被读入游戏世界)',
`remove_time` INT(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '删除时间',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'对象删除时间',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'对象是否删除(0:不是删除状态 1:删除状态)',

PRIMARY KEY (`role_id`),
UNIQUE KEY `role_id`(`role_id`),
INDEX `account_id`(`account_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT '角色信息表(用于游戏世界)';


/*创建关于获取nexus_game.character的role_id最大值的存储过程*/
DROP PROCEDURE IF EXISTS `get_role_max`;

DELIMITER ;;
CREATE PROCEDURE `get_role_max`()
	BEGIN
		SELECT MAX(role_id) FROM `character`;
	END;; 
DELIMITER ;

/*创建关于nexus_game.character的account_id的最大值的存储过程*/
DROP PROCEDURE IF EXISTS `get_account_max`;

DELIMITER ;;
CREATE PROCEDURE `get_account_max`()
	BEGIN
		SELECT MAX(account_id) FROM `character`;
	END;; 
DELIMITER ;

/*创建关于nexus_game.character的character_role的数据缓存的存储过程*/
DROP PROCEDURE IF EXISTS `character_role`;

DELIMITER ;;
CREATE PROCEDURE `character_role`()
	BEGIN
		SELECT account_id, role_id FROM `character` WHERE `character`.`destroy`=0;
	END;; 
DELIMITER ;


/*创建关于nexus_game.character的数据缓存的存储过程*/
DROP PROCEDURE IF EXISTS `character_cache`;

DELIMITER ;;
CREATE PROCEDURE `character_cache`(IN role_idV INT UNSIGNED)
	BEGIN
		-- SELECT	[0]account_id,[1]role_id,[2]name,
		--			[3]sex,[4]race,[5]class_type,[6]level,[7]cur_exp,[8]hp,[9]mp,
		--			[10]map_id,[11]x,[12]y,[13]z,[14]orient,
		--			[15]create_time,[16]login_time,[17]logout_time,[18]online_time FROM `character` WHERE `character`.`role_id`=role_idV and `character`.`destroy`=0;
		SELECT  account_id,role_id,name,
				sex,race,class_type,level,cur_exp,hp,mp,
				map_id,x,y,z,orient,
				create_time,login_time,logout_time,online_time FROM `character` WHERE `character`.`role_id`=role_idV and `character`.`destroy`=0;
	END;;
DELIMITER ;

/*创建关于nexus_game.character的更新存储过程*/
DROP PROCEDURE IF EXISTS `character_update`;

DELIMITER ;;
--	[1]role_id,[2]level,[3]cur_exp,[4]hp,[5]mp,
--	[6]map_id,[7]x,[8]y,[9]z,[10]orient,
--	[11]login_time,[12]logout_time,[13]online_time
CREATE PROCEDURE `character_update`(
IN role_idV INT UNSIGNED,
IN levelV TINYINT UNSIGNED, IN cur_expV INT UNSIGNED,IN hpV MEDIUMINT UNSIGNED, IN mpV MEDIUMINT UNSIGNED,
IN map_idV SMALLINT UNSIGNED,IN xV FLOAT, IN yV FLOAT,IN zV FLOAT, IN orientV FLOAT,
IN login_timeV INT, IN logout_timeV INT, IN online_timeV INT)
	BEGIN
		UPDATE `character` SET level=levelV,cur_exp=cur_expV,hp=hpV,mp=mpV,
						map_id=map_idV,x=xV,y=yV,z=zV,orient=orientV,
						login_time=login_timeV,logout_time=logout_timeV,online_time=online_timeV
				WHERE `character`.`role_id` = role_idV;
	END;;
DELIMITER ;

/*创建关于nexus_game.character的增加存储过程*/
DROP PROCEDURE IF EXISTS `character_insert`;

DELIMITER ;;
--	[1]account_id,[2]role_id,
--	[3]name,[4]sex,[5]race,[6]class_type,
--	[7]level,[8]cur_exp,[9]hp,[10]mp,
--	[11]map_id,[12]x,[13]y,[14]z,[15]orient,
--	[16]create_time, [17]login_time,[18]logout_time,[19]online_time
CREATE PROCEDURE `character_insert`(
IN account_idV INT UNSIGNED,IN role_idV INT UNSIGNED, nameV VARCHAR(32),
IN sexV TINYINT UNSIGNED, raceV TINYINT UNSIGNED, class_typeV TINYINT UNSIGNED,
IN levelV TINYINT UNSIGNED, IN cur_expV INT UNSIGNED,IN hpV MEDIUMINT UNSIGNED, IN mpV MEDIUMINT UNSIGNED,
IN map_idV SMALLINT UNSIGNED,IN xV FLOAT, IN yV FLOAT,in zV FLOAT, IN orientV FLOAT,
IN create_timeV INT, IN login_timeV INT, IN logout_timeV INT, IN online_timeV INT)
	BEGIN
		INSERT INTO `character` (account_id, role_id, name, sex, race, class_type,
				level,cur_exp,hp,mp,
				map_id,x,y,z,orient,
				create_time,login_time,logout_time,online_time)
				VALUES (account_idV, role_idV, nameV, sexV, raceV, class_typeV,
				levelV,cur_expV,hpV,mpV,
				map_idV,xV,yV,xV,orientV,
				create_timeV,login_timeV,logout_timeV,online_timeV);
	END;;
DELIMITER ;

/*创建关于nexus_game.character的移除角色的存储过程*/
DROP PROCEDURE IF EXISTS `character_delete`;

DELIMITER ;;
CREATE PROCEDURE `character_delete`(
IN role_idV INT UNSIGNED)
	BEGIN
		UPDATE `character` SET destroy_time=NOW(), destroy=1 WHERE `character`.`role_id`=role_idV;
		UPDATE `item` SET destroy_time=NOW(), destroy=1 WHERE `item`.`owner_id`=role_idV and `item`.`destroy`=0;
		UPDATE `equip` SET destroy_time=NOW(), destroy=1 WHERE `equip`.`owner_id`=role_idV and `equip`.`destroy`=0;
		UPDATE `skill` SET destroy_time=NOW(), destroy=1 WHERE `skill`.`role`=role_idV and `skill`.`destroy`=0;
	END;;
DELIMITER ;


/*创建关于nexus_game.item的数据缓存的存储过程*/
DROP PROCEDURE IF EXISTS `item_cache`;

DELIMITER ;;
CREATE PROCEDURE `item_cache`(IN owner_idV INT UNSIGNED)
	BEGIN
		-- SELECT [0]guid,[1]info_id,[2]count,[3]slot,[4]bag,[5]flag FROM `item` WHERE `item`.`owner_id`=owner_idV and `item`.`destroy`=0;
		SELECT guid,info_id,count,slot,bag,flag FROM `item` WHERE `item`.`owner_id`=owner_idV and `item`.`destroy`=0;
	END;;
DELIMITER ;

/*创建关于nexus_game.item的更新的存储过程*/
DROP PROCEDURE IF EXISTS `item_update`;

DELIMITER ;;
--	[1]guid,[2]owner_id,
--	[3]info_id,[4]count,
--	[5]slot,[6]bag,[7]flag
CREATE PROCEDURE `item_update`(
 IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
 IN info_idV MEDIUMINT UNSIGNED, IN countV SMALLINT UNSIGNED,
 IN slotV SMALLINT UNSIGNED, IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED
 )
	BEGIN
		UPDATE `item` SET info_id=info_idV,count=countV,slot=slotV,bag=bagV,flag=flagV WHERE `item`.`guid`=guidV;
	END;;
DELIMITER ;

/*创建关于nexus_game.item的增加的存储过程*/
DROP PROCEDURE IF EXISTS `item_insert`;

DELIMITER ;;
--	[1]guid,[2]owner_id,
--	[3]info_id,[4]count,
--	[5]slot,[6]bag,[7]flag
CREATE PROCEDURE `item_insert`(
IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
IN info_idV INT UNSIGNED, IN countV SMALLINT UNSIGNED, IN slotV SMALLINT UNSIGNED,
IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED
)
	BEGIN
		INSERT INTO `item` (guid, owner_id, info_id, count, slot, bag, flag)
						VALUES (guidV,owner_idV,info_idV, countV, slotV, bagV, flagV);
	END;;
DELIMITER ;

/*创建关于nexus_game.item的删除的存储过程*/
DROP PROCEDURE IF EXISTS `item_delete`;

DELIMITER ;;
CREATE PROCEDURE `item_delete`(IN guidV BIGINT UNSIGNED)
	BEGIN
		UPDATE `item` SET destroy_time=NOW(), destroy=1 WHERE `item`.`guid`=guidV;
	END;;
DELIMITER ;


/*创建关于nexus_game.equip的数据缓存的存储过程*/
DROP PROCEDURE IF EXISTS `equip_cache`;

DELIMITER ;;
CREATE PROCEDURE `equip_cache`(IN owner_idV INT UNSIGNED)
	BEGIN
		-- SELECT	[0]guid,[1]info_id,[2]count,[3]slot,[4]bag,[5]flag,
		--			[6]quality,[7]level,[8]durability,[9]weight,[10]sharp,[11]solid,[12]tenacity,
		--			[13]stat_type0,[14]stat_type1,[15]stat_type2,[16]stat_type3,
		--			[17]stat_type4,[18]stat_type5,[19]stat_type6,[20]stat_type7,
		--			[21]stat_value0,[22]stat_value1,[23]stat_value2,[24]stat_value3,
		--			[25]stat_value4,[26]stat_value5,[27]stat_value6,[28]stat_value7,
		--			[29]gem0,[30]gem1,[31]gem2,[32]gem3,[33]gem4,[34]gem5,
		--			[35]spell0,[36]spell1,[37]spell2,[38]spell3,
		--			[39]spell_rate0,[40]spell_rate1,[41]spell_rate2,
		--			[42]enchantment,[43]creator FROM `equip` WHERE `equip`.`owner_id`=owner_idV and `equip`.`destroy`=0;
		SELECT		guid,info_id,count,slot,bag,flag,
					quality,level,durability,weight,sharp,solid,tenacity,
					stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
					stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
					gem0,gem1,gem2,gem3,gem4,gem5,
					spell0,spell1,spell2,spell3,
					spell_rate0,spell_rate1,spell_rate2,
					enchantment,creator FROM `equip` WHERE `equip`.`owner_id`=owner_idV and `equip`.`destroy`=0;
	END;;
DELIMITER ;

/*创建关于nexus_game.equip的更新存储过程*/
DROP PROCEDURE IF EXISTS `equip_update`;

DELIMITER ;;
-- 	[1]guid,[2]owner_id
--  [3]info_id,[4]count,
--  [5]slot,[6]bag,[7]flag,
--	[8]quality,[9]level,[10]durability,
--  [11]weight,[12]sharp,[13]solid,[14]tenacity,
--	[15]stat_type0,[16]stat_type1,[17]stat_type2,[18]stat_type3,
--	[19]stat_type4,[20]stat_type5,[21]stat_type6,[22]stat_type7,
--	[23]stat_value0,[24]stat_value1,[25]stat_value2,[26]stat_value3,
--	[27]stat_value4,[28]stat_value5,[29]stat_value6,[30]stat_value7,
--	[31]gem0,[32]gem1,[33]gem2,
--  [34]gem3,[35]gem4,[36]gem5,
--	[37]spell0,[38]spell1,[39]spell2,[40]spell3,
--	[41]spell_rate0,[42]spell_rate1,[43]spell_rate2,
--	[44]enchantment,[45]creator
CREATE PROCEDURE `equip_update`(
 IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
 IN info_idV MEDIUMINT UNSIGNED, IN countV SMALLINT UNSIGNED,
 IN slotV SMALLINT UNSIGNED, IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED,
 IN qualityV TINYINT UNSIGNED, IN levelV TINYINT UNSIGNED, IN durabilityV MEDIUMINT UNSIGNED, 
 IN weightV SMALLINT UNSIGNED, IN sharpV SMALLINT UNSIGNED, IN solidV SMALLINT UNSIGNED, IN tenacityV TINYINT UNSIGNED,
 IN stat_type0V TINYINT UNSIGNED,IN stat_type1V TINYINT UNSIGNED,IN stat_type2V TINYINT UNSIGNED,IN stat_type3V TINYINT UNSIGNED,
 IN stat_type4V TINYINT UNSIGNED,IN stat_type5V TINYINT UNSIGNED,IN stat_type6V TINYINT UNSIGNED,IN stat_type7V TINYINT UNSIGNED,
 IN stat_value0V SMALLINT UNSIGNED,IN stat_value1V SMALLINT UNSIGNED,IN stat_value2V SMALLINT UNSIGNED,IN stat_value3V SMALLINT UNSIGNED,
 IN stat_value4V SMALLINT UNSIGNED,IN stat_value5V SMALLINT UNSIGNED,IN stat_value6V SMALLINT UNSIGNED,IN stat_value7V SMALLINT UNSIGNED,
 IN gem0V SMALLINT UNSIGNED,IN gem1V SMALLINT UNSIGNED,IN gem2V SMALLINT UNSIGNED,
 IN gem3V SMALLINT UNSIGNED,IN gem4V SMALLINT UNSIGNED,IN gem5V SMALLINT UNSIGNED,
 IN spell0V SMALLINT UNSIGNED,IN spell1V SMALLINT UNSIGNED,IN spell2V SMALLINT UNSIGNED,IN spell3V SMALLINT UNSIGNED,
 IN spell_rate0V SMALLINT UNSIGNED,IN spell_rate1V SMALLINT UNSIGNED,IN spell_rate2V SMALLINT UNSIGNED,
 IN enchantmentV SMALLINT UNSIGNED,IN creatorV SMALLINT UNSIGNED
 )
	BEGIN
		UPDATE `equip` SET		info_id=info_idV,count=countV,slot=slotV,bag=bagV,flag=flagV,
								quality=qualityV,level=levelV,durability=durabilityV,
								weight=weightV,sharp=sharpV,solid=solidV,tenacity=tenacityV,
								stat_type0=stat_type0V,stat_type1=stat_type1V,stat_type2=stat_type2V,stat_type3=stat_type3V,
								stat_type4=stat_type4V,stat_type5=stat_type5V,stat_type6=stat_type6V,stat_type7=stat_type7V,
								stat_value0=stat_value0V,stat_value1=stat_value1V,stat_value2=stat_value2V,stat_value3=stat_value3V,
								stat_value4=stat_value4V,stat_value5=stat_value5V,stat_value6=stat_value6V,stat_value7=stat_value7V,
								gem0=gem0V,gem1=gem1V,gem2=gem2V,
								gem3=gem3V,gem4=gem4V,gem5=gem5V,
								spell0=spell0V,spell1=spell1V,
								spell2=spell2V,spell3=spell3V,
								spell_rate0=spell_rate0V,spell_rate1=spell_rate1V,spell_rate2=spell_rate2V,
								enchantment=enchantmentV,creator=creatorV WHERE `equip`.`guid`=guidV;
	END;;
DELIMITER ;


/*创建关于nexus_game.equip的增加存储过程*/
DROP PROCEDURE IF EXISTS `equip_insert`;

DELIMITER ;;
-- 	[1]guid,[2]owner_id
--  [3]info_id,[4]count,
--  [5]slot,[6]bag,[7]flag,
--	[8]quality,[9]level,[10]durability,
--  [11]weight,[12]sharp,[13]solid,[14]tenacity,
--	[15]stat_type0,[16]stat_type1,[17]stat_type2,[18]stat_type3,
--	[19]stat_type4,[20]stat_type5,[21]stat_type6,[22]stat_type7,
--	[23]stat_value0,[24]stat_value1,[25]stat_value2,[26]stat_value3,
--	[27]stat_value4,[28]stat_value5,[29]stat_value6,[30]stat_value7,
--	[31]gem0,[32]gem1,[33]gem2,
--  [34]gem3,[35]gem4,[36]gem5,
--	[37]spell0,[38]spell1,[39]spell2,[40]spell3,
--	[41]spell_rate0,[42]spell_rate1,[43]spell_rate2,
--	[44]enchantment,[45]creator
CREATE PROCEDURE `equip_insert`(
 IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
 IN info_idV MEDIUMINT UNSIGNED, IN countV SMALLINT UNSIGNED,
 IN slotV SMALLINT UNSIGNED, IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED,
 IN qualityV TINYINT UNSIGNED, IN levelV TINYINT UNSIGNED, IN durabilityV MEDIUMINT UNSIGNED, 
 IN weightV SMALLINT UNSIGNED, IN sharpV SMALLINT UNSIGNED, IN solidV SMALLINT UNSIGNED, IN tenacityV TINYINT UNSIGNED,
 IN stat_type0V TINYINT UNSIGNED,IN stat_type1V TINYINT UNSIGNED,IN stat_type2V TINYINT UNSIGNED,IN stat_type3V TINYINT UNSIGNED,
 IN stat_type4V TINYINT UNSIGNED,IN stat_type5V TINYINT UNSIGNED,IN stat_type6V TINYINT UNSIGNED,IN stat_type7V TINYINT UNSIGNED,
 IN stat_value0V SMALLINT UNSIGNED,IN stat_value1V SMALLINT UNSIGNED,IN stat_value2V SMALLINT UNSIGNED,IN stat_value3V SMALLINT UNSIGNED,
 IN stat_value4V SMALLINT UNSIGNED,IN stat_value5V SMALLINT UNSIGNED,IN stat_value6V SMALLINT UNSIGNED,IN stat_value7V SMALLINT UNSIGNED,
 IN gem0V SMALLINT UNSIGNED,IN gem1V SMALLINT UNSIGNED,in gem2V SMALLINT UNSIGNED,
 IN gem3V SMALLINT UNSIGNED,IN gem4V SMALLINT UNSIGNED,in gem5V SMALLINT UNSIGNED,
 IN spell0V SMALLINT UNSIGNED,IN spell1V SMALLINT UNSIGNED,in spell2V SMALLINT UNSIGNED,IN spell3V SMALLINT UNSIGNED,
 IN spell_rate0V SMALLINT UNSIGNED,IN spell_rate1V SMALLINT UNSIGNED,IN spell_rate2V SMALLINT UNSIGNED,
 IN enchantmentV SMALLINT UNSIGNED,IN creatorV SMALLINT UNSIGNED
 )
	BEGIN
		INSERT INTO `equip` ( guid,owner_id, info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,
								spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator )
								VALUES ( guidV,owner_idV,info_idV,countV,slotV,bagV,flagV,
								qualityV,levelV,durabilityV,weightV,sharpV,solidV,tenacityV,
								stat_type0V,stat_type1V,stat_type2V,stat_type3V,stat_type4V,stat_type5V,stat_type6V,stat_type7V,
								stat_value0V,stat_value1V,stat_value2V,stat_value3V,stat_value4V,stat_value5V,stat_value6V,stat_value7V,
								gem0V,gem1V,gem2V,gem3V,gem4V,gem5V,
								spell0V,spell1V,spell2V,spell3V,
								spell_rate0V,spell_rate1V,spell_rate2V,
								enchantmentV,creatorV );
	END;;
DELIMITER ;


/*创建关于nexus_game.equip的删除的存储过程*/
DROP PROCEDURE IF EXISTS `equip_delete`;

DELIMITER ;;
CREATE PROCEDURE `equip_delete`(IN guidV BIGINT UNSIGNED)
	BEGIN
		UPDATE `equip` SET destroy_time=NOW(), destroy=1 WHERE `equip`.`guid`=guidV;
	END;;
DELIMITER ;


/*删除nexus_game.skill角色技能表*/
DROP TABLE IF EXISTS `skill`;

/*创建nexus_game.skill角色技能表*/

CREATE TABLE `skill` (
`role` INT(10) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '角色ID',
`id` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '技能模板ID',
`cd` MEDIUMINT(11) UNSIGNED NOT NULL COMMENT '技能冷却时间',
`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT '对象删除时间' ,
`destroy` TINYINT(3) ZEROFILL NOT NULL DEFAULT '0' COMMENT'对象是否删除(0:不是删除状态 1:删除状态)',
PRIMARY KEY (`role`, `id`),
UNIQUE KEY `role_id`(`role`,`id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='角色技能表';

/*创建关于nexus_game.skill的数据缓存的存储过程*/
DROP PROCEDURE IF EXISTS `skill_cache`;

DELIMITER ;;
CREATE PROCEDURE `skill_cache`(IN roleV INT UNSIGNED)
	BEGIN
		-- SELECT [0]id,[1]cd FROM `skill` WHERE `skill`.`role`=roleV and `skill`.`destroy`=0;
		SELECT id,cd FROM `skill` WHERE `skill`.`role`=roleV and `skill`.`destroy`=0;
	END;;
DELIMITER ;

/*创建关于nexus_game.skill的更新存储过程*/
DROP PROCEDURE IF EXISTS `skill_update`;

DELIMITER ;;
CREATE PROCEDURE `skill_update`(IN roleV INT UNSIGNED, IN idV INT UNSIGNED, IN cdV MEDIUMINT UNSIGNED)
	BEGIN
		UPDATE `skill` SET cd=cdV WHERE `skill`.`role`=roleV and `skill`.`id`=idV;
	END;;
DELIMITER ;

/*创建关于nexus_game.skill的增加的存储过程*/
DROP PROCEDURE IF EXISTS `skill_insert`;

DELIMITER ;;
CREATE PROCEDURE `skill_insert`(IN roleV INT UNSIGNED,IN idV INT UNSIGNED,IN cdV MEDIUMINT UNSIGNED)
	BEGIN
		DECLARE num INT UNSIGNED DEFAULT 0; 
		SELECT COUNT(*) INTO num FROM `skill` WHERE  `skill`.`role`=roleV and `skill`.`id`=idV and `skill`.`destroy`=1;
		IF num=1 THEN
			UPDATE `skill` SET destroy=0 WHERE `skill`.`role`=roleV and `skill`.`id`=idV;
		ELSE
			INSERT INTO `skill` (role, id, cd) VALUES (roleV, idV, cdV);
		END IF;
	END;;
DELIMITER ;


/*创建关于nexus_game.skill的删除的存储过程*/
DROP PROCEDURE IF EXISTS `skill_delete`;

DELIMITER ;;
CREATE PROCEDURE `skill_delete`(IN roleV INT UNSIGNED, IN idV INT UNSIGNED)
	BEGIN
		UPDATE `skill` SET destroy_time=NOW(), destroy=1 WHERE `skill`.`role`=roleV and `skill`.`id`=idV;
	END;;
DELIMITER ;



/*删除nexus_game.buff角色BUFF表*/
DROP TABLE IF EXISTS `buff`;

/*创建nexus_game.buff角色BUFF表*/
CREATE TABLE `buff` (
`dest` INT(10) unsigned NOT NULL COMMENT '作用目标ID',
`src` INT(10) unsigned NOT NULL COMMENT '产生源ID',
`skill_id` INT(10) unsigned NOT NULL COMMENT '触发该BUFF的技能ID',
`buff_id` INT(10) unsigned NOT NULL COMMENT 'Buff ID',
`levels` TINYINT(3) NOT NULL COMMENT '当前叠加次数',
`leave_time` FLOAT(4)ZEROFILL DEFAULT '0' COMMENT '剩余时间',
`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT '对象删除时间' ,
`destroy` TINYINT(3) ZEROFILL NOT NULL DEFAULT '0' COMMENT'对象是否删除(0:不是删除状态 1:删除状态)',
PRIMARY KEY  (`dest`, `buff_id`)
)ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='角色buff表';



