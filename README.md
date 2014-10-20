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
	primary key(no)
	);

3.
