drop procedure if exists addUser;
drop procedure if exists deleteUser;
drop procedure if exists addToCart;
drop procedure if exists completeOrder;


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
		INSERT INTO cart (userName, type , name, size, price)
        SELECT userName, type, name, size, price FROM user AS US, items AS IT WHERE US.userName = username AND IT.name = name AND IT.size = size;
	END;
|
DELIMITER ;

DELIMITER |
CREATE PROCEDURE completeOrder( IN username VARCHAR(20))
	BEGIN
		-- trigger inserts a datetime in orderInfo before inserts
		INSERT INTO orderInfo (	userName,
								total,
								firstName,
								lastName,
								email,
								street,
								streetNr,
								plz,
								city
								)
		SELECT 	U.userName,
				SUM(C.price),
				P.firstName,
				P.lastName,
				P.email,
				P.street,
				P.streetNr,
				P.plz,
				P.city
		FROM user AS U, person AS P, cart AS C
        WHERE U.userName = username AND P.userName = username AND C.userName = username;


		-- triger inserts datetime() before inserts into orderContents
		INSERT INTO orderContents (	userName,
									type,
									name,
									size,
									price
								)
		SELECT userName, type, name, size, price 
        FROM user AS U, cart AS C 
        WHERE U.userName = username AND C.userName = username;
        
        

        
	END;
|
DELIMITER ;


-- tests
-- CALL addToCart("mihi", "Fries", "small");

-- CALL completeOrder("mihi94");
-- CALL completeOrder("user");