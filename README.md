1. setting.sh를 실행시킨다.

2. DB 설정
	mysql -u root -p jjssm

	create database shield;
	
	create table userlist(
	no int(10) not null auto_increment,
	id varchar(20),
	pw varchar(20),
	name varchar(20),
	perm varchar(20),
	groupid int(10),
	fd int(10),
	primary key(no)
	);

3.






insert into userlist(id, pw, name, perm, groupid, fd) values('test_1', '1234', '', 'parents', 54321, -1);
insert into userlist(id, pw, name, perm, groupid, fd) values('test_2', '1234', '권회근', 'child', 54321, -1);

parents
child

Shield Protocol

1. parents, child

	(1) parents
		join id pw
		login id pw
