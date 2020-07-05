
var updateInterval=100;	 



var config = {
	type: 'line',
	data: {
		labels:"" ,
		datasets: [{
			label: 'Soil Humidité',
			
			backgroundColor: window.chartColors.red,
			borderColor: window.chartColors.red,
			data: 0,
			lineTension:0,
			hidden: false,
			fill: false,
		         },{
		        label: 'Soil ph',
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
			text: 'Soil Sensors Chart'
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

var soilhumgage = new JustGage ({
    id: 'soilhumiditygauge',
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
   
var soilphgage = new JustGage   ({
    id:'soilphgauge',
    value: 0,
    min: 0,
    max: 100,
    title: "PH",
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
	if(addresult['humi']!=null){       
	    canvasChartInstance.data.labels.push(addresult['datehum']);
	    
	   var temp={x:addresult['datehum'],y:addresult['humi']};
        canvasChartInstance.data.datasets[0].data.push(temp);
        soilhumgage.refresh(addresult['humi']);     
	  if(canvasChartInstance.data.labels.length>20)
	  {
		 canvasChartInstance.data.labels.shift(); 
		 canvasChartInstance.data.datasets[0].data.shift(); 
	  }
	  
	    canvasChartInstance.update();
	  }
	  
	  
	  
	  if(addresult['ph']!=null){       
	    canvasChartInstance.data.labels.push(addresult['dateph']);
	     var temp={x:addresult['dateph'],y:addresult['ph']};
        canvasChartInstance.data.datasets[1].data.push(temp);
        soilphgage.refresh(addresult['ph']);     
	  if(canvasChartInstance.data.labels.length>20)
	  {
		 canvasChartInstance.data.labels.shift(); 
		 canvasChartInstance.data.datasets[1].data.shift(); 
	  }
	  
	    canvasChartInstance.update();
	  }
	
}
















function updategraphe(){
   console.log("Update Data"); 
   $.getJSON("../php/Soillastdata.php", addData); 
   setTimeout(updategraphe,updateInterval);
	
}



function init(result)
{
var sdata=[];
var soildata=[];
var phdata=[];
var k=0;
var j=0;
for(var i=0;i<20;i++){
	
	if(i%2==0)
	    {
		soildata[j]={x:result['labels'][i],y:result['SoilHumidite'][j]};
	    j++;
	    
	    }
		else
		{
			phdata[k]={x:result['labels'][i],y:result['SoilPh'][k]};
			k++
		}
} 
canvasChartInstance.data.labels=result['labels'];    
canvasChartInstance.data.datasets[0].data=soildata;      
canvasChartInstance.data.datasets[1].data=phdata;       
canvasChartInstance.update(); 

                
document.getElementById("soilhumvaluetext").innerHTML = "Humidité:"+result['humiditegauge']+"%";
document.getElementById("soilphvaluetext").innerHTML = "Ph:"+result['phgauge']+" ";                   
soilhumgage.refresh(result['humiditegauge']);
soilphgage.refresh(result['phgauge']);

            
console.log("success"); 
//canvasChartInstance.data.labels=result['labels'];
//canvasChartInstance.data.datasets[0].data=result['temperature'];
//canvasChartInstance.data.datasets[1].data=result['humidity'];
canvasChartInstance.update();

}




function initdata(canvasChartInstance){
$.getJSON("../php/Soilinitdata.php", init); 
updategraphe();	
}


window.onload = function() 
{	
var ctx = document.getElementById('soilsensorschart');
canvasChartInstance= new Chart(ctx, config);
initdata(canvasChartInstance);

};
