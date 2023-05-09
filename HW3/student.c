#include "student.h"
#include <string.h>


elem_t student_clone(elem_t e) {
    if (e == NULL) {
        return NULL;
    }
    pstudent_t student = (student_t *) e;
    pstudent_t copy_student = malloc(sizeof(pstudent_t));
    if (copy_student == NULL) {
        return NULL;
    }
    copy_student->name = malloc(sizeof(char)* strlen(student->name));
    if (copy_student->name == NULL) {
        return NULL;
    }
    copy_student->age = student->age;

    strcpy(copy_student->name, student->name);
    copy_student->id = student->id;
    return copy_student;
}

void student_print(elem_t e) {
    if (e == NULL) {
        return;
    }
    pstudent_t student = (student_t *) e;
    printf("student name: %s, age: %d, id: %d\n", student->name, student->age, student->id);
}

void student_destroy(elem_t e) {
    if (e == NULL) {
        return;
    }
    pstudent_t student = (student_t *) e;
    free(student->name);
    free(student);
}
