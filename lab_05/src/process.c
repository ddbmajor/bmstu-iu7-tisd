#include "process.h"


float random_float(float min, float max)
{
    if (min >= max)
        return min;
    
    return (float)rand() / RAND_MAX * (max - min) + min;
}

float max(float a, float b, float c)
{
    if (a > b)
        if (a > c)
            return a;
        else
            return c;
    else
        if (b > c)
            return b;
        else
            return c;
}


float min(float a, float b, float c)
{
    if (a < b)
        if (a < c)
            return a;
        else
            return c;
    else
        if (b < c)
            return b;
        else
            return c;
}


void simulate_arr()
{
    extern float q1_f_s, q1_f_f, q2_f_s, q2_f_f;
    extern float q1_w_s, q1_w_f, q2_w_s, q2_w_f;
    float fetch_time1 = 0;
    float fetch_time2 = 0;

    float work_time = 0;
    float hold_time = 0;

    float time_until_fetch1 = 0;
    float time_until_fetch2 = 0;
    float time_until_work = 0;

    arr_queue_t *queue1 = create_arr_queue();
    arr_queue_t *queue2 = create_arr_queue();
    sim_log_t log1 = { 0 };
    sim_log_t log2 = { 0 };


    while (log1.tasks_out != 1000)
    {
        if (time_until_fetch1 <= 0)
        {
            time_until_fetch1 = random_float(q1_f_s, q1_f_f);
            task_t *task = create_task();
            int rc = push_arr_queue(queue1, task);
            if (rc == OVERFLOW_ERROR)
            {
                log1.tasks_failed += 1;
                delete_task(task);
            }
            else if (rc == 0)
            {
                log1.tasks_in += 1;
            }
            fetch_time1 += time_until_fetch1;
        }
        if (time_until_fetch2 <= 0)
        {
            time_until_fetch2 = random_float(q2_f_s, q2_f_f);
            task_t *task = create_task();
            int rc = push_arr_queue(queue2, task);
            if (rc == OVERFLOW_ERROR)
            {
                log2.tasks_failed += 1;
                delete_task(task);
            }
            else if (rc == 0)
            {
                log2.tasks_in += 1;
            }
            fetch_time2 += time_until_fetch2;
        }

        if (time_until_work <= 0)
        {
            if (queue1->amount == 0 && queue2->amount == 0)
                hold_time += fmax(time_until_fetch1, time_until_fetch2);
            else
            {
                if (queue1->amount != 0)
                {
                    task_t *task;
                    task = pop_arr_queue(queue1);

                    time_until_work = random_float(q1_w_s, q1_w_f);

                    work_time += time_until_work;
                    log1.calls += 1;
                    log1.tasks_out += 1;
                    log1.overall_len += queue1->amount;

                    delete_task(task);
                }
                else if (queue2->amount != 0)
                {
                    task_t *task;
                    task = pop_arr_queue(queue2);

                    time_until_work = random_float(q2_w_s, q2_w_f);

                    work_time += time_until_work;
                    log2.calls += 1;
                    log2.tasks_out += 1;
                    log2.overall_len += queue2->amount;

                    delete_task(task);
                }

                if (log1.tasks_out % 100 == 0)
                {
                    printf("Вышедшие заявки 1: %d\n", log1.tasks_out);
                    printf("Вошедшие завяки 1: %d\n", log1.tasks_in);
                    // printf("%d\n", log.calls);
                    printf("Неудачные заявки 1: %d\n", log1.tasks_failed);
                    printf("Текущая длина очереди 1: %d\n", queue1->amount);
                    printf("Средняя длина очереди 1: %d\n", log1.tasks_out != 0 ? log1.overall_len / log1.tasks_out : log1.tasks_in);
                    printf("Среднее время ожидания в очереди 1: %f\n", log1.tasks_out != 0 ? \
                                        (log1.overall_len / log1.tasks_out) * (fetch_time1 / log1.tasks_in) \
                                        : log1.tasks_in * (fetch_time1 / log1.tasks_in));
                    printf("\n");

                    printf("Вышедшие заявки 2: %d\n", log2.tasks_out);
                    printf("Вошедшие завяки 2: %d\n", log2.tasks_in);
                    // printf("%d\n", log.calls);
                    printf("Неудачные заявки 2: %d\n", log2.tasks_failed);
                    printf("Текущая длина очереди 2: %d\n", queue2->amount);
                    printf("Средняя длина очереди 2: %d\n", log2.tasks_out != 0 ? log2.overall_len / log2.tasks_out : log2.tasks_in);
                    printf("Среднее время ожидания в очереди 2: %f\n", log2.tasks_out != 0 ? \
                                        (log2.overall_len / log2.tasks_out) * (fetch_time2 / log2.tasks_in) \
                                        : log2.tasks_in * (fetch_time2 / log2.tasks_in));
                    printf("\n");      
                }
            }
        }

        float smallest_time = 0;
        if (time_until_work != 0 && time_until_fetch1 != 0 && time_until_fetch2 != 0)
            smallest_time = min(time_until_work, time_until_fetch1, time_until_fetch2);
        else
            smallest_time = max(time_until_work, time_until_fetch1, time_until_fetch2);
        time_until_work -= smallest_time;
        time_until_fetch1 -= smallest_time;
        time_until_fetch2 -= smallest_time;
    }

    printf("Время ожидания: %f\n", hold_time);
    printf("Время работы: %f\n", work_time);
    printf("Общее время моделирования : %f\n", hold_time + work_time);

    float expected_time = (q1_f_f + q1_f_s) > (q1_w_f + q1_w_s) ? ((float)(q1_f_f + q1_f_s) / 2.0) * 1000 : ((float)(q1_w_f + q1_w_s) / 2.0) * 1000;

    printf("Ожидаемое теоритическое время: %f\n", expected_time);
    printf("Погрешность: %3.2f%%\n", fabs((hold_time + work_time - expected_time) / expected_time * 100));

    delete_arr_queue(queue1);
    delete_arr_queue(queue2);
}


