scp src/cat/s21_cat roxaneos@192.168.100.11:/home/roxaneos
scp src/grep/s21_grep roxaneos@192.168.100.11:/home/roxaneos
ssh roxaneos@192.168.100.11 "echo "1234" | sudo -S mv /home/roxaneos/s21_* /usr/local/bin/"