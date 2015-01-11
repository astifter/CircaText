var locationOptions = {timeout: 15000, maximumAge: 60000},
    selectedVersion = window.localStorage.getItem('selectedVersion') ? window.localStorage.getItem('selectedVersion') : 'Regular'; 

Pebble.addEventListener('showConfiguration', function() {
    Pebble.openURL('https://www.googledrive.com/host/0B97_0Rk-p4m0WVhRcFVlc2xPM00?selectedVersion=' + selectedVersion);
});

Pebble.addEventListener('webviewclosed', function(e) {
    var options = JSON.parse(decodeURIComponent(e.response));
    selectedVersion = encodeURIComponent(options.selectedVersion);
    //console.log('selectedVersion: '+selectedVersion);
  
    window.localStorage.setItem('selectedVersion', selectedVersion);
    //console.log('stored selected version');
    var transactionId = Pebble.sendAppMessage({
      "selectedVersion": decodeURIComponent(selectedVersion)
    });
    //console.log('transaction id is: ' + transactionId);
});
