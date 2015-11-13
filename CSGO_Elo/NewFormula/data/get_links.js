var page = require('webpage').create();

page.open('http://www.hltv.org/matches/archive/', function(status){
	if (status != "success") {
		console.log("ERROR: Cannot load page" );
	} else {
		page.includeJs("http://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js", function() {
			for (i = 0; i < 12; i += 1) {
				console.log(i);
				page.evaluate(function() {
					var month = i+1;
 				    var date = "2015-" + month + "-01 to 2015-" + month + "-15";
					document.getElementById("calendar").setAttribute("value", date);
					$(document.getElementById("search")).click();
    			});

				page.evaluate(function() {
					var month_days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
					var date = "2015-" + (i+1) + "-16 to 2015-" + (i+1) + "-" + month_days[i];
					document.getElementById("calendar").setAttribute("value", date);
				});
			}

			phantom.exit()
		});
	}
});