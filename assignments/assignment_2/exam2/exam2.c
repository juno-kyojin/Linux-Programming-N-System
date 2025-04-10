// BT2. Sau mỗi lệnh write dưới đây, 
// Cho biết nội dung sẽ được ghi vào file nào, 
// nội dung là gì và giải thích tại sao?

/* 
fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
fd3 = open(file, O_RDWR);
write(fd1, "Hello,", 6);
write(fd2, "world", 6);
lseek(fd2, 0, SEEK_SET);
write(fd1, "HELLO,", 6);
write(fd3, "Gidday", 6);
*/