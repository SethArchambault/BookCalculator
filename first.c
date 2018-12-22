/*
 * [ ] See my daily stats (pages per minute, done date, note about my energy level)
 * [ ] See my weekly stats
 * [ ] See my monthly stats
 * [ ] See a graph of how things change
 * [ ] Note where chapters begin
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define assert(expr) if(!(expr)) { printf("line %d: %s\n", __LINE__, #expr); __builtin_trap();} 
#define MONTH_OFFSET 1
#define YEAR_OFFSET 1900 

int main() {

    // :input
    int global_total_pages;
    float global_reading_hours_per_week;
    int global_starting_page;
    float global_reading_session_hours;
    int *global_progress; 
    int global_progress_length;
    {
        int total_pages                 = 199;
        float reading_hours_per_week    =  10;
        int starting_page               =  39;
        float reading_session_hours = 2.25;
        int progress[] = {
            39, 0, 31,
            40, 31, 41,
            41, 41, 51,
            42, 0, 11,
            43, 11, 20,
            44, 20, 33,
            45, 33, 43,
            46, 43, 52,
            47, 52, 59,
            48, 59, 67,
            49, 0, 5,
            50, 5, 11,
            51, 11, 16,
            52, 0, 5,
            53, 5, 13,
            54, 13, 19, 
            55, 19, 24,
            56, 0,  9,
            57, 9, 13,
            58, 0, 8,
            59, 0, 42,
            60, 0, 12,
            61, 0, 12,
            62, 12, 18,
            63, 18, 24,
            64, 0, 4,
            65, 4, 9,
            66, 0, 11,
            67, 11, 17,
            68, 17, 28,    
            69, 0, 6,
            70, 6, 11,
            71, 0, 9,
            72, 0, 6,
            73, 0, 6,
            74, 0, 5,
            75, 0, 5,
            76, 0, 9,
            77, 9, 16,
            78, 0, 4,
            79, 0, 11,
            80, 0, 8,
            81, 0, 10,
            82, 0, 10,
            83, 0, 7,
            84, 0, 9,
            85, 0, 7,
            86, 0, 6,
            87, 0, 4,
            88, 0, 5,
            89, 0, 7,
            91, 0, 10, // day after birthday
            92, 0, 11,
            93, 11, 20,
            94, 0, 17,
            95, 0, 15,
            96, 0, 5,
            97, 0, 14,
            98, 0, 6,
            99, 0, 12,
            100, 0, 6,
            101, 0, 4,
            102, 0, 3,
            103, 0, 8,
            104, 0, 6

        };
        int progress_length = sizeof(progress) / sizeof(int);


        global_progress_length          = progress_length;
        global_total_pages              = total_pages;
        global_reading_hours_per_week   = reading_hours_per_week;                
        global_starting_page            = starting_page;           
        global_reading_session_hours    = reading_session_hours;                   
        global_progress                = malloc(sizeof(progress)); 
        global_progress                = progress;       
    }

    // :calculate avg, min, max
    int global_pages_read;
    int global_reading_minutes;
    int global_current_page;
    int global_max_minutes;
    int global_min_minutes;
    float global_avg_minutes_per_page;
    {
        int * progress      = global_progress;
        int starting_page   = global_starting_page;
        int progress_length = global_progress_length;

        assert(progress_length > 0);
        int current_page                    =  progress[progress_length - 3];
        assert(current_page > 0);
        int pages_read                      =  current_page - starting_page;
        int reading_minutes                 = 0;
        int max_minutes = 0;
        int min_minutes = 1000;
        for (int progress_index = 0; 
                 progress_index < progress_length; 
                 progress_index += 3) {
            if (progress[progress_index] < starting_page) continue;
            int start_time_index = progress_index + 1;
            int end_time_index = progress_index + 2;
            int time_took      = progress[end_time_index] - progress[start_time_index];
            reading_minutes   += time_took;
            if (time_took > max_minutes) max_minutes = time_took;
            if (time_took < min_minutes) min_minutes = time_took;
        }
        assert (reading_minutes > 0);
        float avg_minutes_per_page      =       reading_minutes / pages_read;

        global_progress_length           = progress_length;
        global_avg_minutes_per_page      = avg_minutes_per_page;
        global_pages_read                = pages_read;
        global_reading_minutes           = reading_minutes;
        global_current_page              = current_page;
        global_max_minutes               = max_minutes;
        global_min_minutes               = min_minutes;
    }

    // :calculate standard deviation
    float global_std_div;
    {
        int progress_length      = global_progress_length;
        int *progress            = global_progress;
        int starting_page        = global_starting_page;
        float avg_minutes_per_page = global_avg_minutes_per_page;
        int pages_read           = global_pages_read;

        float std_div =0;
        for (int progress_index = 0; 
                 progress_index < progress_length; 
                 progress_index += 3) {
            if (progress[progress_index] < starting_page) continue;
            int start_time_index = progress_index + 1;
            int end_time_index = progress_index + 2;
            int time_took = progress[end_time_index] - progress[start_time_index];
            int avg_dt = time_took - avg_minutes_per_page;
            std_div += avg_dt * avg_dt;
        }
        std_div = std_div / pages_read; 
        std_div = sqrt(std_div);

        global_std_div = std_div;
    }

    // :pages left, hours remaining
    int global_pages_left;
    int global_hours_remaining;
    {
        int total_pages             = global_total_pages;
        int current_page            = global_current_page;
        float avg_minutes_per_page    = global_avg_minutes_per_page;

        int pages_left              = total_pages - current_page;
        int hours_remaining         = pages_left  * (avg_minutes_per_page / 60);

        global_pages_left        = pages_left;
        global_hours_remaining   = hours_remaining;
    }


    // :generated done date, desired date
    struct tm global_todays_date;
    struct tm global_done_date;
    struct tm global_desired_date;
    float global_weeks_remaining;
    {
        int hours_remaining             = global_hours_remaining;
        int reading_hours_per_week      = global_reading_hours_per_week;

        time_t current_time             = time(NULL);
        struct tm todays_date           = *localtime(&current_time);
        struct tm done_date             = *localtime(&current_time);

        float weeks_remaining           = (float)hours_remaining   / reading_hours_per_week;
        done_date.tm_mday              += weeks_remaining * 7;
        mktime(&done_date);

        struct tm desired_date          = *localtime(&current_time);
        int desired_month               = 2;
        int desired_day                 = 1;
        int desired_year                = 2019;
        desired_date.tm_mon             = desired_month - MONTH_OFFSET;
        desired_date.tm_mday            = desired_day;
        desired_date.tm_year            = desired_year - YEAR_OFFSET;
        mktime(&desired_date);

        global_todays_date               = todays_date;
        global_desired_date              = desired_date;
        global_done_date                 = done_date;
        global_weeks_remaining           = weeks_remaining;
    }


    // :print
    {
        int pages_read                = global_pages_read;
        int reading_minutes           = global_reading_minutes;
        float avg_minutes_per_page    = global_avg_minutes_per_page;
        int max_minutes               = global_max_minutes;
        int min_minutes               = global_min_minutes;
        float std_div                 = global_std_div;
        int pages_left                = global_pages_left;
        int hours_remaining           = global_hours_remaining;
        float reading_hours_per_week  = global_reading_hours_per_week;
        float reading_session_hours   = global_reading_session_hours;
        float weeks_remaining         = global_weeks_remaining;
        struct tm todays_date         = global_todays_date;
        struct tm done_date           = global_done_date;
        struct tm desired_date        = global_desired_date;
        

        printf("\n\n");
        printf("I have read:                    %5d pages\n\n", pages_read);
        printf("Hours it took:                  %5.2f hours\n\n", reading_minutes / 60.0f);
        printf("Each page takes:                %5.2f minutes\n\n", avg_minutes_per_page);
        printf("Pages per hour:                 %5.2f pages\n\n", 60.0f / avg_minutes_per_page);
        printf("Maximum time:                   %5d minutes\n\n", max_minutes);
        printf("Minimum time:                   %5d minutes\n\n", min_minutes);
        printf("Standard Deviation:             %5.2f Minutes\n\n", std_div);
        printf("Remaining pages:                %5d pages\n\n", pages_left);
        printf("Remaining hours:                %5d hours\n\n", hours_remaining);
        printf("Every week I read for:          %5.2f hours\n\n", reading_hours_per_week);
        printf("Every reading session takes:    %5.2f hours\n\n",reading_session_hours);
        printf("Reading sessions remaining:     %5.1f Sessions\n\n",hours_remaining / reading_session_hours);
        printf("Remaining weeks:                %5.2f weeks\n\n", weeks_remaining);
        printf("Todays Date:                    %d/%d/%d\n\n", todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET);
        printf("Done date:                      %2d/%d/%d\n\n", done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);
        printf("Desired date:                   %2d/%d/%d\n\n", desired_date.tm_mon + MONTH_OFFSET, desired_date.tm_mday, desired_date.tm_year + YEAR_OFFSET);
    }
    
    // :write to file
    {
        struct tm todays_date       = global_todays_date;
        struct tm done_date         = global_done_date;
        int current_page            = global_current_page;
        float avg_minutes_per_page  = global_avg_minutes_per_page;
        int pages_read              = global_pages_read;

        FILE *f = fopen("record.txt", "a");
        // todays_date, done_date
        fprintf(f, "todays_date %2d/%2d/%4d current_page %3d pages_read %2d avg_minutes_per_page %3f done_date %2d/%2d/%2d\n",
                    todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET,
                                            current_page,    pages_read,    avg_minutes_per_page, 
                    done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);

    }
    return 0;
}
