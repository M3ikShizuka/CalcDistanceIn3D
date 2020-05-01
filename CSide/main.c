#include "main.h"
#include "entity.h"
#include <math.h>
#include <Windows.h>
#include <conio.h>

// Global time vars
double g_d_time_program_init_data = 0.0,
g_d_time_program_processing_data = 0.0,
g_d_time_program_free_data = 0.0,
g_d_time_program_work_full = 0.0;

void init_entities(struct s_entity** pp_entity_list, struct s_entity** pp_my_entity);
void process_data(const struct s_entity* p_entity_list, const struct s_entity* p_my_entity);
const float calc_dist(const struct s_entity* p_entity1, const struct s_entity* p_entity2);
void free_entities(struct s_entity** pp_entity_list, struct s_entity** pp_my_entity);
const double calc_time(const unsigned long long ull_start_tick, const unsigned long long ull_end_tick);
void print_work_info();

int main()
{
    struct s_entity* p_entity_list = NULL,
        * p_my_entity = NULL;
    unsigned long long ull_start_tick = 0,
        ull_end_tick = 0;

    ull_start_tick = GetTickCount64();

    init_entities(&p_entity_list, &p_my_entity);
    process_data(p_entity_list, p_my_entity);
    free_entities(&p_entity_list, &p_my_entity);

    ull_end_tick = GetTickCount64();
    g_d_time_program_work_full = calc_time(ull_start_tick, ull_end_tick);

    print_work_info();

    _getch();
    return 0;
}

void init_entities(struct s_entity** pp_entity_list, struct s_entity** pp_my_entity)
{
    unsigned long long ull_start_tick = 0,
        ull_end_tick = 0;
    ull_start_tick = GetTickCount64();

    // Init my entity.
    *pp_my_entity = (struct s_entity*) malloc(sizeof(struct s_entity));
    (*(struct s_entity**)pp_my_entity)->pos.x = 1.0f;
    (*(struct s_entity**)pp_my_entity)->pos.y = 2.0f;
    (*(struct s_entity**)pp_my_entity)->pos.z = 3.0f;

#ifdef DATA_LIST
    // Init list of entities.
    *pp_entity_list = (struct s_entity*) malloc(sizeof(struct s_entity) * ELEMENT_SIZE);

    for (int i_index = 0; i_index < ELEMENT_SIZE; i_index++)
    {
        struct s_entity* p_entity = &(*pp_entity_list)[i_index];
        p_entity->pos.x = 31.0f;
        p_entity->pos.y = 13.0f;
        p_entity->pos.z = 33.0f;
    }
#else
    // Init list of pointers on entities.
    *pp_entity_list = (struct s_entity**) malloc(sizeof(struct s_entity*) * ELEMENT_SIZE);

    for (int i_index = 0; i_index < ELEMENT_SIZE; i_index++)
    {
        struct s_entity* p_entity = s_entity(31.0f, 13.0f, 33.0f);
        ((*(struct s_entity***)pp_entity_list)[i_index]) = p_entity;
    }
#endif

    ull_end_tick = GetTickCount64();
    g_d_time_program_init_data = calc_time(ull_start_tick, ull_end_tick);
}

void process_data(const struct s_entity* p_entity_list, const struct s_entity* p_my_entity)
{
    unsigned long long ull_start_tick = 0,
        ull_end_tick = 0;

    ull_start_tick = GetTickCount64();

    // Alloc storage.
    float *p_f_dist = (float*) malloc(sizeof(float) * ELEMENT_SIZE);

    int i_index = 0;
    for (;i_index < ELEMENT_SIZE; i_index++)
    {
#ifdef DATA_LIST
        // Process list of entities.
        struct s_entity* p_entity = &p_entity_list[i_index];
        p_f_dist[i_index] = calc_dist(p_my_entity, p_entity);
#else
        // Process list of pointers on entities.
        struct s_entity* p_entity = ((struct s_entity**)p_entity_list)[i_index];
        p_f_dist[i_index] = calc_dist(p_my_entity, p_entity);
#endif
    }

    ull_end_tick = GetTickCount64();
    g_d_time_program_processing_data = calc_time(ull_start_tick, ull_end_tick);

    // Free storage.
    if (p_f_dist)
    {
        free(p_f_dist);
        p_f_dist = NULL;
    }
}

const float calc_dist(const struct s_entity* p_entity1, const struct s_entity* p_entity2)
{
    // Сalculate the distance between points in a three-dimensional coordinate system.
    // dist = ((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)^1/2
    const float f_exponent = 2.0f;

    return sqrtf(
        powf((p_entity2->pos.x - p_entity1->pos.x), f_exponent) +
        powf((p_entity2->pos.y - p_entity1->pos.y), f_exponent) +
        powf((p_entity2->pos.z - p_entity1->pos.z), f_exponent)
    );
}

void free_entities(struct s_entity** pp_entity_list, struct s_entity** pp_my_entity)
{
    unsigned long long ull_start_tick = 0,
        ull_end_tick = 0;

    ull_start_tick = GetTickCount64();

    // Free allocated memory for entities.
    if (*pp_entity_list)
    {
#ifndef DATA_LIST
        // Free pointers of list.
        for (int i_index = 0; i_index < ELEMENT_SIZE; i_index++)
        {
            struct s_entity* p_entity = ((*(struct s_entity***)pp_entity_list)[i_index]);
            free(p_entity);
            p_entity = NULL;
        }
#endif
        // Free list.
        free(*pp_entity_list);
        *pp_entity_list = NULL;
    }

    if (*pp_my_entity)
    {
        free(*pp_my_entity);
        *pp_my_entity = NULL;
    }

    ull_end_tick = GetTickCount64();
    g_d_time_program_free_data = calc_time(ull_start_tick, ull_end_tick);
}

const double calc_time(const unsigned long long ull_start_tick, const unsigned long long ull_end_tick)
{
    return (double)(ull_end_tick - ull_start_tick) / 1000.0;
}

void print_work_info()
{
    printf(
#ifdef DATA_LIST
        "List of entities\n"
#else
        "List of pointers on entities\n"
#endif
        "Data amount: %d\n\
Data initiation:% lf sec\n\
Data processing:% lf sec\n\
Data release:% lf sec\n\
Total run time:% lf sec\n",
        ELEMENT_SIZE,
        g_d_time_program_init_data,
        g_d_time_program_processing_data,
        g_d_time_program_free_data,
        g_d_time_program_work_full
    );
}