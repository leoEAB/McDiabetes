drop function if exists	authorize;
drop function if exists isAdmin;
drop function if exists getUserData;

DELIMITER |
CREATE FUNCTION authorize(username VARCHAR(20), userpassword VARCHAR(30)) RETURNS BOOL
BEGIN
	DECLARE userCheck BOOL;
    SET userCheck = IF((SELECT userName FROM user WHERE user.userName = username AND user.userPassword = userpassword) != 1, 1, 0);
    RETURN userCheck;
END;
|
DELIMITER ;

DELIMITER |
CREATE FUNCTION isAdmin(username VARCHAR(20)) RETURNS BOOL
BEGIN
	DECLARE adminCheck BOOL;
    SET adminCheck = IF((SELECT userName FROM user WHERE user.userName = username AND user.userType = "a") != 1, 1, 0);
    RETURN adminCheck;
END;
|
DELIMITER ;

DELIMITER |
CREATE FUNCTION getUserData(username VARCHAR(20), what VARCHAR(20)) RETURNS VARCHAR(50)
BEGIN
	DECLARE result VARCHAR(50);
    DECLARE strippedWhat VARCHAR(20);
    
    -- SET strippedWhat = (SELECT CONCAT());
    

    SET result = (SELECT strippedWhat FROM person WHERE person.userName = username);
    
    RETURN result;
END;
|
DELIMITER ;

-- test:
-- select isAdmin("mihi");
-- select isAdmin("user");
select getUserData("mihi94", "firstName");



