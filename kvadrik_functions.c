
// Воробьев Иван
// 28.08.2021
// Копирование разрешено, если глазам не больно

//! @file kvadrik_functions.c
//! @mainpage
//! kvadrik_functions.c
//! \author Vorobev Ivan
//! \data 27.08.2021

#include "kvadrik_header.h"

//! количество тестов,которые программа будет проходить
#define TESTS_NUMBER 8

//! значение погрешности для этой программы
static const double ERROR_RATE = 0.000001;

int solve_equations(){

    printf("This programme is created to solve quadratic equation and linear\n");
    printf("Quadratic equation should look like a*x*x + b*x + c = 0\n");
    printf("If a = 0, the equation is linear\n\n");

    //!вводимые величины
    double a = 0, b = 0, c = 0;

    int err_code = 0;//мне так нравится
    err_code = get_coefficients(&a, &b, &c);

    if (err_code != 0)
        return err_code; // при обнаружении ошибки сразу возвращаем ее в main

    printf("You have entered:\n"); // предоставление пользователю введенных им значений
    printf("a = %lf, b = %lf, c = %lf\n", a, b, c);

    double x1 = 0, x2 = 0;
    enum answer ans_code = UNKNOWN;
    ans_code = solving(a, b, c, &x1, &x2);

    if (ans_code >= QUAD_0SOL && ans_code <= LIN_ENDLESS){
        // ans_code из этих значений соотвестует коду решенного уравнения, а не ошибки
        err_code = result_printing(ans_code, x1, x2);
        return err_code;
    }
    return ans_code;
}

int get_coefficients(double *a, double *b, double *c){

    // проверка на ошибки в аргументах
    if ((a == NULL) || (b == NULL) || (c == NULL) || (a == b) || (b == c) || (a == c)) {
        if (a == NULL || b == NULL || c == NULL)
            return NULL_ADDRESS;
        else
            return SAME_ADDRESS;
    }
    // окончание проверки

    printf("Enter coefficient connected to x*x:\n");
    while (scanf("%lf", a) != 1) //TODO read about scanf
    {
        while (getchar() != '\n')
            ;
        printf("Enter one number\n");
    }
    while (getchar() != '\n')
        ;

    printf("Enter coefficient connected to x:\n");
    while (scanf("%lf", b) != 1) {
        while (getchar() != '\n')
            ;
        printf("Enter one number\n");
    }
    while (getchar() != '\n')
        ;

    printf("Enter coefficient unconnected to x:\n");
    while (scanf("%lf", c) != 1){
        while (getchar() != '\n')
            ;
        printf("Enter one number\n");
    }
    while (getchar() != '\n')
        ;

    return 0;
}

int result_printing(int switch_code, double x1, double x2){

    // выполняем проверку отправленных аргументов
    if (isfinite(x1) == 0) {
        return WRONG_X1_PRINTING;
    }
    if (isfinite(x2) == 0){
        return WRONG_X2_PRINTING;
    }
    // окончание проверки

    switch(switch_code){

        case QUAD_2SOL: //                     квадратное,2 корня
            printf("You have 2 solutions:\n");
            printf("x1 = %lf \nx2 = %lf \n", x1, x2);
            break;

        case QUAD_1SOL: //                    квадратное, 1 корень
            printf("You have the only solution: x = %lf\n", x1);
            break;

        case QUAD_0SOL: //                    квадратное, нет корней
            printf("This equation has no real solutions\n");
            break;

        case LIN_ENDLESS: //                  бесконечное кол-во решений при a = b = c = 0
            printf("You entered every coefficient as a 0 and this equation has endless amount of solutions\n");
            break;

        case LIN_1SOL: //                     линейное, 1 решение
            printf("This isn't a quadratic equation,but the solution is %lf\n", x1);
            break;

        case LIN_0SOL: //                     линейное, нет решений
            printf("This linear equation has no real solutions\n");
            break;

        default:
            printf("Mistake in solving function");
            break;
    }

    return 0;
}

