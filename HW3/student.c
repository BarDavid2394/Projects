#include "student.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



elem_t student_clone(elem_t e) {
    if (e == NULL) {
        return NULL;
    }
    pstudent_t student = (student_t *) e;
    pstudent_t copy_student =(struct student*)malloc(sizeof(struct student));
    if (copy_student == NULL) {
        return NULL;
    }
    copy_student->name = malloc(sizeof(char)* strlen(student->name)+1);
    if (copy_student->name == NULL) {
        return NULL;
    }
    strcpy(copy_student->name, student->name);
    copy_student->age = student->age;
    copy_student->id = student->id;
    return copy_student;
}

void student_print(elem_t e) {
    if (e == NULL) {
        return;
    }
    pstudent_t student = (student_t *) e;
    printf("student name: %s, age: %d, id: %d.\n", student->name, student->age, student->id);
}

void student_destroy(elem_t e) {
    if (e == NULL) {
        return;
    }
    pstudent_t student = (student_t *) e;
    free(student->name);
    free(student);
}
