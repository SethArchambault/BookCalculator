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
#include <string.h>

#define assert(expr) if(!(expr)) { printf("\nline %d: %s\n", __LINE__, #expr); __builtin_trap();} 
#define MONTH_OFFSET 1
#define YEAR_OFFSET 1900 



void print_progress(int *progress) {
    printf("page: %d \tstart_time: %d\t end_time %d\n", progress[0], progress[1], progress[2]);
}

int is_break(char value) {
    if (value == ' ' ||
        value == '\n') {
        return 1;
    } else {
        return 0;
    }
}

int is_number(char value) {
    int d = (int)value;
    if(d >= 48 && d <= 57) {
        return 1;
    } else {
        return 0;
    }
}

int is_space(char value) {
    assert(value != '\n');
    if (value == ' ') {
        return 1;
    } else {
        return 0;
    }
}

int parse_number(char * c, int * i) {
    // @Compress: into function
    const int timer_end_max = 5;
    char timer_end_char[timer_end_max]; 
    int timer_end_index = 0;
    //printf("\n-- found dash -\n");
    for(;is_number(c[*i]) ;++*i) {
        timer_end_char[timer_end_index] = (int)c[*i];
        ++timer_end_index;
        assert(timer_end_index <= timer_end_max); // make sure we don't segfault here..
    }
    timer_end_char[timer_end_index] = '\0';

    return atoi(timer_end_char);
}

