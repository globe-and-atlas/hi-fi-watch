"""Show native Hi-Fi meter options using exact2x pixel enlargement."""
from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'.tmp/needle-native';BG=(235,231,218)
FONT=ImageFont.truetype('/System/Library/Fonts/Menlo.ttc',20);SMALL=ImageFont.truetype('/System/Library/Fonts/Menlo.ttc',14)
board=Image.new('RGB',(920,570),BG);d=ImageDraw.Draw(board);d.text((20,15),'HI-FI / TIME 2 METER OPTIONS',font=FONT,fill='#161916')
for i,(name,label) in enumerate([('bars','BARS / current default'),('needles','NEEDLE DIALS / new option')]):
 x=20+i*460;im=Image.open(OUT/'emery'/f'{name}.png');d.text((x,56),label,font=SMALL,fill='#454a45');board.paste(im.resize((400,456),Image.Resampling.NEAREST),(x,90))
board.save(OUT/'time2-comparison.png')
board=Image.new('RGB',(1660,740),BG);d=ImageDraw.Draw(board);d.text((34,22),'HI-FI / NEEDLE DIALS ENABLED',font=FONT,fill='#161916');d.text((34,58),'Native emulator captures / exact2x enlargement / SEC and MIN show real time values',font=SMALL,fill='#454a45')
for i,(p,label) in enumerate([('emery','TIME 2'),('chalk','TIME ROUND'),('gabbro','ROUND 2')]):
 im=Image.open(OUT/p/'needles.png').convert('RGB')
 if p!='emery':
  mask=Image.new('L',im.size);ImageDraw.Draw(mask).ellipse((0,0,im.width-1,im.height-1),fill=255);canvas=Image.new('RGB',im.size,BG);canvas.paste(im,(0,0),mask);im=canvas
 im=im.resize((im.width*2,im.height*2),Image.Resampling.NEAREST);cx=[260,780,1330][i];d.text((cx-im.width//2,112),label,font=SMALL,fill='#454a45');board.paste(im,(cx-im.width//2,150))
board.save(OUT/'three-display-needles.png');print('Native meter comparisons saved')
