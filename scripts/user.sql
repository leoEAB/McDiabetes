set foreign_key_checks = 0;

drop table if exists user;
drop table if exists person;
drop table if exists cart;
drop table if exists orderInfo;
drop table if exists orderContents;

set foreign_key_checks = 1;

create table user (
	userName varchar(20) not null unique primary key,
    userPassword varchar(30) not null,
    userType varchar(1) not null
);

create table person (
	userName varchar(20),
	firstName varchar(20),
    lastName varchar(20),
    email varchar(30) unique primary key,
    street varchar(20),
    streetNr int,
	plz varchar(6),
    city varchar(20),

    FOREIGN KEY (userName) REFERENCES user(userName) ON UPDATE CASCADE ON DELETE CASCADE
);

create table cart (
	userName varchar(20),
    type varchar(20),
    name varchar(50),
    size varchar(20),
	price dec(5,2),
    
	FOREIGN KEY (userName) REFERENCES user(userName) ON UPDATE CASCADE ON DELETE CASCADE

);

create table orderInfo (
	userName varchar(20),
    orderDate datetime,
    total dec(5,2),
    firstName varchar(20),
    lastName varchar(20),
    email varchar(30),
    street varchar(20),
    streetNr int,
	plz varchar(6),
    city varchar(20),
    
    
	FOREIGN KEY (userName) REFERENCES user(userName) ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY (email) REFERENCES person(email) ON UPDATE CASCADE ON DELETE CASCADE

);

create table orderContents(
	userName varchar(20),
    orderDate datetime,
	type varchar(20),
    name varchar(50),
    size varchar(20),
	price dec(5,2),

	FOREIGN KEY (userName) REFERENCES orderInfo(userName) ON UPDATE CASCADE ON DELETE CASCADE
		
);



-- tests

insert into user values ("mihi", "mihi", "a");
insert into user values ("user", "user", "u");
insert into user values ("user1", "user1", "u");

select * from user left outer join person on user.userName = person.userName;

select * from cart;

select * from orderInfo;

select * from orderContents;

select * from orderInfo left outer join orderContents on orderInfo.userName = orderContents.userName;

SET SQL_SAFE_UPDATES = 0;

delete from orderInfo;

select firstName from person where person.userName = "mihi94"; 



