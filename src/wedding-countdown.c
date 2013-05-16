#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "mini-printf.h"
#include "date_def.h"


#define MY_UUID { 0xAD, 0x97, 0xA2, 0x4A, 0xF0, 0x11, 0x41, 0xF7, 0x9A, 0x9F, 0xBB, 0x2C, 0x81, 0xE3, 0xF9, 0xB3 }

PBL_APP_INFO(MY_UUID,
             "Wedding Countdown", "stvsan", //For Kyle Barry Burnett, Congrats!
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);


typedef struct {
    int hour;
    int minute;
}s_time;

//Name of the bride or groom
static char name[] = "Carly Grigoratos";

//The wedding date for countdown
s_date wedding_date = {6,15,2013}; //June 15, 2013

//The wedding time for countdown
s_time wedding_time = {15,0}; //15:00 or 3:00PM.

/*
 Currently, if you set to a time other than the top of the hour, it'll work but the minutes countdown at the top of every hour. Running through this logic every minute would drain your battery faster.
*/

static char line1_text[] = "IT IS";
static char days_text[9];
static char hours_text[9];
static char minutes_text[12];
static char seconds_text[18];
static char line6_text[] = "UNTIL YOU MARRY  ";
static char line7_text[18];

Window window;

BmpContainer background_image_container;

TextLayer text_line1_layer;
TextLayer text_line2_layer;
TextLayer text_line3_layer;
TextLayer text_line4_layer;
TextLayer text_line5_layer;
TextLayer text_line6_layer;
TextLayer text_line7_layer;
TextLayer text_time_layer;

GFont font_tiny,font_small,font_regular,font_bold;

int last_wday = -1;     // used to tell when the day changes
int last_hour = -1;     // used to tell when the hour changes
int last_min = -1;      // used to tell when the minute changes
int checkdate = 0;      // used to tell if current date has past wedding date

void update_text()
{
    mini_snprintf(line1_text,sizeof(line1_text),"YOU");
    mini_snprintf(days_text,sizeof(days_text),"ARE NOW");
    mini_snprintf(hours_text,sizeof(hours_text),"HAPPILY");
    mini_snprintf(minutes_text,sizeof(minutes_text),"MARRIED TO");
    mini_snprintf(seconds_text,sizeof(seconds_text),name);
    mini_snprintf(line6_text,sizeof(line6_text),"A TRUE LOVE STORY");
    mini_snprintf(line7_text,sizeof(line7_text),"NEVER ENDS");
    
    
    text_layer_set_font(&text_line6_layer, font_tiny);
    text_layer_set_text_alignment(&text_line6_layer,GTextAlignmentCenter);
    layer_set_frame(&text_line6_layer.layer, GRect(2, 115, 144-2, 168-115));
    
    text_layer_set_font(&text_line7_layer, font_tiny);
    text_layer_set_text_alignment(&text_line7_layer,GTextAlignmentCenter);
    layer_set_frame(&text_line7_layer.layer, GRect(2, 128, 144-2, 168-128));
    
    text_layer_set_text(&text_line1_layer, line1_text);
    text_layer_set_text(&text_line2_layer, days_text);
    text_layer_set_text(&text_line3_layer, hours_text);
    text_layer_set_text(&text_line4_layer, minutes_text);
    text_layer_set_text(&text_line5_layer, seconds_text);
    text_layer_set_text(&text_line6_layer, line6_text);
    text_layer_set_text(&text_line7_layer, line7_text);
}