void simulate_list()
{
    extern float q1_f_s, q1_f_f, q2_f_s, q2_f_f;
    extern float q1_w_s, q1_w_f, q2_w_s, q2_w_f;
    float fetch_time1 = 0;
    float fetch_time2 = 0;

    float work_time = 0;
    float hold_time = 0;

    float time_until_fetch1 = 0;
    float time_until_fetch2 = 0;
    float time_until_work = 0;

    list_queue_t *queue1 = create_list_queue();
    list_queue_t *queue2 = create_list_queue();
    sim_log_t log1 = { 0 };
    sim_log_t log2 = { 0 };


    while (log1.tasks_out != 1000)
    {
        if (time_until_fetch1 <= 0)
        {
            time_until_fetch1 = random_float(q1_f_s, q1_f_f);
            task_t *task = create_task();
            int rc = push_list_queue(queue1, task);
            if (rc == OVERFLOW_ERROR)
            {
                log1.tasks_failed += 1;
                delete_task(task);
            }
            else if (rc == 0)
            {
                log1.tasks_in += 1;
            }
            fetch_time1 += time_until_fetch1;
        }
        if (time_until_fetch2 <= 0)
        {
            time_until_fetch2 = random_float(q2_f_s, q2_f_f);
            task_t *task = create_task();
            int rc = push_list_queue(queue2, task);
            if (rc == OVERFLOW_ERROR)
            {
                log2.tasks_failed += 1;
                delete_task(task);
            }
            else if (rc == 0)
            {
                log2.tasks_in += 1;
            }
            fetch_time2 += time_until_fetch2;
        }

        if (time_until_work <= 0)
        {
            if (queue1->amount == 0 && queue2->amount == 0)
                hold_time += fmax(time_until_fetch1, time_until_fetch2);
            else
            {
                if (queue1->amount != 0)
                {
                    task_t *task;
                    task = pop_list_queue(queue1);

                    time_until_work = random_float(q1_w_s, q1_w_f);

                    work_time += time_until_work;
                    log1.calls += 1;
                    log1.tasks_out += 1;
                    log1.overall_len += queue1->amount;

                    delete_task(task);
                }
                else if (queue2->amount != 0)
                {
                    task_t *task;
                    task = pop_list_queue(queue2);

                    time_until_work = random_float(q2_w_s, q2_w_f);

                    work_time += time_until_work;
                    log2.calls += 1;
                    log2.tasks_out += 1;
                    log2.overall_len += queue2->amount;

                    delete_task(task);
                }

                if (log1.tasks_out % 100 == 0)
                {
                    printf("Вышедшие заявки 1: %d\n", log1.tasks_out);
                    printf("Вошедшие завяки 1: %d\n", log1.tasks_in);
                    // printf("%d\n", log.calls);
                    printf("Неудачные заявки 1: %d\n", log1.tasks_failed);
                    printf("Текущая длина очереди 1: %d\n", queue1->amount);
                    printf("Средняя длина очереди 1: %d\n", log1.tasks_out != 0 ? log1.overall_len / log1.tasks_out : log1.tasks_in);
                    printf("Среднее время ожидания в очереди 1: %f\n", log1.tasks_out != 0 ? \
                                        (log1.overall_len / log1.tasks_out) * (fetch_time1 / log1.tasks_in) \
                                        : log1.tasks_in * (fetch_time1 / log1.tasks_in));
                    printf("\n");

                    printf("Вышедшие заявки 2: %d\n", log2.tasks_out);
                    printf("Вошедшие завяки 2: %d\n", log2.tasks_in);
                    // printf("%d\n", log.calls);
                    printf("Неудачные заявки 2: %d\n", log2.tasks_failed);
                    printf("Текущая длина очереди 2: %d\n", queue2->amount);
                    printf("Средняя длина очереди 2: %d\n", log2.tasks_out != 0 ? log2.overall_len / log2.tasks_out : log2.tasks_in);
                    printf("Среднее время ожидания в очереди 2: %f\n", log2.tasks_out != 0 ? \
                                        (log2.overall_len / log2.tasks_out) * (fetch_time2 / log2.tasks_in) \
                                        : log2.tasks_in * (fetch_time2 / log2.tasks_in));
                    printf("\n");      
                }
            }
        }

        float smallest_time = 0;
        if (time_until_work != 0 && time_until_fetch1 != 0 && time_until_fetch2 != 0)
            smallest_time = min(time_until_work, time_until_fetch1, time_until_fetch2);
        else
            smallest_time = max(time_until_work, time_until_fetch1, time_until_fetch2);
        time_until_work -= smallest_time;
        time_until_fetch1 -= smallest_time;
        time_until_fetch2 -= smallest_time;
    }

    printf("Время ожидания: %f\n", hold_time);
    printf("Время работы: %f\n", work_time);
    printf("Общее время моделирования : %f\n", hold_time + work_time);

    float expected_time = (q1_f_f + q1_f_s) > (q1_w_f + q1_w_s) ? ((float)(q1_f_f + q1_f_s) / 2.0) * 1000 : ((float)(q1_w_f + q1_w_s) / 2.0) * 1000;

    printf("Ожидаемое теоритическое время: %f\n", expected_time);
    printf("Погрешность: %3.2f%%\n", fabs((hold_time + work_time - expected_time) / expected_time * 100));

    delete_list_queue(queue1);
    delete_list_queue(queue2);
}


