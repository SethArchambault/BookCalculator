#include <stdio.h>
#include <time.h>
#include <math.h>

#define MONTH_OFFSET 1
#define YEAR_OFFSET 1900 

int main() {
    int total_pages                 = 199;
    float reading_hours_per_week    =  10;
    int pages_read;
    int current_page;
    int reading_minutes;
    float avg_minutes_per_page;
    {
        // page, start_time, end_time
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
            89, 0, 7


        };
        int progress_length = sizeof(progress) / sizeof(int);
        int starting_page               =  71;
        //int starting_page               =  progress[0];
        current_page                    =  progress[progress_length - 3];
        pages_read                      =  current_page - starting_page;
        printf("\n\n");
        printf("I have read:                    %5d pages\n\n", pages_read);
        reading_minutes                 = 0;
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
        printf("Hours it took:                  %5.2f hours\n\n", reading_minutes / 60.0f);
        avg_minutes_per_page      =       reading_minutes / pages_read;
        printf("Each page takes:                %5.2f minutes\n\n", avg_minutes_per_page);
        printf("Pages per hour:                 %5.2f pages\n\n", 60.0f / avg_minutes_per_page);
        printf("Maximum time:                   %5d minutes\n\n", max_minutes);
        printf("Minimum time:                   %5d minutes\n\n", min_minutes);

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
        printf("Standard Deviation:             %5.2f Minutes\n\n", std_div);
    }

    int pages_left                  = total_pages - current_page;
    printf("Remaining pages:                %5d pages\n\n", pages_left);
    int hours_remaining             = pages_left  * (avg_minutes_per_page / 60);
    printf("Remaining hours:                %5d hours\n\n", hours_remaining);

    printf("Every week I read for:          %5.2f hours\n\n", reading_hours_per_week);

    float reading_session_hours = 2.25;
    printf("Every reading session takes:    %5.2f hours\n\n",reading_session_hours);
    printf("Reading sessions remaining:     %5.1f Sessions\n\n",hours_remaining / reading_session_hours);

    time_t current_time             = time(NULL);
    struct tm todays_date           = *localtime(&current_time);
    struct tm done_date             = *localtime(&current_time);

    { // done_date, hours_remaining, reading_hours_per_week, weeks_remaining
        float weeks_remaining           = hours_remaining   / reading_hours_per_week;
        printf("Remaining weeks:                %5.2f weeks\n\n", weeks_remaining);
        done_date.tm_mday              += weeks_remaining * 7;
    } // done_date

    // todays_date
    printf("Todays Date:                     %d/%d/%d\n\n", todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET);
    // done_date
    printf("Done date:                       %2d/%d/%d\n\n", done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);
    // desired_date

    mktime(&done_date);

    {
        struct tm desired_date          = *localtime(&current_time);
        int desired_month               = 2;
        int desired_day                 = 1;
        int desired_year                = 2019;
        desired_date.tm_mon             = desired_month - MONTH_OFFSET;
        desired_date.tm_mday            = desired_day;
        desired_date.tm_year            = desired_year - YEAR_OFFSET;
        mktime(&desired_date);
        printf("Desired date:                     %2d/%d/%d\n\n", desired_date.tm_mon + MONTH_OFFSET, desired_date.tm_mday, desired_date.tm_year + YEAR_OFFSET);
    }


    
    FILE *f = fopen("record.txt", "a");
    // todays_date, done_date
    fprintf(f, "todays_date %2d/%2d/%4d current_page %3d pages_read %2d avg_minutes_per_page %3f done_date %2d/%2d/%2d\n",
                todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET,
                                        current_page,    pages_read,    avg_minutes_per_page, 
                done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);
    return 0;
}
