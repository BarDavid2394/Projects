#include "grades.h"
#include "linked-list.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum {SUCCESS=0,FAIL};
#define MAX_GRADE 100
#define MIN_GRADE 0


typedef struct courses {
    char *course_name;
    int grade;
}  *course_t;

typedef struct student {
    char *name;
    int ID;
    struct list* courses;
} *student_t;

typedef struct grades{
    struct list* students;
} *grades_t;

int student_clone (void *element, void **output);
int course_clone (void *element, void **output);
void student_destroy (void *element);
void course_destroy (void *element);
//student_t create_student(int id, const char *name);
course_t course_create (int grade,const char* course_name);



void student_destroy (void *element)
{
    student_t student = (student_t) (student_t *) element;
    if(student == NULL)
    {
        return;
    }
    list_destroy(student->courses);
    free(student->name);
    free(student);
}
void course_destroy (void *element)
{
    course_t course = (course_t) (course_t *) element;
    if(course == NULL)
    {
        return;
    }
    free(course->course_name);
    free(course);
}
struct grades* grades_init()
{
    grades_t grades =(struct grades*) malloc(sizeof(struct grades));
    grades->students = list_init(student_clone, student_destroy);
    return grades;
}

void grades_destroy(struct grades *grades)
{
    int len = list_size(grades->students);
    for(int i=0;i<len;i++)
    {
        struct iterator* student_head = list_begin(grades->students);
        student_t curr_student = list_get(student_head);
//        struct iterator* course_head = list_begin(curr_student->courses);
//            course_t curr_courses = list_get(course_head);
//            course_destroy(curr_courses);
        student_destroy(curr_student);
        student_head = list_next(student_head);
//        course_head = list_next(course_head);
    }
    list_destroy(grades->students);
    free(grades);
}

int student_clone (void *element, void **output)
{
   student_t student = (student_t) (student_t *) element;
    student_t copy_student =(struct student*) malloc(sizeof (struct student));
    if(copy_student == NULL)
    {
        return FAIL;
    }
    copy_student->name= malloc(sizeof(char)* strlen(student->name)+1);
    if(copy_student->name == NULL)
    {
        return FAIL;
    }
    strcpy(copy_student->name,student->name);
    copy_student->courses = list_init(course_clone,course_destroy);
    copy_student->ID = student->ID;
   struct iterator* head_course_src = list_begin(student->courses);
//   struct iterator* head_course_copy = list_begin(copy_student->courses);
   while(head_course_src != NULL)
   {
       course_t src_courses = list_get(head_course_src);
       list_push_back(copy_student->courses, src_courses);
       head_course_src = list_next(head_course_src);
   }
//    course_t copy_courses = list_get(head_course_copy);
        *output = copy_student;
        return SUCCESS;
}
int course_clone (void *element, void **output)
{
    course_t course = (course_t) (course_t *) element;
    course_t copy_course = (struct courses*) malloc(sizeof(struct courses));
    if(copy_course == NULL) {
        return FAIL;
    }
    copy_course->course_name = malloc(sizeof(char) * strlen(course->course_name)+1);
    if(copy_course->course_name == NULL)
    {
        return FAIL;
    }
    strcpy(copy_course->course_name, course->course_name);
    copy_course->grade = course->grade;
    *output = copy_course;
    return SUCCESS;
}

course_t course_create (int grade,const char* course_name)
{
    course_t course = (struct courses*) malloc(sizeof(struct courses));
    if(course == NULL) {
        return NULL;
    }
    course->course_name = malloc(sizeof(char) * strlen(course_name)+1);
    if(course->course_name == NULL)
    {
        return NULL;
    }
    strcpy(course->course_name, course_name);
    course->grade= grade;
    return course;
}

int grades_add_student(struct grades *grades, const char *name, int id)
{
    if(grades == NULL)
    {
        return FAIL;
    }
    student_t current = NULL;
    struct iterator* head = list_begin(grades->students);
    while (head != NULL) {
        current = list_get(head);
        if (current->ID == id) {
            return FAIL;
        }
        head = list_next(head);
    }
    student_t new_student =(struct student*) malloc(sizeof (struct student));
    if(new_student == NULL)
    {
        return FAIL;
    }
    new_student->ID = id;
    new_student->name= malloc(sizeof(char)* strlen(name)+1);
    if(new_student->name == NULL)
    {
        return FAIL;
    }
    strcpy(new_student->name,name);
    new_student->courses = list_init(student_clone,student_destroy);
    return list_push_back(grades->students, new_student);
}

int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade) {
    if (grades == NULL) {
        return FAIL;
    }
    if ((grade > MAX_GRADE) || (grade < MIN_GRADE)) {
        return FAIL;
    }
    struct iterator *head = list_begin(grades->students);

    while (head != NULL)
    {
        student_t curr_student = list_get(head);
        if(curr_student->ID != id)
        {
            head = list_next(head);
            continue;
        }
        struct iterator* course_head = list_begin(curr_student->courses);
        while (course_head != NULL) {
            course_t curr_course = list_get(course_head);
            if (strcmp(curr_course->course_name, name) == 0) {
                return FAIL;
            }
            course_head = list_next(course_head);
        }
        course_t new_course = course_create(grade,name);
        return list_push_back(curr_student->courses, new_course);
    }
    return FAIL;
}

float grades_calc_avg(struct grades *grades, int id, char **out)
{
    if(grades == NULL)
    {
        *out=NULL;
        return -1;
    }
    struct iterator* head = list_begin(grades->students);
    student_t current=NULL;
    while (head != NULL) {
        current = list_get(head);
        if (current->ID == id) {
            break;
        }
        else {
            head = list_next(head);
            continue;
        }
    }
    if(head == NULL)
    {
        *out=NULL;
        return -1;
    }
    float sum=0;
    int course_counter=0;
    char* name = malloc(sizeof(char) * strlen(current->name));
    strcpy(name, current->name);
    *out = name;
    struct iterator* head_course = list_begin(current->courses);
        if(head_course == NULL)
        {
            return 0;
        }
        while(head_course != NULL){
            course_t curr_course = list_get(head_course);
            int curr_grade = curr_course->grade;
            sum +=curr_grade;
            course_counter++;
            head_course = list_next(head_course);
        }
        return (sum/course_counter);
}

int grades_print_student(struct grades *grades, int id)
{
    if(grades==NULL)
    {
        return FAIL;
    }
    struct iterator* head= list_begin(grades->students);
    student_t current_student= list_get(head);
    while(head!=NULL){
        current_student= list_get(head);
        if(current_student->ID!=id){
            head=list_next(head);
            continue;
        }
        printf("%s %d: ", current_student->name,
               current_student->ID);
        struct iterator* head_course= list_begin(current_student->courses);
        course_t current_course=list_get(head_course);
        head=list_next(head);
        while(head_course!=NULL) {
            current_course= list_get(head_course);
            printf("%s %d",current_course->course_name,
                   current_course->grade);
            head_course= list_next(head_course);
        }
        printf("\n");
        return SUCCESS;
    }
    return FAIL;
}

int grades_print_all(struct grades *grades)
{
    if(grades==NULL)
    {
        return FAIL;
    }
    struct iterator* head= list_begin(grades->students);
    student_t current_student= NULL;
    while(head!=NULL)
    {
         current_student= list_get(head);
        if(grades_print_student(grades,current_student->ID)!=SUCCESS)
        {
            return FAIL;
        }
        head= list_next(head);
    }
    return SUCCESS;
}