void make_and_del_arr()
{
    arr_queue_t *queue = create_arr_queue();
    while (queue->amount != MAXLEN)
    {
        task_t *task = create_task();
        push_arr_queue(queue, task);
    }
    while (queue->amount != 0)
    {
        task_t *task = pop_arr_queue(queue);
        delete_task(task);
    }
    delete_arr_queue(queue);
}


void make_and_del_list()
{
    list_queue_t *queue = create_list_queue();
    while (queue->amount != MAXLEN)
    {
        task_t *task = create_task();
        push_list_queue(queue, task);
    }
    while (queue->amount != 0)
    {
        task_t *task = pop_list_queue(queue);
        delete_task(task);
    }
    delete_list_queue(queue);
}


void efficiency()
{
    int64_t sum_simple, sum = 0;;
	struct timeval tv_start, tv_stop;
    int count = 50;

    for (int i = 0; i < count; i++)
    {
        gettimeofday(&tv_start, NULL);
        make_and_del_arr();
        gettimeofday(&tv_stop, NULL);
        sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        sum += sum_simple;
    }
    printf("Время для массива (мс): %" PRId64 "\n", sum / count);
    printf("Память для массива (байт): %ld\n", sizeof(task_t) * MAXLEN + sizeof(int) * 5);

    sum = 0;
    for (int i = 0; i < count; i++)
    {
        gettimeofday(&tv_start, NULL);
        make_and_del_list();
        gettimeofday(&tv_stop, NULL);
        sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        sum += sum_simple;
    }
    printf("Время для списка (мс): %" PRId64 "\n", sum / count);
    printf("Память для списка (байт): %ld\n", sizeof(node_t) * MAXLEN + sizeof(int) * 2);
}


