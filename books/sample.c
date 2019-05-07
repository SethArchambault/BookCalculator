
// :input
int total_pages                 = 259;
float reading_hours_per_week    =  5;
int starting_page               = 7; 
float reading_session_hours     = 2.0;
int page_chapter_ends = 13;
/* progress: page_number, timer_start, timer_end 
 * The point of having "timer_start" is so you can use a separate timer without having to
 * reset it every single time, and without having to set a redundant timer inside this app.
 * Documenting your progress through each page makes it really clear when you're getting stuck. 
 */ 
int progress[] = {
    7, 0, 9, // chapter 1
    8, 9, 17,
    9, 17, 25, 
    10, 25, 34,
    11, 34, 41,
    12, 41, 55,
    13, 0, 47      // Actually a recap and overview of chapter
};
// End of input. No need to enter stuff beyond this point.

int progress_columns = 3;
/*
 * input.txt
 * //   Pages     Time      PerPageTime
 * :total_pages 199
 *    190        100        00:30 
 *    190-195    30-100     00:30 
 *    190-195    60-100     00:30 
 *    190-195    0-100      00:30 
 * :starting_page 177
 * :chapter 1 
 * :page_chapter_ends 199
 * :reading_session_hours 2.0
 * :reading_hours_per_week 5
 *
 * */
