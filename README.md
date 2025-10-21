# Devsoc_Syscall_module0
Module 0 assignment of vertical syscall containing touch.c and cat.c codes
we first analyse all the commands in manual of wsl
then analyse touch.c and understand use of fcntl header file which provides declarations for file control operations used for manipulating file descriptors and then use its predefined functions like open, O_CREAT etc.
then we use the above knowledge and impliment it on cat.c. where we check for correct number of arguments and then open the file and read it while taking care of any errors and close the file
then we modify the touch file and add 3 commands 
./touch -a filename to change access time of file 
./touch -m filename to change modification time of file
./touch -c filename to disable creation if file not present already
we use sys/stat header file for stat structure which is a predefined structure we can use to store various information about the file
we use utime header file for utimbuf structure 
