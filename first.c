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

    // :input
    int total_pages                 = 199;
    float reading_hours_per_week    =  5;
    int starting_page               = 177; 
//    float reading_session_hours     = 2.25;
    float reading_session_hours     = 2.0;
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
        104, 0, 6,
        105, 0, 13, // at detroit vintage
        106, 0, 8,
        107, 0, 13,
        108, 0, 11,
        109, 11, 21, // I don't really know whats going on
        110, 0, 11,
        111, 0, 4,
        112, 4, 12,
        113, 0, 22, // phone call, eating
        115, 0, 23, // lots of review
        116, 23, 40,  // lots of dictionary
        117, 0, 9,
        118, 0, 8,
        119, 0, 5,
        120, 5, 20,
        121, 0, 17,
        122, 0, 4,
        123, 0, 21, // converstaion with Rai
        124, 0, 13,
        125, 13, 19,
        126, 0, 7,
        127, 0, 8,
        128, 8, 15,
        129, 15,22,
        130, 0, 10,
        131, 10, 18,
        132, 0, 5,
        133, 0, 5,
        134, 0, 10,
        136, 0, 11,
        137, 0, 5,
        138, 5, 11,
        139, 0, 6,
        140, 0,  7,
        141, 0,  6,
        142, 0,  4,
        143, 0,  5,
        144, 0,  6,
        145, 0,  5,
        146, 0,  6,
        147, 0,  5,
        148, 0,  5,
        149, 0,  5,
        150, 0,  5,
        151, 0,  5,
        152, 0,  15,
        159, 0,  4,
        160, 0,  6,
        161, 0,  6,
        162, 6,  13,
        163, 13, 20,
        164, 20, 30,
        165, 30, 36,
        166, 36, 40,
        167, 40, 49,
        168, 49, 53,
        170, 53, 56,
        171, 0,  4,
        172, 4,  9,
        173, 9,  12,
        174, 12, 16,
        176, 16, 24,
        177, 0, 4,
        178, 0, 5,
        179, 5, 9,
        180, 9, 14,
        181, 14, 19,
        182, 19, 26,
        183, 26, 31,
        184, 31, 34,
        185, 34, 38,
        186, 0, 5,
        187, 5, 15,
        188, 15, 18,
        189, 18, 21,
        190, 21, 22,
        191, 22, 24,
        192, 0, 2,
        193, 2, 6,
        194, 6, 11
    };

    int page_chapter_ends = 199;

    int pushups = {
        5
    };

    int swinging_pushups = {
        5
    };

    int review[] = {
        114, 14 // learned about tithe, and the jacobite war
    };

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
