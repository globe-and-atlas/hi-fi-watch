"""Capture fixture Hi-Fi from the already-owned Emery emulator."""
from pathlib import Path
import argparse
import json
import socket
import subprocess
import time
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
PALETTES = [(0,0xFFAA00,0xFFAA00),(0,0xAAFFAA,0x00FFAA),(0x000055,0xAAFFFF,0x55AAFF),(0xAAAAAA,0,0),(0xFFFFFF,0,0xFF5500),(0x550000,0xFFFFAA,0xFFAAAA)]

def rgb(value):
    return ((value>>16)&255,(value>>8)&255,value&255)

def screenshot(port, peak=255):
    path=ROOT/'.tmp/native.ppm'
    path.unlink(missing_ok=True)
    with socket.create_connection(('localhost',port),timeout=5) as connection:
        connection.recv(65536)
        connection.sendall(f'screendump "{path.resolve()}"\n'.encode())
        deadline=time.monotonic()+5
        while time.monotonic()<deadline:
            connection.recv(65536)
            try:
                image=Image.open(path).convert('RGB');break
            except (FileNotFoundError,OSError):continue
        else:raise TimeoutError('Incomplete QEMU screendump')
    actual=max(max(pixel) for pixel in image.get_flattened_data())
    image=image.point(lambda channel:round(channel/max(actual,1)*peak/85)*85)
    path.unlink(missing_ok=True)
    assert image.size==(200,228)
    return image

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--monitor',type=int,required=True)
    options=parser.parse_args()
    keys=json.loads((ROOT/'watchface/build/js/message_keys.json').read_text())
    uuid=json.loads((ROOT/'watchface/package.json').read_text())['pebble']['uuid']
    media=ROOT/'media';media.mkdir(exist_ok=True)
    cases=[('amber',10,8,37,2,1,1,0),('mint',10,8,37,2,1,1,1),('ice',10,8,37,2,1,1,2),
           ('silver',10,8,37,2,1,1,3),('paper',10,8,37,2,1,1,4),('midnight',0,0,0,2,1,1,0),
           ('noon-12h',12,0,0,1,1,1,0),('eleven',11,11,59,2,1,1,0),('late',23,59,59,2,1,1,0),
           ('date-off',10,8,37,2,0,1,0),('date-iso',10,8,37,2,2,1,0),('date-us',10,8,37,2,3,1,0),
           ('date-eu',10,8,37,2,4,1,0),('seconds-off',10,8,37,2,1,0,0)]
    cases=[(*case,82) for case in cases]
    cases += [('custom',10,8,37,2,1,1,5,82),('midnight-12h',0,0,0,1,1,1,0,82),
              ('late-12h',23,59,59,1,1,1,0,82),('battery-empty',10,8,0,2,1,1,0,0),
              ('battery-full',10,8,59,2,1,1,0,100)]
    evidence=[]
    for name,hour,minute,second,format_,date,seconds,palette,battery in cases:
        values={keys['Palette']:palette,keys['TimeFormat']:format_,keys['DateFormat']:date,keys['ShowSeconds']:seconds,11000:hour,11001:minute,11002:second,11004:battery,keys['Background']:0x550000,keys['Foreground']:0xFFFFAA,keys['Accent']:0xFFAAAA}
        result=subprocess.run(['pebble','send-app-message','--emulator','emery','--app-uuid',uuid,'--int']+[f'{key}={value}' for key,value in values.items()],cwd=ROOT/'watchface',capture_output=True,text=True,timeout=45)
        if result.returncode:raise RuntimeError(result.stderr[-1500:])
        bg,fg,accent=PALETTES[palette]
        error=None
        for attempt in range(30):
            image=screenshot(options.monitor,max(max(rgb(color)) for color in (bg,fg,accent)))
            try:
                assert image.getpixel((0,0))==rgb(bg),'Background palette'
                for row,value in enumerate([second if seconds else 0,minute]):
                    for i in range(12):
                        if i<(value+1)//5:assert image.getpixel((48+12*i,128+22*row))==rgb(accent),'Active meter'
                        else:assert image.getpixel((48+12*i,128+22*row))!=rgb(accent),'Inactive meter'
                date_pixels=sum(image.getpixel((x,y))==rgb(fg) for y in range(175,194) for x in range(10,190))
                assert bool(date_pixels)==bool(date),'Date presence'
                points=[(x,y) for y in range(41,102) for x in range(200) if image.getpixel((x,y))==rgb(fg)]
                assert points,'Clock missing'
                left=min(x for x,y in points);right=max(x for x,y in points)
                assert abs((left+right)/2-99.5)<=.5,'Clock not optically centered'
                break
            except AssertionError as failure:error=failure;time.sleep(.05)
        else:raise AssertionError(f'{name}: {error}')
        image.save(media/f'hifi-{name}.png')
        if name=='amber':image.save(media/'hifi-default.png')
        evidence.append({'case':name,'ink_midpoint':(left+right)/2,'meters_verified':True})
        print('PASS: '+name,flush=True)
    (ROOT/'.tmp/native-checks.json').write_text(json.dumps(evidence,indent=2))
    board=Image.new('RGB',(5*220,258),(35,35,35));draw=ImageDraw.Draw(board)
    for i,name in enumerate(['amber','mint','ice','silver','paper']):
        board.paste(Image.open(media/f'hifi-{name}.png'),(10+i*220,0));draw.text((10+i*220,234),name,fill='white')
    board.save(media/'hifi-palettes.png')

if __name__=='__main__':main()
