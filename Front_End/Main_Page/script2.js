document.addEventListener('DOMContentLoaded', () => {
    setInterval(datafetch, 1000);
});

async function datafetch(){
    try {
        const response = await fetch("http://172.20.10.6:5000/api/live/");
        const data = await response.json();
        console.log(data);

         let color_array = data.color_log.split(",");

        let red = Number(normalize(color_array[0]));
        let green = Number(normalize(color_array[1]));
        let blue = Number(normalize(color_array[2]));

        let rgbValue = `rgb(${red}, ${green}, ${blue})`;
        console.log(red+" "+green+" "+blue)
        if (red < green && red < blue){
            document.getElementById("myLabel2").textContent = `Current Mood: Romantic`;
             myLabel2.style.backgroundColor = `rgb(255,0,0)`;
            console.log("currentmood now is: ", 
    getComputedStyle(document.documentElement).getPropertyValue("--currentmood")
);
        }else if (green < red && green < blue){
            document.getElementById("myLabel2").textContent = `Current Mood: Tranquil`;
             myLabel2.style.backgroundColor = `rgb(0,255,0)`;
            console.log("currentmood now is: ", 
    getComputedStyle(document.documentElement).getPropertyValue("--currentmood")
);
        }else if (blue < red && blue < green){ 
            document.getElementById("myLabel2").textContent = `Current Mood: Sleepy`;
             myLabel2.style.backgroundColor = `rgb(0,0,255)`;
            console.log("currentmood now is: ", 
    getComputedStyle(document.documentElement).getPropertyValue("--currentmood")
);
            }

        if(data.gas_log){
            document.getElementById("myLabel3").textContent = `SMOKE DETECTED`;
        }
        else{
            document.getElementById("myLabel3").textContent = `Safe`;
        }
        document.getElementById("myLabel").textContent = `Temperature: ${data.temp_log}!`;
        
        
    } catch(error) {
        console.log(error);
    }
}



/* ---------------------------
   CAROUSEL SYSTEM
----------------------------*/

function normalize(value, max = 2000) {
    return Math.min(255, Math.floor((value / max) * 255));
}

let index = 0;

const track = document.getElementById("track");
const prevBtn = document.getElementById("prevBtn");
const nextBtn = document.getElementById("nextBtn");

prevBtn.onclick = () => moveSlide(-1);
nextBtn.onclick = () => moveSlide(1);

function moveSlide(direction) {
  index += direction;
  if (index < 0) index = 2;   // wrap to last slide
  if (index > 2) index = 0;   // wrap to first slide
  track.style.transform = `translateX(-${index * 100}vw)`;
}
