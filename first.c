#include <stdio.h>
#include <time.h>

#define MONTH_OFFSET 1
#define YEAR_OFFSET 1900 

int main() {
    /*
     * 0m  p39 31m 
     * 31m p40 10m
     * 41m p41 10m
     * 51m p42
     *
     *  0m p42 11m
     * 11m p43 9m
     * 20m p44 13m
     * 33m p45 10m
     * 43m p46 9m 
     * 52m p47 7m
     * 59m p48 8m
     * 67m p49
     * 
     *  0m p49 5m 
     *  5m p50 6m
     * 11m p51 5m
     * 16m p52
     *
     *  0m p52 5m
     *  5m p53 8m
     * 13m p54 6m
     * 19m p55 5m 
     * 24m p56
     */

    // page, start_time, end_time
     /*
    int progress = [
        52, 0, 5,
        53, 5, 13,
        54, 13, 19, 
        55, 19, 24
    ];
    int progress_length = (sizeof(progress) / sizeof(int));
    assert(progress_lengt


    for (int progress_index = 0; 
             progress_index < progress_length; 
             progress_index += 3) {

    }
    */


    int starting_page               =  52;
    int ending_page                 =  56;
    int minutes_it_took             =  24;
    int pages_i_just_read           =  ending_page - starting_page;
    float avg_minutes_per_page      =  minutes_it_took / pages_i_just_read;

    int current_page                =  ending_page;
    int total_pages                 = 199;
    int minutes_to_read_page        = avg_minutes_per_page;
    float hours_to_read_page        = minutes_to_read_page / 60.0f;
    float reading_hours_per_week    =  10;

    int pages_left                  = total_pages       - current_page;
    int hours_remaining             = pages_left        * hours_to_read_page;
    float weeks_remaining           = hours_remaining   / reading_hours_per_week;

    
    printf("I am on page:                   %5d pages\n\n", current_page);
    printf("out of a total of:              %5d pages\n\n", total_pages);
    printf("Remaining pages:                %5d pages\n\n", pages_left);
    printf("Each page takes:                %5d minutes\n\n", minutes_to_read_page);
    printf("Each page takes:                %5.2f hours\n\n", hours_to_read_page);
    printf("Remaining hours:                %5d hours\n\n", hours_remaining);
    printf("Every week I read for:          %5.2f hours\n\n", reading_hours_per_week);
    printf("Remaining weeks:                %5.2f weeks\n\n", weeks_remaining);

    {
        time_t current_time             = time(NULL);
        struct tm todays_date           = *localtime(&current_time);
        struct tm done_date             = *localtime(&current_time);
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

        printf("Todays Date:                     %d/%d/%d\n\n", todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET);
        printf("Done date:                       %2d/%d/%d\n\n", done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);
        printf("Desired date:                     %2d/%d/%d\n\n", desired_date.tm_mon + MONTH_OFFSET, desired_date.tm_mday, desired_date.tm_year + YEAR_OFFSET);


        FILE *f = fopen("record.txt", "a");
        fprintf(f, "todays_date %2d/%2d/%4d current_page %3d pages_i_just_read %2d avg_minutes_per_page %3f done_date %2d/%2d/%2d\n",
                    todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET,
                                            current_page,    pages_i_just_read,    avg_minutes_per_page, 
                    done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);
    }
    return 0;
}
