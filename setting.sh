sudo apt-get -y install mysql-server
sudo apt-get -y install libmysqlclient-dev
sudo ufw enable
sudo ufw allow 22/tcp
sudo ufw allow 3306/tcp
sudo ufw allow 55555/tcp
