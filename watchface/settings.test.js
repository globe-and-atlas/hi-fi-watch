var assert=require('assert'),Module=require('module'),path=require('path');
var original=Module._load;
var keys={TimeFormat:10000,DateFormat:10001,ShowSeconds:10002,Palette:10003,Background:10004,Foreground:10005,Accent:10006,NeedleDials:10007};
var bundle=path.join(__dirname,'node_modules/@rebble/clay/src/js/index.js');
var config;
Module._load=function(name) {
  if(name==='message_keys') return keys;
  if(name==='@rebble/clay') {
    var ActualClay=original.call(this,bundle);
    return function(items,custom,options) {config=items;return new ActualClay(items,custom,options);};
  }
  return original.apply(this,arguments);
};
var events={},storage={},sent=[];
global.localStorage={getItem:function(k) {return storage[k]||null;},setItem:function(k,v) {storage[k]=v;}};
global.Pebble={
  addEventListener:function(k,fn) {(events[k]||(events[k]=[])).push(fn);},
  getAccountToken:function() {return 'test';},getWatchToken:function() {return 'test';},
  getActiveWatchInfo:function() {return {platform:'emery',model:'emery',language:'en_US',firmware:{major:4,minor:33,patch:1}};},
  sendAppMessage:function(value) {sent.push(value);},openURL:function(url) {assert(url.indexOf('data:text/html')===0);}
};
var settings=require('./src/pkjs/index');
function fire(name,event) {(events[name]||[]).forEach(function(fn) {fn(event);});}
assert.deepStrictEqual(settings.normalize({}),settings.defaults);
assert.strictEqual(settings.normalize({ShowSeconds:false}).ShowSeconds,0);
assert.strictEqual(settings.normalize({ShowSeconds:true}).ShowSeconds,1);
assert.strictEqual(settings.normalize({ShowSeconds:0}).ShowSeconds,0);
for(var t=0;t<=2;t++) assert.strictEqual(settings.normalize({TimeFormat:String(t)}).TimeFormat,t);
for(var d=0;d<=4;d++) assert.strictEqual(settings.normalize({DateFormat:String(d)}).DateFormat,d);
fire('ready');assert.strictEqual(sent.length,0);fire('showConfiguration');
fire('webviewclosed',{response:''});fire('webviewclosed',{response:'CANCELLED'});fire('webviewclosed',{response:'invalid'});
assert.strictEqual(sent.length,0);
var raw={TimeFormat:{value:'2'},DateFormat:{value:'4'},ShowSeconds:{value:false},Palette:{value:'5'},Background:{value:0},Foreground:{value:0xAAFFAA},Accent:{value:0x55AAFF}};
fire('webviewclosed',{response:encodeURIComponent(JSON.stringify(raw))});
assert.deepStrictEqual(sent[0],{TimeFormat:2,DateFormat:4,ShowSeconds:0,Palette:5,Background:0,Foreground:0xAAFFAA,Accent:0x55AAFF,NeedleDials:0});
fire('ready');assert.deepStrictEqual(sent[0],sent[1]);
for(var p=0;p<=5;p++) assert.strictEqual(settings.normalize({Palette:String(p)}).Palette,p);
assert.strictEqual(settings.normalize({Palette:6}).Palette,0);
assert.strictEqual(settings.normalize({Accent:0x1000000}).Accent,0xFFAA00);
assert.strictEqual(settings.normalize({Background:-1}).Background,0);
assert.strictEqual(settings.normalize({TimeFormat:1.5}).TimeFormat,0);
assert.strictEqual(settings.normalize({DateFormat:'oops'}).DateFormat,1);
var dates=config.filter(function(item) {return item.messageKey==='DateFormat';})[0].options;
assert.strictEqual(dates.length,5);
console.log('PASS: date/time formats, seconds toggle, colors, real Clay Save/Cancel, reload');

assert.strictEqual(settings.normalize({NeedleDials:true}).NeedleDials,1);assert.strictEqual(settings.normalize({NeedleDials:false}).NeedleDials,0);assert.strictEqual(settings.normalize({NeedleDials:'1'}).NeedleDials,1);assert.strictEqual(settings.normalize({NeedleDials:2}).NeedleDials,0);
raw.NeedleDials={value:true};fire('webviewclosed',{response:encodeURIComponent(JSON.stringify(raw))});assert.strictEqual(sent[sent.length-1].NeedleDials,1);fire('ready');assert.strictEqual(sent[sent.length-1].NeedleDials,1);
raw.NeedleDials={value:false};fire('webviewclosed',{response:encodeURIComponent(JSON.stringify(raw))});assert.strictEqual(sent[sent.length-1].NeedleDials,0);
['chalk','gabbro'].forEach(function(platform){var prior=Pebble.getActiveWatchInfo;Pebble.getActiveWatchInfo=function(){return {platform:platform};};assert.strictEqual(settings.normalize({}).NeedleDials,1);assert.strictEqual(settings.normalize({NeedleDials:false}).NeedleDials,0);Pebble.getActiveWatchInfo=prior;});
assert.strictEqual(config.filter(function(c){return c.messageKey==='NeedleDials';}).length,1);assert.deepStrictEqual(require('./package.json').pebble.messageKeys.slice(0,7),Object.keys(keys).slice(0,7));console.log('PASS: needle Save/Off/reload, per-platform legacy defaults and stable keys');

['emery','chalk','gabbro'].forEach(function(platform){Pebble.getActiveWatchInfo=function(){return {platform:platform};};delete require.cache[require.resolve('./src/pkjs/index')];var fresh=require('./src/pkjs/index'),wanted=platform==='emery'?0:1;assert.strictEqual(fresh.defaults.NeedleDials,wanted);assert.strictEqual(fresh.normalize({}).NeedleDials,wanted);assert.strictEqual(config.filter(function(c){return c.messageKey==='NeedleDials';})[0].defaultValue,!!wanted);});console.log('PASS: fresh companion startup and Clay defaults for all three target platforms');
