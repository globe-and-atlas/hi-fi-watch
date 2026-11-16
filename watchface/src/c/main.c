#include <pebble.h>
#include "config.h"
#include "policy.h"
enum {P_TIME=1,P_DATE,P_SECONDS,P_PALETTE,P_BG,P_FG,P_ACCENT,P_NEEDLES};
typedef struct {int time_format,date_format,seconds,palette,bg,fg,accent,needles;} Settings;
static Settings s={0,1,1,0,0,0xFFAA00,0xFFAA00,PBL_IF_ROUND_ELSE(1,0)};
static const uint32_t PALETTES[5][3]={{0,0xFFAA00,0xFFAA00},{0,0xAAFFAA,0x00FFAA},
 {0x000055,0xAAFFFF,0x55AAFF},{0xAAAAAA,0,0},{0xFFFFFF,0,0xFF5500}};
static Window *s_window;
static Layer *s_face,*s_meters;
static GPath *s_segments[7];
static struct tm s_time;
static int s_battery;
#if HIFI_FIXTURE
static int f_hour=HIFI_HOUR,f_minute=HIFI_MINUTE,f_second=HIFI_SECOND,f_battery=HIFI_BATTERY;
#endif
static const GPathInfo SEGMENTS[7]={
  {6,(GPoint[]){{7,0},{27,0},{31,4},{27,8},{7,8},{3,4}}},
  {6,(GPoint[]){{30,5},{34,9},{34,24},{30,29},{26,24},{26,10}}},
  {6,(GPoint[]){{30,31},{34,36},{34,51},{30,57},{26,52},{26,36}}},
  {6,(GPoint[]){{7,52},{27,52},{31,56},{27,60},{7,60},{3,56}}},
  {6,(GPoint[]){{4,31},{8,36},{8,52},{4,57},{0,51},{0,36}}},
  {6,(GPoint[]){{4,5},{8,10},{8,24},{4,29},{0,24},{0,9}}},
  {6,(GPoint[]){{7,26},{27,26},{31,30},{27,34},{7,34},{3,30}}}
};