void hand_mode()
{
    int choice;
    int rc;
    char *menu = "1 - добавить заявку в массив-очередь\n2 - добавить заявку в список-очередь\n"
    "3 - удалить заявку из массива-очереди\n4 - удалить заявку из списка-очереди\n"
    "5 - текущее состояние очередей\n0 - выйти из меню\n";

    arr_queue_t *arr_queue = create_arr_queue();
    list_queue_t *list_queue = create_list_queue();

    while (1)
    {
        printf("%s", menu);
        printf("Введите пункт меню:\n");
        rc = scanf("%d", &choice);
        if (rc != 1)
        {
            printf("Неверный пункт меню\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        switch (choice)
        {
            case 1:
            {
                task_t *task = create_task();
                rc = push_arr_queue(arr_queue, task);
                if (rc != 0)
                {
                    if (rc == OVERFLOW_ERROR)
                    {
                        printf("Очередь переполнена!\n");
                    }
                    else if (rc == ALLOC_ERROR)
                    {
                        printf("Ошибка выделения памяти\n");
                        return;
                    }
                }
                else
                {
                    printf("Адрес добавленного элемента: %p\n", (void *)(arr_queue->data + arr_queue->amount - 1));
                }
                break;
            }
            case 2:
            {
                task_t *task = create_task();
                rc = push_list_queue(list_queue, task);
                if (rc != 0)
                {
                    if (rc == OVERFLOW_ERROR)
                    {
                        printf("Очередь переполнена!\n");
                    }
                    else if (rc == ALLOC_ERROR)
                    {
                        printf("Ошибка выделения памяти\n");
                        return;
                    }
                }
                else
                {
                    printf("Адрес добавленного элемента: %p\n", (void *)list_queue->Pin->task);
                }
                break;
            }
            case 3:
            {
                task_t *task = pop_arr_queue(arr_queue);
                if (task == NULL)
                {
                    printf("Очередь пуста!\n");
                }
                else
                {
                    printf("Адрес удаленного элемента: %p\n", (void *)(arr_queue->data + arr_queue->amount));
                    delete_task(task);
                }
                break;
            }
            case 4:
            {
                task_t *task = pop_list_queue(list_queue);
                if (task == NULL)
                {
                    printf("Очередь пуста!\n");
                }
                else
                {
                    printf("Адрес удаленного элемента: %p\n", (void *)task);
                }
                break;
            }
            case 5:
            {
                printf("Очередь массивом:\n");
                print_arr_queue(arr_queue);
                printf("Очередь списоком:\n");
                print_list_queue(list_queue);
                break;
            }
            case 0:
            {
                delete_arr_queue(arr_queue);
                delete_list_queue(list_queue);
                return;
            }
            default:
                printf("Неверный пункт меню\n");
                break;
        }
    }

}