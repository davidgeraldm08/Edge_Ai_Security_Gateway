let data_temp = ""

let data_color = ""

let data_smoke = ""



async function datafetch(value){

    try{
        const response = await fetch("http://192.168.1.19:5000/api/data/")

        const data = await response.json();
        console.log(data[1].id)

        data_temp = data[0].temp_log;

        data_color = data[1].color_log;

        data_smoke = data[0].gas_log;

        let temperature = data_temp;
  document.getElementById("myLabel").textContent = `Temperature: ${temperature}!`;

        let color = data_color;
  document.getElementById("myLabel2").textContent = `Current Color: ${color}!`;

        let smoke = data_smoke;
  document.getElementById("myLabel3").textContent = `Current Color: ${smoke}!`;


        let currentmood = data[1].color_log;
        document.documentElement.style.setProperty("--currentmood", currentmood);
        
    }
    catch{
        console.log("error ka man")
    }
}





  