static const char *MONTHS[]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
static const char *DAYS[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
static uint32_t background(void) {return s.palette<5?PALETTES[s.palette][0]:(uint32_t)s.bg;}
static uint32_t foreground(void) {return s.palette<5?PALETTES[s.palette][1]:(uint32_t)s.fg;}
static uint32_t accent(void) {return s.palette<5?PALETTES[s.palette][2]:(uint32_t)s.accent;}
static uint32_t ghost(void) {
 uint32_t bg=background(),fg=foreground(),result=0;
 for(int shift=0;shift<=16;shift+=8) result|=(uint32_t)((((bg>>shift)&255)*2+((fg>>shift)&255))/3)<<shift;
 return result;
}
static uint32_t dial_ghost(void) {
 uint32_t raw=ghost(),result=0;
 for(int shift=0;shift<=16;shift+=8)result|=(uint32_t)(((((raw>>shift)&255)+42)/85)*85)<<shift;
 return result;
}
static void clear(Layer *layer,GContext *ctx) {
 graphics_context_set_antialiased(ctx,false);
 graphics_context_set_fill_color(ctx,GColorFromHEX(background()));
 graphics_fill_rect(ctx,layer_get_bounds(layer),0,GCornerNone);
 graphics_context_set_fill_color(ctx,GColorFromHEX(foreground()));
 graphics_context_set_stroke_color(ctx,GColorFromHEX(accent()));
 graphics_context_set_text_color(ctx,GColorFromHEX(foreground()));
}
static void text(GContext *ctx,const char *value,const char *font,GRect rect,GTextAlignment align) {
 graphics_draw_text(ctx,value,fonts_get_system_font(font),rect,GTextOverflowModeFill,align,NULL);
}
static void digit(GContext *ctx,int value,int x,int y) {
 for(int segment=0;segment<7;segment++) if(DIGIT_MASK[value]&(1<<segment)) {
  gpath_move_to(s_segments[segment],GPoint(x,y));gpath_draw_filled(ctx,s_segments[segment]);
 }
}
static void draw_battery(GContext *ctx) {
 char label[6];snprintf(label,sizeof(label),"%d%%",s_battery);
 GFont font=fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
 GSize size=graphics_text_layout_get_content_size(label,font,GRect(0,0,60,30),GTextOverflowModeFill,GTextAlignmentLeft);
 int x=battery_group_left(size.w);
 graphics_context_set_stroke_width(ctx,2);graphics_draw_rect(ctx,GRect(x,205,106,16));
 graphics_context_set_stroke_width(ctx,1);graphics_fill_rect(ctx,GRect(x+106,210,4,6),0,GCornerNone);
 graphics_context_set_fill_color(ctx,GColorFromHEX(accent()));
 for(int i=0;i<battery_blocks(s_battery);i++) graphics_fill_rect(ctx,GRect(x+4+i*16,209,13,8),0,GCornerNone);
 text(ctx,label,FONT_KEY_GOTHIC_24_BOLD,GRect(x+120,197,size.w+2,30),GTextAlignmentLeft);
}
#if defined(PBL_ROUND)
#include "round_digital.h"
static void round_current(GContext *ctx){uint32_t bg=background(),fg=foreground(),ac=accent();RoundData d={0};d.now=s_time;d.bg=bg;d.fg=fg;d.accent=ac;d.battery=s_battery;d.time_mode=s.time_format;d.date_mode=s.date_format;d.seconds=s.seconds;d.dial=s.needles;round_digital(ctx,&d,4,rseven);}
#endif
static void draw_face(Layer *layer,GContext *ctx) {
#if defined(PBL_ROUND)
 (void)layer;round_current(ctx);rproof_marker(ctx);return;
#endif

 clear(layer,ctx);
 text(ctx,"HI-FI",FONT_KEY_GOTHIC_24_BOLD,GRect(12,-1,115,29),GTextAlignmentLeft);
 graphics_context_set_stroke_width(ctx,2);graphics_draw_line(ctx,GPoint(10,31),GPoint(189,31));
 graphics_context_set_stroke_width(ctx,1);
 int hour=display_hour(s_time.tm_hour,s.time_format,clock_is_24h_style());
 int offset=clock_ink_offset(hour,s_time.tm_min);
 digit(ctx,hour/10,16+offset,41);digit(ctx,hour%10,55+offset,41);
 digit(ctx,s_time.tm_min/10,110+offset,41);digit(ctx,s_time.tm_min%10,149+offset,41);
 graphics_fill_rect(ctx,GRect(97+offset,59,6,6),1,GCornersAll);graphics_fill_rect(ctx,GRect(97+offset,79,6,6),1,GCornersAll);
 if(!uses_24(s.time_format,clock_is_24h_style())) text(ctx,s_time.tm_hour>=12?"PM":"AM",FONT_KEY_GOTHIC_14_BOLD,GRect(12,101,176,17),GTextAlignmentCenter);
 graphics_draw_line(ctx,GPoint(10,167),GPoint(189,167));
 char date[32]="";
 switch(s.date_format) {
  case 1:snprintf(date,sizeof(date),"%s %s %02d",DAYS[s_time.tm_wday],MONTHS[s_time.tm_mon],s_time.tm_mday);break;
  case 2:snprintf(date,sizeof(date),"%04d-%02d-%02d",s_time.tm_year+1900,s_time.tm_mon+1,s_time.tm_mday);break;
  case 3:snprintf(date,sizeof(date),"%02d/%02d/%04d",s_time.tm_mon+1,s_time.tm_mday,s_time.tm_year+1900);break;
  case 4:snprintf(date,sizeof(date),"%02d/%02d/%04d",s_time.tm_mday,s_time.tm_mon+1,s_time.tm_year+1900);break;
 }
 text(ctx,date,FONT_KEY_GOTHIC_24_BOLD,GRect(10,169,180,29),GTextAlignmentCenter);
 graphics_draw_line(ctx,GPoint(10,199),GPoint(189,199));
 draw_battery(ctx);
}
static void draw_meters(Layer *layer,GContext *ctx) {
#if defined(PBL_ROUND)
 (void)layer;(void)ctx;return;
#endif

 clear(layer,ctx);
 if(s.needles){
  for(int dial=0;dial<2;dial++){
   int x=10+dial*95,value=dial?s_time.tm_min:s_time.tm_sec;
   graphics_context_set_fill_color(ctx,GColorFromHEX(dial_ghost()));
   graphics_fill_rect(ctx,GRect(x,0,85,41),3,GCornersAll);
   graphics_context_set_stroke_color(ctx,GColorFromHEX(accent()));
   for(int i=0;i<=10;i++)graphics_draw_line(ctx,GPoint(x+7+i*7,5),GPoint(x+7+i*7,i%5==0?12:9));
   if(dial||s.seconds){
    graphics_context_set_stroke_color(ctx,GColorFromHEX(foreground()));
    graphics_draw_line(ctx,GPoint(x+42,26),GPoint(needle_tip(value,x+7,70),6));
   }
   text(ctx,dial?"MIN":s.seconds?"SEC":"OFF",FONT_KEY_GOTHIC_14_BOLD,GRect(x+4,25,77,16),GTextAlignmentCenter);
  }
  return;
 }
 const char *labels[2]={s.seconds?"SEC":"OFF","MIN"};
 int count[2]={s.seconds?meter_segments(s_time.tm_sec):0,meter_segments(s_time.tm_min)};
 for(int row=0;row<2;row++) {
  int y=row*22;
  text(ctx,labels[row],FONT_KEY_GOTHIC_14_BOLD,GRect(10,y-4,30,19),GTextAlignmentLeft);
  for(int i=0;i<12;i++) {
   graphics_context_set_fill_color(ctx,GColorFromHEX(i<count[row]?accent():ghost()));
   graphics_fill_rect(ctx,GRect(43+i*12,y+2,10,11),0,GCornerNone);
  }
 }
}
static void tick(struct tm *now,TimeUnits changed) {
 s_time=*now;
#if HIFI_FIXTURE
 s_time.tm_hour=f_hour;s_time.tm_min=f_minute;s_time.tm_sec=f_second;
 s_time.tm_wday=4;s_time.tm_year=126;s_time.tm_mon=10;s_time.tm_mday=5;
#endif
 layer_mark_dirty(s_meters);
#if defined(PBL_ROUND)
 layer_mark_dirty(s_face);
#endif

 if(changed&MINUTE_UNIT) layer_mark_dirty(s_face);
}
static void battery_changed(BatteryChargeState state) {
 s_battery=state.charge_percent;
#if HIFI_FIXTURE
 s_battery=f_battery;
#endif
 layer_mark_dirty(s_face);
}
static void subscribe_ticks(void) {tick_timer_service_unsubscribe();tick_timer_service_subscribe(s.seconds?SECOND_UNIT:MINUTE_UNIT,tick);}
static void setting(DictionaryIterator *iter,uint32_t message_key,int persist_key,int maximum,int *target) {
 Tuple *tuple=dict_find(iter,message_key);
 if(!tuple || (tuple->type!=TUPLE_INT && tuple->type!=TUPLE_UINT)) return;
 int value=tuple->value->int32;if(value<0 || value>maximum) return;
 *target=value;persist_write_int(persist_key,value);
}
#if HIFI_FIXTURE
static void fixture_value(DictionaryIterator *iter,int key,int maximum,int *target) {
 Tuple *tuple=dict_find(iter,key);
 if(tuple && tuple->type==TUPLE_INT && tuple->value->int32>=0 && tuple->value->int32<=maximum) *target=tuple->value->int32;
}
#endif
static void inbox(DictionaryIterator *iter,void *context) {
#if defined(PBL_ROUND) && (HIFI_FIXTURE)
 Tuple *round_proof=dict_find(iter,11022);if(round_proof && round_proof->type==TUPLE_INT){r_proof=round_proof->value->int32==1;layer_mark_dirty(s_face);}
#endif

 (void)context;int old_seconds=s.seconds;
 setting(iter,MESSAGE_KEY_TimeFormat,P_TIME,2,&s.time_format);setting(iter,MESSAGE_KEY_DateFormat,P_DATE,4,&s.date_format);
 setting(iter,MESSAGE_KEY_ShowSeconds,P_SECONDS,1,&s.seconds);setting(iter,MESSAGE_KEY_Palette,P_PALETTE,5,&s.palette);
 setting(iter,MESSAGE_KEY_Background,P_BG,0xFFFFFF,&s.bg);setting(iter,MESSAGE_KEY_Foreground,P_FG,0xFFFFFF,&s.fg);
 setting(iter,MESSAGE_KEY_Accent,P_ACCENT,0xFFFFFF,&s.accent);
 setting(iter,MESSAGE_KEY_NeedleDials,P_NEEDLES,1,&s.needles);
#if HIFI_FIXTURE
 fixture_value(iter,11000,23,&f_hour);fixture_value(iter,11001,59,&f_minute);fixture_value(iter,11002,59,&f_second);
 fixture_value(iter,11004,100,&f_battery);s_battery=f_battery;
#endif
 if(old_seconds!=s.seconds) subscribe_ticks();
 window_set_background_color(s_window,GColorFromHEX(background()));
 time_t now=time(NULL);tick(localtime(&now),MINUTE_UNIT);
}
static void restore(int key,int maximum,int *target) {if(persist_exists(key)) {int value=persist_read_int(key);if(value>=0 && value<=maximum) *target=value;}}
static void init(void) {
 restore(P_TIME,2,&s.time_format);restore(P_DATE,4,&s.date_format);restore(P_SECONDS,1,&s.seconds);restore(P_PALETTE,5,&s.palette);
 restore(P_BG,0xFFFFFF,&s.bg);restore(P_FG,0xFFFFFF,&s.fg);restore(P_ACCENT,0xFFFFFF,&s.accent);restore(P_NEEDLES,1,&s.needles);
 for(int i=0;i<7;i++) s_segments[i]=gpath_create(&SEGMENTS[i]);
 s_window=window_create();window_set_background_color(s_window,GColorFromHEX(background()));
 Layer *root=window_get_root_layer(s_window);
 s_face=layer_create(PBL_IF_ROUND_ELSE(GRect(0,0,PBL_DISPLAY_WIDTH,PBL_DISPLAY_HEIGHT),GRect(0,0,200,228)));layer_set_update_proc(s_face,draw_face);layer_add_child(root,s_face);
 s_meters=layer_create(PBL_IF_ROUND_ELSE(GRect(0,0,PBL_DISPLAY_WIDTH,PBL_DISPLAY_HEIGHT),GRect(0,121,200,43)));layer_set_update_proc(s_meters,draw_meters);layer_add_child(root,s_meters);
 window_stack_push(s_window,true);
 time_t now=time(NULL);tick(localtime(&now),MINUTE_UNIT);battery_changed(battery_state_service_peek());
 subscribe_ticks();battery_state_service_subscribe(battery_changed);app_message_register_inbox_received(inbox);app_message_open(256,64);
}
static void deinit(void) {
 tick_timer_service_unsubscribe();battery_state_service_unsubscribe();app_message_deregister_callbacks();
 layer_destroy(s_meters);layer_destroy(s_face);window_destroy(s_window);
 for(int i=0;i<7;i++) gpath_destroy(s_segments[i]);
}
int main(void) {init();app_event_loop();deinit();}
