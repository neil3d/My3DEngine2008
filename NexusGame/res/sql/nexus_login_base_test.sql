/*关于nexus_login的基本检测*/


/*检测nexus_login.account_cache*/
DROP PROCEDURE IF EXISTS `test_account_cache`;
DELIMITER ;;
CREATE PROCEDURE `test_account_cache`()
BEGIN
	CALL account_cache();
END;;
DELIMITER ;

/*检测nexus_login.account_login_out*/
DROP PROCEDURE IF EXISTS `test_accont_login_out`;
DELIMITER ;;
CREATE PROCEDURE `test_accont_login_out`(IN idV INT, IN last_ipV TINYINT)
BEGIN
	CALL account_login_out(idV, last_ipV);
END;;
DELIMITER ;


/*增加指定数量的账号*/
DROP PROCEDURE IF EXISTS `test_add_account`;
DELIMITER ;;
CREATE PROCEDURE `test_add_account`(IN num INT)
BEGIN
	DECLARE account_idV INT DEFAULT 1;
	WHILE account_idV <= num DO
		INSERT INTO `account` (id, name, psd) VALUES(account_idV, CONCAT_WS('_','account_name', account_idV), `name`);
		SET account_idV = account_idV + 1;
	END WHILE;
END;;
DELIMITER ;


/*删除所有账号*/
DROP PROCEDURE IF EXISTS `test_delete_account`;
DELIMITER ;;
CREATE PROCEDURE `test_delete_account`()
BEGIN
	DELETE FROM `account`;
END;;
DELIMITER ;