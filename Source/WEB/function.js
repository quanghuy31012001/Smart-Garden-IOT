
// gas
let eq1on = document.querySelector('#eqon');
let eq1off = document.querySelector('#eqoff');
// functions
eq1on.addEventListener('click', ()=>{
    firebase.database().ref("eq1").set({device: 1})
})
eq1off.addEventListener('click', ()=>{
    firebase.database().ref("eq1").set({device: 0})
})

