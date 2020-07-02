/*database nexus_game 的基础测试过程 和 基本维护过程*/


/*创建关于nexus_game.character的真正移除角色的存储过程，维护阶段使用*/
DROP PROCEDURE IF EXISTS `character_delete_real`;

DELIMITER ;;
CREATE PROCEDURE `character_delete_real`()
	BEGIN
		DELETE FROM `character` WHERE `character`.`destroy`=1;
	END;;
DELIMITER ;


/*创建关于nexus_game.item的真实删除的存储过程，维护阶段使用*/
DROP PROCEDURE IF EXISTS `item_delete_real`;

DELIMITER ;;
CREATE PROCEDURE `item_delete_real`()
	BEGIN
		DELETE FROM `item` WHERE `item`.`destroy`=1;
	END;;
DELIMITER ;


/*创建关于nexus_game.equip的真实删除的存储过程，用于维护阶段*/
DROP PROCEDURE IF EXISTS `equip_delete_real`;

DELIMITER ;;
CREATE PROCEDURE `equip_delete_real`()
	BEGIN
		DELETE FROM `equip` WHERE `equip`.`destroy`=1;
	END;;
DELIMITER ;


/*创建关于nexus_game.skill的真实删除的存储过程，维护阶段使用*/
DROP PROCEDURE IF EXISTS `skill_delete_real`;

DELIMITER ;;
CREATE PROCEDURE `skill_delete_real`()
	BEGIN
		DELETE FROM `skill` WHERE `skill`.`destroy`=1;
	END;;
DELIMITER ;


/*维护阶段，删除所有表的真正需要删除的数据*/
DROP PROCEDURE IF EXISTS `delete_real_all_data`;

DELIMITER ;;
CREATE PROCEDURE `delete_real_all_data`()
	BEGIN
		CALL character_delete_real();
		CALL item_delete_real();
		CALL equip_delete_real();
		CALL skill_delete_real();
	END;;
DELIMITER ;


/*以下内容中用于生成检测逻辑的测试数据的存储过程*/
DROP PROCEDURE IF EXISTS `test_procedure_create_data`;

DELIMITER ;;
CREATE PROCEDURE `test_procedure_create_data`(IN num INT UNSIGNED)
	BEGIN
		DECLARE role_idV INT UNSIGNED DEFAULT 1;
		DECLARE index_item INT UNSIGNED DEFAULT 1;
		DECLARE MAX_ITEM INT UNSIGNED DEFAULT 3;
		DECLARE index_equip INT UNSIGNED DEFAULT 1;
		DECLARE MAX_EQUIP INT UNSIGNED DEFAULT 3;
		DECLARE index_skill INT UNSIGNED DEFAULT 1;
		DECLARE MAX_SKILL INT UNSIGNED DEFAULT 3;
		DECLARE item_guid BIGINT UNSIGNED DEFAULT 1;
		DECLARE equip_guid BIGINT UNSIGNED DEFAULT 1;

		DELETE FROM `character`;
		DELETE FROM `item`;
		DELETE FROM `equip`;
		DELETE FROM `skill`;
		WHILE role_idV <= num DO
			/*生成新的角色*/
			INSERT INTO `character` (account_id,role_id,name,sex,race,class_type,
									map_id,x,y,z,orient,
									hp,mp,level,cur_exp )
							VALUES (role_idV, role_idV, CONCAT_WS('_','test',role_idV), 1, 1, 1,
									1, role_idV, role_idV, role_idV, role_idV,
									10,10,role_idV,role_idV );

			
			/*给新角色分发初始化物品*/
			SET index_item = 1;
			WHILE index_item <= MAX_ITEM DO
				INSERT INTO `item` ( guid,owner_id,info_id,count,slot,bag,flag)
							VALUES (item_guid, role_idV, index_item, 1, 1, 1, 1);
				SET index_item = index_item+1;
				SET item_guid = item_guid + 1;
			END WHILE;
			
			/*给新角色分发初始化装备*/
			SET index_equip = 1;
			WHILE index_equip < MAX_EQUIP DO
				INSERT INTO `equip` (	guid,owner_id, info_id,count,slot,bag,flag,
										quality,level,durability,weight,sharp,solid,tenacity,
										stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
										stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
										gem0,gem1,gem2,gem3,gem4,gem5,
										spell0,spell1,
										spell2,spell3,
										spell_rate0,spell_rate1,spell_rate2,
										enchantment,creator )
										VALUES ( equip_guid,role_idV,index_equip,1,1,1,1,
										1,2,3,4,5,6,7,
										1,2,3,4,5,6,7,8,
										1,2,3,4,5,6,7,8,
										1,2,3,4,5,6,
										1,2,3,4,
										1,2,3,
										1,role_idV );
				SET index_equip = index_equip+1;
				SET equip_guid = equip_guid + 1;
			END WHILE;

			/*给角色分发初始化skill*/
			SET index_skill = 1;
			WHILE index_skill < MAX_SKILL DO
				INSERT INTO `skill` (role, id, cd) VALUES (role_idV, index_skill, 1);
				SET index_skill = index_skill+1;
			END WHILE;

			SET role_idV = role_idV+1;
		END WHILE;
	END;;
