const label = document.getElementById("editableLabel");
const defaultText = "Name:"; 
let password = ""
let realemail = label.innerText; // eto yung variable ng email na ininput ng user
//email

//password
const label2 = document.getElementById("editableLabel2");
const defaultText2 = "Password:";
let realValue = ""; // eto yung variable ng password na ininput ng user


//email
label.addEventListener("focus", function() {
    if (label.innerText === defaultText) {
        label.innerText = "";
    }
});
label.addEventListener("blur", function() {
    if (label.innerText.trim() === "") {
        label.innerText = defaultText;
    }
});

//password
label2.addEventListener("focus", function() {
    if (label2.innerText === defaultText2) {
        label2.innerText = "";
    }
});
label2.addEventListener("blur", function() {
    if (label2.innerText.trim() === "") {
        label2.innerText = defaultText2;
    }
});

//password masking
function maskText(el) {
    let typed = el.innerText.replace(/\*/g, ""); 
    
    if (typed.length > 0) {
        
        realValue += typed;
        password = realValue;
    } else {
        realValue = realValue.slice(1,-1);
        password = realValue;
    }

    el.innerText = "*".repeat(realValue.length);

    const range = document.createRange();
    const sel = window.getSelection();
    range.selectNodeContents(el);
    range.collapse(false);
    sel.removeAllRanges();
    sel.addRange(range);
    
}

//fetching part hehe
async function authenticate(){
    realemail = label.innerText
    realValue = label2.innerText
    
    try {
        const response = await fetch(`http://192.168.1.19:5000/api/user/${realemail}/${password}`)
        const data = await response.json()
        goPage2()
    
    }catch(error){
        alert("Wrong Credentials");
    }   
  
}


function goPage2() {

    window.location.href = "http://127.0.0.1:5500/Front_End/Main_Page/dashboard.html";
    console.log(realValue);
    
};