//======================================================================================================================
int test_solving(){

    printf("The test is running\n\n");
    //! входные данные
    struct test_arr input[TESTS_NUMBER] = {
            {.a = 0,  .b = 0,   .c =  0},
            {.a = 0,  .b = 8,   .c =  9},
            {.a = 0,  .b = 0,   .c =  9},
            {.a = 2,  .b = 4,   .c =  2},
            {.a = 5,  .b = 3,   .c =  1},
            {.a = 2,  .b = -24, .c = 70},
            {.a = -5, .b = 15,  .c =  0},
            {.a = 0,  .b = 5,   .c =  2}
    };

    //! правильные ответы на тесты: х1, х2 и код ответа
    const struct exp_ans correct_answers[TESTS_NUMBER] = {
            {.x1 = 0,      .x2 = 0, .ans_code = LIN_ENDLESS},
            {.x1 = -1.125, .x2 = 0, .ans_code = LIN_1SOL},
            {.x1 = 0,      .x2 = 0, .ans_code = LIN_0SOL},
            {.x1 = -1,     .x2 = 0, .ans_code = QUAD_1SOL},
            {.x1 = 0,      .x2 = 0, .ans_code = QUAD_0SOL},
            {.x1 = 7,      .x2 = 5, .ans_code = QUAD_2SOL},
            {.x1 = 0,      .x2 = 3, .ans_code = QUAD_2SOL},
            {.x1 = -0.4,   .x2 = 0, .ans_code = LIN_1SOL}
    };

    int ans_code = 0;

    for (int i = 0; i < TESTS_NUMBER; i++){

        //инициализация структуры для хранения ответов
        struct get_ans prog_answers = {
                .x1 = 0, .x2 = 0, .ans_code = UNKNOWN
        };
        // получение практических ответов
        prog_answers.ans_code = solving(input[i].a, input[i].b, input[i].c, &prog_answers.x1, &prog_answers.x2);

        // сравнение полученных значений и правильных
        if (equal_with_error(prog_answers.x1, correct_answers[i].x1, ERROR_RATE) == 0 ||
            equal_with_error(prog_answers.x2, correct_answers[i].x2, ERROR_RATE) == 0 ||
            prog_answers.ans_code != correct_answers[i].ans_code) {

            ans_code = UNSUCCESS_TEST; // ставим значения ошибки, если она обнаружена

            // вывод данных, при которых произошла ошибка
            printf("Line #%d has not passed the test\n", i);
            printf("Coefficients: %lf, %lf, %lf\n",            input[i].a,            input[i].b,            input[i].c);
            printf("Expected correct_answers: %lf, %lf, %d\n", correct_answers[i].x1, correct_answers[i].x2, correct_answers[i].ans_code);
            printf("Received correct_answers: %lf, %lf, %d\n", prog_answers.x1,    prog_answers.x2,    prog_answers.ans_code);
        }

    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (ans_code == 0)
        printf("Test was successful\n");

    return ans_code;
}

//======================================================================================================================
int solving(double a, double b, double c, double *x1, double *x2){

    // проверка правильности введенных данных
    if (isfinite(a) == 0){
        printf("first coefficient sent incorrect\n");
        return WRONG_SOLVING_A;
    }

    if (isfinite(b) == 0){
        printf("second coefficient sent incorrect\n");
        return WRONG_SOLVING_B;
    }

    if (isfinite(c) == 0){
        printf("third coefficient sent incorrect\n");
        return WRONG_SOLVING_C;
    }

    if (x1 == x2 || x1 == NULL || x2 == NULL){
        printf("Incorrect address in solving\n");
        return WRONG_SOLVING_X1X2;
    }
    // проверка окончена, если она успешна, переходим к вычислению


    int ans_code = 0;

    if (equal_with_error(a, 0, ERROR_RATE) == 0)
        // вызывает решение квадратного уравнения
        ans_code = solving_quadratic(a, b, c, x1, x2);
    else
        // вызывает решение линейного уравнения
        ans_code = solving_lin(b, c, x1);

    return  ans_code;
}

static int solving_quadratic(double a, double b, double c, double *x1, double *x2){
    // проверка
    assert(isfinite(a));

    assert(isfinite(b));

    assert(isfinite(c));

    assert(x1 != NULL);

    assert(x2 != NULL);
    // конец проверки

    int ans_code = 0;
    double disc = b * b - (4 * a * c);

    if (disc >= 0)
        if (equal_with_error(disc, 0, ERROR_RATE) == 0) {
            disc = sqrt(disc);
            *x1 = (disc - b) / (2 * a);
            *x2 = (-disc - b) / (2 * a);
            ans_code = QUAD_2SOL;
        }
        else{
            *x1 = (-1)*b / (2 * a);
            ans_code = QUAD_1SOL;
        }

    else
        ans_code = QUAD_0SOL;

    return ans_code;
}

static int solving_lin(double b, double c,double *x){
    // проверка
    assert(isfinite(b));

    assert(isfinite(c));

    assert(x != NULL);
    // конец проверки

    int ans_code = 0;

    // проверка, равен ли линейный коэффициент нулю
    if (equal_with_error(0, b, ERROR_RATE)) {
        if (equal_with_error(0, c, ERROR_RATE))
            ans_code = LIN_ENDLESS;
        else
            ans_code = LIN_0SOL;
    }

    else {
        *x = (-1) * c / b;
        ans_code = LIN_1SOL;
    }

    return ans_code;
}

//======================================================================================================================
int equal_with_error(double a, double b, double err_rate){

    // проверка переданных значений
    assert(isfinite(a));

    assert(isfinite(b));

    assert(isfinite(err_rate));
    // окончание проверки

    return (fabs(a-b) <= err_rate);
}

//======================================================================================================================
void print_error(int error_code){

    switch(error_code)
    {

        // ошибки из solving функции
        case WRONG_SOLVING_A:
            printf("first coefficient wasn't send to solving() correctly\n");
            break;

        case WRONG_SOLVING_B:
            printf("second coefficient wasn't send to solving() correctly\n");
            break;

        case WRONG_SOLVING_C:
            printf("third coefficient wasn't send to solving() correctly\n");
            break;

        case WRONG_SOLVING_X1X2:
            printf("Answer "
                   "pointers weren't send to solving() correctly\n");
            break;

        // ошибки в get_coefficients
        case NULL_ADDRESS:
            printf("NULL addresses in get_coefficients\n");
            break;

        case SAME_ADDRESS:
            printf("Same addresses in get_coefficients\n");
            break;

        // ошибки в тестовой функциии
        case UNSUCCESS_TEST:
            printf("Programme was in test mode and it didn't work\n");
            break;

        // ошибки из printing_result()
        case WRONG_X1_PRINTING:
            printf("Incorrect value as x1 in printing_result()\n");
            break;

        case WRONG_X2_PRINTING:
            printf("Incorrect value as x2 in printing_result()\n");
            break;

        default:
        // неизвестная ошибка
            printf("Your value of error_code: %d\n", error_code);
            printf("Check your main function\n");
            assert(0);
    }
}