DELIMITER ;


/*测试nexus_game.character的character_select  character_update  character_insert  character_delete存储过程*/
DROP PROCEDURE IF EXISTS `test_procedure_character`;

DELIMITER ;;
CREATE PROCEDURE `test_procedure_character`()
	BEGIN
		DECLARE doneV INT DEFAULT 0;
		DECLARE account_idV BIGINT DEFAULT 0;
		DECLARE role_idV INT DEFAULT 0;
		DECLARE nameV VARCHAR(32) DEFAULT '0';
		DECLARE character_cur CURSOR FOR  SELECT account_id,role_id,name FROM `character` WHERE `character`.`destroy`=0;
		DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET doneV = 1;
		OPEN character_cur;
		REPEAT
			FETCH character_cur INTO account_idV,role_idV,nameV;
			/*SELECT guidV,owner_idV,info_idV;*/
		UNTIL doneV END REPEAT;
		CLOSE character_cur;
		SELECT 'character_cache start';
		CALL `nexus_game`.`character_cache`(1);
		SELECT 'character_insert start';
		CALL `nexus_game`.`character_insert`(66,66,66,66,66,66, 1,2,3,4, 1,2,3,4,5,  1,2,3,4);
		SELECT 'character_update start';
		CALL `nexus_game`.`character_insert`(77,77,77,77,77,77, 1,2,3,4, 1,2,3,4,5,  1,2,3,4);
		CALL `nexus_game`.`character_update`(77,71,72,73,74, 1,2,3,4,5,  1,2,3);
		SELECT 'character_delete start';
		CALL `nexus_game`.`character_delete`(1);
	END;;
DELIMITER ;

/*测试nexus_game.item的item_select  item_update  item_insert  item_delete存储过程*/
DROP PROCEDURE IF EXISTS `test_procedure_item`;

DELIMITER ;;
CREATE PROCEDURE `test_procedure_item`()
	BEGIN
		DECLARE doneV INT DEFAULT 0;
		DECLARE guidV BIGINT DEFAULT 0;
		DECLARE owner_idV INT DEFAULT 0;
		DECLARE info_idV INT DEFAULT 0;
		DECLARE item_cur CURSOR FOR  SELECT guid,owner_id,info_id FROM `item` WHERE `item`.`destroy`=0;
		DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET doneV = 1;
		OPEN item_cur;
		REPEAT
			FETCH item_cur INTO guidV,owner_idV,info_idV;
			/*SELECT guidV,owner_idV,info_idV;*/
		UNTIL doneV END REPEAT;
		CLOSE item_cur;
		SELECT 'item_cache start';
		CALL `nexus_game`.`item_cache`(1);
		SELECT 'item_insert start';
		CALL `nexus_game`.`item_insert`(66, 1, 66, 66, 66, 66, 66);
		SELECT 'item_update start';
		CALL `nexus_game`.`item_insert`(77, 1, 77, 77, 77, 77, 77);
		CALL `nexus_game`.`item_update`(66, 2, 99, 99, 99, 99, 99);
		SELECT 'item_delete start';
		CALL `nexus_game`.`item_delete`(1);
	END;;
