"""Verify Hi-Fi's selectable native meters on the already-owned emulator."""
from pathlib import Path
import argparse,json,time
from native_io import ROOT,run,capture,monitor_port
SIZES={'emery':(200,228),'chalk':(180,180),'gabbro':(260,260)}
def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--platform',choices=list(SIZES),required=True);opt=parser.parse_args()
 keys=json.loads((ROOT/'watchface/build/js/message_keys.json').read_text());uuid=json.loads((ROOT/'watchface/package.json').read_text())['pebble']['uuid'];port=monitor_port();out=ROOT/'.tmp/needle-native'/opt.platform;out.mkdir(parents=True,exist_ok=True)
 def send(v):run(['pebble','send-app-message','--emulator',opt.platform,'--app-uuid',uuid,'--int']+[f'{keys.get(k,k)}={value}' for k,value in v.items()]);time.sleep(.12)
 defaults={'TimeFormat':2,'DateFormat':1,'ShowSeconds':1,'Palette':0,'NeedleDials':0,11000:10,11001:8,11002:37,11004:82}
 cases=[('bars',defaults),('needles',{'NeedleDials':1}),('needle-zero',{11001:0,11002:0}),('needle-middle',{11001:30,11002:30}),('needle-full',{11001:59,11002:59}),('needle-seconds-off',{'ShowSeconds':0}),('bar-seconds-off',{'NeedleDials':0}),('needle-iso',{'NeedleDials':1,'DateFormat':2,'ShowSeconds':1,11001:8,11002:37}),('needle-mint',{'DateFormat':1,'Palette':1}),('needle-ice',{'Palette':2}),('needle-paper',{'Palette':4}),('needle-custom',{'Palette':5,'Background':0x550000,'Foreground':0xFFFFAA,'Accent':0xFFAAAA})]
 images={}
 for name,values in cases:
  send(values);im=capture(port);assert im.size==SIZES[opt.platform];
  if name=='bars':
   point=(48,128) if opt.platform=='emery' else (22*im.width//180,49*im.width//180)
   assert im.getpixel(point)==(255,170,0),'Hi-Fi not foregrounded: select Hi-Fi in emulator Watchfaces before capture'
  images[name]=im;im.save(out/f'{name}.png');print('CAPTURED',opt.platform,name,flush=True)
 assert images['bars'].tobytes()!=images['needles'].tobytes(),'Meter toggle has no visual effect'
 # Different seconds must alter SEC only; minutes/display stay exactly fixed.
 send(defaults);send({'NeedleDials':1,11002:0});start=capture(port);send({11002:59});end=capture(port)
 secbox=(10,121,95,162) if opt.platform=='emery' else tuple(round(v*im.width/180) for v in (16,39,84,74))
 for y in range(start.height):
  for x in range(start.width):
   if secbox[0]<=x<secbox[2] and secbox[1]<=y<secbox[3]:continue
   assert start.getpixel((x,y))==end.getpixel((x,y)),'Seconds changed non-SEC content'
 assert start.tobytes()!=end.tobytes(),'SEC needle is stationary'
 send({'ShowSeconds':0,11002:0});off=capture(port);send({11002:59});assert capture(port).tobytes()==off.tobytes(),'Seconds Off retains a moving needle'
 # Reinstall must preserve explicit toggle choices and existing appearance settings.
 for style in [1,0]:
  send({'NeedleDials':style,'ShowSeconds':1,11002:30,11001:8})
  expected=capture(port);run(['pebble','install','--emulator',opt.platform,str(ROOT/'.tmp/round-qa/fixture.pbw')]);deadline=time.monotonic()+12
  while True:
   restored=capture(port)
   if restored.tobytes()==expected.tobytes():break
   if time.monotonic()>deadline:raise AssertionError('Meter setting not restored after reinstall')
   time.sleep(.15)
  restored.save(out/('persisted-needles.png' if style else 'persisted-bars.png'))
 (out/'checks.json').write_text(json.dumps({'platform':opt.platform,'cases':[name for name,v in cases],'toggle_changes_meters':True,'sec_motion_isolated':True,'seconds_off_static':True,'needle_toggle_persists':True},indent=2))
 send(defaults);print('PASS native needle toggle',opt.platform,flush=True)
if __name__=='__main__':main()
