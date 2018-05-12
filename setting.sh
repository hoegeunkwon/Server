#!/bin/bash

sudo apt-get -y install mysql-server libmysqlclient-dev

STATUS=`sudo ufw status | grep inactive`

if [ -z "$STATUS" ]; then
	echo "Currently ufw enabled"
	echo "allow port: (3306, 55555)"
	sudo ufw allow 3306/tcp
	sudo ufw allow 55555/tcp
else
	echo "Currently ufw disabled"
fi