void handle_init(AppContextRef ctx)
{
    (void) ctx;
    
    ResHandle res_t;
    ResHandle res_s;
    ResHandle res_r;
    ResHandle res_b;
    
    window_init(&window, "Wedding");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorBlack);

    resource_init_current_app(&APP_RESOURCES);
    res_t = resource_get_handle(RESOURCE_ID_FONT_SF_ARCH_RIVAL_14);
    res_s = resource_get_handle(RESOURCE_ID_FONT_SF_ARCH_RIVAL_16);
    res_r = resource_get_handle(RESOURCE_ID_FONT_SF_ARCH_RIVAL_18);
    res_b = resource_get_handle(RESOURCE_ID_FONT_SF_ARCH_RIVAL_18_BOLD);
    
    font_tiny = fonts_load_custom_font(res_t);
    font_small = fonts_load_custom_font(res_s);
    font_regular = fonts_load_custom_font(res_r);
    font_bold = fonts_load_custom_font(res_b);
    
    PblTm t;
    get_time(&t);
    
    //Check the date if it's after the wedding date to be used in the handler
    checkdate = diffdays((s_date){t.tm_mon+1,t.tm_mday,t.tm_year+1900}, wedding_date);
  
    bmp_init_container(RESOURCE_ID_IMAGE_COUPLE, &background_image_container);
    
    GRect frame = layer_get_frame(&background_image_container.layer.layer);
    frame.origin.x = 84;
    frame.origin.y = 0;
    layer_set_frame(&background_image_container.layer.layer, frame);
    layer_add_child(&window.layer, &background_image_container.layer.layer);

    text_layer_init(&text_line1_layer, window.layer.frame);
    text_layer_set_text_color(&text_line1_layer, GColorWhite);
    text_layer_set_background_color(&text_line1_layer, GColorClear);
    layer_set_frame(&text_line1_layer.layer, GRect(2, 8, 144-2, 168-2));
    text_layer_set_font(&text_line1_layer, font_bold);
    layer_add_child(&window.layer, &text_line1_layer.layer);
    
    text_layer_init(&text_line2_layer, window.layer.frame);
    text_layer_set_text_color(&text_line2_layer, GColorWhite);
    text_layer_set_background_color(&text_line2_layer, GColorClear);
    layer_set_frame(&text_line2_layer.layer, GRect(2, 30, 144-2, 168-30));
    text_layer_set_font(&text_line2_layer, font_regular);
    layer_add_child(&window.layer, &text_line2_layer.layer);

    
    text_layer_init(&text_line3_layer, window.layer.frame);
    text_layer_set_text_color(&text_line3_layer, GColorWhite);
    text_layer_set_background_color(&text_line3_layer, GColorClear);
    layer_set_frame(&text_line3_layer.layer, GRect(2, 48, 144-2, 168-48));
    text_layer_set_font(&text_line3_layer, font_regular);
    layer_add_child(&window.layer, &text_line3_layer.layer);

    
    text_layer_init(&text_line4_layer, window.layer.frame);
    text_layer_set_text_color(&text_line4_layer, GColorWhite);
    text_layer_set_background_color(&text_line4_layer, GColorClear);
    layer_set_frame(&text_line4_layer.layer, GRect(2, 66, 144-2, 168-66));
    text_layer_set_font(&text_line4_layer, font_regular);
    layer_add_child(&window.layer, &text_line4_layer.layer);

    
    text_layer_init(&text_line5_layer, window.layer.frame);
    text_layer_set_text_color(&text_line5_layer, GColorWhite);
    text_layer_set_background_color(&text_line5_layer, GColorClear);
    layer_set_frame(&text_line5_layer.layer, GRect(2, 84, 144-2, 168-84));
    text_layer_set_font(&text_line5_layer, font_regular);
    layer_add_child(&window.layer, &text_line5_layer.layer);
    
    text_layer_init(&text_line6_layer, window.layer.frame);
    text_layer_set_text_color(&text_line6_layer, GColorWhite);
    text_layer_set_background_color(&text_line6_layer, GColorClear);
    layer_set_frame(&text_line6_layer.layer, GRect(2, 110, 144-2, 168-110));
    text_layer_set_font(&text_line6_layer, font_small);
    layer_add_child(&window.layer, &text_line6_layer.layer);
    
    text_layer_init(&text_line7_layer, window.layer.frame);
    text_layer_set_text_color(&text_line7_layer, GColorWhite);
    text_layer_set_background_color(&text_line7_layer, GColorClear);
    layer_set_frame(&text_line7_layer.layer, GRect(2, 128, 144-2, 168-128));
    text_layer_set_font(&text_line7_layer, font_small);
    layer_add_child(&window.layer, &text_line7_layer.layer);
    
    text_layer_init(&text_time_layer, window.layer.frame);
    text_layer_set_text_color(&text_time_layer, GColorBlack);
    text_layer_set_background_color(&text_time_layer, GColorWhite);
    layer_set_frame(&text_time_layer.layer, GRect(2, 150, 144-2, 150));
    text_layer_set_font(&text_time_layer, font_small);
    text_layer_set_text_alignment(&text_time_layer,GTextAlignmentCenter);
    layer_add_child(&window.layer, &text_time_layer.layer);

    text_layer_set_text(&text_line1_layer, line1_text);
    text_layer_set_text(&text_line6_layer, line6_text);
    text_layer_set_text(&text_line7_layer, name);
    
}

