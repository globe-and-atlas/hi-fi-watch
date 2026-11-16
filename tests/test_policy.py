from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]

def test_clock_meters_battery(tmp_path):
    source = r'''#include <assert.h>
#include "policy.h"
int main(void) {
 for(int v=0;v<60;v++){assert(needle_tip(v,17,70)>=17&&needle_tip(v,17,70)<=87);if(v)assert(needle_tip(v,17,70)>=needle_tip(v-1,17,70));}
 assert(needle_tip(0,17,70)==17);assert(needle_tip(59,17,70)==87);assert(needle_tip(-1,17,70)==17);assert(needle_tip(60,17,70)==87);
 for(int hour=0;hour<24;hour++) {
  assert(display_hour(hour,2,0)==hour);
  assert(display_hour(hour,0,1)==hour);
  assert(display_hour(hour,1,1)==(hour%12?hour%12:12));
  assert(display_hour(hour,0,0)==(hour%12?hour%12:12));
  for(int minute=0;minute<60;minute++) {
   int left,right;clock_ink_bounds(hour,minute,&left,&right);
   int offset=clock_ink_offset(hour,minute);
   assert(left+offset>=0 && right+offset<200);
   assert(left+right+2*offset==199);
  }
 }
 assert(clock_ink_offset(10,8)==-13);
 assert(meter_segments(0)==0);
 assert(meter_segments(5)==1);
 assert(meter_segments(4)==1);
 assert(meter_segments(59)==12);
 assert(meter_segments(60)==12);
 for(int i=0;i<60;i++) assert(meter_segments(i)==(i+1)/5);
 for(int p=0;p<=100;p++) {
  int bars=battery_blocks(p);assert(bars>=0 && bars<=6);
  if(p==0) assert(bars==0);
  if(p>0) assert(bars>0);
  if(p==100) assert(bars==6);
 }
 for(int w=10;w<=55;w++) {
  int left=battery_group_left(w),right=left+120+w;
  assert(left>=0 && right<=200);
  assert(left+right>=199 && left+right<=200);
 }
 return 0;
}'''
    folder=ROOT/'.tmp/host-test';folder.mkdir(parents=True,exist_ok=True)
    (folder/'test.c').write_text(source)
    subprocess.run(['cc','-Wall','-Werror','-I',str(ROOT/'watchface/src/c'),str(folder/'test.c'),'-o',str(folder/'test')],check=True)
    subprocess.run([str(folder/'test')],check=True)