#if 1
int main(int argc, char ** argv) {
    assert(argc == 2);
    char *filename = argv[1];
    int   total_pages                 = 0;
    float reading_hours_per_week      = 0;
    int   starting_page               = 0; 
    int   last_page                   = 0;
    float reading_session_hours       = 0;
    int   page_chapter_ends           = 0;
    int   progress_length             = 15000;
    int   progress_count              = 0;
    int   progress[progress_length];
    {
        int f_len;
        char * c;
        {
            FILE * f = fopen(filename, "r");
            assert(f);
            fseek(f, 0, SEEK_END);
            f_len = ftell(f);
            fseek(f, 0, SEEK_SET);
            c = malloc(f_len);
            fread(c, f_len, 1, f);
            fclose(f);
        }


#if 0
        {
            FILE * f = fopen("books/backup.txt", "w");
            fwrite(c, f_len, 1, f);
            fclose(f);
        }
#endif

        int mode = 0;
        char * reformatted_data = malloc(f_len + f_len * 0.5);
        for (int i = 0; i < f_len; ++i) {
            for (;c[i] == ' ';++i); // skip space
            if (mode == 0) {
                if (c[i] == '/') { // skip comment
                    for (;c[i] != '\n';++i) {
                        sprintf(reformatted_data, "%s%c", reformatted_data, c[i]);
                    }
                    strcat(reformatted_data, "\n");
                    continue;
                }
                if (c[i] == ':') { // assign variable
                    ++i;
                    char variable_name[50];
                    char variable_name_len = 0;
                    // get variable name
                    for (;c[i] != ' '; ++i) {
                        variable_name[variable_name_len] = c[i];
                        ++variable_name_len;
                    }
                    variable_name[variable_name_len] = '\0';


                    for (;c[i] == ' ';++i); // skip whitespace
                    
                    char variable_value[50];
                    char variable_value_len = 0;
                    // get variable value
                    for (;!is_break(c[i]); ++i) {
                        variable_value[variable_value_len] = c[i];
                        ++variable_value_len;
                    }
                    variable_value[variable_value_len] = '\0';
                    
                    sprintf(reformatted_data, "%s:%s %s\n", 
                            reformatted_data, variable_name, variable_value);
                    if (strcmp(variable_name, "total_pages") == 0) {
                        total_pages                 = atoi(variable_value);
                    }
                    if (strcmp(variable_name, "reading_hours_per_week") == 0) {
                        reading_hours_per_week      = atof(variable_value);
                    }
                    if (strcmp(variable_name, "starting_page") == 0) {
                        starting_page               = atoi(variable_value);
                    }
                    if (strcmp(variable_name, "reading_session_hours") == 0) {
                        reading_session_hours       = atof(variable_value);
                    }
                    if (strcmp(variable_name, "page_chapter_ends") == 0) {
                        page_chapter_ends           = atoi(variable_value);
                    }

                    for (;c[i] != '\n';++i);
                    continue;
                }
                mode = 1;
            }
            if (mode == 1) {
                if (c[i] == '\n') {
                    mode = 0;
                    continue;
                }
                if (!is_number(c[i])) {
                    printf("Error: '");
                    for (;c[i] != '\n';++i) { // print rest of line 
                        printf("%c", c[i]);
                    }
                    printf("'");
                }
                assert(is_number(c[i])); 

                int page_start = parse_number(c, &i);
                int page_end   = 0;


                if (c[i] == '-') {
                    ++i;
                    page_end = parse_number(c, &i);
                } else {
                    page_end = page_start;
                }
                last_page = page_end;
                
                for (;c[i] == ' ';++i); // skip space

                int timer_start = parse_number(c, &i);
                int timer_end   = 0;
                
                if (c[i] == '-') {
                    ++i;
                    timer_end = parse_number(c, &i);
                } else {
                    timer_end = timer_start;
                }
                int timer_dif = timer_end - timer_start;
                int timer_hours = (int)(timer_dif / 60);
                int timer_min   = (int)(timer_dif % 60);

                if (page_start == page_end) {
                    sprintf(reformatted_data, "%s%3d     %3d-%-3d %02d:%02d\n", 
                        reformatted_data, page_start, timer_start, timer_end, timer_hours, timer_min);
                } else {
                    sprintf(reformatted_data, "%s%3d-%-3d %3d-%-3d %02d:%02d\n", 
                        reformatted_data, page_start, page_end, timer_start, timer_end, timer_hours, timer_min);
                }
                progress[progress_count] = page_start;
                ++progress_count;
                progress[progress_count] = timer_start;
                ++progress_count;
                progress[progress_count] = timer_end;
                ++progress_count;
                for (;c[i] != '\n';++i); // fastforaard
                mode = 0;
                continue;

                // get first number
                //printf("%d ", (int)c[i]);
            } // mode
        } // for

        {
            assert(strlen(reformatted_data) > 0); 
            FILE *f = fopen(filename, "w");
            fwrite(reformatted_data, strlen(reformatted_data), 1, f);
            fclose(f);
        }

        printf("%s", reformatted_data);
    } // scope
#if 0
    printf("total_pages %d\n", total_pages);
    printf("reading_hours_per_week %f\n", reading_hours_per_week);
    printf("starting_pages %d\n", starting_page);
    printf("reading_session_hours %f\n", reading_session_hours);
    printf("page_chapter_ends %d\n", page_chapter_ends);
#endif

    int progress_columns = 3;
    assert(progress_count > 0);

    // :calculate avg, min, max
    int reading_minutes                 = 0;
    int pages_remaining_in_chapter = page_chapter_ends - last_page;
    int pages_read                      =  0;
    assert(last_page> 0);
    int max_minutes = 0;
    int min_minutes = 1000;
    for (int progress_index = 0; 
             progress_index < progress_count; 
             progress_index += 3) {
        if (progress[progress_index] < starting_page) continue;
        ++pages_read;
        int start_time_index = progress_index + 1;
        int end_time_index = progress_index + 2;
        int time_took      = progress[end_time_index] - progress[start_time_index];
        if (time_took <= 0) print_progress(&progress[progress_index]);
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
             progress_index < progress_count; 
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
    int pages_left              = total_pages - last_page;
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

        desired_date.tm_mon             = 5- MONTH_OFFSET;
        desired_date.tm_mday            = 14;
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
        fprintf(f, "todays_date %2d/%2d/%4d last_page %3d pages_read %2d avg_minutes_per_page %3f done_date %2d/%2d/%2d\n",
                    todays_date.tm_mon + MONTH_OFFSET, todays_date.tm_mday, todays_date.tm_year + YEAR_OFFSET,
                                            last_page,    pages_read,    avg_minutes_per_page, 
                    done_date.tm_mon + MONTH_OFFSET, done_date.tm_mday, done_date.tm_year + YEAR_OFFSET);

    return 0;
}
#endif