void swap_background()
{
    layer_remove_from_parent(&background_image_container.layer.layer);
    
    bmp_deinit_container(&background_image_container);
    bmp_init_container(RESOURCE_ID_IMAGE_HEARTS, &background_image_container);
    
    GRect frame = layer_get_frame(&background_image_container.layer.layer);
    frame.origin.x = 84;
    frame.origin.y = 0;
    
    layer_set_frame(&background_image_container.layer.layer, frame);
    layer_add_child(&window.layer, &background_image_container.layer.layer);
    
    
}



void handle_tick(AppContextRef ctx, PebbleTickEvent *t)
{
    (void)ctx;
    
    static char time_text[] = "00:00 PM";
    char *time_format;

    //only needed to run on change in minutes since seconds aren't displayed
    if (t->tick_time->tm_min != last_min)
    {
        time_format = clock_is_24h_style() ? "%R" : "%I:%M %p";
        string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);
        
        // Kludge to handle lack of non-padded hour format string
        // for twelve hour clock.
        if (!clock_is_24h_style() && (time_text[0] == '0'))
        {
            memmove(time_text, &time_text[1], sizeof(time_text) - 1);
        }
        
        text_layer_set_text(&text_time_layer, time_text);
    }
    
    //Check if it's past the date. If so, bypass countdown and go to the updated screen
    if (checkdate >= 0)
    {
        //Changes second countdown
        mini_snprintf(seconds_text,sizeof(seconds_text),"%u Seconds",59-t->tick_time->tm_sec);
        text_layer_set_text(&text_line5_layer, seconds_text);
        
        if (t->tick_time->tm_min != last_min)
        {
            //Changes minute countdown
            mini_snprintf(minutes_text,sizeof(minutes_text),"%u Minutes",59-t->tick_time->tm_min);
            text_layer_set_text(&text_line4_layer, minutes_text);
            
            if (t->tick_time->tm_hour != last_hour)
            {
                int resultdays;
                resultdays = diffdays((s_date){t->tick_time->tm_mon+1,t->tick_time->tm_mday,t->tick_time->tm_year+1900}, wedding_date);
                
                int remaining_hours;
                remaining_hours = ((wedding_time.hour-1) == t->tick_time->tm_hour) ? 0 : (wedding_time.hour - t->tick_time->tm_hour);
                
                if ((resultdays > 0) && (remaining_hours <= 0))
                {
                    remaining_hours = 23 + remaining_hours;
                }
                
                //Change hours countdown
                mini_snprintf(hours_text,sizeof(hours_text),"%u Hours",remaining_hours);
                text_layer_set_text(&text_line3_layer, hours_text);
                
                //Change days countdown. Added logic to say TODAY if days is 0
                if (t->tick_time->tm_wday != last_wday)
                {
                    if (resultdays > 0)
                    {
                        mini_snprintf(days_text,sizeof(days_text),"%u Days",resultdays);
                    }
                    else
                    {
                        mini_snprintf(days_text,sizeof(days_text),"TODAY &");
                    }
                    
                    text_layer_set_text(&text_line2_layer, days_text);
                    
                    last_wday = t->tick_time->tm_wday;
                }
                
                //Kickoff the layer swap when countdown is over.
                if ((resultdays == 0) && (t->tick_time->tm_hour >= wedding_time.hour) && (t->tick_time->tm_min >= wedding_time.minute))
                {
                    checkdate = -1;
                    swap_background();
                    update_text();
                }
                
                last_hour = t->tick_time->tm_hour;
                
            }
            
            last_min = t->tick_time->tm_min;
        }

    }
    else
    {
        //Added day logic to optimize battery since this runs every second
        if (t->tick_time->tm_wday != last_wday)
        {
            swap_background();
            update_text();
        }
    }
}

void handle_deinit(AppContextRef ctx) {
    
	bmp_deinit_container(&background_image_container);
}


void pbl_main(void *params)
{
    PebbleAppHandlers handlers =
    {
        .init_handler = &handle_init,
        .deinit_handler = &handle_deinit,
        .tick_info =
        {
            .tick_handler = &handle_tick,
            .tick_units = SECOND_UNIT
        }
    };

    app_event_loop(params, &handlers);
}
