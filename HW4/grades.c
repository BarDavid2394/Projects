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

/*------declarations of the aux functions------*/
int student_clone (void *element, void **output);
int course_clone (void *element, void **output);
void student_destroy (void *element);
void course_destroy (void *element);
course_t course_create (int grade,const char* course_name);

//Initializes the "grades" data-structure.
struct grades* grades_init()
{
    grades_t grades =(struct grades*) malloc(sizeof(struct grades));
    grades->students = list_init(student_clone, student_destroy);
    return grades;
}
//Destroys "grades", de-allocate all memory!
void grades_destroy(struct grades *grades)
{
    if(grades==NULL){
        return;
    }
    list_destroy(grades->students);
    free(grades);
}
//creates course's type element and return it
course_t course_create (int grade,const char* course_name)
{
    course_t course = (struct courses*) malloc(sizeof(struct courses));
    if(course == NULL) {
        return NULL;
    }
    course->course_name = malloc(sizeof(char) * strlen(course_name)+1);
    if(course->course_name == NULL)
    {
        free(course);
        return NULL;
    }
    //inserting the course name and grade into the element
    strcpy(course->course_name, course_name);
    course->grade= grade;
    return course;
}
/*the function creates copy of the course element and return both the element
 and feedback if the action success */
int course_clone (void *element, void **output)
{
    course_t course = (struct courses *) element;
    course_t copy_course = (struct courses*) malloc(sizeof(struct courses));
    if(copy_course == NULL) {
        return FAIL;
    }
    copy_course->course_name = malloc(sizeof(char)
            * strlen(course->course_name)+1);
    //if course_name allocation is NULL, we want to free the element.
    if(copy_course->course_name == NULL)
    {
        free(copy_course);
        return FAIL;
    }
    strcpy(copy_course->course_name, course->course_name);
    copy_course->grade = course->grade;
    //output will point to the copy we've just made
    *output = copy_course;
    course_destroy(course);
    return SUCCESS;
}
//the function destroy the course element and it's elements
void course_destroy (void *element)
{
    course_t course = (struct courses *) element;
    if(course == NULL)
    {
        return;
    }
    free(course->course_name);
    free(course);
}
/*the function creates copy of the student element and return both the element
 and feedback if the action success */
int student_clone (void *element, void **output)
{
    student_t student = (struct student *) element;
    student_t copy_student =(struct student*) malloc(sizeof (struct student));
    if(copy_student == NULL)
    {
        return FAIL;
    }
    copy_student->name= malloc(sizeof(char)* strlen(student->name)+1);
    //if course_name allocation is NULL, we want to free the element.
    if(copy_student->name == NULL)
    {
        free(copy_student);
        return FAIL;
    }
    strcpy(copy_student->name,student->name);
    //initializing a list for the copy courses
    copy_student->courses = list_init(course_clone,course_destroy);
    copy_student->ID = student->ID;
    struct iterator* head_course_src = list_begin(student->courses);
    while(head_course_src != NULL)
    {
        course_t src_courses = list_get(head_course_src);
        list_push_back(copy_student->courses, src_courses);
        head_course_src = list_next(head_course_src);
    }
    //output will point to the copy we've just made
    *output = copy_student;
    student_destroy(student);
    return SUCCESS;
}
//the function destroy the student element and it's elements
void student_destroy (void *element)
{
    student_t student = (struct student *) element;
    if(student == NULL)
    {
        return;
    }
    free(student->name);
    list_destroy(student->courses);
    free(student);
}
//Adds a student with "name" and "id" to "grades"
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
//Adds a course with "name" and "grade" to the student with "id"
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade) {
    if (grades == NULL) {
        return FAIL;
    }
    //grade must be a number in the range 0-100
    if ((grade > MAX_GRADE) || (grade < MIN_GRADE)) {
        return FAIL;
    }
    struct iterator *head = list_begin(grades->students);

    while (head != NULL)
    {
        student_t curr_student = list_get(head);
        //checks if the student doesn't already exist
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
//Calculates the average of the student with "id" in "grades".
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
    char* name = malloc(sizeof(char) * strlen(current->name)+1);
    if(name==NULL){
        return -1;
    }
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
        return (float)(sum/course_counter);
}
//Prints the courses of the student with "id"
int grades_print_student(struct grades *grades, int id)
{
    if(grades==NULL)
    {
        return FAIL;
    }
    int counter=0;
    struct iterator* head= list_begin(grades->students);
    student_t current_student= list_get(head);
    while(head!=NULL){
        current_student= list_get(head);
        //looking for the exact student that needs to pe printed
        if(current_student->ID!=id){
            head=list_next(head);
            continue;
        }
        printf("%s %d:", current_student->name,
               current_student->ID);
        struct iterator* head_course= list_begin(current_student->courses);
        course_t current_course=list_get(head_course);
        head=list_next(head);
        while(head_course!=NULL) {
            //counter is for output issues. we want a different first print.
            counter++;
            current_course = list_get(head_course);
            if(counter==1){
                printf(" %s %d",current_course->course_name,
                       current_course->grade);
            }
            else {
                printf(", %s %d", current_course->course_name,
                       current_course->grade);
            }
            head_course = list_next(head_course);
        }
        printf("\n");
        return SUCCESS;
    }
    return FAIL;
}
//Prints all students in "grade" with all grades of each student
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
