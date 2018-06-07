set foreign_key_checks = 0;
drop table if exists user;
drop table if exists person;
drop table if exists cart;

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


insert into user values ("user", "user", "u");

select * from user;