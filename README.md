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

	
	Test Value
	insert into userlist(id, pw, name, perm, groupid, fd) values('test1', '1234', '부모', 'parents', 54321, -1);
	insert into userlist(id, pw, name, perm, groupid, fd) values('', '', '권회근', 'child', 54321, -1);

3. Shield Protocol
	처음에 클라이언트로 접속을하면 parents OR child 를 전송하여 자신이 부모인지, 자식인지 서버에게 알린다.
	구분자 : '|'

	1. parents
		(1) login
			login id pw
		(2) join
			join id pw
		(3) send msg
			childID|msg
	
	2. child
		(1) login
			login id pw
		(2) join
			join id pw
		(3) send msg
			parentsID|msg


