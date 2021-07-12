/*------------------------------------------------------------------
* Bootstrap Simple Admin Template
* Version: 2.1
* Author: Alexis Luna
* Website: https://github.com/alexis-luna/bootstrap-simple-admin-template
-------------------------------------------------------------------*/
// Toggle sidebar on Menu button click
$('#sidebarCollapse').on('click', function () {
    $('#sidebar').toggleClass('active');
    $('#body').toggleClass('active');
});

var tx_url = "/GetTXData";
var nx_url = "/GetNXData";
function loadData() {
    jQuery.get(tx_url, function (data, status) {
        var array = data.split(',');
        var content =
        document.getElementById("tx_active").innerHTML = "Resumptions : " + array[0];
        document.getElementById("tx_suspended").innerHTML = "Suspentions : " + array[1];
        document.getElementById("idle_returns").innerHTML = "Idle Returns : " + array[2];
        document.getElementById("non_idle_returns").innerHTML = "Non Idle returns : " + array[3];
    });

    jQuery.get(nx_url, function (data, status) {
        var array = data.split(',');
        document.getElementById("nx_sent").innerHTML = "Total Bytes Sent  : " + array[0];
        document.getElementById("nx_received").innerHTML = "Total Bytes Received  :  " + array[1];
        document.getElementById("nx_connect").innerHTML = "Total connections  : " + array[2];
        document.getElementById("nx_disconnect").innerHTML = "Total Disconnections  :  " + array[3];


    });

    var t = setTimeout(function () { loadData() }, 3000);
}

var led_request = "/LedToggle";
function toggleLed() {

    var led = document.getElementById("greenLed");
    var label = document.getElementById("buttonLabel");
    if(led.checked){
        label.innerHTML = "On";
        jQuery.post("/LedOn");

    }
    else
    {
        label.innerHTML = "Off";
        jQuery.post("/LedOff");
    }
}
