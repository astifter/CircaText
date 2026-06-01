// Initialize variable selectedVersion from local storage.
var selectedVersion = window.localStorage.getItem('selectedVersion') ? window.localStorage.getItem('selectedVersion') : 'Regular';
var batteryDisplay = window.localStorage.getItem('batteryDisplay') ? window.localStorage.getItem('batteryDisplay') : '7';

// On showing the configuration, open webpage with stored selectedVersion.
Pebble.addEventListener('showConfiguration', function() {
    Pebble.openURL('http://astifter.github.io/CircaText/resources/configurable.1.4.html?selectedVersion=' + selectedVersion + '&batteryDisplay=' + batteryDisplay);
});

// On closing the configuration read the options store them appropriately and
// send them to the watchface.
Pebble.addEventListener('webviewclosed', function(e) {
    var options = JSON.parse(decodeURIComponent(e.response));
    selectedVersion = encodeURIComponent(options.selectedVersion);
    batteryDisplay = encodeURIComponent(options.batteryDisplay);
    //console.log('selectedVersion: '+selectedVersion);
    //console.log('batteryDisplay: '+batteryDisplay);
  
    window.localStorage.setItem('selectedVersion', selectedVersion);
    window.localStorage.setItem('batteryDisplay', batteryDisplay);
    //console.log('stored selected version');
    var transactionId = Pebble.sendAppMessage({
      "selectedVersion": decodeURIComponent(selectedVersion),
      "batteryDisplay": decodeURIComponent(batteryDisplay),
    });
    //console.log('transaction id is: ' + transactionId);
});
