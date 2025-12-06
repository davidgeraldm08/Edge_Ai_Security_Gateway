
let data_temp = "";
let data_color = "";
let data_smoke = "";

async function datafetch(value){
    try {
        const response = await fetch("http://192.168.1.119:5000/api/live/");
        const data = await response.json();
        console.log(data);

        let red = data.red;
        let green = data.green;
        let blue = data.blue;

        if (red < green && red < blue){
            console.log("Red")
        }else if (green < red && green < blue){
            console.log("Green")
        }else if (blue < red && blue < green){ 
            console.log("Blue")
        }

        document.getElementById("myLabel").textContent = `Temperature: ${data_temp}!`;
        document.getElementById("myLabel2").textContent = `Current Mood: ${data_color}!`;
        document.getElementById("myLabel3").textContent = `Smoke/Gas Level: ${data_smoke}!`;

        let currentmood = data_color;
        document.documentElement.style.setProperty("--currentmood", currentmood);
        
    } catch(error) {
        console.log(error);
    }
}



/* ---------------------------
   CAROUSEL SYSTEM
----------------------------*/

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
