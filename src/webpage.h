#pragma once

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>SolarVault</title>

<style>
:root{
 --bg:#1b1f2a;
 --panel:#252b3b;
 --border:#343c52;
 --text:#d6deff;
 --muted:#9aa7c7;
 --accent:#6ea8fe;
}

*{box-sizing:border-box;margin:0;padding:0}

body{
 font-family:Segoe UI,system-ui;
 background:var(--bg);
 color:var(--text);
 padding:20px;
}

/* Header */
.header{
 text-align:center;
 margin-bottom:30px;
}

.header h2{
 font-weight:600;
 margin-bottom:6px;
}

.header small{
 color:var(--muted);
}

/* Cards */
.card{
 background:var(--panel);
 padding:20px;
 border-radius:14px;
 border:1px solid var(--border);
 margin-bottom:18px;
}

.card h3{
 font-size:12px;
 color:var(--muted);
 margin-bottom:8px;
 text-transform:uppercase;
 letter-spacing:0.8px;
}

.value{
 font-size:22px;
 font-weight:600;
}

/* Buttons */
button{
 background:var(--accent);
 border:none;
 padding:8px 14px;
 border-radius:8px;
 cursor:pointer;
 font-weight:600;
 color:#0f1422;
 margin-right:10px;
 margin-top:8px;
}

button:active{
 transform:scale(0.98);
}

/* Slider */
input[type=range]{
 width:100%;
 margin-top:10px;
}

/* Charts */
canvas{
 width:100%;
 height:220px;
 background:#1f2436;
 border-radius:12px;
 border:1px solid var(--border);
}

/* Desktop layout enhancement */
@media (min-width:768px){
 .grid{
  display:grid;
  grid-template-columns:repeat(2,1fr);
  gap:20px;
 }
}
</style>
</head>

<body>

<div class="header">
 <h2>Solar Tracker Monitoring</h2>
 <small>esp32-solar.local</small>
</div>

<div class="grid">

<div class="card">
 <h3>Panel Voltage</h3>
 <div class="value" id="voltage">0 V</div>
</div>

<div class="card">
 <h3>Power Output</h3>
 <div class="value" id="power">0 W</div>
</div>

<div class="card">
 <h3>Servo Angle</h3>
 <div class="value" id="angle">0°</div>
</div>

<div class="card">
 <h3>Mode</h3>
 <div class="value" id="mode">AUTO</div>
</div>

</div>

<div class="card">
 <h3>LDR Sensor Readings</h3>
 <div id="ldr" style="color:var(--muted)">0 | 0 | 0 | 0</div>
</div>

<div class="card">
 <h3>Manual Override</h3>
 <button onclick="setMode('auto')">AUTO</button>
 <button onclick="setMode('manual')">MANUAL</button>
 <input type="range" min="0" max="180" oninput="setAngle(this.value)">
</div>

<div class="card">
 <h3>Live Voltage History</h3>
 <canvas id="liveChart"></canvas>
</div>

<div class="card">
 <h3>Weather Simulation</h3>
 <canvas id="weatherChart"></canvas>
 <button onclick="weather('sunny')">Sunny</button>
 <button onclick="weather('rainy')">Rainy</button>
 <button onclick="weather('foggy')">Foggy</button>
</div>

<script>

let liveCtx=document.getElementById("liveChart").getContext("2d");
let weatherCtx=document.getElementById("weatherChart").getContext("2d");

/* ===== Chart Draw ===== */

function drawChart(ctx,data,color){

 let w=ctx.canvas.width;
 let h=ctx.canvas.height;

 ctx.clearRect(0,0,w,h);

 let max=Math.max(...data);
 let min=Math.min(...data);
 if(max===min){ max+=1; min-=1; }

 ctx.beginPath();

 for(let i=0;i<data.length;i++){
  let x=i*(w/(data.length-1));
  let norm=(data[i]-min)/(max-min);
  let y=h-(norm*h*0.85)-10;

  if(i===0) ctx.moveTo(x,y);
  else ctx.lineTo(x,y);
 }

 ctx.strokeStyle=color;
 ctx.lineWidth=2;
 ctx.stroke();
}

/* ===== Live Data ===== */

setInterval(()=>{
 fetch("/data")
 .then(r=>r.json())
 .then(d=>{
   document.getElementById("voltage").innerText=d.voltage.toFixed(2)+" V";
   document.getElementById("power").innerText=d.power.toFixed(2)+" W";
   document.getElementById("angle").innerText=d.angle+"°";
   document.getElementById("ldr").innerText=d.ldr.join(" | ");
   document.getElementById("mode").innerText=d.mode.toUpperCase();
 });

 fetch("/history")
 .then(r=>r.json())
 .then(d=>{
   drawChart(liveCtx,d,"#6ea8fe");
 });

},1000);

/* ===== Weather Profiles ===== */

function sunny(){
 let arr=[];
 for(let i=0;i<60;i++){
   let hour=i/60;
   let irradiance=Math.exp(-Math.pow((hour-0.5)*3,2));
   arr.push(irradiance*10.5);
 }
 return arr;
}

function rainy(){
 let arr=[];
 for(let i=0;i<60;i++){
   let base=2+Math.random()*1.2;
   if(Math.random()>0.9) base+=0.8;
   arr.push(base);
 }
 return arr;
}

function foggy(){
 let arr=[];
 for(let i=0;i<60;i++){
   let hour=i/60;
   let mid=Math.exp(-Math.pow((hour-0.5)*4,2))*2;
   arr.push(4+mid);
 }
 return arr;
}

function weather(type){
 if(type==="sunny") drawChart(weatherCtx,sunny(),"#6ea8fe");
 if(type==="rainy") drawChart(weatherCtx,rainy(),"#6ea8fe");
 if(type==="foggy") drawChart(weatherCtx,foggy(),"#6ea8fe");
}

weather("sunny");

function setMode(m){ fetch("/control?mode="+m); }
function setAngle(v){ fetch("/control?angle="+v); }

</script>

</body>
</html>
)rawliteral";