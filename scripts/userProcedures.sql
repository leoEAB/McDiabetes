drop procedure if exists addUser;
drop procedure if exists deleteUser;
drop procedure if exists addToCart;


DELIMITER |
CREATE PROCEDURE addUser(	IN username VARCHAR(20),
							IN password VARCHAR(30),
							IN firstname VARCHAR(20),
                            IN lastname varchar(20),
                            IN email varchar(30),
                            IN street varchar(20),
                            IN streetNr int,
                            IN plz varchar(6),
                            IN city varchar(20)
						)
	
    BEGIN
		INSERT INTO user(userName, userPassword, userType) VALUES (username, password, "u");
		INSERT INTO person VALUES (username, firstname, lastname, email, street, streetNr, plz, city);
	END;
    
|
DELIMITER ;

DELIMITER |
CREATE PROCEDURE deleteUser (IN username VARCHAR(20))
	BEGIN
		DELETE FROM user WHERE user.userName = username;
        DELETE FROM person WHERE person.userName = username;
        DELETE FROM cart WHERE cart.userName = username;
	END;
|
DELIMITER ;


DELIMITER |
CREATE PROCEDURE addToCart (	IN username VARCHAR(20), 
								IN name VARCHAR(20),
                                IN size VARCHAR(20)
							)
	BEGIN
		INSERT INTO cart(userName) VALUES (username);
		INSERT INTO cart(type, name, size, price) SELECT (type, name, size, price) FROM items where items.name = name AND items.size = size;
	END;
|
DELIMITER ;
