#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 50
#define  num_students 12
#define num_assignments 3
#define num_groups 2


typedef struct student_record                                                                                                              //1st
{
    int student_id;
    char name[50];
    int group_partner_id;
    int group_id;
    int assignment_id;
    char assignment_name[50];
    char deadline[20];
    char status[20];
    int offline_marks;
    int viva_marks;
    int height;
    struct student_record* left;
    struct student_record* right;
} student_record;


typedef struct assignment_record {
    int assignment_id;
    char topic_name[50];
    char status[20];
    struct student_group_record* root;
    int height;
    struct assignment_record* left;
    struct assignment_record* right;
} assignment_record;


typedef struct student_group_record {
    int group_id;
    int student1_id;
    int student2_id;
    char status[20];
    int marks_given_student1;
    int marks_given_student2;
    int height;
    struct student_group_record* left;
    struct student_group_record* right;
} student_group_record;

int height(student_record *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

student_record* rightRotate(student_record *y)
{
    student_record *x = y->left;
    student_record *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

student_record* leftRotate(student_record *x)
{
    student_record *y = x->right;
    student_record *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(student_record *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return height(n->left) - height(n->right);
}

student_record* newNode(int student_id, int group_partner_id, int group_id, int assignment_id, int offline_marks, int viva_marks)
{
    student_record* node = (student_record*) malloc(sizeof(student_record));
    node->student_id = student_id;
    node->group_partner_id=group_partner_id;
    node->group_id=group_id;
    node->assignment_id=assignment_id;
    node->offline_marks=offline_marks;
    node->viva_marks=viva_marks;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

student_record* insert(student_record* node, int key, int group_partner_id, int group_id, int assignment_id, int offline_marks, int viva_marks)
{
    if (node == NULL)
    {
        return (newNode(key, group_partner_id, group_id, assignment_id,  offline_marks, viva_marks));
    }

    if (key < node->student_id)
    {
        node->left = insert(node->left, key, group_partner_id, group_id, assignment_id,  offline_marks, viva_marks);
    }
    else if (key > node->student_id)
    {
        node->right = insert(node->right, key, group_partner_id, group_id, assignment_id,  offline_marks, viva_marks);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->student_id)
    {
        return rightRotate(node);
    }
    if (balance < -1 && key > node->right->student_id)
    {
        return leftRotate(node);
    }
    if (balance > 1 && key > node->left->student_id)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->student_id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}


student_record* createsr(student_record* sr_root)
{
    int student_id, group_partner_id, group_id, assignment_id, offline_marks, viva_marks=0;

    FILE *fp2 = fopen("student_record_num.txt", "r");
    if (fp2 == NULL)
    {
        printf("Error opening file.\n");
    }
    int i;
    for( i=0; i<num_students; i++)
    {
        fscanf(fp2, "%d", &student_id);
        fscanf(fp2, "%d", &group_partner_id);
        fscanf(fp2, "%d", &group_id);
        fscanf(fp2, "%d", &assignment_id);
        fscanf(fp2, "%d", &offline_marks);
        fscanf(fp2, "%d", &viva_marks);
        sr_root= insert(sr_root, student_id, group_partner_id, group_id, assignment_id,  offline_marks, viva_marks);
    }
    fclose(fp2);
    
    
    FILE *fp1 = fopen("student_record_char.txt", "r");
    if (fp1 == NULL) {
        printf("Error opening file.\n");
    }
    
    student_record* stack[num_students];
    int top = -1;
    student_record* current = sr_root;
    while (current != NULL || top != -1)
    {
        while (current != NULL)
        {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        fgets(current->name, size, fp1);
        fgets(current->assignment_name, size, fp1);
        fgets(current->deadline, size, fp1);
        fgets(current->status, size, fp1);
        current = current->right;
    }
    fclose(fp1);
    return sr_root;
}


void display(student_record *root)
{
    if(root!=NULL)
    {
        display(root->left);
        printf("Student ID: %d\n", root->student_id);
        printf("Name: %s", root->name);
        printf("Group Partner ID: %d\n", root->group_partner_id);
        printf("Group ID: %d\n", root->group_id);
        printf("Assignment ID: %d\n", root->assignment_id);
        printf("Assignment Name: %s", root->assignment_name);
        printf("Deadline: %s", root->deadline);
        printf("Status: %s", root->status);
        printf("Offline Evaluation Marks: %d\n", root->offline_marks);
        printf("Viva Marks: %d\n\n", root->viva_marks);
        display(root->right);
    }
}



int ass_height(assignment_record *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

assignment_record* ass_rightRotate(assignment_record *y)
{
    assignment_record *x = y->left;
    assignment_record *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(ass_height(y->left), ass_height(y->right)) + 1;
    x->height = max(ass_height(x->left), ass_height(x->right)) + 1;

    return x;
}

assignment_record* ass_leftRotate(assignment_record *x)
{
    assignment_record *y = x->right;
    assignment_record *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(ass_height(x->left), ass_height(x->right)) + 1;
    y->height = max(ass_height(y->left), ass_height(y->right)) + 1;

    return y;
}

int getassBalance(assignment_record *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return ass_height(n->left) - ass_height(n->right);
}

assignment_record* newassNode(int assignment_id)
{
    assignment_record* node = (assignment_record*) malloc(sizeof(assignment_record));
    node->assignment_id = assignment_id;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

int grp_height(student_group_record *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

student_group_record* grp_rightRotate(student_group_record *y)
{
    student_group_record *x = y->left;
    student_group_record *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(grp_height(y->left), grp_height(y->right)) + 1;
    x->height = max(grp_height(x->left), grp_height(x->right)) + 1;

    return x;
}

student_group_record* grp_leftRotate(student_group_record *x)
{
    student_group_record *y = x->right;
    student_group_record *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(grp_height(x->left), grp_height(x->right)) + 1;
    y->height = max(grp_height(y->left), grp_height(y->right)) + 1;

    return y;
}

int getgrpBalance(student_group_record *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return grp_height(n->left) - grp_height(n->right);
}

student_group_record* newgrpNode(int group_id, int student1_id, int student2_id, int marks_given_student1, int marks_given_student2)
{
    student_group_record* node = (student_group_record*) malloc(sizeof(student_group_record));
    node->group_id=group_id;
    node->student1_id=student1_id;
    node->student2_id=student2_id;
    node->marks_given_student1=marks_given_student1;
    node->marks_given_student2=marks_given_student2;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}



student_group_record* grp_insert(student_group_record* node, int key, int student1_id, int student2_id, int marks_given_student1, int marks_given_student2)
{
    if (node == NULL)
    {
        return (newgrpNode(key, student1_id, student2_id, marks_given_student1,marks_given_student2));
    }

    if (key < node->group_id)
    {
        node->left = grp_insert(node->left, key, student1_id, student2_id, marks_given_student1,marks_given_student2);
    }
    else if (key > node->group_id)
    {
        node->right = grp_insert(node->right, key, student1_id, student2_id, marks_given_student1,marks_given_student2);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(grp_height(node->left), grp_height(node->right));

    int balance = getgrpBalance(node);

    if (balance > 1 && key < node->left->group_id)
    {
        return grp_rightRotate(node);
    }
    if (balance < -1 && key > node->right->group_id)
    {
        return grp_leftRotate(node);
    }
    if (balance > 1 && key > node->left->group_id)
    {
        node->left = grp_leftRotate(node->left);
        return grp_rightRotate(node);
    }
    if (balance < -1 && key < node->right->group_id)
    {
        node->right = grp_rightRotate(node->right);
        return grp_leftRotate(node);
    }

    return node;
}


assignment_record* ass_insert(assignment_record* node, int key)
{
    if (node == NULL)
    {
        return (newassNode(key));
    }

    if (key < node->assignment_id)
    {
        node->left = ass_insert(node->left, key);
    }
    else if (key > node->assignment_id)
    {
        node->right = ass_insert(node->right, key);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(ass_height(node->left), ass_height(node->right));

    int balance = getassBalance(node);

    if (balance > 1 && key < node->left->assignment_id)
    {
        return ass_rightRotate(node);
    }
    if (balance < -1 && key > node->right->assignment_id)
    {
        return ass_leftRotate(node);
    }
    if (balance > 1 && key > node->left->assignment_id)
    {
        node->left = ass_leftRotate(node->left);
        return ass_rightRotate(node);
    }
    if (balance < -1 && key < node->right->assignment_id)
    {
        node->right = ass_rightRotate(node->right);
        return ass_leftRotate(node);
    }

    return node;
}


assignment_record* createar(assignment_record* ar_root)
{
    int assignment_id, group_id, student1_id, student2_id, marks_given_student1, marks_given_student2=0;

    FILE *fp2 = fopen("assignment_record_num.txt", "r");
    if (fp2 == NULL)
    {
        printf("Error opening file.\n");
    }
    int i;
    for(i=0; i<num_assignments; i++)
    {
        fscanf(fp2, "%d", &assignment_id);
        ar_root= ass_insert(ar_root, assignment_id);
    }
    fclose(fp2);
    
    
    FILE *fp1 = fopen("assignment_record_char.txt", "r");
    FILE *fp3 = fopen("student_group_record_char.txt", "r");
    FILE *fp4 = fopen("student_group_record_num.txt", "r");
    if (fp1 == NULL)
    {
        printf("Error opening file.\n");
    }
    
    assignment_record* stack[num_assignments];
    int top = -1;
    assignment_record* current = ar_root;
    while (current != NULL || top != -1)
    {
        while (current != NULL)
        {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        fgets(current->topic_name, size, fp1);
        fgets(current->status, size, fp1);
        
        int i;
        for( i=0;i<num_groups;i++)
        {
            fscanf(fp4, "%d", &group_id);
            fscanf(fp4, "%d", &student1_id);
            fscanf(fp4, "%d", &student2_id);
            fscanf(fp4, "%d", &marks_given_student1);
            fscanf(fp4, "%d", &marks_given_student2);
            current->root=grp_insert(current->root, group_id, student1_id, student2_id, marks_given_student1, marks_given_student2);
        }
        student_group_record* current_grp = current->root;
            student_group_record* grp_stack[num_groups];
            int grp_top = -1;
            
            while (current_grp != NULL || grp_top != -1)
            {
                while (current_grp != NULL)
                {
                    grp_stack[++top] = current_grp;
                    current_grp = current_grp->left;
                }
                
                current_grp = grp_stack[top--];
                fgets(current_grp->status, size, fp3);
                
                current_grp = current_grp->right;
            }
        
        current = current->right;
    }
    fclose(fp1);
    
    return ar_root;
}


void displaygrp(student_group_record* grp_root)
{
    if (grp_root != NULL)
    {
        displaygrp(grp_root->left);
        printf("Group ID: %d\n", grp_root->group_id);
        printf("Student 1 ID: %d\n", grp_root->student1_id);
        printf("Student 2 ID: %d\n", grp_root->student2_id);
        printf("Marks given to student 1: %d\n", grp_root->marks_given_student1);
        printf("Marks given to student 2: %d\n", grp_root->marks_given_student2);
        printf("Status: %s\n", grp_root->status);
        displaygrp(grp_root->right);
    }
}


void displayar(assignment_record* ar_root)
{
    if (ar_root != NULL)
    {
        displayar(ar_root->left);
        printf("Assignment ID: %d\n", ar_root->assignment_id);
        printf("Topic name: %s\n", ar_root->topic_name);
        printf("Status: %s\n", ar_root->status);
        displaygrp(ar_root->root);
        displayar(ar_root->right);
    }
}


student_record* studentnewNode(int student_id, int group_partner_id, int group_id, int assignment_id, int offline_marks, int viva_marks, char *name, char *assignment_name, char *deadline, char *status)
{
    student_record* node = (student_record*) malloc(sizeof(student_record));
    node->student_id = student_id;
    node->group_partner_id=group_partner_id;
    node->group_id=group_id;
    node->assignment_id=assignment_id;
    node->offline_marks=offline_marks;
    node->viva_marks=viva_marks;
    strcpy(node->name, name);
    printf("%s.....", node->name);
    strcpy(node->assignment_name, assignment_name) ;
    strcpy(node->deadline, deadline);
    strcpy(node->status, status);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

student_record* insert_studentNode(student_record* node, int key, int group_partner_id, int group_id, int assignment_id, int offline_marks, int viva_marks, char* name, char* assignment_name, char* deadline, char* status)
{
    if (node == NULL)
    {
        return (studentnewNode(key, group_partner_id, group_id, assignment_id, offline_marks, viva_marks, name, assignment_name, deadline, status));
    }

    if (key < node->student_id)
    {
        node->left = insert_studentNode(node->left, key, group_partner_id, group_id, assignment_id,  offline_marks, viva_marks, name, assignment_name, deadline, status);
    }
    else if (key > node->student_id)
    {
        node->right = insert_studentNode(node->right, key, group_partner_id, group_id, assignment_id,  offline_marks, viva_marks, name, assignment_name, deadline, status);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->student_id)
    {
        return rightRotate(node);
    }
    if (balance < -1 && key > node->right->student_id)
    {
        return leftRotate(node);
    }
    if (balance > 1 && key > node->left->student_id)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->student_id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}


assignment_record* update_assignment_record(assignment_record* ar_root, int student_id, int group_partner_id, int group_id, int assignment_id, int offline_marks, int viva_marks, char* name, char* assignment_name, char* deadline, char* status)
{
    int grp_exists = 0;
    int assignment_exists = 0;
    assignment_record* current_ar = ar_root;
    while (current_ar != NULL && assignment_exists == 0 && grp_exists == 0)
    {
        if (assignment_id < current_ar->assignment_id)
        {
            current_ar = current_ar->left;
        }
        else if (assignment_id > current_ar->assignment_id)
        {
            current_ar = current_ar->right;
        }
        else if (assignment_id == current_ar->assignment_id)
        {
            student_group_record* current_sgr = current_ar->root;
            while (current_sgr != NULL)
            {
                if (group_id < current_sgr->group_id)
                {
                    current_sgr = current_sgr->left;
                }
                else if (group_id > current_sgr->group_id)
                {
                    current_sgr = current_sgr->right;
                }
                else if (group_id == current_sgr->group_id)
                {
                    grp_exists = 1;
                    current_sgr->student2_id = student_id;
                    current_sgr->marks_given_student2 = offline_marks + viva_marks;
                    break;
                }
            }
            if (current_sgr == NULL && grp_exists == 0)
            {
                current_ar->root = grp_insert(current_ar->root, group_id, student_id, group_partner_id, offline_marks + viva_marks, 0);
                grp_exists = 1;
            }
        }
    }
    if (assignment_exists == 0)
    {
        assignment_exists = 1;
        current_ar = ass_insert(current_ar, assignment_id);
        if (current_ar == ar_root) {
            ar_root = current_ar;
        }
        while (current_ar != NULL)
        {
            if (assignment_id < current_ar->assignment_id)
            {
                current_ar = current_ar->left;
            }
            else if (assignment_id > current_ar->assignment_id)
            {
                current_ar = current_ar->right;
            }
            else if (assignment_id == current_ar->assignment_id)
            {
                current_ar->root = grp_insert(current_ar->root, group_id, student_id, group_partner_id, offline_marks + viva_marks, 0);
                student_group_record* current_sgr = current_ar->root;
                while (current_sgr != NULL)
                {
                    if (group_id < current_sgr->group_id)
                    {
                        current_sgr = current_sgr->left;
                    }
                    else if (group_id > current_sgr->group_id)
                    {
                        current_sgr = current_sgr->right;
                    }
                    else if (group_id == current_sgr->group_id)
                    {
                        strcpy(current_sgr->status,status);
                        break;
                    }
                }
                break;
            }
        }
    }
    return ar_root;
}




assignment_record* assignmentnewnode(int assignment_id, char *topic_name, char *status){
    assignment_record* node = (assignment_record*) malloc(sizeof(assignment_record));
    node->assignment_id = assignment_id;
    strcpy(node->topic_name, topic_name);
    strcpy(node->status, status) ;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}


student_group_record* grpnewnode(int group_id, int student1_id, int student2_id, int marks_given_student1, int marks_given_student2, char *status){
    student_group_record* node = (student_group_record*) malloc(sizeof(student_group_record));
    node->group_id = group_id;
    node->student1_id=student1_id;
    node->student2_id=student2_id;
    node->marks_given_student1=marks_given_student1;
    node->marks_given_student2=marks_given_student2;
    strcpy(node->status, status) ;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

student_group_record* grp_insert_assignment(student_group_record* node, int key, int student1_id, int student2_id, int marks_given_student1, int marks_given_student2, char* status)
{
    if (node == NULL)
    {
        return (grpnewnode(key, student1_id, student2_id, marks_given_student1,marks_given_student2, status));
    }

    if (key < node->group_id)
    {
        node->left = grp_insert_assignment(node->left, key, student1_id, student2_id, marks_given_student1,marks_given_student2, status);
    }
    else if (key > node->group_id)
    {
        node->right = grp_insert_assignment(node->right, key, student1_id, student2_id, marks_given_student1,marks_given_student2, status);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(grp_height(node->left), grp_height(node->right));

    int balance = getgrpBalance(node);

    if (balance > 1 && key < node->left->group_id)
    {
        return grp_rightRotate(node);
    }
    if (balance < -1 && key > node->right->group_id)
    {
        return grp_leftRotate(node);
    }
    if (balance > 1 && key > node->left->group_id)
    {
        node->left = grp_leftRotate(node->left);
        return grp_rightRotate(node);
    }
    if (balance < -1 && key < node->right->group_id)
    {
        node->right = grp_rightRotate(node->right);
        return grp_leftRotate(node);
    }

    return node;
}


assignment_record* insert_assignmentNode(assignment_record* node, int key, char* topic_name, char* status, int group_id, int student1_id, int student2_id, int marks_given_student1, int marks_given_student2, char* grp_status)
{
    if (node == NULL)
    {
        return (assignmentnewnode(key, topic_name, status));
    }

    if (key < node->assignment_id)
    {
        node->left = insert_assignmentNode(node->left, key, topic_name, status, group_id, student1_id, student2_id, marks_given_student1, marks_given_student2, grp_status);
    }
    else if (key > node->assignment_id)
    {
        node->right = insert_assignmentNode(node->right, key, topic_name, status, group_id, student1_id, student2_id, marks_given_student1, marks_given_student2, grp_status);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(ass_height(node->left), ass_height(node->right));

    int balance = getassBalance(node);

    if (balance > 1 && key < node->left->assignment_id)
    {
        node=ass_rightRotate(node);
    }
    if (balance < -1 && key > node->right->assignment_id)
    {
        node=ass_leftRotate(node);
    }
    if (balance > 1 && key > node->left->assignment_id)
    {
        node->left = ass_leftRotate(node->left);
        node=ass_rightRotate(node);
    }
    if (balance < -1 && key < node->right->assignment_id)
    {
        node->right = ass_rightRotate(node->right);
        node=ass_leftRotate(node);
    }
    assignment_record* current_ar=node;
    while (current_ar != NULL)
    {
        if (key < current_ar->assignment_id)
        {
            current_ar = current_ar->left;
        }
        else if (key > current_ar->assignment_id)
        {
            current_ar = current_ar->right;
        }
        else if (key == current_ar->assignment_id)
        {
            current_ar->root=grpnewnode(group_id, student1_id, student2_id, marks_given_student1, marks_given_student2, status);
            break;
        }
        
    }
                
    return node;
}


student_record* studentnewNode_updatestudent(int student_id, int group_partner_id, int group_id, int assignment_id, int offline_marks, int viva_marks, char* assignment_name)
{
    student_record* node = (student_record*) malloc(sizeof(student_record));
    node->student_id = student_id;
    node->group_partner_id=group_partner_id;
    node->group_id=group_id;
    node->assignment_id=assignment_id;
    node->offline_marks=offline_marks;
    node->viva_marks=viva_marks;
    strcpy(node->assignment_name, assignment_name) ;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

student_record* update_student_record(student_record* node, int key, int group_partner_id, int group_id, int assignment_id, char* assignment_name, int offline_marks, int viva_marks)
{
    if (node == NULL)
    {
        return (studentnewNode_updatestudent(key, group_partner_id, group_id, assignment_id, offline_marks, viva_marks, assignment_name));
    }

    if (key < node->student_id)
    {
        node->left = update_student_record(node->left, key, group_partner_id, group_id, assignment_id, assignment_name, offline_marks, viva_marks);
    }
    else if (key > node->student_id)
    {
        node->right = update_student_record(node->right, key, group_partner_id, group_id, assignment_id, assignment_name, offline_marks, viva_marks);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->student_id)
    {
        return rightRotate(node);
    }
    if (balance < -1 && key > node->right->student_id)
    {
        return leftRotate(node);
    }
    if (balance > 1 && key > node->left->student_id)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->student_id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}


void display_updated_student(student_record* node)
{
    if (node != NULL)
    {
        display_updated_student(node->left);
        printf("Student ID: %d\n", node->student_id);
        printf("Group Partner ID: %d\n", node->group_partner_id);
        printf("Group ID: %d\n", node->group_id);
        printf("Assignment ID: %d\n", node->assignment_id);
        printf("Assignment Name: %s", node->assignment_name);
        printf("Offline Marks: %d\n", node->offline_marks);
        printf("Viva Marks: %d\n\n", node->viva_marks);
        display_updated_student(node->right);
    }
}

void change_status_of_assignment(assignment_record* ar_root)
{
    if(ar_root!=NULL)
    {
        change_status_of_assignment(ar_root->left);
        student_group_record* current_group=ar_root->root;
        printf("%s%s", current_group->status, current_group->right->status);
        
        int status_change=0;
        int not_submitted=0;
        int submitted=0;
        
        while(current_group!=NULL && current_group->right != NULL && status_change==0)
        {
            if(strcmp((current_group->status),((current_group->right)->status))!=0)
            {
                status_change=1;
            }
            current_group=current_group->right;
        }
        if(status_change==0)
        {
            printf("Status of the assignment%d is changed to %s", ar_root->assignment_id, current_group->status);
        }
        
        if(status_change==1)
        {
            while(current_group!=NULL && not_submitted==0)
            {
                if((strcmp(current_group->status,"not-submitted\n")==0))
                {
                    not_submitted=1;
                }
                current_group=current_group->right;
            }
            
            if(not_submitted==1)
            {
                    printf("Status of the assignment%d is changed to %s\n", ar_root->assignment_id, "not-submitted");
            }
            
            if(not_submitted==0)
            {
                while(current_group!=NULL && submitted==0)
                {
                    if((strcmp(current_group->status,"submitted\n")==0))
                    {
                        submitted=1;
                    }
                    current_group=current_group->right;
                }
            }
            if(submitted==1)
            {
                    printf("Status of the assignment%d is changed to %s\n", ar_root->assignment_id, "submitted");
            }
        }
        change_status_of_assignment(ar_root->right);
    }
}


void assignment_declared_or_evaluated(assignment_record* ar_root)                                               //4th
{
    if(ar_root!=NULL)
    {
        assignment_declared_or_evaluated(ar_root->left);
        student_group_record* current_group=ar_root->root;
        
        int evaluated=0;
        while(current_group!=NULL && evaluated==0)
        {
            if((strcmp(current_group->status,"evaluated\n")!=0))
            {
                evaluated=1;
            }
            current_group=current_group->right;
        }
        if(evaluated==1)
        {
            printf("Assignment%d is declared\n", ar_root->assignment_id);
        }
        else
        {
            printf("Assignment%d is evaluated\n", ar_root->assignment_id);
        }
        assignment_declared_or_evaluated(ar_root->right);
    }
}



struct Date                                                                                                     //5th
{
    int d, m, y;
};
const int monthDays[12]
    = { 31, 28, 31, 30, 31, 30,
       31, 31, 30, 31, 30, 31 };
int countLeapYears(struct Date d)
{
    int years = d.y;
    if (d.m <= 2)
        years--;
    return ((years+1) / 4) - ((years+1) / 100) + ((years+1) / 400);
}
int getDifference(struct Date dt1,struct Date dt2)
{
    int i;
    long int n1 = dt1.y * 365 + dt1.d;
    for ( i = 0; i < dt1.m - 1; i++){
        n1 = n1 + monthDays[i];
    n1 = n1 + countLeapYears(dt1);
}
    long int n2 = dt2.y * 365 + dt2.d;
    for (i = 0; i < dt2.m - 1; i++){
        n2 = n2 + monthDays[i];
    n2 = n2 + countLeapYears(dt2);
}
    return (n2 - n1);
}

void duedate_is_over(assignment_record* ar_root) {
    if (ar_root == NULL) {
        return;
    }
    
    duedate_is_over(ar_root->left);
    
        int n;
        struct Date dt1 = { 16+ar_root->assignment_id, 3, 2023 };
        struct Date dt2 = { 17, 3, 2023 };
        n = getDifference(dt1, dt2);
        
        student_group_record* current_group = ar_root->root;
        while(current_group != NULL)
        {
            if(strcmp(current_group->status, "not-submitted\n") == 0)
            {
                if(n>=0)
                {
                    printf("Due date is over for student group %d\n", current_group->group_id);
                }
                else
                {
                    printf("Due date is not over for student group %d\n", current_group->group_id);
                }
                
                printf("Assignment ID: %d, Group ID: %d, Student 1 ID: %d, Student 2 ID: %d\n",
                       ar_root->assignment_id, current_group->group_id, current_group->student1_id, current_group->student2_id);
            }
            current_group = current_group->right;
        }
    
    
    duedate_is_over(ar_root->right);
}



void print_yet_to_be_evaluated(student_record* root)                                            //6th
{
    if (root == NULL) {
        return;
    }

    print_yet_to_be_evaluated(root->left);

    if (strcmp(root->status, "evaluated\n") != 0) {
        if (strcmp(root->status, "submitted\n") == 0) {
            printf("Student with student_id%d from student_group%d is yet to be evaluated\n", root->student_id, root->group_id);
            if (root->viva_marks == 0) {
                printf("Viva is to be taken for student with student_id%d from student_group%d\n\n", root->student_id, root->group_id);
            }
        }
    }

    print_yet_to_be_evaluated(root->right);
}


student_group_record* grp_insert_marks(student_group_record* node, int group_id, int student1_id, int student2_id, int marks_given_student1, int marks_given_student2)
{
    if (node == NULL)
    {
        return (newgrpNode(group_id, student1_id, student2_id, marks_given_student1,marks_given_student2));
    }

    if ((marks_given_student2 + marks_given_student1) < (node->marks_given_student2 + node->marks_given_student1))
    {
        node->left = grp_insert_marks(node->left, group_id, student1_id, student2_id, marks_given_student1,marks_given_student2);
    }
    else if ((marks_given_student2 + marks_given_student1) > (node->marks_given_student2+node->marks_given_student1))
    {
        node->right = grp_insert_marks(node->right, group_id, student1_id, student2_id, marks_given_student1,marks_given_student2);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(grp_height(node->left), grp_height(node->right));

    int balance = getgrpBalance(node);

    if (balance > 1 &&  (marks_given_student2+marks_given_student1< node->left->marks_given_student1+node->left->marks_given_student2))
    {
        return grp_rightRotate(node);
    }
    if (balance < -1 && (marks_given_student2+marks_given_student1 > node->right->marks_given_student1+node->right->marks_given_student2))
    {
        return grp_leftRotate(node);
    }
    if (balance > 1 && (marks_given_student2+marks_given_student1 > node->left->marks_given_student1+node->left->marks_given_student2))
    {
        node->left = grp_leftRotate(node->left);
        return grp_rightRotate(node);
    }
    if (balance < -1 && (marks_given_student2+marks_given_student1 < node->right->marks_given_student1+node->right->marks_given_student2))
    {
        node->right = grp_rightRotate(node->right);
        return grp_leftRotate(node);
    }

    return node;
}


assignment_record* createar_marks(assignment_record* ar_root)
{
    int assignment_id, group_id, student1_id, student2_id, marks_given_student1, marks_given_student2=0;

    FILE *fp2 = fopen("assignment_record_num.txt", "r");
    if (fp2 == NULL)
    {
        printf("Error opening file.\n");
    }

    int i;
    for( i=0; i<num_assignments; i++)
    {
        fscanf(fp2, "%d", &assignment_id);
        ar_root= ass_insert(ar_root, assignment_id);
    }
    fclose(fp2);


    FILE *fp1 = fopen("assignment_record_char.txt", "r");
    FILE *fp3 = fopen("student_group_record_char.txt", "r");
    FILE *fp4 = fopen("student_group_record_num.txt", "r");
    if (fp1 == NULL)
    {
        printf("Error opening file.\n");
    }

    assignment_record* stack[num_assignments];
    int top = -1;
    assignment_record* current = ar_root;
    while (current != NULL || top != -1)
    {
    
        while (current != NULL)
        {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        fgets(current->topic_name, size, fp1);
        fgets(current->status, size, fp1);
    
        int i;
        for( i=0;i<num_groups;i++)
        {

            fscanf(fp4, "%d", &group_id);
            fscanf(fp4, "%d", &student1_id);
            fscanf(fp4, "%d", &student2_id);
            fscanf(fp4, "%d", &marks_given_student1);
            fscanf(fp4, "%d", &marks_given_student2);
            current->root=grp_insert_marks(current->root, group_id, student1_id, student2_id, marks_given_student1, marks_given_student2);
        }
        student_group_record* current_grp = current->root;
        student_group_record* grp_stack[num_groups];
        int grp_top = -1;
        
        while (grp_top != -1)
        {
            while (current_grp != NULL)
            {
                grp_stack[++top] = current_grp;
                current_grp = current_grp->left;
            }
            
            current_grp = grp_stack[top--];
            fgets(current_grp->status, size, fp3);
            
            current_grp = current_grp->right;
        }
    
        current = current->right;
    }
    fclose(fp1);

    return ar_root;
}


void displaygrp_marks(student_group_record* grp_root)
{
    if (grp_root != NULL)
    {
        displaygrp_marks(grp_root->right);
        printf("Group ID %d(Students%d and%d)  Total Marks: %d\n", grp_root->group_id, grp_root->student1_id, grp_root->student2_id,  grp_root->marks_given_student1+grp_root->marks_given_student2);
        displaygrp_marks(grp_root->left);
    }
}


void displayar_sorted(assignment_record* ar_root)
{
    if (ar_root != NULL)
    {
        displayar_sorted(ar_root->left);
        printf("Assignment ID: %d\n", ar_root->assignment_id);
        displaygrp_marks(ar_root->root);
        displayar_sorted(ar_root->right);
    }
}

 
 void display_top_marks(assignment_record* ar_root_marks)
 {
     if(ar_root_marks!=NULL)
     {
         display_top_marks(ar_root_marks->left);
         printf("Assignment %d : \n", ar_root_marks->assignment_id);
         displaygrp_marks(ar_root_marks->root->right);
         display_top_marks(ar_root_marks->right);
     }
 }
 

void rangeSearch(assignment_record *root, int lower, int upper)
{
    if (root == NULL)
    {
        return;
    }
    if (root->assignment_id <= upper)
        rangeSearch(root->right, lower, upper);

    if (root->assignment_id >= lower && root->assignment_id <= upper)
    {
        printf("Assignment id %d ", root->assignment_id);
        student_group_record* grp_root=root->root;
        displaygrp(grp_root);
    }
   
    if (root->assignment_id >= lower)
        rangeSearch(root->left, lower, upper);
}


int main()
{
    int choice;
    student_record *sr_root = NULL;
    assignment_record *ar_root = NULL;
    sr_root=createsr(sr_root);
    //display(sr_head);
    ar_root=createar(ar_root);
    //displayar(ar_root);
    assignment_record* ar_root_marks=NULL;
    ar_root_marks=createar_marks(ar_root_marks);

    
    while (1) {
        printf("\n");
        printf("1. Insert a student record\n");
        printf("2. Insert an assignment record\n");
        printf("3. Change aassignment status\n");
        printf("4. Print if assignment is declared or evaluated\n");
        printf("5. Print the list of student groups that have not submitted the assignments even if th eduedate is over\n");
        printf("6. Print the list of groups yet to be evaluated\n");
        printf("7. Print the details of student-groups for a given assignment in decreasing order of their marks\n");
        printf("8. Print the student-groups receiving top marks in each of the assignments\n");
        printf("9. Range-search for details of assignments between lower and upper(between two assignment ids");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        int student_id, group_partner_id, group_id, assignment_id, offline_marks, viva_marks, student1_id, student2_id, marks_given_student1, marks_given_student2;
        char name[size], assignment_name[size], deadline[size], status[size], topic_name[size], ass_status[size], grp_status[size];
        int lower, upper;

        switch (choice) {
            case 1:
                printf("Enter student id: ");
                scanf("%d", &student_id);
                printf("Enter student name: ");
                scanf("%s", name);
                printf("Enter group partner id: ");
                scanf("%d", &group_partner_id);
                printf("Enter group id: ");
                scanf("%d", &group_id);
                printf("Enter assignment id: ");
                scanf("%d", &assignment_id);
                printf("Enter assignment name: ");
                scanf("%s", assignment_name);
                printf("Enter deadline: ");
                scanf("%s", deadline);
                printf("Enter status of assignment(submitted/not-submitted/evaluated): ");
                scanf("%s", status);
                printf("Enter offline marks: ");
                scanf("%d", &offline_marks);
                printf("Enter viva marks: ");
                scanf("%d", &viva_marks);
                sr_root=insert_studentNode(sr_root, student_id, group_partner_id, group_id, assignment_id, offline_marks, viva_marks, name, assignment_name, deadline, status);
                display(sr_root);
                ar_root=update_assignment_record(ar_root, student_id, group_partner_id, group_id, assignment_id, offline_marks, viva_marks, name, assignment_name, deadline, status);
                displayar(ar_root);
                printf("--------------------------------------\n");
                break;
            case 2:
                printf("Enter assignment id: ");
                scanf("%d", &assignment_id);
                printf("Enter topic name: ");
                scanf("%s", topic_name);
                printf("Enter status of assignment(submitted/not-submitted/evaluated): ");
                scanf("%s", ass_status);
                printf("Enter group id: ");
                scanf("%d", &group_id);
                printf("Enter first student id: ");
                scanf("%d", &student1_id);
                printf("Enter second student id: ");
                scanf("%d", &student2_id);
                printf("Enter status of group(submitted/not-submitted/evaluated): ");
                scanf("%s", grp_status);
                printf("Enter marks given to student 1: ");
                scanf("%d", &marks_given_student1);
                printf("Enter marks given to student 2: ");
                scanf("%d", &marks_given_student2);
                
                ar_root=insert_assignmentNode(ar_root, assignment_id, topic_name, ass_status, group_id, student1_id, student2_id, marks_given_student1, marks_given_student2, grp_status);
                displayar(ar_root);
                sr_root=update_student_record(sr_root, student1_id, student2_id, group_id, assignment_id, topic_name, (marks_given_student1/2), (marks_given_student1/2));
                sr_root=update_student_record(sr_root, student2_id, student1_id, group_id, assignment_id, topic_name, (marks_given_student2/2), (marks_given_student2/2));
                display_updated_student(sr_root);
                printf("--------------------------------------\n");
                break;
            case 3:
                change_status_of_assignment(ar_root);
                printf("--------------------------------------\n");
                break;
            case 4:
                assignment_declared_or_evaluated(ar_root);
                printf("--------------------------------------\n");
                break;
            case 5:
                duedate_is_over(ar_root);
                printf("--------------------------------------\n");
                break;
            case 6:
                print_yet_to_be_evaluated(sr_root);
                printf("--------------------------------------\n");
                break;
            case 7:
                displayar_sorted(ar_root_marks);
                printf("--------------------------------------\n");
                break;
            case 8:
                display_top_marks(ar_root_marks);
                printf("--------------------------------------\n");
                break;
            case 9:

                printf("Give the lower and upper assignment ids for Range Search : ");
                scanf("%d %d", &lower, &upper);
                rangeSearch(ar_root, lower, upper);
                printf("--------------------------------------\n");
                break;
            case 10:
                exit(0);
            default:
                printf("Invalid choice\n");
                printf("--------------------------------------\n");
        }
    }

    return 0;
}
