#pragma once

enum { COMPLICATION_TIME=0, COMPLICATION_BATTERY=1, COMPLICATION_WEEKDAYS=2 };
static const unsigned char DIGIT_MASK[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
static inline void clock_ink_bounds(int hour,int minute,int *left,int *right) {
  // Bounds of the same seven segment polygons used by the native renderer.
  static const int minimum[7]={3,26,26,3,0,0,3};
  static const int maximum[7]={31,34,34,31,8,8,31};
  static const int positions[4]={16,55,110,149};
  int digits[4]={hour/10,hour%10,minute/10,minute%10};
  *left=97;*right=102; // Include the colon.
  for(int i=0;i<4;i++) for(int segment=0;segment<7;segment++)
    if(DIGIT_MASK[digits[i]]&(1<<segment)) {
      int a=positions[i]+minimum[segment],b=positions[i]+maximum[segment];
      if(a<*left) *left=a;
      if(b>*right) *right=b;
    }
}
static inline int clock_ink_offset(int hour,int minute) {
  int left,right;clock_ink_bounds(hour,minute,&left,&right);
  return (199-left-right)/2;
}
static inline int uses_24(int preference,int watch_24) {return preference==2 || (preference==0 && watch_24);}
static inline int display_hour(int hour,int preference,int watch_24) {
  return uses_24(preference,watch_24)?hour:(hour%12?hour%12:12);
}
static inline int meter_segments(int value) {return value<=0?0:value>=60?12:(value+1)/5;}
static inline int battery_blocks(int percent) {return percent<=0?0:percent>=100?6:(percent*6+99)/100;}
static inline int battery_group_left(int text_width) {return (200-(110+10+text_width))/2;}

static inline int needle_tip(int value,int left,int span) {if(value<0)value=0;if(value>59)value=59;return left+value*span/59;}
