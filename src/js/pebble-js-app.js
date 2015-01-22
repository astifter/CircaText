// Initialize variable selectedVersion from local storage.
var selectedVersion = window.localStorage.getItem('selectedVersion') ? window.localStorage.getItem('selectedVersion') : 'Regular';

// On showing the configuration, open webpage with stored selectedVersion.
Pebble.addEventListener('showConfiguration', function() {
    Pebble.openURL('http://astifter.github.io/CircaText/resources/configurable.html?selectedVersion=' + selectedVersion);
});

// On closing the configuration read the options store them appropriately and
// send them to the watchface.
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
