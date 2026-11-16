var Clay=require('@rebble/clay');
function needleDefault(){try{var p=Pebble.getActiveWatchInfo().platform;return p==='chalk'||p==='gabbro'?1:0;}catch(e){return 0;}}
var defaults={TimeFormat:0,DateFormat:1,ShowSeconds:1,Palette:0,Background:0,Foreground:0xFFAA00,Accent:0xFFAA00,NeedleDials:needleDefault()};
function integer(value,fallback,maximum) {
 if(value===undefined || value===null || value==='' || typeof value==='boolean') return fallback;
 var n=Number(value);return isFinite(n) && Math.floor(n)===n && n>=0 && n<=maximum?n:fallback;
}
function normalize(raw) {
 raw=raw||{};return {TimeFormat:integer(raw.TimeFormat,0,2),DateFormat:integer(raw.DateFormat,1,4),
 ShowSeconds:raw.ShowSeconds===false?0:raw.ShowSeconds===true?1:integer(raw.ShowSeconds,1,1),
 Palette:integer(raw.Palette,0,5),Background:integer(raw.Background,0,0xFFFFFF),Foreground:integer(raw.Foreground,0xFFAA00,0xFFFFFF),Accent:integer(raw.Accent,0xFFAA00,0xFFFFFF),NeedleDials:raw.NeedleDials===true?1:raw.NeedleDials===false?0:integer(raw.NeedleDials,needleDefault(),1)};
}
var clay=new Clay([
 {type:'heading',defaultValue:'Hi-Fi'},
 {type:'text',defaultValue:'A stereo-inspired clock. SEC follows seconds through the minute; MIN follows minutes through the hour. Needle dials sweep from00 to59; bar mode counts five-second or five-minute intervals. Both reset at00.'},
 {type:'toggle',messageKey:'NeedleDials',label:'Needle dials',defaultValue:!!needleDefault()},
 {type:'toggle',messageKey:'ShowSeconds',label:'Seconds meter',defaultValue:true},
 {type:'text',defaultValue:'Turn off seconds for minute-only updates. The SEC meter is then marked OFF and its needle is hidden.'},
 {type:'select',messageKey:'TimeFormat',label:'Time format',defaultValue:'0',options:[
  {label:'Watch default',value:'0'},{label:'12-hour',value:'1'},{label:'24-hour',value:'2'}]},
 {type:'select',messageKey:'DateFormat',label:'Date',defaultValue:'1',options:[
  {label:'Off',value:'0'},{label:'THU NOV 05',value:'1'},{label:'YYYY-MM-DD',value:'2'},
  {label:'MM/DD/YYYY',value:'3'},{label:'DD/MM/YYYY',value:'4'}]},
 {type:'heading',defaultValue:'Colors'},
 {type:'select',messageKey:'Palette',label:'Palette',defaultValue:'0',options:[
  {label:'Amber',value:'0'},{label:'Mint',value:'1'},{label:'Ice',value:'2'},
  {label:'Silver',value:'3'},{label:'Paper',value:'4'},{label:'Custom colors',value:'5'}]},
 {type:'text',defaultValue:'Select Custom colors to apply the choices below. Use contrasting colors.'},
 {type:'color',messageKey:'Background',label:'Background',defaultValue:'000000',sunlight:false,allowGray:true},
 {type:'color',messageKey:'Foreground',label:'Digits and labels',defaultValue:'FFAA00',sunlight:false,allowGray:true},
 {type:'color',messageKey:'Accent',label:'Meters and rules',defaultValue:'FFAA00',sunlight:false,allowGray:true},
 {type:'submit',defaultValue:'Save to watch'}
],null,{autoHandleEvents:false});
function sendSaved() {
 var raw=localStorage.getItem('clay-settings');if(!raw) return;
 try {Pebble.sendAppMessage(normalize(JSON.parse(raw)));} catch(error) {console.log('Hi-Fi settings could not be loaded');}
}
Pebble.addEventListener('ready',sendSaved);
Pebble.addEventListener('showConfiguration',function() {Pebble.openURL(clay.generateUrl());});
Pebble.addEventListener('webviewclosed',function(event) {
 if(!event || !event.response || event.response==='CANCELLED') return;
 try {clay.getSettings(event.response,false);sendSaved();} catch(error) {console.log('Hi-Fi settings were not saved');}
});
module.exports={normalize:normalize,defaults:defaults};
