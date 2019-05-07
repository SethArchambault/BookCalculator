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


void print_progress(int *progress) {
    printf("page: %d \tstart_time: %d\t end_time %d\n", progress[0], progress[1], progress[2]);
}


int main() {

    // Swap in book here. 
#include "books/sample.c"

    int progress_length = sizeof(progress) / sizeof(int);
    assert(progress_length > 0);

    // :calculate avg, min, max
    int reading_minutes                 = 0;
    int current_page                    =  progress[progress_length - 3];
    int pages_remaining_in_chapter = page_chapter_ends - current_page;
    int pages_read                      =  0;
    assert(current_page > 0);
    int max_minutes = 0;
    int min_minutes = 1000;
    for (int progress_index = 0; 
             progress_index < progress_length; 
             progress_index += 3) {
        if (progress[progress_index] < starting_page) continue;
        ++pages_read;
        int start_time_index = progress_index + 1;
        int end_time_index = progress_index + 2;
        int time_took      = progress[end_time_index] - progress[start_time_index];
        if (time_took < 0) print_progress(&progress[progress_index]);
        assert(time_took > 0);
        reading_minutes   += time_took;
        if (time_took > max_minutes) max_minutes = time_took;
        if (time_took < min_minutes) min_minutes = time_took;
    }
    assert (reading_minutes > 0);
    float avg_minutes_per_page          = reading_minutes / pages_read;
    int total_minutes_remaining_in_chapter  = avg_minutes_per_page * pages_remaining_in_chapter;
    int hours_remaining_in_chapter = (int)total_minutes_remaining_in_chapter / 60.0f;

    int minutes_remaining_in_chapter = total_minutes_remaining_in_chapter % 60;

    // :calculate standard deviation
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

    // :pages left, hours remaining
    int pages_left              = total_pages - current_page;
    float hours_remaining         = pages_left  * (avg_minutes_per_page / 60);


    // :generated done date, desired date
    struct tm todays_date;
    struct tm done_date;
    struct tm desired_date;
    {
        time_t current_time   = time(NULL);
        todays_date           = *localtime(&current_time);
        done_date             = *localtime(&current_time);
        desired_date          = *localtime(&current_time);
    }

    float weeks_remaining = (float)hours_remaining / reading_hours_per_week;
    done_date.tm_mday              += weeks_remaining * 7;
    mktime(&done_date);

        desired_date.tm_mon             = 2- MONTH_OFFSET;
        desired_date.tm_mday            = 1;
        desired_date.tm_year            = 2019- YEAR_OFFSET;
        mktime(&desired_date);


    // :print
        printf("\n\n");
        printf("I have read:                    %5d pages\n\n", pages_read);
        printf("Hours it took:                  %5.2f hours\n\n", reading_minutes / 60.0f);
        printf("Each page takes:                %5.2f minutes\n\n", avg_minutes_per_page);
        printf("Pages per hour:                 %5.2f pages\n\n", 60.0f / avg_minutes_per_page);
        printf("Maximum time:                   %5d minutes\n\n", max_minutes);
        printf("Minimum time:                   %5d minutes\n\n", min_minutes);
        printf("Standard Deviation:             %5.2f Minutes\n\n", std_div);
        printf("Remaining pages in chapter:     %5d pages\n\n", pages_remaining_in_chapter);
        printf("Time remaining in chapter:   %5d:%02d \n\n", hours_remaining_in_chapter, minutes_remaining_in_chapter);
        printf("Remaining pages:                %5d pages\n\n", pages_left);
        printf("Remaining hours:                %5.2f hours\n\n", hours_remaining);
        printf("Every week I read for:          %5.2f hours\n\n", reading_hours_per_week);
        printf("Every reading session takes:    %5.2f hours\n\n",reading_session_hours);
        printf("Reading sessions remaining:     %5.1f Sessions\n\n",hours_remaining / reading_session_hours);
        printf("Remaining weeks:                %5.2f weeks\n\n", weeks_remaining);
        printf("Todays Date:                    %d/%d/%d\n\n", todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET);
        printf("Done date:                      %2d/%d/%d\n\n", done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);
        printf("Desired date:                   %2d/%d/%d\n\n", desired_date.tm_mon + MONTH_OFFSET, desired_date.tm_mday, desired_date.tm_year + YEAR_OFFSET);
    
    // :write to file

        FILE *f = fopen("record.txt", "a");
        // todays_date, done_date
        fprintf(f, "todays_date %2d/%2d/%4d current_page %3d pages_read %2d avg_minutes_per_page %3f done_date %2d/%2d/%2d\n",
                    todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET,
                                            current_page,    pages_read,    avg_minutes_per_page, 
                    done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);

    return 0;
}
