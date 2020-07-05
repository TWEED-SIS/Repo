var updateInterval=100;	 
var gaugestate=true;

var config = {
	type: 'line',
	data: {
		labels:"" ,
		datasets: [{
			label: 'Temperature',
			
			backgroundColor: window.chartColors.red,
			borderColor: window.chartColors.red,
			data: 0,
			lineTension:0,
			hidden: false,
			fill: false,
		         },{
		        label: 'Humidity',
			backgroundColor: window.chartColors.blue,
			borderColor: window.chartColors.blue,
			data: 0,
			lineTension:0,
			hidden: false,
			fill: false,
				 
				 
		          }]
	      },
	options: {
		responsive: true,
		title: {
			display: true,
			text: 'DHT Sensor Chart'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			x: {
				display: true,
				scaleLabel: {
					display: true,
					labelString: 'sec'
				}
			},
			y: {
				display: true,
				scaleLabel:
				 {
					display: true,
					labelString: 'Value'
				 }
				 
			}
		}
	}
};



var tempgauge = new JustGage
    ({
    id: 'temperaturegauge',
    value: 0,
    min: 0,
    max: 100,
    title: "Temperature",
    customSectors: {
    percents: true, // lo and hi values are in %
    ranges: [{
    color : "#0000ff",
    lo : 0,
    hi : 50
    },
    {
    color : "#ff0000",
    lo : 51,
    hi : 100
    }]
    }, 
    relativeGaugeSize: false
    });
    

    
var humigauge = new JustGage
    ({
    id:'humiditegauge',
    value: 0,
    min: 0,
    max: 100,
    title: "Humidité",
    customSectors: {
    percents: true, // lo and hi values are in %
    ranges: [{
    color : "#0000ff",
    lo : 0,
    hi : 50
    },
    {
    color : "#ff0000",
    lo : 51,
    hi : 100
    }]
    }, 
    relativeGaugeSize: false
    });	




function addData(addresult){
	if(addresult['temp']!=null){
	        
	    canvasChartInstance.data.labels.push(addresult['date']);
        canvasChartInstance.data.datasets[0].data.push(addresult['temp']);
        canvasChartInstance.data.datasets[1].data.push(addresult['humi']);

        tempgauge.refresh(addresult['temp']);
        humigauge.refresh(addresult['humi']);
      
        
             
	  if(canvasChartInstance.data.labels.length>20){
		 canvasChartInstance.data.labels.shift(); 
		 canvasChartInstance.data.datasets[0].data.shift(); 
		 canvasChartInstance.data.datasets[1].data.shift();  
		
	  }
	  
	    canvasChartInstance.update();
	  }
	
}


function updategraphe(){
   console.log("Update Data"); 
   $.getJSON("assets/php/dhtlastdata.php", addData); 
   setTimeout(updategraphe,updateInterval);
	
}



function init(result)
{
canvasChartInstance.data.labels=result['labels'];
canvasChartInstance.data.datasets[0].data=result['temperature'];
canvasChartInstance.data.datasets[1].data=result['humidity'];
canvasChartInstance.update();

tempgauge.refresh(result['gaugetemperature']);
humigauge.refresh(result['gaugehumidity']);

document.getElementById("tempvaluetext").innerHTML = "Temperature:"+result['gaugetemperature']+"C";
document.getElementById("humivaluetext").innerHTML = "Temperature:"+result['gaugehumidity']+"%";
}




function initdata(canvasChartInstance){
$.getJSON("assets/php/DHTinitdata.php", init); 
updategraphe();	
}


window.onload = function() 
{	
var ctx = document.getElementById('dhtchart');
canvasChartInstance= new Chart(ctx, config);
initdata(canvasChartInstance);

};
