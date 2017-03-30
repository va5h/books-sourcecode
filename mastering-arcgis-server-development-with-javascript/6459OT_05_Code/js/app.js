require([
  "dojo/parser",
  "dojo/dom",
  "dojo/dom-construct",
  "dojo/dom-style",
  "dojo/on",
  "esri/config",
  "esri/map",
  "esri/graphic",
  "esri/layers/FeatureLayer",
  "esri/layers/ArcGISDynamicMapServiceLayer",
  "esri/symbols/SimpleMarkerSymbol",
  "esri/geometry/Extent",
  "esri/dijit/editing/Editor",
  "esri/dijit/editing/TemplatePicker",
  "esri/dijit/AttributeInspector",
  "esri/dijit/LocateButton",
  "esri/tasks/query",
  "dijit/form/Button",
  "dijit/layout/BorderContainer", 
  "dijit/layout/ContentPane", 
  "dojo/domReady!"
], function (
	parser, dojoDom, domConstruct, domStyle, dojoOn,
	esriConfig, Map, Graphic,
	FeatureLayer, ArcGISDynamicMapServiceLayer,
	MarkerSymbol, Extent,
	Editor, TemplatePicker, AttributeInspector, 
	LocateButton, Query, Button
) {

  var maxExtent = new Extent({
  	"xmin":-13519092.335425414,
  	"ymin":4413224.664902497,
  	"xmax":-13507741.43672508,
  	"ymax":4421766.502813354,
  	"spatialReference":{"wkid":102100}
  }),
  	map, incidentLayer, visibleIncidentLayer, selected, updateFeature, attInspector, layerInfos;
  
  parser.parse();
  
  esriConfig.defaults.io.proxyUrl = "./proxy/proxy.ashx";
  
  map = new Map("map", {
  	basemap: "osm",
  	extent: maxExtent
  });
  
  incidentLayer = new FeatureLayer("http://sampleserver3.arcgisonline.com/ArcGIS/rest/services/SanFrancisco/311Incidents/FeatureServer/0", {
    mode: FeatureLayer.MODE_SELECTION,
    outFields: ["req_type","req_date","req_time","address","district","status"],
    id: "incidentLayer"
  });
  
  visibleIncidentLayer = new ArcGISDynamicMapServiceLayer("http://sampleserver3.arcgisonline.com/ArcGIS/rest/services/SanFrancisco/311Incidents/MapServer");
  
  incidentLayer.on("edits-complete", function() {
    visibleIncidentLayer.refresh();
  });
  
  function startEditing () {
  	// add the Locate button
  	var locator = new LocateButton({map: map}, "locatebutton");
  	
  	var incidentLayer = map.getLayer("incidentLayer");
  	generateTemplatePicker(incidentLayer);
  	generateAttributeInspector(incidentLayer);
  	// add map click event to create the 
  	map.on("click", function(evt) {
  	  if (!maxExtent.contains(evt.mapPoint)) {
  	  	alert("Sorry, that point is outside our area of interest.");
  	  	return;
  	  }
      if (selected) {
        var currentDate = new Date();
        var incidentAttributes = {
          req_type: selected.template.name,
          req_date:(currentDate.getMonth() + 1) + "/" + currentDate.getDate() + "/" + currentDate.getFullYear(),
          req_time: currentDate.toLocaleTimeString(),
          address: "",
          district: "",
          status: 1
        };
        var incidentGraphic = new Graphic(evt.mapPoint, selected.symbol, incidentAttributes);
        incidentLayer.applyEdits([incidentGraphic],null,null).then(function () {
        	showInspector(evt);
        });
         
      } else {
      	showInspector(evt);
      }
    });
    
  	incidentLayer.setSelectionSymbol(
      new MarkerSymbol({color:[255,0,0]})
    );
  
    incidentLayer.on("click", showInspector);
    
    map.infoWindow.on("hide", function() {
      incidentLayer.clearSelection();
    });

  }
  
  function generateTemplatePicker(layer) {
  	console.log("layer", layer);
    var widget = new TemplatePicker({
      featureLayers: [ layer ],
      rows: layer.types.length,
      columns: 1,
      grouping: false,
      style: "width:98%;"
    }, "editordiv");

    widget.startup();

    widget.on("selection-change", function() {
      selected = widget.getSelected();
      console.log("selected", selected);
    });
  }
  
  function generateAttributeInspector(layer) {
  	
  	layerInfos = [{
  	  featureLayer: layer,
  	  showAttachments: true,
  	  isEditable: true,
  	  showDeleteButton: false,
  	  fieldInfos: [
  	    {'fieldName': 'req_type', 'isEditable':true, 'tooltip': 'What\'s wrong?', 'label':'Status:'},
        {'fieldName': 'req_date', 'isEditable':false, 'tooltip': 'Date incident was reported.', 'label':'Date:'},
        {'fieldName': 'req_time', 'isEditable':false,'label':'Time:'},
        {'fieldName': 'address', 'isEditable':true, 'label':'Address:'},
        {'fieldName': 'district', 'isEditable':true, 'label':'District:'},
        {'fieldName': 'status', 'isEditable':false, 'label':'Status:'}
  	  ]
  	}]; //"","req_date","req_time","address","district","status"
  	
  	attInspector = new AttributeInspector({
  		layerInfos: layerInfos
  	}, domConstruct.create("div", null, document.body));
  	
  	attInspector.startup();
  	
  	//add a save button next to the delete button
    var saveButton = new Button({ label: "Save", "class": "saveButton"});
    domConstruct.place(saveButton.domNode, attInspector.deleteBtn.domNode, "after");
         
    saveButton.on("click", function(){
      updateFeature.getLayer().applyEdits(null, [updateFeature], null);         
    });
        
    attInspector.on("attribute-change", function(evt) {
      //store the updates to apply when the save button is clicked
      updateFeature.attributes[evt.fieldName] = evt.fieldValue;
    });
          
    attInspector.on("next", function(evt) {
      updateFeature = evt.feature;
      console.log("Next " + updateFeature.attributes.objectid);
    });
          
    attInspector.on("delete", function(evt){
      evt.feature.getLayer().applyEdits(null,null,[updateFeature]);
      map.infoWindow.hide();
    });

	if (attInspector.domNode) {
      map.infoWindow.setContent(attInspector.domNode);
      map.infoWindow.resize(350, 240);

    }
	
  }
  
  function showInspector(evt) {
  	var selectQuery = new Query(),
  		point = evt.mapPoint,
  		mapScale = map.getScale();
  	  
      selectQuery.geometry = new Extent({
      	xmin: point.x - mapScale / 96,
      	xmax: point.x + mapScale / 96,
      	ymin: point.y - mapScale / 96,
      	ymax: point.y + mapScale / 96,
      	spatialReference: map.spatialReference
      });
      
      incidentLayer.selectFeatures(selectQuery, FeatureLayer.SELECTION_NEW, function (features) {
      	if (!features.length) {
      		map.infoWindow.hide();
      		return;
      	}
      	
      	updateFeature = features[0];
      	
      	map.infoWindow.setTitle(updateFeature.getLayer().name);
      	map.infoWindow.show(evt.screenPoint, map.getInfoWindowAnchor(evt.screenPoint));
      });
  }
  
  function onMapExtentChange (response) {
  	if (!response.extent.intersects(maxExtent)) {
  		// show these blocks if the map extent is outside the city extent
  		domStyle.set("blockerdiv", "display", "block");
  		domStyle.set("outsidemessage", "display", "block");
  	} else {
  		// hide these blocks if the max Extent is visible within the view.
  		domStyle.set("blockerdiv", "display", "none");
  		domStyle.set("outsidemessage", "display", "none");
  	}
  }
  
  dojoOn(dojoDom.byId("outsidemessage"), "click", function () {
  	map.setExtent(maxExtent);
  })
    
  // zoom in to our location if available.
  
  map.on("extent-change", onMapExtentChange);
  
  map.on("layers-add-result", startEditing);
  
  map.addLayers([visibleIncidentLayer, incidentLayer]);
  
  

});
