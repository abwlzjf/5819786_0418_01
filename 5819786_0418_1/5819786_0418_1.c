#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 100

typedef int element;
typedef struct StackType {
    element* data;
    int capacity;
    int top;
} StackType;

void init(StackType* sptr, int ofs) {
    sptr->data = (element*)malloc(sizeof(element) * ofs);
    sptr->top = -1;
    sptr->capacity = ofs;
}

int is_full(StackType* sptr) {
    if (sptr->top == sptr->capacity - 1) {
        sptr->capacity = sptr->capacity * 2;
        sptr->data = (element*)realloc(sptr->data, sizeof(element) * sptr->capacity);
        return 1;
    }
    return 0;
}

int is_empty(StackType* sptr) {
    return (sptr->top == -1);
}

void push(StackType* sptr, element item) {
    if (is_full(sptr)) {
        fprintf(stderr, "Stack is full\n");
        return;
    }
    else {
        sptr->top = sptr->top + 1;
        sptr->data[sptr->top] = item;
    }
}

element pop(StackType* sptr) {
    element r;
    if (is_empty(sptr)) {
        fprintf(stderr, "Stack is empty\n");
        return -1;
    }
    else {
        return(sptr->data[(sptr->top)--]);
    }
}

element peek(StackType* sptr) {
    element r;
    if (is_empty(sptr)) {
        fprintf(stderr, "Stack is empty\n");
        return -1;
    }
    else {
        return (sptr->data[sptr->top]);
    }
}

int prec(char op) {
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

int eval(char* expr) {
    StackType s;
    init(&s, MAX_STACK_SIZE);

    int len = strlen(expr);
    for (int i = 0; i < len; i++) {
        char ch = expr[i];

        if (isdigit(ch)) {
            int operand = 0;
            while (isdigit(expr[i])) {
                operand = operand * 10 + (expr[i] - '0');
                i++;
            }
            i--;
            push(&s, operand);
        }
        else if (ch != ' ') {
            int op2 = pop(&s);
            int op1 = pop(&s);
            int result;

            switch (ch) {
            case '+': result = op1 + op2; break;
            case '-': result = op1 - op2; break;
            case '*': result = op1 * op2; break;
            case '/': result = op1 / op2; break;
            default:
                printf("Invalid expression\n");
                return -1;
            }
            push(&s, result);
        }
    }
    return pop(&s);
}

void infix_to_postfix(char expr[], char postfix[]) {
    char top_op;
    int len = strlen(expr);
    StackType s;

    init(&s, MAX_STACK_SIZE);
    int idx = 0; // postfix 배열의 인덱스를 나타내는 변수
    for (int i = 0; i < len; i++) {
        switch (expr[i]) {
        case '+':
        case '-':
        case '*':
        case '/':
            while (!is_empty(&s) && prec(expr[i]) <= prec((char)peek(&s))) {
                postfix[idx++] = pop(&s); // 후위식에 연산자를 저장
            }
            push(&s, (int)expr[i]);
            break;
        case '(': push(&s, (int)expr[i]); break;
        case ')':
            top_op = (char)pop(&s);
            while (top_op != '(') {
                postfix[idx++] = top_op; // 후위식에 연산자를 저장
                top_op = (char)pop(&s);
            }
            break;
        default:
            if (isdigit(expr[i])) {
                postfix[idx++] = expr[i]; // 후위식에 피연산자를 저장
                if (!isdigit(expr[i + 1]) && expr[i + 1] != '\0') {
                    postfix[idx++] = ' '; // 피연산자와 연산자를 구분하기 위한 공백 저장
                }
            }
            break;
        }
    }

    while (!is_empty(&s)) {
        postfix[idx++] = pop(&s); // 남은 연산자를 후위식에 저장
    }
    postfix[idx] = '\0'; // 후위식의 끝에 널 문자 추가
}

int main() {
    char infix[MAX_STACK_SIZE];
    char postfix[MAX_STACK_SIZE];
    int choice;

    while (1) {
        printf("다음과 같은 메뉴로 동작하는 프로그램입니다:\n");
        printf("1. 중위식을 입력 받음\n");
        printf("2. 중위식을 후위식으로 변환\n");
        printf("3. 후위식을 계산\n");
        printf("4. 종료\n");
        printf("메뉴를 선택하세요: ");

        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("중위식을 입력하세요: ");
            getchar();
            fgets(infix, sizeof(infix), stdin);
            printf("입력된 중위식 : %s\n", infix);
            break;
        case 2:
            infix_to_postfix(infix, postfix);
            printf("후위식: %s\n", postfix);
            break;
        case 3:
            infix_to_postfix(infix, postfix);
            printf("계산 결과 : %d\n", eval(postfix));
            break;
        case 4:
            printf("프로그램을 종료합니다.\n");
            exit(1);
            break;
        default:
            printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
            break;
        }
    }
    return 0;
}