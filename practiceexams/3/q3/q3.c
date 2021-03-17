// COMP1521 18s1 Q2 ... addStudent() function
// - insert a Student record into a file in order

#include "Students.h"
// put any other required #include's here

void addStudent(int fd, Student stu)
{   
    int size = sizeof(Student);
    Student *temp = NULL;
    Student *student = malloc (sizeof(Student));
    while (read(fd, &student, size) == sizeof(Student)) {
        if (student->id > stu->id) {
            temp = student;
            write(fd, &stu, size); 
            break;
        }
    }
    lseek (
    
	// your code goes here
}