DELIMITER ;


/*测试nexus_game.equip的equip_select  equip_update  equip_insert  equip_delete存储过程*/
DROP PROCEDURE IF EXISTS `test_procedure_equip`;

DELIMITER ;;
CREATE PROCEDURE `test_procedure_equip`()
	BEGIN
		DECLARE doneV INT DEFAULT 0;
		DECLARE guidV BIGINT DEFAULT 0;
		DECLARE owner_idV INT DEFAULT 0;
		DECLARE info_idV INT DEFAULT 0;
		DECLARE equip_cur CURSOR FOR  SELECT guid,owner_id,info_id FROM `equip` WHERE `equip`.`destroy`=0;
		DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET doneV = 1;
		OPEN equip_cur;
		REPEAT
			FETCH equip_cur INTO guidV,owner_idV,info_idV;
			/*SELECT guidV,owner_idV,info_idV;*/
		UNTIL doneV END REPEAT;
		CLOSE equip_cur;
		SELECT 'equip_cache start';
		CALL `nexus_game`.`equip_cache`(1);
		SELECT 'equip_insert start';
		CALL `nexus_game`.`equip_insert`(66,1,66,66,66,66,66,  1,2,3,4,5,6,7, 1,2,3,4,5,6,7,8, 1,2,3,4,5,6,7,8, 1,2,3,4,5,6, 1,2,3,4, 1,2,3, 1,2);
		SELECT 'equip_update start';
		CALL `nexus_game`.`equip_insert`(77,1,77,77,77,77,77,  1,2,3,4,5,6,7, 1,2,3,4,5,6,7,8, 1,2,3,4,5,6,7,8, 1,2,3,4,5,6, 1,2,3,4, 1,2,3, 1,2);
		CALL `nexus_game`.`equip_update`(77,2,  99,99, 99,99,99, 99,99,99, 1,2,3,4,  1,2,3,4,5,6,7,8, 1,2,3,4,5,6,7,8, 1,2,3,4,5,6, 1,2,3,4, 1,2,3, 1,2);
		SELECT 'equip_delete start';
		CALL `nexus_game`.`equip_delete`(1);
	END;;
DELIMITER ;

/*测试nexus_game.skill的skill_select  skill_update  skill_insert  skill_delete存储过程*/
DROP PROCEDURE IF EXISTS `test_procedure_skill`;

DELIMITER ;;
CREATE PROCEDURE `test_procedure_skill`()
	BEGIN
		DECLARE doneV INT DEFAULT 0;
		DECLARE roleV BIGINT DEFAULT 0;
		DECLARE idV INT DEFAULT 0;
		DECLARE cdV INT DEFAULT 0;
		DECLARE skill_cur CURSOR FOR  SELECT role,id,cd FROM `skill` WHERE `skill`.`destroy`=0;
		DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET doneV = 1;
		OPEN skill_cur;
		REPEAT
			FETCH skill_cur INTO roleV,idV,cdV;
			/*SELECT roleV,idV,cdV;*/
		UNTIL doneV END REPEAT;
		CLOSE skill_cur;
		SELECT 'skill_cache start';
		CALL `nexus_game`.`skill_cache`(1);
		SELECT 'skill_insert start';
		CALL `nexus_game`.`skill_insert`(1, 66, 66);
		SELECT 'skill_update start';
		CALL `nexus_game`.`skill_insert`(1,77,77);
		CALL `nexus_game`.`skill_update`(1,77,99);
		SELECT 'skill_delete start';
		CALL `nexus_game`.`skill_delete`(1,1);
	END;;
DELIMITER ;


/*测试nexus_game中同时进行 character item equip skill 的基本测试*/
DROP PROCEDURE IF EXISTS `test_procedure_all`;

DELIMITER ;;
CREATE PROCEDURE `test_procedure_all`()
	BEGIN
		CALL `nexus_game`.`test_procedure_character`();
		CALL `nexus_game`.`test_procedure_item`();
		CALL `nexus_game`.`test_procedure_equip`();
		CALL `nexus_game`.`test_procedure_skill`();
		CALL `nexus_game`.`get_role_max`();
	END;;
DELIMITER ;