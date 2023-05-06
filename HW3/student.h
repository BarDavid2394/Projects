#ifndef __STUDENT__
#define __STUDENT__

#include "stack.h"
#include "stdlib.h"

typedef struct student {
	char *name;
	int age;
    int id;
}*pstudent_t, student_t;

elem_t student_clone (elem_t e);
/* ------------------------------------------ */
/* Add here the reset of the declarations     */
/* ------------------------------------------ */

#endif /* __STUDENT__ */ 